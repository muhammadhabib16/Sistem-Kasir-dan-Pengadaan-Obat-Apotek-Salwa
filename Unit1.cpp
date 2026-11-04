//---------------------------------------------------------------------------
// Sistem Informasi Kasir dan Pengadaan Obat - Apotek Salwa
// Borland C++ Builder 6 (VCL GUI)
//
// Modul:
//   1. Buka Toko      : input kas awal (saldoKasAwal)
//   2. Pengadaan Obat : input pasokan + Selection Sort / Bubble Sort
//   3. Kasir (POS)    : Sequential Search / Binary Search, keranjang,
//                       konfirmasi, pembayaran, struk (GUI State Locking)
//   4. Tutup Toko     : buku transaksi, rekap kas, rekonsiliasi
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//===========================================================================
// DATA GLOBAL (Array of Struct)
//===========================================================================
Obat          dataObat[MAX_OBAT];           // inventaris gudang
int           jumlahObat = 0;

ItemKeranjang keranjang[MAX_KERANJANG];     // keranjang pelanggan aktif
int           jumlahKeranjang = 0;

Transaksi     riwayat[MAX_TRANSAKSI];       // buku transaksi harian
int           jumlahRiwayat = 0;
int           nomorTransaksi = 0;           // jumlah transaksi lunas

long saldoKasAwal    = 0;
bool tokoDibuka      = false;
bool tokoDitutup     = false;
bool dataUrutNama    = false;   // true jika array terurut nama A-Z (syarat Binary Search)
AnsiString kodeDitemukan = "";  // kode obat hasil pencarian terakhir
int  stateKasir      = STATE_INPUT;
long biayaRacik      = 0;       // biaya racik transaksi aktif
long totalTagihan    = 0;
bool rekapSudah      = false;
long totalPenjualan  = 0;
long totalUangSistem = 0;
bool cocokSudah      = false;
long uangFisik       = 0;

//===========================================================================
// FUNGSI BANTU
//===========================================================================
AnsiString FormatRupiah(long nilai)
{
        return "Rp " + FormatFloat("#,##0", nilai);
}
//---------------------------------------------------------------------------
// Membaca bilangan bulat dari TEdit. Titik/koma pemisah ribuan diabaikan.
bool BacaAngka(TEdit *edt, long &hasil)
{
        AnsiString s = Trim(edt->Text);
        s = StringReplace(s, ".", "", TReplaceFlags() << rfReplaceAll);
        s = StringReplace(s, ",", "", TReplaceFlags() << rfReplaceAll);
        if (s == "") return false;
        try
        {
                hasil = StrToInt(s);
        }
        catch (EConvertError &)
        {
                return false;
        }
        return true;
}
//---------------------------------------------------------------------------
void Pesan(AnsiString teks, AnsiString judul, int flags)
{
        Application->MessageBox(teks.c_str(), judul.c_str(), flags);
}
//---------------------------------------------------------------------------
AnsiString PadKanan(AnsiString s, int lebar)
{
        if (s.Length() > lebar) s = s.SubString(1, lebar);
        while (s.Length() < lebar) s += " ";
        return s;
}
//---------------------------------------------------------------------------
AnsiString PadKiri(AnsiString s, int lebar)
{
        while (s.Length() < lebar) s = " " + s;
        return s;
}
//---------------------------------------------------------------------------
// Satu baris teks rata kiri-kanan, misal "TOTAL ........ Rp 10.000"
AnsiString Baris(AnsiString kiri, AnsiString kanan, int lebar)
{
        return PadKanan(kiri, lebar - kanan.Length()) + kanan;
}
//---------------------------------------------------------------------------
void TambahObat(AnsiString kode, AnsiString nama, AnsiString kategori,
                long harga, int stok)
{
        if (jumlahObat >= MAX_OBAT) return;
        dataObat[jumlahObat].kode     = kode;
        dataObat[jumlahObat].nama     = nama;
        dataObat[jumlahObat].kategori = kategori;
        dataObat[jumlahObat].harga    = harga;
        dataObat[jumlahObat].stok     = stok;
        jumlahObat++;
}

//===========================================================================
// ALGORITMA PENCARIAN (SEARCHING)
//===========================================================================
// Pencarian persis berdasarkan kode obat (dipakai internal).
int CariKode(AnsiString kode)
{
        for (int i = 0; i < jumlahObat; i++)
                if (AnsiCompareText(dataObat[i].kode, Trim(kode)) == 0)
                        return i;
        return -1;
}
//---------------------------------------------------------------------------
// Sequential Search: cocok jika kode sama persis ATAU nama mengandung
// kata kunci (substring). Data tidak perlu terurut.
int SequentialSearch(AnsiString kunci, int &langkah)
{
        kunci = AnsiLowerCase(Trim(kunci));
        langkah = 0;
        for (int i = 0; i < jumlahObat; i++)
        {
                langkah++;
                if (AnsiLowerCase(dataObat[i].kode) == kunci ||
                    AnsiLowerCase(dataObat[i].nama).Pos(kunci) > 0)
                        return i;
        }
        return -1;
}
//---------------------------------------------------------------------------
// Binary Search: nama harus sama persis (tidak case-sensitive) dan
// array wajib sudah terurut berdasarkan nama A-Z.
int BinarySearchNama(AnsiString kunci, int &langkah)
{
        int kiri = 0, kanan = jumlahObat - 1;
        kunci = Trim(kunci);
        langkah = 0;
        while (kiri <= kanan)
        {
                int tengah = (kiri + kanan) / 2;
                int banding = AnsiCompareText(dataObat[tengah].nama, kunci);
                langkah++;
                if (banding == 0)
                        return tengah;
                else if (banding < 0)
                        kiri = tengah + 1;
                else
                        kanan = tengah - 1;
        }
        return -1;
}

//===========================================================================
// ALGORITMA PENGURUTAN (SORTING)
//===========================================================================
// true jika a seharusnya berada SETELAH b menurut kriteria
bool HarusDitukar(const Obat &a, const Obat &b, int kriteria)
{
        if (kriteria == URUT_STOK)
                return a.stok > b.stok;
        return AnsiCompareText(a.nama, b.nama) > 0;
}
//---------------------------------------------------------------------------
void TukarObat(Obat &a, Obat &b)
{
        Obat temp = a;
        a = b;
        b = temp;
}
//---------------------------------------------------------------------------
// Selection Sort: cari elemen minimum lalu tukar ke posisi i.
// Mengembalikan jumlah pertukaran yang terjadi.
int SelectionSort(int kriteria)
{
        int jumlahTukar = 0;
        for (int i = 0; i < jumlahObat - 1; i++)
        {
                int idxMin = i;
                for (int j = i + 1; j < jumlahObat; j++)
                        if (HarusDitukar(dataObat[idxMin], dataObat[j], kriteria))
                                idxMin = j;
                if (idxMin != i)
                {
                        TukarObat(dataObat[i], dataObat[idxMin]);
                        jumlahTukar++;
                }
        }
        return jumlahTukar;
}
//---------------------------------------------------------------------------
// Bubble Sort: tukar elemen bertetangga; berhenti lebih awal jika
// tidak ada pertukaran pada satu putaran.
int BubbleSort(int kriteria)
{
        int jumlahTukar = 0;
        for (int i = 0; i < jumlahObat - 1; i++)
        {
                bool adaTukar = false;
                for (int j = 0; j < jumlahObat - 1 - i; j++)
                {
                        if (HarusDitukar(dataObat[j], dataObat[j + 1], kriteria))
                        {
                                TukarObat(dataObat[j], dataObat[j + 1]);
                                jumlahTukar++;
                                adaTukar = true;
                        }
                }
                if (!adaTukar) break;
        }
        return jumlahTukar;
}

//===========================================================================
// FORM
//===========================================================================
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
        // Header tabel stok
        GridStok->Cells[0][0] = "No";
        GridStok->Cells[1][0] = "Kode";
        GridStok->Cells[2][0] = "Nama Obat";
        GridStok->Cells[3][0] = "Kategori";
        GridStok->Cells[4][0] = "Harga";
        GridStok->Cells[5][0] = "Stok";
        GridStok->Cells[6][0] = "Status";
        GridStok->ColWidths[0] = 35;
        GridStok->ColWidths[1] = 70;
        GridStok->ColWidths[2] = 210;
        GridStok->ColWidths[3] = 100;
        GridStok->ColWidths[4] = 100;
        GridStok->ColWidths[5] = 60;
        GridStok->ColWidths[6] = 80;

        // Header tabel keranjang
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

        CmbKategori->Items->Add("Tablet");
        CmbKategori->Items->Add("Kapsul");
        CmbKategori->Items->Add("Sirup");
        CmbKategori->Items->Add("Salep");
        CmbKategori->Items->Add("Tetes");
        CmbKategori->Items->Add("Injeksi");
        CmbKategori->Items->Add("Alat Kesehatan");
        CmbKategori->Items->Add("Lainnya");
        CmbKategori->ItemIndex = 0;

        CmbKriteria->Items->Add("Stok Terkecil (Ascending)");
        CmbKriteria->Items->Add("Nama Obat (A-Z)");
        CmbKriteria->ItemIndex = 0;

        // Data awal inventaris (contoh)
        TambahObat("OBT001", "Paracetamol 500mg",      "Tablet",  5000, 50);
        TambahObat("OBT002", "Amoxicillin 500mg",      "Kapsul", 12000,  8);
        TambahObat("OBT003", "OBH Combi 100ml",        "Sirup",  18000, 15);
        TambahObat("OBT004", "Betadine Salep 10g",     "Salep",  15000,  5);
        TambahObat("OBT005", "Promag Tablet",          "Tablet",  8000, 30);
        TambahObat("OBT006", "Vitamin C 1000mg",       "Tablet", 20000, 25);
        TambahObat("OBT007", "Insto Tetes Mata",       "Tetes",  14000,  3);
        TambahObat("OBT008", "Antangin JRG",           "Sirup",   4000, 40);
        TambahObat("OBT009", "Ambroxol 30mg",          "Tablet",  6000,  0);
        TambahObat("OBT010", "Minyak Kayu Putih 60ml", "Lainnya",22000, 12);
        TampilkanGridStok();

        // Semua menu terkunci sampai toko dibuka
        TabPengadaan->TabVisible = false;
        TabKasir->TabVisible     = false;
        TabTutup->TabVisible     = false;
        PageControl1->ActivePage = TabKasAwal;

        KosongkanKeranjang();
        ResetHasilCari();
        AturStateKasir(STATE_INPUT);
        TampilkanBukuTransaksi();
        ResetRekap();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        if (tokoDibuka && !tokoDitutup)
        {
                CanClose = Application->MessageBox(
                        "Toko belum ditutup melalui menu Tutup Toko.\n"
                        "Data transaksi hari ini akan hilang. Tetap keluar?",
                        "Konfirmasi Keluar", MB_YESNO | MB_ICONWARNING) == IDYES;
        }
}

//===========================================================================
// TAHAP 1 - BUKA TOKO (KAS AWAL)
//===========================================================================
void __fastcall TForm1::BtnBukaTokoClick(TObject *Sender)
{
        long kas;
        if (!BacaAngka(EdtKasAwal, kas) || kas < 0)
        {
                Pesan("Masukkan nominal kas awal berupa angka (minimal 0).",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtKasAwal->SetFocus();
                return;
        }

        saldoKasAwal = kas;
        tokoDibuka   = true;

        EdtKasAwal->Text    = FormatFloat("#,##0", saldoKasAwal);
        EdtKasAwal->Enabled = false;
        BtnBukaToko->Enabled = false;
        LblStatusToko->Caption = "Status: TOKO BUKA sejak " +
                FormatDateTime("dd/mm/yyyy hh:nn", Now()) +
                "  |  Kas Awal " + FormatRupiah(saldoKasAwal);
        LblStatusToko->Font->Color = clGreen;

        // Buka kunci menu Pengadaan, Kasir, Tutup Toko
        TabPengadaan->TabVisible = true;
        TabKasir->TabVisible     = true;
        TabTutup->TabVisible     = true;
        PageControl1->ActivePage = TabKasir;
        ResetRekap();
        EdtCariObat->SetFocus();
}

//===========================================================================
// TAHAP 2 - PENGADAAN OBAT & SORTING
//===========================================================================
void TForm1::TampilkanGridStok()
{
        if (jumlahObat == 0)
        {
                GridStok->RowCount = 2;
                for (int c = 0; c < GridStok->ColCount; c++)
                        GridStok->Cells[c][1] = "";
                return;
        }

        GridStok->RowCount = jumlahObat + 1;
        for (int i = 0; i < jumlahObat; i++)
        {
                int r = i + 1;
                GridStok->Cells[0][r] = IntToStr(r);
                GridStok->Cells[1][r] = dataObat[i].kode;
                GridStok->Cells[2][r] = dataObat[i].nama;
                GridStok->Cells[3][r] = dataObat[i].kategori;
                GridStok->Cells[4][r] = FormatRupiah(dataObat[i].harga);
                GridStok->Cells[5][r] = IntToStr(dataObat[i].stok);
                if (dataObat[i].stok <= 0)
                        GridStok->Cells[6][r] = "HABIS";
                else if (dataObat[i].stok <= BATAS_KRITIS)
                        GridStok->Cells[6][r] = "KRITIS";
                else
                        GridStok->Cells[6][r] = "Aman";
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GridStokDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
        // Warnai baris obat yang stoknya habis / kritis
        if (ARow == 0) return;
        AnsiString status = GridStok->Cells[6][ARow];
        if (status != "KRITIS" && status != "HABIS") return;

        TCanvas *cv = GridStok->Canvas;
        if (State.Contains(gdSelected))
        {
                cv->Brush->Color = clMaroon;
                cv->Font->Color  = clWhite;
        }
        else
        {
                cv->Brush->Color = (status == "HABIS") ? TColor(0x009999FF)
                                                       : TColor(0x00CCE6FF);
                cv->Font->Color  = clBlack;
        }
        cv->FillRect(Rect);
        cv->TextRect(Rect, Rect.Left + 2, Rect.Top + 3, GridStok->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GridStokClick(TObject *Sender)
{
        // Isi form pengadaan dari baris yang dipilih (untuk restock cepat)
        int i = GridStok->Row - 1;
        if (i < 0 || i >= jumlahObat) return;

        EdtKode->Text     = dataObat[i].kode;
        EdtNamaObat->Text = dataObat[i].nama;
        EdtHarga->Text    = IntToStr(dataObat[i].harga);
        EdtStok->Text     = "";
        int k = CmbKategori->Items->IndexOf(dataObat[i].kategori);
        CmbKategori->ItemIndex = (k >= 0) ? k : CmbKategori->Items->Count - 1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BtnSimpanStokClick(TObject *Sender)
{
        AnsiString kode = Trim(EdtKode->Text).UpperCase();
        AnsiString nama = Trim(EdtNamaObat->Text);
        long harga, jumlah;

        if (kode == "" || nama == "")
        {
                Pesan("Kode dan Nama Obat wajib diisi.", "Input Tidak Valid",
                      MB_OK | MB_ICONWARNING);
                return;
        }
        if (CmbKategori->ItemIndex < 0)
        {
                Pesan("Pilih kategori obat.", "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                return;
        }
        if (!BacaAngka(EdtHarga, harga) || harga <= 0)
        {
                Pesan("Harga satuan harus berupa angka lebih dari 0.",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtHarga->SetFocus();
                return;
        }
        if (!BacaAngka(EdtStok, jumlah) || jumlah <= 0)
        {
                Pesan("Jumlah pasokan harus berupa angka lebih dari 0.",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtStok->SetFocus();
                return;
        }

        int idx = CariKode(kode);
        if (idx >= 0)
        {
                // Kode sudah ada: tambah stok & perbarui harga
                dataObat[idx].stok  += jumlah;
                dataObat[idx].harga  = harga;
                Pesan("Stok " + dataObat[idx].nama + " bertambah " + IntToStr(jumlah) +
                      ".\nStok sekarang: " + IntToStr(dataObat[idx].stok),
                      "Pengadaan Berhasil", MB_OK | MB_ICONINFORMATION);
        }
        else
        {
                if (jumlahObat >= MAX_OBAT)
                {
                        Pesan("Kapasitas data obat penuh (maks " + IntToStr(MAX_OBAT) + ").",
                              "Gagal", MB_OK | MB_ICONERROR);
                        return;
                }
                TambahObat(kode, nama, CmbKategori->Text, harga, jumlah);
                dataUrutNama = false;   // data baru ditambahkan di akhir array
                LblHasilUrut->Caption = "Data berubah, belum diurutkan ulang.";
                Pesan("Obat baru " + nama + " berhasil didaftarkan.",
                      "Pengadaan Berhasil", MB_OK | MB_ICONINFORMATION);
        }

        TampilkanGridStok();
        BtnBersihObatClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BtnBersihObatClick(TObject *Sender)
{
        EdtKode->Text     = "";
        EdtNamaObat->Text = "";
        EdtHarga->Text    = "";
        EdtStok->Text     = "";
        CmbKategori->ItemIndex = 0;
        EdtKode->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BtnUrutkanClick(TObject *Sender)
{
        if (jumlahObat < 2)
        {
                Pesan("Data obat kurang dari 2, tidak perlu diurutkan.", "Info",
                      MB_OK | MB_ICONINFORMATION);
                return;
        }

        int kriteria = CmbKriteria->ItemIndex;
        int jumlahTukar;
        AnsiString algoritma;
        if (RgAlgoritma->ItemIndex == 0)
        {
                jumlahTukar = SelectionSort(kriteria);
                algoritma = "Selection Sort";
        }
        else
        {
                jumlahTukar = BubbleSort(kriteria);
                algoritma = "Bubble Sort";
        }

        dataUrutNama = (kriteria == URUT_NAMA);
        TampilkanGridStok();
        LblHasilUrut->Caption = "Diurutkan dengan " + algoritma + " berdasarkan " +
                CmbKriteria->Text + ". Jumlah pertukaran: " + IntToStr(jumlahTukar) + ".";
}

//===========================================================================
// TAHAP 3 - KASIR (POS) & SEARCHING
//===========================================================================
// GUI State Locking: aktif/nonaktifkan kontrol sesuai tahap transaksi
void TForm1::AturStateKasir(int state)
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

        BtnSetuju->Enabled      = (state == STATE_KONFIRMASI);
        BtnBatal->Enabled       = (state == STATE_KONFIRMASI || state == STATE_BAYAR);
        EdtUangBayar->Enabled   = (state == STATE_BAYAR);
        BtnProsesBayar->Enabled = (state == STATE_BAYAR);
        BtnTransaksiBaru->Enabled = (state == STATE_SELESAI);
}
//---------------------------------------------------------------------------
void TForm1::ResetHasilCari()
{
        kodeDitemukan = "";
        LblHNama->Caption  = "Nama Obat : -";
        LblHHarga->Caption = "Harga     : -";
        LblHStok->Caption  = "Sisa Stok : -";
        LblMetodeCari->Caption = "";
}
//---------------------------------------------------------------------------
void TForm1::KosongkanKeranjang()
{
        jumlahKeranjang = 0;
        totalTagihan = 0;
        biayaRacik = 0;
        TampilkanKeranjang();
}
//---------------------------------------------------------------------------
void TForm1::TampilkanKeranjang()
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
void TForm1::PerbaruiTotalRealtime()
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
void __fastcall TForm1::RgJenisClick(TObject *Sender)
{
        bool resep = (RgJenis->ItemIndex == 0);
        EdtBiayaRacik->Enabled = resep && stateKasir == STATE_INPUT;
        if (!resep) EdtBiayaRacik->Text = "0";
        PerbaruiTotalRealtime();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EdtBiayaRacikChange(TObject *Sender)
{
        PerbaruiTotalRealtime();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EdtCariObatKeyPress(TObject *Sender, char &Key)
{
        if (Key == VK_RETURN)
        {
                Key = 0;
                BtnCariClick(Sender);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BtnCariClick(TObject *Sender)
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
                        Pesan("Binary Search membutuhkan data terurut Nama (A-Z).\n"
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
void __fastcall TForm1::BtnTambahKeranjangClick(TObject *Sender)
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
void __fastcall TForm1::BtnHapusItemClick(TObject *Sender)
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
void __fastcall TForm1::BtnKonfirmasiClick(TObject *Sender)
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
void __fastcall TForm1::BtnSetujuClick(TObject *Sender)
{
        AturStateKasir(STATE_BAYAR);
        EdtUangBayar->Text = "";
        EdtUangBayar->SetFocus();
}
//---------------------------------------------------------------------------
// Pelanggan tidak setuju: batalkan, stok tidak dipotong
void __fastcall TForm1::BtnBatalClick(TObject *Sender)
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
void __fastcall TForm1::BtnProsesBayarClick(TObject *Sender)
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

        TampilkanGridStok();
        TampilkanBukuTransaksi();
        ResetRekap();               // ada transaksi baru, rekap harus dihitung ulang
        AturStateKasir(STATE_SELESAI);
        BtnTransaksiBaru->SetFocus();
}
//---------------------------------------------------------------------------
void TForm1::CetakStruk(int noTrans, long uangBayar, long kembalian)
{
        const int L = 40;
        AnsiString garis  = AnsiString::StringOfChar('=', L);
        AnsiString strip  = AnsiString::StringOfChar('-', L);
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
void __fastcall TForm1::BtnTransaksiBaruClick(TObject *Sender)
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
        EdtCariObat->SetFocus();
}

//===========================================================================
// TAHAP 4 - TUTUP TOKO & REKONSILIASI
//===========================================================================
void TForm1::TampilkanBukuTransaksi()
{
        TStrings *s = MemoBukuTransaksi->Lines;
        s->BeginUpdate();
        s->Clear();
        s->Add("BUKU TRANSAKSI PENJUALAN HARIAN - APOTEK SALWA");
        s->Add(AnsiString::StringOfChar('=', 74));
        s->Add(PadKanan("Nota", 9) + PadKanan("Jam", 6) + PadKanan("Jenis", 7) +
               PadKanan("Nama Obat", 24) + PadKiri("Qty", 4) +
               PadKiri("Harga", 11) + PadKiri("Total", 13));
        s->Add(AnsiString::StringOfChar('-', 74));

        long total = 0;
        for (int i = 0; i < jumlahRiwayat; i++)
        {
                s->Add(PadKanan("TRX-" + FormatFloat("0000", riwayat[i].noTrans), 9) +
                       PadKanan(FormatDateTime("hh:nn", riwayat[i].waktu), 6) +
                       PadKanan(riwayat[i].jenis, 7) +
                       PadKanan(riwayat[i].namaObat, 24) +
                       PadKiri(IntToStr(riwayat[i].jumlah), 4) +
                       PadKiri(FormatFloat("#,##0", riwayat[i].harga), 11) +
                       PadKiri(FormatFloat("#,##0", riwayat[i].subtotal), 13));
                total += riwayat[i].subtotal;
        }
        if (jumlahRiwayat == 0)
                s->Add("(belum ada transaksi)");
        s->Add(AnsiString::StringOfChar('-', 74));
        s->Add(Baris("TOTAL PENJUALAN (" + IntToStr(nomorTransaksi) + " transaksi)",
                     FormatRupiah(total), 74));
        s->EndUpdate();
}
//---------------------------------------------------------------------------
void TForm1::ResetRekap()
{
        rekapSudah = false;
        cocokSudah = false;
        LblRKasAwal->Caption        = "Kas Awal              : " + FormatRupiah(saldoKasAwal);
        LblRJmlTransaksi->Caption   = "Jumlah Transaksi      : -";
        LblRTotalPenjualan->Caption = "Total Penjualan       : -";
        LblTotalUangSistem->Caption = "Total Uang Sistem: -";
        LblHasilCocok->Caption = "Tekan Rekap Malam terlebih dahulu.";
        LblHasilCocok->Font->Color = clGray;
        EdtUangFisik->Enabled = false;
        BtnCocokkan->Enabled  = false;
}
//---------------------------------------------------------------------------
// Iterative Accumulation: jumlahkan seluruh riwayat + kas awal
void __fastcall TForm1::BtnRekapMalamClick(TObject *Sender)
{
        if (stateKasir == STATE_KONFIRMASI || stateKasir == STATE_BAYAR)
        {
                Pesan("Masih ada transaksi kasir yang belum selesai.",
                      "Rekap Malam", MB_OK | MB_ICONWARNING);
                return;
        }

        totalPenjualan = 0;
        for (int i = 0; i < jumlahRiwayat; i++)
                totalPenjualan += riwayat[i].subtotal;
        totalUangSistem = saldoKasAwal + totalPenjualan;

        LblRKasAwal->Caption        = "Kas Awal              : " + FormatRupiah(saldoKasAwal);
        LblRJmlTransaksi->Caption   = "Jumlah Transaksi      : " + IntToStr(nomorTransaksi);
        LblRTotalPenjualan->Caption = "Total Penjualan       : " + FormatRupiah(totalPenjualan);
        LblTotalUangSistem->Caption = "Total Uang Sistem: " + FormatRupiah(totalUangSistem);

        rekapSudah = true;
        cocokSudah = false;
        LblHasilCocok->Caption = "Hitung uang fisik di laci, lalu tekan Cocokkan.";
        LblHasilCocok->Font->Color = clNavy;
        EdtUangFisik->Enabled = true;
        BtnCocokkan->Enabled  = true;
        EdtUangFisik->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BtnCocokkanClick(TObject *Sender)
{
        if (!BacaAngka(EdtUangFisik, uangFisik) || uangFisik < 0)
        {
                Pesan("Masukkan jumlah uang fisik berupa angka.",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtUangFisik->SetFocus();
                return;
        }

        long selisih = uangFisik - totalUangSistem;
        if (selisih == 0)
        {
                LblHasilCocok->Caption = "SESUAI - uang fisik cocok dengan catatan sistem.";
                LblHasilCocok->Font->Color = clGreen;
        }
        else if (selisih > 0)
        {
                LblHasilCocok->Caption = "TIDAK SESUAI - uang fisik LEBIH " +
                        FormatRupiah(selisih) + " dari catatan sistem.";
                LblHasilCocok->Font->Color = clMaroon;
        }
        else
        {
                LblHasilCocok->Caption = "TIDAK SESUAI - uang fisik KURANG " +
                        FormatRupiah(-selisih) + " dari catatan sistem.";
                LblHasilCocok->Font->Color = clRed;
        }
        cocokSudah = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BtnTutupTokoClick(TObject *Sender)
{
        if (!rekapSudah)
        {
                Pesan("Lakukan Rekap Malam terlebih dahulu.", "Tutup Toko",
                      MB_OK | MB_ICONWARNING);
                return;
        }
        if (Application->MessageBox("Tutup toko dan simpan laporan harian?\n"
                "Aplikasi akan ditutup setelah laporan disimpan.",
                "Tutup Toko", MB_YESNO | MB_ICONQUESTION) != IDYES)
                return;

        const int L = 50;
        AnsiString namaFile = ExtractFilePath(Application->ExeName) + "Laporan_" +
                FormatDateTime("yyyymmdd_hhnnss", Now()) + ".txt";
        TStringList *laporan = new TStringList();
        try
        {
                laporan->Add("LAPORAN HARIAN APOTEK SALWA");
                laporan->Add("Ditutup : " + FormatDateTime("dd/mm/yyyy hh:nn", Now()));
                laporan->Add("");
                laporan->AddStrings(MemoBukuTransaksi->Lines);
                laporan->Add("");
                laporan->Add("REKAPITULASI & REKONSILIASI KAS");
                laporan->Add(AnsiString::StringOfChar('-', L));
                laporan->Add(Baris("Kas Awal", FormatRupiah(saldoKasAwal), L));
                laporan->Add(Baris("Total Penjualan", FormatRupiah(totalPenjualan), L));
                laporan->Add(Baris("Total Uang Sistem", FormatRupiah(totalUangSistem), L));
                if (cocokSudah)
                {
                        laporan->Add(Baris("Uang Fisik di Laci", FormatRupiah(uangFisik), L));
                        laporan->Add(Baris("Selisih", FormatRupiah(uangFisik - totalUangSistem), L));
                }
                else
                        laporan->Add("Uang fisik belum dicocokkan.");
                laporan->Add("");
                laporan->Add("STOK AKHIR GUDANG");
                laporan->Add(AnsiString::StringOfChar('-', L));
                for (int i = 0; i < jumlahObat; i++)
                        laporan->Add(PadKanan(dataObat[i].kode, 8) +
                                     PadKanan(dataObat[i].nama, 30) +
                                     PadKiri(IntToStr(dataObat[i].stok), 6) +
                                     (dataObat[i].stok <= BATAS_KRITIS ? "  *restock" : ""));
                laporan->SaveToFile(namaFile);
        }
        catch (Exception &e)
        {
                delete laporan;
                Pesan("Gagal menyimpan laporan: " + e.Message, "Error", MB_OK | MB_ICONERROR);
                return;
        }
        delete laporan;

        Pesan("Laporan disimpan ke:\n" + namaFile + "\n\nApotek Salwa TUTUP.",
              "Apotek Tutup", MB_OK | MB_ICONINFORMATION);
        tokoDitutup = true;
        Close();
}
//---------------------------------------------------------------------------
