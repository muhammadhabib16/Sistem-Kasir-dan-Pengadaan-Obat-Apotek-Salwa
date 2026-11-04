object FormBukaToko: TFormBukaToko
  Left = 300
  Top = 200
  BorderStyle = bsDialog
  Caption = 'Apotek Salwa - 1. Buka Toko'
  ClientHeight = 240
  ClientWidth = 440
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
    Width = 440
    Height = 40
    Align = alTop
    BevelOuter = bvNone
    Color = clNavy
    TabOrder = 0
    object LblNav: TLabel
      Left = 12
      Top = 12
      Width = 250
      Height = 16
      Caption = 'Menu Utama  >  1. Buka Toko'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object LblInfo: TLabel
    Left = 24
    Top = 56
    Width = 392
    Height = 30
    AutoSize = False
    Caption = 'Hitung sisa uang tunai di laci dari malam sebelumnya. Nilai ini menjadi modal kembalian dan dasar rekonsiliasi kas saat tutup toko.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object LblKasAwal: TLabel
    Left = 24
    Top = 100
    Width = 170
    Height = 13
    Caption = 'Masukkan Kas Awal Semalam (Rp):'
  end
  object EdtKasAwal: TEdit
    Left = 24
    Top = 118
    Width = 392
    Height = 21
    TabOrder = 1
  end
  object BtnBukaToko: TButton
    Left = 24
    Top = 156
    Width = 392
    Height = 32
    Caption = 'Buka Toko'
    Default = True
    TabOrder = 2
    OnClick = BtnBukaTokoClick
  end
  object BtnKembali: TButton
    Left = 24
    Top = 198
    Width = 392
    Height = 26
    Cancel = True
    Caption = '<< Kembali ke Menu Utama'
    ModalResult = 2
    TabOrder = 3
  end
end
