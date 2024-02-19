#ifndef unt_OptionValuesH
#define unt_OptionValuesH

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * *
        *
        *    unt_OptionValues    ver: 0.5    (feb.2009)
        *    2009 - matteo.gattanini@gmail.com
        *
        *    Overview
        *    --------------------------------------------
        *    This unit contains a class tree to manage
        *    different option value types
        *
        *    Licenses
        *    --------------------------------------------
        *    Use and modify freely
        *
        *    Release history
        *    --------------------------------------------
        *
        *    Example of usage:
        *    --------------------------------------------
        *    #include "unt_OptionValues.h"
        *    cls_OptionValue* v = new cls_OptionBool("Edit","ScrollOn","comment",true);
        *    try { o->ReadFromFile("C:\\file.ini",v); }
        *    catch(EConvertError& e) {ShowMessage("Invalid option: " + e.Message);}
        *    delete v;
        *
        * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "unt_MatCommon.h"

//---------------------------------------------------------------------------
class cls_OptionValue {// Abstract class
public://							-------------
        // . . . Public methods
	cls_OptionValue (const String& t ="",const String& l ="",const String& g ="",const String& c ="")
	: Type(t),Label(l),Group(g),Comment(c) {;}
    ~cls_OptionValue ()  {;}

    virtual void __fastcall SetDefault() =0; // Set value to default
    virtual String __fastcall GetDefault() =0; // Get default string

// . . . Operators
	// Assegnamento e cast verso `String'
	virtual cls_OptionValue& operator= (const String&) =0;
	virtual operator String() const =0;
    virtual String StringRep() const {return String(*this);} // different only for 'cls_OptionString'

// . . . Properties
	//__property String Value = {read=get_Value, write=set_Value};

// . . . Public attributes (!)
    const String Group;
    const String Label;
    const String Comment;
    const String Type;

private://							-------------
// . . . Private attributes
// . . . Private methods
// . . . Properties
	//String __fastcall get_Value() const {return i_Value;}
	//void __fastcall set_Value(const String& s) { i_Value=s; }

};// end cls_OptionValue definition -----------------------------------------




//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionString : public cls_OptionValue {// String values
	public:
        // . . . Public methods
		cls_OptionString (const String& g ="",const String& l ="",const String& c ="",const String& d ="")
                : cls_OptionValue("String",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return mat::HasNonVisibleChars(DefaultValue) ? mat::ToQuotedString(DefaultValue) : DefaultValue;}

        // . . . Operators
	        // Assegnamento e cast verso `String'
	        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s); return *this;}
	        virtual operator String() const {return Value;}
            virtual String StringRep() const {return ToString(Value);}
	
	private:
	// . . . Private attributes
            String Value;
            const String DefaultValue;
        // . . . Private methods
            static inline String ToString(const String& s) {if (mat::HasNonVisibleChars(s)) return mat::ToQuotedString(s); else return s;}
            static inline String FromString(const String& s) {if (mat::IsQuotedString(s)) return mat::FromQuotedString(s); else return s;}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionBool : public cls_OptionValue {// Boolean values
	public:
        // . . . Public methods
		cls_OptionBool (const String& g ="",const String& l ="",const String& c ="",bool d =true)
                : cls_OptionValue("bool",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
	        // Assegnamento e cast verso `String'
	        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);return *this;}
	        virtual operator String() const {return ToString(Value);}
	        // Assegnamento e cast verso `bool'
	        cls_OptionValue& operator= (const bool& b) {Value=b;return *this;}
	        operator bool() const {return Value;}
	private:
	// . . . Private attributes
            bool Value;
            const bool DefaultValue;
	// . . . Private methods
            static inline String ToString(bool b) {return mat::BoolToString(b);}
            static inline bool FromString(const String& s) {return mat::StringToBool(s);}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionChar : public cls_OptionValue {// Character values
	public:
        // . . . Public methods
		cls_OptionChar (const String& g ="",const String& l ="",const String& c ="",char d =0)
                : cls_OptionValue("char",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
	        // Assegnamento e cast verso `String'
	        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s); return *this;}
	        virtual operator String() const {return ToString(Value);}
	        // Assegnamento e cast verso `char'
	        cls_OptionValue& operator= (const char& c) {Value=c;return *this;}
	        operator char() const {return Value;}

	private:
	// . . . Private attributes
            char Value;
            const char DefaultValue;
	// . . . Private methods
            static inline String ToString(char c) {return (c>=0 && c<=20) ? String("0x") + mat::ByteToHex(c) : String(c);}
            static inline char FromString(const String& s) {if (s.Length()==1) return s[1]; else return s.ToInt(); }
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionWord : public cls_OptionValue {// Word values
	public:
        // . . . Public methods
		cls_OptionWord (const String& g ="",const String& l ="",const String& c ="",U16 d =0)
                : cls_OptionValue("word",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
	        // Assegnamento e cast verso `String'
	        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s); return *this;}
	        virtual operator String() const {return ToString(Value);}
	        // Assegnamento e cast verso `int'
	        cls_OptionValue& operator= (const U16& w) {Value=w; return *this;}
	        operator U16() const {return Value;}

	private:
	// . . . Private attributes
            U16 Value;
            const U16 DefaultValue;
	// . . . Private methods
            static inline String ToString(U16 w) {return mat::HexPrefix + ::IntToHex(w,4);}
            static inline U16 FromString(const String& s) {return s.ToInt();}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionInt : public cls_OptionValue {// Integer values
	public:
        // . . . Public methods
		cls_OptionInt (const String& g ="",const String& l ="",const String& c ="",int d =0)
                : cls_OptionValue("int",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
	        // Assegnamento e cast verso `String'
	        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);return *this;}
	        virtual operator String() const {return ToString(Value);}
	        // Assegnamento e cast verso `int'
	        cls_OptionValue& operator= (const int& i) {Value=i;return *this;}
	        operator int() const {return Value;}

	private:
	// . . . Private attributes
            int Value;
            const int DefaultValue;
	// . . . Private methods
            static inline String ToString(int i) {return String(i);}
            static inline int FromString(const String& s) {return s.ToInt();}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionDouble : public cls_OptionValue {// Floating point values
	public:
        // . . . Public methods
		cls_OptionDouble (const String& g ="",const String& l ="",const String& c ="",double d =0)
                : cls_OptionValue("double",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
	        // Assegnamento e cast verso `String'
	        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);return *this;}
	        virtual operator String() const {return ToString(Value);}
	        // Assegnamento e cast verso `double'
	        cls_OptionValue& operator= (const double& d) {Value=d;return *this;}
	        operator double() const {return Value;}

	private:
        // . . . Private attributes
            double Value;
            const double DefaultValue;
	// . . . Private methods
            static inline String ToString(double d) {return String(d);}
            static inline double FromString(const String& s) {return s.ToDouble();}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionTShortCut : public cls_OptionValue {// TShortCut values
	public:
        // . . . Public methods
		cls_OptionTShortCut (const String& g ="",const String& l ="",const String& c ="",TShortCut d =0)
                : cls_OptionValue("TShortCut",l,g,c),Value(d),DefaultValue(d) {;}

            virtual void __fastcall SetDefault() {Value=DefaultValue;}
            virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
            // Assegnamento e cast verso `String'
            virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);::ShortCutToKey(Value,i_Key,i_Shift);return *this;}
            virtual operator String() const {return ToString(Value);}
            // Assegnamento e cast verso `TShortCut'
            cls_OptionValue& operator= (const TShortCut& k) {Value=k;::ShortCutToKey(Value,i_Key,i_Shift);return *this;}
            operator TShortCut() const {return Value;}
            // Altri cast
            //operator TShiftState() const {return Shift;}

        // . . . Properties
            __property Word Key = { read=get_Key };
            __property TShiftState& Shift = { read=get_Shift };

	private:
	// . . . Private attributes
            TShortCut Value;
            const TShortCut DefaultValue;
	// . . . Private methods
            static inline String ToString(TShortCut k) {return ::ShortCutToText(k);}
            static inline TShortCut FromString(const String& s) {return ::TextToShortCut(s);}
        // . . . Properties
            Word i_Key; TShiftState i_Shift;
            Word __fastcall get_Key() const {return i_Key;} // Get Key only
            TShiftState __fastcall get_Shift() const {return i_Shift;} // Get Shift state only

	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionTColor : public cls_OptionValue {// TColor values
	public:
        // . . . Public methods
		cls_OptionTColor (const String& g ="",const String& l ="",const String& c ="",TColor d =TColor(0))
                : cls_OptionValue("TColor",l,g,c),Value(d),DefaultValue(d) {;}

        virtual void __fastcall SetDefault() {Value=DefaultValue;}
        virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
        // Assegnamento e cast verso `String'
        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);return *this;}
        virtual operator String() const {return ToString(Value);}
        // Assegnamento e cast verso `TColor'
        cls_OptionValue& operator= (const TColor& c) {Value=c;return *this;}
        operator TColor() const {return Value;}

	private:
	// . . . Private attributes
        TColor Value;
        const TColor DefaultValue;
	// . . . Private methods
        static inline String ToString(TColor c) {return mat::TColorToRGBStr(c);}
        static inline TColor FromString(const String& s) {try{return mat::RGBStrToTColor(s);} catch(EConvertError&){return TColor(s.ToInt());}}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionTPenStyle : public cls_OptionValue {// TPenStyle values
	public:
        // . . . Public methods
		cls_OptionTPenStyle (const String& g ="",const String& l ="",const String& c ="",TPenStyle d =psSolid)
                : cls_OptionValue("TPenStyle",l,g,c),Value(d),DefaultValue(d) {;}

        virtual void __fastcall SetDefault() {Value=DefaultValue;}
        virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
        // Assegnamento e cast verso `String'
        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);return *this;}
        virtual operator String() const {return ToString(Value);}
        // Assegnamento e cast verso `TPenStyle'
        cls_OptionValue& operator= (const TPenStyle& p) {Value=p;return *this;}
        operator TPenStyle() const {return Value;}

	private:
	// . . . Private attributes
        TPenStyle Value;
        const TPenStyle DefaultValue;
	// . . . Private methods
        static inline String ToString(TPenStyle p) {return mat::PenStyleToString(p);}
        static inline TPenStyle FromString(const String& s) {return mat::StringToPenStyle(s);}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionTBrushStyle : public cls_OptionValue {// TBrushStyle values
	public:
        // . . . Public methods
		cls_OptionTBrushStyle (const String& g ="",const String& l ="",const String& c ="",TBrushStyle d =bsSolid)
                : cls_OptionValue("TBrushStyle",l,g,c),Value(d),DefaultValue(d) {;}

        virtual void __fastcall SetDefault() {Value=DefaultValue;}
        virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
        // Assegnamento e cast verso `String'
        virtual cls_OptionValue& operator= (const String& s) {Value=FromString(s);return *this;}
        virtual operator String() const {return ToString(Value);}
        // Assegnamento e cast verso `TBrushStyle'
        cls_OptionValue& operator= (const TBrushStyle& p) {Value=p;return *this;}
        operator TBrushStyle() const {return Value;}

	private:
	// . . . Private attributes
        TBrushStyle Value;
        const TBrushStyle DefaultValue;
	// . . . Private methods
        static inline String ToString(TBrushStyle p) {return mat::BrushStyleToString(p);}
        static inline TBrushStyle FromString(const String& s) {return mat::StringToBrushStyle(s);}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionNumlist : public cls_OptionValue {// 'mat::numlist' values
	public:
        // . . . Public methods
		cls_OptionNumlist (const String& g ="",const String& l ="",const String& c ="",const mat::numlist& d =mat::numlist(1))
                : cls_OptionValue("mat::numlist",l,g,c),Value(d),DefaultValue(d) {;}

        virtual void __fastcall SetDefault() {Value=DefaultValue;}
        virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
        // Assegnamento e cast verso `String'
        virtual cls_OptionValue& operator= (const String& s) {Value.clear(); mat::StringToNumlist(Value,s); return *this;}
        virtual operator String() const {return ToString(Value);}
        // Assegnamento e cast verso `mat::numlist'
        cls_OptionValue& operator= (const mat::numlist& p) {Value=p;return *this;}
        operator mat::numlist() const {return Value;}

	private:
	// . . . Private attributes
        mat::numlist Value;
        const mat::numlist DefaultValue;
	// . . . Private methods
        static inline String ToString(const mat::numlist& p) {return mat::NumlistToString(p);}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionStringIntIntList : public cls_OptionValue {// A structure
	public:
        // . . . Public methods
		cls_OptionStringIntIntList (const String& g ="",const String& l ="",const String& c ="",const mat::cls_StringIntIntList& d =mat::cls_StringIntIntList(""))
                : cls_OptionValue("mat::cls_StringIntIntList",l,g,c),Value(d),DefaultValue(d) {;}

        virtual void __fastcall SetDefault() {Value=DefaultValue;}
        virtual String __fastcall GetDefault() {return ToString(DefaultValue);}

        // . . . Operators
        // Assegnamento e cast verso `String'
        virtual cls_OptionValue& operator= (const String& s) {Value=s; return *this;}
        virtual operator String() const {return ToString(Value);}
        // Assegnamento e cast verso `mat::cls_StringIntIntList'
        cls_OptionValue& operator= (const mat::cls_StringIntIntList& p) {Value=p; return *this;}
        operator mat::cls_StringIntIntList() const {return Value;}

	private:
	// . . . Private attributes
        mat::cls_StringIntIntList Value;
        const mat::cls_StringIntIntList DefaultValue;
	// . . . Private methods
        static inline String ToString(const mat::cls_StringIntIntList& p) {return p.ToString();}
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/*---------------------------------------------------------------------------
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	class cls_OptionTFont : public cls_OptionValue {// TFont values
	public:
        // . . . Public methods
		cls_OptionTFont (const String& g ="",const String& l ="",const String& c ="",String d ="")
                : cls_OptionValue("TFont",l,g,c),DefaultValue(d) {SetDefault();}

        virtual void __fastcall SetDefault() { Font = DefaultValue; }
        virtual String __fastcall GetDefault() {return DefaultValue;}

        // . . . Operators
        // Assegnamento e cast verso `String'
        virtual cls_OptionValue& operator= (const String& s) {Font=s; return *this;}
        virtual operator String() const {return String(Font);}
        // Assegnamento e cast verso `TFont*'
        cls_OptionValue& operator= (TFont* f) {Font=f; return *this;}
        operator TFont*() const {return Font;}

	private:
	// . . . Private attributes
        mat::cls_Font Font;
        const String DefaultValue;
	// . . . Private methods
	};//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


//---------------------------------------------------------------------------
#endif
