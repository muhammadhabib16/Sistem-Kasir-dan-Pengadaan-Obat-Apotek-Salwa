//---------------------------------------------------------------------------
// Langkah 1 - Buka Toko (Kas Awal)
//---------------------------------------------------------------------------

#ifndef UBukaTokoH
#define UBukaTokoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormBukaToko : public TForm
{
__published:	// IDE-managed Components
        TPanel *PnlNav;
        TLabel *LblNav;
        TLabel *LblInfo;
        TLabel *LblKasAwal;
        TEdit *EdtKasAwal;
        TButton *BtnBukaToko;
        TButton *BtnKembali;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnBukaTokoClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormBukaToko(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBukaToko *FormBukaToko;
//---------------------------------------------------------------------------
#endif
