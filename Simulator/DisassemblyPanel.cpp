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
#include "DisassemblyPanel.h"
//==============================================================================
#include "Empty.xpm"
#include "Bkpt.xpm"
#include "Exec.xpm"
#include "Already.xpm"
#include "AlreadyC.xpm"
#include "AlreadyY.xpm"
//==============================================================================
class	DisassemblyBitmap : public wxStaticBitmap
	{
	//--------------------------------------------------------------------------
	public:
	//--------------------------------------------------------------------------
							DisassemblyBitmap	( unsigned				line,
												  wxWindow				*parent,
												  wxWindowID			id,
												  const wxBitmapBundle	&label,
												  const wxPoint			&pos	= wxDefaultPosition,
												  const wxSize			&size	= wxDefaultSize,
												  long					style	= 0,
												  const wxString		&name	= wxASCII_STR( wxStaticBitmapNameStr ))
												  : wxStaticBitmap( parent, id, label, pos, size, style, name ), Line( line ) {};
		virtual				~DisassemblyBitmap	() {};
	//--------------------------------------------------------------------------
		unsigned			GetLine				() { return Line; };
	//--------------------------------------------------------------------------
	protected:
	//--------------------------------------------------------------------------
		unsigned			Line;
	//--------------------------------------------------------------------------
	};
//==============================================================================
class	DisassemblyText : public wxStaticText
	{
	//--------------------------------------------------------------------------
	public:
	//--------------------------------------------------------------------------
							DisassemblyText		( unsigned			line,
												  wxWindow			*parent,
												  wxWindowID		id,
												  const wxString	&label,
												  const wxPoint		&pos	= wxDefaultPosition,
												  const wxSize		&size	= wxDefaultSize,
												  long				style	= 0,
												  const wxString	&name	= wxASCII_STR( wxStaticTextNameStr ))
												  : wxStaticText( parent, id, label, pos, size, style, name ), Line( line ) {};
		virtual				~DisassemblyText	() {};
	//--------------------------------------------------------------------------
		unsigned			GetLine				() { return Line; };
	//--------------------------------------------------------------------------
	protected:
	//--------------------------------------------------------------------------
		unsigned			Line;
	//--------------------------------------------------------------------------
	};
//==============================================================================
DisassemblyPanel::DisassemblyPanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name )
									: wxPanel( parent, id, pos, size, style, name ), m_BitVector( nullptr )
	{
	m_bitmaps[0]		= new wxBitmap( Empty_xpm );
	m_bitmaps[1]		= new wxBitmap( Already_xpm );
	m_bitmaps[2]		= new wxBitmap( AlreadyC_xpm );
	m_bitmaps[3]		= new wxBitmap( AlreadyY_xpm );
	m_bitmaps[4]		= new wxBitmap( Bkpt_xpm );
	m_bitmaps[5]		= new wxBitmap( Exec_xpm );

	wxFlexGridSizer	*fgSizer1;
	fgSizer1		= new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxHORIZONTAL );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	for( int Line = 0; Line < DISASSEMBLY_LINES; Line++ )
		{
		m_Lines[Line].Address	= -1;

		m_Lines[Line].Bitmap1	= new DisassemblyBitmap( Line, this, wxID_ANY, *m_bitmaps[0], wxDefaultPosition, wxSize( 13, 16 ), 0 );
//		m_Lines[Line].Bitmap1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ));
		fgSizer1->Add( m_Lines[Line].Bitmap1, 0, wxLEFT | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5 );

		m_Lines[Line].Bitmap2	= new DisassemblyBitmap( Line, this, wxID_ANY, Line == DISASSEMBLY_LINES / 2 ? *m_bitmaps[4] : *m_bitmaps[0], wxDefaultPosition, wxSize( 13, 16 ), 0 );
//		m_Lines[Line].Bitmap2->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ));
		fgSizer1->Add( m_Lines[Line].Bitmap2, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL, 5 );

		m_Lines[Line].Text	= new DisassemblyText( Line, this, wxID_ANY, _("0000: 00 00 00 00 00 00  MOV  R1,R2"), wxDefaultPosition, wxSize( -1, 16 ), 0 );
		m_Lines[Line].Text->Wrap( -1 );
		m_Lines[Line].Text->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT( "Courier New" )));

		fgSizer1->Add( m_Lines[Line].Text, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxRIGHT, 5 );
		}

	this->SetSizer( fgSizer1 );
	this->Layout();

	// Connect Events
	for( int Line = 0; Line < DISASSEMBLY_LINES; Line++ )
		{
		m_Lines[Line].Bitmap1->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DisassemblyPanel::OnLeftDownBkpt ), NULL, this );
		m_Lines[Line].Bitmap2->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DisassemblyPanel::OnLeftDownBkpt ), NULL, this );
		m_Lines[Line].Text->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( DisassemblyPanel::OnLeftDClick ), NULL, this );
		}
	}
//==============================================================================
DisassemblyPanel::~DisassemblyPanel()
	{
	}
//==============================================================================
void DisassemblyPanel::SetBitVector( bitvector_t *bitvector )
	{
	m_BitVector	= bitvector;
	}
//==============================================================================
void DisassemblyPanel::SetLine( size_t line, int address, unsigned icon1, unsigned icon2, const wxString &text )
	{
	if( line >= DISASSEMBLY_LINES )
		return;

	m_Lines[line].Text->SetLabel( text );
	m_Lines[line].Address	= address;

	SetIcon1( line, m_BitVector != nullptr && m_Lines[line].Address >= 0 && BitVectorQueryBit( m_BitVector, m_Lines[line].Address ) ? 4 : 0 );
	SetIcon2( line, icon2 );
	}
//==============================================================================
void DisassemblyPanel::SetIcon1( size_t line, unsigned icon )
	{
	if( line >= DISASSEMBLY_LINES || icon >= DISASSEMBLY_ICONS )
		return;

	m_Lines[line].Bitmap1->SetBitmap( *m_bitmaps[icon] );
	}
//==============================================================================
void DisassemblyPanel::SetIcon2( size_t line, unsigned icon )
	{
	if( line >= DISASSEMBLY_LINES || icon >= DISASSEMBLY_ICONS )
		return;

	m_Lines[line].Bitmap2->SetBitmap( *m_bitmaps[icon] );
	}
//==============================================================================
void DisassemblyPanel::ToggleBreakpoint( unsigned line )
	{
	if( m_BitVector == nullptr || m_Lines[line].Address < -1 || ( m_Lines[line].Address == -1 && line >= DISASSEMBLY_LINES - 1 ))
		SetIcon1( line, 0 );
	else
		{
		if( m_Lines[line].Address == -1 )
			{
			BitVectorToggleBit( m_BitVector, m_Lines[line+1].Address );
			SetIcon1( line + 1, BitVectorQueryBit( m_BitVector, m_Lines[line+1].Address ) ? 4 : 0 );
			}
		else
			{
			BitVectorToggleBit( m_BitVector, m_Lines[line].Address );
			SetIcon1( line, BitVectorQueryBit( m_BitVector, m_Lines[line].Address ) ? 4 : 0 );
			}
		}
	}
//==============================================================================
void DisassemblyPanel::OnLeftDownBkpt( wxMouseEvent &event )
	{
	DisassemblyBitmap	*BitMap	= static_cast<DisassemblyBitmap*>( event.GetEventObject() );
	unsigned			Line	= BitMap->GetLine();

	ToggleBreakpoint( Line );
	}
//==============================================================================
void DisassemblyPanel::OnLeftDClick( wxMouseEvent& event )
	{
	DisassemblyText		*Text	= static_cast<DisassemblyText*>( event.GetEventObject() );
	unsigned			Line	= Text->GetLine();

	ToggleBreakpoint( Line );
	}
//==============================================================================
