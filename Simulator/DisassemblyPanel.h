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
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
//==============================================================================
#include "BitVector.h"
//==============================================================================
#define	DISASSEMBLY_LINES	15
#define	DISASSEMBLY_ICONS	 6
//==============================================================================
class	DisassemblyBitmap;
class	DisassemblyText;
//==============================================================================
typedef struct
	{
	int					Address;
	DisassemblyBitmap	*Bitmap1;
	DisassemblyBitmap	*Bitmap2;
	DisassemblyText		*Text;
	} disassemblyline_t;
//==============================================================================
class DisassemblyPanel : public wxPanel
	{
	//--------------------------------------------------------------------------
	protected:
	//--------------------------------------------------------------------------
		wxBitmap			*m_bitmaps[DISASSEMBLY_ICONS];
		disassemblyline_t	m_Lines[DISASSEMBLY_LINES];
		bitvector_t			*m_BitVector;
	//--------------------------------------------------------------------------
		virtual void		OnLeftDownBkpt				( wxMouseEvent &event );
		virtual void		OnLeftDClick				( wxMouseEvent& event );

		void				ToggleBreakpoint			( unsigned line );
	//--------------------------------------------------------------------------
	public:
	//--------------------------------------------------------------------------
							DisassemblyPanel			( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize( 500, 300 ), long style = wxBORDER_SIMPLE | wxTAB_TRAVERSAL, const wxString &name = wxEmptyString );
							~DisassemblyPanel			();
		void				SetLine						( size_t line, int address, unsigned icon1, unsigned icon2, const wxString &text );
		void				SetIcon1					( size_t line, unsigned icon );
		void				SetIcon2					( size_t line, unsigned icon );
		void				SetBitVector				( bitvector_t *bitvector );
	//--------------------------------------------------------------------------
	};
//==============================================================================
