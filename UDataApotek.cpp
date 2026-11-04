//---------------------------------------------------------------------------
// Data bersama, struktur data, dan algoritma Apotek Salwa
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UDataApotek.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//===========================================================================
// DATA GLOBAL (Array of Struct)
//===========================================================================
Obat      dataObat[MAX_OBAT];           // inventaris gudang
int       jumlahObat = 0;

Transaksi riwayat[MAX_TRANSAKSI];       // buku transaksi harian
int       jumlahRiwayat = 0;
int       nomorTransaksi = 0;

long      saldoKasAwal = 0;
TDateTime waktuBuka;
bool      tokoDibuka   = false;
bool      tokoDitutup  = false;
bool      dataUrutNama = false;
bool      dataUrutDesc = false;

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

//===========================================================================
// DATA OBAT
//===========================================================================
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
//---------------------------------------------------------------------------
// Data awal inventaris (contoh)
void InisialisasiDataObat()
{
        jumlahObat = 0;
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
        dataUrutNama = false;
        dataUrutDesc = false;
}
//---------------------------------------------------------------------------
int JumlahObatKritis()
{
        int n = 0;
        for (int i = 0; i < jumlahObat; i++)
                if (dataObat[i].stok <= BATAS_KRITIS)
                        n++;
        return n;
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
// array wajib sudah terurut berdasarkan nama (ascending atau descending).
int BinarySearchNama(AnsiString kunci, int &langkah)
{
        int kiri = 0, kanan = jumlahObat - 1;
        kunci = Trim(kunci);
        langkah = 0;
        while (kiri <= kanan)
        {
                int tengah = (kiri + kanan) / 2;
                int banding = AnsiCompareText(dataObat[tengah].nama, kunci);
                if (dataUrutDesc) banding = -banding;   // urutan Z-A: arah dibalik
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
// true jika a seharusnya berada SETELAH b menurut kriteria dan arah
static bool HarusDitukar(const Obat &a, const Obat &b, int kriteria, int arah)
{
        int banding;    // < 0 jika a < b, 0 jika sama, > 0 jika a > b
        if (kriteria == URUT_STOK)
                banding = a.stok - b.stok;
        else
                banding = AnsiCompareText(a.nama, b.nama);

        if (arah == ARAH_ASC)
                return banding > 0;
        return banding < 0;
}
//---------------------------------------------------------------------------
static void TukarObat(Obat &a, Obat &b)
{
        Obat temp = a;
        a = b;
        b = temp;
}
//---------------------------------------------------------------------------
// Selection Sort: cari elemen terkecil (ascending) / terbesar (descending)
// lalu tukar ke posisi i. Mengembalikan jumlah pertukaran yang terjadi.
int SelectionSort(int kriteria, int arah)
{
        int jumlahTukar = 0;
        for (int i = 0; i < jumlahObat - 1; i++)
        {
                int idxPilih = i;
                for (int j = i + 1; j < jumlahObat; j++)
                        if (HarusDitukar(dataObat[idxPilih], dataObat[j], kriteria, arah))
                                idxPilih = j;
                if (idxPilih != i)
                {
                        TukarObat(dataObat[i], dataObat[idxPilih]);
                        jumlahTukar++;
                }
        }
        return jumlahTukar;
}
//---------------------------------------------------------------------------
// Bubble Sort: tukar elemen bertetangga; berhenti lebih awal jika
// tidak ada pertukaran pada satu putaran.
int BubbleSort(int kriteria, int arah)
{
        int jumlahTukar = 0;
        for (int i = 0; i < jumlahObat - 1; i++)
        {
                bool adaTukar = false;
                for (int j = 0; j < jumlahObat - 1 - i; j++)
                {
                        if (HarusDitukar(dataObat[j], dataObat[j + 1], kriteria, arah))
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
// TRANSAKSI
//===========================================================================
// Iterative Accumulation: jumlahkan seluruh riwayat penjualan
long HitungTotalPenjualan()
{
        long total = 0;
        for (int i = 0; i < jumlahRiwayat; i++)
                total += riwayat[i].subtotal;
        return total;
}
//---------------------------------------------------------------------------
void SusunBukuTransaksi(TStrings *s)
{
        s->BeginUpdate();
        s->Clear();
        s->Add("BUKU TRANSAKSI PENJUALAN HARIAN - APOTEK SALWA");
        s->Add(AnsiString::StringOfChar('=', 74));
        s->Add(PadKanan("Nota", 9) + PadKanan("Jam", 6) + PadKanan("Jenis", 7) +
               PadKanan("Nama Obat", 24) + PadKiri("Qty", 4) +
               PadKiri("Harga", 11) + PadKiri("Total", 13));
        s->Add(AnsiString::StringOfChar('-', 74));

        for (int i = 0; i < jumlahRiwayat; i++)
        {
                s->Add(PadKanan("TRX-" + FormatFloat("0000", riwayat[i].noTrans), 9) +
                       PadKanan(FormatDateTime("hh:nn", riwayat[i].waktu), 6) +
                       PadKanan(riwayat[i].jenis, 7) +
                       PadKanan(riwayat[i].namaObat, 24) +
                       PadKiri(IntToStr(riwayat[i].jumlah), 4) +
                       PadKiri(FormatFloat("#,##0", riwayat[i].harga), 11) +
                       PadKiri(FormatFloat("#,##0", riwayat[i].subtotal), 13));
        }
        if (jumlahRiwayat == 0)
                s->Add("(belum ada transaksi)");
        s->Add(AnsiString::StringOfChar('-', 74));
        s->Add(Baris("TOTAL PENJUALAN (" + IntToStr(nomorTransaksi) + " transaksi)",
                     FormatRupiah(HitungTotalPenjualan()), 74));
        s->EndUpdate();
}
//---------------------------------------------------------------------------
