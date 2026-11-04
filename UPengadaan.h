//---------------------------------------------------------------------------
// Langkah 2 - Pengadaan Obat (Inventory) & Sorting
//---------------------------------------------------------------------------

#ifndef UPengadaanH
#define UPengadaanH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TFormPengadaan : public TForm
{
__published:	// IDE-managed Components
        TPanel *PnlNav;
        TLabel *LblNav;
        TButton *BtnKembali;
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
        TRadioGroup *RgArah;
        TRadioGroup *RgAlgoritma;
        TButton *BtnUrutkan;
        TLabel *LblHasilUrut;
        TGroupBox *GbDaftarObat;
        TStringGrid *GridStok;
        TLabel *LblKeteranganStok;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnKembaliClick(TObject *Sender);
        void __fastcall BtnSimpanStokClick(TObject *Sender);
        void __fastcall BtnBersihObatClick(TObject *Sender);
        void __fastcall BtnUrutkanClick(TObject *Sender);
        void __fastcall GridStokClick(TObject *Sender);
        void __fastcall GridStokDrawCell(TObject *Sender, int ACol, int ARow,
                TRect &Rect, TGridDrawState State);
private:	// User declarations
        void TampilkanGridStok();
public:		// User declarations
        __fastcall TFormPengadaan(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPengadaan *FormPengadaan;
//---------------------------------------------------------------------------
#endif
