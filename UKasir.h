//---------------------------------------------------------------------------
// Langkah 3 - Kasir (Point of Sale) & Searching
//---------------------------------------------------------------------------

#ifndef UKasirH
#define UKasirH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
// Tahapan form kasir (GUI State Locking)
const int STATE_INPUT      = 0; // memilih obat & mengisi keranjang
const int STATE_KONFIRMASI = 1; // tagihan dihitung, menunggu persetujuan pelanggan
const int STATE_BAYAR      = 2; // pelanggan setuju, input uang bayar
const int STATE_SELESAI    = 3; // transaksi lunas, menunggu pelanggan berikutnya
//---------------------------------------------------------------------------
class TFormKasir : public TForm
{
__published:	// IDE-managed Components
        TPanel *PnlNav;
        TLabel *LblNav;
        TLabel *LblTahap;
        TButton *BtnKembali;
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
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall BtnKembaliClick(TObject *Sender);
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
private:	// User declarations
        int        stateKasir;
        AnsiString kodeDitemukan;       // kode obat hasil pencarian terakhir
        long       biayaRacik;          // biaya racik transaksi aktif
        long       totalTagihan;

        void TampilkanKeranjang();
        void PerbaruiTotalRealtime();
        void AturStateKasir(int state);
        void ResetHasilCari();
        void KosongkanKeranjang();
        void CetakStruk(int noTrans, long uangBayar, long kembalian);
public:		// User declarations
        __fastcall TFormKasir(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKasir *FormKasir;
//---------------------------------------------------------------------------
#endif
