//---------------------------------------------------------------------------
// Langkah 4 - Tutup Toko & Rekonsiliasi Keuangan (Malam Hari)
//
// Alur di dalam form:
//   Rekap Malam --> Cocokkan uang fisik --> Tutup Toko (simpan laporan)
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UTutupToko.h"
#include "UDataApotek.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTutupToko *FormTutupToko;
//---------------------------------------------------------------------------
__fastcall TFormTutupToko::TFormTutupToko(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormTutupToko::FormShow(TObject *Sender)
{
        // Bisa ada transaksi baru sejak form terakhir dibuka
        SusunBukuTransaksi(MemoBukuTransaksi->Lines);
        ResetRekap();
        ActiveControl = BtnRekapMalam;
}
//---------------------------------------------------------------------------
void __fastcall TFormTutupToko::BtnKembaliClick(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------
void TFormTutupToko::ResetRekap()
{
        rekapSudah = false;
        cocokSudah = false;
        LblRKasAwal->Caption        = "Kas Awal              : " + FormatRupiah(saldoKasAwal);
        LblRJmlTransaksi->Caption   = "Jumlah Transaksi      : -";
        LblRTotalPenjualan->Caption = "Total Penjualan       : -";
        LblTotalUangSistem->Caption = "Total Uang Sistem: -";
        LblHasilCocok->Caption = "Tekan Rekap Malam terlebih dahulu.";
        LblHasilCocok->Font->Color = clGray;
        EdtUangFisik->Text    = "";
        EdtUangFisik->Enabled = false;
        BtnCocokkan->Enabled  = false;
        BtnTutupToko->Enabled = false;
}
//---------------------------------------------------------------------------
// Iterative Accumulation: total penjualan + kas awal
void __fastcall TFormTutupToko::BtnRekapMalamClick(TObject *Sender)
{
        totalPenjualan  = HitungTotalPenjualan();
        totalUangSistem = saldoKasAwal + totalPenjualan;

        LblRKasAwal->Caption        = "Kas Awal              : " + FormatRupiah(saldoKasAwal);
        LblRJmlTransaksi->Caption   = "Jumlah Transaksi      : " + IntToStr(nomorTransaksi);
        LblRTotalPenjualan->Caption = "Total Penjualan       : " + FormatRupiah(totalPenjualan);
        LblTotalUangSistem->Caption = "Total Uang Sistem: " + FormatRupiah(totalUangSistem);

        rekapSudah = true;
        cocokSudah = false;
        LblHasilCocok->Caption = "Hitung uang fisik di laci, lalu tekan Cocokkan.";
        LblHasilCocok->Font->Color = clNavy;
        EdtUangFisik->Enabled = true;
        BtnCocokkan->Enabled  = true;
        BtnTutupToko->Enabled = true;
        EdtUangFisik->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormTutupToko::BtnCocokkanClick(TObject *Sender)
{
        if (!BacaAngka(EdtUangFisik, uangFisik) || uangFisik < 0)
        {
                Pesan("Masukkan jumlah uang fisik berupa angka.",
                      "Input Tidak Valid", MB_OK | MB_ICONWARNING);
                EdtUangFisik->SetFocus();
                return;
        }

        long selisih = uangFisik - totalUangSistem;
        if (selisih == 0)
        {
                LblHasilCocok->Caption = "SESUAI - uang fisik cocok dengan catatan sistem.";
                LblHasilCocok->Font->Color = clGreen;
        }
        else if (selisih > 0)
        {
                LblHasilCocok->Caption = "TIDAK SESUAI - uang fisik LEBIH " +
                        FormatRupiah(selisih) + " dari catatan sistem.";
                LblHasilCocok->Font->Color = clMaroon;
        }
        else
        {
                LblHasilCocok->Caption = "TIDAK SESUAI - uang fisik KURANG " +
                        FormatRupiah(-selisih) + " dari catatan sistem.";
                LblHasilCocok->Font->Color = clRed;
        }
        cocokSudah = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormTutupToko::BtnTutupTokoClick(TObject *Sender)
{
        if (!rekapSudah)
        {
                Pesan("Lakukan Rekap Malam terlebih dahulu.", "Tutup Toko",
                      MB_OK | MB_ICONWARNING);
                return;
        }
        if (Application->MessageBox("Tutup toko dan simpan laporan harian?\n"
                "Aplikasi akan selesai setelah laporan disimpan.",
                "Tutup Toko", MB_YESNO | MB_ICONQUESTION) != IDYES)
                return;

        const int L = 50;
        AnsiString namaFile = ExtractFilePath(Application->ExeName) + "Laporan_" +
                FormatDateTime("yyyymmdd_hhnnss", Now()) + ".txt";
        TStringList *laporan = new TStringList();
        try
        {
                laporan->Add("LAPORAN HARIAN APOTEK SALWA");
                laporan->Add("Dibuka  : " + FormatDateTime("dd/mm/yyyy hh:nn", waktuBuka));
                laporan->Add("Ditutup : " + FormatDateTime("dd/mm/yyyy hh:nn", Now()));
                laporan->Add("");
                laporan->AddStrings(MemoBukuTransaksi->Lines);
                laporan->Add("");
                laporan->Add("REKAPITULASI & REKONSILIASI KAS");
                laporan->Add(AnsiString::StringOfChar('-', L));
                laporan->Add(Baris("Kas Awal", FormatRupiah(saldoKasAwal), L));
                laporan->Add(Baris("Total Penjualan", FormatRupiah(totalPenjualan), L));
                laporan->Add(Baris("Total Uang Sistem", FormatRupiah(totalUangSistem), L));
                if (cocokSudah)
                {
                        laporan->Add(Baris("Uang Fisik di Laci", FormatRupiah(uangFisik), L));
                        laporan->Add(Baris("Selisih", FormatRupiah(uangFisik - totalUangSistem), L));
                }
                else
                        laporan->Add("Uang fisik belum dicocokkan.");
                laporan->Add("");
                laporan->Add("STOK AKHIR GUDANG");
                laporan->Add(AnsiString::StringOfChar('-', L));
                for (int i = 0; i < jumlahObat; i++)
                        laporan->Add(PadKanan(dataObat[i].kode, 8) +
                                     PadKanan(dataObat[i].nama, 30) +
                                     PadKiri(IntToStr(dataObat[i].stok), 6) +
                                     (dataObat[i].stok <= BATAS_KRITIS ? "  *restock" : ""));
                laporan->SaveToFile(namaFile);
        }
        catch (Exception &e)
        {
                delete laporan;
                Pesan("Gagal menyimpan laporan: " + e.Message, "Error", MB_OK | MB_ICONERROR);
                return;
        }
        delete laporan;

        Pesan("Laporan disimpan ke:\n" + namaFile + "\n\nApotek Salwa TUTUP.",
              "Apotek Tutup", MB_OK | MB_ICONINFORMATION);
        tokoDitutup = true;
        ModalResult = mrOk;     // kembali ke Menu Utama, lalu aplikasi selesai
}
//---------------------------------------------------------------------------
