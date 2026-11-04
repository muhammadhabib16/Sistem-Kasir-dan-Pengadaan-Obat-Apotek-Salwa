//---------------------------------------------------------------------------
// Data bersama, struktur data, dan algoritma Apotek Salwa
// (dipakai oleh semua form)
//---------------------------------------------------------------------------

#ifndef UDataApotekH
#define UDataApotekH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
// Batas kapasitas array
const int MAX_OBAT      = 100;
const int MAX_KERANJANG = 50;
const int MAX_TRANSAKSI = 1000;
const int BATAS_KRITIS  = 10;   // stok <= nilai ini dianggap kritis (perlu restock)

// Kriteria (kolom) pengurutan
const int URUT_STOK = 0;
const int URUT_NAMA = 1;

// Arah pengurutan
const int ARAH_ASC  = 0;        // kecil ke besar / A-Z
const int ARAH_DESC = 1;        // besar ke kecil / Z-A

// Data master obat di gudang
struct Obat
{
        AnsiString kode;
        AnsiString nama;
        AnsiString kategori;
        long       harga;       // harga jual satuan (Rp)
        int        stok;
};

// Satu baris obat di keranjang belanja
struct ItemKeranjang
{
        AnsiString kode;
        AnsiString nama;
        long       harga;
        int        jumlah;
        long       subtotal;
};

// Satu baris pada buku transaksi penjualan harian
struct Transaksi
{
        int        noTrans;
        TDateTime  waktu;
        AnsiString jenis;       // "Resep" atau "Bebas"
        AnsiString namaObat;
        int        jumlah;
        long       harga;
        long       subtotal;
};
//---------------------------------------------------------------------------
// Data global (Array of Struct)
extern Obat      dataObat[MAX_OBAT];
extern int       jumlahObat;
extern Transaksi riwayat[MAX_TRANSAKSI];
extern int       jumlahRiwayat;
extern int       nomorTransaksi;        // jumlah transaksi lunas

extern long      saldoKasAwal;
extern TDateTime waktuBuka;
extern bool      tokoDibuka;
extern bool      tokoDitutup;
extern bool      dataUrutNama;          // true jika array terurut berdasarkan nama (syarat Binary Search)
extern bool      dataUrutDesc;          // arah urutan terakhir: true = descending
//---------------------------------------------------------------------------
// Fungsi bantu
AnsiString FormatRupiah(long nilai);
bool       BacaAngka(TEdit *edt, long &hasil);
void       Pesan(AnsiString teks, AnsiString judul, int flags);
AnsiString PadKanan(AnsiString s, int lebar);
AnsiString PadKiri(AnsiString s, int lebar);
AnsiString Baris(AnsiString kiri, AnsiString kanan, int lebar);

// Data obat
void TambahObat(AnsiString kode, AnsiString nama, AnsiString kategori,
                long harga, int stok);
void InisialisasiDataObat();
int  JumlahObatKritis();

// Searching
int CariKode(AnsiString kode);
int SequentialSearch(AnsiString kunci, int &langkah);
int BinarySearchNama(AnsiString kunci, int &langkah);

// Sorting
int SelectionSort(int kriteria, int arah);
int BubbleSort(int kriteria, int arah);

// Transaksi
long HitungTotalPenjualan();
void SusunBukuTransaksi(TStrings *s);
//---------------------------------------------------------------------------
#endif
