object FormMenu: TFormMenu
  Left = 200
  Top = 120
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Apotek Salwa - Menu Utama'
  ClientHeight = 452
  ClientWidth = 744
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PnlHeader: TPanel
    Left = 0
    Top = 0
    Width = 744
    Height = 70
    Align = alTop
    BevelOuter = bvNone
    Color = clNavy
    TabOrder = 0
    object LblJudul: TLabel
      Left = 16
      Top = 10
      Width = 175
      Height = 29
      Caption = 'APOTEK SALWA'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LblSubJudul: TLabel
      Left = 16
      Top = 44
      Width = 300
      Height = 16
      Caption = 'Sistem Informasi Kasir dan Pengadaan Obat  -  Menu Utama'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
  end
  object GbStatus: TGroupBox
    Left = 16
    Top = 84
    Width = 300
    Height = 310
    Caption = ' Status Operasional '
    TabOrder = 1
    object LblStatusToko: TLabel
      Left = 12
      Top = 26
      Width = 160
      Height = 16
      Caption = 'TOKO BELUM DIBUKA'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LblInfoKasAwal: TLabel
      Left = 12
      Top = 64
      Width = 150
      Height = 14
      Caption = 'Kas Awal         : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblInfoTransaksi: TLabel
      Left = 12
      Top = 88
      Width = 150
      Height = 14
      Caption = 'Transaksi Lunas  : 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblInfoPenjualan: TLabel
      Left = 12
      Top = 112
      Width = 150
      Height = 14
      Caption = 'Total Penjualan  : Rp 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblInfoKritis: TLabel
      Left = 12
      Top = 136
      Width = 150
      Height = 14
      Caption = 'Obat Stok Kritis : 0 jenis'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object BtnKeluar: TButton
      Left = 12
      Top = 266
      Width = 276
      Height = 30
      Caption = 'Keluar Aplikasi'
      TabOrder = 0
      OnClick = BtnKeluarClick
    end
  end
  object GbAlur: TGroupBox
    Left = 328
    Top = 84
    Width = 400
    Height = 310
    Caption = ' Alur Kerja Apotek (ikuti urutan nomor) '
    TabOrder = 2
    object LblDesc1: TLabel
      Left = 20
      Top = 60
      Width = 250
      Height = 13
      Caption = 'Pagi hari: masukkan sisa kas semalam. Wajib pertama.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LblDesc2: TLabel
      Left = 20
      Top = 128
      Width = 250
      Height = 13
      Caption = 'Catat pasokan dari supplier, urutkan stok (Sorting).'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LblDesc3: TLabel
      Left = 20
      Top = 196
      Width = 250
      Height = 13
      Caption = 'Cari obat (Searching), keranjang, konfirmasi, bayar, struk.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LblDesc4: TLabel
      Left = 20
      Top = 264
      Width = 250
      Height = 13
      Caption = 'Malam hari: rekap kas, cocokkan uang fisik, simpan laporan.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object BtnMenuBukaToko: TButton
      Left = 16
      Top = 22
      Width = 368
      Height = 34
      Caption = '1.  Buka Toko  (Input Kas Awal)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = BtnMenuBukaTokoClick
    end
    object BtnMenuPengadaan: TButton
      Left = 16
      Top = 90
      Width = 368
      Height = 34
      Caption = '2.  Pengadaan Obat  (Gudang)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = BtnMenuPengadaanClick
    end
    object BtnMenuKasir: TButton
      Left = 16
      Top = 158
      Width = 368
      Height = 34
      Caption = '3.  Kasir  (Pelayanan Pelanggan)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      OnClick = BtnMenuKasirClick
    end
    object BtnMenuTutup: TButton
      Left = 16
      Top = 226
      Width = 368
      Height = 34
      Caption = '4.  Tutup Toko  (Rekap dan Rekonsiliasi)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      OnClick = BtnMenuTutupClick
    end
  end
  object LblLangkah: TLabel
    Left = 16
    Top = 404
    Width = 712
    Height = 40
    AutoSize = False
    Caption = 'Langkah berikutnya:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    WordWrap = True
  end
end
