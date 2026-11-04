object FormKasir: TFormKasir
  Left = 160
  Top = 80
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Apotek Salwa - 3. Kasir'
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
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
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
      Width = 200
      Height = 16
      Caption = 'Menu Utama  >  3. Kasir'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LblTahap: TLabel
      Left = 216
      Top = 13
      Width = 580
      Height = 14
      AutoSize = False
      Caption = 'Tahap:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object BtnKembali: TButton
      Left = 812
      Top = 7
      Width = 204
      Height = 26
      Caption = '<< Kembali ke Menu Utama'
      TabOrder = 0
      OnClick = BtnKembaliClick
    end
  end
  object RgJenis: TRadioGroup
    Left = 8
    Top = 48
    Width = 240
    Height = 64
    Caption = ' Jenis Permintaan '
    ItemIndex = 1
    Items.Strings = (
      'Resep Dokter'
      'Beli Langsung')
    TabOrder = 1
    OnClick = RgJenisClick
  end
  object GbBiayaRacik: TGroupBox
    Left = 256
    Top = 48
    Width = 252
    Height = 64
    Caption = ' Biaya Racik / Tuslah (Rp) '
    TabOrder = 2
    object EdtBiayaRacik: TEdit
      Left = 12
      Top = 26
      Width = 228
      Height = 21
      TabOrder = 0
      Text = '0'
      OnChange = EdtBiayaRacikChange
    end
  end
  object GbCari: TGroupBox
    Left = 8
    Top = 120
    Width = 500
    Height = 210
    Caption = ' Pencarian Obat (Searching) '
    TabOrder = 3
    object LblCari: TLabel
      Left = 12
      Top = 22
      Width = 89
      Height = 13
      Caption = 'Nama / Kode Obat:'
    end
    object LblHNama: TLabel
      Left = 12
      Top = 94
      Width = 70
      Height = 14
      Caption = 'Nama Obat : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblHHarga: TLabel
      Left = 12
      Top = 112
      Width = 70
      Height = 14
      Caption = 'Harga     : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblHStok: TLabel
      Left = 12
      Top = 130
      Width = 70
      Height = 14
      Caption = 'Sisa Stok : -'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object LblMetodeCari: TLabel
      Left = 12
      Top = 150
      Width = 476
      Height = 13
      AutoSize = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object LblJumlah: TLabel
      Left = 12
      Top = 178
      Width = 59
      Height = 13
      Caption = 'Jumlah Beli:'
    end
    object EdtCariObat: TEdit
      Left = 12
      Top = 38
      Width = 300
      Height = 21
      TabOrder = 0
      OnKeyPress = EdtCariObatKeyPress
    end
    object BtnCari: TButton
      Left = 320
      Top = 36
      Width = 168
      Height = 25
      Caption = 'Cari'
      TabOrder = 1
      OnClick = BtnCariClick
    end
    object CbBinarySearch: TCheckBox
      Left = 12
      Top = 68
      Width = 476
      Height = 17
      Caption = 'Gunakan Binary Search (nama persis, data harus terurut berdasarkan Nama)'
      TabOrder = 2
    end
    object EdtJumlahBeli: TEdit
      Left = 80
      Top = 174
      Width = 80
      Height = 21
      TabOrder = 3
    end
    object BtnTambahKeranjang: TButton
      Left = 168
      Top = 172
      Width = 160
      Height = 25
      Caption = 'Tambah ke Keranjang'
      TabOrder = 4
      OnClick = BtnTambahKeranjangClick
    end
    object BtnHapusItem: TButton
      Left = 336
      Top = 172
      Width = 152
      Height = 25
      Caption = 'Hapus Item Terpilih'
      TabOrder = 5
      OnClick = BtnHapusItemClick
    end
  end
  object GbKeranjang: TGroupBox
    Left = 8
    Top = 336
    Width = 500
    Height = 268
    Caption = ' Keranjang Belanja '
    TabOrder = 4
    object GridKeranjang: TStringGrid
      Left = 8
      Top = 18
      Width = 484
      Height = 240
      ColCount = 6
      DefaultRowHeight = 20
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVerticalLine, goFixedHorizontalLine, goVertLine, goHorzLine, goRowSelect]
      TabOrder = 0
    end
  end
  object GbTagihan: TGroupBox
    Left = 516
    Top = 48
    Width = 504
    Height = 250
    Caption = ' Konfirmasi dan Pembayaran '
    TabOrder = 5
    object LblTotalTagihan: TLabel
      Left = 12
      Top = 20
      Width = 160
      Height = 20
      Caption = 'Total Tagihan: Rp 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object LblTotalTag: TLabel
      Left = 12
      Top = 88
      Width = 98
      Height = 13
      Caption = 'Total Tagihan (Rp):'
    end
    object LblUangBayar: TLabel
      Left = 12
      Top = 154
      Width = 87
      Height = 13
      Caption = 'Uang Bayar (Rp):'
    end
    object LblKembalian: TLabel
      Left = 12
      Top = 184
      Width = 140
      Height = 20
      Caption = 'Kembalian: Rp 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object BtnKonfirmasi: TButton
      Left = 12
      Top = 48
      Width = 480
      Height = 28
      Caption = 'Hitung Tagihan dan Kunci Form'
      TabOrder = 0
      OnClick = BtnKonfirmasiClick
    end
    object EdtTotalTagihan: TEdit
      Left = 140
      Top = 84
      Width = 200
      Height = 21
      Color = clInfoBk
      ReadOnly = True
      TabOrder = 1
    end
    object BtnSetuju: TButton
      Left = 12
      Top = 114
      Width = 236
      Height = 28
      Caption = 'Pelanggan Setuju'
      TabOrder = 2
      OnClick = BtnSetujuClick
    end
    object BtnBatal: TButton
      Left = 256
      Top = 114
      Width = 236
      Height = 28
      Caption = 'Tidak Setuju / Batal'
      TabOrder = 3
      OnClick = BtnBatalClick
    end
    object EdtUangBayar: TEdit
      Left = 140
      Top = 150
      Width = 200
      Height = 21
      TabOrder = 4
    end
    object BtnProsesBayar: TButton
      Left = 348
      Top = 148
      Width = 144
      Height = 25
      Caption = 'Proses Bayar'
      TabOrder = 5
      OnClick = BtnProsesBayarClick
    end
    object BtnTransaksiBaru: TButton
      Left = 12
      Top = 214
      Width = 480
      Height = 26
      Caption = 'Pelanggan Berikutnya (Transaksi Baru)'
      TabOrder = 6
      OnClick = BtnTransaksiBaruClick
    end
  end
  object GbStruk: TGroupBox
    Left = 516
    Top = 304
    Width = 504
    Height = 300
    Caption = ' Struk Pembayaran '
    TabOrder = 6
    object MemoStruk: TMemo
      Left = 8
      Top = 18
      Width = 488
      Height = 274
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
end
