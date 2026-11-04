//---------------------------------------------------------------------------
// Langkah 2 - Pengadaan Obat (Inventory) & Sorting
// Mencatat pasokan obat dari supplier dan mengurutkan data stok.
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UPengadaan.h"
#include "UDataApotek.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPengadaan *FormPengadaan;
//---------------------------------------------------------------------------
__fastcall TFormPengadaan::TFormPengadaan(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormPengadaan::FormCreate(TObject *Sender)
{
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

        CmbKategori->Items->Add("Tablet");
        CmbKategori->Items->Add("Kapsul");
        CmbKategori->Items->Add("Sirup");
        CmbKategori->Items->Add("Salep");
        CmbKategori->Items->Add("Tetes");
        CmbKategori->Items->Add("Injeksi");
        CmbKategori->Items->Add("Alat Kesehatan");
        CmbKategori->Items->Add("Lainnya");
        CmbKategori->ItemIndex = 0;

        CmbKriteria->Items->Add("Stok");
        CmbKriteria->Items->Add("Nama Obat");
        CmbKriteria->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormPengadaan::FormShow(TObject *Sender)
{
        // Stok bisa berubah dari transaksi kasir, tampilkan ulang
        TampilkanGridStok();
        ActiveControl = EdtKode;
}
//---------------------------------------------------------------------------
void __fastcall TFormPengadaan::BtnKembaliClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void TFormPengadaan::TampilkanGridStok()
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
void __fastcall TFormPengadaan::GridStokDrawCell(TObject *Sender, int ACol,
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
void __fastcall TFormPengadaan::GridStokClick(TObject *Sender)
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
void __fastcall TFormPengadaan::BtnSimpanStokClick(TObject *Sender)
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
void __fastcall TFormPengadaan::BtnBersihObatClick(TObject *Sender)
{
        EdtKode->Text     = "";
        EdtNamaObat->Text = "";
        EdtHarga->Text    = "";
        EdtStok->Text     = "";
        CmbKategori->ItemIndex = 0;
        EdtKode->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormPengadaan::BtnUrutkanClick(TObject *Sender)
{
        if (jumlahObat < 2)
        {
                Pesan("Data obat kurang dari 2, tidak perlu diurutkan.", "Info",
                      MB_OK | MB_ICONINFORMATION);
                return;
        }

        int kriteria = CmbKriteria->ItemIndex;
        int arah     = RgArah->ItemIndex;
        int jumlahTukar;
        AnsiString algoritma;
        if (RgAlgoritma->ItemIndex == 0)
        {
                jumlahTukar = SelectionSort(kriteria, arah);
                algoritma = "Selection Sort";
        }
        else
        {
                jumlahTukar = BubbleSort(kriteria, arah);
                algoritma = "Bubble Sort";
        }

        dataUrutNama = (kriteria == URUT_NAMA);
        dataUrutDesc = (arah == ARAH_DESC);
        TampilkanGridStok();
        LblHasilUrut->Caption = "Diurutkan dengan " + algoritma + " berdasarkan " +
                CmbKriteria->Text + " (" + RgArah->Items->Strings[arah] +
                "). Jumlah pertukaran: " + IntToStr(jumlahTukar) + ".";
}
//---------------------------------------------------------------------------
