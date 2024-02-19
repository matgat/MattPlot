object frmGetString: TfrmGetString
  Left = 830
  Top = 230
  Hint = 'Insert a string then press enter'
  BorderStyle = bsNone
  ClientHeight = 21
  ClientWidth = 121
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  ShowHint = True
  PixelsPerInch = 96
  TextHeight = 13
  object edtGetString: TEdit
    Left = 0
    Top = 0
    Width = 121
    Height = 21
    TabOrder = 0
    OnKeyDown = edtGetStringKeyDown
  end
end
