/******************************************************************************
BigWorld Technology 
Copyright BigWorld Pty, Ltd.
All Rights Reserved. Commercial in confidence.

WARNING: This computer program is protected by copyright law and international
treaties. Unauthorized use, reproduction or distribution of this program, or
any portion of this program, may result in the imposition of civil and
criminal penalties as provided by law.
******************************************************************************/

// text_gui_component.ipp

#ifdef CODE_INLINE
#define INLINE inline
#else
#define INLINE
#endif


INLINE void
TextGUIComponent::slimLabel( const std::string& l )
{
	BW_GUARD;
	//font can't handle more than 256 sets of indices, so truncate this string.
	std::string label = l.substr(0,255);
	static wchar_t buf[256];
	bw_snwprintf( buf, sizeof(buf)/sizeof(wchar_t), L"%S\0", label.c_str() );
	this->label( buf );
}


INLINE void
TextGUIComponent::label( const std::wstring& l )
{
	BW_GUARD;
	label_ = l;
	dirty_ = true;
}


INLINE const std::wstring&
TextGUIComponent::label( void )
{
	return label_;
}

INLINE void
TextGUIComponent::multiline( bool b )
{
	multiline_ = b;
	dirty_ = true;
}

INLINE bool 
TextGUIComponent::multiline()
{
	return multiline_;
}

INLINE void
TextGUIComponent::colourFormatting( bool b )
{
	colourFormatting_ = b;
	dirty_ = true;
}

INLINE bool 
TextGUIComponent::colourFormatting()
{
	return colourFormatting_;
}


INLINE void	TextGUIComponent::textureName( const std::string& name )
{
	//can't set texture name - the font has the texture name.
	//stub the fn out.
}


INLINE uint TextGUIComponent::stringWidth( const std::wstring& theString )
{
	BW_GUARD;
	return font_ ? font_->metrics().stringWidth( theString, multiline_, colourFormatting_ ) : 0;
}

INLINE PyObject* TextGUIComponent::stringDimensions( const std::wstring& theString )
{
	BW_GUARD;
	int w=0;
	int h=0;
	
	if ( font_ )
	{
		font_->metrics().stringDimensions( theString, w, h, multiline_, colourFormatting_ );
	}
	
	PyObject* r = PyTuple_New(2);
	PyTuple_SetItem( r, 0, PyInt_FromLong(w) );
	PyTuple_SetItem( r, 1, PyInt_FromLong(h) );
	return r;
}


// text_gui_component.ipp
