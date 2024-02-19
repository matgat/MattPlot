object frmLogViewer: TfrmLogViewer
  Left = 805
  Top = 539
  Width = 441
  Height = 348
  BorderStyle = bsSizeToolWin
  Caption = 'Log'
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object memLog: TMemo
    Left = 0
    Top = 0
    Width = 433
    Height = 296
    Align = alClient
    Color = clBlack
    Font.Charset = ANSI_CHARSET
    Font.Color = clLime
    Font.Height = -16
    Font.Name = 'Arial Narrow'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object pnlBottom: TPanel
    Left = 0
    Top = 296
    Width = 433
    Height = 18
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object trkTransparency: TTrackBar
      Left = 0
      Top = 0
      Width = 433
      Height = 18
      Align = alClient
      Max = 255
      Min = 10
      Orientation = trHorizontal
      PageSize = 5
      Frequency = 1
      Position = 10
      SelEnd = 0
      SelStart = 0
      TabOrder = 0
      ThumbLength = 15
      TickMarks = tmBottomRight
      TickStyle = tsNone
      OnChange = trkTransparencyChange
    end
  end
end
