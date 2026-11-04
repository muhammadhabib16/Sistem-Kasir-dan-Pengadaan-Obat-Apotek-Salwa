//---------------------------------------------------------------------------
// Langkah 1 - Buka Toko (Kas Awal)
// Input sisa kas semalam sebagai saldo awal operasional hari ini.
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UBukaToko.h"
#include "UDataApotek.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBukaToko *FormBukaToko;
//---------------------------------------------------------------------------
__fastcall TFormBukaToko::TFormBukaToko(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormBukaToko::FormShow(TObject *Sender)
{
        EdtKasAwal->Text = "";
        ActiveControl = EdtKasAwal;
}
//---------------------------------------------------------------------------
void __fastcall TFormBukaToko::BtnBukaTokoClick(TObject *Sender)
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
        waktuBuka    = Now();
        tokoDibuka   = true;

        Pesan("Toko dibuka pukul " + FormatDateTime("hh:nn", waktuBuka) +
              "\nKas awal: " + FormatRupiah(saldoKasAwal),
              "Buka Toko", MB_OK | MB_ICONINFORMATION);
        ModalResult = mrOk;     // kembali ke Menu Utama
}
//---------------------------------------------------------------------------
