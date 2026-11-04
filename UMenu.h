//---------------------------------------------------------------------------
// Menu Utama - pusat navigasi aplikasi Apotek Salwa
//---------------------------------------------------------------------------

#ifndef UMenuH
#define UMenuH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormMenu : public TForm
{
__published:	// IDE-managed Components
        TPanel *PnlHeader;
        TLabel *LblJudul;
        TLabel *LblSubJudul;
        TGroupBox *GbStatus;
        TLabel *LblStatusToko;
        TLabel *LblInfoKasAwal;
        TLabel *LblInfoTransaksi;
        TLabel *LblInfoPenjualan;
        TLabel *LblInfoKritis;
        TButton *BtnKeluar;
        TGroupBox *GbAlur;
        TButton *BtnMenuBukaToko;
        TLabel *LblDesc1;
        TButton *BtnMenuPengadaan;
        TLabel *LblDesc2;
        TButton *BtnMenuKasir;
        TLabel *LblDesc3;
        TButton *BtnMenuTutup;
        TLabel *LblDesc4;
        TLabel *LblLangkah;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall BtnMenuBukaTokoClick(TObject *Sender);
        void __fastcall BtnMenuPengadaanClick(TObject *Sender);
        void __fastcall BtnMenuKasirClick(TObject *Sender);
        void __fastcall BtnMenuTutupClick(TObject *Sender);
        void __fastcall BtnKeluarClick(TObject *Sender);
private:	// User declarations
        void PerbaruiMenu();
public:		// User declarations
        __fastcall TFormMenu(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMenu *FormMenu;
//---------------------------------------------------------------------------
#endif
