object frmMplotMain: TfrmMplotMain
  Left = 857
  Top = 200
  Width = 398
  Height = 250
  Caption = 'MatPlot'
  Color = clWindow
  Constraints.MinHeight = 250
  Constraints.MinWidth = 200
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Microsoft Sans Serif'
  Font.Style = []
  Menu = mnuMain
  OldCreateOrder = False
  Position = poDefault
  Scaled = False
  ShowHint = True
  OnDeactivate = FormDeactivate
  OnKeyDown = FormKeyDown
  OnKeyUp = FormKeyUp
  PixelsPerInch = 96
  TextHeight = 16
  object splBottomDock: TSplitter
    Left = 0
    Top = 158
    Width = 390
    Height = 3
    Cursor = crVSplit
    Align = alBottom
  end
  object splRightDock: TSplitter
    Left = 387
    Top = 29
    Width = 3
    Height = 129
    Cursor = crHSplit
    Align = alRight
  end
  object splLeftDock: TSplitter
    Left = 0
    Top = 29
    Width = 3
    Height = 129
    Cursor = crHSplit
  end
  object barStatus: TStatusBar
    Left = 0
    Top = 178
    Width = 390
    Height = 21
    BorderWidth = 1
    Constraints.MinHeight = 10
    Constraints.MinWidth = 10
    Panels = <
      item
        Alignment = taCenter
        Width = 150
      end
      item
        Alignment = taCenter
        Width = 50
      end
      item
        Alignment = taCenter
        Width = 50
      end
      item
        Width = 50
      end>
    ParentColor = True
    ParentFont = True
    SimplePanel = False
    UseSystemFont = False
    OnClick = barStatusClick
  end
  object tobMain: TToolBar
    Left = 0
    Top = 0
    Width = 390
    Height = 29
    AutoSize = True
    ButtonHeight = 24
    ButtonWidth = 24
    EdgeBorders = [ebTop, ebBottom]
    Images = imlActions
    Indent = 4
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
  end
  object pnlPlotArea: TPanel
    Left = 3
    Top = 29
    Width = 384
    Height = 129
    Align = alClient
    BevelOuter = bvNone
    UseDockManager = False
    ParentColor = True
    ParentShowHint = False
    ShowHint = False
    TabOrder = 1
  end
  object barProgress: TProgressBar
    Left = 0
    Top = 161
    Width = 390
    Height = 17
    Align = alBottom
    Constraints.MinHeight = 10
    Constraints.MinWidth = 10
    Min = 0
    Max = 100
    TabOrder = 2
    Visible = False
  end
  object ctrRightDock: TPanel
    Left = 387
    Top = 29
    Width = 0
    Height = 129
    Align = alRight
    BevelOuter = bvNone
    DockSite = True
    ParentColor = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 4
    OnDockDrop = ctrRightDockDockDrop
    OnUnDock = ctrRightDockUnDock
  end
  object ctrBottomDock: TPanel
    Left = 0
    Top = 158
    Width = 390
    Height = 0
    Align = alBottom
    BevelOuter = bvNone
    DockSite = True
    ParentColor = True
    ParentShowHint = False
    ShowHint = False
    TabOrder = 5
    OnDockDrop = ctrRightDockDockDrop
    OnUnDock = ctrRightDockUnDock
  end
  object ctrPane: TPanel
    Left = 236
    Top = 40
    Width = 100
    Height = 82
    BevelOuter = bvNone
    DragKind = dkDock
    DragMode = dmAutomatic
    ParentColor = True
    TabOrder = 6
    Visible = False
    object grdPane: TStringGrid
      Left = 0
      Top = 0
      Width = 100
      Height = 82
      Align = alClient
      ColCount = 2
      Constraints.MinHeight = 10
      Constraints.MinWidth = 10
      DefaultRowHeight = 20
      DragKind = dkDock
      RowCount = 1
      FixedRows = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
      TabOrder = 0
      OnDrawCell = grdPaneDrawCell
      OnMouseDown = grdPaneMouseDown
      ColWidths = (
        54
        128)
      RowHeights = (
        20)
    end
  end
  object ctrLog: TPanel
    Left = 124
    Top = 40
    Width = 100
    Height = 85
    BevelOuter = bvNone
    DragKind = dkDock
    DragMode = dmAutomatic
    ParentColor = True
    TabOrder = 7
    Visible = False
    object memLog: TMemo
      Left = 0
      Top = 0
      Width = 100
      Height = 85
      Align = alClient
      Color = clBlack
      Constraints.MinHeight = 10
      Constraints.MinWidth = 10
      DragKind = dkDock
      Font.Charset = ANSI_CHARSET
      Font.Color = clLime
      Font.Height = -13
      Font.Name = 'Lucida Sans Unicode'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
      WordWrap = False
    end
  end
  object ctrLeftDock: TPanel
    Left = 3
    Top = 29
    Width = 0
    Height = 129
    Align = alLeft
    BevelOuter = bvNone
    DockSite = True
    ParentColor = True
    ParentShowHint = False
    ShowHint = True
    TabOrder = 8
    OnDockDrop = ctrRightDockDockDrop
    OnUnDock = ctrRightDockUnDock
  end
  object ctrTree: TPanel
    Left = 12
    Top = 40
    Width = 100
    Height = 82
    BevelOuter = bvNone
    Constraints.MinHeight = 10
    Constraints.MinWidth = 10
    DragKind = dkDock
    DragMode = dmAutomatic
    ParentColor = True
    TabOrder = 9
    Visible = False
    object treePlot: TTreeView
      Left = 0
      Top = 0
      Width = 100
      Height = 82
      Align = alClient
      DragMode = dmAutomatic
      HideSelection = False
      Images = imlTree
      Indent = 19
      ReadOnly = True
      RightClickSelect = True
      TabOrder = 0
      OnDragDrop = treePlotDragDrop
      OnDragOver = treePlotDragOver
      OnMouseDown = treePlotMouseDown
    end
  end
  object aclMain: TActionList
    Images = imlActions
    OnUpdate = aclMainUpdate
    Left = 43
    Top = 1
    object actShowTree: TAction
      Tag = 4
      Category = 'Tools'
      Caption = 'Show tree'
      Hint = 'Show plot frames and tracks tree'
      ImageIndex = 75
      OnExecute = actShowTreeExecute
    end
    object actStayOnTop: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Stay on top'
      Hint = 'Force form in foreground'
      ImageIndex = 22
      OnExecute = actStayOnTopExecute
    end
    object actRefresh: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Refresh'
      Hint = 'Refresh'
      ImageIndex = 24
      OnExecute = actRefreshExecute
    end
    object actShowPane: TAction
      Tag = 3
      Category = 'Tools'
      Caption = 'Show pane'
      Hint = 'Show/hide values pane'
      ImageIndex = 76
      OnExecute = actShowPaneExecute
    end
    object actAbort: TAction
      Caption = 'Abort'
      Hint = 'Abort a lengthy operation (typically reading files)'
      ImageIndex = 25
      OnExecute = actAbortExecute
    end
    object actShowLog: TAction
      Tag = 3
      Category = 'Tools'
      Caption = 'Show log'
      Hint = 'Toggle Log window'
      ImageIndex = 78
      OnExecute = actShowLogExecute
    end
    object actSnapshot: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Snapshot'
      Hint = 'Take quickly a plot image'
      ImageIndex = 26
      OnExecute = actSnapshotExecute
    end
    object actAddFrame: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Add frame'
      Hint = 'Add frame'
      ImageIndex = 32
      OnExecute = actAddFrameExecute
    end
    object actRemoveFrame: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Remove frame'
      Hint = 'Remove frame'
      ImageIndex = 33
      OnExecute = actRemoveFrameExecute
    end
    object actClearFrame: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Clear frame'
      Hint = 'Remove all contained tracks in current frame'
      ImageIndex = 35
      OnExecute = actClearFrameExecute
    end
    object actFrameProperties: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Frame properties'
      Hint = 'Edit frame properties'
      ImageIndex = 34
      OnExecute = actFramePropertiesExecute
    end
    object actAddMarker: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Add marker'
      Hint = 'Add a line passing through a given value'
      ImageIndex = 36
      OnExecute = actAddMarkerExecute
    end
    object actAddText: TAction
      Category = 'Frame'
      Caption = 'Add text'
      Hint = 'Add a text label in a given position'
      ImageIndex = 37
      OnExecute = actAddTextExecute
    end
    object actLockX: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Lock x axes'
      Hint = 'Synchronize x axes'
      ImageIndex = 30
      OnExecute = actLockXExecute
    end
    object actLockY: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Lock y axes'
      Hint = 'Synchronize y axes'
      ImageIndex = 31
      OnExecute = actLockYExecute
    end
    object actClearAll: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Clear All'
      Hint = 'Clear all tracks'
      ImageIndex = 23
      OnExecute = actClearAllExecute
    end
    object actUpdate: TAction
      Tag = 2
      Category = 'Help'
      Caption = 'Update'
      Hint = 'Check for updates'
      ImageIndex = 80
      OnExecute = actUpdateExecute
    end
    object actDemo: TAction
      Tag = 2
      Category = 'Help'
      Caption = 'Demo'
      Hint = 'A demonstration of MatPlot'
      ImageIndex = 81
      OnExecute = actDemoExecute
    end
    object actExperimentalStuff: TAction
      Tag = 2
      Category = 'Help'
      Caption = 'Experimental stuff'
      Hint = 'Experimental stuff'
      ImageIndex = 82
      OnExecute = actExperimentalStuffExecute
    end
    object actOpen: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Open'
      Hint = 'Browse for a plot or data file'
      ImageIndex = 0
      OnExecute = actOpenExecute
    end
    object actSave: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Save'
      Enabled = False
      Hint = 'Save current plot'
      ImageIndex = 1
      OnExecute = actSaveExecute
    end
    object actExport: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Export'
      Hint = 'Export plot to...'
      ImageIndex = 2
      OnExecute = actExportExecute
    end
    object actAddTrack: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Add track'
      Hint = 'Add track'
      ImageIndex = 50
      OnExecute = actAddTrackExecute
    end
    object actRemoveTrack: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Remove track'
      Hint = 'Remove track'
      ImageIndex = 51
      OnExecute = actRemoveTrackExecute
    end
    object actPrint: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Print'
      Hint = 'Print current plot'
      ImageIndex = 8
      OnExecute = actPrintExecute
    end
    object actExportFrameToText: TAction
      Tag = 5
      Caption = 'Plain text'
      Hint = 'Export frame data to ASCII text'
      ImageIndex = 6
      OnExecute = actExportFrameToTextExecute
    end
    object actHelpContents: THelpContents
      Tag = 2
      Category = 'Help'
      Caption = 'Contents'
      Hint = 'Open help'
      ImageIndex = 84
      OnExecute = actHelpContentsExecute
    end
    object actWhatsNew: TAction
      Category = 'Help'
      Caption = 'What'#39's new'
      Hint = 'See what'#39's new in this version'
      ImageIndex = 85
      OnExecute = actWhatsNewExecute
    end
    object actAbout: TAction
      Tag = 2
      Category = 'Help'
      Caption = 'About'
      Hint = 'Informations about this program'
      ImageIndex = 86
      OnExecute = actAboutExecute
    end
    object actTrackProperties: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Track properties'
      Hint = 'Edit track properties'
      ImageIndex = 52
      OnExecute = actTrackPropertiesExecute
    end
    object actTrackData: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Edit track data'
      Hint = 'Edit track data'
      ImageIndex = 53
      OnExecute = actTrackDataExecute
    end
    object actExportFrameToMatlab: TAction
      Tag = 5
      Caption = 'Matlab'
      Hint = 'Pass current frame to Matlab'
      ImageIndex = 7
      OnExecute = actExportFrameToMatlabExecute
    end
    object actEditOptions: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Edit Options'
      Hint = 'Edit options'
      ImageIndex = 10
      OnExecute = actEditOptionsExecute
    end
    object actGotoOrigin: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Go to origin'
      Hint = 'Return to axes origin'
      ImageIndex = 43
      OnExecute = actGotoOriginExecute
    end
    object actFitToAxes: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Fit to tracks and axes'
      Hint = 'Change scale to fit contained tracks and axes'
      ImageIndex = 40
      OnExecute = actFitToAxesExecute
    end
    object actFitToTracks: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Fit to tracks'
      Hint = 'Change scale to fit contained tracks'
      ImageIndex = 41
      OnExecute = actFitToTracksExecute
    end
    object actFitToTrack: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Fit to track'
      Hint = 'Change scale to fit current track'
      ImageIndex = 42
      OnExecute = actFitToTrackExecute
    end
    object actToggleScaleX: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'X logarithmic scale'
      Hint = 'Toggle linear/logarithmic scale for X'
      ImageIndex = 44
      OnExecute = actToggleScaleXExecute
    end
    object actToggleScaleY: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Y logarithmic scale'
      Hint = 'Toggle linear/logarithmic scale for Y'
      ImageIndex = 45
      OnExecute = actToggleScaleYExecute
    end
    object actLegendToggle: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Toggle legend'
      Hint = 'Toggle frame legend'
      ImageIndex = 46
      OnExecute = actLegendToggleExecute
    end
    object actGridToggle: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Toggle grid'
      Hint = 'Toggle grid (frame)'
      ImageIndex = 47
      OnExecute = actGridToggleExecute
    end
    object actGridMore: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Grid +'
      Hint = 'Shrink grid lines'
      ImageIndex = 48
      OnExecute = actGridMoreExecute
    end
    object actGridLess: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Grid -'
      Hint = 'Expand grid lines'
      ImageIndex = 49
      OnExecute = actGridLessExecute
    end
    object actSelectMode: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Select mode'
      Hint = 'Select frame and tracks, edit style and layout'
      ImageIndex = 63
      OnExecute = actSelectModeExecute
    end
    object actEditMode: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Edit mode'
      Hint = 'Edit data and crop tool'
      ImageIndex = 64
      OnExecute = actEditModeExecute
    end
    object actPanMode: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Pan mode'
      Hint = 'Translate plot origin'
      ImageIndex = 65
      OnExecute = actPanModeExecute
    end
    object actZoomMode: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Zoom mode'
      Hint = 'Change axes scale'
      ImageIndex = 66
      OnExecute = actZoomModeExecute
    end
    object actMeasureMode: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Measure mode'
      Hint = 'Measure selected track quantities'
      ImageIndex = 67
      OnExecute = actMeasureModeExecute
    end
    object actTrackStatistics: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Statistics'
      Hint = 'Calculate statistics for this viewed interval'
      ImageIndex = 61
      OnExecute = actTrackStatisticsExecute
    end
    object actPenMode: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Pen Mode'
      Hint = 'A pencil to scribble on plot'
      ImageIndex = 68
      OnExecute = actPenModeExecute
    end
    object actCompute: TAction
      Tag = 3
      Category = 'Tools'
      Caption = 'Compute'
      Hint = 'Elaborate existing tracks'
      ImageIndex = 77
      OnExecute = actComputeExecute
    end
    object actReadOptions: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Read options'
      Hint = 'Reload options from MatPlot ini file'
      ImageIndex = 11
      OnExecute = actReadOptionsExecute
    end
    object actStoreOptions: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Store options'
      Hint = 'Save options in MatPlot ini file'
      ImageIndex = 12
      OnExecute = actStoreOptionsExecute
    end
    object actSetDefaults: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Set defaults'
      Hint = 'Back to default options'
      ImageIndex = 14
      OnExecute = actSetDefaultsExecute
    end
    object actEasterEgg: TAction
      Caption = 'EasterEgg'
      Hint = 'Hello!'
      ImageIndex = 87
      Visible = False
      OnExecute = actEasterEggExecute
    end
    object actZoomIn: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Zoom+'
      Hint = 
        'Better use zoom mode) mouse wheel, mouse left double click or mo' +
        'use center click'
      ImageIndex = 38
      OnExecute = actZoomInExecute
    end
    object actZoomOut: TAction
      Tag = 5
      Category = 'Frame'
      Caption = 'Zoom-'
      Hint = 'Better use zoom mode mouse wheel or mouse right click'
      ImageIndex = 39
      OnExecute = actZoomOutExecute
    end
    object actTrackTransform: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Transform track'
      Hint = 'Elaborate current track'
      ImageIndex = 54
      OnExecute = actTrackTransformExecute
    end
    object actMarkPoints: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Mark Points'
      Hint = 'View track points'
      ImageIndex = 57
      OnExecute = actMarkPointsExecute
    end
    object actShowCoords: TAction
      Category = 'Track'
      Caption = 'Show coordinates'
      Hint = 'Show point coordinates'
      ImageIndex = 58
      OnExecute = actShowCoordsExecute
    end
    object actStrokeLines: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Stroke track lines'
      Hint = 'Draw linear interpolation of track points'
      ImageIndex = 56
      OnExecute = actStrokeLinesExecute
    end
    object actFillArea: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Fill area'
      Hint = 'Hatch track area'
      ImageIndex = 59
      OnExecute = actFillAreaExecute
    end
    object actDrawHistograms: TAction
      Tag = 6
      Category = 'Track'
      Caption = 'Draw histograms'
      Hint = 'Draw histograms or stems'
      ImageIndex = 60
      OnExecute = actDrawHistogramsExecute
    end
    object actMeasureAbscissa: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Measure abscissa'
      Hint = 'View abscissa line in measure mode'
      ImageIndex = 72
      OnExecute = actMeasureAbscissaExecute
    end
    object actMeasureOrdinate: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Measure ordinate'
      Hint = 'Measure track ordinate'
      ImageIndex = 73
      OnExecute = actMeasureOrdinateExecute
    end
    object actMeasureSlope: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Measure slope'
      Hint = 'Enable track slope measure'
      ImageIndex = 74
      OnExecute = actMeasureSlopeExecute
    end
    object actTextEditor: TAction
      Tag = 3
      Category = 'Tools'
      Caption = 'TextEditor'
      Hint = 'Open the text editor'
      ImageIndex = 79
      OnExecute = actTextEditorExecute
    end
    object actSetStylePlain: TAction
      Tag = 1
      Category = 'File'
      Caption = 'Plain style'
      Hint = 'Set plain plot style, good for printing'
      ImageIndex = 15
      OnExecute = actSetStylePlainExecute
    end
    object actSetStylePSpice: TAction
      Tag = 1
      Category = 'File'
      Caption = 'PSpice style'
      Hint = 'An example of plot customization, store and see ini file'
      ImageIndex = 16
      OnExecute = actSetStylePSpiceExecute
    end
    object actExit: TAction
      Caption = 'Exit'
      Hint = 'Quit program'
      ImageIndex = 17
      OnExecute = actExitExecute
    end
    object actViewMeasureMarkers: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'View measure markers'
      Hint = 'Create measure markers'
      ImageIndex = 71
      OnExecute = actViewMeasureMarkersExecute
    end
    object actProperties: TAction
      Tag = 4
      Category = 'Plot'
      Caption = 'Properties'
      Hint = 'Edit clicked object properties'
      ImageIndex = 28
      OnExecute = actPropertiesExecute
    end
    object actTrySomething: TAction
      Caption = 'Try something'
      Hint = 'An auxiliary action'
      ImageIndex = 69
      OnExecute = actTrySomethingExecute
    end
    object actCopyGrid: TAction
      Category = 'Edit'
      Caption = 'Copy cells'
      Hint = 'Copy selected grid quantities to clipboard'
      ImageIndex = 20
      OnExecute = actCopyGridExecute
    end
    object actRemove: TAction
      Category = 'Plot'
      Caption = 'Remove'
      Hint = 'Remove clicked object'
      ImageIndex = 29
      OnExecute = actRemoveExecute
    end
    object actExecMacro: TAction
      Category = 'Plot'
      Caption = 'Execute macro'
      Hint = 'Execute macro'
      ImageIndex = 83
      OnExecute = actExecMacroExecute
    end
  end
  object mnuMain: TMainMenu
    Images = imlActions
    Left = 77
    Top = 1
    object mnuFile: TMenuItem
      Caption = 'File'
      object mnuOpen: TMenuItem
        Action = actOpen
      end
      object mnuSave: TMenuItem
        Action = actSave
      end
      object mnuSaveBitmapAs: TMenuItem
        Action = actExport
      end
      object mnuPrint: TMenuItem
        Action = actPrint
      end
      object mnuSep00: TMenuItem
        Caption = '-'
      end
      object mnuEditOptions: TMenuItem
        Action = actEditOptions
      end
      object mnuLoadOptions: TMenuItem
        Action = actReadOptions
      end
      object mnuSaveOptions: TMenuItem
        Action = actStoreOptions
      end
      object mnySetStyles: TMenuItem
        Caption = 'Set styles'
        ImageIndex = 13
        object mnuSetDefaults: TMenuItem
          Action = actSetDefaults
        end
        object mnuSetStylePlain: TMenuItem
          Action = actSetStylePlain
        end
        object mnuSetStylePSpice: TMenuItem
          Action = actSetStylePSpice
        end
      end
      object mnuSep01: TMenuItem
        Caption = '-'
      end
      object mnuExit: TMenuItem
        Action = actExit
      end
    end
    object mnuPlot: TMenuItem
      Caption = 'Plot'
      object mnuMode: TMenuItem
        Caption = 'Mode'
        Hint = 'Choose mode'
        ImageIndex = 62
        object mnuEditMode: TMenuItem
          Action = actSelectMode
          GroupIndex = 1
        end
        object mnuPanMode: TMenuItem
          Action = actPanMode
          GroupIndex = 1
        end
        object mnuZoomMode: TMenuItem
          Action = actZoomMode
          GroupIndex = 1
        end
        object mnuMeasureMode: TMenuItem
          Action = actMeasureMode
          GroupIndex = 1
        end
        object mnuPenMode: TMenuItem
          Action = actPenMode
          GroupIndex = 1
        end
      end
      object mnuMeasureLines: TMenuItem
        Caption = 'Measure lines'
        ImageIndex = 71
        object mnuMeasureAbscissa: TMenuItem
          Action = actMeasureAbscissa
        end
        object mnuMeasureOrdinate: TMenuItem
          Action = actMeasureOrdinate
        end
        object mnuMeasureSlope: TMenuItem
          Action = actMeasureSlope
        end
        object mnuMeasureMarkers: TMenuItem
          Action = actViewMeasureMarkers
        end
      end
      object mnuObject: TMenuItem
        Caption = 'Object'
        ImageIndex = 27
        object mnuProperties: TMenuItem
          Action = actProperties
        end
        object mnuRemove: TMenuItem
          Action = actRemove
        end
      end
      object mnuSep10: TMenuItem
        Caption = '-'
      end
      object mnuPlotAdd: TMenuItem
        Caption = 'Add'
        ImageIndex = 5
        object mnuPlotAddFrame: TMenuItem
          Action = actAddFrame
        end
      end
      object mnuScreenShoot: TMenuItem
        Action = actSnapshot
      end
      object mnuSep11: TMenuItem
        Caption = '-'
      end
      object mnuSyncX: TMenuItem
        Action = actLockX
      end
      object mnuSyncY: TMenuItem
        Action = actLockY
      end
      object mnuSep12: TMenuItem
        Caption = '-'
      end
      object mnuStayOnTop: TMenuItem
        Action = actStayOnTop
      end
      object mnuClearAll: TMenuItem
        Action = actClearAll
      end
      object mnuRefresh: TMenuItem
        Action = actRefresh
      end
      object mnuExecMacro: TMenuItem
        Action = actExecMacro
      end
    end
    object mnuFrame: TMenuItem
      Caption = 'Frame'
      object mnuAddTracks: TMenuItem
        Action = actAddFrame
      end
      object mnuRemoveframe: TMenuItem
        Action = actRemoveFrame
      end
      object mnuFrameStyle: TMenuItem
        Action = actFrameProperties
      end
      object mnuClearFrame: TMenuItem
        Action = actClearFrame
      end
      object mnuSep20: TMenuItem
        Caption = '-'
      end
      object mnuAddToFrame: TMenuItem
        Caption = 'Add'
        ImageIndex = 5
        object mnuAddMarker: TMenuItem
          Action = actAddMarker
        end
        object mnuAddText: TMenuItem
          Action = actAddText
        end
        object mnuFrameAddTrack: TMenuItem
          Action = actAddTrack
        end
      end
      object mnuExport: TMenuItem
        Caption = 'Export to'
        Hint = 'Export current plot'
        ImageIndex = 4
        object mnuPlainText: TMenuItem
          Action = actExportFrameToText
        end
        object mnuToMatlab: TMenuItem
          Action = actExportFrameToMatlab
        end
      end
      object mnuSep21: TMenuItem
        Caption = '-'
      end
      object mnuViewAxes: TMenuItem
        Action = actGotoOrigin
      end
      object mnuFitToAxes: TMenuItem
        Action = actFitToAxes
      end
      object mnuResetView: TMenuItem
        Action = actFitToTracks
      end
      object mnuFitToTrack: TMenuItem
        Action = actFitToTrack
      end
      object mnuSep23: TMenuItem
        Caption = '-'
      end
      object mnuZoomP: TMenuItem
        Action = actZoomIn
      end
      object mnuZoomM: TMenuItem
        Action = actZoomOut
      end
      object mnuSep24: TMenuItem
        Caption = '-'
      end
      object mnuToggleLegend: TMenuItem
        Action = actLegendToggle
      end
      object mnuTogglegrid: TMenuItem
        Action = actGridToggle
      end
      object mnuGridThick: TMenuItem
        Action = actGridMore
      end
      object mnuGridThin: TMenuItem
        Action = actGridLess
      end
      object mnuSep25: TMenuItem
        Caption = '-'
      end
      object mnuToggleXScale: TMenuItem
        Action = actToggleScaleX
      end
      object mnuToggleYScale: TMenuItem
        Action = actToggleScaleY
      end
    end
    object mnuTrack: TMenuItem
      Caption = 'Track'
      object mnuAddtrack: TMenuItem
        Action = actAddTrack
      end
      object mnuRemovetrack: TMenuItem
        Action = actRemoveTrack
      end
      object mnuTrackStyle: TMenuItem
        Action = actTrackProperties
      end
      object mnuTrackPlottingStyle: TMenuItem
        Caption = 'Plotting style'
        ImageIndex = 55
        object mnuMarkPoints: TMenuItem
          Action = actMarkPoints
        end
        object mnuShowCoords: TMenuItem
          Action = actShowCoords
        end
        object mnuStrokeLines: TMenuItem
          Action = actStrokeLines
        end
        object mniFillArea: TMenuItem
          Action = actFillArea
        end
        object mnuDrawHistograms: TMenuItem
          Action = actDrawHistograms
        end
      end
      object mnuSep30: TMenuItem
        Caption = '-'
      end
      object mnuTrackData: TMenuItem
        Action = actTrackData
      end
      object mnuTrackTransform: TMenuItem
        Action = actTrackTransform
      end
      object mnuSep31: TMenuItem
        Caption = '-'
      end
      object mnuStatistics: TMenuItem
        Action = actTrackStatistics
      end
    end
    object mnuTools: TMenuItem
      Caption = 'Tools'
      object mnuShowTree: TMenuItem
        Action = actShowTree
      end
      object mnuShowdisplay: TMenuItem
        Action = actShowPane
      end
      object mnuShowLog: TMenuItem
        Action = actShowLog
      end
      object mnuSep40: TMenuItem
        Caption = '-'
      end
      object mnuElaborate: TMenuItem
        Action = actCompute
      end
      object mnuTextEditor: TMenuItem
        Action = actTextEditor
      end
      object mnuSep41: TMenuItem
        Caption = '-'
      end
      object mnuAbort: TMenuItem
        Action = actAbort
      end
    end
    object mnuHelp: TMenuItem
      Caption = 'Help'
      object mnuDemo: TMenuItem
        Action = actDemo
      end
      object mnuExperimentalStuff: TMenuItem
        Action = actExperimentalStuff
      end
      object mnuUpdate: TMenuItem
        Action = actUpdate
      end
      object mnuSep50: TMenuItem
        Caption = '-'
      end
      object mnuHelpContents: TMenuItem
        Action = actHelpContents
      end
      object mnuWhatsnew: TMenuItem
        Action = actWhatsNew
      end
      object mnuAbout: TMenuItem
        Action = actAbout
      end
    end
  end
  object imlActions: TImageList
    Left = 112
    Top = 1
    Bitmap = {
      494C010158005900040010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000007001000001002000000000000070
      0100000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000EFE7
      DE00BDBDB5006B6363004A4A4A004A4A42004A4A42004A4A4200524A4A007B73
      7300CEC6C600F7EFE70000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000848484006B6B6B006B6B6B006B6B6B006B6B6B00737373000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E0EAEA0095B8B90075A5A60074A5A6008EB4B500D5E2E2000000
      0000000000000000000000000000000000000000000000000000E7DED600ADA5
      A500E7CEC600F7E7D600F7E7DE00F7E7DE00F7E7DE00F7E7D600CEBDAD005A52
      52006B6B6B00B5ADA500EFE7E700000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C6BD
      B500BD9C8400DEB58C00DEB58C00DEB58C00DEB58C00D6B58C00B59C7B007373
      6B00737373000000000000000000000000000000000000000000000000000000
      0000CFDEDE006DA1A20075ACAD0079B0B10079B0B10077ADAE006A9FA000BDD2
      D2000000000000000000000000000000000000000000EFE7E700CEBDB500F7E7
      DE00FFFFFF00E7E7E700CEB5A500D6AD9400DEC6BD00EFF7F700F7F7F700F7E7
      D6008C7B7B006B636300BDB5AD00F7EFEF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CEBDAD00DEBD
      9400E7BD9400EFC69C00F7D6A500F7D6AD00F7D6A500EFC69C00E7BD9400D6B5
      8C008C7B73006B6B6B000000000000000000000000000000000000000000E0E9
      E90071A4A5007EB4B50082B8B90084BABA0085BABB0084BABA0082B8B8006FA4
      A500CFDEDE0000000000000000000000000000000000DECEC600F7EFE700F7FF
      FF00CE9C8400B54A1000BD633900DEAD9C00C65A2100BD522100D6AD9C00F7FF
      FF00F7E7DE00736B6B00736B6B00DED6CE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000E7BD9C00E7BD9400EFC6
      9C00FFDEB500BD8C6B008C422900B55A3100AD5A2900C6946B00FFDEB500EFC6
      9C00DEBD9400847B73007B7B7B00000000000000000000000000FEFFFF008CB4
      B40086BCBD0090C4C40093C7C70096C9C90096CACA0096C9C90093C7C7008DC1
      C10084B0B100F9FBFB000000000000000000EFDEDE00F7E7D600FFFFFF00C67B
      5A00BD420800C64A1000C6A5940000000000DE947300C64A1000BD420800CE9C
      8400FFFFFF00F7E7D60052525200949494000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000E7BD9C00EFCEA500FFDE
      B500FFDEB500F7E7C6008452420084100000AD5A3100F7E7BD00FFDEB500FFDE
      B500EFCEA500D6B594006B6B6B00000000000000000000000000D3E1E10080B5
      B60099CCCB009BCECE009ED1D1009FD2D2009FD2D3009FD1D2009DD0D1009CCF
      CF0088BCBD00C0D6D6000000000000000000EFDED600FFFFFF00DEAD9400BD4A
      0800CE632900CE632900CE6B3900DE8C6B00CE5A2900CE632900CE5A2900BD4A
      1000E7CEBD00F7EFEF00A5949400635A5A0074BFFF000B8FFF0074BFFF0069BA
      FF00008AFF0069BAFF00068CFF00259AFF0038A3FF0072BEFF00098EFF0067B9
      FF0069BAFF00008AFF0069BAFF0000000000E7BD9C00EFCEAD00FFDEBD00FFE7
      BD00FFDEBD00FFF7CE00946B520084100000B56B4200FFEFCE00FFDEBD00FFE7
      BD00FFDEB500EFCEAD009C8C7B00949494000000000000000000AECCCC0097C9
      CA00A0D2D300A2D4D500A4D7D800A6D8D900A6D8D900A5D8D900A4D6D600A1D4
      D4009BCFCF009EC4C4000000000000000000F7E7D600FFFFFF00C6633100CE5A
      2100CE6B3100CE5A2100CE8C6B00F7E7DE00D66B3900CE5A2100CE633100C652
      1800C67B5200FFFFFF00E7CEC6004A4A4A000B8FFF00F4FAFF000B8FFF0077C1
      FF00FEFFFE0077C1FF00F9FCFF00DAEEFF00C7E5FF00098EFF00FEFFFE0098D0
      FF006FBDFF00FEFFFE00008AFF0000000000EFCEB500F7D6BD00FFE7C600FFE7
      C600FFE7C600FFF7D600946B5A0084100000B56B4200FFEFD600FFE7C600FFE7
      C600FFE7C600F7D6BD00C6AD94008484840000000000000000009BC3C300A0D2
      D300A4D6D700A7DADB00AADDDE00ACDFE000ACDFE000ACDEE000AADCDE00A7D9
      DA00A3D6D70096C3C3000000000000000000F7E7DE00F7DED600C6521800CE63
      3100CE633100CE5A2100C6846B00FFFFFF00EFAD9400C64A1000CE632900CE63
      2900C65A2900F7F7F700F7DED6004A4A4A000B8FFF00F4FAFF000B8FFF00EEF7
      FF00F4FAFF0077C1FF00E8F4FF001794FF0038A3FF00038BFF00F9FCFF00B7DE
      FF00AAD8FF00FBFDFF00279CFF00AED9FF00EFCEB500FFE7C600FFEFCE00FFE7
      CE00FFE7CE00FFFFE7009473630084100000B56B4A00FFF7DE00FFE7C600FFE7
      C600FFEFD600FFE7CE00CEB5940084848400000000000000000095C2C200A4D7
      D800A9DBDD00ACDFE100B1E2E500B5E4E700B6E4E700B4E3E600AFE2E400ACDE
      E000A9DBDC0097C6C6000000000000000000F7E7DE00F7DECE00CE5A2100CE63
      3100CE633100CE632900C6522100CEB5AD00FFFFFF00E79C7B00C6521800CE63
      2900C65A2900F7F7EF00EFDED600524A4A000B8FFF00F4FAFF0099D0FF0077C1
      FF00F4FAFF0077C1FF00F9FCFF00DAEEFF00C7E5FF009FD3FF00E4F3FF00DCEF
      FF00E3F2FF00CDE8FF00BADFFF0045A9FF00EFD6B500FFE7D600FFEFD600FFE7
      CE00FFEFD600EFE7CE008C4A31008C100000B56B4200FFF7E700FFE7CE00FFE7
      CE00FFF7DE00FFEFD600CEB59C008484840000000000000000009CC6C700A8DA
      DC00AEE0E200B5E4E600C0E6EB00C7E6EE00C9E6EF00C5E6ED00BCE5E900B2E2
      E500ADDFE0009AC8C9000000000000000000F7E7D600FFEFE700DE6B3100D66B
      3100CE5A2900CE5A2100CE521800C64A1000E7D6C600FFFFFF00D6734200CE5A
      2100CE6B3900FFFFFF00EFDED6006B636300098EFF00F6FBFF00EEF7FF000B8F
      FF00F4FAFF0077C1FF00E8F4FF001794FF000C90FF00EDF7FF0078C1FF00BBE0
      FF00BBE0FF0056B1FF00E3F2FF001C96FF00EFD6BD00FFEFDE00FFF7DE00FFEF
      D600FFF7DE009C847300844239008C423100AD7B6300FFFFE700FFEFD600FFEF
      D600FFFFE700FFF7E700D6B59C00949494000000000000000000B2D1D100A7DA
      DC00B3E4E600BFE6EA00CDE7F000D7E9F400DAEBF500D5E9F300C9E6EE00BBE5
      E900AEE0E200A5CCCD000000000000000000F7DED600FFFFFF00F79C6B00E763
      2900CE8C6B00F7EFE700D67B5200BD310000D69C7B00FFFFFF00DE8C6300D652
      1000E79C7300FFFFFF00DECEBD00ADA5A5001191FF00EEF7FF0044A9FF001794
      FF00E8F4FF0044A9FF00F4FAFF00DAEEFF00CBE7FF00ABD8FF000C90FF0066B9
      FF0066B9FF00289CFF00D7EDFF001895FF00EFDEC600FFEFE700FFFFEF00FFEF
      DE00FFEFDE00FFFFE700FFFFEF00EFE7DE00FFF7E700FFF7DE00FFEFDE00FFF7
      DE00FFFFF700FFF7EF00C6AD9C00949494000000000000000000D4E5E5009CD1
      D200B9E6EA00C7E6EE00D8EAF400E9F3F900F0F7FB00E3EFF700D3E8F200C4E7
      EE00A7D9DC00C3DCDC000000000000000000F7DED600F7F7EF00FFEFDE00FF8C
      4A00DE845A00EFFFFF00FFFFFF00E7BDA500FFFFFF00F7FFFF00E7733900E773
      3100FFF7EF00F7E7E700A59C9400DED6D6007AC1FF001191FF007AC1FF0080C4
      FF001794FF0074BFFF000B8FFF00259AFF0034A1FF0054B0FF00BDE0FF0099D0
      FF0099D0FF0091CCFF00289CFF0091CCFF0000000000FFEFE700FFFFF700FFFF
      EF00FFF7E700FFFFEF009C6352009C290800D6AD9400FFFFF700FFF7E700FFFF
      F700FFFFFF00FFF7E700A59C8C00000000000000000000000000FDFDFE00A1CB
      CD00B8E4E800CBE7EF00DBEBF500F0F6FB00F9FBFD00E8F3F900D6E9F400C5E7
      ED00A1CDCF00F7FAFA00000000000000000000000000F7DED600FFFFFF00FFEF
      CE00FFB57300EFAD8400EFE7DE00EFF7F700F7E7E700F7A57B00FF8C4A00FFDE
      CE00FFFFFF00F7DED600C6BDBD00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F7E7CE00FFFFFF00FFFF
      FF00FFFFF700F7F7EF00633121006B000000AD7B6300FFFFFF00FFFFF700FFFF
      FF00FFFFFF00DEC6B5009494940000000000000000000000000000000000DAE9
      E900A0D1D300C8E8EE00D5E9F300E3EFF700E8F2F800DFEDF600D2EAF300AED7
      DC00CBE0E10000000000000000000000000000000000F7EFE700F7DED600FFFF
      FF00FFFFFF00FFF7C600FFDEAD00FFCE9C00FFCE9400FFD6AD00FFFFFF00FFFF
      FF00F7DED600C6BDBD00F7EFEF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFEFE700FFFF
      FF00FFFFFF00FFFFFF00D6CECE009C949400EFEFEF00FFFFFF00FFFFFF00FFFF
      FF00EFDED6009494940000000000000000000000000000000000000000000000
      0000C4DDDD00A9D6DA00CBE9F000D2E9F200D4E9F300D1EAF200B7DBE200B9D6
      D700000000000000000000000000000000000000000000000000FFEFE700F7DE
      D600F7EFE700FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00F7E7E700F7DE
      D600DED6CE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000F7EF
      DE00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00EFDE
      CE00949494000000000000000000000000000000000000000000000000000000
      000000000000BFDADB00A3D2D400BFE4E900C1E5EB00AAD6DA00B7D5D700FEFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000F7E7D600F7DED600EFDED600EFDED600EFDED600F7DED600EFDED600EFE7
      E700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000F7E7D600F7E7DE00F7E7DE00F7E7DE00F7E7D600F7E7DE000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000D7E8E800A4CDCE00A1CCCD00CFE3E400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F1F1F100DCDCDC00DCDCDC00F1F1F100000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FBFBFB00E8E8F100BBB9E400A09EDC009F9DDA00B4B3DF00DCDCE600F0F0
      F000FBFBFB00000000000000000000000000000000000000000000000000D0EC
      F700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000BFBEBD0077666000663A2B00692D18006A2E1A006333230075625C00BCBB
      BA000000000000000000000000000000000000000000B4692B00B3682B00B268
      2A00B1682A00B0662A00AF662900AF662900AE652800AE652800AD642800AD64
      2800AC642800AB632800AB63280000000000000000000000000000000000ECEC
      F6008885DC004B47D100615FD500726FDC00726FDC00615FD5004A46D1007E7C
      D400D7D6E300F7F7F70000000000000000000000000000000000000000004CB9
      E30039B0DF00F4FAFD0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FCFCFC008377
      720070291000913515009838170099371500C6978800AD644C008E2C0B006E26
      0D0081747000FAFAFA00000000000000000000000000B66A2D00BCD0BF00C3D5
      C600C9CFC4000000000000000000000000000000000000000000000000000000
      000000000000A8BBA700F4F8F500000000000000000000000000E5E4F5005350
      D3006865DB00908DE700908EE800908EE800908EE800908EE800908DE7006865
      DB004E4BD100CCCBDE00F7F7F70000000000000000000000000000000000F5FB
      FD002DAEDF002BABDE0089CEEB00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000081736E00842F
      1300993919009C3C1B00A13F1E009F341000C9998800EFF7FA00CBA79B009938
      1600842D0F007E6E6A00000000000000000000000000B76B2D00EAF1EB007FAC
      870044844D00397E4600A8C7AD00000000000000000000000000000000000000
      00004792510084B88B00000000000000000000000000F2F2FA005250D3006F6C
      E0008280E6008280E6008280E6008280E6008280E6008280E6008280E6008280
      E6006F6CE0004D4AD100D7D6E300FBFBFB000000000000000000000000000000
      000052BEE5004FBCE7004CBAE60035ADDE00EBF6FB0000000000000000000000
      00000000000000000000000000000000000000000000B7B4B300792B11009839
      1800A03E1D00A9432100B0462300B4472300D9B4A700F0F6F700F1F6F800D4BA
      B100993A1A007A2A0F00B3B0AF000000000000000000B96D2E00C9C4E6000000
      00000000000086B98F003D8F4B007CB4850000000000000000000000000091C3
      99003C984A00000000000000000000000000000000008F8DE1004E4AE4006A66
      EE006A66EE006A66EE006A66EE006A66EE006A66EE006A66EE006A66EE006A66
      EE006A66EE004E4AE4007E7CD400F0F0F0000000000000000000000000000000
      0000000000002BAFDF0084D3F20055BDE7002EAADE0083CBEA00000000000000
      0000000000000000000000000000000000000000000074554A00953716009C3C
      1B00A9442100B44A2600BB472000D6978200F6FEFF00F1F1F000EFEFEF00F5FF
      FF00B97F6B00922F0C0074524700FEFEFE0000000000BB6E2F00ACABFD000000
      00000000000000000000C5E1CA004AA258004FA65E0077BB810069B574003DA3
      4C00B3DBB800000000000000000000000000F7F7FC00423ED7004D48FC004E4A
      FC004E4AFC004E4AFC004641FF00423DFF00423DFF00423DFF004641FE004A46
      FC004C48FC004D48FC003F3BD500DCDCE6000000000000000000000000000000
      00000000000060C5E70070CCEE0083D2F2007ECEF1004AB6E40030A9DC00EBF6
      FB0000000000000000000000000000000000E5E5E50071311B0096371800A13F
      1E00B1482400BE4F2A00C74A2000E6B2A100F9FFFF00F7FCFE00F5FAFC00DDBE
      B300BA765F009734130074301800E2E2E20000000000BD6F3000B1B0FF002824
      FF00000000000000000000000000EEAD88008DC7930063B66F0060B56C00BADF
      BF000000000000000000615EFC0000000000CDCCF2003935E3004C47FB004C47
      FB00A5A3FD00A4A2FD00A3A1FE00A09EFF00A09EFF00A09EFF00A09EFF00A09E
      FF00433EFF004540FF003835E400B4B3DF0043C5E8003BC1E6003ABEE50038BC
      E40036B9E30034B8E20086D7F3002FB6EB004ABCEC0080CEF10051B9E6002CA8
      DD0077C5E800000000000000000000000000CAC8C7008E341400AC441E00B14A
      2400BA4C2600C34A2100D66B4800EA9A8000F8E0D800F0D0C600D68B7200B546
      2100A43B1800993918007D280A00C6C3C20000000000BE713000DED8E600110D
      FD008280FD00EEA87B00E0570100E0580300E0570100F4C5AA00000000000000
      000000000000000000002825F90000000000B1B0EB004844E500726EE800BBB9
      F400FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFA
      FD00A09EFF00423CFF003C37EB009F9DDA004AC9E90075DAF20093E6F80091E3
      F7008DE0F6008ADCF5008ADBF50088D7F40084D3F2007FCFF1007CCCF0007AC9
      EF0048B4E3002BA6DB00E0F1F90000000000C6C3C200A43D1700C9532500CC5C
      2B00D56F3F00E5AB9000EFC1AB00EA825900E77E5B00D4593100C34A2200B74B
      2600A84220009D432400BE998D00C5C3C20000000000C0723200000000006A68
      FC001F1CF900BC534300EDA4720000000000E78D5200DE651700EEB18B000000
      000000000000605EFA003A37F70000000000B2B1EC004E4AE2008F8DE500ADAB
      EC00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFAFD00FAFA
      FD00716CFF00403CFF003B37EB00A09EDC00D4F2FA004FCDEC0098E9F9004AD5
      F30045CFF10040CAF00038C2EE0089D9F4002EB4E10031B3E00030B0DF002EAE
      DE002CABDC002AA8DB0035AADD0000000000DCDBDB009B3E1D00C5502200C956
      2500E5B6A100F3E1D600E58C5C00EB9E7200E7834D00DC6F3D00CA5A3100B64B
      27009F381700B4715A00E8E9E900DAD9D90000000000C2733200000000000000
      0000392AEB004935E100000000000000000000000000EAA06C00DE742700EAA7
      7800000000003633F600B89DCA0000000000CFCEF3003C38E000A3A1E900A6A4
      EA00A6A4EA00A6A4EA00A6A4EA00A6A4EA00A6A4EA00A6A4EA00A6A4EA009896
      E6004D49F800403BFF003834E300BBB9E4000000000043C9E90080E1F5008EE6
      F80043D2F3003FCDF10039C7EF008CDCF50058C6EA009FDCF100000000000000
      000000000000000000000000000000000000FBFBFB0090564100C54A1C00CB67
      3D00F2EEEC00F6F7F700F7F3F000F3D7C900DE713900DC723B00D76C3900CC5F
      2F00B0411700CE958100C5C2C100FAFAFA0000000000C3733300F0EAE500EEB4
      8300B06369003E38F5008B89F900000000000000000000000000EDB88E00DF81
      3800DF833C00DF884000DF883E0000000000F9F9FD00423ED7009390EC00BAB8
      EE00BAB8EE00BAB8EE00BAB8EE00BAB8EE00BAB8EE00BAB8EE00BAB8EE00BAB8
      EE00B9B7EE009A98E4003F3CD500E8E8F10000000000BFEDF8005BD4EF0099EA
      F90047D6F40042D0F2003DCBF0006ED5F3007FD7F3004AC0E700C2E9F6000000
      00000000000000000000000000000000000000000000A1928C00BD431700C864
      3D00F1EFEF00F5F7F800F5F1F000F5F1EF00E9B79F00D56F3F00CF623000CA5B
      2B00C24B1B00E4AB9500ACA9A9000000000000000000C474340000000000E082
      3000E8A56B009A98F9004340F400D3D2FC000000000000000000000000005452
      F1005650EE00EFC8A400000000000000000000000000918EE5004E4AE500C6C4
      F100CECEF400CECEF400CECEF400CECEF400CECEF400CECEF400CECEF400CECE
      F400CCCCF3005E5BE3008885DC00FBFBFB00000000000000000041CBEA0093E9
      F90072E1F70045D4F30041CEF2003CC9F0008ADCF50070D0EF003DBBE400E2F4
      FA000000000000000000000000000000000000000000F5F5F50090553F00C646
      1700E0B7A800F4FBFD00F2EFED00EACFC400F2EDEA00D6886600C5512000C14C
      1D00DB815E00A48A8000F4F4F4000000000000000000C5753400F8E7D700DE89
      400000000000000000007976F4005552F1007573F4006968F2005552F1006664
      F100E8E7FD0000000000000000000000000000000000F6F5FD005652D9006865
      EC00D6D5F400E2E2F800E2E2F800E2E2F800E2E2F800E2E2F800E2E2F800DEDD
      F6007C7AE5004441D200ECECF6000000000000000000000000009CE4F40064D9
      F1009AEBFA0048D8F40044D3F3003FCEF1003AC8F0008CDCF50062CBED0033B8
      E300000000000000000000000000000000000000000000000000E2E1E100934F
      3600DD937700FCFFFF00F5FAFC00E5C2B400DDA79200D99B8300CB603600D06A
      4400A9887C00E0DFDF00000000000000000000000000C6763400E8CBB200E8B7
      8B00000000000000000000000000A5A4F6006E6CF0007573F100C1C1F9000000
      0000000000000000000000000000000000000000000000000000ECECFB005652
      DA004D4AE600AAA8F500E5E4F800F2F2FB00F2F2FB00ECEBFA00BBB9F0005C58
      E4004542D200CFCEED00000000000000000000000000000000000000000040CC
      EB009AEDFA0099EBF90097E8F90094E5F80091E2F7008EDFF6008BDBF50056C7
      EB003ABBE400000000000000000000000000000000000000000000000000F1F1
      F100A18F8800B37F6A00C8775700E2B09C00E5B5A300CF896F00A95F4300A18F
      8800EFEFEF0000000000000000000000000000000000C6763400F3DCC8000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000F6F5
      FD009492E700423ED9003834E4005451E0005E5BDC003835E400423ED7008685
      DF00E8E7F70000000000000000000000000000000000000000000000000085DF
      F2003FCCEB003ECBEA003CC9E9003BC7E9003AC4E80038C2E70036C0E60035BD
      E50033BBE40049C0E60000000000000000000000000000000000000000000000
      000000000000F0F0F000C5BEBD00AF9F9A00AFA09A00C4BDBC00EEEEEE000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000F9F9FE00CFCEF400B3B1EC00B1B0EB00CDCCF200F7F7FC000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000B5B5B500B5B5
      B500B5B5B500B5B5B500B5B5B500B5B5B500B5B5B500B5B5B500B5B5BD00B5B5
      BD00B5B5B500C6C6C60000000000000000000000000000000000000000000000
      00000000000000000000DEA58C00D6845200DE946300CE846300EFCEBD00FFF7
      EF00000000000000000000000000000000008282820083838300848484008383
      8300838383008180800082818100828282008181810081818100828282008181
      8100EAEAEA00FDFDFD00FCFCFC00FEFEFE0097A5B40035455900354559003545
      5900354559003545590063493500634935006349350063493500634935006349
      35006349350063493500634935006349350000000000000000007B7B7B00D6D6
      D600BDBDBD00BDBDBD00D6D6D600BDBDBD00BDBDBD00E7DEDE00D6B59C00D6AD
      9C00EFEFE7009494940000000000000000000000000000000000000000000000
      000000000000FFF7EF00D6845200D6946B00D69C7B00CE845A00D68C6B00DEB5
      AD00FFFFFF0000000000000000000000000087878700DBDBDB00D9D9D900D6D6
      D600D5D6D600D5D6D600D3D2D200D1D0D000D0D1D100CBCBCB00D8D8D8009191
      9100E1E1E10000000000000000000000000099A7B500EEF1F300E4E9ED00B8C3
      CD00B3BEC90065768900F7DAC800F5D1BA00F5D1BA00F5D1BA00F5D1BA00F5D1
      BA00F5CFB700F0AE8400F0AE840063493500000000000000000084848400B5B5
      B5008C8C8C0094949400BDBDBD008C8C8C008C8C8C00DECEBD00BD846300BD84
      6300E7D6CE009494940000000000000000000000000000000000000000000000
      000000000000DE9C7300DEA58400D6B5AD00DEBDBD00DEB5AD00D69C8400CE84
      6300CE734200DEA59400F7E7E7000000000099999900F6F6F600F4F4F400F0F1
      F100EEEEEE00EBEAEA00ECF1F200E4E7E800DADAD900D4D3D300E0E0E0009C9C
      9C00CACACA00EEEEEE00F0F0F000FDFDFD009CAAB800EEF1F300E9EDF000E4E9
      ED00B8C3CD006A7B8E00F8DFCE00F7DAC800F7DAC800F7DAC800F5D1BA00F5D1
      BA00F5D1BA00F5CFB700F0AE84006349350000000000000000007B7B7B00DEDE
      DE00C6C6C600C6C6C600D6D6D600C6C6C600C6C6C600DECEC600BD846B00BD84
      6300E7D6D6009494940000000000000000000000000000000000000000000000
      0000E7BD9400DE946B00D6ADA500DEC6BD00EFDED600E7C6C600DEBDBD00D6A5
      9400D69C8C00DE947300CE7B4A00CE846B009A9A9A00F1F1F100EFEEEE00EEF1
      F200E0D9D600A9736100AC847500C1BDBA00CCD3D500C6C7C800CDCCCC009797
      9600BBBBBB00D4D4D400CFCFCF00E8E8E8009EACBA009BA9B7007F8FA0007A8A
      9C00748597006F809200A67E6500A47A6000A1755B00A47A6000A1755B009E70
      56009B6D510099694E0097664A0063493500000000000000000084848400B5B5
      B5008C8C8C0094949400BDBDBD008C8C8C008C8C8C00DEC6BD00BD846300BD7B
      5A00E7D6CE00949494000000000000000000000000000000000000000000F7DE
      C600DE945A00D6A59C00E7C6BD00EFDED600F7EFEF00E7CEC600F7E7E700F7DE
      DE00E7C6BD00CEA59C00E7BD9C00C66B3900A0A0A000F4F4F400F3F2F200F2F3
      F300EAE9E700BD8F8100B2705F00A6655000A97F7000C4C0BF00D8DDDF009897
      9700BCBCBC00D4D4D400D1D1D100EFEFEF00A2AFBC00F6F8F900EEF1F300C6CF
      D700C3CCD50074859700FAE6DA00F7DAC800F7DAC800F7DAC800F7DAC800F7DA
      C800F6D7C200EEB28D00EEB088006349350000000000000000007B7B7B00DEDE
      DE00C6C6C600CECECE00DEDEDE00C6C6C600C6C6C600EFE7DE00D6C6BD00D6C6
      BD00F7EFE700949494000000000000000000000000000000000000000000E7AD
      7B00DEA58C00E7C6BD00F7DEDE00F7DEDE00F7EFEF00F7DEDE00E7C6C600F7EF
      EF00F7EFEF00E7C6BD00D68C6B00DEA58C00A6A6A600FAFAFA00F4F4F400F2F1
      F100F5F7F900D0B7AE00BD887B00BD817200A15137009E655100CBC2BF009EA1
      A300BBBCBB00D7D6D600EDEDED0000000000A5B2BF00F6F8F900F6F8F900EEF1
      F300C8D1D90078899B00FAEBE100FAE6DA00FAE6DA00FAE6DA00F7DAC800F7DA
      C800F7DAC800F7D7C200EEB28D0063493500000000000000000084848400B5B5
      B5009494940094949400BDBDBD008C8C8C00948C8C009CC6F700006BFF00006B
      FF00B5D6F7009C94940000000000000000000000000000000000FFEFE700DE9C
      5A00D6A59C00F7E7DE00EFD6CE00FFF7F700F7EFEF00F7EFEF00EFCECE00EFD6
      CE00F7E7E700DEAD9C00D67B4A00F7DEDE00A9A9A90000000000F9F9F900F3F3
      F200F5F7F800E7E2DF00BD948800C99A9100BC837300A85C4400A76149008474
      6D00DDE1E100F6F5F500FDFDFD00FEFEFE00A9B5C1009BA9B7008C9BAB008998
      A8008393A4007E8E9F00AF8C7600AC877000AA836B00AC877000AA836B00A67E
      6500A37A6000A1755B009E7056006349350000000000000000007B7B7B00CECE
      CE00ADADAD00B5B5B500CECECE00ADADAD00B5ADAD00BDD6EF003194FF003194
      FF00CEDEF7009494940000000000000000000000000000000000EFBD9400DEA5
      8400DEBDB500F7DEDE00F7E7DE00FFFFFF00FFEFEF00F7EFEF00F7E7E700E7C6
      BD00EFD6CE00DEA57B00CE846300FFFFFF00ACACAC00FEFEFE00FEFEFE00FBFC
      FB00F7F7F600FCFEFF00D9C8C100C3998E00CB9F9600B97E6E00AE634C008D49
      3400A0918B00F5F9F90000000000FDFDFD00AFBBC600F6F8F900EEF1F300D3DA
      E000CFD7DE008292A300FCF2ED00FAE6DA00FAE6DA00FAE6DA00FAE6DA00FAE6
      DA00F9E2D400EABCA000EBB9990063493500000000000000000084848400C6C6
      C600A5A5A500ADADAD00CECECE00A5A5A500A5A5A500DED6D600ADA59C00ADA5
      9C00E7E7DE0094949400000000000000000000000000FFFFF700E7A56300D6A5
      9400EFDEDE00EFCEC60000000000FFFFFF00FFF7F700F7EFEF00EFDED600EFD6
      D600DEB5AD00D68C5A00E7BDAD0000000000B0B0B000FEFEFE00000000000000
      0000FDFCFC00FFFEFE00ECE2DE00AA776400CDA69E00CEA49C00B87B6B00B369
      5400934D36009C877F00F7F9FA00FEFDFD00B5C0CB00F6F8F900F6F8F900EEF1
      F300D3DAE0008998A800FDF6F200FCF2ED00FCF2ED00FCF2ED00FAE6DA00FAE6
      DA00FAE6DA00F9E2D400EABCA00063493500000000000000000084848400BDBD
      BD00949494009C9C9C00C6C6C6009494940094949400D6D6D600949494009494
      9400DEDEDE0094949400000000000000000000000000F7D6B500E7AD7B00DEB5
      AD00F7DED600F7E7DE0000000000FFFFFF00FFF7F700FFF7F700E7CEC600EFDE
      D600DEAD9400CE7B4A00FFF7F70000000000B2B2B20000000000FEFEFE00FEFE
      FE00FDFCFC00FEFFFF00E9DAD5009F675000A0644F00D5B2AE00CCA09700B476
      6500B469530083584900A0999600FDFDFD00B9C4CE008C9BAB0098A6B40095A3
      B200909FAE008C9BAB00B99A8600B6968100B2917B00B6968100B2917B00AF8C
      7600AD877100A9836B00A67D65006349350000000000000000007B7B8400CED6
      C600B5BDA500B5BDA500ADB5A500B5BDA500B5BDA500ADB59C00ADB59C00ADB5
      9C00D6D6CE0094949400000000000000000000000000F7BD8400D6A58C00E7C6
      BD00E7CEC600E7CEC60000000000FFFFFF00FFFFFF00F7E7E700EFDED600E7C6
      C600DE946B00E7B594000000000000000000B6B6B600FEFEFE0000000000FEFE
      FE00FEFEFE00FEFEFE0000000000DDC8C100A8776300B1807100D3AEA900D0A5
      9C00BDA59F00A5A5A40075686200C8C6C500BDC7D100F6F8F900EEF1F300D3DA
      E000CFD7DE008292A300FCF2ED00FAE6DA00FAE6DA00FAE6DA00FAE6DA00FAE6
      DA00F9E2D400EABCA000EBB999006349350000000000000000008C848C00638C
      31007BBD39007BBD390084C642008CCE520094CE5A009CD67300A5DE8400B5E7
      94007B945A009C9C9C000000000000000000FFEFD600EFB57B00D6AD9C00DEBD
      B500FFF7F700EFD6CE00F7EFEF000000000000000000EFD6CE00F7E7DE00E7BD
      AD00D6844A00FFEFE7000000000000000000B9B9B90000000000000000000000
      0000FEFEFE00FCFCFC00FEFEFE00BEBFBF00997D7200BB908000A9726000C2A4
      9C00E6E9EB00C6CCCE00958B860088756F00C0CAD300FDFEFE00FDFEFE00EEF1
      F300D3DAE0008998A800FDF6F200FCF2ED00FCF2ED00FCF2ED00FAE6DA00FAE6
      DA00FAE6DA00F9E2D400EABCA0006349350000000000000000008C848C00638C
      31007BB529007BBD31007BBD390084C6390084C642008CCE420094D652009CDE
      63007B9C5A009C9C9C000000000000000000F7CE9400DEAD8400CE9C9400F7EF
      E70000000000FFFFFF00E7CEBD00FFFFFF00FFFFF700E7CEC600E7C6BD00D684
      4A00E7A57300FFFFFF000000000000000000BCBCBC0000000000000000000000
      0000FDFDFD00FDFDFD00FEFEFE00BEC1C100E1DEDC00DEC7BE00BC978800A287
      7D00EEF2F300ECF0F200CBA79F0091605000C1CBD400C1CBD400C0CAD300BDC7
      D100BBC5CF00B7C2CC00D4C1B300D1BEB000CEBAAC00CBB8AA00C7B4A600C5B1
      A200C2AE9F00C0AB9C00BDA99900BBA7970000000000000000008C848C008CA5
      6B00638431006384390063843900638439006384390063843900638431005A84
      29009CAD84009C9C9C000000000000000000F7B56300EFCEBD00F7E7E7000000
      00000000000000000000F7EFE700EFDED600F7E7DE00F7E7E700FFFFFF00F7E7
      D600FFEFE700000000000000000000000000BEBEBE00FDFDFD00FDFDFD00FDFD
      FD00FCFCFC00FCFCFC00FCFCFC00CFCFCF00F4F7F800A29B9900B4978D00E6DE
      DB00A3948E00BC9C9300BE7E6E009B7569000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000052525200B5AD
      B500ADADB500ADADB500ADADB500ADADB500ADADB500ADADB500ADADB500ADAD
      B500ADADAD007B7B7B000000000000000000FFEFD60000000000000000000000
      0000000000000000000000000000E7CEC600DEBDB500FFFFFF00000000000000
      000000000000000000000000000000000000C4C4C40000000000000000000000
      0000000000000000000000000000D5D5D500A4A3A300BBBFC200E2D3CF00C1A1
      9400C9BBB500D2BEB700C8A39600DFD8D5000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000FFFFFF0000000000000000000000
      0000000000000000000000000000F7E7DE00E7C6BD0000000000000000000000
      000000000000000000000000000000000000B1B1B100C7C7C700C4C4C400C1C1
      C100BDBDBD00BABABA00B8B8B800A6A6A600C1C1C100FDFDFD00FEFCFC00F1EE
      ED000000000000000000000000000000000000000000000000008237DF00AC79
      EA00000000000000000000000000DC5299000000000000000000000000000000
      00000000000000000000000000000000000000000000000000008237DF00AC79
      EA00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000008237DF00AC79
      EA00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000A9957B00907758008A6F4E008A70
      4F008A704F008A704F008A704F008A704F008A704F008A704F008A704F008A70
      4F008A704F008B704E008F75510091887B000000000000000000B98FED007623
      DC00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000B98FED007623
      DC00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000B98FED007623
      DC00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DAA25E00E6B97B00EABC7E00EDBE
      7E00EDBD7D00EDBD7C00EBBC7A00EABA7500F0BC7100F5BF6F00F3BD6E00EBB8
      6F00E6B47200EBB87200E1A24A008C704B000000000000000000F7F2FD007929
      DD00B98EED00FCFCFF0000000000DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F7F2FD007929
      DD00B98EED00FDFDFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DC52990000000000F7F2FD007929
      DD00B98EED00FDFDFF0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E7C09E00FFF4E000FDF0DC00EFE1
      CE00ECDECA00EBDDC800F0DEC600FAE7CC00F3E0D000E6D5CE00EDDACC00F8E6
      C700FFEFC800FFEBC400EBB770008A704D00000000000000000000000000BA90
      ED006E16DA00F5F0FD0000000000DC5299000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BA90
      ED006E16DA00FAF7FE0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DC529900DC52990000000000BA90
      ED006E16DA00FAF7FE0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E4C0A000FFFDED00CCCAC400B8B6
      B200C0BDB700BFBBB500D3C7B200D6D4CD0075A5F0002672F9004281F100AABA
      DB00FFFDC800FFECC700E6B471008A714F00000000000000000000000000FBF9
      FE007929DD009556E40000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FBF9
      FE007929DD009556E40000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DC529900FBF9
      FE007929DD009556E40000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E4BF9E00FFFCEA00BEBCB600D8D0
      C500EDE9D600EDE8D500F3F1CD00D8E2D50047A3F1000073FF000D74F80093A7
      DA00FFFFC400FFEBC600E6B470008A704F000000000000000000000000000000
      0000E2D1F8007019DA00B99EF100DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000E2D1F8007019DA00B99EF100000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DC529900DC52
      9900E2D1F8007019DA00B99EF100000000000000000000000000000000000000
      000000000000000000000000000000000000E4BF9E00FFFDED00C0BFBA00E4DD
      CF00FFF9E300FFF8E200FFFEDA00E1E9DC0057AEEE000085FD002482F4009DAD
      DA00FFFFC900FFEBC700E6B571008A704F000000000000000000000000000000
      000000000000A873E9007826DC00DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000A873E9007826DC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000DC529900A873E9007826DC00000000000000000000000000000000000000
      000000000000000000000000000000000000E4BF9E00FFFEF000C0BFBB00E9E2
      D500FFFFEC00FFFFEB00FFFDE500F9F4DE00D5E6DD00BBE0DE00C8DDD900E7E1
      D200FFF2D000FFEBCA00E6B571008A704F000000000000000000000000000000
      000000000000000000008033DE006F2AC300FEFDFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000008033DE006F2AC300FEFDFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000DC529900DC5299008033DE006F2AC300FEFDFF0000000000000000000000
      000000000000000000000000000000000000E4BFA000FFFEF300C2C1BD00C8C4
      BD00D8D6CB00D6D5CA00E0D7C800EEE4D500E0DCDE00C8CCDB00D3D5D800EBE6
      D300FFF3D400FFEDCD00E6B673008A704F000000000000000000000000000000
      00000000000000000000E8DBF900711BDB008B44E10000000000000000000000
      000000000000000000000000000000000000DC529900DC52990000000000DC52
      9900DC52990000000000DC529900DC5299008B44E100DC529900DC5299000000
      0000DC529900DC52990000000000DC5299000000000000000000000000000000
      00000000000000000000DC529900711BDB008B44E10000000000000000000000
      000000000000000000000000000000000000E4BFA000FFFEF600C2C2BF00B9B6
      B300C6C3BD00C6C2BC00D8CEBA00D5D7D40071A5F1002571F6004080F000A8B9
      DF00FFFED400FFEECF00E6B674008A704F000000000000000000000000000000
      0000000000000000000000000000000000007521DC007D2EDE00E7D9F9000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000007521DC007D2EDE00E7D9F9000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000DC529900DC5299007521DC007D2EDE00E7D9F9000000
      000000000000000000000000000000000000E4C0A000FFFEF800C1C1BF00DBD5
      D000F3F0E500F4F0E300FCF7DC00D7E6E3003EA0F500006BFF00026DF9008BA6
      E300FFFFD600FFEED300E6B676008A704F000000000000000000000000000000
      0000000000000000000000000000DC529900F1E9FB00914FE3007624DC00AD7B
      EA00F1E9FB000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F1E9FB00914FE3007624DC00AD7B
      EA00F1E9FB000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DC529900914FE3007624DC00AD7B
      EA00F1E9FB00000000000000000000000000E4C0A100FFFEFA00BFBFBE00E5E0
      D900FFFFF400FFFFF300FFFFEB00E4EFE9006BB8F0002793F8003F91F200A7B8
      E200FFFED900FFEFD600E6B777008A704F000000000000000000000000000000
      0000000000000000000000000000DC5299000000000000000000C19BEF007B2B
      DD007826DC00A46DE800F4EEFC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C19BEF007B2B
      DD007826DC00A46DE800F4EEFC00000000000000000000000000000000000000
      000000000000000000000000000000000000DC529900DC529900C19BEF007B2B
      DD007826DC00A46DE800F4EEFC0000000000E4C0A300FFFFFE00BEC0C000DFDE
      DA00FDFDF500FFFCF500FFFDF100F8F9F000D6EAF100BADEF000C9DEED00E7E8
      E600FFFCE700FFF5E000E6B97B008A704F000000000000000000000000000000
      0000000000000000000000000000DC5299000000000000000000000000000000
      0000C5A1F0007B2BDD006A10D9008236DF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C5A1F0007B2BDD006A10D9008236DF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DC5299000000
      0000C5A1F0007B2BDD006A10D9008236DF00E4BFA000FCF7F100CFCBC600E6E0
      D800FAF6ED00FDF6EC00FCF2E900FEF5E700FFFAE300FFFBDF00FFFBDD00FFF7
      DA00FCEEE000FEF0DA00E5B97C008A704F000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E8DBF900914EE3000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E8DBF900914EE3000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DC529900DC52
      99000000000000000000E8DBF900914EE300E3B89600F7DAC200FCDBBE00F9D4
      AF00F9CCA000F9C49200F9BD8400F9B77800F9B47200F9B27100F9B27000F9B2
      6E00FAB37100FDB67100EAA049008A714E000000000000000000000000000000
      0000000000000000000000000000DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000DC529900000000000000000000000000E3B58F00FBCFA800FDC89600FDB9
      7B00FDAA5A00FD9A3A00FD8A1900FD7D0300FD740000FD730000FD740000FD74
      0000FD740000FD770000EE7A05008E7955000000000000000000000000000000
      0000000000000000000000000000DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000DC529900DC5299000000000000000000DFB48F00E3B79000E4B58D00E4B1
      8600E4AE8000E4AC7900E4A87100E4A56C00E4A36900E4A26800E4A36800E4A3
      6800E4A46A00E8A66B00DE9B4D00A9967B0056565600DDDDDD00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EEF6FF0084B7FB001A79F7000A6F
      F7000C70F7000B70F7000971F7000771F7000670F7000870F700096FF7000C6F
      F7000C6EF7000B6EF7000D6DF70095BDFB000000000000000000000000000000
      0000000000000000000000000000F7F7F700E7E7E700EFEFEF00000000000000
      00000000000000000000000000000000000002020200020202005509B1003907
      7700020202000202020002020200DC5299000202020002020200020202000202
      020002020200020202000202020002020200DEDEDE0068686800767676000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000ABD4FE0047A5FC003BA4FC003DA4
      FC0037A1FC002BA1FC0021A5FC001AA8FC0018A8FC001BA7FC0020A3FC002B9D
      FC002F99FC002E94FC003794FC0095C1FB000000000000000000000000000000
      0000F7F7F700E7E7E700F7E7DE00CEAD9C00AD9C9400D6D6D60000000000E7E7
      E700EFEFEF000000000000000000000000000202020000000000B98FED007623
      DC00000000000000000000000000000000000000000000000000000000000000
      000000000000020202000000000000000000000000007F7F7F00000000003E3E
      3E00ABABAB000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DCECFC0066B2FA0055BEFE0069D3
      FE005BCFFD0037CCFF001CCCF7001C91A600169AAC0009DDFA000FDEFF0023CA
      FE002CBDFE0032ABFD005EB0FC00E7F2FE00000000000000000000000000F7E7
      DE00B5948C00B5B5B500DEB5A500D66B4A00A57B6B00C6C6C600D6B5A500B5A5
      9C00B5B5B500F7F7F7000000000000000000DC52990000000000F7F2FD007929
      DD00B98EED00FCFCFF0000000000DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000141414003838
      3800787878009E9E9E0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FAFBFD00ABCFF1004FB9FF0062DB
      FE0065E3FD0053E1FF0037DAF20018828A000A86900007E6F50006EFFF001AD7
      FE0021C6FE002DAFFD00ACD4FC00FBFCFF000000000000000000FFF7F700DE8C
      6B00BD633900B5948400CE846300D6735A00A5634A00B5948400C6633900AD5A
      3900CEC6BD00000000000000000000000000DC529900DC52990000000000BA90
      ED006E16DA00F5F0FD0000000000DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000A4A4A4000000
      0000AAAAAA00A4A4A40099999900000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EBF4FD0061B8FD0060D4
      FD0070E9FC0068E4FF0048E3F8002AC3CE002BC5CF001BEEF90016EEFE0025D4
      FD0022BDFD003CACFB00E3F1FE00000000000000000000000000E7E7E700EFC6
      B500E77B5A00CE6B4A00E7846300EF847300D6736300D6734A00CE735A00B57B
      6B00CECECE00EFEFEF00DEDEDE00000000000000000000000000DC529900FBF9
      FE007929DD009556E40000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000A3A3
      A30013131300D8D8D80096969600929292000000000000000000000000000000
      00000000000000000000000000000000000000000000FCFDFF00AED8FE0049B9
      FD0073E5FD008BF3FF0067DAED0013495000174F550041E2F00053F1FF0048CC
      FD002EABFC00A8D3FD00FDFEFF000000000000000000EFC6B500BDA59C00C694
      7B00E7846300E77B6300E7846300E7946B00E7A58400EFAD9400D67B6300A573
      5200AD9C8C00AD847300B5B5B500F7F7F7000000000000000000DC529900DC52
      9900E2D1F8007019DA00B99EF100DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000A0A0A00000000000CACACA009B9B9B009999990000000000000000000000
      0000000000000000000000000000000000000000000000000000EAF4FF0062B9
      FD0062D4FD0097F4FF0081DCED0020485000254D55006EDDF10082EEFF004EC1
      FD0044A9FB00E4F1FE000000000000000000FFFFFF00DE845A00DE734A00E784
      6300E77B6300C66B4200DEB5A500FFF7F700FFEFE700EFC6B500E79C8C00C673
      6300BD6B4200BD634200D6C6BD00FFFFFF000000000000000000000000000000
      0000DC529900A873E9007826DC00DC5299000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000009C9C9C0011111100D9D9D900969696009C9C9C0000000000FBFB
      FB00000000000000000000000000000000000000000000000000FDFEFF00B1D8
      FE004DB9FD0085EBFF0099DFED0035484F00374D5500A2DCF200A1E4FF0053B2
      FD00A5D0FD00FBFDFF000000000000000000FFFFFF00F7DECE00EFBDA500F7AD
      9C00C66B4A00B58C7B00F7F7F7000000000000000000FFEFEF00EFB5A500DE8C
      7B00BD6B4200CE9C8C00FFF7F700000000000202020000000000000000000000
      0000DC529900DC5299008033DE006F2AC300FEFDFF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000009D9D9D0011111100EFEFEF0075757500B1B1B100ABAB
      AB006F6F6F00000000000000000000000000000000000000000000000000EEF6
      FF0069B8FD0064D3FF009BDFED00424A4F0044505400B7DEF10088CFFF006BB2
      FB00E4F0FE0000000000000000000000000000000000FFFFFF00DEBDB500E794
      7300B55A3900AD9C9400DEDEDE000000000000000000FFFFF700E7AD8C00D684
      6300AD7B6300B5B5B500C6C6C600F7F7F700DC529900DC52990000000000DC52
      9900DC52990000000000DC529900DC5299008B44E100DC529900DC5299000000
      0000DC529900DC52990000000000DC5299000000000000000000000000000000
      0000000000000000000000000000B3B3B300373737003A3A3A00969696004545
      4500000000007B7B7B000000000000000000000000000000000000000000FCFD
      FF00A3D1FE0051BCFF008DD6EE00474F51004A545600A6D6F10055B0FF00A9D0
      FD00FBFDFF00000000000000000000000000FFF7F700E7AD8C00DE846300EF94
      7B00BD6B4A00A5847300B5B5B500D6D6D600EFEFEF00DEBDB500E79C7B00CE8C
      8400B56B4A00AD735200AD9C9400F7F7F7000202020000000000000000000000
      00000000000000000000DC529900DC5299007521DC007D2EDE00E7D9F9000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000009999990000000000424242007070
      70008B8B8B001C1C1C0073737300000000000000000000000000000000000000
      0000EEF5FF005CB3FF0073C4F3006C888C006F8C8F0079C1F500459EFE00E5F0
      FE0000000000000000000000000000000000FFFFFF00E7AD8C00E7A58C00EFB5
      9C00E78C7300A55A3100A57B6B00B59C9400BD947B00D66B4200DE948400CE7B
      5A00D66B4200CE6B4200E7DED600000000000202020000000000000000000000
      0000000000000000000000000000DC529900DC529900914FE3007624DC00AD7B
      EA00F1E9FB000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000A6A6A600000000003F3F
      3F00F2F2F200E5E5E50023232300737373000000000000000000000000000000
      0000FCFDFF009FCDFF005BB9FD0095E0EE0095DCEE005BB2FB008FC0FD00F8FB
      FF000000000000000000000000000000000000000000FFE7DE00FFF7EF00F7E7
      DE00E79C8400D68C7300C6735200BD634200CE735200DE9C8C00D6846B00BD7B
      6300C6BDBD00F7DECE0000000000000000000202020000000000000000000000
      0000000000000000000000000000DC529900DC529900DC529900C19BEF007B2B
      DD007826DC00A46DE800F4EEFC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000A6A6A6000000
      000030303000F6F6F600E2E2E2001A1A1A000000000000000000000000000000
      000000000000E8F2FF0070BAFD006DCCFD0062C6FD0059ABFA00DDEBFE000000
      000000000000000000000000000000000000000000000000000000000000EFCE
      BD00E7A59400DE8C6B00E7AD9400E7A59400D6846300DE947300DE8C7300B56B
      4A00B5B5B500F7F7F70000000000000000000202020000000000000000000000
      0000000000000000000000000000DC5299000000000000000000DC5299000000
      0000C5A1F0007B2BDD006A10D9008236DF000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000A3A3
      A3000000000042424200B0B0B000222222000000000000000000000000000000
      000000000000FCFDFF00AED3FE0043A2FE003298FC009AC8FC00FAFCFF000000
      0000000000000000000000000000000000000000000000000000FFFFFF00EFBD
      A500DE846300E7D6C600F7DECE00E7A58C00BD735200CEC6C600E7AD9400CE6B
      4200E7D6CE000000000000000000000000000202020002020200000000000000
      0000000000000000000000000000000000000000000000000000DC529900DC52
      99000000000000000000E8DBF900914EE3000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000A9A9A9000000000045454500272727000000000000000000000000000000
      00000000000000000000E9F3FF0050A1FC004999FB00DDEBFE00000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00F7DECE0000000000FFE7DE00DE947300D6947B00F7F7F700FFF7EF00FFEF
      E700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000DC5299000000000000000000000000000000
      0000DC5299000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000A4A4A40000000000848484000000000000000000000000000000
      00000000000000000000FDFEFF00D6E8FE00D2E5FE00F9FBFF00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFEFE700FFEFE70000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000DC5299000000000000000000000000000000
      0000DC529900DC52990000000000000000000000000000000000000000000000
      0000FDFDFD00F0F1F2007E8B8D00193035008592940060707300E2E6E600F8F9
      F900000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F9FAFA00607073006F7E8100142C31002C424600485B5E005E6F7200919C
      9F00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000BAC1C20069787B001B323700919D9F00AEB6B8004B5E61002C414600A5AE
      B000C0C7C8000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00002C424600172F3300939EA000F4F5F500FDFDFD00D6DADB002E4348001C33
      380043565A000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00008B9799002C414600B9C0C100FDFEFE0000000000F6F7F700354A4E006A79
      7C0099A4A500000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00000000000000000000000000000000000000000000000000FFFF
      F70000000000000000000000000000000000FFFFF70000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000ADB6B7005C6C6F0054656900DEE2E300F6F7F7009BA5A7002C4146003247
      4C0095A0A200000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00000000000000000000000000FFFFF700000000000000
      0000FFFFF700FFFFF700FFFFF700FFFFF7000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6C6C6000000
      0000F5F6F6007D8B8D0044575B0034494D003C5054002E4348001F363B00BEC5
      C600000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF0000000000000000000000000000000000FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000001C1C1C00C6C6
      C600C6C6C600A8ABAB00BBC2C300182F3400778588005B6C6F00ADB6B700E4E7
      E7000000000000000000000000000000000000000000FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF000000000000000000FFFFF70000000000FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF70000000000FFFF
      F700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555555008E8E
      8E008E8E8E00C6C6C6008E8E8E00C6C6C600D2D7D800A8B1B200000000000000
      00000000000000000000000000000000000000000000FFFFFF00FFFFFF000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF000000000000000000FFFFF700FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555555000000
      0000C6C6C600000000001C1C1C005555550055555500C6C6C600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF000000000000000000FFFFF700FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555555000000
      0000000000000000000000000000C6C6C6008E8E8E0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF0000000000FFFFFF00FFFFFF000000000000000000FFFFF700FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555555000000
      00000000000000000000C6C6C6008E8E8E000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FFFFFF00FFFFFF0000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF000000000000000000FFFFFF000000000000000000FFFFF700FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555555000000
      000000000000C6C6C6008E8E8E00000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF0000000000000000000000000000000000FFFFF70000000000FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF70000000000FFFF
      F700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000555555000000
      0000C6C6C6008E8E8E0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF00FFFFFF000000000000000000FFFFFF00FFFFFF0000000000FFFFFF00FFFF
      FF00000000000000000000000000000000000000000000000000FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700FFFFF700000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000055555500C6C6
      C6008E8E8E000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF0000000000000000000000
      00000000000000000000000000000000000000000000FFFFF700000000000000
      0000FFFFF700FFFFF700FFFFF700FFFFF7000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000800080000000000000000000000
      00000000000000000000000000000000000000000000000000001C1C1C008E8E
      8E00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D6A67E000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000202020002020200020202000202
      0200020202000202020002020200020202000202020002020200020202000202
      0200020202000202020002020200020202000000000000000000000000000000
      0000000000006363630000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007C7C7C0067676700FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE002A2A2A00D1D1D100000000000000000000000000CF976A000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000202020000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000202020000000000000000000000000000000000000000000000
      0000000000005252520000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F9F9F900858585005E5E5E00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE000E0E0E00F1F1F100D7A77E00CD966800BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B000401010000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000008C8C8C0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00009090900057575700FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE003C3C3C00C9C9C9000000000000000000CB9569000000
      00006F6DFD006964F900605BFC0000000000000000007B77FD007772FD008A86
      FD0000000000D0986B00EBD4C200000000000401010000000000000000000000
      000000000000636A6D000000000000000000000000008C979B0000000000636A
      6D00000000008C979B0000000000636A6D000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BEBE
      BE001B1B1B0000000000E4E4E400DCDCDC00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00FEFEFE00000000000000000000000000D0986B000000
      0000716BFD006C6AFD00736FFD000000000000000000706CFD008D8CFD007C76
      FD00000000000000000000000000000000000000000000000000A59A2900AAAB
      4600000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000D6D6D6008484840084848400DEDE
      DE0000000000ADADAD0000000000DEDEDE008484840084848400000000000000
      0000000000000000000000000000000000000000000000000000000000008383
      830068686800000000007F7F7F004D4D4D00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00FEFEFE000F0F0F000000000000000000D0986B000000
      0000837DFD008D8CFD007975FD0000000000000000007772FD008D8CFD007E77
      FD000000000000000000000000000000000000000000B8A31B00E2CE5600AF96
      0D00C3BD320000000000636A6D00000000000000000000000000000000000000
      000000000000000000000000000000000000E7E7E700BDBDBD00BDBDBD00EFEF
      EF0000000000D6D6D60000000000EFEFEF00BDBDBD00BDBDBD00000000000000
      00000000000000000000000000000000000000000000000000008C8C8C005F5F
      5F0000000000E9E9E9004A4A4A0068686800FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00B2B2B200F5F5F500101010000000000000000000D0986B000000
      00007B75FD008D8CFD007975FD0000000000000000007772FD008D8CFD007E77
      FD000000000000000000000000000000000000000000AF960D00F5F2E500E8E0
      AE00B9A21400C7C33F0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000008484
      840084848400FFFFFF00848484009C9C9C000000000000000000000000000000
      0000C6C6C60073737300B5B5B50000000000B4B4B40000000000000000000000
      0000E5E5E5006D6D6D00343434006F6F6F00FEFEFE00B7B7B700DFDFDF00B2B2
      B200EBEBEB0007070700F3F3F300151515000000000000000000D0986B000000
      00007571FD00ACABFE007B77FD0000000000000000007772FD009C9BFE007E77
      FD000000000000000000000000000000000002020200EADF9500000000008C97
      9B0000000000636A6D00C4C96400636A6D0000000000636A6D00000000008C97
      9B0000000000636A6D00000000008C979B000000000000000000BDBDBD008484
      8400FFFFFF006B6B6B00FFFFFF009C9C9C00000000000000000000000000F7F7
      F70094949400BDBDBD0094949400000000000B0B0B00B8B8B800DFDFDF006F6F
      6F005F5F5F0098989800000000005A5A5A00FEFEFE0039393900A7A7A7002A2A
      2A00DDDDDD003232320062626200000000000000000000000000D0986B000000
      00007D78FD00BEBDFE007C78FE0000000000000000007772FD00ACABFE007F79
      FD00000000000000000000000000000000000202020000000000000000000000
      00000000000000000000BDBB4100000000000000000000000000AA931300BBA8
      1A000000000000000000000000000000000000000000C6C6C6007B7B7B00FFFF
      FF00FFFFFF004A4A4A00FFFFFF00FFFFFF005A5A5A00EFF7F700EFEFEF00C6C6
      C600ADADAD008C8C8C00E7E7E70000000000000000002A2A2A00A9A9A9000000
      00000000000000000000000000005A5A5A00FEFEFE0049494900C1C1C1005555
      55004B4B4B000000000060606000868686000000000000000000D0986B000000
      00007571FD00C3C2FE008D89FE0000000000000000007772FD00ACABFE007F79
      FD00000000000000000000000000000000000202020000000000000000000000
      00000000000000000000B29F1D0000000000636A6D00BAAD1D00DAC33D00E2CE
      5600B8A11300ECE090000000000000000000F7F7F7005A5A5A00E7E7E700FFFF
      FF00FFFFFF00ADADAD00FFFFFF00FFFFFF00ADADAD009C9C9C00949494009494
      9400ADADAD00ADADAD0000000000000000000000000000000000141414008282
      8200777777005C5C5C00000000006E6E6E00FEFEFE005E5E5E00373737000A0A
      0A005F5F5F009494940000000000000000000000000000000000D0986B000000
      00009292FD006D6AFD009898FD0000000000000000007772FD00ACABFE007F79
      FD00000000000000000000000000000000000202020000000000000000000000
      00000000000000000000D7C35300BCAF200000000000C4AA1C00000000000000
      0000E3DECD00B29F2D00BBB730000000000063636300FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00DED6D6007B7B7B00DEDEDE00D6D6
      D6009C9C9C008C8C8C006B6B6B00DEDEDE000000000000000000000000000000
      000000000000262626000000000068686800FEFEFE005757570055555500A2A2
      A200FCFCFC000000000000000000000000000000000000000000D0986B000000
      000000000000000000000000000000000000000000007772FD00C0BFFE00807A
      FD0000000000000000000000000000000000020202000000000000000000636A
      6D00000000008C979B0000000000C7AD1E00AF960D0000000000000000000000
      00000000000000000000978D2500000000005A5A5A00FFFFFF0031313100FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF008C8C8C00FFFFFF00FFFF
      FF00FFFFFF00949494009C9C9C00000000000000000000000000000000000000
      000022222200DEDEDE000000000068686800FEFEFE004A4A4A009F9F9F000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      00000000000000000000000000000000000000000000837DFD00C6C5FE008580
      FD00000000000000000000000000000000000202020000000000000000000000
      0000000000000000000000000000E5DEBC00DAC8620000000000000000000000
      000000000000000000000000000000000000D6D6D6008C8C8C00736B6B00FFFF
      FF00B5B5B500FFFFFF00C6C6C600FFFFFF00ADADAD009C9C9C00FFFFFF00FFFF
      FF00ADADAD00BDBDBD0000000000000000000000000000000000000000001C1C
      1C00DBDBDB0000000000000000005D5D5D00FEFEFE00464646008D8D8D000000
      00000000000000000000000000000000000000000000D0986B00D0986B00D098
      6B000000000000000000000000000000000000000000AAA7FF008885FD00A8A6
      FE00000000000000000000000000000000000202020002020200000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D6D6D6009C9C9C00FFFF
      FF006B6B6B00FFFFFF0094949400FFFFFF003131310084848400FFFFFF00ADAD
      AD00B5B5B500000000000000000000000000000000000000000018181800E1E1
      E1000000000000000000000000007B7B7B001C1C1C0088888800EEEEEE000000
      00000000000000000000000000000000000000000000EBD4C200D0986B00EBD4
      C200000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CECECE002929
      290073737300FFFFFF00737373009C9C9C00949494006B6B6B00ADADAD00B5B5
      B500000000000000000000000000000000000000000014141400E9E9E9000000
      0000000000000000000000000000F3F3F300DDDDDD00F6F6F600000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000BDBD
      BD008C8C8C00848484007B7B7B00A5A5A5000000000042424200B5B5B5000000
      00000000000000000000000000000000000014141400F2F2F200000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D6A67E000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D6A67E000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D6A67E000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D6A67E000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CF976A000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000000000000000000CF976A000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000000000000000000CF976A000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000000000000000000CF976A000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C20000000000D7A77E00CD966800BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B00D7A77E00CD966800BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B00D7A77E00CD966800BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B00D7A77E00CD966800BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B000000000000000000CB9569000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000000000000000000CB9569000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000000000000000000CB9569000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000000000000000000000CB956900A3A0
      FE00A19EFE009F9AF800A19EFE00A19EFE00A09DFE00A09DFE00A19EFE00A19E
      FE00A09DFE00CC956D00EBD4C200000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B00A3A0
      FE00A19EFE00A19EFE00A19EFE00A19EFE00A09DFE00A09DFE00A19EFE00A19E
      FE00A09DFE00A09DFE0000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      00008484F7006464F6007575F700C1C1FB007575F7006464F6006464F600B6B6
      FA00C1C1FB006464F6005252F400B6B6FA000000000000000000D0986B00A3A0
      FE00A19EFE00A19EFE00A19EFE00A19EFE00A09DFE00A09DFE00A19EFE00A19E
      FE00A09DFE00A09DFE0000000000000000000000000000000000D0986B006863
      FD00BCBBFE000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000007575F700000000009E9EF9009292F800CCCCFC00CCCCFC000000
      00007474F60000000000000000006464F6000000000000000000D0986B006863
      FD009998FE00B2B0FE00A19EFE00A19EFE00A09DFE00A09DFE00A19EFE00A19E
      FE00A09DFE00A09DFE0000000000000000000000000000000000D0986B000000
      00006865FD00B9B8FE0000000000000000000000000000000000000000000000
      000000000000A7A5FE0000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      00009594FD000000000000000000000000000000000000000000D0986B000000
      0000000000007575F70000000000000000006464F600CCCCFC00000000000000
      00000000000000000000000000006464F6000000000000000000D0986B000000
      00006865FD009695FE00B2B0FE00A19EFE00A09DFE00A09DFE00A19EFE00A19E
      FE009793FE00A5A2FE0000000000000000000000000000000000D0986B000000
      0000000000006A66FD00B2B0FF00000000000000000000000000000000000000
      0000B6B4FE00625FFC0000000000000000000000000000000000D0986B000000
      0000000000009594FD000000000000000000000000000000000000000000A3A3
      FE005B58FC008D8CFD0000000000000000000000000000000000D0986B000000
      0000000000006464F6000000000000000000000000006464F6009E9EF9000000
      0000000000009292F8006464F6008484F7000000000000000000D0986B000000
      0000000000006A66FD00918EFE00B2B0FE00A19EFE00A09DFE00A19EFE009E9B
      FE00B2B0FE00625FFC0000000000000000000000000000000000D0986B000000
      00000000000000000000615FFC00B2B0FF00000000000000000000000000C3C2
      FE006561FD000000000000000000000000000000000000000000D0986B000000
      0000A3A3FE005B58FC008D8CFD00000000000000000000000000000000000000
      00009594FD000000000000000000000000000000000000000000D0986B000000
      0000000000006464F600000000000000000000000000000000007575F700CCCC
      FC000000000000000000AAAAFA006464F6000000000000000000D0986B000000
      00000000000000000000615FFC00918EFE00A09DFE00A19EFE00A19EFE00C0BE
      FE006561FD000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000625DFC00BCBBFE0000000000B6B4FE006562
      FD00000000000000000000000000000000000000000000000000D0986B000000
      0000000000009594FD0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      00008484F7007575F70000000000B6B6FA00B6B6FA00000000009191F8009E9E
      F900CCCCFC0000000000000000006464F6000000000000000000D0986B000000
      0000000000000000000000000000625DFC00A9A8FE00A19EFE00B2B0FE006562
      FD00000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000006865FD00B7B5FE00645FFC000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      000000000000000000000000000000000000000000009594FD00000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000006464F60000000000000000008484F7006464F6006464F6000000
      0000CCCCFC006464F6005252F4009191F8000000000000000000D0986B000000
      0000000000000000000000000000000000006865FD00B7B5FE00645FFC000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      000000000000000000000000000000000000000000006865FD00000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      000000000000000000000000000000000000A3A3FE005B58FC008D8CFD000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      000000000000000000000000000000000000000000006865FD00000000000000
      00000000000000000000000000000000000000000000D0986B00D0986B00D098
      6B00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D0986B00D0986B00D098
      6B0000000000000000000000000000000000000000009594FD00000000000000
      00000000000000000000000000000000000000000000D0986B00D0986B00D098
      6B00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D0986B00D0986B00D098
      6B00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EBD4C200D0986B00EBD4
      C200000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EBD4C200D0986B00EBD4
      C200000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EBD4C200D0986B00EBD4
      C200000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EBD4C200D0986B00EBD4
      C200000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000006363FF000808
      FF0000000000000000000000000000000000000000000000000000000000B5B5
      B500A5A5A500ADADAD000000000000000000E4E4E40080808000828282008282
      8200838383008282820082828200818181008181810081818100818181007D7D
      7D0084848400F3F3F30000000000FDFDFD000202020002020200020202000202
      0200020202000202020002020200020202000202020002020200020202000202
      0200020202000202020002020200020202000000000000000000D6A67E000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000003939FF000808
      FF0000000000000000000000000000000000A5A5A500A5A5A500FFFFFF005A5A
      5A00000000009C9C9C000000000000000000DDDDDD00ADADAE00EFF1F000E4E5
      E400E5E4E400E1E2E200DFDFE000DFDFDF00DDDDDD00DADADA00DADADA00D9D9
      D90091919100DBDBDB0000000000FDFDFD000202020000000000064BCF000338
      C700036AFF0000000000000000000000000000000000FBFAF900DDDEDE00DDDC
      DB00000000000202020000000000000000000000000000000000CF976A000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C2000000000000000000000000002929FF000808
      FF00000000000000000000000000000000009C9C9C007B7B7B0021212100F7F7
      F700F7F7F700949494000000000000000000DEDEDE00B2B2B200F9F2F600EDE8
      EA00EDEEEE00F0EFEF00ECECEB00E8E8E900E6E7E700E4E4E400E1E1E100E0E0
      E00093939300DBDBDB0000000000FDFDFD000401010000000000373895000540
      D2000B2EAE000000000000000000E8E8E800D7DADA00EDCFC100BA8C7800BBB8
      B600F0F0F000D5D7D700E2E3E40000000000D7A77E00CD966800BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B0000000000000000002121FF000808
      FF00000000000000000000000000000000007373730000000000F7F7F700F7F7
      F700393939004A4242000000000000000000E1E2E100B7B2B5006EC398009AD0
      B500E0D8DC00B3B8BC00C1C5C900D6D5D400D1D1D100CDCDCD00D1D1D100E0E0
      E00094949400DBDBDB0000000000FDFDFD00040101000E33B100818FAA000000
      00007775AA00212F9C00F5DDD200B97D5F00A99B9200D3917600B85E3F00A698
      8E00C6937B00A8766100CBCAC900000000000000000000000000CB956900A3A0
      FE004F85AC004B83A8004582A4009E9DFB00A09DFE005486B3005286B1005E8A
      BD00A09DFE00CC956D00EBD4C2000000000000000000000000005252FF000808
      FF005A5A5A008C8C8C00000000000000000084848400EFEFEF00FFFFFF00FFFF
      FF00D6D6D60063636B000000000000000000F0E5EA0093ADA00079CDA30064C4
      9300D0DDD600A1A4AB0094A4AF00B7C2CB00D7D8D800DCDCDB00DADADA00E2E2
      E20096969600DADADA0000000000FDFDFD00000000000543D100000000000000
      0000000000008897B900E5D1C600DB7B5700C5705100DE7E6400D4746200BD6F
      5300C6674400B1765D00DCDFE100EAECEC000000000000000000D0986B00A3A0
      FE004D85AD004D85AB005086AE00A09EFD00A09DFE004D85AD00618ABE005587
      B400A09DFE00A09DFE0000000000000000005A5A5A008C8C8C00DEDEFF000808
      FF006B6B6B0052525200000000000000000094949400EFEFEF00FFFFFF00FFFF
      FF008C8C8C00848C94000000000000000000D5D6D600A7A5A600FEFEFE00AFDB
      C500BFE2D000DBE0E800D2ECFB0082B7DE006C8EAE00E4E4E400E9E9E900E6E6
      E60097979700DBDBDB0000000000FDFDFD000000000000000000000000000000
      0000FEF6F200DABBAD00BB9E8F00E08C7200E2765800E0876700E3987B00E7A0
      8900D8826A00AA7B6500B2958900AB978E000000000000000000D0986B00A3A0
      FE005888B700608ABD005387B100A19EFE00A09DFE005085B100608ABD005688
      B500A09DFE00A09DFE0000000000000000006B6B6B0052525200000000000808
      FF002121FF0000000000000000000000000084848400F7F7F700FFFFFF008484
      84009CA5B500E7EFEF000000000000000000DADADA00A8A7A80069C999009BD4
      B800D1D9D400C5CACE0087C7E50059BFFD002673B5007794AE00DCD8D600E5E6
      E70098989800DADADA0000000000FDFDFD000000000000000000000000000000
      0000F6E7DF00DC876200E0836200E1806800C2715000E2CBC00000000000F1DA
      CD00E7A99500C8796300B75B3300C89B85000000000000000000D0986B006863
      FD004F81B8006598B3005387B100A19EFE00A09DFE005085B100608ABD005688
      B500A09DFE00A09DFE0000000000000000000000000000000000000000003131
      FF000808FF00DEDEFF00000000000000000084848400EFEFEF00848C8C00BDC6
      C600FFFFFF00000000000000000000000000E4DCE00093A69C00B0EACD0075D0
      A200E5EFEA00E5DFE40091CFDD005FD5FF0043AAF6002771AE00A0B2C200F5F1
      EE0096979800D9D9DA0000000000FDFDFD000202020000000000A59A2900AAAB
      460000000000F0E6E100E9B8A700CA715100AD90810000000000000000000000
      0000ECC0AD00D17F6300B17E6500D7D1D0000000000000000000D0986B000000
      0000425AD7006B88D3005795A600A19EFE00A09DFE005085B100688DC6005688
      B5009793FE00A5A2FE000000000000000000000000000000000000000000FFFF
      FF000808FF001818FF005A5A5A008C8C8C007B7B7B0073737300E7E7E7000000
      000000000000000000000000000000000000D9DBDA00B3B1B200FEFEFE00C5E8
      D800C4EAD800FDFCFC00D9E0E6006CD4E40046C9FF00319CEC00377FB600C9D3
      DB00A19F9E00D8D9DA0000000000FDFDFD0002020200B8A31B00E2CE5600AF96
      0D00FAF1EE00E1BAA700DC917600C46B4D00A2867700C6CACC0000000000EEEA
      E800E0A68D00C7816A00A6755F00A4938A000000000000000000D0986B000000
      00006ECF78005860EB004F7BBE00B2B0FE00A19EFE005085B1007390CF005486
      B600B2B0FE00625FFC0000000000000000000000000000000000000000000000
      00004A4AFF000808FF006B6B6B0052525200EFEFEF00F7F7F700000000000000
      00005A5A5A008C8C8C000000000000000000E4E1E200A4AAA7004CB57F008ECC
      AD00DCDBDB00D4D6D700E3DDDB00A1BCC5003FCDE70032B9FF001D8EE2005491
      BE00A2A2A200DBDBDC0000000000FCFCFD0002020200AF960D00F5F2E500E8E0
      AE00F8EAE400E09A7A00E5A38800E1927E00A95F4000A3887B00B8A8A000C885
      6700D98D7300D0846800C7603100CA977F000000000000000000D0986B000000
      000068D26C00B8E3C2004456E000918EFE00A09DFE005286B1007390CF005FA1
      A1006561FD000000000000000000000000000000000000000000000000000000
      0000FFFFFF002929FF000808FF001010FF009C9CFF00FFFFFF00000000000000
      00006B6B6B00525252000000000000000000EBE1E60096AFA200ABE4C70076CF
      A200DCEDE400F5F3F400F1F2F300F7F2F2009AD5E10023D5FA001FA8FE001688
      D9005986A800E6E1DE00FEFEFE00FDFDFD0002020200EADF9500000000000000
      0000DAC44400F8EAE400F8EFEA00EBB7A300D1816900BF735700BF6F4E00D688
      6C00D68A6F00B5846D00CBBEB900FCEDE7000000000000000000D0986B000000
      00008BDB8B0061D064008ED69700605CFA00A9A8FE005185B0007CA0C8004257
      DC00000000000000000000000000000000000000000000000000000000000000
      00005A5A5A008C8C8C009494FF001010FF000808FF000808FF000808FF000808
      FF000808FF001010FF001010FF002121FF00DCDFDD00C0B9BC00ECEDEC00BCDF
      CD00C4ECD800FEFEFE00FEFEFE00FEFEFE00A69D9F0032A0AF0006CDFF00129D
      FE001786D100A1BCD20000000000FBFCFC000202020000000000000000000000
      000000000000C5AB1D00F4E4DB00DF9B7E00DD9D8100EABBAC00D7927C00CE8F
      7200DA947700B66A4700D0D1D100000000000000000000000000D0986B000000
      0000000000000000000000000000000000006865FD005799A100545AED006ECC
      7E00000000000000000000000000000000000000000000000000000000000000
      00006B6B6B00525252000000000000000000F7F7FF008C8CFF003939FF001818
      FF001010FF001818FF001818FF001818FF00E2E3E200BAB8B9005CC490007ECF
      A600D8D8D900D4D4D400D3D3D300E1E2E30097979600D9DBE2002ECCE20000BB
      FD000F98FA003596D500C8D6E200FEFEFE000202020000000000000000000000
      000000000000E8E0BE00F8EFEB00E6B29A00F0E4DD00F3D9CD00CA785400CEB9
      AF00EFD9D000E7B59E00F9F5F200000000000000000000000000D0986B000000
      00000000000000000000000000000000000000000000455BDB00B8E0C60079D7
      7E00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000005A5A
      5A008C8C8C00000000000000000000000000EAE4E700A9BAB100D6EEE3007BCE
      A400E0F4EA00FCFCFC00FBFBFB00FBFBFB00A9ACAC00F9F9F9007F99A30004BB
      D80000B1FF0025A0E400C5DDEA00FEFEFF000202020002020200000000000000
      00000000000000000000D7C35300BCAF2000C7C33F00F9EDE800EDCEC100F8EF
      EB00E3DECD00B29F2D00BBB730000000000000000000D0986B00D0986B00D098
      6B000000000000000000000000000000000000000000A2E4A4007ED88100A1E2
      A300000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000006B6B
      6B0052525200000000000000000000000000E1E2E100C9C8C800FEFEFE00DFF8
      EC00D7F7E700FEFEFE00FEFEFE00FEFEFE00B1B1B1009FA1A300C1B5B30094D2
      DE0057C0E700BFDCED0000000000FDFEFE000000000000000000000000000000
      0000000000000000000000000000C7AD1E00AF960D00EADF9D00000000000000
      00000000000000000000978D25000000000000000000EBD4C200D0986B00EBD4
      C200000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E8E7E800ABABAB00C0C1C100C2BE
      C000B1B7B400B7B7B700B4B4B400B0B0B00099999900CACACA0000000000FFFA
      FB00FBF4F50000000000FCFDFE00000000000000000000000000000000000000
      0000000000000000000000000000E5DEBC00DAC8620000000000000000000000
      0000000000000000000000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000202020002020200020202000202
      0200020202000202020002020200020202000202020002020200020202000202
      0200020202000202020002020200020202000202020002020200020202000202
      0200020202000202020002020200020429000203110002020200020202000202
      0200020202000202100002022700020202000000000000000000B97144000000
      0000B971440000000000AD65380000000000AD65380000000000A55C2E000000
      0000A1582A0000000000000000000000000000000000B9714400000000000000
      000000000000AD653800000000000000000000000000A55C2E00000000000000
      000000000000A1582A0000000000000000000202020000000000064BCF000338
      C700036AFF0000000000000000000000000000000000000000000540D2000540
      D200000000000202020000000000000000000202020000000000A59A2900AAAB
      46000000000000000000E7E9FA003F54DF007F87E50000000000000000000000
      000000000000010165003B3BDA00E7E7F9000000000000000000BB7548000000
      0000BB75480000000000B16B3C0000000000B16B3C0000000000A75F32000000
      0000A1582A00000000000000000000000000BB754800C7815400C37D5000BF79
      4C00BD754800B9714200B56D4000B1693C00AE663700AB623400A85F3100A65B
      2E00A35A2C00A1582A00A1582A00000000000401010000000000373895000540
      D2000B2EAE00000000000000000000000000000000000044DD00493D8E003238
      99000051E90000000000000000000000000004010100B8A31B00E2CE5600AF96
      0D00C3BD3200CBD0F5003D5BE1003174FE001849EC00868CE500000000000000
      00008687E3000000EB000000FE003636D900C7815400C7815400BF794C00BF79
      4C00BF794C00B9714200B56D4000B1693C00B56D4000AB623400AB623400A65B
      2E00A35A2C00A1582A00A1582A000000000000000000BF794C0000000000FEFE
      FE0000000000B56D40000000000000000000FEFEFE00AB623400000000000000
      000000000000A35A2C000000000000000000040101000E33B100818FAA000000
      00007775AA00212F9C000056E000000000000000000021329A0000000000879F
      B4001F2D9A0000000000000000000000000004010100AF960D00F5F2E500E8E0
      AE00B9A21400B6B54C008191E900255FEF003A7DFF001747EB008A90E6008A8F
      E5000319EA00000BFF000001EB007F7FE3000000000000000000C37D52000000
      0000C37D520000000000B971440000000000B971440000000000AE6639000000
      0000A55C2E0000000000000000000000000000000000C37D5200000000000000
      000000000000B9714400000000000000000000000000AE663900000000000000
      000000000000A55C2E000000000000000000000000000543D100000000000000
      0000000000008897B9003F3A9400013DCF000732B800747BA200000000000000
      00007D7CB30019399900000000000000000000000000EADF9500000000000000
      0000DAC44400AF960D00C4C964008694E900245DEE00387BFF001848EB000E39
      EB000F3FFF00021AEA008687E30000000000D48C5F00D48C5F00C7835400CB85
      5800C7835400C37D5000BD754600BB754800BD754600B56B4000B1693C00AE65
      3700A85F3100A85F3100A55B30000000000000000000C7835400000000000000
      000000000000BD754600000000000000000000000000B1693C00000000000000
      000000000000A85F310000000000000000000000000000000000000000000000
      00000000000000000000000000000546D1000541D20000000000000000000000
      0000000000000544D1000D33B300000000000000000000000000000000000000
      000000000000C5AB1D00BDBB4100000000008A97EA00235DEE003878FF002766
      FF000D38EB008A8EE50000000000000000000000000000000000CB8558000000
      0000CB85580000000000C1794C0000000000C1794C0000000000B56B40000000
      0000AA623500000000000000000000000000CB855800D48C5F00CF895C00CB85
      5800C7815400C37D5000C1794C00BB754800B9714400B56B4000B2693B00AE65
      3700AA623500A85F3100A55B3000000000000000000000000000000000000000
      000000000000000000000000000000000000FCFEFF0062E2FD007DE7FE00C3E4
      FC00000000000000000000000000000000000000000000000000000000000000
      000000000000E8E0BE00B29F1D00000000007E89C300306AEE00498AFF003778
      FF001848EB00807FAA000000000000000000D48C5F00D48C5F00CE8A5D00CB85
      5800CE8A5D00C37D5000C37D5000BB754800C37D5000B56B4000B9714200AE65
      3700AD673800A85F3100A55B30000000000000000000CE8A5D00000000000000
      000000000000C37D5000FEFEFE000000000000000000B8704100000000000000
      000000000000AD67380000000000000000000202020000000000A59A2900AAAB
      4600000000000000000000000000DFE1F000CCE4FC00ACCFFD005CCDFA005DCB
      FE0040B5FC0070CFFD0000000000000000000202020000000000A408B000B934
      D0000000000000000000D7C3530063727900417DF0006BA9FF00306AEF00245D
      EE00387AFF001748EB0062667900000000000000000000000000D28E61000000
      0000D28E610000000000C781540000000000C781540000000000BB7548000000
      0000B1693C0000000000000000000000000000000000D28E6100000000000000
      000000000000C7815400000000000000000000000000BB754800000000000000
      000000000000B1693C00000000000000000002020200B8A31B00E2CE5600AF96
      0D00C3BD32000000000000000000C1E8FF0019D4FC0036DBFF0069FEFF0056E2
      FD0012D0FD003CC7FD00000000000000000002020200B00EB400F297ED009101
      9D00CC4CDA00E9EEFD0083A0F2005692F40090C5FF00417EF1008A9AEC008A97
      EA00245DEE00397DFF001849EC008088E500DB976800DB976800D5916400D591
      6400D5916400CF895C00CB875800C7815400CB875800C1794C00C1794C00B770
      4400B56D3E00B1693C00AE6639000000000000000000D5916400000000000000
      000000000000CB875800000000000000000000000000C0784B00000000000000
      000000000000B56D3E00000000000000000002020200AF960D00F5F2E500E8E0
      AE00B9A21400C7C33F000000000000000000ABCEFD0037D0FC00D7FDFC0099F3
      FD002EC0FD00F4F4FE000000000000000000020202008D02A00000000000FCE0
      FB00AA0AB000AA5FE300477FF00096CCFF005792F400869CEE00000000000000
      00008694E900255FEF003174FE003A50DE000000000000000000D8946700FEFE
      FD00D894670000000000CF895C0000000000CF895C00FEFCFC00C47D51000000
      0000B9714400000000000000000000000000D8946700DB976800D8946800D591
      6400D28E6100CF895C00CB875800C7815400C37D5000C1794C00BD754600B770
      4400B56D3E00B1693C00AE6639000000000002020200EADF9500000000000000
      0000DAC44400AF960D00C4C96400B6DDFD000ECAFD0029CEFD0068FCFD0056E3
      FD0012D0FD003CC7FD00000000000000000002020200FBE6FC00000000000000
      0000E776E50091019D00C68AED004C82F000809FF20000000000000000000000
      0000000000007F90E900425FE200E7E9FA00DF9B6E00DF9B6E00DB956A00DE98
      6B00DB956A00D8946700D28E6100D28E6100D28E6100CB855800C7835400C57D
      5000BB754800BD754800B77144000000000000000000DB956A00000000000000
      000000000000D28E6100000000000000000000000000C7835400000000000000
      000000000000BB75480000000000000000000202020000000000000000000000
      000000000000C5AB1D00BDBB4100DDEFFD0030B9FD0079C6FD0000A5FD004EBD
      FD0042B7FD0070CFFD0000000000000000000202020000000000000000000000
      0000000000009C00A500CA4FDB00CDD9FA00ECF0FD00000000008A03A200B015
      BB0000000000ECEEFC00CDD2F600000000000000000000000000DE986B000000
      0000DE986B0000000000D591640000000000D591640000000000CB8558000000
      0000C1794C0000000000000000000000000000000000DE986B0000000000FEFE
      FE0000000000D5916400000000000000000000000000CB855800000000000000
      000000000000C1794C0000000000000000000202020000000000000000000000
      000000000000E8E0BE00B29F1D0000000000E8E0B400BAAD1D0095CDFD00D3DA
      B200B8A11300ECE0900000000000000000000202020000000000000000000000
      000000000000FBE4FC00AA0AB00000000000F9EEFD00AC26C600E768E200F197
      ED00A708AF00FCE0FB000000000000000000DF9B6E00DF9B6E00DF9B6E00DE98
      6B00DF9B6E00D8946700D9936600D28E6100D9936600CB855800CF895C00C57D
      5000C57D5000BD754800B77144000000000000000000DF9B6E00000000000000
      000000000000D9936600000000000000000000000000CF895C00000000000000
      000000000000C57D500000000000000000000202020002020200000000000000
      00000000000000000000D7C35300BCAF2000C7C33F00C4AA1C00000000000000
      0000E3DECD00B29F2D00BBB73000000000000202020002020200000000000000
      00000000000000000000E970E400B825C800D36DE3009E00A000000000000000
      0000EB90EB00B413BA00C43AD100000000000000000000000000DF9B6E000000
      0000DF9B6E0000000000DB976A0000000000DB976A0000000000D28E5F000000
      0000C9815400000000000000000000000000DF9B6E00DF9B6E00DF9B6E00DE98
      6B00DB976800D8946700D5916400D28E6100CE8A5D00CB855800C9815400C57D
      5000C1794C00BD754800B7714400000000000000000000000000000000000000
      0000000000000000000000000000C7AD1E00AF960D00EADF9D00000000000000
      00000000000000000000978D2500000000000000000000000000000000000000
      0000000000000000000000000000C620C50091019D00FCE1FB00000000000000
      000000000000EB90EB007C06AC00C43AD1000000000000000000DF9B6E000000
      0000DF9B6E0000000000DB976A0000000000DB976A0000000000D28E5F000000
      0000C981540000000000000000000000000000000000DF9B6E00000000000000
      000000000000DB976A00000000000000000000000000D28E5F00000000000000
      000000000000C981540000000000000000000000000000000000000000000000
      0000000000000000000000000000E5DEBC00DAC8620000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FAE1FA00EB90EB0000000000000000000000
      0000000000000000000000000000EB90EB000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000EFEFEF005252
      52008C8C8C00F7F7F70000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00F7FF
      F700D6D6D600A5A5A500FFFFFF000000000000000000B9A39500634935006349
      3500634935006349350063493500634935006349350063493500634935006349
      350063493500634935006349350000000000000000000000000000000000B971
      44000000000000000000AD6538000000000000000000A55C2E00000000000000
      0000A1582A000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000ADADAD000000
      00008C848C008C8C8C0000000000B5B5B5008C848C0000000000FFFFFF00FFFF
      FF00CECECE0052525200FFFFFF00FFFFFF0000000000B9A39500FEFDF200DEDB
      D500DAD2CB00D7CBC200D3C2B700CEBAAD00C7B0A300C1A99C00BAA19300B297
      8900AB908100A5897A006349350000000000000000000000000000000000BB75
      48000000000000000000B16B3C000000000000000000A75F3200000000000000
      0000A1582A000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF000000
      0000D6D6D600D6D6D60000000000FFFFFF00D6D6D60000000000FFFFFF00FFFF
      FF009494940031393100D6D6D600FFFFFF0000000000B9A39500FFFFF600B3D0
      DD00337085009DA7AB00FADCC600F9D2B800F4C6AA00EFBDA000E7B39500DDA7
      8900D39C7D00AB908100634935000000000000000000C7815400C37D5000BF79
      4C00BD754800B9714200B56D4000B1693C00AE663700AB623400A85F3100A65B
      2E00A35A2C00A1582A00A1582A000000000000000000ADAEAD00000000008C86
      8C008C8A8C0000000000B5B6B5008C868C0000000000FEFCFE008C8A8C000000
      0000F7F7F7006B696B0052515200000000000000000000000000FFFFFF000000
      0000CED6CE00D6D6D60000000000FFFFFF00CED6CE0000000000FFFFFF00FFFF
      FF0039393900CECECE0094949400FFFFFF0000000000B9A39500FFFFF60000AF
      FC00C1FDFF002D677F00FBE4D200B7A39900B49F9200B69B8E00AF948500AD8F
      7F00DDA78900B29789006349350000000000000000000000000000000000C37D
      52000000000000000000B97144000000000000000000AE663900000000000000
      0000A55C2E0000000000000000000000000000000000FEFCFE0000000000D6D7
      D600D6D7D60000000000FEFCFE00D6D7D60000000000FEFCFE00FEFCFE00ADAE
      AD00B5B6B50052515200EFEFEF00000000000000000000000000FFFFFF000000
      0000DED6DE00D6D6D60000000000E7EFE700DED6DE0039393900FFFFFF00B5B5
      B50052525200FFFFFF007B7B7B00E7EFE70000000000BAA59600FFFFF600B6DD
      EF0000AFFC00B2D2DC00FCEDDD00FBE2CE00F9D7C000F8CDB200F4C3A600EFBD
      A000E7B39500BAA193006349350000000000000000000000000000000000C783
      54000000000000000000BD7546000000000000000000B1693C00000000000000
      0000A85F310000000000000000000000000000000000FEFCFE0000000000CED3
      CE00D6D7D60000000000FEFCFE00CED3CE0000000000FEFCFE00FEFCFE00FEFC
      FE0039383900B5B6B500FEFCFE00000000000000000000000000F7FFF7000000
      0000D6D6D6008C8C8C00313931008C8C8C007B737B00C6C6C600F7F7F7005252
      520039393900F7F7F7006B6B6B008C8C8C0000000000BCA69800FFFFF600FFFF
      F600FFFFF600FEFCF200FEF5E800FCEAD900FBDFCB00F9D4BC00F8CCB000F4C3
      A600EFBDA000C1A99C00634935000000000000000000D48C5F00CF895C00CB85
      5800C7815400C37D5000C1794C00BB754800B9714400B56B4000B2693B00AE65
      3700AA623500A85F3100A55B30000000000000000000FEFCFE0000000000DED7
      DE00D6D7D60000000000E7EBE700DED7DE0039383900FEFCFE00FEFCFE00A5A2
      A5008C8A8C007B757B00FEFCFE00000000000000000000000000FFFFFF000000
      0000D6D6D600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00000000000000000000000000FFFFFF0000000000BEA89900FFFFF60095A3
      FF002E3BA5008F96CD00FEFAF000FDF2E400FCE8D500FADCC600F9D2B800F8CA
      AE00F4C3A600C7B0A3006349350000000000000000000000000000000000CE8A
      5D000000000000000000C37D50000000000000000000B9714200000000000000
      0000AD67380000000000000000000000000000000000F7FBF70000000000D6D3
      D6008C8A8C00313831008C8A8C007B757B00C6C3C600F7F7F700949694000000
      0000CECBCE006B696B0052515200FEFCFE000000000000000000B5ADB5000000
      0000D6DED600FFFFFF00EFEFEF00525252008C8C8C00F7F7F700FFFFFF000000
      00000000000000000000000000000000000000000000C0AA9C00FFFFF6004360
      FF0095B3FF002F39AD00FFFFF600B9A79E00B6A29700B49D9100B2998B00B096
      8800F7C8AC00CCB6A9006349350000000000000000000000000000000000D28E
      61000000000000000000C78154000000000000000000BB754800000000000000
      0000B1693C0000000000000000000000000000000000FEFCFE0000000000D6D7
      D600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF00FFFF
      FF00FFFFFF000000000000000000F7FFF700D6D6D600A5A5A500FFFFFF000000
      00000000000000000000000000000000000000000000C3AE9E00FFFFF60096A3
      FF004661FF0094A1FF00FFFFF600FFFEF500FEF7EA00FCEDDD00FBE2CE00F9D7
      BF00F8CDB200CEB8AB00634935000000000000000000DB976800D8946800D591
      6400D28E6100CF895C00CB875800C7815400C37D5000C1794C00BD754600B770
      4400B56D3E00B1693C00AE6639000000000000000000B5AEB50000000000D6DB
      D60000000000FEFCFE008C8A8C0000000000F7F7F7006B696B0052515200D6D7
      D600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00CECECE0052525200FFFFFF000000
      00000000000000000000000000000000000000000000C5B0A000FFFFF600FFFF
      F600FFFFF600FFFFF600FFFFF600FFFFF600FFFDF300FEF4E700FCEAD900FADE
      CA00F9D4BC00D0BBB0006349350000000000000000000000000000000000D894
      67000000000000000000CF895C000000000000000000C47D5100000000000000
      0000B97144000000000000000000000000000000000000000000000000000000
      00000000000000000000FEFCFE00ADAEAD00B5B6B50052515200EFEFEF00F7FB
      F700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF009494940031393100D6D6D600FFFF
      FF000000000000000000000000000000000000000000C7B1A200FFFFF600FF9B
      9900B8303000FF8D6A00FFFFF600FFFFF600FFFFF600FEFAF000FDF1E400FCE7
      D500FADCC700D3C1B6006349350000000000000000000000000000000000DB95
      6A000000000000000000D28E61000000000000000000C7835400000000000000
      0000BB7548000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FEFCFE0039383900B5B6B500FEFCFE000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF0039393900CECECE0094949400FFFF
      FF000000000000000000000000000000000000000000C9B3A500FFFFF600FF83
      7E00FFC3C400B7252500FFFFF600BAAAA100B8A69C00B6A29600B39D9000B198
      8B00FBE4D200D6C7BD00634935000000000000000000DF9B6E00DF9B6E00DE98
      6B00DB976800D8946700D5916400D28E6100CE8A5D00CB855800C9815400C57D
      5000C1794C00BD754800B7714400000000000000000000000000000000000000
      00000000000000000000FEFCFE00A5A2A5008C8A8C007B757B00FEFCFE00FEFC
      FE00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FFFFFF00B5B5B50052525200FFFFFF007B7B7B00E7EF
      E700F7FFF70000000000000000000000000000000000CBB5A600FFFFF600FFA9
      A900FF827F00FF989300FFFFF600FFFFF600FFFFF600FFFFF600FFFEF400FDF7
      EB00FCEDDD00D7CCC3006349350000000000000000000000000000000000DF9B
      6E000000000000000000D99366000000000000000000CF895C00000000000000
      0000C57D50000000000000000000000000000000000000000000000000000000
      000000000000FEFCFE009496940000000000CECBCE006B696B0052515200F7F7
      F700000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F7F7F7005252520039393900F7F7F7006B6B6B008C8C
      8C00FFFFFF0000000000000000000000000000000000CBB5A600FFFFF600FFFF
      F600FFFFF600FFFFF600FFFFF600FFFFF600FFFFF600FFFFF600FFFFF600FEFC
      F300FDF4E700FCEADA006349350000000000000000000000000000000000DF9B
      6E000000000000000000DB976A000000000000000000D28E5F00000000000000
      0000C98154000000000000000000000000000000000000000000000000000000
      00000000000000000000FEFCFE00FEFCFE0000000000FEFCFE00FEFCFE000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000FFFFFF00FFFFFF0000000000F7FFF700FFFF
      FF000000000000000000000000000000000000000000CCB6A700CCB6A700CAB5
      A600CAB3A400C8B2A300C5AFA100C3AD9E00C0AB9C00BEA99A00BCA69700BAA4
      9600B8A39400B8A39400B8A39400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000053A85A0051A658004FA356004EA1
      54004C9F52000000000000000000000000000000000000000000000000004290
      4600418E4500408D43003F8B42003E8A410053A85A0051A658004FA356004EA1
      54004C9F52000000000000000000000000000000000000000000000000004290
      4600418E4500408D43003F8B42003E8A410053A85A0051A658004FA356004EA1
      54004C9F52000000000000000000000000000000000000000000000000004290
      4600418E4500408D43003F8B42003E8A41000000000000000000EBD4C2000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000054AB5C0083BF89007DBB830054A6
      5B00EFF6EF000000000000000000000000000000000000000000000000000000
      0000429046006DAD71006EAE73003F8C420054AB5C0083BF89007DBB830054A6
      5B00EFF6EF000000000000000000000000000000000000000000000000000000
      0000429046006DAD71006EAE73003F8C420054AB5C0083BF89007DBB830054A6
      5B00EFF6EF000000000000000000000000000000000000000000000000000000
      0000429046006DAD71006EAE73003F8C42000000000000000000CF976A000000
      000000000000FEFEFE0000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C2000000000056AD5F0083C08A0073B77A006CB4
      730059A86000EFF6EF00000000000000000000000000000000000000000080B5
      82005AA05E005EA664006CAD7000408D440056AD5F0083C08A0073B77A006CB4
      730059A86000EFF6EF00000000000000000000000000000000000000000080B5
      82005AA05E005EA664006CAD7000408D440056AD5F0083C08A0073B77A006CB4
      730059A86000EFF6EF00000000000000000000000000000000000000000080B5
      82005AA05E005EA664006CAD7000408D4400EBD3C000CF976A00BA753C00C893
      6600C6916600C5916600C5916600D0986B00D0986B00D0986B00D0986B00D098
      6B00D0986B00D0986B00D0986B00D0986B0058B0610057AE5F006CB6730084C0
      8A006EB5740058A25800C08F6E00BA896A00B8876800B5846600699154005DA5
      610075B379005FA4630047944C00418F450058B0610057AE5F006CB6730084C0
      8A006EB5740058A25800C08F6E00BA896A00B8876800B5846600699154005DA5
      610075B379005FA4630047944C00418F450058B0610057AE5F006CB6730084C0
      8A006EB5740058A25800C08F6E00BA896A00B8876800B5846600699154005DA5
      610075B379005FA4630047944C00418F45000000000000000000CB9569000000
      000000000000FBF7F40000000000000000000000000000000000000000000000
      000000000000D0986B00EBD4C200000000005AB36300000000008CC791006CB6
      73006AB4710085C18A0000000000FFFFFE00FFFFFE00FFFFFE007CB67F005FA7
      640062A867004F9A5300EEF5EE00439147005AB36300000000008CC791006CB6
      73006AB4710085C18A0000000000FFFFFE00FFFFFE00FFFFFE007CB67F005FA7
      640062A867004F9A5300EEF5EE00439147005AB36300000000008CC791006CB6
      73006AB47100528D8900EFEEFD00FEFEFD00FEFEFD00FEFEFD007CB67F005FA7
      640062A867004F9A5300EEF5EE00439147000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000000000000081B1
      730084C38900FFFFFD00CE976900FEFEFC00FEFEFB00CE976900FEFEF9007FB8
      820051995200EEF5EE00000000000000000000000000000000000000000081B1
      730084C38900BACFBD009CBC9F00E3EBE200FBFAF700CCE0CD00C7DAC5007FB8
      820051995200EEF5EE00000000000000000000000000000000000000000081B1
      73007BB988005D5BFA00D6D3F800FDFDFB00FDFDFA00FDFCF800F6F6F8005D96
      820051995200EEF5EE0000000000000000000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000DEBB
      A000FBF7F200CE976900BA753C00C7926500C6916500C5906400C48F6300F5F0
      E800C9A48E00000000000000000000000000000000000000000000000000DEBB
      A000FFFFFD00C9C8F700D9E9DA0088BA8900A1D0A70083C18B00E3E6EF00FCFC
      F900C9A48E00000000000000000000000000000000000000000000000000DEBB
      A000F9F9FC00ADA8F600897FEE00F6F6FA00FDFDFA00FBFAF700D7D6F400615F
      F500C9A48E000000000000000000000000000000000000000000D0986B000000
      0000000000000000000055AB5D0053A95B0052A7590050A457004EA255000000
      000000000000000000000000000000000000000000000000000000000000E1BE
      A200FEFEFD00FEFEFC00CB946700FDFDFA00FCFCF800CE966800FBF9F500FCF9
      F700CBA79000000000000000000000000000000000000000000000000000E1BE
      A200FEFEFD008380F700C69FB400E3945D00C5AE7B00E9EFE3009390F600FCF9
      F700CBA79000000000000000000000000000000000000000000000000000E1BE
      A200FDFCFB00ECEAF8006F6AF400D3D2F800FBFBF700F5F3F400AEABEF00918E
      F100CBA790000000000000000000000000000000000000000000D0986B000000
      0000000000000000000057AE5F008CC6920087C38D0057AA5E00EFF6EF000000
      000000000000000000000000000000000000000000000000000000000000E3BF
      A300FEFEFD00FDFDFB00D0986900FCFBF700FBF9F600FBF7F400FBF5F100FBF7
      F400CDA89200000000000000000000000000000000000000000000000000E3BF
      A300FEFEFD00DCD0E2006E4FC700EEE3E300ECB18700E7B391009471B200FBF7
      F400CDA89200000000000000000000000000000000000000000000000000E3BF
      A300FDFCFB00F9F7F7009592F600A19FF400E0DEF500CAC6F2007671EF00D2CC
      EF00CDA892000000000000000000000000000000000000000000D0986B000000
      0000000000000000000059B161008CC793007DBF850072B979005BAB6200EFF6
      EF0000000000000000000000000000000000000000000000000000000000E4C1
      A500FEFEFC00CE966900D1996A00CE956700FBF7F300F9F4EF00F8F2EC00F9F4
      F100CFAA9300000000000000000000000000000000000000000000000000E4C1
      A500FEFEFC00ECBD9500B9A6D3009A97F300BDB7ED008C7BD500EBD0C000F9F4
      F100CFAA9300000000000000000000000000000000000000000000000000E4C1
      A500FDFDFA00FBFAF700D4D2F6007D7AF200817FF0007F7CEF00A09CEE00F0EB
      F000CFAA93000000000000000000000000000000000000000000D0986B000000
      000000000000000000005AB3640059B1620070BB78008DC7940073BA7A005EAE
      6600000000000000000000000000000000000000000000000000F0F9F10069BB
      6E008DCD9300FCFBF800D3996A00FAF7F200F9F4EF00F6EFEA00F5ECE60080BC
      83007AA86C000000000000000000000000000000000000000000F0F9F10069BB
      6E008DCD9300EFD5BD00F9F6F500CDCAF100B9B6EF00EAE3EA00F5ECE60080BC
      83007AA86C000000000000000000000000000000000000000000F0F9F10069BB
      6E008DCC9200FBF9F500F4F1F500ABA8F1006D6BEF009F9BEC00E1D9E70080BC
      83007AA86C0000000000000000000000000000000000FEFEFE00D0986B000000
      000000000000000000005CB66600000000008DC9930071BB79006FB9770086C3
      8C000000000000000000000000000000000065C37000F1F9F2006BC2750080C9
      890079C482008BCC9200FEFEFD00FEFDFD00FEFDFC00FDFCFB0088C48D006EB8
      75006CB5730088C28D00000000004EA2550065C37000F1F9F2006BC2750080C9
      890079C482008BCC9200FEFEFD00FEFDFD00FEFDFC00FDFCFB0088C48D006EB8
      75006CB5730088C28D00000000004EA2550065C37000F1F9F2006BC2750080C9
      890079C482008BCC9200FEFEFD00FEFDFD00FEFDFC00FDFCFB0088C48D006EB8
      75006CB5730088C28D00000000004EA255000000000000000000D0986B000000
      000000000000000000000000000000000000000000008DCA930086C58C000000
      00000000000000000000000000000000000066C472006BC5750083CC8C009BD3
      A4007BC7840088B77300DCAB8500D9A88200D7A68000D3A37E0061AF630075BD
      7D008CC793006DB6730052A7590050A5570066C472006BC5750083CC8C009BD3
      A4007BC7840088B77300DCAB8500D9A88200D7A68000D3A37E0061AF630075BD
      7D008CC793006DB6730052A7590050A5570066C472006BC5750083CC8C009BD3
      A4007BC7840088B77300DCAB8500D9A88200D7A68000D3A37E0061AF630075BD
      7D008CC793006DB6730052A7590050A557000000000000000000D0986B000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000067C673009DD6A50092D19B007ECA
      870094D49C000000000000000000000000000000000000000000F0F8F10062B5
      6A0076BD7E007EC086008AC5900052A85A0067C673009DD6A50092D19B007ECA
      870094D49C000000000000000000000000000000000000000000F0F8F10062B5
      6A0076BD7E007EC086008AC5900052A85A0067C673009DD6A50092D19B007ECA
      870094D49C000000000000000000000000000000000000000000F0F8F10062B5
      6A0076BD7E007EC086008AC5900052A85A0000000000D0986B00D0986B00D098
      6B00000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000068C77400A1D8A9009ED6A70065C3
      710000000000000000000000000000000000000000000000000000000000F0F8
      F1005FB467008DC894008EC9950054AA5C0068C77400A1D8A9009ED6A70065C3
      710000000000000000000000000000000000000000000000000000000000F0F8
      F1005FB467008DC894008EC9950054AA5C0068C77400A1D8A9009ED6A70065C3
      710000000000000000000000000000000000000000000000000000000000F0F8
      F1005FB467008DC894008EC9950054AA5C0000000000EBD4C200D0986B00EBD4
      C20000000000FDFBFA00FEFEFE00FCF9F6000000000000000000000000000000
      00000000000000000000000000000000000068C7740068C7740067C6730066C5
      720065C371000000000000000000000000000000000000000000000000005CB6
      66005BB4640059B2620058AF600056AD5E0068C7740068C7740067C6730066C5
      720065C371000000000000000000000000000000000000000000000000005CB6
      66005BB4640059B2620058AF600056AD5E0068C7740068C7740067C6730066C5
      720065C371000000000000000000000000000000000000000000000000005CB6
      66005BB4640059B2620058AF600056AD5E000000000000000000D0986B000000
      00000000000000000000FEFEFE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00001309D20000000000000000000000000000000000FDFEFE00FDFEFE00FDFE
      FE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFE
      FE00FDFEFE00FDFEFE00FDFEFE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E7D6D600F7EFE700FFF7EF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F7E7DE00C6B5BD00CEC6C600FFF7EF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00001309D200000000000000000000000000FDFEFE000000000000000000ACAC
      AC00ACACAC00ACACAC00ACACAC00ACACAC00ACACAC00ACACAC00ACACAC00ACAC
      AC00ACACAC000000000000000000FDFEFE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000D6B5BD00847BA5006B738400DED6D6000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000F7E7
      DE00AD94A5006373AD006384A500DED6D6000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FDFEFE0000000000F3F3F300E9E9
      E900EAEAEA00EAEAEA00EAEAEA00EBEBEB00EBEBEB00ECECEC00EDEDED00EEEE
      EE00EEEEEE00D6D6D60000000000FDFEFE000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000D6B5
      BD00847BA5004A73C6004AA5F700D6DEE7000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000F7E7DE00AD94
      A5006373AD00398CE70052BDFF00DEE7EF001309D2001309D200000000000000
      00001309D2001309D20000000000000000001309D2001309D200000000000000
      00001309D2001309D200000000001309D200FDFEFE00ACACAC00FDFEFE00F3F3
      F300F3F3F3007A7A7A00000000000000000000000000000000006C6C6C00F8F8
      F800F9F9F900EEEEEE00ACACAC00FDFEFE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D6B5BD00847B
      A5004A73C60042A5FF0073C6FF00F7EFF7000000000000000000000000000000
      00000000000000000000000000000000000000000000F7E7DE00AD94A5006373
      AD00398CE70052B5FF00BDDEF700000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FDFEFE00ACACAC00FDFEFE00F2F2
      F200F3F3F300F3F3F300B1B1B1000000000000000000B2B2B200F7F7F700F7F7
      F700F8F8F800EEEEEE00ACACAC00FDFEFE000000000000000000000000000000
      00000000000000000000000000000000000000000000D6B5BD00847BA5004A73
      C60042A5FF0073C6FF00F7EFF700000000000000000000000000000000000000
      000000000000000000000000000000000000F7E7DE00AD94A5006373AD00398C
      E70052B5FF00BDDEF70000000000000000000704080007040800070408000000
      0000070408000704080007040800000000000704080007040800000000000000
      000000000000000000000000000000000000FDFEFE00ACACAC00FDFEFE00F2F2
      F200F2F2F200F3F3F300F3F3F3000000000000000000F6F6F600F6F6F600F7F7
      F700F7F7F700EDEDED00ACACAC00FDFEFE000000000000000000F7E7DE00EFCE
      C600E7B5A500DEAD9C00DEB5A500EFCEC600EFDED6008484A5004A73C60042A5
      FF0073C6FF00F7EFF70000000000000000000000000000000000F7E7DE00E7BD
      B500DEAD9C00DEAD9C00DEAD9C00E7C6BD00DECECE006B73A500398CE70052B5
      FF00BDDEF7000000000000000000000000000000000007040800000000000000
      0000000000000704080000000000000000000704080000000000070408000000
      00001309D200000000000000000000000000FDFEFE00ACACAC00FDFEFE00F1F1
      F100F2F2F200F2F2F200F3F3F3000000000000000000F5F5F500F6F6F600F6F6
      F600F7F7F700ECECEC00ACACAC00FDFEFE0000000000F7DED600DEB5A500DEB5
      A500EFDEBD00EFDEBD00E7BD9C00D6AD9C00D6ADA500A59494004AADFF0073C6
      FF00F7EFF70000000000000000000000000000000000E7CEC600DEAD9C00EFD6
      BD00FFF7CE00FFFFD600FFE7BD00DEB59C00D6AD9C00B5A5A50063B5F700BDDE
      F700000000000000000000000000000000000000000007040800000000000000
      0000000000000704080000000000000000000000000000000000070408000000
      00001309D200000000000000000000000000FDFEFE00ACACAC00FDFEFE00F0F0
      F000F1F1F100F2F2F200F2F2F2000000000000000000F5F5F500F5F5F500F6F6
      F600F6F6F600ECECEC00ACACAC00FDFEFE00F7E7DE00DEB5A500EFDED600FFFF
      EF00FFFFE700FFFFDE00FFFFD600FFF7C600E7B59C00D6AD9C00D6D6DE00F7EF
      F70000000000000000000000000000000000F7DED600DEAD9C00FFF7EF00FFFF
      F700FFFFE700FFFFDE00FFFFCE00FFF7C600F7C6A500DEADA500F7E7DE000000
      0000000000000000000000000000000000000000000007040800000000000000
      0000000000000000000007040800000000000000000007040800070408000000
      000000000000000000000000000000000000FDFEFE00ACACAC00FDFEFE00F0F0
      F000F0F0F000F1F1F100F2F2F2000000000000000000F4F4F400F5F5F500F5F5
      F500F6F6F600EBEBEB00ACACAC00FDFEFE00E7C6BD00DEB5A500FFFFFF00FFFF
      FF00FFFFFF00C6732900F7E7B500FFEFB500FFF7C600DEAD9C00EFCEBD000000
      000000000000000000000000000000000000DEB5AD00EFD6B500FFFFFF00FFFF
      FF00FFFFF700FFFFDE00FFFFCE00FFE7B500FFF7C600DEB5A500EFCEC6000000
      0000000000000000000000000000000000000000000007040800000000000000
      0000070408000000000007040800000000000000000000000000070408000000
      000000000000000000000000000000000000FDFEFE00ACACAC00FDFEFE00F0F0
      F000F0F0F000F0F0F000F1F1F1000000000000000000F3F3F300F4F4F400F5F5
      F500F5F5F500EBEBEB00ACACAC00FDFEFE00DEAD9C00F7EFC600FFFFE700EFDE
      CE00EFDECE00C6732900F7D69C00F7CE9C00FFEFBD00F7D6B500E7B5A5000000
      000000000000000000000000000000000000DEAD9C00FFF7CE00FFFFEF00E7D6
      C600E7D6C600E7D6C600E7D6C600E7D6C600FFEFBD00EFCEB500E7B5A5000000
      0000000000000000000000000000000000000704080007040800000000000000
      0000000000000704080007040800000000000704080007040800070408000000
      00001309D200000000000000000000000000FDFEFE00ACACAC00FDFEFE00AEAE
      AE00F0F0F000F0F0F000F0F0F0000000000000000000F3F3F300F3F3F300F4F4
      F400B1B1B100EAEAEA00ACACAC00FDFEFE00DEAD9C00FFFFD600FFFFDE00C673
      2900C6732900C6732900C6732900CE630000FFE7AD00FFF7BD00DEAD9C000000
      000000000000000000000000000000000000DEAD9C00FFFFCE00FFFFDE00CE63
      0000CE630000CE630000CE630000E7B58400FFE7B500F7DEB500DEAD9C000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00001309D200000000000000000000000000FDFEFE00ACACAC00FDFEFE000000
      0000AEAEAE00F0F0F000F0F0F0000000000000000000F2F2F200F3F3F300B0B0
      B00000000000EAEAEA00ACACAC00FDFEFE00DEAD9C00FFE7BD00FFFFCE00FFEF
      BD00F7DEAD00C6732900F7D69400F7D69400FFEFBD00F7E7BD00DEB5A5000000
      000000000000000000000000000000000000DEAD9C00FFE7BD00FFFFCE00FFFF
      CE00FFFFCE00FFFFCE00FFFFCE00FFDEB500FFF7CE00EFCEB500E7B5A5000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FDFEFE00ACACAC00FDFEFE000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000EAEAEA00ACACAC00FDFEFE00DEBDAD00E7BDA500FFFFC600FFF7
      C600FFFFC600C6732900F7D69400FFEFDE00FFFFE700E7C6A500E7C6BD000000
      000000000000000000000000000000000000DEB5AD00E7BD9C00FFF7C600FFE7
      B500FFE7B500FFEFBD00FFE7C600FFFFF700FFFFEF00E7BDA500EFCEC6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FDFEFE0000000000FDFEFE00FDFE
      FE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFE
      FE00FDFEFE00E0E0E00000000000FDFEFE00F7E7DE00DEAD9C00FFDEAD00FFF7
      BD00FFE7AD00FFEFBD00FFEFCE00FFFFFF00F7EFDE00DEAD9C00F7E7DE000000
      000000000000000000000000000000000000F7E7DE00DEAD9C00F7D6AD00FFF7
      BD00FFE7B500FFE7B500FFF7E700FFFFFF00E7CEBD00DEB5AD00F7E7DE000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00001309D200000000000000000000000000FDFEFE000000000000000000ACAC
      AC00ACACAC00ACACAC00ACACAC00ACACAC00ACACAC00ACACAC00ACACAC00ACAC
      AC00ACACAC000000000000000000FDFEFE0000000000EFD6CE00DEAD9C00EFC6
      A500FFE7BD00FFF7BD00FFEFC600EFCEBD00DEAD9C00EFD6CE00000000000000
      00000000000000000000000000000000000000000000EFD6CE00DEAD9C00E7BD
      9C00F7E7BD00FFF7C600EFD6B500D6ADA500DEB5A500F7DED600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00001309D20000000000000000000000000000000000FDFEFE00FDFEFE00FDFE
      FE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFEFE00FDFE
      FE00FDFEFE00FDFEFE00FDFEFE00000000000000000000000000F7DED600DEB5
      AD00DEAD9C00DEAD9C00DEAD9C00DEB5AD00F7E7DE0000000000000000000000
      0000000000000000000000000000000000000000000000000000F7E7DE00D6AD
      9C00DEAD9C00DEAD9C00DEB5A500E7CEBD00F7E7DE0000000000000000000000
      000000000000000000000000000000000000FEFEFE00FCFCFC00FBFBFB00FBFB
      FB00FBFBFB00FBFBFB00FBFBFB00FBFBFB000000000000000000F6F6F70080CA
      FD0080CAFD00F6F6F70000000000000000000000000000000000F4F5FD00B0B8
      F100334FE100989FEA00FCFCFE0000000000FCFCFE009898E8002B2BDD00AFAF
      EE00F4F4FC000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000B5B5
      B500CECECE00C6C6C600C6C6C600EFEFEF000000000080808000808080006969
      690022313B006161610080808000808080008080800080808000808080008080
      800080808000808080008080800080808000EFEFEF00C6C6C600BEBEBE00C0C0
      C000C0C0C000C0C0C000C0C0C000C5C5C500A7ADB2007199BA00979EA9002EC0
      F6002EC0F600979EA900779CBC00DCE6EE00000000008E7555006A5B6C002242
      CA00347AFF001F46DF00594C7B00856C5400594B7900090ADD000001FF001511
      C2006A5569008F744E0091887B00000000000000000000000000000000000000
      00000000000000000000000000000000000000000000000000007B7B7B008C8C
      8C005A5A5A00BDBDBD00D6D6D600F7F7F700C4A08D00BB8E7C00BC917E004244
      49000084D500074B71006D564C00B8907C00BC927F00BC927F00BC927F00BC92
      7F00BC927F00BD917E00BC917E0080808000B0823E00A9772D00A7752900A876
      2A00A8762A00A8762A00A9752900A6743200379FD3000EE2FF002D99CD005ACC
      D8005ACCD8002B97CC001AE8FF0050CDFF00E4A96400F3C88B00E4BC8B009486
      A5003161E400387DFF002C4EDC00655AAC001E32DB00031CFF001213DE009275
      9900E7B98500F6B454008C6F480000000000848484006B6B6B00F7F7F700F7F7
      F700F7F7F700FFFFFF00FFFFFF009C9C9C007B7B7B00DEDEDE00424242000000
      0000636363007B7B7B005A5A5A00CECECE00C59A7C00D5CABD00C2B8AA001357
      770000A1EF0000A5EE000081B500165062004F565400C6C0B400C9C2B600D1C6
      B600D2C6B700D9CBBD00BC917E0080808000EABE9400FFEDD000FFE9C900FFE8
      C700FFE7C400FFE6C200FFE5BF00FFE5BB00D2D6CB004CC7F30062E2EE00CFFF
      FC00CFFFFC0069E9F40048B0D600CFDEEB00E6C2A500FFFCEF00FFF7E700FFF6
      E400A3A7DB003E6AE9003172FC001E57F5001248FB00263DE300A39ACB00FFEE
      CD00FFF2D500F5C481008A714F00000000005252520073737300E7E7E700CECE
      CE00CECECE00DEDEDE00D6D6D600848484007B7B7B0021212100F7F7F700F7F7
      F7006B6B6B00ADADAD0039393900B5B5B500BC917E00E5FFFF00B2C8CA000079
      A20000B0F10000B1E90000BAF20000C0FC00057A9C000F5D7A00295668007982
      8600DAEAF100E9FFFF00BC917E0080808000E4C09F00FFFBEB00FFF5E000FFF4
      DD00FFF2DA00FFF1D700FFF1D400FFEFD100FFF6C50086B5E10017A1BD00EBFF
      FE00EBFFFE002BB5CD007091B50000000000E4BF9E00FFF9E900FFF4E100FFF3
      DE00FFF2DB006573D6003776F700448AFF001D57F6006567CA00FFECC900FFEB
      C600FFEECC00F2C07A008A714F0000000000B5B5B500EFEFEF00C6C6C600D6D6
      D600D6D6D600D6D6D600D6D6D6006363630000000000F7F7F700F7F7F7003939
      390021212100B5B5AD008C8C8C00E7E7E700BC917E00E0FBFF00ABB8BD000096
      BB0000C9FB0000C7F20000BCED0000BEEE0000BEF40000B6F80000AAE3000066
      8C0058666E00BCCAD000B98F7C0080808000E4C09F00FFFAEB00FFF4E100FFF3
      DE00FFF1DB00FFF0D800FFEFD400FCEFD0009DCBE00030D3F30059DCF100B1F5
      F200B2F5F2005FE5FE0036CCE8009ECDF000E4BF9E00FFFBEE00FFF6E600FFF5
      E300A3ABDE00527EEA005795FB003676F6003172FB003559E400A39BCB00FFED
      C900FFF0D000F2C17C008A704F0000000000E7E7E700D6D6D6009C9C9C00BDBD
      BD00CEFFFF00CEFFFF00CEFFFF006B6B6B00EFEFEF00FFFFFF00FFFFFF00D6D6
      D6005A5A5A00ADB5B500A5A5A50000000000BC917E00DAF2F6007C7E80005EA7
      B4000FE0FA0000DFFF0000C3F30000C4F00000B4EA0000ABE60000A8E70000B5
      F30000B2EC0013506C0072594F0080808000E4C0A000FFFCEF00FFF6E500FFF5
      E100FFF3DE00FFF2DB00FFF1D700F9ECD40063C4F00000B8ED0054A9D90047C5
      DF0044C2DA0041849B0014CEFD0064CCFF00E4BF9E00FFF9E800F0E9E2009EAD
      E100598CEF0081BEFF004A7BEB006E77D4003764E800377EFF002850E5009B96
      CC00F0E3D100F3C47F008A704F0000000000EFEFEF00DEDEDE009C9C9C00C6C6
      C600CEFFFF00CEFFFF00CEFFFF0073737300EFEFEF00FFFFFF00FFFFFF008C8C
      8C0063738400E7E7E700ADADAD0000000000BC917E009AABAE00584E4D00E4A6
      9D00C5A69C0063ADB40012B9E00000C7F90000AEEC0000AAE60000BDF200009B
      D8001E5A720086979C00A781700080808000E4C1A100FFFFF600FFF9EC00FFF8
      E800FFF6E500FFF5E300FFF4E000FFF6DB00FBEDD900BFD2E200E8E0D9002FC5
      FF0033C5FD009B6B2C00909BAC00F7F8F900DFB28B00E7BF9500B19FA8004577
      E1009AD1FF005789EB009688AE00DEB798009585AB002F5FE700347BFF002C42
      D100B399AB00E9B26C008C724F0000000000D6D6D600E7E7E7009C9C9C00C6C6
      C600CEFFFF00CEFFFF00CEFFFF0073737300F7F7F700FFFFFF0084848400949C
      AD009CADBD00EFEFEF00C6BDBD00FFFFFF00BC917E00D8F0F300B6C3C9006A6B
      6C0086655F00C57F6F00B27E6E0063848C00178AC30000A6EF000096E2003556
      6900D7E6ED00E7F8FF00BC917E0080808000DFB59100ECCFB600EACDB300EACD
      B300EACDB300EACDB300EACDB300EACCB300EECEB200F9D3B100F3D1B1009EB5
      C300A8B6B600B0762500CBCBCB0000000000E0B79500EED8C200E4CEC000A7AB
      CB004A7FE5009097C900EBD1B700EED2B500EBCFB3008F8BBF003554D700A598
      B900E5CAB500E9B36E008B714F000000000063636300C6C6C600A5A5A500C6C6
      C600CEFFFF00CEFFFF00CEFFFF007B7B7B00EFEFEF00848C8C00A5ADB500DEE7
      F700ADBDC600DEDEDE0063636300DEDEDE00BC917E00E3FCFF00E7F8FF00E7F8
      FF0070777A0088503B00B76C5300B5695100AA5E4200625C680019416300727B
      7E00E7F8FF00E7F8FF00BC917E0080808000E0B79200ECD3BB00ECD0B600ECCF
      B400ECCFB300ECCEB000ECCEAF00ECCDAD00ECCCAC00ECCBAA00ECCBA800F3CE
      A700F7CA9B00A7742700C0C0C000FBFBFB00E5C1A20000000000FFFFF800F6F7
      F500C6D2EF00F3F3EE00FFFBEB00FFFAE700FFF8E500F3ECDF00C6C4DA00F6ED
      DA00FFF8E000F3C684008A704F000000000052525200BDBDBD00A5A5A500C6C6
      C600CEFFFF00CEFFFF00CEFFFF006363630073737300CECECE00CEFFFF00EFF7
      FF00B5C6CE00D6D6D6004A4A4A00D6D6D600BC917E00E2FCFF00E7F8FF00D2E2
      E80064494000B25F3F007A2F0E004F5051006E585200A557380046190800A1AD
      B200E7F8FF00E7F8FF00BC917E0080808000E5C3A40000000000FFFFF900FFFF
      F600FFFEF300FFFCEF00FFFCED00FFFAE800FFF9E500FFF7E200FFF6DE00FFF7
      DD00FFEAC600A8762A00C0C0C000FBFBFB00E4C0A100FFFEFA00FFF9F200FFF9
      EF00FFF8EC00FFF7E900FFF6E700FFF5E300FFF3E100FFF2DD00FFF1DA00FFF1
      D800FFF4DE00F2C483008A704F0000000000CECECE00E7E7E7009C9C9C00C6C6
      C600CEFFFF00CEFFFF00CEFFFF00E7E7E700DEDEDE00CEFFFF00CEFFFF00CEFF
      FF00BDC6CE00F7F7F700BDBDBD00F7F7F700BC917E00E3FCFF00E7F8FF008088
      8B00A3685600B25F3F005D2E1900C8D7DD00DEEEF5007F878B00686E7100E4F5
      FC00E7F8FF00E7FDFF00BC917E0080808000E4C1A300FFFFFD00FFFAF300FFFA
      F000FFF9EE00FFF7EB00FFF7E800FFF5E500FFF4E200FFF2DE00FFF1DB00FFF2
      DA00FFE6C400A8762A00C0C0C000FBFBFB00E4C0A200FFFFFD00FFFBF500FFFB
      F300FFFAF000FFF8ED00FFF8EA00FFF7E700FFF5E400FFF4E200FFF3DE00FFF2
      DC00FFF5E200F2C484008A704F0000000000EFEFEF00DEDEDE009C9C9C00C6C6
      C600CEFFFF00CEFFFF00CEFFFF00CEFFFF00CEFFFF00CEFFFF00CEFFFF00CEFF
      FF00C6C6CE00E7E7E700D6D6D60000000000BC917E00E8FFFF00D9E9F0006854
      5000C47F6900A1563A0065615F00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E7F8
      FF00E5F8FF00ECFFFF00BC917E0080808000E4C1A40000000000FFFCF800FFFD
      F700FFFCF600FFFCF500FFFCF300FFFBF000FFFAEE00FFF8EA00FFF7E700FFF8
      E700FFECCF00A8762B00C0C0C000FBFBFB00E4C0A200FDFEFD00FDFAF700FDFB
      F600FDFBF600FDFBF600FDFBF500FDFCF300FDFAF100FDF9EE00FDF8EB00FDF7
      E900FEFBF000F1C98F008A704F0000000000EFEFEF00DEDEDE009C9C9C00B5B5
      B500C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6C600C6C6
      C600B5B5B500DEE7DE00D6D6D60000000000BC917E00DCEBE600878B8800A97A
      7000C77F69006F3E2B00AFB4AE00E5EADE00E5E8E200E2E4DD00DBE1DB00D9DD
      D800D9DFD900E4EBDE00BC917E0080808000E2BA9A00F9E5D500F7E0CA00F7DD
      C400F7D9BC00F7D6B400F7D2AE00F7D0A700F7CFA600F7CEA400F7CEA200F7CD
      A100FECF9C00A8752800BFBFBF00FBFBFB00E3BA9800F8E1CD00F8DBC200F8D7
      B900F8D2AF00F8CDA400F8C89A00F8C59200F8C38E00F8C28D00F8C28B00F8C1
      8A00FAC59200F4B15F008A714E0000000000CECECE00EFEFEF00B5B5B5009C9C
      9C009C9C9C009C9C9C00A5A5A500A5A5A500A5A5A5009C9C9C00A5A5A5009C9C
      9C00A5A5A500F7F7F700BDBDBD00F7F7F700B2580000954800005A371D00D39A
      8D00BA7762004F280A00AD530000AE560000AD530000AC530000AD540000B45F
      0E00B75E040099582900AF6A2B0080808000E5B69000FFD5AE00FFC69100FFB6
      6F00FFA44B00FF912700FF820800FF770000FF760000FF760000FF760000FF76
      0000FF7C0000A96C1700C6C6C600FBFBFB00E3B69000FFD4AD00FFC79300FFB8
      7400FFA75100FF952F00FF860F00FF7A0000FF740000FF750000FF750000FF75
      0000FF760000FC7D00008C775200000000005A5A5A008C8C8C00EFEFEF00DEDE
      DE00DEDEDE00EFEFEF00BDBDBD00BDBDBD00B5B5B500E7E7E700DEDEDE00E7E7
      E700E7E7E700B5B5B5004A4A4A00D6D6D600D9872800B56D1900714F3400E5A7
      98006D473800B5691100E4841600E3841500E3811300E2800D00E17F0D00E88B
      1F00EE8C1A00CC874600C885420000000000DFB38D00E4B79000E4B38900E4AF
      8100E4AB7900E4A67000E4A46800E4A16400E4A16300E4A16300E4A16300E4A1
      6400EAA56300B0813A00EFEFEF00FEFEFE00DFB48F00E4B79000E4B48B00E4B0
      8400E4AD7D00E4AA7600E4A66F00E4A46A00E4A26800E4A36800E4A36800E4A3
      6800E6A66F00E89F4E0000000000000000006B6B6B0052525200EFEFEF00E7E7
      E700E7E7E700F7F7F7009C9C9C004A4A4A0073737300EFEFEF00E7E7E700EFEF
      EF00EFEFEF00848484005A5A5A00D6D6D60000000000F0F0F0008D8A8A005F4D
      4900ABAAAA000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FFF0E300FFF0E300FFF0
      E300FFF0E300FFF0E300FFF0E300FFF0E300FFF0E300FFF0E300FFF0E300FFF0
      E300FFF0E30000000000000000000000000000000000FEFEFE00DDDFF800B9BD
      F000F7F7FD00000000000000000000000000000000000000000000000000F7F7
      FD00B9B9EE00DDDDF700FEFEFE00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000937B6A0060463200634935006349
      350063493500634935006349350063493500634935006349350063493500684E
      3A00FFF0E300000000000000000000000000FEFEFE00D6D9F600566CE4001432
      D900989EE900F3F3FC0000000000000000000000000000000000F3F3FC009898
      E7000D0DD6005252DF00D6D6F500FEFEFE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000097806F00FCFEFC00E7E6E400E7E2
      DC00E6DCD400E5D6CB00E4CFC100E3CAB800E3C5B100E2C2AC00C7AA9800684E
      3A00FFF0E300000000000000000000000000DDE0F8005872E500265BEF00235D
      F6002140DE009298E800F3F3FC000000000000000000F3F3FC009293E6001818
      DB000000F6000808EC005252DF00DDDDF70000000000000000009EADC300346D
      A00000000000266DA800256BA8002469A800000000002366A8002163A8001F61
      A800000000009AABC300000000000000000000000000000000009EADC300346D
      A0002670A800266DA800256BA8002469A8002367A8002366A8002163A8001F61
      A800235DA0009AABC30000000000000000009C857400FCFEFC00B09B8C00AA94
      8500FBF5EF00B6A19200B09A8C00AB958500A58F7F00F6DBC800C7AA9800684E
      3A00FFF0E300000000000000000000000000B9C1F2001943DE002F6EF8003472
      FE00225BF6002140DE00989EE900F7F7FD00F7F7FD00989AE700181FDB000102
      F5000000FE000000F6000D0DD600B9B9EE000000000000000000829BB8002DAE
      ED0018BBFF0017B3FF0014ADFF0012A7FF000FA2FF000C9CFF000996FF000592
      FF00027EED007A95B80000000000000000000000000000000000829BB8002DAE
      ED0018BBFF0017B3FF0014ADFF0012A7FF000FA2FF000C9CFF000996FF000592
      FF00027EED007A95B8000000000000000000A08A7A00FCFEFC00FCFEFC00FCFE
      FC00FBFAF700FBF5EF00FAEFE600F9E8DC00F7E1D100F6DBC800C8AE9C00684E
      3A00FFF0E300000000000000000000000000F7F7FD0098A4EC002651E200306F
      F7003472FE00255FF8001336DE008A93E8008A90E7000A1FDC00041FF8000110
      FE000102F5001818DB009898E700F7F7FD000000000000000000829CB90028AE
      F0001DBEFF001DB8FF001AB3FF0016AEFF0014A8FF0011A2FF000E9CFF000A99
      FF000587F0007C97B900000000000000000000000000000000000000000028AE
      F0001DBEFF001DB8FF001AB3FF0016AEFF0014A8FF0011A2FF000E9CFF000A99
      FF000587F0007C97B9000000000000000000A58F7F00FCFEFC00B09B8C00AA94
      8500FBFAF700B5A19200B09B8C00AA948500A48F7E00F7E1D100C8B2A200684E
      3A00FFF0E30000000000000000000000000000000000F3F4FC00929EEB002651
      E2003271F9003472FE00245EF7001437DF000F2FDF000D3AF7000A32FE00041F
      F800181FDB009293E600F3F3FC00000000000000000000000000829DB9002CB5
      EF0020C4FF0023C3FF0021C0FF0020BEFF001FBCFF001BB5FF0013A7FF000E9E
      FF000A8EEF007D98B90000000000000000000000000000000000829DB9002CB5
      EF0020C4FF0023C3FF0021C0FF0020BEFF001FBCFF001BB5FF0013A7FF000E9E
      FF000A8EEF007D98B9000000000000000000AA958500FCFEFC00FCFEFC00FCFE
      FC00FCFEFC00FCFEFC00FBFAF700FBF5F000FAF1E900F9E8DD00CAB8AB00684E
      3A00FFF0E3000000000000000000000000000000000000000000F3F4FC0098A4
      EC001A48E200306FF8003472FE002660F9001C54F900184DFE000D3AF7000A1F
      DC00989AE700F3F3FC000000000000000000000000000000000083A3BF0035C5
      F0002AD6FF002CD4FF002BD4FF002CD4FF002CD4FF002BD3FF0025C8FF0017B0
      FF000D95F0007D9DBF000000000000000000000000000000000083A3BF0035C5
      F0002AD6FF002CD4FF002BD4FF002CD4FF002CD4FF002BD3FF0025C8FF0017B0
      FF000D95F0007D9DBF000000000000000000AF9B8B00FCFEFC00DBCBC100E5DA
      D100FCFEFC00DCCAC100DCCAC100B0ADAC00415C7200FAF3EC00D4C9C1006A51
      3D00FFF0E3000000000000000000F4F7F400000000000000000000000000F7F7
      FD008B9AEB001C4AE3003473FA003472FE002965FE001C54F9000F2FDF008A90
      E700F7F7FD00000000000000000000000000000000000000000084AFCE003FD8
      F30032E8FF0034E6FF0034E7FF0034E7FF0034E7FF0034E7FF0035E8FF002DDB
      FF0016ABF3007DA8CE000000000000000000000000000000000084AFCE003FD8
      F30032E8FF0034E6FF0034E7FF0034E7FF0034E7FF0034E7FF0035E8FF002DDB
      FF0016ABF3007DA8CE000000000000000000B4A09100FCFEFC00E2E9E9005E75
      8400DFE4E500FCFEFC00C2CACE004A6170002EA9D60023354700C2BEB9007F69
      5800FFF0E30000000000A8C1AB0041824C00000000000000000000000000F7F8
      FD008B9DEC002252E4003F7EFA00407FFE003472FE002660F9001437DF008A93
      E800F7F7FD00000000000000000000000000000000000000000089BBDB0063E3
      F50050F6FE004AF4FF004BF4FE004BF3FE004BF3FE004BF4FE004CF4FF004BF7
      FE0043D5F50083B7DB00000000000000000000000000000000000000000063E3
      F50050F6FE004AF4FF004BF4FE004BF3FE004BF3FE004BF4FE004CF4FF004BF7
      FE0043D5F50083B7DB000000000000000000DF9D7D00F1CAB7008FA4AC0086D3
      E5004B617000A79289004A61700061C1DE004C616F0029B6E90015273300102C
      3A0017293500798187003D7443004BA761000000000000000000F3F5FD0098A9
      F000265BE5004D8CF9005491FE003F7EFA003473FA003472FE00245EF7001336
      DE00989EE900F3F3FC0000000000000000000000000000000000C1DFF20070B4
      DC0067B1D60064ACD30072BAE1007BC5EA007BC5EA0072BAE00061A9CF0065AE
      D3006CB2DC00C1DEF20000000000000000000000000000000000C1DFF20070B4
      DC0067B1D60064ACD30072BAE1007BC5EA007BC5EA0072BAE00061A9CF0065AE
      D3006CB2DC00C1DEF2000000000000000000DF9D7D00FFC5A400E5C9B9008FA4
      AC0083E1F6004B6170007ACDE2004C616F0065D2F200455D710032B2DF001D99
      C8001A88B3001F536D004F8458004CAC640000000000F3F5FD0092A7F000346C
      E900609DFA0068A3FE004D8CF9002252E4001C4AE300306FF8003472FE00255F
      F8002140DE009298E800F3F3FC00000000000000000000000000FBFDFE00E0DA
      DD00BBB9BE00B1ADB100DEE1E600F3FBFF00F2FBFF00DDDFE400A6A2A700B2B0
      B600DDDADC00FBFDFE0000000000000000000000000000000000FBFDFE00E0DA
      DD00BBB9BE00B1ADB100DEE1E600F3FBFF00F2FBFF00DDDFE400A6A2A700B2B0
      B600DDDADC00FBFDFE000000000000000000DF9D7D00DF9D7D00DF9D7D00DABA
      AA008FA4AC0083E1F6004B61700083E1F6003C59730065D2F20050C9EF003BBF
      EB0027B2E4001CA3D60064956E0053B46C00F7F8FE0098AFF3003874EB006DA6
      F9007DB2FE00609DFA00265BE5008B9DEC008B9AEB001A48E2003271F9003472
      FE00225BF6002140DE00989EE900F7F7FD000000000000000000FFFEFE00E5DE
      DD00D2C8C700C7BBBA00ECECEE000000000000000000EEEDEF00B7A9A900C6BC
      BB00E6DEDD00FFFEFE0000000000000000000000000000000000FFFEFE00E5DE
      DD00D2C8C700C7BBBA00ECECEE000000000000000000EEEDEF00B7A9A900C6BC
      BB00E6DEDD00FFFEFE0000000000000000000000000000000000000000000000
      0000E6EBEC008FA4AC0083E1F600405B720083E1F60077DBF40065D2F20050C9
      EF003BBFEB0029B6E90076A583005DC07800B9CAF8002C6DED007CB5FA0095C2
      FE006DA6F900346CE90098A9F000F7F8FD00F7F7FD0098A4EC002651E200306F
      F7003472FE00235DF6001432D900B9BDF0000000000000000000FEFEFE00E7E1
      E100D4CACA00D8D2D200C8BABA00F9F7F600FEFCFC00CCBEBE00C3BBBB00CDC2
      C200E7E0E000FEFEFE000000000000000000000000000000000000000000E7E1
      E100D4CACA00D8D2D200C8BABA00F9F7F600FEFCFC00CCBEBE00C3BBBB00CDC2
      C200E7E0E000FEFEFE0000000000000000000000000000000000000000000000
      000000000000E6EBEC008FA4AC0083E1F60083E1F60083E1F60077DBF40065D2
      F2005FB2CF007A8B9600AEC1A60093D39A00DDE5FB006495F20068A4F7007CB5
      FA003874EB0092A7F000F3F5FD000000000000000000F3F4FC00929EEB002651
      E2002F6EF800265BEF00566CE400DDDFF800000000000000000000000000F6F4
      F400D2C6C600EAE5E500D3C9C900CBBEBE00CBBEBE00C7BEBE00DFDBDB00D1C5
      C500F6F4F400000000000000000000000000000000000000000000000000F6F4
      F400D2C6C600EAE5E500D3C9C900CBBEBE00CBBEBE00C7BEBE00DFDBDB00D1C5
      C500F6F4F4000000000000000000000000000000000000000000000000000000
      00000000000000000000E6EBEC008FA4AC008CA1A900889CA5008396A0007E90
      9A007A8B9600BFC8CD0099B89C00AEC1A600FEFEFE00D6E0FB006495F2002C6D
      ED0098AFF300F3F5FD0000000000000000000000000000000000F3F4FC0098A4
      EC001943DE005872E500D6D9F600FEFEFE00000000000000000000000000FEFD
      FD00E3DADA00D6C9C900EFEBEB00E9E6E600E6E2E200E9E6E600D5C9C900E3DA
      DA00FEFDFD00000000000000000000000000000000000000000000000000FEFD
      FD00E3DADA00D6C9C900EFEBEB00E9E6E600E6E2E200E9E6E600D5C9C900E3DA
      DA00FEFDFD000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FEFEFE00DDE5FB00B9CA
      F800F7F8FE00000000000000000000000000000000000000000000000000F7F7
      FD00B9C1F200DDE0F800FEFEFE00000000000000000000000000000000000000
      0000FDFBFB00E7E0E000D2C6C600D3C8C800D3C8C800D2C5C500E7E0E000FDFB
      FB00000000000000000000000000000000000000000000000000000000000000
      0000FDFBFB00E7E0E000D2C6C600D3C8C800D3C8C800D2C5C500E7E0E000FDFB
      FB00000000000000000000000000000000000000000000000000000000000000
      000000000000CECECE00848C8400637B5A00637B63008C8C8400D6D6D6000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F7F7F700D6CECE00948C8C008C7B7B008C7B7B0094848400D6CECE00F7F7
      F700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FFFF
      FF0073846B00089C000000EF000000FF000000FF000000D60000187B08008484
      7B00FFFFFF00000000000000000000000000000000000000000000000000C6BD
      BD006B525200523131006B4A4A007B5A5A007B5A5A00634A4A004A313100634A
      4A00C6BDBD000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E2E2E200B1B1B100AEAEAE00DEDEDE00000000000000
      0000000000000000000000000000000000000000000000000000FFFFFF006B7B
      630010BD000000FF000000FF000000FF000000FF000000FF000000FF000008AD
      00007B847B00FFFFFF0000000000000000000000000000000000ADA5A5005231
      31006B5252008C6B6B00947373008C7373008C737300947373008C6B6B006B4A
      4A004A292900A59C9C000000000000000000BCBCBC00B2B2B200ACACAC00ABAB
      AB00A9A9A900A7A7A700A5A5A500A3A3A300A1A1A1009E9E9E009D9D9D009C9C
      9C009A9A9A009A9A9A0099999900BFBFBF000000000000000000000000000000
      0000EEEEEE00BEBEBE009A9A9A00CDCDCD00BDBDBD00A1A1A100B7B7B700EAEA
      EA00000000000000000000000000000000000000000000000000ADADAD0031A5
      210039EF390021C6180010940000108C000008CE000000FF000000FF000000FF
      0000088C00004A634200526B4A00C6C6C60000000000CEC6C6004A2929007352
      52008C6B6B009C8484007B5A5A007B5A5A007B5A5A00735252009C8484008C6B
      6B006B4A4A0042212100C6BDBD0000000000BCBCBC00EAEAEA00DDDDDD00D9D9
      D900D5D5D500D1D1D100CDCDCD00C8C8C800CFCFCF00CCCCCC00CACACA00C2C2
      C200BEBEBE00BEBEBE00BEBEBE00979797000000000000000000F6F6F600CCCC
      CC00A3A3A300C5C5C500EBEBEB00E8E8E800CCCCCC00CDCDCD00B3B3B3009999
      9900C3C3C300F3F3F300000000000000000000000000FFFFFF005A7B4A0094FF
      8C004AA531005A7B5200BDBDBD00D6D6D6006B7B630018A5080008EF080000FF
      000000FF000000FF000000DE00006B7B6B00F7F7F7006B5252005A3939007352
      5200CEB5B500FFFFFF00AD94940063424200634242009C7B7B00FFF7F700DED6
      D6007B5A5A005A393900634A4A00F7F7F700A7A7A700ABABAB00A0A0A0009A9A
      9A00979797008F8F8F008B8B8B00B5B5B500CECECE00C9C9C900C3C3C300AFAF
      AF008888880081818100878787008E8E8E00FCFCFC00DADADA00AEAEAE00C1C1
      C100E8E8E800EBEBEB00DEDEDE00D6D6D600BBBBBB00BBBBBB00CBCBCB00C7C7
      C700ABABAB00A1A1A100D1D1D100FAFAFA0000000000DEDEDE0084AD6B007BA5
      5A009C9C9C000000000000000000F7F7F7004273310021BD100008E7080000F7
      000000FF000000FF000000D600008C948C00D6CECE004A2929005A3939006B4A
      4A00EFDEDE00FFFFFF00FFFFFF00A58C8C008C737300F7EFEF00FFFFFF00FFF7
      F700846363005A39390042212100D6CECE00A9A9A900AEAEAE00A2A2A2009C9C
      9C00989898008D8D8D00B7B7B700E1E1E100DBDBDC00D0D0D100C1C2C200C0C0
      C000AAAAAA007E7E7E00828282008E8E8E00BDBDBD00BEBEBE00E3E3E300F1F1
      F100E7E7E700DDDDDD00DADADA00D6D6D600BEBEBE00C1C1C100C1C1C100BBBB
      BB00CBCBCB00C0C0C000A6A6A600AEAEAE0000000000B5B5B5004A7B18005263
      4200EFEFEF000000000000000000E7E7E700428C290084FF8C0073EF730052DE
      520042DE420031E7310021840800D6D6D6009C8C8C0052313100634242005A39
      3900735A5A00E7D6D600FFFFFF00FFFFFF00F7F7F700FFFFFF00F7F7F7008C73
      730052313100634242004A292900948C8C00AAAAAA00B2B2B200A5A5A5009F9F
      9F00999999009B9B9B00E4E4E400E7E7E700C3C4C500969796009FA0A100B3B3
      B300BFBFBF0091919100818181008E8E8E00A8A8A800F5F5F500F2F2F200E7E7
      E700E4E4E400DFDFDF00DBDBDB00D6D6D600C0C0C000C2C2C200C2C2C200C2C2
      C200BDBDBD00BEBEBE00C9C9C9009797970000000000848C7B0000AD000000CE
      0000297B21008C948C00FFFFFF0000000000BDBDBD00638C4200DEFFCE00F7FF
      F700DEFFDE009CE78C005A73520000000000947B7B005A393900735252007352
      52006B4A4A0084636300E7DEDE00FFFFFF00FFFFFF00F7F7F7008C6B6B005231
      31005A3939005A393900523131008C7B7B00ACACAC00B5B5B500A7A7A700A2A2
      A2009B9B9B00B0B0B000F0F0F000D4D8DA006F554700D79160006E4F3C008084
      8600C4C4C400A4A4A400858585008F8F8F00AEAEAE00F3F3F300F0F0F000ECEC
      EC00EAEAEA00E4E4E400DADADA00D8D8D800C4C4C400BBBBBB00BEBEBE00C2C2
      C200C3C3C300C0C0C000CBCBCB009E9E9E00EFEFEF002973180000FF000000FF
      000000FF000000CE00004A734A00DEDEDE0000000000D6D6D6007384630094AD
      7300D6DEC6006B8C4200BDBDBD0000000000947B7B00634A4A008C7373008C6B
      6B0084636300AD949400F7F7F700FFFFFF00FFFFFF00FFFFFF009C8C8C005231
      31005A3939005A393900523131008C7B7B00AEAEAE00B7B7B700A9A9A900A4A4
      A4009E9E9E00B6B6B600F4F4F400C7CDCF005B332100F393650062321C006C71
      7400C5C5C500C5C5C500C9C9C9009D9D9D00AFAFAF00F3F3F300F1F1F100EEEE
      EE00E1E1E100C7C7C700CBCBCB00D1D1D100D1D1D100CCCCCC00B1B1B100BBBB
      BB00C0C0C000C0C0C000CECECE009F9F9F00949C940010B5000000F7080000FF
      000000FF000000FF000000FF000052734A00FFFFFF000000000000000000D6D6
      D60031631800186B1000EFEFEF00000000009C8C8C0063424200A58484009C84
      8400BDA5A500FFF7F700FFFFFF00F7F7F700EFE7E700FFFFFF00FFFFFF00A58C
      8C00634242005A3939004A292900948C8C00AEAEAE00B8B8B800A9A9A900A5A5
      A500A0A0A000A4A4A400F6F6F600DCDDDD00767473004A3B360056525100A6A7
      A700CACACA00CCCCCC00DCDCDC00A0A0A000AFAFAF00F3F3F300E9E9E900DDDD
      DD00DEDEDE00C2C2C200DADADA00FEFEFF00FEFEFF00DADADA00C3C3C300D7D7
      D700BBBBBB00BBBBBB00CFCFCF009F9F9F005A7B4A0029CE290010DE100000EF
      000000FF000000FF000000CE00006B846B000000000000000000F7F7F7006B7B
      6B0000CE000039843900FFFFFF0000000000D6CECE0052313100A58C8C00BDA5
      A500F7F7F700FFFFFF00FFF7F700CEBDBD00B59C9C00E7DEDE00FFFFFF00FFFF
      FF00BDA5A500947373004A292900D6CECE00BCBCBC00E4E4E400D6D6D600D0D0
      D000CACACA00BFBFBF00E2E2E200F2F1F100C7C8C800A4A7A800B2B3B300CBCB
      CB00D6D6D600D2D6DA00DFE3E600A0A0A000ADADAD00D8D8D800CDCDCD00CECE
      CE00D1D1D100ACACAC00FEFEFF00FEFEFF00FEFEFF00FEFEFF00ACACAC00D1D1
      D100CCCCCC00C1C1C100BEBEBE009C9C9C00427321005ADE520029D6290031E7
      310021E7290010DE100021730800A5A5A500EFEFEF00ADADAD00396B310000E7
      000000FF0000848C84000000000000000000F7F7F7006B52520084636300CEBD
      BD00D6C6C600F7F7F700DECECE00C6B5B500C6B5B500C6ADAD00EFE7E700EFE7
      E700D6C6C600947B7B0063525200F7F7F700C7C7C70000000000F9F9F900F0F0
      F000E9E9E900E1E1E100D1D1D100EAEAEA00F1F1F100E9E9E900E7E7E700E2E3
      E300D0CECE00D7B59B00DCBDA700A0A1A100CCCCCC00AEAEAE00D5D5D500FEFE
      FF00FEFEFF00C7C7C700B5B5B500A3A3A300A3A3A300B5B5B500C7C7C700FCFC
      FC00FBFBFB00D2D2D200AEAEAE00BFBFBF00949C8C005A7B4A00426B31008CCE
      7B00A5FFA5004ADE4A0018CE1800109400002184100008AD000000FF000000FF
      0000188C1000DEDEDE00000000000000000000000000CEC6C6004A292900A58C
      8C00E7D6D600E7D6D600DECECE00DECECE00DECECE00DECECE00E7D6D600E7D6
      D600B59C9C004A292900CEC6C60000000000B6B6B600F9F9F900F5F5F500F0F0
      F000E5E5E500E2E2E200DEDEDE00D7D7D700F0F0F000F4F4F400F0F0F000E6E7
      E700E3E1E100EBC6AE00ECC6AF00A1A1A10000000000ABABAB00FEFEFF00FEFE
      FF00FEFEFF00E0E0E000ACACAC00D1D1D100D1D1D100ABABAB00E0E0E000FEFE
      FF00FEFEFF00FEFEFF00ABABAB00000000000000000000000000EFEFEF005273
      3900BDE7AD00C6FFC6006BEF6B0029DE290010E7100000FF080000FF000008AD
      0000A5A5A5000000000000000000000000000000000000000000B5A5A5005231
      3100947B7B00D6C6C600FFF7F700FFFFFF00FFFFFF00FFFFFF00DED6D6009C84
      840052313100ADA5A5000000000000000000EDEDED00BCBCBC00919191009C9C
      9C00CFCFCF00D3D3D300D3D3D300C6C6C600B9B9B900BABABA00BCBCBC00BDBD
      BD00BBBBBB00BDBFBF00BDBFC000BEBEBE0000000000DEDEDE00BCBCBC00A2A2
      A200A2A2A200B1B1B100D6D6D600FEFEFF00FCFCFC00D3D3D300B1B1B100A2A2
      A200A2A2A200BCBCBC00DEDEDE0000000000000000000000000000000000E7E7
      E700637B4A0084AD63009CE78C0073E7730039D6310018BD0800218418009C9C
      940000000000000000000000000000000000000000000000000000000000CEC6
      C6006B5252005A393900846B6B008C7373008C7B7B00846B6B00634242006352
      5200C6C6C6000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F7F7F700AAAAAA00FEFEFF00FEFEFF00FEFEFF00FEFEFF00AAAAAA00F5F5
      F500000000000000000000000000000000000000000000000000000000000000
      000000000000BDBDBD007B8C7300637B5200637B5A00949C9400E7E7E7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000F7F7F700D6CECE009C8C8C008C7B7B008C7B7B009C8C8C00D6CECE00F7F7
      F700000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000DEDEDE00BBBBBB00A2A2A200A2A2A200BBBBBB00DEDEDE000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000CDBBBB00B79B9B00BAA0A000BAA0A000B99F9F00B99C
      9C00B99C9C00B79B9B00B79C9C00A888880000000000FEFEFE00FBFBFB00FBFB
      FB00FBFBFB00FBFBFB00FAFAFA00EEEEEE00DCDCDC00D8D8D800D9D9D900D9D9
      D900D9D9D900D8D8D800DDDDDD00F1F1F100C6C6C600F3F3F300000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000F7F7
      F7005E6970007F848700FCFCFC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E6D9D900FEF9F900FEFDFD00FEF9F900FEF7F700FCF1
      F100F8EDED00F5E7E700F7E8E800AC8E8E00FDFDFD00D0D0D000B7B7B700B7B7
      B700B7B7B700B7B7B700BCBCBC0095887D00B3763A00AB703800AB703800AB70
      3800AB703800AB703800B4763B00C1BBB500D9D9D9009F9F9F00C6C6C600FDFD
      FD0000000000000000000000000000000000FEFCFF00FFFDFF00000000000000
      000000000000000000000000000000000000000000000000000000000000A4A5
      A500006BAD00006EAD004D5D6500DCDCDC000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000CDBB
      BB00B79B9B00BAA0A000E6D8D800FDF9F900FEF8F800FDF3F300F9F0F000F7EC
      EC00F5E7E700F0E1E100F3E2E200AB8D8D0093A0AC00355A93002E558C002954
      8C0028538C0027538C00234B8A0069657300FFE9BD00FFE9CD00FFE0C000FFDE
      BB00FFDBB500FFDAB600FFBB6700B8A79B00E5E5E500979797008D8D8D00C1C1
      C100FBFBFB0000000000F7F8FB00C9DCC500ACCC8E00BCD98500DFE7C8000000
      000000000000000000000000000000000000000000000000000000000000556C
      76000095DF0000A3ED000099DA000C638400284D5A00B1B2B200F8F8F800FCFC
      FC0000000000000000000000000000000000000000000000000000000000E6D9
      D900FEF9F900FEFDFD00E1D3D300FEFCFC00FEFEFE00FCF3F300F9F0F000F6E9
      E900F5E7E700F0E1E100F3E2E200AB8D8D0069C9FB00B4E8FC0076D3FC006ACF
      FC0062CBFC005AC9FD0041BDFB007FA8BA00FFE6CE00FBD3B600FBC7A400FBC4
      A100FBC19B00FDC4A400FFBB7500B9AA9F0000000000E4E4E4009F9F9F008C8C
      8C00C3C1C500E4E8EA0091C56C0082C819009ADF02009DE10000A6D71B00DDE7
      BF00000000000000000000000000000000000000000000000000F7F7F7002A64
      780000B1F20000ACE70000B7EE0000BDFA0000AADD000F5E7700105F7F004F5D
      6300AAAAAA00000000000000000000000000C2ACAC00C1ABAB00C1ABAB00E6D8
      D800FDF9F900FEF8F800C5ADAD00FEFEFE00FEFEFE00FEF6F600F7EDED00F5E8
      E800F3E4E400EEDEDE00F0E0E000AB8D8D0069C0F000A9DFF30048BBF30043B9
      F3003BB6F30032B1F4001AA8F2006A9BB400FFE6CC00FBDBC300FBC7A600FBC7
      A400FBC39F00FDC6A500FFB97300B9AA9F000000000000000000E7E7E700A7A4
      AB009093930080BF440084DA0000ACEC0000A5E70000A1E70000A9EB00009BCC
      4B00F6F6FC000000000000000000000000000000000000000000F0F0F0001C6E
      820000CFFF0000C3F00000C3F00000BAEC0000C0F10000B8F50000B6F6000097
      CC0017546C0087888A00E7E7E70000000000D8C4C400F9F1F100F1E4E400E1D3
      D300FEFCFC00FEFEFE00DCC6C600FDF8F800FEFEFE00FEFEFE00FCF1F100F1E2
      E200EDDCDC00E9D7D700ECDADA00AA8D8D0069C0F000B7E3F3005BBFF3004EBC
      F30044BAF3003BB5F40021AAF2006E9CB400FFE6CB00FBDBC200FBC6A400FBC0
      9700FBBC9200FCC09B00FFB56E00B9AA9F00000000000000000000000000E2E2
      E3008AB8440096E00000B5EC000095E1000066D4000057C7000062CE00006DBE
      3A00F5F3F8000000000000000000000000000000000000000000BAB9B9005B76
      7C002ADAF20000E4FF0000CDF80000C2F00000BEEE0000AEE80000A7E50000AD
      EC0000B8F600009CD200465D6A0000000000E0D1D100FEFEFE00FEF7F700C5AD
      AD00FEFEFE00FEFEFE00E0CCCC00FCF7F700FEFEFE00FEFEFE00FEFEFE00F8ED
      ED00ECDDDD00E0CCCC00E1CFCF00AA89890068C0F000BCE4F30073CCF30059C1
      F30041B8F30038B4F4001FA9F2006C9CB400FFE5CB00FBDBC200FBCEAC00FBC1
      9A00FAB38700FCB58C00FFB16600B9AA9F000000000000000000FFFCFF00AAD1
      890098E00000B2EC000088DD000053CD000044BB000040AF000035A600009DC4
      860000000000000000000000000000000000000000000000000068646400C693
      8B00DDA89C008BA9A7002EB4CE0000C2F20000BFF60000A4E50000B6EA0000B8
      F300007FB3003E606D00B9BBBC0000000000DFCFCF00FEFEFE00FEF7F700DCC6
      C600FDF8F800FEFEFE00DECACA00FCF7F700FEFEFE00FEFEFE00FEFEFE00FEF7
      F700F6E9E900E1CECE00DAC6C600A988880068C0F000BAE4F30090D7F30071CB
      F3003EB7F30036B4F4001AA8F2006A9AB400FFE6CC00FBE2CD00FBD7BB00FCD6
      BB00FEDBC700FFC4A200FFA95900BAABA0000000000000000000DFE4DD0052B6
      17006FD700007CD9000050CC000044B8000044B200004BC600004BCC000071BE
      4100DAE4D600F3F3FA00FFFDFF00000000000000000000000000DFDFDF008685
      8500735D5B00B2776B00C6806E008B7F78003888A8000592D90000AEF800006E
      AE007F838600000000000000000000000000DFCFCF00FEFEFE00FEFDFD00E0CC
      CC00FCF7F700FEFEFE00DECACA00FCF7F700FEFEFE00FEFEFE00FEFEFE00FDF5
      F500F1E2E200CEB7B700C6ABAB00A484840068C0F000B5E4F30092DAF3008DD6
      F30055C0F3002AADF4000E9EF2006598B400FFE6CC00FBE3CE00FBD6B900FCD8
      C000F9C08F00F68E3A00FF8E1900C2B7AC000000000000000000B9D5A70048C3
      00004CCD00004ECE000048BF000044B100004ECA000050D100004DCD00003FB5
      000058A9250092CE5100AED19200F8F7FA000000000000000000000000000000
      0000C6C6C60054342900B56A4F00B26A5400B76549008C584800415E7F000E24
      3600CDCDCD00000000000000000000000000DFCFCF00FEFEFE00FEFEFE00DECA
      CA00FCF7F700FEFEFE00DDC9C900FCF8F800FEFEFE00FEFEFE00FEFEFE00FCF1
      F100ECDCDC00B99E9E00A8878700C9B6B60068C0F000BBE6F3008ED7F30089D4
      F30078D0F3004BBBF4000E9DF2005C92B300FFEAD100FAE0C800FACEAC00FBCE
      AE00FEC9A600FFA65300B8997C00FBFBFB000000000000000000A4CC8C0045C5
      00004ECD00004ECB000042AE00004BC500004FCF00004DCA000041B1000062BE
      000098E90000A8F6000096DE0200C1D9A7000000000000000000000000000000
      0000716A6800A55F4600A04620004F322500676262008956440095452400382C
      2900EFEFEF00000000000000000000000000DFCFCF00FEFEFE00FEFEFE00DECA
      CA00FCF7F700FEFEFE00DAC6C600F8F1F100FEFEFE00FEFEFE00FEF3F300F3E6
      E600F0E1E100C6ABAB00D4C5C5000000000066BFF000CBE9F300ABE0F300A0DD
      F30088D5F3006FCCF40036B3F200689BB600FFCEA100FFD8BB00FFD2B000FFCD
      A800FFCCA200B395790000000000000000000000000000000000B8D1A80045BE
      00004BD0000048C500003AA7000047C600004FD0000042B600006AC40000B1F7
      000088E4000060D5000074DB0000A2CD7D00000000000000000000000000CBCB
      CB0076504500BB6D52008E4323007D77750000000000C8C8C80067656500A9A9
      A90000000000000000000000000000000000DFCFCF00FEFEFE00FEFEFE00DDC9
      C900FCF8F800FEFEFE00E6D2D200D6BFBF00D0B9B900D1BDBD00EADEDE00E6D9
      D900DECDCD00D7C9C900000000000000000065BFF000CFEAF300B2E2F200ADE7
      FB009FE9FF0086E0FF0061CEFF003BADE8002492DA001A76D300216CCB002166
      C100555B6C00E4E4E40000000000000000000000000000000000F4F5F3008AC0
      69005EC0220058BA1C0085C15E0079C04D0042B8000040AC00006ED9000069D8
      000049C7000047CB000047B80800C4D6BB000000000000000000000000007570
      6E00B67B6A00B86A4E0060362500D5D5D5000000000000000000000000000000
      000000000000000000000000000000000000DFCFCF00FEFEFE00FEFEFE00DAC6
      C600F8F1F100FEFEFE00FEFEFE00FEF3F300F3E6E600F0E1E100C6ABAB00D4C5
      C5000000000000000000000000000000000064BFF000D8F0F300B7E9F9009DB3
      BB008284800079848600687B870052718500426984002563A5000068FC000056
      E90049557000E1E1E1000000000000000000000000000000000000000000F7F7
      F700DCE4D700E0EAD900FFFCFF00F0F0EF0079B5530044BE00004CCE00004ACA
      00004ECF000046C9000084C05F00FAF6FC000000000000000000D5D5D5007C5E
      5800CE8B7900A35E4600746D6A00000000000000000000000000000000000000
      000000000000000000000000000000000000DFCFCF00FEFEFE00FEFEFE00E6D2
      D200D6BFBF00D0B9B900D1BDBD00EADEDE00E6D9D900DECDCD00D7C9C9000000
      00000000000000000000000000000000000061BEEF00DFF0F300BFF2FE00B0AA
      9E00BD785200C28F7400C18B6C00C8947500CB815600625653000289FA000568
      F2004A5A7600EDEDED0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000BFD6B10044C1000049CE00004ACC
      000044C4000068B73700E0E6DC0000000000000000000000000094939300B789
      8000D08C78006B443600D0D0D000000000000000000000000000000000000000
      000000000000000000000000000000000000DFCFCF00FEFEFE00FEFEFE00FEFE
      FE00FEF9F900F7EDED00E7D7D700C2A6A600D2C2C20000000000000000000000
      00000000000000000000000000000000000081CBF30085CFF10075CBF10076CF
      F7008EA3A900ECD7C800FDF7F200F6E3D600838181002A9CD8002EA7E2005A8D
      AC00A6AAB3000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E0EAD9006BB93A005CBE200055BA
      19008EC46C00E2E7DE000000000000000000000000000000000087838300D49F
      9400A57061007F7B7A0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DCC9C900E7D8D800E5D7D700E3D3
      D300E2D1D100E5D3D300C2A6A600D9CBCB000000000000000000000000000000
      000000000000000000000000000000000000F8FCFE00C1E6FA00C2E6FA00C1E6
      FB00C6E5F500DBBEAC00D2AD980096776900B8BFC200C4E7F900C5EAFD000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000EFF0EF00DFE6DA00DBE5
      D600FAF5FD000000000000000000000000000000000000000000BABABA005E4D
      49007C787600F8F8F80000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FD6F1800F9740F00F87F7200AEAA
      AE00808080008080800080808000808080008080800080808000808080008080
      8000808080008080800080808000808080000000000000000000BDBDE7008494
      D6008C94D6008C94D6008C94D6008C94D6008494D6008494D600848CD600848C
      D600848CD600BDC6E70000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E0E9F1005E96C9000C77D000247C
      C7009EBCD900FBFCFD0000000000000000000000000000000000000000000000
      00000000000000000000CDBBBB00B79B9B00BAA0A000BAA0A000B99F9F00B99C
      9C00B99C9C00B79B9B00B79C9C00A8888800F0685B00FC741E00FD6B1600F9AC
      9000D0B2A300BC927F00BC927F00BC937F00BC927F00BC927F00BC927F00BC92
      7F00BC927F00BD917E00BC917E0080808000FFFFFF007B84CE002942B500395A
      BD00395ABD00395ABD00315ABD00315ABD003152BD003152BD003152BD003152
      BD002942B5001029AD007B84CE00000000000000000000000000000000000000
      0000000000000000000000000000000000007DAED6000C87DF002B8BD5001487
      DC002388D300DCE7F20000000000000000000000000000000000000000000000
      00000000000000000000E6D9D900FEF9F900FEFDFD00FEF9F900FEF7F700FCF1
      F100F8EDED00F5E7E700F7E8E800AC8E8E00CA957A00F2837400F77011009632
      5800B0A6CF00DDD8D000DBCFBD00E7D3B400D4C8B700D2CBBE00D0C9BD00D1C6
      B600D2C6B700D2B3A200BC917E0080808000BDC6E700294AB5005A84CE005A84
      CE005A84CE005A84CE00638CCE007394D600739CD600739CD6007B9CD60084A5
      D6008CADDE006384CE001029AD00BDC6E70000000000FEFEFE00F3F6F900EAF2
      F900EDF3F900F8FAFC0000000000000000003093D5003DA0DF00CAD9E80090BD
      DF001596E3009FC3DF0000000000000000000000000000000000000000000000
      00000000000000000000E6D8D800FDF9F900FEF8F800FDF3F300F9F0F000F7EC
      EC00F5E7E700F0E1E100F3E2E200AB8D8D00BC917E001F0F0E00A45B5A001409
      BA001213C100A19EB8003E444700020303000000000000000000000000000000
      00000000000000000000BC917E00808080008C94D6004263BD00527BC600527B
      C600527BC6008CADDE00D6DEF700FFFFFF00F7FFFF00D6DEF7009CB5E7007B9C
      D60084A5D6008CADDE00294AB500848CD60000000000D8E1EB005793CB000F7D
      D1001F89D2007CB4DC00E0ECF400FDFEFE00179CE0006DC4EF00F2F7FB00BDD8
      EC0027B0EC0097C3E10000000000000000000000000000000000000000000000
      00000000000000000000E1D3D300FEFCFC00FEFEFE00FCF3F300F9F0F000F6E9
      E900F5E7E700F0E1E100F3E2E200AB8D8D00BC917E00000000001E1418004446
      92000405BF002518B800717EBB006B88A10012253100080E1200000000000000
      00000000000000000000BC917E00808080008C94D6004263BD004A73C6004A73
      C600ADC6E700FFFFFF00EFEFFF00ADC6E700B5CEE700F7F7FF00FFFFFF00ADC6
      E700739CD60084A5D6003152BD00848CD6000000000088B1D7000887E8002A86
      CE004198D4001C9BE20059B1E200D7E7F3001CB0E9003BC0F100B0D9F30073C4
      EE0020B6F100ABD1EA000000000000000000C2ACAC00C1ABAB00C1ABAB00C1AB
      AB00C1ABAB00C0AAAA00C5ADAD00FEFEFE00FEFEFE00FEF6F600F7EDED00F5E8
      E800F3E4E400EEDEDE00F0E0E000AB8D8D00BC917E0000000000000000001A18
      22004A69B3000054E4000C60DC0050C5FD003DA6D6003B89AE00000000000000
      00000000000000000000BC917E00808080008C94D6004263BD00426BC60084A5
      DE00FFFFFF00BDCEEF00527BCE009CB5E70094ADDE00638CCE00D6E7F700FFFF
      FF0094B5DE007B9CD6003152BD00848CD600000000007BB2DF00138BE200B1C4
      D900F1F5F90077BBE3001DBDF60044B2E30021BDEC0026E1FD002DD6FD0029D4
      FA0043B8E700EBF4FA000000000000000000D8C4C400F9F1F100F1E4E400F0E2
      E200EEE0E000EDDDDD00DCC6C600FDF8F800FEFEFE00FEFEFE00FCF1F100F1E2
      E200EDDCDC00E9D7D700ECDADA00AA8D8D00BC917E0000000000000000000505
      06006D8E970038D4FC005BE6FE006DE7FD002BCCFD0063C2EB00000000000000
      00000000000000000000BC917E00808080008C94D6004263BD004273C600D6E7
      F700E7EFF7005273C600527BC600E7EFFF00CEDEF700527BC6006B94CE00F7FF
      FF00CEDEF7007394D6003152BD00848CD6000000000095BADA001999E80082B4
      D800F1F6FA00BEE5F80021CBF70027E3FE0026C3EE0025C3F00052CDF3006FCE
      F100C4E3F400FDFEFF000000000000000000E0D1D100FEFEFE00FEF7F700FDF5
      F500FCF3F300FCF0F000E0CCCC00FCF7F700FEFEFE00FEFEFE00FEFEFE00F8ED
      ED00ECDDDD00E0CCCC00E1CFCF00AA898900BC917E0000000000000000000405
      05004E616C0050C9F50078F1FD0098F7FD0034CCFD008FC6E400000000000000
      00000000000000000000BC917E00808080008C94D6004263BD004A73C600FFFF
      FF00ADBDE700396BBD00527BC600DEE7F700CED6EF00527BC6005A84CE00CEDE
      EF00E7EFFF007394D6003152BD008C94D60000000000E4ECF3003F9BD60026B1
      EE0051B6E80047C5F30025D6FA0029D7F70059B1BE005F7B9600C5D4E100EFF7
      FC00FDFEFF00000000000000000000000000DFCFCF00FEFEFE00FEF7F700FCF1
      F100F8EEEE00F7ECEC00DECACA00FCF7F700FEFEFE00FEFEFE00FEFEFE00FEF7
      F700F6E9E900E1CECE00DAC6C600A9888800BC917E000000000000000000191A
      1C0093C4DE0026C2FD0037CAFD0034CAFD0033C5FD003BB2E200000000000000
      00000000000000000000BC917E00808080008C94D6004263BD004A73C600FFFF
      FF00A5BDE7003963BD004A73C600DEE7F700C6D6EF00527BC600527BC600CEDE
      EF00E7EFFF006B94D600315ABD008C94D6000000000000000000E3EDF60067B3
      DD0041CAF20043D9F60060C5ED00AED6F10097969F00B6A0A000A88F8F00E8E2
      E200FEFEFE00000000000000000000000000DFCFCF00FEFEFE00FEFDFD00F9F0
      F000F8EDED00F7ECEC00DECACA00FCF7F700FEFEFE00FEFEFE00FEFEFE00FDF5
      F500F1E2E200CEB7B700C6ABAB00A4848400BC917E0000000000000000000707
      0700343E43001D4056004F85B30083C8FD002C567300213B4800000000000000
      00000000000000000000BC917E00808080008C94D6004263BD003963BD00CEDE
      F700E7EFF700426BC600426BC600E7EFFF00CED6EF004273C600638CCE00F7F7
      FF00C6D6EF005A84CE00315ABD008C94D600000000000000000000000000F3F8
      FC00D3E5F300CDE4F400EAF4FB00000000009D848600B7A2A200CEC0C000BDAB
      AB00E9E3E300FFFEFE000000000000000000DFCFCF00FEFEFE00FEFEFE00FEF6
      F600F6E9E900F3E4E400DDC9C900FCF8F800FEFEFE00FEFEFE00FEFEFE00FCF1
      F100ECDCDC00B99E9E00A8878700C9B6B600BC917E0000000000000000000101
      01000C0F1000070F150043627E0093BBDD0014273400070E1100000000000000
      00000000000000000000BC917E00808080008C94D6004A6BC600315ABD007394
      D600FFFFFF00B5C6EF004A6BC6008CA5DE00849CD600527BCE00D6DEF700FFFF
      FF007B9CD6005A84CE00395ABD008C94D6000000000000000000000000000000
      000000000000000000000000000000000000BCA7A700B9A4A400C2AFAF00DED4
      D400C9B9B900F2EEEE000000000000000000DFCFCF00FEFEFE00FEFEFE00FEFE
      FE00FEF3F300F0E2E200DAC6C600F8F1F100FEFEFE00FEFEFE00FEF3F300F3E6
      E600F0E1E100C6ABAB00D4C5C50000000000BC917E0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000BC917E00808080008C94D6004A6BC600395ABD00315A
      BD0094ADDE00FFFFFF00EFF7FF00A5BDE700ADC6E700F7FFFF00FFFFFF0094AD
      DE00527BC6005A84CE00395ABD008C94D6000000000000000000000000000000
      000000000000000000000000000000000000CBBABA00C6B5B500D5C9C900D8CD
      CD00EEE8E800D9CCCC00F4F1F10000000000DFCFCF00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FCF3F300E6D2D200D6BFBF00D0B9B900D1BDBD00EADEDE00E6D9
      D900DECDCD00D7C9C9000000000000000000BC917E0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000BC917E00808080008C94D600526BC600315ABD003152
      BD00315ABD00738CD600BDCEEF00EFF7FF00EFF7FF00BDCEEF007394D6004273
      C600527BC6005A84CE00395ABD008494D6000000000000000000000000000000
      000000000000000000000000000000000000D6C9C900D2C4C400F0EBEB00E7E0
      E000DDD3D300F2EEEE00E9E2E20000000000DFCFCF00FEFEFE00FEFEFE00FEFE
      FE00FEFDFD00F9F0F000F0E0E000DCC8C800C5ABAB00C2ACAC00000000000000
      000000000000000000000000000000000000BC917E00D5D9D100DAE2DC00DBE2
      DB00DDE2DC00E1E4DD00E0E7DF00E5EADE00E5E8E200E2E4DD00DBE1DB00D9DD
      D800D9DFD900DCD5C300BC917E0080808000BDBDE7004252BD005A7BC600315A
      BD00395ABD00315ABD003963BD004A6BC6004A73C600426BC600426BC6004A73
      C600527BC6005A84CE002942B500BDC6E7000000000000000000000000000000
      000000000000000000000000000000000000E0D7D700DDD2D200F4F0F000FEFE
      FE00EEE9E900E1D7D700F0EAEA0000000000DFCFCF00FEFEFE00FEFEFE00FEFE
      FE00FEFCFC00FDF3F300CCB2B200B1929200A3828200CEBDBD00000000000000
      000000000000000000000000000000000000B2580000AD530000AD540000AC52
      0000AB550000AD540000AD530000AE560000AD530000AC530000AD540000B45F
      0E00B75E040099582900AF6A2B0080808000FFFFFF007B84CE004252BD00526B
      C6004A6BC6004A6BC6004A63BD004263BD004263BD004263BD004263BD004263
      BD004263BD00294AB5007B84CE00FFFFFF000000000000000000000000000000
      000000000000000000000000000000000000E1D7D700E4DBDB00F6F3F3000000
      000000000000F2EEEE00F2EEEE0000000000DFCFCF00FEFEFE00FEFEFE00FEFE
      FE00FEF9F900F7EDED00E7D7D700C2A6A600D2C2C20000000000000000000000
      000000000000000000000000000000000000D9872800E48A2000E4851500E383
      1400E3831300E4841600E4841600E3841500E3811300E2800D00E17F0D00E88B
      1F00EE8C1A00CC874600C8854200000000000000000000000000BDBDE7008C94
      D6008C9CD6008C94D6008C94D6008C94D6008C94D6008C94D6008C94D6008C94
      D6008C94D600BDBDE70000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EEEAEA00DBD0D000F7F4F4000000
      000000000000000000000000000000000000DCC9C900E7D8D800E5D7D700E3D3
      D300E2D1D100E5D3D300C2A6A600D9CBCB000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C98A6100C3845800D38B6800E18F7000DC8D6C00DA8B
      6D00D78A6E00CD8B6C00AB6D4400A65F2E00FD5B0300FDA52F00FD410000FDA4
      A200AFA9AC008080800080808000808080008080800080808000808080008080
      800080808000808080008080800080808000FD6F1800F9740F00F87F7200AEAA
      AE00808080008080800080808000808080008080800080808000808080008080
      80008080800080808000808080008080800000000000F9740F00F87F7200AEAA
      AE00808080008080800080808000808080008080800080808000808080008080
      800080808000808080008080800080808000E2C0A900CC8D6500BF703F00BA66
      3000BC6A3600BC693600C6835500EFCEBA00DDFFFF0087EEC700A2F4D700A2F6
      D7008CEEC700E0FFFF00DDA28500AB6A3E00FD584500FD691400FD851400FA5A
      3A00F8B9BD00CEB2A500BC927F00BC937F00BC927F00BC927F00BC927F00BC92
      7F00BC927F00BD917E00BC917E0080808000F0685B00FC741E00FD6B1600F9AC
      9000D0B2A300BC927F00BC927F00BC937F00BC927F00BC927F00BC927F00BC92
      7F00BC927F00BD917E00BC917E0080808000F0685B00FC741E00FD6B1600F9AC
      9000D0B2A300BC927F00BC927F00BC937F00BC927F00BC927F00BC927F00BC92
      7F00BC927F00BD917E00BC917E0080808000C47C4D00F8F1EB00F7ECDF00F6EA
      DC00F6EADC00F6EADC00C37F5100EFB69A00EAF3E80051BF84006FC9980071C9
      990054BF8400E4F4E900DD9C7B00AA693A00D38A7300FB6E6300FC6B1700FF7D
      0000FD6B3200F9C9C200E5DFD200E7D3B400D4C8B700D2CBBE00D0C9BD00D1C6
      B600D2C6B700D2B3A200BC917E0080808000CA957A00F2837400F77011009632
      5800B0A6CF00DDD8D000DBCFBD00E7D3B400D4C8B700D2CBBE00D0C9BD00D1C6
      B600D2C6B700D2B3A200BC917E0080808000CA957A00F2837400F77011009632
      5800B0A6CF00DDD8D000DBCFBD00E7D3B400D4C8B700D2CBBE00D0C9BD00D1C6
      B600D2C6B700D2B3A200BC917E0080808000C1763F00F5EBDF00FDBF6800FBBE
      6500FCBE6400FCBE6400C4815400EAB69700F3F3EA00EDF1E500EFF0E600EFF0
      E600EDF1E500F3F5ED00D59C7900B0704400BC917E00E7D0CC00FB826B00EF6C
      1A006E1D6C004330B100C6CCDC00EBF8F900E0F4F600E0F4F600E0F4F600E0F4
      F600E0F4F600E2E8E100BC917E0080808000BC917E00E4DFD900DC9090001409
      BA001213C100BFBBD400F0ECF100F7EAED00F8EAED00F8EAED00F8EAED00F8EA
      ED00F8EAED00E2E8E100BC917E0080808000BC917E00E4DFD900DE9593001409
      BA001213C100C0BDD600F4FAFD00FDFEFE00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00E2E8E100BC917E0080808000C27C4200F7EDE300FDC26E00FFD7
      9E00FFD69900FFD69900C98A6100E6B59200E2A78100E1A78100DEA37D00DCA1
      7B00DB9F7900D99E7700D49A7300BB7E5700BC917E00E0F8FA00E8D6D300CB83
      8A001B11BB000000D0003A3AB600CFD3DC00E0F5F900E0F4F600E0F4F600E0F4
      F600E0F4F600E4F1F300BC917E0080808000BC917E00F8EAED00F0DAE1007E7C
      C9000607C1002518B8008B97D400C7DFF800DBE2F000EEE8EE00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00F5EBEF007F81
      CD000607C1002518B8008C99D600C9E6FF00E0F2FE00F4FAFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C5804500F7F0E600F8B45500F7B5
      5400F8B45300F8B25300CA8D6500EAB89900DDA57E00DDA68000DBA37C00D9A0
      7900D9A07900D89E7800D89E7800BF845D00BC917E00DEF4F600E0F4F600D3D3
      E1007276C6001819C7000000C3004837B300A2ADD500C0E0F800CCEBF900E0F4
      F600E0F4F600E2EAEF00BC917E0080808000BC917E00F8EAED00F8EAED00E2D4
      E1006583CD000054E4000C60DC0050C5FD0063CAFB0089D3F800F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00E6E4
      EE006685CF000054E4000C60DC0050C5FD0064CDFD008BD9FD00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C5824500F8F2EB00FEE7D600FDE6
      D500FDE6D500FDE6D500C8885D00EFBFA100FDFCFA00FEFCFB00FEFDFC00FEFD
      FC00FDFBF900FDFCFA00DDA88500C17F5300BC917E00DFF4F600E0F4F600E0F4
      F600C6D3E3007072BA000F2ED000001CD0001939BF005AAAF7008BD7FE0093D4
      F900AAE2FA00E2EBEF00BC917E0080808000BC917E00F8EAED00F8EAED00F7EA
      ED00CFEAF4003BD7FF005BE6FE006DE7FD002BCCFD0074D3FC00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FDFD
      FE00D1F2FB003BD7FF005BE6FE006DE7FD002BCCFD0074D4FD00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C7874C00F9F3EC00FEE8D600FDE6
      D500FDE6D500FDE6D500C7865B00EFC09E00FFF7FB00CC936E00FFF7FB00FFF7
      FB00FFFBF700FFF8F100E4AF8C00C78A6100BC917E00E0F4F600E0F4F600E0F4
      F600E0F4F600BECBE10044ADE7000098F700168AEF0028B7F60046D2FE002DC2
      FC004BC8FD00E2EBEF00BC917E0080808000BC917E00F8EAED00F8EAED00F7EA
      ED00DDE7F50058D0FC0078F1FD0098F7FD0034CCFD00A7DDFB00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FDFE
      FE00E1F3FE0058D0FD0078F1FD0098F7FD0034CCFD00A7DFFD00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C78B4E00F9F4EE00FEE8D800FEE8
      D600FEE7D600FDE5D300CC8D6500F3CDB000FFF7FB00E3C7B300FFF7FB00FFF7
      FB00FFF7FB00FFF7FB00EABFA100C9896000BC917E00E0F4F600E0F4F600E0F4
      F600E0F4F600DDF5F900A8EBFD003ADCFF0063F9FF00A5FFFF0053E0FD0024CB
      FD0084D7FD00E2EBEF00BC917E0080808000BC917E00F8EAED00F8EAED00F6EA
      EF00B1E0FB0026C2FD0037CAFD0034CAFD0033C5FD0055CBFB00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FBFC
      FE00B2E3FD0026C2FD0037CAFD0034CAFD0033C5FD0056CDFD00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C78B4E00F9F4EE00FEE7D600FDE6
      D500FDE6D500FCE6D200D4976E00D49E7B00D0987100D6A48200CD8E6800CD90
      6900D09A7500D1997300C88B6200EDDCCF00BC917E00E0F5F600E0F4F600E0F4
      F600E0F4F600DCF2F700C0E3FB0051C8FC0061E2FC00CCFFFC0067E4FD0039C6
      FD00CAE6FD00E2EBEF00BC917E0080808000BC917E00F8EAED00F8EAED00F8EA
      ED00EAE9F100C0DAF20098CAF80083C8FD00B3D6F400D2E2F200F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FEFE
      FE00EEF8FE00C4E7FE009AD0FD0083C8FD00B7E1FE00D6F0FE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C78C5000F9F4F000FDE6D500FDE7
      D300FCE4D100FBE3CD00FAE0C900F8DCC200F5D6BA00F3D4B500F1D2B300F8F4
      F000C5854B00000000000000000000000000BC917E00DFF5F600E0F4F600E0F4
      F600E0F4F600D6E9FB005CD2FD0009C3FD0040D7FD0043E5FD003DD2FD001BCA
      FD0035C5FD00E2ECEF00BC917E0080808000BC917E00F8EAED00F8EAED00F8EA
      ED00F5EAEE00EAE6EE00BFD8F500B2D9FB00DAE1F000EFE8EE00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FEFE
      FE00FAFDFE00F0F9FE00C2E2FE00B3DBFD00DEF1FE00F4FBFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C78C5000F9F4F000FCE3CD00FCE4
      CF00FCE3CD00FAE0C900F9DDC400F6D9BC00F4E9DF00F7F2EC00FBF7F300F5EF
      E900C2804700000000000000000000000000BC917E00E2F6F700E0F4F600E0F4
      F600E0F4F600E9F5F90094D9FC0066C4FC0068BFFC001FA9FD0066C0FD0067C4
      FC0088D6FC00E3F2F000BC917E0080808000BC917E00F8EAED00F8EAED00F8EA
      ED00F8EAED00F8EAED00F8EAED00F8EAED00F8EAED00F8EAED00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C88D5200F9F4F000FCE3CD00FBE3
      CD00FBE2CA00F9E0C800F8DCC200F5D6BA00FDFBF900FCE6CD00FAE5C900E2B6
      8400D5A78300000000000000000000000000BC917E00E4F7F400E0F4F600E0F4
      F600E0F4F600E0F4F600E0F4F600E0F4F600C0E4F900AED7FD00B6E1FB00E0F4
      F600E0F4F600E7F1EC00BC917E0080808000BC917E00F8EAED00F8EAED00F8EA
      ED00F8EAED00F8EAED00F8EAED00F8EAED00F8EAED00F8EAED00F8EAED00F8EA
      ED00F8EAED00F8EAED00BC917E0080808000BC917E00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFEFE00FEFE
      FE00FEFEFE00FEFEFE00BC917E0080808000C9915900FAF6F200FAE0C700FBE2
      CA00FBE0C800F9DFC500F8DBC100F4D6B800FFFBF800F6D8B400E1B07D00DC95
      6900FCFAF900000000000000000000000000BC917E00D5D9D100DAE2DC00DBE2
      DB00DDE2DC00E1E4DD00E0E7DF00E5EADE00E5E8E200E2E4DD00DBE1DB00D9DD
      D800D9DFD900DCD5C300BC917E0080808000BC917E00D5D9D100DAE2DC00DBE2
      DB00DDE2DC00E1E4DD00E0E7DF00E5EADE00E5E8E200E2E4DD00DBE1DB00D9DD
      D800D9DFD900DCD5C300BC917E0080808000BC917E00D5D9D100DAE2DC00DBE2
      DB00DDE2DC00E1E4DD00E0E7DF00E5EADE00E5E8E200E2E4DD00DBE1DB00D9DD
      D800D9DFD900DCD5C300BC917E0080808000D1A17300F7F2EC00F8F4EE00F8F2
      EC00F8F2EC00F8F2EC00F8F2EC00F7F2EC00F2E6D700E2B27D00DC986B00FCFA
      F90000000000000000000000000000000000B2580000AD530000AD540000AC52
      0000AB550000AD540000AD530000AE560000AD530000AC530000AD540000B45F
      0E00B75E040099582900AF6A2B0080808000B2580000AD530000AD540000AC52
      0000AB550000AD540000AD530000AE560000AD530000AC530000AD540000B45F
      0E00B75E040099582900AF6A2B0080808000B2580000AD530000AD540000AC52
      0000AB550000AD540000AD530000AE560000AD530000AC530000AD540000B45F
      0E00B75E040099582900AF6A2B0080808000E7CEB800D6A97C00CB935A00C88C
      4F00C98F5400C9905400CA8F5400C88E5400CE9C6900DDB18F00FDFBF9000000
      000000000000000000000000000000000000D9872800E48A2000E4851500E383
      1400E3831300E4841600E4841600E3841500E3811300E2800D00E17F0D00E88B
      1F00EE8C1A00CC874600C885420000000000D9872800E48A2000E4851500E383
      1400E3831300E4841600E4841600E3841500E3811300E2800D00E17F0D00E88B
      1F00EE8C1A00CC874600C885420000000000D9872800E48A2000E4851500E383
      1400E3831300E4841600E4841600E3841500E3811300E2800D00E17F0D00E88B
      1F00EE8C1A00CC874600C8854200FFFFFF000000000000000000FFFFFF00A5A5
      A500A5A5A500A5A5A500A5A5A500A5A5A5009C9C9C009C9C9C009C9C9C009C9C
      9C009C9C9C00FFFFFF000000000000000000D1D1D100A9A9A900A6A6A6009A9A
      9A00EEEEEE000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000B5B5B5009C9C9C00CECE
      CE00B5B5B5009494940094949400949494009494940094949400949494009494
      94009494940094949400949494009494940000000000E2C0A900CC8D6600BF70
      3F00BC6A3600BC6A3600BC6A3600BC693500BC693500BB6934000E7B2200037B
      1E00BA6D3B00C98A6200E3C2AD000000000000000000F7F7F700CECECE00CECE
      CE00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00F7F7F700EFEFEF00EFEF
      EF00A5A5A500C6C6C600F7F7F70000000000C3C3C300E1E1E100E2E2E200C2C2
      C20092929200EDEDED0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000D6D6D600EFEFEF00D6D6D6009C9C
      9C0073737300C6C6C600BDBDBD00BDBDBD00BDBDBD00BDBDBD00BDBDBD00BDBD
      BD00B5B5B500B5B5B500BDBDBD009C9C9C0000000000C47C4D00F8F1E900F7EC
      DF00F6EADD00F6EADD00F6EADD00F6EADD00FAF2EA00FAF2EA00158C3A0043A1
      5F0032944A00F8FBF800C27A4D0000000000DEDEDE00949494008C8C8C008C8C
      8C00F7F7F700FFFFFF00FFFFFF00FFFFFF00F7F7F700EFEFEF00E7E7E700D6D6
      D600848484008C8C8C008C8C8C00DEDEDE00D5D5D500E9E9E900D9D9D900DBDB
      DB00C7C7C70091919100ECECEC00000000000000000000000000000000000000
      000000000000000000000000000000000000D6D6D600DEDEDE00DEDEDE00C6C6
      C6009C9C9C00F7F7F700FFFFFF00F7F7F700F7F7F700F7F7F700F7F7F700F7F7
      F700EFEFEF00EFEFEF00F7F7F7009494940000000000C1763F00F5EBDF00FDBF
      6800FCBD6700FBBE6500239751001D914900178F4300118B3B003A9F5E0080C1
      960046A362002E944600B76C350000000000A5A5A500D6D6D6005A5A5A000000
      000052525200636363006363630063636300636363005A5A5A005A5A5A004A4A
      4A00000000006B6B6B00D6D6D6009C9C9C00CDCDCD00E1E1E100DDDDDD00D2D2
      D200E3E3E300C6C6C60091919100EFEFEF000000000000000000000000000000
      000000000000000000000000000000000000EFEFEF00D6D6D600D6D6D600DEDE
      DE00CECECE00B5B5B500EFEFEF00F7F7F700F7F7F700EFEFEF00E7E7E700E7E7
      E700E7E7E700DEDEDE00E7E7E7009494940000000000C27C4200F7EDE300FDC2
      6E00FFD8A000FFD79E00299B5B0090CAA8008DC8A5008AC6A10088C59E006AB6
      850082C2970048A566001D7A230000000000ADADAD00D6D6D600ADADAD008C8C
      8C00848484008484840084848400848484008484840084848400848484008484
      84008C8C8C00B5B5B500D6D6D6009C9C9C00FDFDFD00D0D0D000DCDCDC00DFDF
      DF00D4D4D400D9D9D900C1C1C1008F8F8F00F0F0F00000000000000000000000
      00000000000000000000000000000000000000000000F7F7F700D6D6D600D6D6
      D600DEDEDE00C6C6C600BDBDBD00CECECE00BDBDBD00D6D6D600EFEFEF00EFEF
      EF00E7E7E700E7E7E700EFEFEF009494940000000000C5804500F7F0E600F8B4
      5500F7B45500F7B45500319F630094CDAD006FBA8E006BB8890066B6850061B3
      800067B5820083C298003CA05C0006822A00B5B5B500DEDEDE00DEDEDE00DEDE
      DE00E7E7E700E7E7E700E7E7E700E7E7E700E7E7E700E7E7E700E7E7E700E7E7
      E700DEDEDE00DEDEDE00DEDEDE00A5A5A50000000000F8F8F800C7C7C700D8D8
      D800DDDDDD00D4D4D400DBDBDB00BFBFBF0091919100EEEEEE0000000000FDFD
      FD00000000000000000000000000000000000000000000000000F7F7F700CECE
      CE00D6D6D600D6D6D600B5B5B500ADADAD009C9C9C008C8C8C00BDBDBD00F7F7
      F700EFEFEF00E7E7E700EFEFEF009494940000000000C6834700F8F1E900FEE5
      D500FDE5D300FDE5D30037A36B0096CEB00094CDAD0091CBAA0090CAA80074BC
      90008AC6A10046A568000A87350000000000B5B5B500F7F7F700C6FFCE00EFEF
      EF00EFEFEF00EFEFEF00F7F7F700F7F7F700F7F7F700F7F7F700EFEFEF00EFEF
      EF00EFEFEF00EFEFEF00F7F7F700A5A5A5000000000000000000F9F9F900CCCC
      CC00D4D4D400D7D7D700D5D5D500D8D8D800B7B7B7008B8B8B00929292008D8D
      8D0089898900ADADAD00F5F5F50000000000000000000000000000000000F7F7
      F700ADADAD00CECECE00D6D6D600CECECE00BDBDBD00ADADAD008C8C8C00C6C6
      C600F7F7F700E7E7E700EFEFEF009494940000000000C6854A00F8F2EB00FEE7
      D600FDE6D500FDE6D5003DA56F003AA46F0036A26900329E630055AF7C0091CB
      AA004FAB74001B914800C580450000000000B5ADB500C6FFCE0000FF1000C6FF
      CE00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00ADADAD00000000000000000000000000FEFE
      FE00BFBFBF00D1D1D100D8D8D800CBCBCB00CACACA00C3C3C300B8B8B800B3B3
      B300A8A8A8008A8A8A008E8E8E00EFEFEF000000000000000000000000000000
      000094949400D6D6D600E7E7E700CECECE00BDBDBD00BDBDBD009C9C9C00A5A5
      A500F7F7F700EFEFEF00F7F7F7009494940000000000C7874C00F9F3EC00FEE8
      D600FEE8D600FDE6D500FDE6D500FDE6D500FDE5D300FBE4D00041A46B005AB3
      810028985700FAF2EA00C783470000000000D6D6D600C6D6C600C6FFCE00FFFF
      FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00CECECE00DEDEDE000000000000000000000000000000
      0000FDFDFD00C8C8C800D1D1D100D3D3D300D1D1D100CDCDCD00C2C2C200BABA
      BA00B5B5B500ADADAD0093939300909090000000000000000000000000000000
      00009C9C9C00DEDEDE00F7F7F700D6D6D600F7F7F700FFFFFF00C6C6C600ADAD
      AD00F7F7F700EFEFEF00F7F7F7009494940000000000C78B4E00F9F4EE00FEE8
      D800FEE8D800FEE8D600FEE7D600FDE5D300FCE4D100FBE1CC0046A87200319F
      6500F8DCC200FAF4EE00C7854A000000000000000000E7E7E700CEC6CE00E7E7
      E700FFFFFF00FFFFFF00FFFFFF00FFFFFF00F7F7F700F7F7F700EFEFEF00EFEF
      EF00C6C6C600BDBDBD00E7E7E700000000000000000000000000000000000000
      000000000000F7F7F700C0C0C000E4E4E400DFDFDF00D3D3D300CCCCCC00CACA
      CA00BCBCBC00B1B1B100AEAEAE008F8F8F000000000000000000000000000000
      0000A5A5A500D6D6D600FFFFFF00E7E7E700F7F7F700FFFFFF00FFFFFF00F7F7
      F700F7F7F700EFEFEF00F7F7F7009494940000000000C78B4E00F9F4EE00FEE7
      D600FDE6D500FDE6D500FDE6D500FCE6D200FBE1CC00FADFC700F8DCC200F6DA
      BD00F6D8BB00FAF4EE00C7864B0000000000000000000000000000000000B5B5
      B500C6C6C600C6C6C600C6C6C600BDBDBD00B5B5B500ADADAD00A5A5A500ADAD
      AD00A5A5A5000000000000000000000000000000000000000000000000000000
      00000000000000000000D5D5D500DFDFDF00EBEBEB00DFDFDF00D7D7D700A1A1
      A100C0C0C000B5B5B500ADADAD00A7A7A7000000000000000000000000000000
      0000B5B5B500EFEFEF00DEDEDE00DEDEDE00C6C6C600E7E7E700FFFFFF00FFFF
      FF00FFFFFF00F7F7F700FFFFFF009494940000000000C78C5000F9F4F000FCE6
      D400FCE6D400FDE7D300FCE4D100FBE3CD00FAE0C900F8DCC200F5D6BA00F3D4
      B500F1D2B300F8F4F000C5854B0000000000000000000000000000000000EFEF
      EF00C6C6C600EFEFEF00E7E7E700DEDEDE00D6D6D600CECECE00CECECE009C9C
      9C00F7F7F7000000000000000000000000000000000000000000000000000000
      00000000000000000000D7D7D700EEEEEE00F3F3F300F1F1F100AFAFAF00D7D7
      D70000000000FCFCFC00A8A8A800A3A3A3000000000000000000000000000000
      0000B5B5B500FFFFFF00EFEFEF00E7E7E700DEDEDE00EFEFEF00FFFFFF00DEDE
      DE0094949400949494009C9C9C007B7B7B0000000000C78C5000F9F4F000FCE3
      CD00FBE4D000FCE4CF00FCE3CD00FAE0C900F9DDC400F6D9BC00F4E9DF00F7F2
      EC00FBF7F300F5EFE900C2804700000000000000000000000000000000000000
      0000C6C6C600FFFFFF00FFFFFF00F7F7F700EFEFEF00E7E7E700E7E7E700A5A5
      A500000000000000000000000000000000000000000000000000000000000000
      00000000000000000000E0E0E000DFDFDF00F8F8F800F8F8F80097979700F2F2
      F2000000000000000000F7F7F700C9C9C9000000000000000000000000000000
      0000B5B5B500FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00D6D6
      D600CECECE00E7E7E7008C8C8C008C8C8C0000000000C88D5200F9F4F000FCE3
      CD00FBE3CD00FBE3CD00FBE2CA00F9E0C800F8DCC200F5D6BA00FDFBF800FCE6
      CD00FAE5C900E2B68400D5A78300000000000000000000000000000000000000
      0000C6C6C600FFFFFF00FFFFFF00F7F7F700EFEFEF00EFEFEF00E7E7E700A5A5
      A500000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FEFEFE00C3C3C300F8F8F800F8F8F800BDBDBD00A9A9
      A900000000000000000000000000000000000000000000000000000000000000
      0000BDBDBD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00DEDE
      DE00FFFFFF00CECECE008C8C8C00F7F7F70000000000C9915900FAF6F200FAE0
      C700FBE0C800FBE2CA00FBE0C800F9DFC500F8DBC100F4D6B800FFFBF800F6D8
      B400E1B07D00DC95690000000000000000000000000000000000000000000000
      0000C6C6C600FFFFFF00FFFFFF00FFFFFF00FFFFFF00F7F7F700F7F7F700A5A5
      A500000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000ECECEC00C1C1C100F8F8F800F8F8F800C0C0
      C000B1B1B100F6F6F60000000000000000000000000000000000000000000000
      0000BDBDBD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00EFEF
      EF00B5B5B500A5A5A500EFEFEF000000000000000000D1A17300F7F2EC00F8F4
      ED00F8F4ED00F8F2EC00F8F2EC00F8F2EC00F8F2EC00F7F2EC00F2E6D700E2B2
      7D00DC986B000000000000000000000000000000000000000000000000000000
      0000B5B5B500E7E7E700DEDEDE00DEDEDE00EFEFEF00E7E7E700E7E7E700ADAD
      AD00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F1F1F100BEBEBE00CFCFCF00E7E7
      E7009F9F9F00E6E6E60000000000000000000000000000000000000000000000
      0000BDBDBD00C6C6C600C6C6C600C6C6C600BDBDBD00BDBDBD00BDBDBD00ADAD
      AD00ADADAD00F7F7F700000000000000000000000000E7CEB800D6A97C00CB93
      5A00C9905400C98F5400C98F5400C9905400CA8F5400C88E5400CE9C6900DDB1
      8F00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000736B3900E7E7E700E7E7E700E7E7
      E700000000000000000000000000000000000000000000000000000000000000
      00000000000049BD510008A6130008A6130008A6130008A6130049BD51000000
      00000000000000000000000000000000000000000000B5A59400634A3100634A
      3100634A3100634A3100634A3100634A3100634A3100634A3100634A3100634A
      3100634A3100634A3100634A3100000000000000000000000000000000000000
      00000000000000000000A8ABAB004E606000F9F9F90000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008C8C5A00736B3900E7E7E700E7E7
      E700E7E7E7000000000000000000000000000000000000000000000000000000
      00000000000008A613007EDF910075D8850075D07A0075CD780008A613000000
      00000000000000000000000000000000000000000000B5A59400FFEFE700FFEF
      E700FFEFE700FFEFDE00FFE7DE00FFE7DE00F7E7DE00F7E7DE00F7E7D600F7E7
      D600F7DED600F7DED600634A3100000000000000000000000000000000000000
      000000000000ECECEC001378740009CFCA0054757300F9F9F900000000000000
      00000000000000000000000000000000000000000000E7E7E700E7E7E700E7E7
      E700E7E7E700E7E7E700E7E7E700E7E7E7008C8C5A009C9C7300736B3900E7E7
      E700E7E7E700E7E7E70000000000000000000000000000000000000000000000
      00000000000008A6130042D4620012C5350024C13D005ECE6C0008A613000000
      00000000000000000000000000000000000000000000B5A59400FFEFE700E7CE
      C600E7D6C600E7CEC600E7CEC600DEC6B500DEBDB500E7CEC600D6B5AD00CEB5
      A500DEC6BD00F7DED600634A3100000000000000000000000000000000000000
      0000000000008181820002B1CB0008EBFF000ABEEC004F6A7800F9F9F9000000
      0000000000000000000000000000F1F1F30000000000E7E7E700E7E7E700E7E7
      E700E7E7E700E7E7E700E7E7E700E7E7E7007B8C5200A5C68400949C6B00736B
      3900E7E7E700E7E7E700E7E7E700000000000000000000000000000000000000
      00000000000008A6130056DD770029D04F0012C5350063D6790008A613000000
      00000000000000000000000000000000000000000000B5A59400FFEFEF00CEBD
      AD0094846B00846B5A00CEBDAD0094846B00846B5A00CEBDAD0094846B00846B
      5A00C6B5A500F7E7D600634A3100000000000000000000000000000000000000
      00000000000018203D000B5DC500077DE7000892FF00058EE4004E626A00F9F9
      F9000000000000000000AEAEBD002B2E5200635A2900635A2900635A2900635A
      2900635A2900635A2900635A2900635A2900738C42009CD67300A5C68C008C94
      6B00736B3900E7E7E700E7E7E700E7E7E7000000000000000000000000000000
      00000000000008A613006AE48A0045DA6B0029D04F0072DF8A0008A613000000
      00000000000000000000000000000000000000000000B5A59400FFF7EF00D6BD
      B500A594840094846B00D6BDB500A594840094846B00D6BDB500A59484009484
      6B00CEB5AD00F7E7DE00634A3100000000000000000000000000B9B9B9009698
      9400868582000C125E000D218A000A3CA2000859C5000C89FD00058ED6005068
      7200E2E6E700868A8C000422870069728F005A6B31007B9C63007B9C6300739C
      6300739C63007B9C6300739C6300739C63007BBD5A008CD66B00A5DE8C00A5CE
      94008C9C6B00736B3900CECECE00E7E7E70047B54C00069E0E00069E0E00069E
      0E00069E0E0008A613007AE799005BE17F0041D968007EE4960008A6130008A6
      130008A6130008A6130008A6130049BD510000000000B5A59400FFF7EF00EFDE
      D600E7D6C600E7CEC600EFDED600DEC6B500DEBDB500E7CEC600D6B5AD00CEB5
      A500E7D6CE00F7E7DE00634A31000000000000000000B9B9BA005A62360098A1
      6300868B4A00232A4500090D66000E2285000F40A500036BD9000BA0FF00089A
      E400165DA7000535BB000B1E8A00BAB9BB00636B39007BBD63007BBD6B007BBD
      6B007BC673007BBD6B007BBD6B007BC6730084CE730094D67B00A5DE9400ADDE
      A500B5D6AD0084946300736B3900E7E7E700059A0C0066E186006CE48E0081E9
      9E0089EBA7008DECA80083EAA10068E48C0058E07D0087E7A00091E8A50087E4
      9B007DDD900075D7840088D78D0008A6130000000000BDA59400FFF7EF00DEC6
      BD0094846B00846B5A00DEC6BD0094846B00846B5A00DEC6BD0094846B00846B
      5A00D6BDB500F7E7DE00634A3100000000006F715500969F5D00B0C16B0099A7
      6800A19F6C008586500018203E0006147000102B9100095BCB00089CFD000BAB
      FF000777FF000934D1004B577D00000000006373390063C652006BD663006BDE
      630073DE6B006BD663006BDE630073DE6B0084DE7B0094DE94009CDE9C00B5E7
      AD00CEEFBD00CEE7C600A5AD8C008C9C6B00059A0C0049D76A0033D5590049DC
      6F0061E3850073E6940076E8980073E6940068E48C0058E07D0045DA6B0031D3
      58001BC940002CC849005ECE6C0008A6130000000000BDAD9C00FFF7F700DECE
      C600A594840094846B00DECEC600A594840094846B00DECEC600A59484009484
      6B00D6C6BD00F7E7DE00634A310000000000747469008F9C4F00B1C06500AEBD
      6D009FA365008B946B00444646000A0D4D000E197E00084BB5000894FE0005A9
      FF000C59E600062AA00091929400000000007B84520073CE6B007BDE7B007BDE
      7B007BDE7B007BDE7B007BDE7B007BDE7B008CE78C009CE79C009CE79C00B5E7
      AD00CEEFC600E7F7E700CEDEBD008C9C6B00059A0C003DD15D003FD35F0031D3
      580047DB6D005FE283006FE691007EEA9D007EEA9D0076E8980064E388004FDE
      750037D65F0021CD460064D77A0008A6130000000000C6AD9C00FFF7F700FFEF
      E700E7D6C600E7CEC600FFEFE700DEC6B500DEBDB500E7CEC600D6B5AD00CEB5
      A500F7E7DE00FFEFE700634A310000000000F9F9FA007F7F7F003C3B2D006164
      3B00838D5D003E3C4A0017172D0021214300090B6D000C43AA000696FD0009A4
      FE00084FD800162B8A00EAEAEC00000000008C946B0094DE94009CE79C009CEF
      9C009CEF9C009CE79C009CEF9C009CEF9C009CEF9C00A5EFA500B5EFB500CEEF
      C600E7F7E700CEE7BD008C9C6B0000000000059A0C0034CD530035CE54003FD3
      5F004ED970005FDF800058E07D006CE58E007AE99A0081EAA10086EBA4007AE9
      9A0064E388004FDE750052DB730008A6130000000000CEB5A500FFF7F700E7D6
      CE0094846B00846B5A00E7D6CE0094846B00846B5A00E7D6CE0094846B00846B
      5A00DECEC600FFEFE700634A3100000000000000000000000000F2F2F200B7B7
      B7004E4E5100575736004E4E35003E3C34000A154E000A309A000AA4FD000AAC
      FF000C38CC007378A70000000000000000008C9C6B00DEF7DE00E7F7E700E7F7
      E700E7F7E700E7F7E700E7F7E700E7F7E700BDF7C600B5EFB500CEF7CE00E7F7
      E700D6E7C6008C9C6B00000000000000000047B54C00059A0C00059A0C00059A
      0C00059A0C00059A0C0056DD770058E07D006CE58E0089ECA800069E0E00069E
      0E00069E0E00069E0E0008A6130049BD510000000000CEB5A500FFF7F700EFDE
      D600A594840094846B00EFDED600A594840094846B00EFDED600A59484009484
      6B00E7D6CE00FFEFE700634A3100000000000000000000000000000000000000
      00000000000069695900565623006C6C3F0035314200081488000BAEF50005A3
      FE000830B000BCBDC10000000000000000008C9C6B008C9C6B008C9C6B008C9C
      6B008C9C6B008C9C6B008C9C6B008C9C6B008C9C6B00CEF7CE00E7F7E700D6E7
      C6008C9C6B000000000000000000000000000000000000000000000000000000
      000000000000059A0C0043D564003FD9650054E07B007AE99A00059A0C000000
      00000000000000000000000000000000000000000000D6BDAD00FFFFF700EFE7
      DE00D6C6BD00CEB5AD00EFE7DE00D6C6BD00CEB5AD00EFE7DE00D6C6BD00634A
      3100634A3100634A3100634A31000000000005B4470005B4470005B4470005B4
      470005B4470000000000B2B2B40039392300726D33000618690007C2F1000698
      F40022297800F9F9F90000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008C9C6B00E7F7E700D6EFCE008C9C
      6B00000000000000000000000000000000000000000000000000000000000000
      000000000000059A0C0038CF570026CF4D003AD7610061E38500059A0C000000
      00000000000000000000000000000000000000000000D6BDAD00FFFFF700FFFF
      F700FFFFF700FFFFF700FFF7F700FFF7F700FFF7F700FFF7EF00FFF7EF00CEB5
      A500D6C6BD00634A3100C6B5A5000000000000000000000000000000000005B4
      470005B447000000000000000000B3B3B3005753270040324F0008ACE5000777
      E90081809F000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008C9C6B00D6EFC6008C9C6B000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000059A0C002FCA4C0039D0580024CE4A0045DA6B00059A0C000000
      00000000000000000000000000000000000000000000DEC6B500FFFFF700FFFF
      F700FFFFF700FFFFF700FFFFF700FFF7F700FFF7F700FFF7F700FFF7F700CEB5
      A500634A3100C6B5A5000000000000000000000000000000000005B447000000
      000005B44700000000000000000000000000858482004C4335000A8AD1001A5C
      AE00ECECF0000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008C9C6B008C9C6B00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000059A0C0029C6450032CC51003AD159004ED97000059A0C000000
      00000000000000000000000000000000000000000000DEC6B500DEC6B500DEC6
      B500DEC6B500DEC6B500DEC6B500DEC6B500DEC6B500D6BDAD00CEBDAD00CEB5
      A500E7D6CE000000000000000000000000000000000005B44700000000000000
      000005B44700000000000000000000000000F3F3F300616056001F3771008081
      9A00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008C9C6B0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000047B54C00059A0C00059A0C00059A0C00059A0C0047B54C000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000005B4470000000000000000000000
      000005B4470000000000000000000000000000000000DBDBDB0055565800DADA
      DA00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000C6C6C600A5A5A5009C9C
      9C009C9C9C009C9C9C009C9C9C00A5A5A5009C9C9C009C9C9C009C9C9C009C9C
      9C009C9C9C009C9C9C00B5B5B500E7E7E7000000000000000000000000000000
      0000E6E6E600A0A0A0009090900094949400A9A9A900ABABAB00A8A8A800A6A6
      A6009A9A9A00808080008A8A8A00D2D2D2000000000000000000000000000000
      0000E6E6E6009E9E9C0084815B00C1C1C000A9A9A900ABABAB00A8A8A800A6A6
      A6009A9A9A00808080008A8A8A00D2D2D2000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000BDBDBD00636363005A5A5A006B6B
      6B005A5A5A00BDBDBD00BDBDBD009C9C9C00CECECE00C6C6C600BDBDBD008484
      8400212121002929290031313100CECECE000000000000000000000000000000
      00009191910059595900606060007B7B7B00C0C0C00079797900D4D4D400D8D8
      D80098989800212121001D1D1D00A1A1A1000000000000000000000000000000
      00009191910059595900838356008D8A6600D2D2D10079797900D4D4D400D8D8
      D80098989800212121001D1D1D00A1A1A1000000000000000000187B9C00187B
      9C00187B9C00187B9C00187B9C00187B9C00187B9C00187B9C00187B9C00187B
      9C0000000000000000000000000000000000848484006B6B6B00636363007373
      730063636300DEDEDE008484840039393900E7E7E700E7E7E700DEDEDE009C9C
      9C00292929003131310021212100C6C6C6000000000000000000000000000000
      000084848400666666006A6A6A0085858500B7B7B70048484800E2E2E200F0F0
      F000A9A9A900343434002C2C2C00A0A0A000E5E4DC00EBEAEB00E6E4E300E6E4
      E300C9C8C600C3C1C000919F680092A36E0089856000A5A4A300E2E2E200F0F0
      F000A9A9A900343434002C2C2C00A0A0A000000000001894C6001894C6001894
      C6001894C6001894C6001894C6001894C6001894C6001894C6001894C6001894
      C600187B9C000000000000000000000000008C8C8C006B6B6B00636363007373
      73005A5A5A00D6D6D6008484840042424200E7E7E700E7E7E700DEDEDE009C9C
      9C00313131003939390029292900C6C6C6000000000000000000000000000000
      0000929292006C6C6C00757575008B8B8B00BEBEBE008B8B8B00DDDDDD00E4E4
      E400A8A8A8003D3D3D0034343400A2A2A2006C6B3B006F6F40006D6E3F006C6E
      3F006D6E3F006B6D3C007DA44F00A0D585008C9E6B0088876000D6D5D400E4E4
      E400A8A8A8003D3D3D0034343400A2A2A200189CC600189CC6009CFFFF006BD6
      FF006BD6FF006BD6FF006BD6FF006BD6FF006BD6FF006BD6FF006BD6FF006BD6
      FF00299CBD000873A500000000000000000094949400737373006B6B6B008484
      840073737300DEDEDE00A5A5A5007B7B7B00FFFFFF0000000000F7F7F700ADAD
      AD00313131004242420031313100C6C6C6000000000000000000000000000000
      0000969696007574720083805A00B3B3B1007575750078787800686868006161
      6100585858004E4E4E003C3C3C00A4A4A4006A7D45007FAF6C007BAC6B007CAF
      6C007CAB6C007AAF6B0080C6670098D98300AADD9E0090A575007D7C5500ACAA
      A900585858004E4E4E003C3C3C00A4A4A400189CC600189CC6007BE7F7009CFF
      FF007BE7FF007BE7FF007BE7FF007BE7FF007BE7FF007BE7FF007BE7FF007BDE
      FF0042B5DE000873A50000000000000000009C9C9C0073737300737373007373
      7300737373008C8C8C0094949400949494008C8C8C0084848400848484006B6B
      6B00424242004A4A4A0031313100C6C6C6000000000000000000000000000000
      0000999999007D7D7D00848457008D8A6600B4B4B3006B6B6B00666666006060
      60005D5D5D006060600043434300A5A5A500688345006BCF60006FD5670072D6
      6A006ED3640072D76A0087DA7E009ADC9300ADE1A600C7E6C100A9B491009AA4
      7E005D5D5D006060600043434300A5A5A500189CC60021A5CE0039BDD6009CFF
      FF0084EFFF0084EFFF0084EFFF0084EFFF0084EFFF0084EFFF0084EFFF0084E7
      FF0042BDEF000873A50000000000000000009C9C9C007B7B7B007B7B7B007373
      7300737373006B6B6B00636363005A5A5A0052525200525252004A4A4A004A4A
      4A00525252005252520039393900C6C6C600E5E4DC00EBEAEB00E6E4E300E6E4
      E300CDCBCA00D8D7D60092A0690092A36E0089856000E6E5E500EFEFEF00EEEE
      EE00F6F6F600D7D7D7004E4E4E00A2A2A2007D99600075D975007AE17E007CE0
      7D0079DF7B007CE07E0091E38E009FE79C00B0E6AE00DCF4D900D6E8CC00A1B0
      8900F6F6F600D7D7D7004E4E4E00A2A2A200189CC60042B5E70021A5CE00A5FF
      FF0094F7FF0094F7FF0094F7FF0094F7FF0094F7FF0094F7FF0094F7FF0094F7
      FF0052BDE7005ABDCE000873A500000000009C9C9C007B7B7B007B7B7B007B7B
      7B007B7B7B0073737300737373006B6B6B006B6B6B0063636300636363005A5A
      5A005A5A5A005A5A5A0042424200C6C6C6006E6D3E006F6F40006D6E3F006C6E
      3F006D6E3F006B6D3C007DA44F00A0D585008C9E6B0088876000DBDAD900EAEA
      EA0000000000E3E3E30054545400A3A3A30097AC8000C6F5CC00C7F6CC00C5F6
      CA00C8F5CC00C5F6CA00ADF2B200B1EEB200D6F6D700D7EBCF00A8B69200E3E4
      E00000000000E3E3E30054545400A3A3A300189CC6006BD6FF00189CC6008CF7
      F7009CFFFF009CFFFF009CFFFF009CFFFF009CFFFF009CFFFF009CFFFF009CFF
      FF005AC6FF0094FFFF000873A500000000009494940094949400F7F7F700F7F7
      F700000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFFFF00F7F7
      F700F7F7F7007B7B7B0042424200C6C6C6006B8049007FAF6C007BAC6B007CAF
      6C007CAB6C007AAF6B0080C6670098D98300AADD9E0090A575007D7C5500DEDD
      DC00FCFCFC00E2E2E2005D5D5D00A4A4A40095A27700A9B69300A9B59200A9B5
      9200A5B18F00A4B08C00A2BF9100D7FCDC00D8EBCF00A6B39000DCDDD900E3E3
      E300FCFCFC00E2E2E2005D5D5D00A4A4A400189CC60084D6FF00189CC6006BBD
      DE00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF0084E7FF00FFFFFF000873A500000000009494940094949400FFFFFF00F7F7
      F700E7E7E700E7E7E700E7E7E700E7E7E700E7E7E700E7E7E700EFEFEF00FFFF
      FF00000000008484840042424200C6C6C6006A8749006BCF60006FD5670072D6
      6A006ED3640072D76A0087DA7E009ADC9300ADE1A600C7E6C100A9B491009AA4
      7E00FFFBFF00EFE9EF0065666500A9A9A900C3AA9A00E1F6F200E3F6F900E3F4
      F90093959200CBC8C900A4B48E00DDF2D800A8B39100EEE5EC00F3E9F300F3E9
      F400FFFBFF00EFE9EF0065666500A9A9A900189CC60084EFFF004AC6E700189C
      C600189CC600189CC600189CC600189CC600189CC600189CC600189CC600189C
      C600189CC600189CC6000873A500000000009494940094949400FFFFFF00F7F7
      F700EFEFEF00EFEFEF00EFEFEF00EFEFEF00EFEFEF00EFEFEF00F7F7F700FFFF
      FF00F7F7F700848484004A4A4A00C6C6C6007F9C640075D975007AE17E007CE0
      7D0079DF7B007CE07E0091E38E009FE79C00B0E6AE00DCF4D900D6E8CC0097B1
      7F00C2FFC100C1E4C100605C6000BBBBBB00C5AB9C00E5FAF800E7FAFF00E7F8
      FF0094909400B1C8B00097A77B00A3BC8D00B4EBB200B9F3B900B9F3B900B9F3
      B900C2FFC100C1E4C100605C6000BBBBBB00189CC6009CF7FF008CF7FF008CF7
      FF008CF7FF008CF7FF008CF7FF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00189CC600187B9C0000000000000000009494940094949400FFFFFF00F7F7
      F700E7E7E700E7E7E700DEDEDE00DEDEDE00E7E7E700DEDEDE00E7E7E700FFFF
      FF00F7F7F7008C8C8C004A4A4A00C6C6C60099AF8300C6F5CC00C7F6CC00C5F6
      CA00C8F5CC00C5F6CA00ADF2B200B1EEB200D6F6D700D7EBCF0088AE70005BC0
      510052C34A0072B96D008A858A00DCDBDC00C6ADA000E7FCFB00E9FDFF00E9FC
      FF00AFAAB0007DAF79007CA363005AC0510057C3510057C3500057C3500055C0
      4D004EBE450072B96D008A858A00DCDBDC00189CC600FFFFFF009CFFFF009CFF
      FF009CFFFF009CFFFF00FFFFFF00189CC600189CC600189CC600189CC600189C
      C600189CC6000000000000000000000000009C9C9C0094949400FFFFFF00F7F7
      F700EFEFEF00EFEFEF00EFEFEF00EFEFEF00EFEFEF00EFEFEF00EFEFEF00FFFF
      FF00FFFFFF008C8C8C0052525200C6C6C60099A87E00AAB69300A9B59200A9B5
      9200A9B59300A6B28E00A2BF9100D7FCDC00D8EBCF00ABB99500F6F7F3000000
      000000000000000000000000000000000000C5AB9D00E5F2EF00E5F5F400E4F5
      F300E5F6F400E6F7F400E7F8F400E6F7F400E3F4F300E7F6F500E8EBE600BDAB
      A100877873000000000000000000000000000000000021A5CE00FFFFFF00FFFF
      FF00FFFFFF00FFFFFF00189CC600000000000000000000000000000000000000
      0000000000000000000000000000000000009C9C9C009C9C9C00FFFFFF00F7F7
      F700E7E7E700E7E7E700E7E7EF00EFEFEF00EFEFEF00EFEFEF00EFEFEF00FFFF
      FF00FFFFFF008C8C8C005A5A5A00D6D6D600FBFBFA00FAFBF900FAFBF900FAFB
      F900FAFBF900F6F7F300A4B48E00DDF2D800ABB89400F9FAF800000000000000
      000000000000000000000000000000000000BB865B00C9AE8C00C7AA8900C9AA
      8800CBAA8A00CDAF8B00CFB08D00CBAC8C00C8A98800CAB08E00CBAC8800B08D
      730089797100000000000000000000000000000000000000000021A5CE0021A5
      CE0021A5CE0021A5CE0000000000000000000000000000000000000000000000
      00000000000000000000000000000000000094949400949494008CF78C007BF7
      7B0084F7840084F7840084F7840084F7840084F7840084F7840084F784007BF7
      7B0094EF94008C84840052525200EFEFEF000000000000000000000000000000
      000000000000FBFCFA0097A77B00B2BF9C00F6F7F40000000000000000000000
      000000000000000000000000000000000000C76A0800C7620500C75F0500C660
      0500C75F0500C6600500C65E0500C55B0500CA610500CD690500BE620700B67F
      4E00C7B6A5000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000B5B5B50094A5940052B54A004AAD
      42004AAD42004AAD42004AAD42004AAD42004AAD42004AAD42004AAD42004AAD
      42005AAD52009C9C9C008C8C8C00FFFFFF000000000000000000000000000000
      000000000000F7F8F500A5B18D00F6F7F3000000000000000000000000000000
      000000000000000000000000000000000000FAE7CF00FDE7CB00FDE6C900FDE6
      C900FDE6CA00FDE6CA00FDE5CA00FCE4C700FDE5C800FFE7CC00FAE4CD00F6EB
      DF00FFFFFD00000000000000000000000000424D3E000000000000003E000000
      2800000040000000700100000100010000000000800B00000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000E003FFFFF81FF81FC001FFFFE007F00F
      8000FFFFC003E0078000FFFF8001C0030100FFFF8001C003000000010000C003
      000000010000C003000000000000C003000000000000C003000000000000C003
      000000000000C003000000008001C0038001FFFF8001E0078001FFFFC003F00F
      C007FFFFE007F80FF00FFFFFF81FFC3FFC3FFFFFF007EFFFF00F8001E003E3FF
      C00387F9C001E1FFC00381F38000F07F800198E78000F83F80009C070000F80F
      00008E0D000000070000803D000000010000A119000000010000B3890000803F
      000081C10000801F8001A0E38000C00F80018C078001C00FC0038E1FC003E007
      E0079FFFE007E003F81FFFFFF81FFFFFFFFFC003FC0F00000000C003F8070007
      0000C003F80100000000C003F00000000000C003E00000000000C003E0000001
      0000C003C00040000000C003C00000020000C003820130000000C00382014000
      0000C003820322000000C003018370000000C003080370000000C0031C070000
      FFFFC0037E3F7E00FFFFE0077E7F000FCEFFCFFFCFFF0000CFFFCFFFCFFF0000
      C2FFC3FF43FF0000E2FFE3FF23FF0000E3FFE3FFC3FF0000F0FFF1FFC1FF0000
      F8FFF9FFF1FF0000FC7FFC7FF07F0000FC7F2412FC7F0000FF1FFF1FFC1F0000
      FE07FF07FF070000FEC1FFC1FF010000FEF0FFF0FFD00000FFFCFFFCFFCC0000
      FEFFFFFFFFF70000FEFFFFFFFFF300003FFF0000FE3F00001FFF0000F0274FFB
      87FF0000E00342FFC3FF0000C00722FFC1FF8001C00143FFE0FF8001800040FF
      F07FC003000070FFF82FC0030181707FFC07E00781802412FE03E00700007C1F
      FF01F00F00017E07FF80F00F80037E01FFC0F81FE0037ED0FFE0F81FC0073FCC
      FFF0FC3FE40F7EF7FFF8FC3FFC7F7EF3F00FF807FFFDFF7FF00FF807FFF8FF7F
      F007F007FFF1FF7FF007E003FFE3FF7FF087C003E047FFFFF007C001800FFFFF
      D00F8001801FFFFFC00F0001000F8770C03F0000000FFFFFD43F9000000FFFFF
      DE7FE000000FFFFFDCFFE000000FFF7FD9FFC005000FFF7FD3FFC007801FFF7F
      C7FFE40F803FFF7FCFFFFE7FF0FFFFFFDFFF0000FBFFFC01DFF97FFBFBFFF000
      00007FFFFBFFF000D1897B88FFFFE400D18F4BAA0A3FE400D18F05AA0A3FC800
      D18F01AAE0F15000D18F2088C0E10200D18F68CF80011E00D18F690300030203
      D18F683100000207DF8F623D0001021FDF8F7E7F0003061F8F8F3FFF80070E1F
      8FFF7FFFC00F1E3FDFFF7FFFE09F3FFFDFFFDFFFDFFFDFFFDFF9DFF9DFF9DFF9
      0000000000000000DFF9DFF9DFF9C001DFFFDFFFDFFFC003DFFFDFFFD000C003
      C7FFDFFFDA16C003D3FBDFF7DB3ED003D9F3DBE3DB98D803DCE7D1F7DBCCDC07
      DE4FDBFFD246DE0FDF1FDFBFDB10DF1FDFBFDF1FDFFFDFBF8FFF8FBF8FFF8FFF
      8FFF8FFF8FFF8FFFDFFFDFFFDFFFDFFFCFE300020000DFFFCF0B0002478BDFF9
      CF03000246010000CF4300021001C001C30300023800C003030300027000C003
      270300027020C003E30700024870D003E01F00020020D003F03300020000D007
      F03300003000D00FF00000027801DF0FF30000007801DF8FFFE700003C018F8F
      FFE700027E3D8FFFFFFF00257E7FDFFFFFFFFFFF00000000D557BBBB47CB4C78
      D5570001478700300001AB3B11A70000D557BBBB383330010001BBBB7E797903
      D55700017F0F79030001B9BB4E034C01D557BBBB060300000001BBBB03032030
      C5170001300330780001BBBB78037849D557ABBB790379030001BBBB3C313C31
      D55700017E3D7E38D557BBBB7E7F7E7E00001FC3FFFFFFFF77767FC18001EDB7
      777640008001EDB7FFFF400080018001800140008001EDB7800140008001EDB7
      80010000800180018001400E8001EDB78000401F8001EDB78FFF461F80018001
      880F7E1F8001EDB7FC0F1E0F8001EDB7FE1F7E0F80018001FC0F7C078001EDB7
      F80F1C078001EDB7FC9F7E4F8001FFFF07E007E007E0DFFF07F007F007F0DBF9
      03E003E003E00000000000000000DBF9420042004000DFFFE003E003E003DFFF
      E007E007E007DC1FE007E007E007DC1FE007E007E007DC0FE007E007E007DC0F
      C007C007C0079D0F000200020002DF9F000000000000DFFF07C007C007C08FFF
      0FE00FE00FE088FF07E007E007E0DDFFFFF78001FFF8FFF0FFF70000FFF0FFE0
      FFFF0000FFE0FFC033320000FFC0FF81FFFF0000FF81FF03113F0000C003C007
      BB5700008007800FBBD70000000F001FBD9F0000001F001FB5DF0000001F001F
      39170000001F001FFFF70000001F001FFFFF0000001F001FFFFF0000001F001F
      FFF70000803F803FFFF78001C07FC07F00C3C107FFE0800000008001FFC00000
      0000000100100000000000010000000000010001008000000000000100010000
      0000000100010000000000010000000000010001000000000000400100000000
      4000000100000000000000010001000040000001000100000000000100000000
      000000010000000100000003000087FF800787E100001FFF000703C077767FFF
      000701804002400300070000C003400300070000C003000300078001C0034003
      0007C003C00340030006E007C00340030004E007C00300030000C003C0034003
      00008001C003400300000000C1834183F0000000C0030003F8000180E0076007
      FC0003C0E0076007FFFF87E1F00F100FF81FF00FFFFFFFFFE007E007FFFFFC3F
      C003C0030000F00FC00080010000C00380000000000000008600000000000000
      8600000000000000810100000000000000810000000000000061000000000000
      00C100000000000000030000400000000003800100008001C007C00300008001
      E00FE007FFFFF00FF81FF00FFFFFF81FFC0080003FFFE1FFFC0000000F3FE0FF
      E0000000041FE00FE0000000800FC00700000000C007C00100000000E007C001
      00000000C00FC00100000000C001C00700000000C000F00700000000C000F007
      00010003C000E08F00030003C000E0FF000F0003E000C1FF001F0003FF01C1FF
      007F0007FF03C3FF00FF001FFF87C3FF0000C003FF03FC0000000001FF03FC00
      000000008303FC00000000008003FC0000000000800300000000000080030000
      0000000080030000000000008007000000000000C007000000000000E1030000
      00000000FF03000100000000FF01000300000000FF01003F00000000FF01003F
      00000000FF19007F0001C003FF1F00FFFC000000000080000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0007000000000000000700000000000000070000000000000007000000000000
      000F000000000000001F000100010000C00307FF80008001800103FF00008001
      000001FF00008001000000FF000080010000007F800080000000802FC0008001
      0000C001E00080010000E000F00080010000F000F00080018001F800F0008001
      E007FC00F0008001E007FC08F0008001F00FFC0CF0008001F00FFC0FF0008003
      F00FFE03F0018007F00FFF03F003800FFF0FF81F8001FC7FFF07F81F8001F83F
      8003F81F8001F81E8001F81F8001F80C0000F81F8001C0000000000080018000
      000000008001000100000000800100010000000080010001000100008001C003
      000300008001F8030007F81F80010403FF0FF81F8001E607FF1FF81F8003D707
      FF3FF81F8007B70FFF7FF81FFFFF778FFFFF8000F000F000FFFF0000F000F000
      C00F0000F000000080070000F000000000030040F000000000030000F0000000
      0003000000000000000100000008000800010E00000000000001000800000000
      0001000000000000000300000000000000070000001F000781FF0000003F0007
      C3FF0000F87F0007FFFF0000F8FF000700000000000000000000000000000000
      000000000000}
  end
  object appEvents: TApplicationEvents
    OnActivate = appEventsActivate
    OnDeactivate = appEventsDeactivate
    OnException = appEventsException
    OnHelp = appEventsHelp
    OnHint = appEventsHint
    Left = 8
    Top = 1
  end
  object popMeasure: TPopupMenu
    AutoPopup = False
    Images = imlActions
    Left = 195
    Top = 1
    object pitMeasureAbscissa: TMenuItem
      Action = actMeasureAbscissa
    end
    object pitMeasureOrdinate: TMenuItem
      Action = actMeasureOrdinate
    end
    object pitMeasureSlope: TMenuItem
      Action = actMeasureSlope
    end
    object pitMeasureMarkers: TMenuItem
      Action = actViewMeasureMarkers
    end
  end
  object popEdit: TPopupMenu
    AutoPopup = False
    Images = imlActions
    Left = 229
    Top = 1
    object pitCompute: TMenuItem
      Action = actCompute
    end
    object pitTrackTransform: TMenuItem
      Action = actTrackTransform
    end
  end
  object popPaneGrid: TPopupMenu
    Images = imlActions
    Left = 264
    Top = 1
    object pitCopyGridAll: TMenuItem
      Action = actCopyGrid
    end
  end
  object imlTree: TImageList
    Height = 10
    Width = 10
    Left = 21
    Top = 46
    Bitmap = {
      494C01010300040004000A000A00FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000280000000A00000001002000000000004006
      000000000000000000000000000000000000D2CBC100B8AD9F00B9AEA000B9AE
      A000BBAFA100BDB4A600BBB2A400B9AD9F00B9B0A300DADAD900CBBBB5008E7B
      730094827A0094827A0093827A0094827A0094827A0094827A0096837B008882
      8000FFFFFF00A670E800C9A7F100FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C9A88200CAC4BD00CEC5BB00CFC6
      BC00BEBAB000B3A49900C9B9AE00D4D0C700B7A99600B9AFA200C8A58F00D4C5
      B700D3C2B400D4C4B800DEC9B300D6C4B500D3C3B700D5C4B500D7B9A8009684
      7C00FFFFFF00DDC8F6009859E400FBFAFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E3CDB800EAF0F400ECEDEE00F4F9
      FB00C8C0C10094515200B5707100EADEE100D3D0C600B8AD9F00CABAAE00E9FF
      FF00E8FCFF00E8FCFF00E9FCFF00E9FCFF00E8FCFF00EBFFFF00E3E5E2009380
      7800FFFFFF00FFFFFF009E64E600C19CEF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DBC1AC00E5E4E700EEEFEF00F4F9
      F900BC7F7F00CA989800CDA1A100BF818200D7D5CA00B9AFA100CAB8AD00E8FF
      FF00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E9FDFF00E1E0DF009380
      7800FFFFFF00FFFFFF00EDE2FB008A46E100E8DDFB0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000B5756100AF8C8F00D2DDDD00C3B6
      B6008C3D3D00C5C8C800CCCECE00A5575800B6A29800BCB5A700CAB8AD00E9FF
      FF00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E9FDFF00E1E0DF009380
      7800FFFFFF00FFFFFF00FFFFFF00CAA9F2008C4CDA00F9F6FC00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000B6776300AA7E8000D2DDDD00BEA9
      A900883D3D00C2CACA00D0D6D600A85C5E00AF928800BDB5A700CAB8AD00E9FF
      FF00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E7F8FF00E9FDFF00E1E0DF009380
      7800FFFFFF00FFFFFF00FFFFFF00FFFFFF00B184EA009656E400F0E7FB000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E1CEBA00A45E6000EADBDB00B566
      6600B9A5A500D0D6D600F6F9F900DCCACC008C3B3100BDB6A800CBB9AE00EAFF
      FF00E8FAFF00E8FAFF00E7F9FF00E7F9FF00E8FAFF00EAFFFF00E2E4E1009380
      7800FFFFFF00FFFFFF00FFFFFF00FFFFFF0000000000BC93EE009250E300C5A2
      F000F7F3FD000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E3D3C000CDACAE00A3585800C08C
      8C00CCCCCC00CBCDCD00EEEFEF00EBEAEC00B0877C00BAB1A300C8B5A900E2F5
      F500DFEDF000E2EEF100E3F1F200E4F0F300E0EDF000DFEFF100DFDDD5008F7C
      7500FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000E0CDF700A56F
      E8009250E300B589EC0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DBC2AE00E5EEF300D8D0D300E9EE
      F200C7CDD100C5C9CD00E9EEF200E9F1F700CDCFC700B8AD9E00BC661100BF6B
      1400BD691000BF6A1200C06C1200BF691000BE691100C5721900B36C2A00A68D
      7700FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000E9DBF900B182EB0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000DDB69600DCC0A700E0C1A400E1BD
      9B00CEA58100CEA47F00E0B79200E1B69200CBA37400D1CBC000F4D4AD00F7D1
      A500F7D0A300F7D1A500F6D0A500F6CFA200F6CFA200FAD4A700EDCFB000FCF4
      EB00FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      28000000280000000A0000000100010000000000500000000000000000000000
      000000000000000000000000FFFFFF00000000FC00000000000000FC00000000
      000000FC000000000000007C000000000000003C000000000000001C00000000
      0000008400000000000000C000000000000000F000000000000000FC00000000
      00000000000000000000000000000000000000000000}
  end
  object popFoilObject: TPopupMenu
    Images = imlActions
    Left = 160
    Top = 1
  end
end
