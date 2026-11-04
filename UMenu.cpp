//---------------------------------------------------------------------------
// Menu Utama - pusat navigasi aplikasi Apotek Salwa
//
// Alur navigasi:
//   [1. Buka Toko] --> [2. Pengadaan Obat] dan [3. Kasir] (bisa bolak-balik)
//                  --> [4. Tutup Toko] --> aplikasi selesai
//
// Setiap modul dibuka sebagai form modal; tombol "Kembali ke Menu Utama"
// pada setiap form mengembalikan pengguna ke menu ini.
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UMenu.h"
#include "UDataApotek.h"
#include "UBukaToko.h"
#include "UPengadaan.h"
#include "UKasir.h"
#include "UTutupToko.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMenu *FormMenu;
//---------------------------------------------------------------------------
__fastcall TFormMenu::TFormMenu(TComponent* Owner)
        : TForm(Owner)
{
        InisialisasiDataObat();
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::FormShow(TObject *Sender)
{
        PerbaruiMenu();
}
//---------------------------------------------------------------------------
// Tampilkan status toko & aktifkan tombol sesuai urutan alur kerja
void TFormMenu::PerbaruiMenu()
{
        if (!tokoDibuka)
        {
                LblStatusToko->Caption = "TOKO BELUM DIBUKA";
                LblStatusToko->Font->Color = clMaroon;
        }
        else
        {
                LblStatusToko->Caption = "TOKO BUKA sejak " + FormatDateTime("hh:nn", waktuBuka);
                LblStatusToko->Font->Color = clGreen;
        }

        int kritis = JumlahObatKritis();
        LblInfoKasAwal->Caption   = "Kas Awal         : " + (tokoDibuka ? FormatRupiah(saldoKasAwal) : AnsiString("-"));
        LblInfoTransaksi->Caption = "Transaksi Lunas  : " + IntToStr(nomorTransaksi);
        LblInfoPenjualan->Caption = "Total Penjualan  : " + FormatRupiah(HitungTotalPenjualan());
        LblInfoKritis->Caption    = "Obat Stok Kritis : " + IntToStr(kritis) + " jenis";
        LblInfoKritis->Font->Color = (kritis > 0) ? clRed : clWindowText;

        // Langkah 1 hanya sekali; langkah 2-4 terbuka setelah toko dibuka
        BtnMenuBukaToko->Enabled  = !tokoDibuka;
        BtnMenuPengadaan->Enabled = tokoDibuka && !tokoDitutup;
        BtnMenuKasir->Enabled     = tokoDibuka && !tokoDitutup;
        BtnMenuTutup->Enabled     = tokoDibuka && !tokoDitutup;
        BtnMenuBukaToko->Caption  = tokoDibuka ? "1.  Buka Toko  [SELESAI]"
                                               : "1.  Buka Toko  (Input Kas Awal)";

        if (!tokoDibuka)
        {
                LblLangkah->Caption = "Langkah berikutnya: tekan  1. Buka Toko  untuk "
                                      "memasukkan kas awal. Menu lain masih terkunci.";
                ActiveControl = BtnMenuBukaToko;
        }
        else if (!tokoDitutup)
        {
                ActiveControl = BtnMenuKasir;
                AnsiString s = "Toko buka. Layani pelanggan di  3. Kasir , catat pasokan "
                               "di  2. Pengadaan Obat . Malam hari pilih  4. Tutup Toko .";
                if (kritis > 0)
                        s += "  Perhatian: ada " + IntToStr(kritis) +
                             " obat stok kritis, cek menu Pengadaan.";
                LblLangkah->Caption = s;
        }
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::BtnMenuBukaTokoClick(TObject *Sender)
{
        if (FormBukaToko->ShowModal() == mrOk)
        {
                PerbaruiMenu();
                // Setelah buka toko, langsung tawarkan masuk ke kasir
                if (Application->MessageBox("Toko sudah dibuka. Langsung ke menu Kasir?",
                        "Buka Toko", MB_YESNO | MB_ICONQUESTION) == IDYES)
                        BtnMenuKasirClick(Sender);
        }
        PerbaruiMenu();
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::BtnMenuPengadaanClick(TObject *Sender)
{
        FormPengadaan->ShowModal();
        PerbaruiMenu();
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::BtnMenuKasirClick(TObject *Sender)
{
        FormKasir->ShowModal();
        PerbaruiMenu();
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::BtnMenuTutupClick(TObject *Sender)
{
        FormTutupToko->ShowModal();
        PerbaruiMenu();
        if (tokoDitutup)
                Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::BtnKeluarClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormMenu::FormCloseQuery(TObject *Sender, bool &CanClose)
{
        if (tokoDibuka && !tokoDitutup)
        {
                CanClose = Application->MessageBox(
                        "Toko belum ditutup melalui menu 4. Tutup Toko.\n"
                        "Data transaksi hari ini akan hilang. Tetap keluar?",
                        "Konfirmasi Keluar", MB_YESNO | MB_ICONWARNING) == IDYES;
        }
}
//---------------------------------------------------------------------------
