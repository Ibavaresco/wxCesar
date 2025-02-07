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
//===============================================================================
#if			!defined __WXCESARLCD_H__
#define __WXCESARLCD_H__
//===============================================================================
#include <wx/statbmp.h>
//===============================================================================
class wxCesarLCD : public wxStaticBitmap
	{
	public:
						wxCesarLCD			( wxWindow *parent,
											  wxWindowID id,
											  const wxBitmapBundle& label,
											  const wxPoint& pos = wxDefaultPosition,
											  const wxSize& size = wxDefaultSize,
											  long style = 0,
											  const wxString& name = wxASCII_STR(wxStaticBitmapNameStr))
											  : wxStaticBitmap( parent, id, label, pos, size, style, name ) {};

	void				OnEraseBackGround	( wxEraseEvent& event ) {};

	DECLARE_EVENT_TABLE()
	};
/*============================================================================*/
//===============================================================================
#endif	//	!defined __WXCESARLCD_H__
//===============================================================================
