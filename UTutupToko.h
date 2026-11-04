//---------------------------------------------------------------------------
// Langkah 4 - Tutup Toko & Rekonsiliasi Keuangan (Malam Hari)
//---------------------------------------------------------------------------

#ifndef UTutupTokoH
#define UTutupTokoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormTutupToko : public TForm
{
__published:	// IDE-managed Components
        TPanel *PnlNav;
        TLabel *LblNav;
        TButton *BtnKembali;
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
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnKembaliClick(TObject *Sender);
        void __fastcall BtnRekapMalamClick(TObject *Sender);
        void __fastcall BtnCocokkanClick(TObject *Sender);
        void __fastcall BtnTutupTokoClick(TObject *Sender);
private:	// User declarations
        bool rekapSudah;
        bool cocokSudah;
        long totalPenjualan;
        long totalUangSistem;
        long uangFisik;

        void ResetRekap();
public:		// User declarations
        __fastcall TFormTutupToko(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTutupToko *FormTutupToko;
//---------------------------------------------------------------------------
#endif
