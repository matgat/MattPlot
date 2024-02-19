//---------------------------------------------------------------------------
#ifndef unt_PropertiesInterfaceH
#define unt_PropertiesInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*
	*		unt_PropertiesInterface
	*
	*		ver: 0.5    (31.jul.2007)
	*		2008 - Matteo Gattanini
	*
    *       This unit defines a class tree related to
    *       plot objects properties interface, for
    *       runtime editing.
	*
	* * * * * * * * * * * * * * * * * * * * * * * * * * * */

////////////////////////// cls_PropertiesInterface //////////////////////////
//---------------------------------------------------------------------------
class cls_PropertiesInterface { // Creates interface to edit properties

public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_PropertiesInterface (class TWinControl* p, class cls_FoilObject* fo) : i_Parent(p), i_FoilObject(fo)
                                       {i_Margin=10;} // Screen->MenuFont->Size
    virtual __fastcall ~cls_PropertiesInterface() {;}

    void Apply() {Apply(i_FoilObject);} // Apply values to linked foil object

    virtual void Apply(class cls_FoilObject*) =0; // Apply values
    virtual void Revert(class cls_FoilObject*) =0; // Revert values
    virtual void AlterOptions(class cls_OptionsSet*) {;} // Alter options

// . . . Properties
    __property class TWinControl* Parent = { read=i_Parent, write=i_Parent};
    __property int Margin = { read=i_Margin, write=i_Margin};
    __property int width = { read=get_width};

protected://                                                    ~~~~~~~~~~~~~
// . . . Protected methods
    virtual void __fastcall Update(TObject*) =0; // Update interface
    class cls_FoilObject *i_FoilObject; // Passed object

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
// . . . Properties
    TWinControl* i_Parent;
    int i_Margin;
    int get_width() const {return i_Parent->Width - 2*i_Margin;}
};//-------------------------------------------------------------------------


//////////////////////////// cls_FoilObjectEdit /////////////////////////////
//---------------------------------------------------------------------------
class cls_FoilObjectEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_FoilObjectEdit (class TWinControl*, class cls_FoilObject*);
    virtual __fastcall ~cls_FoilObjectEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*); // Control change

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes

    // Interface
    TPanel* pnlFixed;
    TPaintBox* pbxStyle;
    TCheckBox* chkVisible;
    TComboBox *cbxLineWeight;
    TComboBox *cbxColorMode, *cbxLineStyle, *cbxAreaStyle;
    TShape *shpLineColor, *shpAreaColor;
    TComboBox *cbxMarginX, *cbxMarginY;
    TLabel *btnFont;

    // Events
    void __fastcall PaintPreview(TObject*); // Style preview
    void __fastcall ShapeMouseDown(TObject*, TMouseButton, TShiftState, int, int); // Color dialog
    void __fastcall FontMouseDown(TObject*, TMouseButton, TShiftState, int, int); // Font dialog
};//-------------------------------------------------------------------------


/////////////////////////////// cls_FrameEdit ///////////////////////////////
//---------------------------------------------------------------------------
class cls_FrameEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_FrameEdit (class TWinControl*, class cls_Frame*);
    virtual __fastcall ~cls_FrameEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Frame *i_Frame; // Passed object

    // Interface
    TEdit* edtFrameTitle;
    TEdit* edtSampPerPix;
};//-------------------------------------------------------------------------

/////////////////////////////// cls_TrackEdit ///////////////////////////////
//---------------------------------------------------------------------------
class cls_TrackEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_TrackEdit (class TWinControl*, class cls_Track*);
    virtual __fastcall ~cls_TrackEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Track *i_Track; // Passed object

    // Interface
    TEdit* edtTrackName;
    TCheckBox* chkMarkPoints;
    TComboBox* cbxPointStyle;
    TEdit* edtPointSize;
    TCheckBox* chkStrokeLines;
    TCheckBox* chkFillArea;
    TCheckBox* chkDrawHistograms;
    TEdit* edtHistWidthPerc;
    TCheckBox* chkShowCoords;
    TEdit* edtCoordsDist;
};//-------------------------------------------------------------------------


//////////////////////////////// cls_AxesEdit ///////////////////////////////
//---------------------------------------------------------------------------
class cls_AxesEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_AxesEdit (class TWinControl*, class cls_Axes*);
    virtual __fastcall ~cls_AxesEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Axes *i_Axes; // Passed object

    // Interface
    TEdit* edtXlabel;
    TEdit* edtYlabel;
};//-------------------------------------------------------------------------


//////////////////////////////// cls_GridEdit ///////////////////////////////
//---------------------------------------------------------------------------
class cls_GridEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_GridEdit (class TWinControl*, class cls_Grid*);
    virtual __fastcall ~cls_GridEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*);

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Grid *i_Grid; // Passed object

    // Interface
    TEdit* edtVGridDist;
    TEdit* edtVGridDivs;
    TEdit* edtHGridDist;
    TEdit* edtHGridDivs;
};//-------------------------------------------------------------------------


/////////////////////////////// cls_LegendEdit //////////////////////////////
//---------------------------------------------------------------------------
class cls_LegendEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_LegendEdit (class TWinControl*, class cls_Legend*);
    virtual __fastcall ~cls_LegendEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Legend *i_Legend; // Passed object

    // Interface
    TEdit* edtStrokeLength;
};//-------------------------------------------------------------------------


/////////////////////////////// cls_MarkerEdit //////////////////////////////
//---------------------------------------------------------------------------
class cls_MarkerEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_MarkerEdit (class TWinControl*, class cls_Marker*);
    virtual __fastcall ~cls_MarkerEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Marker *i_Marker; // Passed object

    // Interface
    TEdit* edtValue;
    TEdit* edtPrefix;
    TCheckBox* chkVertical;
};//-------------------------------------------------------------------------


//////////////////////////////// cls_LabelEdit //////////////////////////////
//---------------------------------------------------------------------------
class cls_LabelEdit : public cls_PropertiesInterface {
// A group of controls to change FoilObject properties
public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_LabelEdit (class TWinControl*, class cls_Label*);
    virtual __fastcall ~cls_LabelEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    virtual void AlterOptions(class cls_OptionsSet*);

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private attributes
    class cls_Label *i_Label; // Passed object

    // Interface
    TEdit* edtText;
    TEdit *edtX, *edtY;
    TCheckBox* chkShowCoords;
};//-------------------------------------------------------------------------


////////////////////////// cls_SimpleActionInterface ////////////////////////
//---------------------------------------------------------------------------
class cls_SimpleActionInterface : public cls_PropertiesInterface { // A button

public://                                                       ~~~~~~~~~~~~~

// . . . Public methods
    __fastcall cls_SimpleActionInterface (class TWinControl*, class cls_Frame*, const String&);
    virtual __fastcall ~cls_SimpleActionInterface();

    virtual void Apply(class cls_FoilObject*) {;}
    virtual void Revert(class cls_FoilObject*) {;}
    virtual void AlterOptions(class cls_OptionsSet*) {;}

protected://                                                    ~~~~~~~~~~~~~

// . . . Protected methods
    virtual void __fastcall Update(TObject*) {;}

private://                                                      ~~~~~~~~~~~~~

// . . . Private methods
    void __fastcall OnClick(TObject*);

// . . . Private attributes
    class cls_Frame *i_Frame; // Passed object

    // Interface
    TButton *btnDo;
};//-------------------------------------------------------------------------


///////////////////////////// cls_FoilObjectsEdit ///////////////////////////
//---------------------------------------------------------------------------
class cls_FoilObjectsEdit : public cls_FoilObjectEdit { // A group

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef std::vector<class cls_FoilObject*> typ_FoilObjects;
    typedef typ_FoilObjects::iterator  typ_FoilObjectsIt;

// . . . Public methods
    __fastcall cls_FoilObjectsEdit (class TWinControl* p, typ_FoilObjects* o)
             : cls_FoilObjectEdit(p,(o->size()>0)?o->front():0), i_PObjects(o) {;}
    virtual __fastcall ~cls_FoilObjectsEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    //virtual void AlterOptions(class cls_OptionsSet*);

private://                                                      ~~~~~~~~~~~~~

// . . . Private methods
    void __fastcall OnClick(TObject*);

// . . . Private attributes
    typ_FoilObjects *i_PObjects;
};//-------------------------------------------------------------------------


/////////////////////////// cls_PlotFramesStyleEdit /////////////////////////
//---------------------------------------------------------------------------
class cls_PlotFramesStyleEdit : public cls_FoilObjectEdit { // A group

public://                                                       ~~~~~~~~~~~~~
// . . .  Types definitions
    typedef std::list<class cls_FoilObject*> typ_FoilObjects;
    typedef typ_FoilObjects::iterator  typ_FoilObjectsIt;

// . . . Public methods
    __fastcall cls_PlotFramesStyleEdit (class TWinControl* p, typ_FoilObjects* o)
             : cls_FoilObjectEdit(p,(o->size()>0)?o->front():0), i_PObjects(o) {;}
    virtual __fastcall ~cls_PlotFramesStyleEdit() {;}

    virtual void Apply(class cls_FoilObject*);
    virtual void Revert(class cls_FoilObject*);
    //virtual void AlterOptions(class cls_OptionsSet*);

private://                                                      ~~~~~~~~~~~~~

// . . . Private methods
    void __fastcall OnClick(TObject*);

// . . . Private attributes
    typ_FoilObjects *i_PObjects;
};//-------------------------------------------------------------------------


//---------------------------------------------------------------------------
#endif
