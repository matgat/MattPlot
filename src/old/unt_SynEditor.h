//---------------------------------------------------------------------------

#ifndef unt_SynEditorH
#define unt_SynEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmSynEditor : public TForm
{
__published:	// IDE-managed Components
        TMemo *synEditor;
private:	// User declarations
        bool get_Modified(){return synEditor->Modified;}
public:		// User declarations
        TfrmSynEditor(TComponent*,String);

        // . . . I/O
        void Open( const String&  ); // Load from path
        void Save( const String&  ); // Save to path

        // . . . Properties
        __property bool Modified = { read=get_Modified };
};
//---------------------------------------------------------------------------
#endif
