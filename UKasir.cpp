//---------------------------------------------------------------------------
// Langkah 3 - Kasir (Point of Sale) & Searching
//
// Alur di dalam form (GUI State Locking):
//   [1] Isi Keranjang  : pilih jenis layanan, cari obat, tambah ke keranjang
//   [2] Konfirmasi     : tagihan dihitung, form dikunci, tunggu Setuju / Batal
//   [3] Bayar          : input uang bayar, validasi Uang >= Tagihan
//   [4] Selesai        : struk tercetak, stok dipotong, transaksi dicatat
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UKasir.h"
#include "UDataApotek.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKasir *FormKasir;

// Keranjang pelanggan aktif
static ItemKeranjang keranjang[MAX_KERANJANG];
static int           jumlahKeranjang = 0;
//---------------------------------------------------------------------------
__fastcall TFormKasir::TFormKasir(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::FormCreate(TObject *Sender)
{
        GridKeranjang->Cells[0][0] = "No";
        GridKeranjang->Cells[1][0] = "Kode";
        GridKeranjang->Cells[2][0] = "Nama Obat";
        GridKeranjang->Cells[3][0] = "Harga";
        GridKeranjang->Cells[4][0] = "Jml";
        GridKeranjang->Cells[5][0] = "Subtotal";
        GridKeranjang->ColWidths[0] = 30;
        GridKeranjang->ColWidths[1] = 60;
        GridKeranjang->ColWidths[2] = 150;
        GridKeranjang->ColWidths[3] = 80;
        GridKeranjang->ColWidths[4] = 40;
        GridKeranjang->ColWidths[5] = 95;

        KosongkanKeranjang();
        ResetHasilCari();
        AturStateKasir(STATE_INPUT);
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::FormShow(TObject *Sender)
{
        // Transaksi sebelumnya sudah lunas: mulai pelanggan baru
        if (stateKasir == STATE_SELESAI)
                BtnTransaksiBaruClick(Sender);
        ActiveControl = EdtCariObat;
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::BtnKembaliClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        if (stateKasir == STATE_KONFIRMASI || stateKasir == STATE_BAYAR)
        {
                Pesan("Transaksi sedang berjalan.\nSelesaikan pembayaran atau tekan "
                      "Tidak Setuju / Batal terlebih dahulu.",
                      "Kasir", MB_OK | MB_ICONWARNING);
                CanClose = false;
        }
}
//---------------------------------------------------------------------------
// GUI State Locking: aktif/nonaktifkan kontrol sesuai tahap transaksi
void TFormKasir::AturStateKasir(int state)
{
        stateKasir = state;
        bool input = (state == STATE_INPUT);

        RgJenis->Enabled            = input;
        EdtBiayaRacik->Enabled      = input && RgJenis->ItemIndex == 0;
        EdtCariObat->Enabled        = input;
        BtnCari->Enabled            = input;
        CbBinarySearch->Enabled     = input;
        EdtJumlahBeli->Enabled      = input;
        BtnTambahKeranjang->Enabled = input;
        BtnHapusItem->Enabled       = input;
        BtnKonfirmasi->Enabled      = input;

        BtnSetuju->Enabled        = (state == STATE_KONFIRMASI);
        BtnBatal->Enabled         = (state == STATE_KONFIRMASI || state == STATE_BAYAR);
        EdtUangBayar->Enabled     = (state == STATE_BAYAR);
        BtnProsesBayar->Enabled   = (state == STATE_BAYAR);
        BtnTransaksiBaru->Enabled = (state == STATE_SELESAI);

        // Tidak boleh kembali ke menu saat transaksi belum selesai
        BtnKembali->Enabled = (state == STATE_INPUT || state == STATE_SELESAI);

        // Penunjuk tahap pada bar navigasi
        AnsiString tahap[4] = { "Isi Keranjang", "Konfirmasi", "Bayar", "Selesai" };
        AnsiString s = "Tahap:  ";
        for (int i = 0; i < 4; i++)
        {
                AnsiString t = IntToStr(i + 1) + " " + tahap[i];
                s += (i == state) ? "[ " + t + " ]" : t;
                if (i < 3) s += "  >  ";
        }
        LblTahap->Caption = s;
}
//---------------------------------------------------------------------------
void TFormKasir::ResetHasilCari()
{
        kodeDitemukan = "";
        LblHNama->Caption  = "Nama Obat : -";
        LblHHarga->Caption = "Harga     : -";
        LblHStok->Caption  = "Sisa Stok : -";
        LblMetodeCari->Caption = "";
}
//---------------------------------------------------------------------------
void TFormKasir::KosongkanKeranjang()
{
        jumlahKeranjang = 0;
        totalTagihan = 0;
        biayaRacik = 0;
        TampilkanKeranjang();
}
//---------------------------------------------------------------------------
void TFormKasir::TampilkanKeranjang()
{
        if (jumlahKeranjang == 0)
        {
                GridKeranjang->RowCount = 2;
                for (int c = 0; c < GridKeranjang->ColCount; c++)
                        GridKeranjang->Cells[c][1] = "";
        }
        else
        {
                GridKeranjang->RowCount = jumlahKeranjang + 1;
                for (int i = 0; i < jumlahKeranjang; i++)
                {
                        int r = i + 1;
                        GridKeranjang->Cells[0][r] = IntToStr(r);
                        GridKeranjang->Cells[1][r] = keranjang[i].kode;
                        GridKeranjang->Cells[2][r] = keranjang[i].nama;
                        GridKeranjang->Cells[3][r] = FormatRupiah(keranjang[i].harga);
                        GridKeranjang->Cells[4][r] = IntToStr(keranjang[i].jumlah);
                        GridKeranjang->Cells[5][r] = FormatRupiah(keranjang[i].subtotal);
                }
        }
        PerbaruiTotalRealtime();
}
//---------------------------------------------------------------------------
// Total tagihan sementara = jumlah subtotal keranjang + biaya racik
void TFormKasir::PerbaruiTotalRealtime()
{
        long total = 0;
        for (int i = 0; i < jumlahKeranjang; i++)
                total += keranjang[i].subtotal;

        long racik = 0;
        if (RgJenis->ItemIndex == 0 && BacaAngka(EdtBiayaRacik, racik) && racik > 0)
                total += racik;

        LblTotalTagihan->Caption = "Total Tagihan: " + FormatRupiah(total);
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::RgJenisClick(TObject *Sender)
{
        bool resep = (RgJenis->ItemIndex == 0);
        EdtBiayaRacik->Enabled = resep && stateKasir == STATE_INPUT;
        if (!resep) EdtBiayaRacik->Text = "0";
        PerbaruiTotalRealtime();
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::EdtBiayaRacikChange(TObject *Sender)
{
        PerbaruiTotalRealtime();
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::EdtCariObatKeyPress(TObject *Sender, char &Key)
{
        if (Key == VK_RETURN)
        {
                Key = 0;
                BtnCariClick(Sender);
        }
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::BtnCariClick(TObject *Sender)
{
        AnsiString kunci = Trim(EdtCariObat->Text);
        if (kunci == "")
        {
                Pesan("Ketik nama atau kode obat yang dicari.", "Pencarian",
                      MB_OK | MB_ICONWARNING);
                EdtCariObat->SetFocus();
                return;
        }

        int idx, langkah;
        AnsiString metode;
        if (CbBinarySearch->Checked && dataUrutNama)
        {
                idx = BinarySearchNama(kunci, langkah);
                metode = "Binary Search";
        }
        else
        {
                if (CbBinarySearch->Checked)
                        Pesan("Binary Search membutuhkan data terurut berdasarkan Nama\n"
                              "(Ascending atau Descending).\n"
                              "Urutkan dulu di menu Pengadaan. Pencarian dialihkan "
                              "ke Sequential Search.", "Info", MB_OK | MB_ICONINFORMATION);
                idx = SequentialSearch(kunci, langkah);
                metode = "Sequential Search";
        }

        ResetHasilCari();
        LblMetodeCari->Caption = metode + ": " + IntToStr(langkah) + " kali perbandingan.";

        if (idx == -1)
        {
                Pesan("Obat \"" + kunci + "\" tidak ditemukan / tidak tersedia.",
                      "Obat Tidak Ditemukan", MB_OK | MB_ICONWARNING);
                return;
        }

        LblHNama->Caption  = "Nama Obat : " + dataObat[idx].nama + " (" + dataObat[idx].kode + ")";
        LblHHarga->Caption = "Harga     : " + FormatRupiah(dataObat[idx].harga);
        LblHStok->Caption  = "Sisa Stok : " + IntToStr(dataObat[idx].stok);

        if (dataObat[idx].stok <= 0)
        {
                Pesan("Stok " + dataObat[idx].nama + " HABIS.\nSilakan tawarkan obat lain.",
                      "Stok Habis", MB_OK | MB_ICONWARNING);
                return;
        }

        kodeDitemukan = dataObat[idx].kode;
        EdtJumlahBeli->Text = "1";
        EdtJumlahBeli->SetFocus();
        EdtJumlahBeli->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::BtnTambahKeranjangClick(TObject *Sender)
{
        if (kodeDitemukan == "")
        {
                Pesan("Cari obat terlebih dahulu.", "Keranjang", MB_OK | MB_ICONWARNING);
                return;
        }
        int idx = CariKode(kodeDitemukan);
        if (idx == -1)
        {
                ResetHasilCari();
                return;
        }

        long jumlah;
        if (!BacaAngka(EdtJumlahBeli, jumlah) || jumlah <= 0)
        {
                Pesan("Jumlah beli harus berupa angka lebih dari 0.",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtJumlahBeli->SetFocus();
                return;
        }

        // Cek apakah obat sudah ada di keranjang
        int posisi = -1;
        for (int i = 0; i < jumlahKeranjang; i++)
                if (keranjang[i].kode == kodeDitemukan)
                        posisi = i;

        int sudahDiKeranjang = (posisi >= 0) ? keranjang[posisi].jumlah : 0;
        if (sudahDiKeranjang + jumlah > dataObat[idx].stok)
        {
                Pesan("Stok gudang tidak cukup!\nStok tersedia: " +
                      IntToStr(dataObat[idx].stok) + ", sudah di keranjang: " +
                      IntToStr(sudahDiKeranjang) + ".",
                      "Stok Tidak Cukup", MB_OK | MB_ICONERROR);
                return;
        }

        if (posisi >= 0)
        {
                keranjang[posisi].jumlah  += jumlah;
                keranjang[posisi].subtotal = keranjang[posisi].jumlah * keranjang[posisi].harga;
        }
        else
        {
                if (jumlahKeranjang >= MAX_KERANJANG)
                {
                        Pesan("Keranjang penuh.", "Keranjang", MB_OK | MB_ICONERROR);
                        return;
                }
                keranjang[jumlahKeranjang].kode     = dataObat[idx].kode;
                keranjang[jumlahKeranjang].nama     = dataObat[idx].nama;
                keranjang[jumlahKeranjang].harga    = dataObat[idx].harga;
                keranjang[jumlahKeranjang].jumlah   = jumlah;
                keranjang[jumlahKeranjang].subtotal = jumlah * dataObat[idx].harga;
                jumlahKeranjang++;
        }

        TampilkanKeranjang();
        ResetHasilCari();
        EdtJumlahBeli->Text = "";
        EdtCariObat->Text = "";
        EdtCariObat->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::BtnHapusItemClick(TObject *Sender)
{
        int i = GridKeranjang->Row - 1;
        if (i < 0 || i >= jumlahKeranjang)
        {
                Pesan("Pilih item keranjang yang akan dihapus.", "Keranjang",
                      MB_OK | MB_ICONWARNING);
                return;
        }
        for (int j = i; j < jumlahKeranjang - 1; j++)
                keranjang[j] = keranjang[j + 1];
        jumlahKeranjang--;
        TampilkanKeranjang();
}
//---------------------------------------------------------------------------
// Hitung tagihan (Harga x Jumlah + BiayaRacik) lalu kunci form input
void __fastcall TFormKasir::BtnKonfirmasiClick(TObject *Sender)
{
        if (jumlahKeranjang == 0)
        {
                Pesan("Keranjang masih kosong.", "Konfirmasi", MB_OK | MB_ICONWARNING);
                return;
        }

        biayaRacik = 0;
        if (RgJenis->ItemIndex == 0)
        {
                if (!BacaAngka(EdtBiayaRacik, biayaRacik) || biayaRacik < 0)
                {
                        Pesan("Biaya racik harus berupa angka (minimal 0).",
                              "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                        EdtBiayaRacik->SetFocus();
                        return;
                }
        }

        totalTagihan = biayaRacik;
        for (int i = 0; i < jumlahKeranjang; i++)
                totalTagihan += keranjang[i].subtotal;

        EdtTotalTagihan->Text = FormatFloat("#,##0", totalTagihan);
        LblTotalTagihan->Caption = "Total Tagihan: " + FormatRupiah(totalTagihan);
        AturStateKasir(STATE_KONFIRMASI);
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::BtnSetujuClick(TObject *Sender)
{
        AturStateKasir(STATE_BAYAR);
        EdtUangBayar->Text = "";
        EdtUangBayar->SetFocus();
}
//---------------------------------------------------------------------------
// Pelanggan tidak setuju: batalkan, stok tidak dipotong
void __fastcall TFormKasir::BtnBatalClick(TObject *Sender)
{
        if (Application->MessageBox("Batalkan transaksi ini?\n"
                "Obat dikembalikan ke etalase dan stok tidak dipotong.",
                "Batal Transaksi", MB_YESNO | MB_ICONQUESTION) != IDYES)
                return;

        KosongkanKeranjang();
        ResetHasilCari();
        EdtTotalTagihan->Text = "";
        EdtUangBayar->Text = "";
        LblKembalian->Caption = "Kembalian: Rp 0";
        AturStateKasir(STATE_INPUT);
        EdtCariObat->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormKasir::BtnProsesBayarClick(TObject *Sender)
{
        long uangBayar;
        if (!BacaAngka(EdtUangBayar, uangBayar) || uangBayar < 0)
        {
                Pesan("Masukkan nominal uang bayar berupa angka.",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtUangBayar->SetFocus();
                return;
        }
        if (uangBayar < totalTagihan)
        {
                Pesan("Uang kurang " + FormatRupiah(totalTagihan - uangBayar) +
                      ".\nMinta pelanggan menambahkan uang bayar.",
                      "Uang Kurang", MB_OK | MB_ICONERROR);
                EdtUangBayar->SetFocus();
                EdtUangBayar->SelectAll();
                return;
        }

        int barisBaru = jumlahKeranjang + (biayaRacik > 0 ? 1 : 0);
        if (jumlahRiwayat + barisBaru > MAX_TRANSAKSI)
        {
                Pesan("Buku transaksi penuh. Lakukan tutup toko.", "Gagal",
                      MB_OK | MB_ICONERROR);
                return;
        }

        // Validasi ulang stok sebelum dipotong
        for (int i = 0; i < jumlahKeranjang; i++)
        {
                int idx = CariKode(keranjang[i].kode);
                if (idx == -1 || dataObat[idx].stok < keranjang[i].jumlah)
                {
                        Pesan("Stok " + keranjang[i].nama + " tidak cukup.",
                              "Stok Tidak Cukup", MB_OK | MB_ICONERROR);
                        return;
                }
        }

        long kembalian = uangBayar - totalTagihan;
        nomorTransaksi++;
        TDateTime sekarang = Now();
        AnsiString jenis = (RgJenis->ItemIndex == 0) ? "Resep" : "Bebas";

        // Potong stok gudang & catat ke buku transaksi
        for (int i = 0; i < jumlahKeranjang; i++)
        {
                int idx = CariKode(keranjang[i].kode);
                dataObat[idx].stok -= keranjang[i].jumlah;

                riwayat[jumlahRiwayat].noTrans  = nomorTransaksi;
                riwayat[jumlahRiwayat].waktu    = sekarang;
                riwayat[jumlahRiwayat].jenis    = jenis;
                riwayat[jumlahRiwayat].namaObat = keranjang[i].nama;
                riwayat[jumlahRiwayat].jumlah   = keranjang[i].jumlah;
                riwayat[jumlahRiwayat].harga    = keranjang[i].harga;
                riwayat[jumlahRiwayat].subtotal = keranjang[i].subtotal;
                jumlahRiwayat++;
        }
        if (biayaRacik > 0)
        {
                riwayat[jumlahRiwayat].noTrans  = nomorTransaksi;
                riwayat[jumlahRiwayat].waktu    = sekarang;
                riwayat[jumlahRiwayat].jenis    = jenis;
                riwayat[jumlahRiwayat].namaObat = "Biaya Racik";
                riwayat[jumlahRiwayat].jumlah   = 1;
                riwayat[jumlahRiwayat].harga    = biayaRacik;
                riwayat[jumlahRiwayat].subtotal = biayaRacik;
                jumlahRiwayat++;
        }

        CetakStruk(nomorTransaksi, uangBayar, kembalian);
        LblKembalian->Caption = "Kembalian: " + FormatRupiah(kembalian);
        EdtUangBayar->Text = FormatFloat("#,##0", uangBayar);

        AturStateKasir(STATE_SELESAI);
        BtnTransaksiBaru->SetFocus();
}
//---------------------------------------------------------------------------
void TFormKasir::CetakStruk(int noTrans, long uangBayar, long kembalian)
{
        const int L = 40;
        AnsiString garis = AnsiString::StringOfChar('=', L);
        AnsiString strip = AnsiString::StringOfChar('-', L);
        TStrings *s = MemoStruk->Lines;

        s->Clear();
        s->Add(garis);
        s->Add("             APOTEK SALWA");
        s->Add("     Struk Pembayaran Obat (Tunai)");
        s->Add(garis);
        s->Add("No. Nota : TRX-" + FormatFloat("0000", noTrans));
        s->Add("Tanggal  : " + FormatDateTime("dd/mm/yyyy hh:nn", Now()));
        s->Add("Layanan  : " + AnsiString(RgJenis->ItemIndex == 0 ? "Resep Dokter" : "Beli Langsung"));
        s->Add(strip);
        for (int i = 0; i < jumlahKeranjang; i++)
        {
                s->Add(keranjang[i].nama);
                s->Add(Baris("   " + IntToStr(keranjang[i].jumlah) + " x " +
                             FormatRupiah(keranjang[i].harga),
                             FormatRupiah(keranjang[i].subtotal), L));
        }
        s->Add(strip);
        if (biayaRacik > 0)
                s->Add(Baris("Biaya Racik", FormatRupiah(biayaRacik), L));
        s->Add(Baris("TOTAL",   FormatRupiah(totalTagihan), L));
        s->Add(Baris("Bayar",   FormatRupiah(uangBayar), L));
        s->Add(Baris("Kembali", FormatRupiah(kembalian), L));
        s->Add(garis);
        s->Add("              *** LUNAS ***");
        s->Add("   Terima kasih, semoga lekas sembuh");
        s->Add(garis);
}
//---------------------------------------------------------------------------
// Ada pelanggan lain: kembali ke awal alur pelayanan
void __fastcall TFormKasir::BtnTransaksiBaruClick(TObject *Sender)
{
        KosongkanKeranjang();
        ResetHasilCari();
        MemoStruk->Lines->Clear();
        EdtTotalTagihan->Text = "";
        EdtUangBayar->Text = "";
        EdtBiayaRacik->Text = "0";
        EdtCariObat->Text = "";
        LblKembalian->Caption = "Kembalian: Rp 0";
        RgJenis->ItemIndex = 1;
        AturStateKasir(STATE_INPUT);
        ActiveControl = EdtCariObat;
}
//---------------------------------------------------------------------------
