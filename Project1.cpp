//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("UMenu.cpp", FormMenu);
USEFORM("UBukaToko.cpp", FormBukaToko);
USEFORM("UPengadaan.cpp", FormPengadaan);
USEFORM("UKasir.cpp", FormKasir);
USEFORM("UTutupToko.cpp", FormTutupToko);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Apotek Salwa";
                 // Form pertama = form utama (Menu Utama)
                 Application->CreateForm(__classid(TFormMenu), &FormMenu);
                 Application->CreateForm(__classid(TFormBukaToko), &FormBukaToko);
                 Application->CreateForm(__classid(TFormPengadaan), &FormPengadaan);
                 Application->CreateForm(__classid(TFormKasir), &FormKasir);
                 Application->CreateForm(__classid(TFormTutupToko), &FormTutupToko);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
