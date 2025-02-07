///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "DisassemblyPanel.h"
#include "MemoryDumpPanel.h"
#include "wxCesarLCD.h"

#include "wxCesarFrameBase.h"

///////////////////////////////////////////////////////////////////////////

wxCesarFrameBase::wxCesarFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 898,611 ), wxSize( 898,611 ) );
	this->SetExtraStyle( wxWS_EX_PROCESS_IDLE );

	m_menubar2 = new wxMenuBar( 0 );
	m_menu3 = new wxMenu();
	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("L&oad MEM File...") ) + wxT('\t') + wxT("Alt-O"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem5 );

	wxMenuItem* m_menuItem6;
	m_menuItem6 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("&Load State...") ) + wxT('\t') + wxT("Alt-L"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem6 );

	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( m_menu3, wxID_ANY, wxString( _("&Save State...") ) + wxT('\t') + wxT("Alt-S"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem7 );

	m_menu3->AppendSeparator();

	wxMenuItem* m_menuItem8;
	m_menuItem8 = new wxMenuItem( m_menu3, wxID_QUIT, wxString( _("&Quit") ) + wxT('\t') + wxT("Alt-Q"), wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_menuItem8 );

	m_menubar2->Append( m_menu3, _("&File") );

	m_menu6 = new wxMenu();
	wxMenuItem* m_menuItem15;
	m_menuItem15 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("Run") ) + wxT('\t') + wxT("F5"), wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem15 );

	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("&Pause") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem11 );

	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("Animate") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem18 );

	wxMenuItem* m_menuItem131;
	m_menuItem131 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("Run Max. Speed") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem131 );

	m_menu6->AppendSeparator();

	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("Step Into") ) + wxT('\t') + wxT("F7"), wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem12 );

	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("Step Over") ) + wxT('\t') + wxT("F8"), wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem13 );

	m_menu6->AppendSeparator();

	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( m_menu6, wxID_ANY, wxString( _("Reset") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( m_menuItem16 );

	m_menubar2->Append( m_menu6, _("&Run") );

	m_menu5 = new wxMenu();
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem( m_menu5, wxID_ANY, wxString( _("&Help...") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem9 );

	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( m_menu5, wxID_ANY, wxString( _("A&bout...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( m_menuItem10 );

	m_menubar2->Append( m_menu5, _("&Help") );

	this->SetMenuBar( m_menubar2 );

	wxBoxSizer* bSizer58;
	bSizer58 = new wxBoxSizer( wxVERTICAL );

	m_panel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCLIP_CHILDREN );
	wxBoxSizer* bSizer61;
	bSizer61 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer63;
	bSizer63 = new wxBoxSizer( wxVERTICAL );

	m_Disassembly = new DisassemblyPanel( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE );
	m_Disassembly->SetBackgroundColour( wxColour( 255, 255, 255 ) );

	bSizer63->Add( m_Disassembly, 1, wxTOP|wxRIGHT|wxLEFT|wxEXPAND, 5 );

	m_Dump = new MemoryDumpPanel( m_panel3, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxBORDER_SIMPLE|wxTAB_TRAVERSAL );
	bSizer63->Add( m_Dump, 0, wxALL, 5 );

	m_BitMapLCD = new wxCesarLCD( m_panel3, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 619,32 ), wxBORDER_SIMPLE );
	m_BitMapLCD->SetMinSize( wxSize( 619,32 ) );
	m_BitMapLCD->SetMaxSize( wxSize( 619,32 ) );

	bSizer63->Add( m_BitMapLCD, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxRIGHT|wxLEFT, 5 );


	bSizer61->Add( bSizer63, 0, 0, 5 );

	wxBoxSizer* bSizer64;
	bSizer64 = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 9, 4, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText48 = new wxStaticText( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer1->Add( m_staticText48, 0, wxTOP|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText69 = new wxStaticText( m_panel3, wxID_ANY, _("Hexa"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText69->Wrap( -1 );
	fgSizer1->Add( m_staticText69, 0, wxTOP|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText70 = new wxStaticText( m_panel3, wxID_ANY, _("Unsig."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText70->Wrap( -1 );
	fgSizer1->Add( m_staticText70, 0, wxTOP|wxLEFT|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_staticText71 = new wxStaticText( m_panel3, wxID_ANY, _("Signed"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer1->Add( m_staticText71, 0, wxTOP|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText72 = new wxStaticText( m_panel3, wxID_ANY, _("R0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText72->Wrap( -1 );
	fgSizer1->Add( m_staticText72, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R0Hexa = new wxTextCtrl( m_panel3, wcID_R0_Hexa, _("8000h"), wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R0Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R0Hexa->SetMaxLength( 5 );
	}
	#else
	m_R0Hexa->SetMaxLength( 5 );
	#endif
	m_R0Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R0Hexa, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_R0Unsigned = new wxTextCtrl( m_panel3, wcID_R0_Unsigned, _("32768d"), wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R0Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R0Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R0Unsigned->SetMaxLength( 6 );
	#endif
	m_R0Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R0Unsigned, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_R0Signed = new wxTextCtrl( m_panel3, wcID_R0_Signed, _("-32768d"), wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R0Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R0Signed->SetMaxLength( 7 );
	}
	#else
	m_R0Signed->SetMaxLength( 7 );
	#endif
	m_R0Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R0Signed, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxLEFT, 5 );

	m_staticText481 = new wxStaticText( m_panel3, wxID_ANY, _("R1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText481->Wrap( -1 );
	fgSizer1->Add( m_staticText481, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_R1Hexa = new wxTextCtrl( m_panel3, wcID_R1_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R1Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R1Hexa->SetMaxLength( 5 );
	}
	#else
	m_R1Hexa->SetMaxLength( 5 );
	#endif
	m_R1Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R1Hexa, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R1Unsigned = new wxTextCtrl( m_panel3, wcID_R1_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R1Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R1Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R1Unsigned->SetMaxLength( 6 );
	#endif
	m_R1Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R1Unsigned, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R1Signed = new wxTextCtrl( m_panel3, wcID_R1_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R1Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R1Signed->SetMaxLength( 7 );
	}
	#else
	m_R1Signed->SetMaxLength( 7 );
	#endif
	m_R1Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R1Signed, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText4811 = new wxStaticText( m_panel3, wxID_ANY, _("R2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4811->Wrap( -1 );
	fgSizer1->Add( m_staticText4811, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5 );

	m_R2Hexa = new wxTextCtrl( m_panel3, wcID_R2_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R2Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R2Hexa->SetMaxLength( 5 );
	}
	#else
	m_R2Hexa->SetMaxLength( 5 );
	#endif
	m_R2Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R2Hexa, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R2Unsigned = new wxTextCtrl( m_panel3, wcID_R2_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R2Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R2Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R2Unsigned->SetMaxLength( 6 );
	#endif
	m_R2Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R2Unsigned, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R2Signed = new wxTextCtrl( m_panel3, wcID_R2_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R2Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R2Signed->SetMaxLength( 7 );
	}
	#else
	m_R2Signed->SetMaxLength( 7 );
	#endif
	m_R2Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R2Signed, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText48111 = new wxStaticText( m_panel3, wxID_ANY, _("R3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48111->Wrap( -1 );
	fgSizer1->Add( m_staticText48111, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R3Hexa = new wxTextCtrl( m_panel3, wcID_R3_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R3Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R3Hexa->SetMaxLength( 5 );
	}
	#else
	m_R3Hexa->SetMaxLength( 5 );
	#endif
	m_R3Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R3Hexa, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R3Unsigned = new wxTextCtrl( m_panel3, wcID_R3_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R3Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R3Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R3Unsigned->SetMaxLength( 6 );
	#endif
	m_R3Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R3Unsigned, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R3Signed = new wxTextCtrl( m_panel3, wcID_R3_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R3Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R3Signed->SetMaxLength( 7 );
	}
	#else
	m_R3Signed->SetMaxLength( 7 );
	#endif
	m_R3Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R3Signed, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText481111 = new wxStaticText( m_panel3, wxID_ANY, _("R4"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText481111->Wrap( -1 );
	fgSizer1->Add( m_staticText481111, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R4Hexa = new wxTextCtrl( m_panel3, wcID_R4_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R4Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R4Hexa->SetMaxLength( 5 );
	}
	#else
	m_R4Hexa->SetMaxLength( 5 );
	#endif
	m_R4Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R4Hexa, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R4Unsigned = new wxTextCtrl( m_panel3, wcID_R4_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R4Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R4Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R4Unsigned->SetMaxLength( 6 );
	#endif
	m_R4Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R4Unsigned, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R4Signed = new wxTextCtrl( m_panel3, wcID_R4_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R4Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R4Signed->SetMaxLength( 7 );
	}
	#else
	m_R4Signed->SetMaxLength( 7 );
	#endif
	m_R4Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R4Signed, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText4811111 = new wxStaticText( m_panel3, wxID_ANY, _("R5"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4811111->Wrap( -1 );
	fgSizer1->Add( m_staticText4811111, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R5Hexa = new wxTextCtrl( m_panel3, wcID_R5_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R5Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R5Hexa->SetMaxLength( 5 );
	}
	#else
	m_R5Hexa->SetMaxLength( 5 );
	#endif
	m_R5Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R5Hexa, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R5Unsigned = new wxTextCtrl( m_panel3, wcID_R5_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R5Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R5Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R5Unsigned->SetMaxLength( 6 );
	#endif
	m_R5Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R5Unsigned, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R5Signed = new wxTextCtrl( m_panel3, wcID_R5_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R5Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R5Signed->SetMaxLength( 7 );
	}
	#else
	m_R5Signed->SetMaxLength( 7 );
	#endif
	m_R5Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R5Signed, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText48111111 = new wxStaticText( m_panel3, wxID_ANY, _("R6/SP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48111111->Wrap( -1 );
	fgSizer1->Add( m_staticText48111111, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R6Hexa = new wxTextCtrl( m_panel3, wcID_R6_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R6Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R6Hexa->SetMaxLength( 5 );
	}
	#else
	m_R6Hexa->SetMaxLength( 5 );
	#endif
	m_R6Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R6Hexa, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R6Unsigned = new wxTextCtrl( m_panel3, wcID_R6_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R6Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R6Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R6Unsigned->SetMaxLength( 6 );
	#endif
	m_R6Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R6Unsigned, 0, wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_R6Signed = new wxTextCtrl( m_panel3, wcID_R6_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R6Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R6Signed->SetMaxLength( 7 );
	}
	#else
	m_R6Signed->SetMaxLength( 7 );
	#endif
	m_R6Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R6Signed, 0, wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText481111111 = new wxStaticText( m_panel3, wxID_ANY, _("R7/PC"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText481111111->Wrap( -1 );
	fgSizer1->Add( m_staticText481111111, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5 );

	m_R7Hexa = new wxTextCtrl( m_panel3, wcID_R7_Hexa, wxEmptyString, wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R7Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_R7Hexa->SetMaxLength( 5 );
	}
	#else
	m_R7Hexa->SetMaxLength( 5 );
	#endif
	m_R7Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R7Hexa, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5 );

	m_R7Unsigned = new wxTextCtrl( m_panel3, wcID_R7_Unsigned, wxEmptyString, wxDefaultPosition, wxSize( 66,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R7Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_R7Unsigned->SetMaxLength( 6 );
	}
	#else
	m_R7Unsigned->SetMaxLength( 6 );
	#endif
	m_R7Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R7Unsigned, 0, wxALIGN_CENTER_VERTICAL|wxBOTTOM|wxLEFT, 5 );

	m_R7Signed = new wxTextCtrl( m_panel3, wcID_R7_Signed, wxEmptyString, wxDefaultPosition, wxSize( 74,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_R7Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_R7Signed->SetMaxLength( 7 );
	}
	#else
	m_R7Signed->SetMaxLength( 7 );
	#endif
	m_R7Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer1->Add( m_R7Signed, 0, wxBOTTOM|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer64->Add( fgSizer1, 0, wxEXPAND, 5 );

	m_staticline4 = new wxStaticLine( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer64->Add( m_staticline4, 0, wxEXPAND|wxRIGHT|wxLEFT, 5 );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 4, 9, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText79 = new wxStaticText( m_panel3, wxID_ANY, _("Flags"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText79->Wrap( -1 );
	fgSizer2->Add( m_staticText79, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_Flags7 = new wxStaticText( m_panel3, wcID_Flags_7, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_Flags7->Wrap( -1 );
	fgSizer2->Add( m_Flags7, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_Flags6 = new wxStaticText( m_panel3, wcID_Flags_6, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_Flags6->Wrap( -1 );
	fgSizer2->Add( m_Flags6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_Flags5 = new wxStaticText( m_panel3, wcID_Flags_5, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_Flags5->Wrap( -1 );
	fgSizer2->Add( m_Flags5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_Flags4 = new wxStaticText( m_panel3, wcID_Flags_4, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_Flags4->Wrap( -1 );
	fgSizer2->Add( m_Flags4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_FlagsN = new wxStaticText( m_panel3, wcID_Flags_N, _("N"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_FlagsN->Wrap( -1 );
	fgSizer2->Add( m_FlagsN, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_FlagsZ = new wxStaticText( m_panel3, wcID_Flags_Z, _("Z"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_FlagsZ->Wrap( -1 );
	fgSizer2->Add( m_FlagsZ, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_FlagsV = new wxStaticText( m_panel3, wcID_Flags_V, _("V"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_FlagsV->Wrap( -1 );
	fgSizer2->Add( m_FlagsV, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_FlagsC = new wxStaticText( m_panel3, wcID_Flags_C, _("C"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_FlagsC->Wrap( -1 );
	fgSizer2->Add( m_FlagsC, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_staticText791 = new wxStaticText( m_panel3, wxID_ANY, _("INTE"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText791->Wrap( -1 );
	fgSizer2->Add( m_staticText791, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_INTE_I = new wxStaticText( m_panel3, wcID_INTE_7, _("I"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_I->Wrap( -1 );
	fgSizer2->Add( m_INTE_I, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_6 = new wxStaticText( m_panel3, wcID_INTE_6, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_6->Wrap( -1 );
	fgSizer2->Add( m_INTE_6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_5 = new wxStaticText( m_panel3, wcID_INTE_5, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_5->Wrap( -1 );
	fgSizer2->Add( m_INTE_5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_4 = new wxStaticText( m_panel3, wcID_INTE_4, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_4->Wrap( -1 );
	fgSizer2->Add( m_INTE_4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_3 = new wxStaticText( m_panel3, wcID_INTE_3, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_3->Wrap( -1 );
	fgSizer2->Add( m_INTE_3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_S = new wxStaticText( m_panel3, wcID_INTE_2, _("S"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_S->Wrap( -1 );
	fgSizer2->Add( m_INTE_S, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_K = new wxStaticText( m_panel3, wcID_INTE_1, _("K"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_K->Wrap( -1 );
	fgSizer2->Add( m_INTE_K, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTE_T = new wxStaticText( m_panel3, wcID_INTE_0, _("T"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTE_T->Wrap( -1 );
	fgSizer2->Add( m_INTE_T, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_staticText7911 = new wxStaticText( m_panel3, wcID_INTS_7, _("INTS"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7911->Wrap( -1 );
	fgSizer2->Add( m_staticText7911, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_INTS_i = new wxStaticText( m_panel3, wcID_INTS_7, _("i"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_i->Wrap( -1 );
	fgSizer2->Add( m_INTS_i, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_6 = new wxStaticText( m_panel3, wcID_INTS_6, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_6->Wrap( -1 );
	fgSizer2->Add( m_INTS_6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_5 = new wxStaticText( m_panel3, wcID_INTS_5, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_5->Wrap( -1 );
	fgSizer2->Add( m_INTS_5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_4 = new wxStaticText( m_panel3, wcID_INTS_4, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_4->Wrap( -1 );
	fgSizer2->Add( m_INTS_4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_3 = new wxStaticText( m_panel3, wcID_INTS_3, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_3->Wrap( -1 );
	fgSizer2->Add( m_INTS_3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_s = new wxStaticText( m_panel3, wcID_INTS_2, _("s"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_s->Wrap( -1 );
	fgSizer2->Add( m_INTS_s, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_k = new wxStaticText( m_panel3, wcID_INTS_1, _("k"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_k->Wrap( -1 );
	fgSizer2->Add( m_INTS_k, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_INTS_t = new wxStaticText( m_panel3, wcID_INTS_0, _("t"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_INTS_t->Wrap( -1 );
	fgSizer2->Add( m_INTS_t, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_staticText79111 = new wxStaticText( m_panel3, wxID_ANY, _("TECST"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText79111->Wrap( -1 );
	fgSizer2->Add( m_staticText79111, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_TECST_A = new wxStaticText( m_panel3, wcID_TECST_7, _("A"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_A->Wrap( -1 );
	fgSizer2->Add( m_TECST_A, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_6 = new wxStaticText( m_panel3, wcID_TECST_6, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_6->Wrap( -1 );
	fgSizer2->Add( m_TECST_6, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_5 = new wxStaticText( m_panel3, wcID_TECST_5, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_5->Wrap( -1 );
	fgSizer2->Add( m_TECST_5, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_4 = new wxStaticText( m_panel3, wcID_TECST_4, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_4->Wrap( -1 );
	fgSizer2->Add( m_TECST_4, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_3 = new wxStaticText( m_panel3, wcID_TECST_3, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_3->Wrap( -1 );
	fgSizer2->Add( m_TECST_3, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_2 = new wxStaticText( m_panel3, wcID_TECST_2, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_2->Wrap( -1 );
	fgSizer2->Add( m_TECST_2, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_1 = new wxStaticText( m_panel3, wcID_TECST_1, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_1->Wrap( -1 );
	fgSizer2->Add( m_TECST_1, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECST_0 = new wxStaticText( m_panel3, wcID_TECST_0, _("-"), wxDefaultPosition, wxSize( 12,-1 ), wxALIGN_CENTER_HORIZONTAL|wxBORDER_SIMPLE );
	m_TECST_0->Wrap( -1 );
	fgSizer2->Add( m_TECST_0, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );


	bSizer64->Add( fgSizer2, 0, wxEXPAND, 0 );

	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 5, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText791111 = new wxStaticText( m_panel3, wxID_ANY, _("TECDT"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText791111->Wrap( -1 );
	fgSizer3->Add( m_staticText791111, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxRIGHT|wxLEFT, 5 );

	m_TECDT_Hexa = new wxTextCtrl( m_panel3, wcID_TECDT_Hexa, _("80h"), wxDefaultPosition, wxSize( 42,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_TECDT_Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_TECDT_Hexa->SetMaxLength( 3 );
	}
	#else
	m_TECDT_Hexa->SetMaxLength( 3 );
	#endif
	m_TECDT_Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer3->Add( m_TECDT_Hexa, 0, wxALIGN_CENTER_VERTICAL|wxTOP, 5 );

	m_TECDT_Unsigned = new wxTextCtrl( m_panel3, wcID_TECDT_Unsigned, _("128d"), wxDefaultPosition, wxSize( 50,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_TECDT_Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_TECDT_Unsigned->SetMaxLength( 4 );
	}
	#else
	m_TECDT_Unsigned->SetMaxLength( 4 );
	#endif
	m_TECDT_Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer3->Add( m_TECDT_Unsigned, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECDT_Signed = new wxTextCtrl( m_panel3, wcID_TECDT_Signed, _("-128d"), wxDefaultPosition, wxSize( 58,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_TECDT_Signed->HasFlag( wxTE_MULTILINE ) )
	{
	m_TECDT_Signed->SetMaxLength( 5 );
	}
	#else
	m_TECDT_Signed->SetMaxLength( 5 );
	#endif
	m_TECDT_Signed->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer3->Add( m_TECDT_Signed, 0, wxALIGN_CENTER_VERTICAL|wxTOP|wxLEFT, 5 );

	m_TECDT_Char = new wxTextCtrl( m_panel3, wcID_TECDT_Char, _("C"), wxDefaultPosition, wxSize( 26,22 ), wxTE_CENTER|wxTE_NOHIDESEL|wxTE_PROCESS_ENTER );
	#ifdef __WXGTK__
	if ( !m_TECDT_Char->HasFlag( wxTE_MULTILINE ) )
	{
	m_TECDT_Char->SetMaxLength( 1 );
	}
	#else
	m_TECDT_Char->SetMaxLength( 1 );
	#endif
	m_TECDT_Char->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer3->Add( m_TECDT_Char, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText7911111 = new wxStaticText( m_panel3, wxID_ANY, _("TIMDT"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7911111->Wrap( -1 );
	fgSizer3->Add( m_staticText7911111, 0, wxTOP|wxRIGHT|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );

	m_TIMDT_Hexa = new wxTextCtrl( m_panel3, wcID_TIMDT_Hexa, _("80h"), wxDefaultPosition, wxSize( 42,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_TIMDT_Hexa->HasFlag( wxTE_MULTILINE ) )
	{
	m_TIMDT_Hexa->SetMaxLength( 3 );
	}
	#else
	m_TIMDT_Hexa->SetMaxLength( 3 );
	#endif
	m_TIMDT_Hexa->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer3->Add( m_TIMDT_Hexa, 0, wxTOP|wxALIGN_CENTER_VERTICAL, 5 );

	m_TIMDT_Unsigned = new wxTextCtrl( m_panel3, wcID_TIMDT_Unsigned, _("128d"), wxDefaultPosition, wxSize( 50,22 ), wxTE_NOHIDESEL|wxTE_PROCESS_ENTER|wxTE_RIGHT );
	#ifdef __WXGTK__
	if ( !m_TIMDT_Unsigned->HasFlag( wxTE_MULTILINE ) )
	{
	m_TIMDT_Unsigned->SetMaxLength( 4 );
	}
	#else
	m_TIMDT_Unsigned->SetMaxLength( 4 );
	#endif
	m_TIMDT_Unsigned->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Courier New") ) );

	fgSizer3->Add( m_TIMDT_Unsigned, 0, wxTOP|wxLEFT|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer64->Add( fgSizer3, 0, wxEXPAND, 0 );

	m_staticline5 = new wxStaticLine( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer64->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 0, 3, 0, 0 );

	m_ButtonRun = new wxButton( m_panel3, wxID_ANY, _("Run"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer2->Add( m_ButtonRun, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_ButtonPause = new wxButton( m_panel3, wxID_ANY, _("Pause"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer2->Add( m_ButtonPause, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	m_ButtonAnimate = new wxButton( m_panel3, wxID_ANY, _("Animate"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer2->Add( m_ButtonAnimate, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_ButtonStepInto = new wxButton( m_panel3, wxID_ANY, _("Step Into"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer2->Add( m_ButtonStepInto, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxTOP, 5 );

	m_ButtonStepOver = new wxButton( m_panel3, wxID_ANY, _("Step Over"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer2->Add( m_ButtonStepOver, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxTOP, 5 );

	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 0, 2, 0, 0 );

	m_ButtonSpeedUp = new wxButton( m_panel3, wxID_ANY, _("+"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	gSizer5->Add( m_ButtonSpeedUp, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxTOP, 5 );

	m_ButtonSlowDown = new wxButton( m_panel3, wxID_ANY, _("-"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	gSizer5->Add( m_ButtonSlowDown, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxTOP, 5 );


	gSizer2->Add( gSizer5, 1, wxEXPAND, 5 );


	bSizer64->Add( gSizer2, 0, wxEXPAND, 5 );

	m_staticline7 = new wxStaticLine( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer64->Add( m_staticline7, 0, wxEXPAND | wxALL, 5 );

	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 0, 3, 0, 0 );

	m_ButtonMaxSpeed = new wxButton( m_panel3, wxID_ANY, _("Max. Sp."), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer3->Add( m_ButtonMaxSpeed, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_ButtonClearTrace = new wxButton( m_panel3, wxID_ANY, _("Clr. Trace"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer3->Add( m_ButtonClearTrace, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );

	m_ButtonReset = new wxButton( m_panel3, wxID_ANY, _("Reset"), wxDefaultPosition, wxSize( 65,-1 ), 0 );
	gSizer3->Add( m_ButtonReset, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );


	bSizer64->Add( gSizer3, 0, wxEXPAND, 5 );


	bSizer61->Add( bSizer64, 0, 0, 5 );


	m_panel3->SetSizer( bSizer61 );
	m_panel3->Layout();
	bSizer61->Fit( m_panel3 );
	bSizer58->Add( m_panel3, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer58 );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, 0, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( wxCesarFrameBase::IdleLoop ) );
	m_menu3->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::LoadMEMFile ), this, m_menuItem5->GetId());
	m_menu3->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::OnQuit ), this, m_menuItem8->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorRun ), this, m_menuItem15->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorPause ), this, m_menuItem11->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorAnimate ), this, m_menuItem18->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_RunMaxSpeed ), this, m_menuItem131->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorStepInto ), this, m_menuItem12->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorStepOver ), this, m_menuItem13->GetId());
	m_menu6->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorReset ), this, m_menuItem16->GetId());
	m_menu5->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxCesarFrameBase::OnAbout ), this, m_menuItem10->GetId());
	m_BitMapLCD->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( wxCesarFrameBase::DontEraseBackGround ), NULL, this );
	m_R0Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R0Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R0Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R0Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R0Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R0Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R0Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R0Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R0Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R1Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R1Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R1Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R1Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R1Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R1Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R1Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R1Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R1Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R2Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R2Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R2Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R2Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R2Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R2Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R2Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R2Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R2Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R3Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R3Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R3Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R3Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R3Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R3Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R3Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R3Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R3Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R4Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R4Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R4Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R4Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R4Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R4Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R4Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R4Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R4Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R5Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R5Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R5Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R5Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R5Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R5Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R5Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R5Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R5Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R6Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R6Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R6Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R6Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R6Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R6Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R6Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R6Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R6Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_R7Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R7Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegHex ), NULL, this );
	m_R7Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterHexa ), NULL, this );
	m_R7Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R7Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegUns ), NULL, this );
	m_R7Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterUnsigned ), NULL, this );
	m_R7Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocus ), NULL, this );
	m_R7Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusRegSign ), NULL, this );
	m_R7Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_RegisterSigned ), NULL, this );
	m_Flags7->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_Flags6->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_Flags5->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_Flags4->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_FlagsN->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_FlagsZ->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_FlagsV->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_FlagsC->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_Flags ), NULL, this );
	m_INTE_I->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_6->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_5->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_4->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_3->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_S->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_K->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTE_T->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTE ), NULL, this );
	m_INTS_i->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_6->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_5->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_4->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_3->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_s->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_k->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_INTS_t->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_INTS ), NULL, this );
	m_TECST_A->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_6->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_5->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_4->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_3->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_2->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_1->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECST_0->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( wxCesarFrameBase::EvtClick_TECST ), NULL, this );
	m_TECDT_Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocusTECDT ), NULL, this );
	m_TECDT_Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusTECDT ), NULL, this );
	m_TECDT_Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_TECDT_Hexa ), NULL, this );
	m_TECDT_Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocusTECDT ), NULL, this );
	m_TECDT_Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusTECDT ), NULL, this );
	m_TECDT_Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_TECDT_Unsigned ), NULL, this );
	m_TECDT_Signed->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocusTECDT ), NULL, this );
	m_TECDT_Signed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusTECDT ), NULL, this );
	m_TECDT_Signed->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_TECDT_Signed ), NULL, this );
	m_TECDT_Char->Connect( wxEVT_CHAR, wxKeyEventHandler( wxCesarFrameBase::EvtOnChar_TECDT_Char ), NULL, this );
	m_TECDT_Char->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocusTECDT ), NULL, this );
	m_TECDT_Char->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocusTECDT ), NULL, this );
	m_TIMDT_Hexa->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocusTIMDT ), NULL, this );
	m_TIMDT_Hexa->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocus ), NULL, this );
	m_TIMDT_Hexa->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_TIMDT_Hexa ), NULL, this );
	m_TIMDT_Unsigned->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_KillFocusTIMDT ), NULL, this );
	m_TIMDT_Unsigned->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( wxCesarFrameBase::Evt_SetFocus ), NULL, this );
	m_TIMDT_Unsigned->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( wxCesarFrameBase::EvtEnter_TIMDT_Unsigned ), NULL, this );
	m_ButtonRun->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorRun ), NULL, this );
	m_ButtonPause->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorPause ), NULL, this );
	m_ButtonAnimate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatotAnimate ), NULL, this );
	m_ButtonStepInto->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorStepInto ), NULL, this );
	m_ButtonStepOver->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorStepOver ), NULL, this );
	m_ButtonSpeedUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorFaster ), NULL, this );
	m_ButtonSlowDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorSlower ), NULL, this );
	m_ButtonMaxSpeed->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_RunMaxSpeed ), NULL, this );
	m_ButtonClearTrace->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_ClearTrace ), NULL, this );
	m_ButtonReset->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( wxCesarFrameBase::EvtClick_SimulatorReset ), NULL, this );
}

wxCesarFrameBase::~wxCesarFrameBase()
{
}
