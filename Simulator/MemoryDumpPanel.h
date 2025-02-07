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
#pragma once
//==============================================================================
#include <stdint.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/scrolbar.h>
//==============================================================================
#include "BitVector.h"
//==============================================================================
#define	MEMORY_DUMP_COLUMNS	16
#define	MEMORY_DUMP_LINES	 9
//==============================================================================
class	HexEdit;
class	CharEdit;
//==============================================================================
class MemoryDumpPanel: public wxPanel
	{
	//--------------------------------------------------------------------------
	protected:
	//--------------------------------------------------------------------------
		wxPanel			*m_panel1;
		wxStaticText	*m_Addresses[MEMORY_DUMP_LINES];
		HexEdit			*m_CellsHex[MEMORY_DUMP_LINES][MEMORY_DUMP_COLUMNS];
		CharEdit		*m_CellsChar[MEMORY_DUMP_LINES][MEMORY_DUMP_COLUMNS];
		wxScrollBar		*m_scrollBar1;
		wxWindow		*m_FocusOwner;

		uint16_t		m_StartAddress;
		uint8_t			*m_Buffer;
		uint16_t		m_BufferLength;
		bool			m_Editable;
		bitvector_t		*m_BreakPointsRead;
		bitvector_t		*m_BreakPointsWrite;
		bitvector_t		*m_Written;
		bitvector_t		*m_Changed;

		wxBitmap		*m_HexBitmaps[256];
	//--------------------------------------------------------------------------
		virtual void	OnCharHex					( wxKeyEvent &event );
		virtual void	OnKillFocusHex				( wxFocusEvent &event );
		virtual void	OnLeftDownHex				( wxMouseEvent &event );
		virtual void	OnSetFocusHex				( wxFocusEvent &event );
		virtual void	OnTextEnterHex				( wxCommandEvent &event );
		virtual void	OnTextMaxLenHex				( wxCommandEvent &event );

		virtual void	OnCharChar					( wxKeyEvent &event );
		virtual void	OnKillFocusChar				( wxFocusEvent &event );
		virtual void	OnLeftDownChar				( wxMouseEvent &event );
		virtual void	OnSetFocusChar				( wxFocusEvent &event );
		virtual void	OnTextEnterChar				( wxCommandEvent &event );
		virtual void	OnTextMaxLenChar			( wxCommandEvent &event );
		virtual void	DummyMouseEvent				( wxMouseEvent& event );


		virtual void	OnPanelSetFocus				( wxFocusEvent &event );
		virtual void	OnScrollChanged				( wxScrollEvent& event );
	//--------------------------------------------------------------------------
		void			MoveUpHex					( unsigned line, unsigned column );
		void			MoveDownHex					( unsigned line, unsigned column );
		void			MoveLeftHex					( unsigned line, unsigned column );
		void			MoveRightHex				( unsigned line, unsigned column );

		void			MoveUpChar					( unsigned line, unsigned column );
		void			MoveDownChar				( unsigned line, unsigned column );
		void			MoveLeftChar				( unsigned line, unsigned column );
		void			MoveRightChar				( unsigned line, unsigned column );
	//--------------------------------------------------------------------------
	public:
	//--------------------------------------------------------------------------
						MemoryDumpPanel				( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize( 455,300 ), long style = wxTAB_TRAVERSAL, const wxString &name = wxEmptyString );
						~MemoryDumpPanel			();
	//--------------------------------------------------------------------------
		void			SetBuffer					( uint8_t *buffer, uint16_t bufferlength );
		void			DumpRedraw					();
		void			SetEditable					( bool editable );
		void			SetBitVectors				( bitvector_t *breakpointsread, bitvector_t *breakpointswrite, bitvector_t *written, bitvector_t *changed );
	//--------------------------------------------------------------------------
		void			SetAdress					( unsigned line, uint16_t address );
		void			SetHexCellValue				( unsigned line, unsigned column, uint8_t value );
		void			SetCharCellValue			( unsigned line, unsigned column, char value );
		void			SetHexCellBackgroundColour	( unsigned line, unsigned column, const wxColour &color );
		void			SetCharCellBackgroundColour	( unsigned line, unsigned column, const wxColour &color );
	//--------------------------------------------------------------------------
	};
//==============================================================================
