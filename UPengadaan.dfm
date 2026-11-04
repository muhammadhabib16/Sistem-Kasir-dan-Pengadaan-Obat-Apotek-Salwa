object FormPengadaan: TFormPengadaan
  Left = 160
  Top = 80
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Apotek Salwa - 2. Pengadaan Obat'
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
      Width = 300
      Height = 16
      Caption = 'Menu Utama  >  2. Pengadaan Obat (Gudang)'
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
  object GbInputObat: TGroupBox
    Left = 8
    Top = 48
    Width = 300
    Height = 300
    Caption = ' Input Pasokan Obat Masuk '
    TabOrder = 1
    object LblKode: TLabel
      Left = 16
      Top = 32
      Width = 54
      Height = 13
      Caption = 'Kode Obat'
    end
    object LblNamaObat: TLabel
      Left = 16
      Top = 64
      Width = 56
      Height = 13
      Caption = 'Nama Obat'
    end
    object LblKategori: TLabel
      Left = 16
      Top = 96
      Width = 39
      Height = 13
      Caption = 'Kategori'
    end
    object LblHarga: TLabel
      Left = 16
      Top = 128
      Width = 85
      Height = 13
      Caption = 'Harga Satuan (Rp)'
    end
    object LblStok: TLabel
      Left = 16
      Top = 160
      Width = 76
      Height = 13
      Caption = 'Jumlah Pasokan'
    end
    object LblInfoPengadaan: TLabel
      Left = 16
      Top = 262
      Width = 270
      Height = 30
      AutoSize = False
      Caption = 'Kode yang sudah ada akan menambah stok. Klik baris tabel untuk mengisi form (restock).'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGray
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object EdtKode: TEdit
      Left = 112
      Top = 28
      Width = 170
      Height = 21
      CharCase = ecUpperCase
      TabOrder = 0
    end
    object EdtNamaObat: TEdit
      Left = 112
      Top = 60
      Width = 170
      Height = 21
      TabOrder = 1
    end
    object CmbKategori: TComboBox
      Left = 112
      Top = 92
      Width = 170
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 2
    end
    object EdtHarga: TEdit
      Left = 112
      Top = 124
      Width = 170
      Height = 21
      TabOrder = 3
    end
    object EdtStok: TEdit
      Left = 112
      Top = 156
      Width = 170
      Height = 21
      TabOrder = 4
    end
    object BtnSimpanStok: TButton
      Left = 16
      Top = 192
      Width = 266
      Height = 30
      Caption = 'Simpan Stok'
      TabOrder = 5
      OnClick = BtnSimpanStokClick
    end
    object BtnBersihObat: TButton
      Left = 16
      Top = 228
      Width = 266
      Height = 25
      Caption = 'Bersihkan Form'
      TabOrder = 6
      OnClick = BtnBersihObatClick
    end
  end
  object GbUrut: TGroupBox
    Left = 8
    Top = 356
    Width = 300
    Height = 248
    Caption = ' Pengurutan Data (Sorting) '
    TabOrder = 2
    object LblKriteria: TLabel
      Left = 16
      Top = 22
      Width = 103
      Height = 13
      Caption = 'Urutkan berdasarkan kolom:'
    end
    object LblHasilUrut: TLabel
      Left = 16
      Top = 198
      Width = 266
      Height = 44
      AutoSize = False
      Caption = 'Data belum diurutkan.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object CmbKriteria: TComboBox
      Left = 16
      Top = 38
      Width = 266
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
    end
    object RgArah: TRadioGroup
      Left = 16
      Top = 66
      Width = 266
      Height = 44
      Caption = ' Arah Urutan '
      Columns = 2
      ItemIndex = 0
      Items.Strings = (
        'Ascending'
        'Descending')
      TabOrder = 1
    end
    object RgAlgoritma: TRadioGroup
      Left = 16
      Top = 114
      Width = 266
      Height = 44
      Caption = ' Algoritma '
      Columns = 2
      ItemIndex = 0
      Items.Strings = (
        'Selection Sort'
        'Bubble Sort')
      TabOrder = 2
    end
    object BtnUrutkan: TButton
      Left = 16
      Top = 164
      Width = 266
      Height = 28
      Caption = 'Urutkan'
      TabOrder = 3
      OnClick = BtnUrutkanClick
    end
  end
  object GbDaftarObat: TGroupBox
    Left = 316
    Top = 48
    Width = 704
    Height = 556
    Caption = ' Daftar Stok Obat Gudang '
    TabOrder = 3
    object LblKeteranganStok: TLabel
      Left = 8
      Top = 530
      Width = 400
      Height = 13
      Caption = 'Oranye = KRITIS (stok <= 10, perlu restock)     Merah = HABIS'
    end
    object GridStok: TStringGrid
      Left = 8
      Top = 20
      Width = 688
      Height = 500
      ColCount = 7
      DefaultRowHeight = 20
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVerticalLine, goFixedHorizontalLine, goVertLine, goHorzLine, goRowSelect]
      TabOrder = 0
      OnClick = GridStokClick
      OnDrawCell = GridStokDrawCell
    end
  end
end
