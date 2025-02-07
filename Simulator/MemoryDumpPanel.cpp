/*===============================================================================
wxCesar

Simulator for the abstract computer 'Cesar'
<https://www.inf.ufrgs.br/arq/wiki/doku.php?id=cesar>

This simulator created by Isaac Marino Bavaresco.
Written using Code::Blocks 20.03 and
wxFormBuilder v4.2.1 using the library wxWidgets v3.2.5
===============================================================================*/
/*===============================================================================
Copyright (c) 2024-2025, Isaac Marino Bavaresco
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Neither the name of the author nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.;;

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
==============================================================================*/
//==============================================================================
#include <ctype.h>
#include "MemoryDumpPanel.h"
#include "wxCesarThread.h"
#include "wxCesarApp.h"
//==============================================================================
wxDECLARE_APP( wxCesarApp );
//==============================================================================
#define	ROW_HEIGHT	16
//==============================================================================
class	HexEdit: public wxTextCtrl
	{
	//--------------------------------------------------------------------------
	public:
	//--------------------------------------------------------------------------
							HexEdit				( unsigned line, unsigned column,
												  wxWindow *parent, wxWindowID id,
												  const wxString& value = wxEmptyString,
												  const wxPoint& pos = wxDefaultPosition,
												  const wxSize& size = wxDefaultSize,
												  long style = 0,
												  const wxValidator& validator = wxDefaultValidator,
												  const wxString& name = wxASCII_STR( wxTextCtrlNameStr ))
												: wxTextCtrl( parent, id, value, pos, size, style, validator, name ), Line( line ), Column( column ) {};

		virtual				~HexEdit			() {};
	//--------------------------------------------------------------------------
		unsigned			GetLine				() { return Line; };
		unsigned			GetColumn			() { return Column; };
	//--------------------------------------------------------------------------
	protected:
		unsigned			Line, Column;
	//--------------------------------------------------------------------------
	};
//==============================================================================
class	CharEdit: public wxTextCtrl
	{
	//--------------------------------------------------------------------------
	public:
	//--------------------------------------------------------------------------
							CharEdit			( unsigned line, unsigned column,
												  wxWindow *parent, wxWindowID id,
												  const wxString& value = wxEmptyString,
												  const wxPoint& pos = wxDefaultPosition,
												  const wxSize& size = wxDefaultSize,
												  long style = 0,
												  const wxValidator& validator = wxDefaultValidator,
												  const wxString& name = wxASCII_STR( wxTextCtrlNameStr ))
												: wxTextCtrl( parent, id, value, pos, size, style, validator, name ), Line( line ), Column( column ) {};

		virtual				~CharEdit			() {};
	//--------------------------------------------------------------------------
		unsigned			GetLine				() { return Line; };
		unsigned			GetColumn			() { return Column; };
	//--------------------------------------------------------------------------
	protected:
		unsigned			Line, Column;
	//--------------------------------------------------------------------------
	};
//==============================================================================
MemoryDumpPanel::MemoryDumpPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxPanel( parent, id, pos, size, style, name ), m_FocusOwner( nullptr )
	{
	this->SetBackgroundColour( *wxWHITE );
	wxString	Text;

	for( int i = 0; i < 256; i++ )
		{
		m_HexBitmaps[i]	= new wxBitmap( 16, 16 );
		wxMemoryDC	DC( *m_HexBitmaps[i] );

		DC.SetBackground( *wxWHITE_BRUSH );
		DC.SetFont( wxFont( -1, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT( "Courier New" )));
		DC.Clear();
		Text.Printf( "%02X", i );
		DC.DrawText( Text, 4, 0 );
		}

	wxBoxSizer		*bSizer1;
	bSizer1			= new wxBoxSizer( wxHORIZONTAL );

	m_panel1		= new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 /*wxTAB_TRAVERSAL*/ );

	wxFlexGridSizer	*fgSizer1;

	fgSizer1 = new wxFlexGridSizer( 0, 33, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	for( int Line = 0; Line < MEMORY_DUMP_LINES; Line++ )
		{
		wxString	Address;
		Address.Printf( "%04X:", Line * MEMORY_DUMP_COLUMNS );
		m_Addresses[Line]	= new wxStaticText( m_panel1, wxID_ANY, Address, wxDefaultPosition, wxSize( 32, ROW_HEIGHT ), wxTAB_TRAVERSAL );
		m_Addresses[Line]->Wrap( -1 );
		m_Addresses[Line]->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT( "Courier New" )));
		fgSizer1->Add( m_Addresses[Line], 0, /*( Line == 0 ? wxTOP : 0 ) | ( Line == MEMORY_DUMP_LINES - 1 ? wxBOTTOM : 0 ) |*/ wxRIGHT | wxLEFT | wxALIGN_CENTER_VERTICAL, 5 );

		for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++ )
			{
			m_CellsHex[Line][Column]	= new HexEdit( Line, Column, m_panel1, wxID_ANY, _( "00" ), wxDefaultPosition, wxSize( 18, ROW_HEIGHT ), wxTE_PROCESS_ENTER | wxTE_RIGHT | wxBORDER_NONE );
#ifdef __WXGTK__
			if( !m_CellsHex[Line][Column]->HasFlag( wxTE_MULTILINE ))
				m_CellsHex[Line][Column]->SetMaxLength( 2 );
#else
			m_CellsHex[Line][Column]->SetMaxLength( 2 );
#endif
			m_CellsHex[Line][Column]->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT( "Courier New" )));

			fgSizer1->Add( m_CellsHex[Line][Column], 0, wxALIGN_CENTER_VERTICAL | /*( Column == 0 ? wxLEFT : 0 ) | ( Column == MEMORY_DUMP_COLUMNS - 1 ? wxRIGHT : 0 ) | ( Line == 0 ? wxTOP : 0 ) | ( Line == MEMORY_DUMP_LINES - 1 ? wxBOTTOM : 0 ) |*/ wxALIGN_CENTER_HORIZONTAL, 5 );
			}

		for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++ )
			{
			m_CellsChar[Line][Column]	= new CharEdit( Line, Column, m_panel1, wxID_ANY, _( "." ), wxDefaultPosition, wxSize( 8, ROW_HEIGHT ), wxTE_PROCESS_ENTER | wxTE_RIGHT | wxBORDER_NONE );
#ifdef __WXGTK__
			if( !m_CellsChar[Line][Column]->HasFlag( wxTE_MULTILINE ))
				m_CellsChar[Line][Column]->SetMaxLength( 1 );
#else
			m_CellsChar[Line][Column]->SetMaxLength( 1 );
#endif
			m_CellsChar[Line][Column]->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

			fgSizer1->Add( m_CellsChar[Line][Column], 0, ( Column == 0 ? wxLEFT : 0 ) | ( Column == MEMORY_DUMP_COLUMNS - 1 ? wxRIGHT : 0 ) /*| ( Line == 0 ? wxTOP : 0 ) | ( Line == MEMORY_DUMP_LINES - 1 ? wxBOTTOM : 0 )*/, 5 );
			}
		}

	m_panel1->SetSizer( fgSizer1 );
	m_panel1->Layout();
	fgSizer1->Fit( m_panel1 );
	bSizer1->Add( m_panel1, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );

	m_scrollBar1 = new wxScrollBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	m_scrollBar1->SetPageSize( 8 );
	m_scrollBar1->SetThumbSize( 9 );
	m_scrollBar1->SetRange( 4092 );

	bSizer1->Add( m_scrollBar1, 0, wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();

	this->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MemoryDumpPanel::OnPanelSetFocus ));
	m_scrollBar1->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( MemoryDumpPanel::OnScrollChanged ), NULL, this );

	for( int Line = 0; Line < MEMORY_DUMP_LINES; Line++ )
		for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++ )
			{
			m_CellsHex[Line][Column]->Connect( wxEVT_CHAR, wxKeyEventHandler( MemoryDumpPanel::OnCharHex ), NULL, this );
			m_CellsHex[Line][Column]->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MemoryDumpPanel::OnKillFocusHex ), NULL, this );
			m_CellsHex[Line][Column]->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MemoryDumpPanel::OnLeftDownHex ), NULL, this );
			m_CellsHex[Line][Column]->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MemoryDumpPanel::OnSetFocusHex ), NULL, this );
			m_CellsHex[Line][Column]->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MemoryDumpPanel::OnTextEnterHex ), NULL, this );
			m_CellsHex[Line][Column]->Connect( wxEVT_COMMAND_TEXT_MAXLEN, wxCommandEventHandler( MemoryDumpPanel::OnTextMaxLenHex ), NULL, this );

			m_CellsHex[Line][Column]->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MemoryDumpPanel::DummyMouseEvent ), NULL, this );
			m_CellsHex[Line][Column]->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MemoryDumpPanel::DummyMouseEvent ), NULL, this );

			m_CellsChar[Line][Column]->Connect( wxEVT_CHAR, wxKeyEventHandler( MemoryDumpPanel::OnCharChar ), NULL, this );
			m_CellsChar[Line][Column]->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MemoryDumpPanel::OnKillFocusChar ), NULL, this );
			m_CellsChar[Line][Column]->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MemoryDumpPanel::OnLeftDownChar ), NULL, this );
			m_CellsChar[Line][Column]->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MemoryDumpPanel::OnSetFocusChar ), NULL, this );
			m_CellsChar[Line][Column]->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MemoryDumpPanel::OnTextEnterChar ), NULL, this );
			m_CellsChar[Line][Column]->Connect( wxEVT_COMMAND_TEXT_MAXLEN, wxCommandEventHandler( MemoryDumpPanel::OnTextMaxLenChar ), NULL, this );

			m_CellsChar[Line][Column]->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MemoryDumpPanel::DummyMouseEvent ), NULL, this );
			m_CellsChar[Line][Column]->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MemoryDumpPanel::DummyMouseEvent ), NULL, this );
			}

	m_Editable	= true;
	}
//==============================================================================
MemoryDumpPanel::~MemoryDumpPanel()
	{
	}
//==============================================================================
void MemoryDumpPanel::DumpRedraw()
	{
	if( m_Buffer == nullptr || m_BufferLength == 0 )
		return;

	uint16_t	Address	= m_StartAddress;

	for( int Line = 0; Line < MEMORY_DUMP_LINES; Line++ )
		{
		wxString	Text;

		Text.Printf( "%04X:", m_StartAddress + Line * MEMORY_DUMP_COLUMNS );
		m_Addresses[Line]->SetLabel( Text );
//		m_Addresses[Line]->Refresh();
		for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++, Address++ )
			{
			uint8_t	Value	= m_Buffer[m_StartAddress + Line * MEMORY_DUMP_COLUMNS + Column];
			Text.Printf( "%02X", Value );
			m_CellsHex[Line][Column]->SetValue( Text );

			if( m_Changed != nullptr && BitVectorQueryBit( m_Changed, Address ) != 0 )
				m_CellsHex[Line][Column]->SetBackgroundColour( *wxRED );
			else if( m_Written != nullptr && BitVectorQueryBit( m_Written, Address ) != 0 )
				m_CellsHex[Line][Column]->SetBackgroundColour( *wxCYAN );
			else
				m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
			m_CellsHex[Line][Column]->Refresh();

//			m_CellsHex[Line][Column]->Refresh();
			Text.Printf( "%c", isprint( Value ) ? Value : '.' );
			m_CellsChar[Line][Column]->SetValue( Text );
//			m_CellsChar[Line][Column]->Refresh();
			if( m_Changed != nullptr && BitVectorQueryBit( m_Changed, Address ) != 0 )
				m_CellsChar[Line][Column]->SetBackgroundColour( *wxRED );
			else if( m_Written != nullptr && BitVectorQueryBit( m_Written, Address ) != 0 )
				m_CellsChar[Line][Column]->SetBackgroundColour( *wxCYAN );
			else
				m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
			m_CellsChar[Line][Column]->Refresh();
			}
		}
	}
//==============================================================================
void MemoryDumpPanel::SetBuffer( uint8_t *buffer, uint16_t bufferlength )
	{
	if( buffer == nullptr || bufferlength == 0 )
		{
		m_Buffer		= nullptr;
		m_BufferLength	= 0;
		return;
		}

	m_Buffer		= buffer;
	m_BufferLength	= bufferlength;
	m_StartAddress	= 0;
	wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );

	DumpRedraw();
	}
//==============================================================================
void MemoryDumpPanel::SetEditable( bool editable )
	{
	m_Editable	= editable;
/*
	if( editable )
		{
		this->SetBackgroundColour( *wxWHITE );
		m_panel1->SetBackgroundColour( *wxWHITE );
		}
	else
		{
		this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
		m_panel1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
		}
*/
	this->Refresh();
	m_panel1->Refresh();

	for( int Line = 0; Line < MEMORY_DUMP_LINES; Line++ )
		{
		for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++ )
			{
			m_CellsHex[Line][Column]->SetEditable( editable );
			m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
			m_CellsChar[Line][Column]->SetEditable( editable );
			m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
			}
/*
		if( editable )
			{
			m_Addresses[Line]->SetBackgroundColour( *wxWHITE );
			for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++ )
				{
				m_CellsHex[Line][Column]->SetEditable( editable );
				m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
				m_CellsChar[Line][Column]->SetEditable( editable );
				m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
				}
			}
		else
			{
			m_Addresses[Line]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
			for( int Column = 0; Column < MEMORY_DUMP_COLUMNS; Column++ )
				{
				m_CellsHex[Line][Column]->SetEditable( editable );
				m_CellsHex[Line][Column]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
				m_CellsChar[Line][Column]->SetEditable( editable );
				m_CellsChar[Line][Column]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
				}
			}
*/
		}
	}
//==============================================================================
void MemoryDumpPanel::OnPanelSetFocus( wxFocusEvent &event )
	{
	if( m_FocusOwner != nullptr )
		m_FocusOwner->SetFocus();
	}
//==============================================================================
void MemoryDumpPanel::MoveUpHex( unsigned line, unsigned column )
	{
	if( line > 0 )
		m_CellsHex[line-1][column]->SetFocus();
	else if( m_StartAddress > 0 )
		{
		m_CellsHex[line][column]->SetFocus();
		m_StartAddress -= MEMORY_DUMP_COLUMNS;
		wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
		DumpRedraw();
		}
	}
//==============================================================================
void MemoryDumpPanel::MoveDownHex( unsigned line, unsigned column )
	{
	if( line < MEMORY_DUMP_LINES - 1 )
		m_CellsHex[line+1][column]->SetFocus();
	else if( m_StartAddress < m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS )
		{
		m_StartAddress += MEMORY_DUMP_COLUMNS;
		wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
		DumpRedraw();
		}
	}
//==============================================================================
void MemoryDumpPanel::MoveLeftHex( unsigned line, unsigned column )
	{
	if( column > 0 )
		m_CellsHex[line][column-1]->SetFocus();
	else
		MoveUpHex( line, MEMORY_DUMP_COLUMNS - 1 );
	}
//==============================================================================
void MemoryDumpPanel::MoveRightHex( unsigned line, unsigned column )
	{
	if( column < MEMORY_DUMP_COLUMNS - 1 )
		m_CellsHex[line][column+1]->SetFocus();
	else
		MoveDownHex( line, 0 );
	}
//==============================================================================
void MemoryDumpPanel::OnScrollChanged( wxScrollEvent& event )
	{
	int	Position	= event.GetPosition();
	m_StartAddress	= Position * MEMORY_DUMP_COLUMNS;
	wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
	DumpRedraw();
	}
//==============================================================================
void MemoryDumpPanel::OnCharHex( wxKeyEvent& event )
	{
	wxString	Text;

	int			KeyCode		= event.GetKeyCode();

	HexEdit		*Control	= static_cast<HexEdit*>( event.GetEventObject() );
	unsigned	Line		= Control->GetLine();
	unsigned	Column		= Control->GetColumn();

	if( !isprint( KeyCode ))
		{
		int			Modifiers	= event.GetModifiers();

		switch( KeyCode )
			{
			case WXK_PAGEUP:
				if( m_StartAddress > ( MEMORY_DUMP_LINES - Line - 1 ) * MEMORY_DUMP_COLUMNS )
					m_StartAddress -= ( MEMORY_DUMP_LINES - Line - 1 ) * MEMORY_DUMP_COLUMNS;
				else
					m_StartAddress	= 0;
				wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
				m_CellsHex[0][Column]->SetFocus();
				DumpRedraw();
				return;
			case WXK_PAGEDOWN:
				if( m_StartAddress < m_BufferLength - ( MEMORY_DUMP_LINES + Line ) * MEMORY_DUMP_COLUMNS )
					m_StartAddress += Line * MEMORY_DUMP_COLUMNS;
				else
					m_StartAddress	= m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS;
				wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
				m_CellsHex[MEMORY_DUMP_LINES-1][Column]->SetFocus();
				DumpRedraw();
				return;
			case WXK_HOME:
				if( event.GetModifiers() & wxMOD_CONTROL )
					{
					if( m_StartAddress > 0 )
						{
						m_StartAddress	= 0;
						wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
						}
					m_CellsHex[0][0]->SetFocus();
					DumpRedraw();
					}
				else
					m_CellsHex[Line][0]->SetFocus();
				return;
			case WXK_END:
				if( event.GetModifiers() & wxMOD_CONTROL )
					{
					if( m_StartAddress < m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS )
						{
						m_StartAddress	= m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS;
						wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
						}
					m_CellsHex[MEMORY_DUMP_LINES-1][MEMORY_DUMP_COLUMNS-1]->SetFocus();
					DumpRedraw();
					}
				else
					m_CellsHex[Line][MEMORY_DUMP_COLUMNS-1]->SetFocus();
				return;
			case WXK_DELETE:
				return;
			case WXK_BACK:
				if( m_Editable && Control->GetInsertionPoint() == 1 )
					{
					Text	= "  ";
					Control->SetValue( Text );
					Control->SetInsertionPoint( 0 );
					}
				return;
			case WXK_ESCAPE:
				Text.Printf( "%02X", m_Buffer != nullptr ? m_Buffer[m_StartAddress+Line*MEMORY_DUMP_COLUMNS+Column] : 0 );
				Control->SetValue( Text );
				//Control->SetSelection( -1, -1 );
				return;
			case WXK_UP:
				MoveUpHex( Line, Column );
				return;
			case WXK_DOWN:
				MoveDownHex( Line, Column );
				return;
			case WXK_LEFT:
				MoveLeftHex( Line, Column );
				return;
			case WXK_RIGHT:
				MoveRightHex( Line, Column );
				return;
			case WXK_TAB:
				if( !( Modifiers & wxMOD_SHIFT ))
					{
					m_CellsChar[Line][Column]->SetFocus();
					return;
					}
				m_FocusOwner	= Control;
				m_CellsHex[0][0]->SetFocus();
				m_CellsHex[0][0]->Navigate( wxNavigationKeyEvent::IsBackward );
				return;
			case WXK_RETURN:
			default:
				event.Skip();
				return;
			}
		return;
		}

	if( !isxdigit( KeyCode ) || !m_Editable )
		return;

	Text					= Control->GetValue();

	if( Text[1] != ' ' || Control->GetInsertionPoint() == 0 )
		{
		Text[0]	= (char)toupper( KeyCode );
		Text[1]	= ' ';
		Control->SetValue( Text );
		Control->SetInsertionPoint( 1 );
		}
	else
		{
		Text[1]	= (char)toupper( KeyCode );
		Control->SetValue( Text );
		uint8_t	Value;
		sscanf( Text, "%hhx", &Value );
		m_Buffer != nullptr && ( m_Buffer[m_StartAddress + Line * MEMORY_DUMP_COLUMNS + Column] = Value );
		Text.Printf( "%c", isprint( Value ) ? Value : '.' );
		m_CellsChar[Line][Column]->SetValue( Text );
		MoveRightHex( Line, Column );
		}

	return;
	}
//==============================================================================
void MemoryDumpPanel::OnKillFocusHex( wxFocusEvent& event )
	{
	HexEdit		*Control	= static_cast<HexEdit*>( event.GetEventObject() );
	unsigned	Line		= Control->GetLine();
	unsigned	Column		= Control->GetColumn();

	wxString	Text;
	uint8_t		Value		= m_Buffer != nullptr ? m_Buffer[m_StartAddress + Line * MEMORY_DUMP_COLUMNS + Column] : 0;

/*
	if( m_Editable )
		m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
	else
		m_CellsHex[Line][Column]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
*/

	m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
	m_CellsHex[Line][Column]->SetForegroundColour( *wxBLACK );
	Text.Printf( "%02X", Value );
	m_CellsHex[Line][Column]->SetValue( Text );
	m_CellsHex[Line][Column]->Refresh();
/*
	if( m_Editable )
		m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
	else
		m_CellsChar[Line][Column]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
*/
	m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
	m_CellsChar[Line][Column]->SetForegroundColour( *wxBLACK );
	Text.Printf( "%c", isprint( Value ) ? Value : '.' );
	m_CellsChar[Line][Column]->SetValue( Text );
	m_CellsChar[Line][Column]->Refresh();

	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::OnLeftDownHex( wxMouseEvent& event )
	{
	HexEdit		*Control	= static_cast<HexEdit*>( event.GetEventObject() );
	Control->SetFocus();
	}
//==============================================================================
void MemoryDumpPanel::OnSetFocusHex( wxFocusEvent& event )
	{
	HexEdit		*Control	= static_cast<HexEdit*>( event.GetEventObject() );
	unsigned	Line		= Control->GetLine();
	unsigned	Column		= Control->GetColumn();

	m_CellsHex[Line][Column]->SetBackgroundColour( *wxBLUE );
	m_CellsHex[Line][Column]->SetForegroundColour( *wxWHITE );
//	m_CellsHex[Line][Column]->Refresh();

	m_CellsChar[Line][Column]->SetBackgroundColour( wxColour( 192, 192, 192 ));
	m_CellsChar[Line][Column]->SetForegroundColour( *wxBLACK );
	m_CellsChar[Line][Column]->Refresh();

	Control->SetInsertionPoint( 0 );

	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::OnTextEnterHex( wxCommandEvent& event )
	{
	HexEdit		*Control		= static_cast<HexEdit*>( event.GetEventObject() );
	unsigned	Column			= Control->GetColumn();
	unsigned	Line			= Control->GetLine();

	MoveRightHex( Line, Column );
	return;
	}
//==============================================================================
void MemoryDumpPanel::OnTextMaxLenHex( wxCommandEvent& event )
	{
	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::MoveUpChar( unsigned line, unsigned column )
	{
	if( line > 0 )
		m_CellsChar[line-1][column]->SetFocus();
	else if( m_StartAddress > 0 )
		{
		m_CellsChar[line][column]->SetFocus();
		m_StartAddress -= MEMORY_DUMP_COLUMNS;
		wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
		DumpRedraw();
		}
	}
//==============================================================================
void MemoryDumpPanel::MoveDownChar( unsigned line, unsigned column )
	{
	if( line < MEMORY_DUMP_LINES - 1 )
		m_CellsChar[line+1][column]->SetFocus();
	else if( m_StartAddress < m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS )
		{
		m_StartAddress += MEMORY_DUMP_COLUMNS;
		wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
		DumpRedraw();
		}
	}
//==============================================================================
void MemoryDumpPanel::MoveLeftChar( unsigned line, unsigned column )
	{
	if( column > 0 )
		m_CellsChar[line][column-1]->SetFocus();
	else
		MoveUpChar( line, MEMORY_DUMP_COLUMNS - 1 );
	}
//==============================================================================
void MemoryDumpPanel::MoveRightChar( unsigned line, unsigned column )
	{
	if( column < MEMORY_DUMP_COLUMNS - 1 )
		m_CellsChar[line][column+1]->SetFocus();
	else
		MoveDownChar( line, 0 );
	}
//==============================================================================
void MemoryDumpPanel::OnCharChar( wxKeyEvent& event )
	{
	int			KeyCode		= event.GetKeyCode();

	CharEdit	*Control	= static_cast<CharEdit*>( event.GetEventObject() );
	unsigned	Line		= Control->GetLine();
	unsigned	Column		= Control->GetColumn();

	if( !isprint( KeyCode ))
		{
		int			Modifiers	= event.GetModifiers();

		switch( KeyCode )
			{
			case WXK_PAGEUP:
				if( m_StartAddress > ( MEMORY_DUMP_LINES - Line - 1 ) * MEMORY_DUMP_COLUMNS )
					m_StartAddress -= ( MEMORY_DUMP_LINES - Line - 1 ) * MEMORY_DUMP_COLUMNS;
				else
					m_StartAddress	= 0;
				wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
				m_CellsChar[0][Column]->SetFocus();
				DumpRedraw();
				return;
			case WXK_PAGEDOWN:
				if( m_StartAddress < m_BufferLength - ( MEMORY_DUMP_LINES + Line ) * MEMORY_DUMP_COLUMNS )
					m_StartAddress += Line * MEMORY_DUMP_COLUMNS;
				else
					m_StartAddress	= m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS;
				wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
				m_CellsChar[MEMORY_DUMP_LINES-1][Column]->SetFocus();
				DumpRedraw();
				return;
			case WXK_HOME:
				if( event.GetModifiers() & wxMOD_CONTROL )
					{
					if( m_StartAddress > 0 )
						{
						m_StartAddress	= 0;
						wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
						}
					m_CellsChar[0][0]->SetFocus();
					DumpRedraw();
					}
				else
					m_CellsChar[Line][0]->SetFocus();
				return;
			case WXK_END:
				if( event.GetModifiers() & wxMOD_CONTROL )
					{
					if( m_StartAddress < m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS )
						{
						m_StartAddress	= m_BufferLength - MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS;
						wxGetApp().SimulatorThread->SetMemoryWindow( m_StartAddress, m_StartAddress + MEMORY_DUMP_LINES * MEMORY_DUMP_COLUMNS - 1 );
						}
					m_CellsChar[MEMORY_DUMP_LINES-1][MEMORY_DUMP_COLUMNS-1]->SetFocus();
					DumpRedraw();
					}
				else
					m_CellsChar[Line][MEMORY_DUMP_COLUMNS-1]->SetFocus();
				return;
			case WXK_UP:
				MoveUpChar( Line, Column );
				return;
			case WXK_DOWN:
				MoveDownChar( Line, Column );
				return;
			case WXK_LEFT:
				MoveLeftChar( Line, Column );
				return;
			case WXK_RIGHT:
				MoveRightChar( Line, Column );
				return;
			case WXK_TAB:
				if( Modifiers & wxMOD_SHIFT )
					{
					m_CellsHex[Line][Column]->SetFocus();
					return;
					}
				m_FocusOwner	= Control;
				m_CellsChar[MEMORY_DUMP_LINES-1][MEMORY_DUMP_COLUMNS-1]->SetFocus();
				m_CellsChar[MEMORY_DUMP_LINES-1][MEMORY_DUMP_COLUMNS-1]->Navigate( wxNavigationKeyEvent::IsForward );
				return;
			case WXK_RETURN:
				return;
			default:
				event.Skip();
				return;
			}
		return;
		}

	if( !isprint( KeyCode ) || !m_Editable )
		return;

	wxString	Text;
	Text.Printf( "%c", KeyCode );
	Control->SetValue( Text );
	Text.Printf( "%02X", KeyCode );
	m_Buffer != nullptr && ( m_Buffer[m_StartAddress + Line * MEMORY_DUMP_COLUMNS + Column] = (uint8_t)KeyCode );
	m_CellsHex[Line][Column]->SetValue( Text );
//	Control->Refresh();

	MoveRightChar( Line, Column );

	return;
	}
//==============================================================================
void MemoryDumpPanel::OnKillFocusChar( wxFocusEvent& event )
	{
	HexEdit		*Control	= static_cast<HexEdit*>( event.GetEventObject() );
	unsigned	Line		= Control->GetLine();
	unsigned	Column		= Control->GetColumn();

	wxString	Text;
	uint8_t		Value		= m_Buffer != nullptr ? m_Buffer[m_StartAddress + Line * MEMORY_DUMP_COLUMNS + Column] : 0;
/*
	if( m_Editable )
		m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
	else
		m_CellsHex[Line][Column]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
*/
	m_CellsHex[Line][Column]->SetBackgroundColour( *wxWHITE );
	m_CellsChar[Line][Column]->SetForegroundColour( *wxBLACK );
	Text.Printf( "%c", isprint( Value ) ? Value : '.' );
	m_CellsChar[Line][Column]->SetValue( Text );
	m_CellsChar[Line][Column]->Refresh();
/*
	if( m_Editable )
		m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
	else
		m_CellsChar[Line][Column]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
*/
	m_CellsChar[Line][Column]->SetBackgroundColour( *wxWHITE );
	m_CellsHex[Line][Column]->SetForegroundColour( *wxBLACK );
	Text.Printf( "%02X", Value );
	m_CellsHex[Line][Column]->SetValue( Text );
	m_CellsHex[Line][Column]->Refresh();

	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::OnLeftDownChar( wxMouseEvent& event )
	{
	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::OnSetFocusChar( wxFocusEvent& event )
	{
	CharEdit	*Control	= static_cast<CharEdit*>( event.GetEventObject() );
	unsigned	Line		= Control->GetLine();
	unsigned	Column		= Control->GetColumn();

	m_CellsChar[Line][Column]->SetBackgroundColour( *wxBLUE );
	m_CellsChar[Line][Column]->SetForegroundColour( *wxWHITE );
//	m_CellsChar[Line][Column]->Refresh();

	m_CellsHex[Line][Column]->SetBackgroundColour( wxColour( 192, 192, 192 ));
	m_CellsHex[Line][Column]->SetForegroundColour( *wxBLACK );
	m_CellsHex[Line][Column]->Refresh();

	Control->SetInsertionPoint( 0 );

	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::OnTextEnterChar( wxCommandEvent& event )
	{
	CharEdit	*Control		= static_cast<CharEdit*>( event.GetEventObject() );
	unsigned	Column			= Control->GetColumn();
	unsigned	Line			= Control->GetLine();

	MoveRightChar( Line, Column );
	return;
	}
//==============================================================================
void MemoryDumpPanel::OnTextMaxLenChar( wxCommandEvent& event )
	{
	event.Skip();
	}
//==============================================================================
void MemoryDumpPanel::SetAdress( unsigned line, uint16_t address )
	{
	if( address >= MEMORY_DUMP_LINES )
		return;

	wxString	Address;
	Address.Printf( "%04X:", address );

	m_Addresses[line]->SetLabel( Address );
	}
//==============================================================================
void MemoryDumpPanel::SetHexCellValue( unsigned line, unsigned column, uint8_t value )
	{
	if( line >= MEMORY_DUMP_LINES || column >= MEMORY_DUMP_COLUMNS )
		return;

	wxString	Value;
	Value.Printf( "%02X", value );

	m_CellsHex[line][column]->SetValue( Value );
	}
//==============================================================================
void MemoryDumpPanel::SetCharCellValue( unsigned line, unsigned column, char value )
	{
	if( line >= MEMORY_DUMP_LINES || column >= MEMORY_DUMP_COLUMNS )
		return;

	wxString	Value;
	Value.Printf( "%c", isprint( value ) ? value : '.' );

	m_CellsChar[line][column]->SetValue( Value );
	}
//==============================================================================
void MemoryDumpPanel::SetHexCellBackgroundColour( unsigned line, unsigned column, const wxColour &color )
	{
	if( line >= MEMORY_DUMP_LINES || column >= MEMORY_DUMP_COLUMNS )
		return;

	m_CellsChar[line][column]->SetBackgroundColour( color );
	}
//==============================================================================
void MemoryDumpPanel::SetCharCellBackgroundColour( unsigned line, unsigned column, const wxColour &color )
	{
	if( line >= MEMORY_DUMP_LINES || column >= MEMORY_DUMP_COLUMNS )
		return;

	m_CellsChar[line][column]->SetBackgroundColour( color );
	}
//==============================================================================
void MemoryDumpPanel::DummyMouseEvent( wxMouseEvent& event )
	{
	if( m_BreakPointsRead == nullptr && m_BreakPointsWrite == nullptr )
		return;

	CharEdit	*Control	= static_cast<CharEdit*>( event.GetEventObject() );
	unsigned	Column		= Control->GetColumn();
	unsigned	Line		= Control->GetLine();

	uint16_t	Address		= m_StartAddress + Line * MEMORY_DUMP_COLUMNS + Column;

	if( m_BreakPointsRead != nullptr )
		BitVectorToggleBit( m_BreakPointsRead, Address );
	if( m_BreakPointsWrite != nullptr )
		BitVectorToggleBit( m_BreakPointsWrite, Address );
	}
//==============================================================================
void MemoryDumpPanel::SetBitVectors( bitvector_t *breakpointsread, bitvector_t *breakpointswrite, bitvector_t *written, bitvector_t *changed )
	{
	m_BreakPointsRead	= breakpointsread;
	m_BreakPointsWrite	= breakpointswrite;
	m_Written			= written;
	m_Changed			= changed;
	}
//==============================================================================
