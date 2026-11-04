object FormTutupToko: TFormTutupToko
  Left = 160
  Top = 80
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Apotek Salwa - 4. Tutup Toko'
  ClientHeight = 612
  ClientWidth = 1028
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PnlNav: TPanel
    Left = 0
    Top = 0
    Width = 1028
    Height = 40
    Align = alTop
    BevelOuter = bvNone
    Color = clNavy
    TabOrder = 0
    object LblNav: TLabel
      Left = 12
      Top = 12
      Width = 400
      Height = 16
      Caption = 'Menu Utama  >  4. Tutup Toko (Rekap dan Rekonsiliasi)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object BtnKembali: TButton
      Left = 812
      Top = 7
      Width = 204
      Height = 26
      Cancel = True
      Caption = '<< Kembali ke Menu Utama'
      TabOrder = 0
      OnClick = BtnKembaliClick
    end
  end
  object GbBuku: TGroupBox
    Left = 8
    Top = 48
    Width = 640
    Height = 556
    Caption = ' Buku Transaksi Penjualan Harian '
    TabOrder = 1
    object MemoBukuTransaksi: TMemo
      Left = 8
      Top = 18
      Width = 624
      Height = 530
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
    end
  end
  object GbRekap: TGroupBox
    Left = 656
    Top = 48
    Width = 364
    Height = 556
    Caption = ' Rekapitulasi dan Rekonsiliasi Kas '
    TabOrder = 2
    object LblRKasAwal: TLabel
      Left = 12
      Top = 68
      Width = 150
      Height = 14
      Caption = 'Kas Awal              : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblRJmlTransaksi: TLabel
      Left = 12
      Top = 88
      Width = 150
      Height = 14
      Caption = 'Jumlah Transaksi      : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblRTotalPenjualan: TLabel
      Left = 12
      Top = 108
      Width = 150
      Height = 14
      Caption = 'Total Penjualan       : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblTotalUangSistem: TLabel
      Left = 12
      Top = 136
      Width = 160
      Height = 16
      Caption = 'Total Uang Sistem: -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LblUangFisik: TLabel
      Left = 12
      Top = 176
      Width = 118
      Height = 13
      Caption = 'Uang Fisik di Laci (Rp):'
    end
    object LblHasilCocok: TLabel
      Left = 12
      Top = 228
      Width = 340
      Height = 44
      AutoSize = False
      Caption = 'Tekan Rekap Malam terlebih dahulu.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
    end
    object LblInfoTutup: TLabel
      Left = 12
      Top = 330
      Width = 340
      Height = 60
      AutoSize = False
      Caption = 'Urutan: Rekap Malam  >  Cocokkan uang fisik  >  Tutup Toko. Laporan harian (buku transaksi, rekap kas, stok akhir) disimpan sebagai file teks di folder aplikasi.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object BtnRekapMalam: TButton
      Left = 12
      Top = 24
      Width = 340
      Height = 30
      Caption = 'Rekap Malam (Hitung Total Kas Sistem)'
      TabOrder = 0
      OnClick = BtnRekapMalamClick
    end
    object EdtUangFisik: TEdit
      Left = 12
      Top = 194
      Width = 200
      Height = 21
      TabOrder = 1
    end
    object BtnCocokkan: TButton
      Left = 220
      Top = 192
      Width = 132
      Height = 25
      Caption = 'Cocokkan'
      TabOrder = 2
      OnClick = BtnCocokkanClick
    end
    object BtnTutupToko: TButton
      Left = 12
      Top = 284
      Width = 340
      Height = 34
      Caption = 'Tutup Toko dan Simpan Laporan'
      TabOrder = 3
      OnClick = BtnTutupTokoClick
    end
  end
end
