///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DisassemblyPanel;
class MemoryDumpPanel;
class wxCesarLCD;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class wxCesarFrameBase
///////////////////////////////////////////////////////////////////////////////
class wxCesarFrameBase : public wxFrame
{
	private:

	protected:
		enum
		{
			wxID_QUIT = 6000,
			wcID_R0_Hexa,
			wcID_R0_Unsigned,
			wcID_R0_Signed,
			wcID_R1_Hexa,
			wcID_R1_Unsigned,
			wcID_R1_Signed,
			wcID_R2_Hexa,
			wcID_R2_Unsigned,
			wcID_R2_Signed,
			wcID_R3_Hexa,
			wcID_R3_Unsigned,
			wcID_R3_Signed,
			wcID_R4_Hexa,
			wcID_R4_Unsigned,
			wcID_R4_Signed,
			wcID_R5_Hexa,
			wcID_R5_Unsigned,
			wcID_R5_Signed,
			wcID_R6_Hexa,
			wcID_R6_Unsigned,
			wcID_R6_Signed,
			wcID_R7_Hexa,
			wcID_R7_Unsigned,
			wcID_R7_Signed,
			wcID_Flags_7,
			wcID_Flags_6,
			wcID_Flags_5,
			wcID_Flags_4,
			wcID_Flags_N,
			wcID_Flags_Z,
			wcID_Flags_V,
			wcID_Flags_C,
			wcID_INTE_7,
			wcID_INTE_6,
			wcID_INTE_5,
			wcID_INTE_4,
			wcID_INTE_3,
			wcID_INTE_2,
			wcID_INTE_1,
			wcID_INTE_0,
			wcID_INTS_7,
			wcID_INTS_6,
			wcID_INTS_5,
			wcID_INTS_4,
			wcID_INTS_3,
			wcID_INTS_2,
			wcID_INTS_1,
			wcID_INTS_0,
			wcID_TECST_7,
			wcID_TECST_6,
			wcID_TECST_5,
			wcID_TECST_4,
			wcID_TECST_3,
			wcID_TECST_2,
			wcID_TECST_1,
			wcID_TECST_0,
			wcID_TECDT_Hexa,
			wcID_TECDT_Unsigned,
			wcID_TECDT_Signed,
			wcID_TECDT_Char,
			wcID_TIMDT_Hexa,
			wcID_TIMDT_Unsigned,
		};

		wxMenuBar* m_menubar2;
		wxMenu* m_menu3;
		wxMenu* m_menu6;
		wxMenu* m_menu5;
		wxPanel* m_panel3;
		DisassemblyPanel* m_Disassembly;
		MemoryDumpPanel* m_Dump;
		wxCesarLCD* m_BitMapLCD;
		wxStaticText* m_staticText48;
		wxStaticText* m_staticText69;
		wxStaticText* m_staticText70;
		wxStaticText* m_staticText71;
		wxStaticText* m_staticText72;
		wxTextCtrl* m_R0Hexa;
		wxTextCtrl* m_R0Unsigned;
		wxTextCtrl* m_R0Signed;
		wxStaticText* m_staticText481;
		wxTextCtrl* m_R1Hexa;
		wxTextCtrl* m_R1Unsigned;
		wxTextCtrl* m_R1Signed;
		wxStaticText* m_staticText4811;
		wxTextCtrl* m_R2Hexa;
		wxTextCtrl* m_R2Unsigned;
		wxTextCtrl* m_R2Signed;
		wxStaticText* m_staticText48111;
		wxTextCtrl* m_R3Hexa;
		wxTextCtrl* m_R3Unsigned;
		wxTextCtrl* m_R3Signed;
		wxStaticText* m_staticText481111;
		wxTextCtrl* m_R4Hexa;
		wxTextCtrl* m_R4Unsigned;
		wxTextCtrl* m_R4Signed;
		wxStaticText* m_staticText4811111;
		wxTextCtrl* m_R5Hexa;
		wxTextCtrl* m_R5Unsigned;
		wxTextCtrl* m_R5Signed;
		wxStaticText* m_staticText48111111;
		wxTextCtrl* m_R6Hexa;
		wxTextCtrl* m_R6Unsigned;
		wxTextCtrl* m_R6Signed;
		wxStaticText* m_staticText481111111;
		wxTextCtrl* m_R7Hexa;
		wxTextCtrl* m_R7Unsigned;
		wxTextCtrl* m_R7Signed;
		wxStaticLine* m_staticline4;
		wxStaticText* m_staticText79;
		wxStaticText* m_Flags7;
		wxStaticText* m_Flags6;
		wxStaticText* m_Flags5;
		wxStaticText* m_Flags4;
		wxStaticText* m_FlagsN;
		wxStaticText* m_FlagsZ;
		wxStaticText* m_FlagsV;
		wxStaticText* m_FlagsC;
		wxStaticText* m_staticText791;
		wxStaticText* m_INTE_I;
		wxStaticText* m_INTE_6;
		wxStaticText* m_INTE_5;
		wxStaticText* m_INTE_4;
		wxStaticText* m_INTE_3;
		wxStaticText* m_INTE_S;
		wxStaticText* m_INTE_K;
		wxStaticText* m_INTE_T;
		wxStaticText* m_staticText7911;
		wxStaticText* m_INTS_i;
		wxStaticText* m_INTS_6;
		wxStaticText* m_INTS_5;
		wxStaticText* m_INTS_4;
		wxStaticText* m_INTS_3;
		wxStaticText* m_INTS_s;
		wxStaticText* m_INTS_k;
		wxStaticText* m_INTS_t;
		wxStaticText* m_staticText79111;
		wxStaticText* m_TECST_A;
		wxStaticText* m_TECST_6;
		wxStaticText* m_TECST_5;
		wxStaticText* m_TECST_4;
		wxStaticText* m_TECST_3;
		wxStaticText* m_TECST_2;
		wxStaticText* m_TECST_1;
		wxStaticText* m_TECST_0;
		wxStaticText* m_staticText791111;
		wxTextCtrl* m_TECDT_Hexa;
		wxTextCtrl* m_TECDT_Unsigned;
		wxTextCtrl* m_TECDT_Signed;
		wxTextCtrl* m_TECDT_Char;
		wxStaticText* m_staticText7911111;
		wxTextCtrl* m_TIMDT_Hexa;
		wxTextCtrl* m_TIMDT_Unsigned;
		wxStaticLine* m_staticline5;
		wxButton* m_ButtonRun;
		wxButton* m_ButtonPause;
		wxButton* m_ButtonAnimate;
		wxButton* m_ButtonStepInto;
		wxButton* m_ButtonStepOver;
		wxButton* m_ButtonSpeedUp;
		wxButton* m_ButtonSlowDown;
		wxStaticLine* m_staticline7;
		wxButton* m_ButtonMaxSpeed;
		wxButton* m_ButtonClearTrace;
		wxButton* m_ButtonReset;
		wxStatusBar* m_statusBar;

		// Virtual event handlers, override them in your derived class
		virtual void IdleLoop( wxIdleEvent& event ) { event.Skip(); }
		virtual void LoadMEMFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorRun( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorPause( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorAnimate( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_RunMaxSpeed( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorStepInto( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorStepOver( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void DontEraseBackGround( wxEraseEvent& event ) { event.Skip(); }
		virtual void Evt_KillFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void Evt_SetFocusRegHex( wxFocusEvent& event ) { event.Skip(); }
		virtual void EvtEnter_RegisterHexa( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evt_SetFocusRegUns( wxFocusEvent& event ) { event.Skip(); }
		virtual void EvtEnter_RegisterUnsigned( wxCommandEvent& event ) { event.Skip(); }
		virtual void Evt_SetFocusRegSign( wxFocusEvent& event ) { event.Skip(); }
		virtual void EvtEnter_RegisterSigned( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_Flags( wxMouseEvent& event ) { event.Skip(); }
		virtual void EvtClick_INTE( wxMouseEvent& event ) { event.Skip(); }
		virtual void EvtClick_INTS( wxMouseEvent& event ) { event.Skip(); }
		virtual void EvtClick_TECST( wxMouseEvent& event ) { event.Skip(); }
		virtual void Evt_KillFocusTECDT( wxFocusEvent& event ) { event.Skip(); }
		virtual void Evt_SetFocusTECDT( wxFocusEvent& event ) { event.Skip(); }
		virtual void EvtEnter_TECDT_Hexa( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtEnter_TECDT_Unsigned( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtEnter_TECDT_Signed( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtOnChar_TECDT_Char( wxKeyEvent& event ) { event.Skip(); }
		virtual void Evt_KillFocusTIMDT( wxFocusEvent& event ) { event.Skip(); }
		virtual void Evt_SetFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void EvtEnter_TIMDT_Hexa( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtEnter_TIMDT_Unsigned( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatotAnimate( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorFaster( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_SimulatorSlower( wxCommandEvent& event ) { event.Skip(); }
		virtual void EvtClick_ClearTrace( wxCommandEvent& event ) { event.Skip(); }


	public:

		wxCesarFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CSIM - A simulator for the abstract computer 'Cesar' - v0.1.1"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 898,611 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~wxCesarFrameBase();

};

