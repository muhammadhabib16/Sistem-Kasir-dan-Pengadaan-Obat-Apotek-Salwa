//---------------------------------------------------------------------------
// Sistem Informasi Kasir dan Pengadaan Obat - Apotek Salwa
// Borland C++ Builder 6 (VCL GUI)
//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
// Batas kapasitas array
const int MAX_OBAT      = 100;
const int MAX_KERANJANG = 50;
const int MAX_TRANSAKSI = 1000;
const int BATAS_KRITIS  = 10;   // stok <= nilai ini dianggap kritis (perlu restock)

// Kriteria pengurutan
const int URUT_STOK = 0;        // stok terkecil (ascending)
const int URUT_NAMA = 1;        // nama obat A-Z

// Tahapan form kasir (GUI State Locking)
const int STATE_INPUT      = 0; // memilih obat & mengisi keranjang
const int STATE_KONFIRMASI = 1; // tagihan dihitung, menunggu persetujuan pelanggan
const int STATE_BAYAR      = 2; // pelanggan setuju, input uang bayar
const int STATE_SELESAI    = 3; // transaksi lunas, menunggu pelanggan berikutnya

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
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabKasAwal;
        TTabSheet *TabPengadaan;
        TTabSheet *TabKasir;
        TTabSheet *TabTutup;

        // Tab 1 - Buka Toko
        TLabel *LblJudul;
        TLabel *LblSubJudul;
        TGroupBox *GbKasAwal;
        TLabel *LblKasAwal;
        TEdit *EdtKasAwal;
        TButton *BtnBukaToko;
        TLabel *LblStatusToko;

        // Tab 2 - Pengadaan & Sorting
        TGroupBox *GbInputObat;
        TLabel *LblKode;
        TEdit *EdtKode;
        TLabel *LblNamaObat;
        TEdit *EdtNamaObat;
        TLabel *LblKategori;
        TComboBox *CmbKategori;
        TLabel *LblHarga;
        TEdit *EdtHarga;
        TLabel *LblStok;
        TEdit *EdtStok;
        TButton *BtnSimpanStok;
        TButton *BtnBersihObat;
        TLabel *LblInfoPengadaan;
        TGroupBox *GbUrut;
        TLabel *LblKriteria;
        TComboBox *CmbKriteria;
        TRadioGroup *RgAlgoritma;
        TButton *BtnUrutkan;
        TLabel *LblHasilUrut;
        TGroupBox *GbDaftarObat;
        TStringGrid *GridStok;
        TLabel *LblKeteranganStok;

        // Tab 3 - Kasir (POS) & Searching
        TRadioGroup *RgJenis;
        TGroupBox *GbBiayaRacik;
        TEdit *EdtBiayaRacik;
        TGroupBox *GbCari;
        TLabel *LblCari;
        TEdit *EdtCariObat;
        TButton *BtnCari;
        TCheckBox *CbBinarySearch;
        TLabel *LblHNama;
        TLabel *LblHHarga;
        TLabel *LblHStok;
        TLabel *LblMetodeCari;
        TLabel *LblJumlah;
        TEdit *EdtJumlahBeli;
        TButton *BtnTambahKeranjang;
        TButton *BtnHapusItem;
        TGroupBox *GbKeranjang;
        TStringGrid *GridKeranjang;
        TGroupBox *GbTagihan;
        TLabel *LblTotalTagihan;
        TButton *BtnKonfirmasi;
        TLabel *LblTotalTag;
        TEdit *EdtTotalTagihan;
        TButton *BtnSetuju;
        TButton *BtnBatal;
        TLabel *LblUangBayar;
        TEdit *EdtUangBayar;
        TButton *BtnProsesBayar;
        TLabel *LblKembalian;
        TButton *BtnTransaksiBaru;
        TGroupBox *GbStruk;
        TMemo *MemoStruk;

        // Tab 4 - Tutup Toko & Rekonsiliasi
        TGroupBox *GbBuku;
        TMemo *MemoBukuTransaksi;
        TGroupBox *GbRekap;
        TButton *BtnRekapMalam;
        TLabel *LblRKasAwal;
        TLabel *LblRJmlTransaksi;
        TLabel *LblRTotalPenjualan;
        TLabel *LblTotalUangSistem;
        TLabel *LblUangFisik;
        TEdit *EdtUangFisik;
        TButton *BtnCocokkan;
        TLabel *LblHasilCocok;
        TButton *BtnTutupToko;
        TLabel *LblInfoTutup;

        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall BtnBukaTokoClick(TObject *Sender);
        void __fastcall BtnSimpanStokClick(TObject *Sender);
        void __fastcall BtnBersihObatClick(TObject *Sender);
        void __fastcall BtnUrutkanClick(TObject *Sender);
        void __fastcall GridStokClick(TObject *Sender);
        void __fastcall GridStokDrawCell(TObject *Sender, int ACol, int ARow,
                TRect &Rect, TGridDrawState State);
        void __fastcall RgJenisClick(TObject *Sender);
        void __fastcall EdtBiayaRacikChange(TObject *Sender);
        void __fastcall BtnCariClick(TObject *Sender);
        void __fastcall EdtCariObatKeyPress(TObject *Sender, char &Key);
        void __fastcall BtnTambahKeranjangClick(TObject *Sender);
        void __fastcall BtnHapusItemClick(TObject *Sender);
        void __fastcall BtnKonfirmasiClick(TObject *Sender);
        void __fastcall BtnSetujuClick(TObject *Sender);
        void __fastcall BtnBatalClick(TObject *Sender);
        void __fastcall BtnProsesBayarClick(TObject *Sender);
        void __fastcall BtnTransaksiBaruClick(TObject *Sender);
        void __fastcall BtnRekapMalamClick(TObject *Sender);
        void __fastcall BtnCocokkanClick(TObject *Sender);
        void __fastcall BtnTutupTokoClick(TObject *Sender);
private:	// User declarations
        void TampilkanGridStok();
        void TampilkanKeranjang();
        void TampilkanBukuTransaksi();
        void PerbaruiTotalRealtime();
        void AturStateKasir(int state);
        void ResetHasilCari();
        void KosongkanKeranjang();
        void CetakStruk(int noTrans, long uangBayar, long kembalian);
        void ResetRekap();
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
