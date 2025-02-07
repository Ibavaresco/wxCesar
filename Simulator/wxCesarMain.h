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
/*============================================================================*/
#if			!defined __WXCESARMAIN_H__
#define __WXCESARMAIN_H__
/*============================================================================*/
#include <stdint.h>
#include "wxCesarApp.h"
#include "wxCesarFrameBase.h"
#include "wxCesarSimulator.h"
#include "wxCesarLCD.h"
/*============================================================================*/
class wxCesarFrame: public wxCesarFrameBase
	{
    public:
						wxCesarFrame				( wxDialog *dlg );
						~wxCesarFrame				();
    private:
        virtual void	OnClose						( wxCloseEvent& event );
        virtual void	OnQuit						( wxCommandEvent& event );
        virtual void	OnAbout						( wxCommandEvent& event );

        virtual void	LoadMEMFile					( wxCommandEvent& event );

		virtual void	IdleLoop					( wxIdleEvent& event );

		virtual void	EvtEnter_RegisterHexa		( wxCommandEvent& event );
		virtual void	EvtEnter_RegisterUnsigned	( wxCommandEvent& event );
		virtual void	EvtEnter_RegisterSigned		( wxCommandEvent& event );
		virtual void	EvtClick_Flags				( wxMouseEvent& event );
		virtual void	EvtClick_INTE				( wxMouseEvent& event );
		virtual void	EvtClick_INTS				( wxMouseEvent& event );
		virtual void	EvtClick_TECST				( wxMouseEvent& event );
		virtual void	Evt_KillFocus				( wxFocusEvent& event );
		virtual void	Evt_SetFocus				( wxFocusEvent& event );
		virtual void	EvtEnter_TECDT_Hexa			( wxCommandEvent& event );
		virtual void	EvtEnter_TECDT_Unsigned		( wxCommandEvent& event );
		virtual void	EvtEnter_TECDT_Signed		( wxCommandEvent& event );
		virtual void	EvtOnChar_TECDT_Char		( wxKeyEvent& event );
		virtual void	Evt_SetFocusTECDT			( wxFocusEvent& event );
		virtual void	Evt_KillFocusTECDT			( wxFocusEvent& event );
		virtual void	EvtEnter_TIMDT_Hexa			( wxCommandEvent& event );
		virtual void	EvtEnter_TIMDT_Unsigned		( wxCommandEvent& event );
		virtual void	Evt_KillFocusTIMDT			( wxFocusEvent& event );

		virtual void	Evt_SetFocusRegHex			( wxFocusEvent& event );
		virtual void	Evt_SetFocusRegUns			( wxFocusEvent& event );
		virtual void	Evt_SetFocusRegSign			( wxFocusEvent& event );

		virtual void	EvtClick_SimulatorRun		( wxCommandEvent& event );
		virtual void	EvtClick_SimulatorPause		( wxCommandEvent& event );
		virtual void 	EvtClick_RunMaxSpeed		( wxCommandEvent& event );
		virtual void	EvtClick_SimulatotAnimate	( wxCommandEvent& event );
		virtual void	EvtClick_SimulatorStepInto	( wxCommandEvent& event );
		virtual void	EvtClick_SimulatorStepOver	( wxCommandEvent& event );
		virtual void	EvtClick_SimulatorFaster	( wxCommandEvent& event );
		virtual void	EvtClick_SimulatorSlower	( wxCommandEvent& event );
		virtual void	EvtClick_SimulatorReset		( wxCommandEvent& event );
		virtual void	EvtClick_ClearTrace			( wxCommandEvent& event );

		virtual void	evtScroll_MemoryDump		( wxScrollEvent& event );
//		virtual void	ValueChanged				( wxDataViewEvent& event );
	/*------------------------------------------------------------------------*/
		virtual void	DontEraseBackGround			( wxEraseEvent& event );
	/*------------------------------------------------------------------------*/
		void			ShowDisassembly				();
		void			RefreshLCD					();
	/*------------------------------------------------------------------------*/
		void			CheckFlags					();
		void			CheckTIMDT					();
		void			CheckINTE					();
		void			CheckINTS					();
		void			CheckTECST					();
		void			CheckTECDT					();

		void			RefreshFlags				( const wxColour &Colour, bool Forced );
		void			RefreshTIMDT				( const wxColour &Colour, bool Forced );
		void			RefreshINTE					( const wxColour &Colour, bool Forced );
		void			RefreshINTS					( const wxColour &Colour, bool Forced );
		void			RefreshTECST				( const wxColour &Colour, bool Forced );
		void			RefreshTECDT				( const wxColour &Colour, bool Forced );

		void			UpdateTIMDT					( uint8_t Value );
		void			Update_TIMDTHexa			();
		void			Update_TIMDTUnsigned		();
	/*------------------------------------------------------------------------*/
		void			RefreshRegister				( unsigned RegisterIndex, const wxColour &Colour, bool Forced );
		void			UpdateRegister				( unsigned RegisterIndex, uint16_t Value, bool Forced );
		void			Update_RegisterHexa			( wxTextCtrl *TextCtrl, int RegisterIndex, bool Forced );
		void			Update_RegisterUnsigned		( wxTextCtrl *TextCtrl, int RegisterIndex, bool Forced );
		void			Update_RegisterSigned		( wxTextCtrl *TextCtrl, int RegisterIndex, bool Forced );
	/*------------------------------------------------------------------------*/
		void			UpdateTECDT					( uint8_t Value, bool Forced );
		void			Update_TECDTHexa			( bool Forced );
		void			Update_TECDTUnsigned		( bool Forced );
		void			Update_TECDTSigned			( bool Forced );
	/*------------------------------------------------------------------------*/
		void			EnableControls				();
		void			DisableControls				();
	/*------------------------------------------------------------------------*/
		void			ShowStatusBarMessage		( int Code, const wxString &Message, const wxColour &Colour );
		void			ClearStatusBarMessage		( int Code );
	/*------------------------------------------------------------------------*/
		wxString		Title;

		wxColour		BkColour;
		wxCesarApp		*App;
		csimcpustate_t	*pCPUState;
		wxWindow		*HadFocus;

		bool			PreviousBusy;
		bool			ControlsEnabled;
		bool			RefreshAll;
		bool			SomethingChanged;
		int				Painted[8];
		wxTextCtrl		*RegHexa[9];
		wxTextCtrl		*RegUnsigned[9];
		wxTextCtrl		*RegSigned[9];
		bool			m_PreviousChangedMem;
	/*------------------------------------------------------------------------*/
		wxMemoryDC		*DC_LCD;
		wxBitmap		BitMapLCD;
	/*------------------------------------------------------------------------*/
		wxString		FileName;
		int				LastMessageId;
		uint16_t		DumpStartAddress;
	/*------------------------------------------------------------------------*/
	};
/*============================================================================*/
#endif	//	!defined __WXCESARMAIN_H__
/*============================================================================*/
