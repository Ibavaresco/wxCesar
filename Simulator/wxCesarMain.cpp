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
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif
//==============================================================================
#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__
//==============================================================================
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include "wxCesarMain.h"
#include "wxCesarSimulator.h"
#include "wxCesarThread.h"
#include "wxCesarLCD.h"
#include "MemoryDumpPanel.h"
#include "DisassemblyPanel.h"
//==============================================================================
wxDECLARE_APP( wxCesarApp );
//==============================================================================
BEGIN_EVENT_TABLE( wxCesarLCD, wxStaticBitmap )
    EVT_ERASE_BACKGROUND( wxCesarLCD::OnEraseBackGround )
END_EVENT_TABLE()
/*============================================================================*/
//helper functions
enum wxbuildinfoformat
	{
	short_f,
	long_f
	};
//==============================================================================
wxString wxbuildinfo( wxbuildinfoformat format )
	{
    wxString wxbuild( wxVERSION_STRING );

    if( format == long_f )
		{
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
		}

    return wxbuild;
	}
//==============================================================================
void wxCesarFrame::OnAbout( wxCommandEvent &event )
    {
    //wxString msg = wxbuildinfo(long_f);
    wxMessageBox( _( "Simulator for the abstract computer \'Cesar\'\n"
					 "<https://www.inf.ufrgs.br/arq/wiki/doku.php?id=cesar>\n" "\n"
					 "This simulator created by Isaac Marino Bavaresco\n" "\n"
					 "Written using Code::Blocks 20.03 and wxFormBuilder v4.2.1\n"
					 "Compiled with MinGW64 v13.1.0\n"
					 "using the library wxWidgets v3.2.5" ), _( "wxCesar" ));
    }
//==============================================================================
wxCesarFrame::wxCesarFrame( wxDialog *dlg ) : wxCesarFrameBase( dlg )
	{
	App			= &wxGetApp();
	pCPUState	= &App->CPUState;

	Title			= this->GetLabel();

	PreviousBusy	= false;
	ControlsEnabled	= false;
	RefreshAll		= true;
	HadFocus		= nullptr;

	App->ShuttingDown	= false;

	App->SimulatorThread	= new wxCesarThread( &App->CPUState );

	if( App->SimulatorThread->Create() != wxTHREAD_NO_ERROR || App->SimulatorThread->Run()  != wxTHREAD_NO_ERROR )
		{
		delete App->SimulatorThread;
		App->SimulatorThread	= nullptr;
		}

	wxTextCtrl	*Hexa[9]		= { m_R0Hexa, m_R1Hexa, m_R2Hexa, m_R3Hexa, m_R4Hexa, m_R5Hexa, m_R6Hexa, m_R7Hexa, m_TECDT_Hexa };
	wxTextCtrl	*Unsigned[9]	= { m_R0Unsigned, m_R1Unsigned, m_R2Unsigned, m_R3Unsigned, m_R4Unsigned, m_R5Unsigned, m_R6Unsigned, m_R7Unsigned, m_TECDT_Unsigned };
	wxTextCtrl	*Signed[9]		= { m_R0Signed, m_R1Signed, m_R2Signed, m_R3Signed, m_R4Signed, m_R5Signed, m_R6Signed, m_R7Signed, m_TECDT_Signed };
	for( int i = 0; i < 9; i++ )
		{
		RegHexa[i]		= Hexa[i];
		RegUnsigned[i]	= Unsigned[i];
		RegSigned[i]	= Signed[i];
		}

	BkColour	= wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW );

	m_TECDT_Hexa->SetValue( "00h" );
	m_TECDT_Unsigned->SetValue( "0d" );
	m_TECDT_Signed->SetValue( "+0d" );
	m_TECDT_Char->SetValue( "." );

	if( !BitMapLCD.Create( 619, 32 ) || !BitMapLCD.IsOk() )
		{
		wxLogError( "Couldn't create the bitmap!" );
		return;
		}

	for( int i = 0; i < (int)LENGTH( Painted ); i++ )
		Painted[i]	= 1;

//	m_MemoryDump->SetRowHeight( 18 );

//	m_scrollBar1->SetRange( 0xffc );
//	m_scrollBar1->SetPageSize( 8 );
//	m_scrollBar1->SetThumbSize( 9 );

	m_Dump->SetBuffer( pCPUState->Memory, 0xffc0 );
	m_Dump->SetBitVectors( pCPUState->BreakPointsRead, pCPUState->BreakPointsWrite, pCPUState->MemoryUpdated, pCPUState->MemoryChanged );
	m_Disassembly->SetBitVector( pCPUState->BreakPointsExec );

	DumpStartAddress	= 0;
	LastMessageId		= 0;
	}
//==============================================================================
wxCesarFrame::~wxCesarFrame()
	{
	wxMutexLocker Locker( App->SimulatorMutex );

	App->ShuttingDown	= true;

//	App->SemaphoreAllDone.Wait();
	}
//==============================================================================
void wxCesarFrame::OnClose( wxCloseEvent &event )
	{
    Destroy();
	}
//==============================================================================
void wxCesarFrame::OnQuit( wxCommandEvent &event )
	{
    Destroy();
	}
//==============================================================================
void wxCesarFrame::DisableControls()
	{
	if( !ControlsEnabled )
		return;

	//m_Disassembly->SetLabel( "" );
	m_ButtonPause->Enable();

	wxWindow *HasFocus	= FindFocus();

	if( HasFocus == m_ButtonRun || HasFocus == m_ButtonAnimate || HasFocus == m_ButtonStepInto ||
		HasFocus == m_ButtonStepOver || HasFocus == m_ButtonMaxSpeed || HasFocus == m_ButtonClearTrace )
		{
		HadFocus	= HasFocus;
		m_ButtonPause->SetFocus();
		}
	else
		HadFocus	= nullptr;

	BkColour		= wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE );
	RefreshAll		= true;

	m_Dump->SetEditable( false );

	m_ButtonRun->Disable();
	m_ButtonAnimate->Disable();
	m_ButtonStepInto->Disable();
	m_ButtonStepOver->Disable();
	m_ButtonMaxSpeed->Disable();
	m_ButtonClearTrace->Disable();

	for( int i = 0; i < (int)LENGTH( RegHexa ) - 1; i++ )
		{
		RegHexa[i]->SetEditable( false );
		RegUnsigned[i]->SetEditable( false );
		RegSigned[i]->SetEditable( false );
		}
	m_TIMDT_Hexa->SetEditable( false );
	m_TIMDT_Unsigned->SetEditable( false );

	ControlsEnabled	= false;
	}
//==============================================================================
void wxCesarFrame::EnableControls()
	{
	if( ControlsEnabled )
		return;

	BkColour		= wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW );
	RefreshAll		= true;

	m_Dump->SetEditable( true );

	m_ButtonRun->Enable();
	m_ButtonAnimate->Enable();
	m_ButtonStepInto->Enable();
	m_ButtonStepOver->Enable();
	m_ButtonMaxSpeed->Enable();
	m_ButtonClearTrace->Enable();

	if( FindFocus() == m_ButtonPause && HadFocus != nullptr )
		HadFocus->SetFocus();

	m_ButtonPause->Disable();

	for( int i = 0; i < (int)LENGTH( RegHexa ) - 1; i++ )
		{
		RegHexa[i]->SetEditable( true );
		RegUnsigned[i]->SetEditable( true );
		RegSigned[i]->SetEditable( true );
		}
	m_TIMDT_Hexa->SetEditable( true );
	m_TIMDT_Unsigned->SetEditable( true );

	ControlsEnabled	= true;
	}
//==============================================================================
void wxCesarFrame::IdleLoop( wxIdleEvent& event )
	{
	wxMutexLocker	Locker( App->SimulatorMutex );

	if( pCPUState->Busy )
		{
		event.RequestMore( true );
		return;
		}

	if( RefreshAll || pCPUState->ChangedLCD )
		RefreshLCD();

	csimcpustate_t	&CPUState	= App->CPUState;

	if( CPUState.TimerOverRun )
		{
		CPUState.TimerOverRun	= 0;
		LastMessageId			= 6;
		m_statusBar->SetStatusText( "Timer interrupt overrun.", 0 );
		m_statusBar->SetBackgroundColour( *wxYELLOW );
		m_statusBar->Refresh();
		}
/*
	else if( CPUState.RunningState == RUNNINGSTATE_ANIMATING && LastMessageId == 6 )
		{
		LastMessageId			= 0;
		m_statusBar->SetStatusText( "", 0 );
		m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
		m_statusBar->Refresh();
		}
*/
	wxString	Text;

	switch( CPUState.SimulatorStatus )
		{
		case STATUS_ERROR_INTERNAL_ERROR:
			if( LastMessageId == 1 )
				break;
			LastMessageId	= 1;
			m_statusBar->SetStatusText( "Unknown internal error.", 0 );
			m_statusBar->SetBackgroundColour( *wxRED );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		case STATUS_ERROR_INVALID_INSTRUCTION:
			if( LastMessageId == 2 )
				break;
			Text.Printf( "Tried to execute an invalid instruction at h%04X = %ud.", CPUState.Registers.R7, CPUState.Registers.R7 );
			LastMessageId	= 2;
			m_statusBar->SetStatusText( Text, 0 );
			m_statusBar->SetBackgroundColour( *wxRED );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		case STATUS_ERROR_INVALID_ADDRESS:
			if( LastMessageId == 3 )
				break;
			Text.Printf( "Tried to access an invalid address at h%04X = %ud.", CPUState.Registers.R7, CPUState.Registers.R7 );
			LastMessageId	= 3;
			m_statusBar->SetStatusText( Text, 0 );
			m_statusBar->SetBackgroundColour( *wxRED );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		case STATUS_BREAKPOINT_EXEC:
			Text.Printf( "Reached execution breakpoint at h%04X = %ud.", CPUState.Registers.R7, CPUState.Registers.R7 );
			LastMessageId	= 7;
			m_statusBar->SetStatusText( Text, 0 );
			m_statusBar->SetBackgroundColour( *wxCYAN );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		case STATUS_BREAKPOINT_READ:
			Text.Printf( "Reached memory read breakpoint at h%04X = %ud.", CPUState.LastReadAccess, CPUState.Registers.R7 );
			LastMessageId	= 8;
			m_statusBar->SetStatusText( Text, 0 );
			m_statusBar->SetBackgroundColour( *wxCYAN );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		case STATUS_BREAKPOINT_WRITE:
			Text.Printf( "Reached memory write breakpoint at h%04X = %ud.", CPUState.LastWriteAccess, CPUState.Registers.R7 );
			LastMessageId	= 9;
			m_statusBar->SetStatusText( Text, 0 );
			m_statusBar->SetBackgroundColour( *wxCYAN );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		case STATUS_HALTED:
			if( LastMessageId == 4 )
				break;
			LastMessageId	= 4;
			m_statusBar->SetStatusText( "Executed instruction HLT.", 0 );
			m_statusBar->SetBackgroundColour( *wxCYAN );
			m_statusBar->Refresh();
			CPUState.SimulatorStatus	= STATUS_STOPPED;
			break;
		default:
			break;
		}

	if( !pCPUState->Running )
		EnableControls();

	if( RefreshAll || CPUState.ChangedStep || SomethingChanged )
		{
		if( RefreshAll || CPUState.RunningState < RUNNINGSTATE_RUNNINGFAST )
			{
			if( RefreshAll || CPUState.ChangedStep || CPUState.Registers.Regs[7] != CPUState.PreviousRegisters.Regs[7] )
				ShowDisassembly();

			for( int i = 0; i < 8; i++ )
				RefreshRegister( i, *wxRED, true );

			CheckFlags();
			CheckTIMDT();
			CheckINTE();
			CheckINTS();
			CheckTECST();
			CheckTECDT();
			}

		if( CPUState.ChangedMem || ( m_PreviousChangedMem && !CPUState.ChangedMem ))
			m_Dump->DumpRedraw();
		m_PreviousChangedMem	= CPUState.ChangedMem;
		CPUState.ChangedMem		= 0;

//		if( RefreshAll || ( CPUState.ChangedMem && CPUState.MemChangeAddressStart < DumpStartAddress + 144 && CPUState.MemChangeAddressEnd >= DumpStartAddress ))
//			m_MemoryDump->ShowMemory( DumpStartAddress, CPUState.Memory, 0xffc0 );

		CPUState.ChangedStep	= 0;
		SomethingChanged		= 0;
		RefreshAll				= false;
		}

	event.RequestMore( true );
	}
//==============================================================================
void wxCesarFrame::LoadMEMFile( wxCommandEvent& event )
	{
	wxFileDialog openFileDialog( this, _("Load MEM file"), "", "", "MEM files (*.mem)|*.mem", wxFD_OPEN | wxFD_FILE_MUST_EXIST );

	if( openFileDialog.ShowModal() == wxID_CANCEL )
		return;     // the user changed idea...

	FileName	= openFileDialog.GetPath();

	if( App->SimulatorThread->LoadFile( FileName ) < 0 )
		{
		wxMessageBox( wxT( "It was not possible to load file." ), wxT( "Error" ), wxICON_ERROR );
		return;
		}

	App->SimulatorThread->Reset();
	m_TECDT_Hexa->SetValue( "00h" );
	m_TECDT_Unsigned->SetValue( "0d" );
	m_TECDT_Signed->SetValue( "+0d" );
	m_TECDT_Char->SetValue( "." );
	SomethingChanged	= 1;
	RefreshAll			= 1;

	for( int i = 0; i < (int)LENGTH( Painted ); i++ )
		Painted[i]	= 1;

	SetTitle( Title + " - " + FileName );
	m_Dump->DumpRedraw();
	m_PreviousChangedMem	= false;

//	m_MemoryDump->ShowMemory( 0, wxGetApp().CPUState.Memory, 65500 );
	}
//==============================================================================
void wxCesarFrame::DontEraseBackGround( wxEraseEvent& event )
	{
	asm volatile( "nop" );
	}
//==============================================================================
void wxCesarFrame::RefreshLCD()
	{
#if         0 && defined __WXMSW__
	char	Buffer[36*2+1];
	char	Buffer2[36+1];
	int		i, j;

	App->SimulatorThread->GetLCDContents( Buffer2 );

	/* We must replace every '&' with '&&', because the ampersand has special meaning
	   in Windows and in wxWidgets. It is not displayed and causes the next character
	   to be shown underscored. The double ampersand is shown as just one normal ampersand. */
	for( i = 0, j = 0; i < 36; i++ )
		if( Buffer2[i] != '&' )
			Buffer[j++]	= Buffer2[i];
		else
			{
			Buffer[j++]	= '&';
			Buffer[j++]	= '&';
			}
	Buffer[j]	= '\0';
#else   //  defined __WXMSW__
	char	Buffer[36+1];
	App->SimulatorThread->GetLCDContents( Buffer );
#endif  //  defined __WXMSW__

	wxMemoryDC dc( BitMapLCD );
	dc.SetBackground( *wxGREEN_BRUSH );
	dc.SetFont( wxFont( 21, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT( "Courier New" )));
	dc.Clear();
	dc.DrawText( Buffer, 4, 0 );

	m_BitMapLCD->SetBitmap( BitMapLCD );
	m_BitMapLCD->Refresh( true );
	}
//==============================================================================
#define	PREVIOUS_LINES_TO_SHOW	 7
//#define	DISASSEMBLY_LINES		15
//==============================================================================
void wxCesarFrame::ShowDisassembly()
	{
	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	static const char * const Separators[]	=
		{
		"",
		";----------------------------------------------------",
		";====================================================",
		";****************************************************"
		};

	char		Buffer[50][100];
	uint16_t	Offset, OffsetN;
	int			Addresses[50];
	int			OurLine, PrevLines, NumLines, i, j, Skip		= 0;

	if( CPUState.Map == NULL )
		OffsetN	= Offset	= CPUState.Registers.R7;
	else
		{
		for( i = 0, OffsetN = Offset = CPUState.Registers.R7; OffsetN > 0 && i < PREVIOUS_LINES_TO_SHOW + 1 && CPUState.Map[OffsetN-1].Visited; )
			{
			OffsetN--;
			if( CPUState.Map[OffsetN].IsFirstByte )
				{
				Offset	= OffsetN;
				i++;
				}
			}
		}

	for( OurLine = -1, PrevLines = 0, NumLines = 0, i = 0; ( OurLine < 0 && NumLines < (int)LENGTH( Buffer )) || NumLines < DISASSEMBLY_LINES; )
		{
		symbol_t	*p;

		/* We have a symbol table and this address has a label... */
		if( CPUState.SymbolIndex != NULL && ( p = CPUState.SymbolIndex[Offset] ) != NULL )
			{
			int		Length, j;

			Length	= sprintf( Buffer[i], "%s", p->Name );
			for( j = 1; Length < 50 && j < 4 && Offset + j < RAM_SIZE && &p[j] - CPUState.SymbolTable < CPUState.NumSymbols && p[j].Address == Offset; j++ )
				Length += sprintf( &Buffer[i][Length], "=%s", p[j].Name );
			sprintf( &Buffer[i][Length], ":" );

			if( ++NumLines >= (int)LENGTH( Buffer ) && OurLine < 0 )
				NumLines	= (int)LENGTH( Buffer ) - 1;

			Addresses[i]	= -1;

			if( ++i >= (int)LENGTH( Buffer ))
				i			= 0;
			}

		/* This line is our current PC... (The test is '==', but we use '>=' just as a safeguard against some possible weird bug). */
		if( OurLine < 0 && Offset >= CPUState.Registers.R7 )
			{
			OurLine		= i;
			PrevLines	= std::min( NumLines, PREVIOUS_LINES_TO_SHOW );
			if( NumLines > PREVIOUS_LINES_TO_SHOW )
				NumLines	= PREVIOUS_LINES_TO_SHOW;
			}
/*
		if( CPUState.Map == NULL )
			{
			Buffer[i][0]	= ' ';
			Buffer[i][1]	= ' ';
			}
		else
			{
			if( !CPUState.Map[Offset].Executed )
				{
				Buffer[i][0]	= ' ';
				Buffer[i][1]	= ' ';
				}
			else
				{
				if( CPUState.Map[Offset].IntDisabled && CPUState.Map[Offset].IntEnabled )
					Buffer[i][0]	= '*';
				else if( CPUState.Map[Offset].IntEnabled )
					Buffer[i][0]	= '+';
				else
					Buffer[i][0]	= '-';

				if( CPUState.Map[Offset].InISR && CPUState.Map[Offset].InThread )
					Buffer[i][1]	= '#';
				else if( CPUState.Map[Offset].InISR )
					Buffer[i][1]	= 'i';
				else
					Buffer[i][1]	= 't';
				}
			}
*/
		Addresses[i]	= Offset;
		SimulatorDisassemble( &CPUState, &Offset, Buffer[i], sizeof Buffer[i], &Skip );

		if( ++NumLines >= (int)LENGTH( Buffer ) && OurLine < 0 )
			NumLines	= (int)LENGTH( Buffer ) - 1;

		if( ++i >= (int)LENGTH( Buffer ))
			i			= 0;

		if( Skip )
			{
			sprintf( Buffer[i], "%-*.*s", 50, 50, Separators[( Skip - 1 ) & 3] );
			Addresses[i]	= -2;

			if( ++NumLines >= (int)LENGTH( Buffer ) && OurLine < 0 )
				NumLines	= (int)LENGTH( Buffer ) - 1;

			if( ++i >= (int)LENGTH( Buffer ))
				i	= 0;
			}
		}

	/* This should not happen, but if it happens this is a safe workaround... */
	if( OurLine < 0 )
		OurLine	= 0;

	for( j = 0; j + PrevLines < PREVIOUS_LINES_TO_SHOW; j++ )
		m_Disassembly->SetLine( j, -2, 0, 0, "" );

	if(( i = OurLine - PrevLines ) < 0 )
		i  += LENGTH( Buffer );

	for( ; j < DISASSEMBLY_LINES; j++ )
		{
		int	Icon2	= 0;

		if( i == OurLine )
			Icon2	= 5;
		else if( CPUState.Map != nullptr && Addresses[i] >= 0 )
			Icon2	= ( CPUState.Map[Addresses[i]].InISR ? 1 : 0 ) + ( CPUState.Map[Addresses[i]].InThread ? 2 : 0 );

		m_Disassembly->SetLine( j, Addresses[i], 0, Icon2, Buffer[i] );

		if( ++i >= (int)LENGTH( Buffer ))
			i	= 0;
		}
	}
//==============================================================================
void wxCesarFrame::RefreshRegister( unsigned RegisterIndex, const wxColour &Colour, bool Forced )
	{
	uint16_t	Value;
	char		Str[16];

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	Value	= CPUState.Registers.Regs[RegisterIndex];

    sprintf( Str, "%04hXh", Value );
	RegHexa[RegisterIndex]->SetValue( Str );

    sprintf( Str, "%hud", Value );
	RegUnsigned[RegisterIndex]->SetValue( Str );

    sprintf( Str, "%+hdd", Value );
	RegSigned[RegisterIndex]->SetValue( Str );

	if( Forced || CPUState.Registers.Regs[RegisterIndex] != CPUState.PreviousRegisters.Regs[RegisterIndex] )
		{
		if( CPUState.Registers.Regs[RegisterIndex] != CPUState.PreviousRegisters.Regs[RegisterIndex] )
			{
			RegHexa[RegisterIndex]->SetBackgroundColour( Colour );
			RegUnsigned[RegisterIndex]->SetBackgroundColour( Colour );
			RegSigned[RegisterIndex]->SetBackgroundColour( Colour );
			}
		else if( CPUState.RegsUpdated[RegisterIndex] )
			{
			RegHexa[RegisterIndex]->SetBackgroundColour( *wxCYAN );
			RegUnsigned[RegisterIndex]->SetBackgroundColour( *wxCYAN );
			RegSigned[RegisterIndex]->SetBackgroundColour( *wxCYAN );
			}
		else
			{
			RegHexa[RegisterIndex]->SetBackgroundColour( BkColour );
			RegUnsigned[RegisterIndex]->SetBackgroundColour( BkColour );
			RegSigned[RegisterIndex]->SetBackgroundColour( BkColour );
			}
		}

	RegHexa[RegisterIndex]->Refresh();
	RegUnsigned[RegisterIndex]->Refresh();
	RegSigned[RegisterIndex]->Refresh();

	if( Forced )
		CPUState.PreviousRegisters.Regs[RegisterIndex]	= CPUState.Registers.Regs[RegisterIndex];
	}
//==============================================================================
void wxCesarFrame::UpdateRegister( unsigned RegisterIndex, uint16_t Value, bool Forced )
	{
	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	RegisterIndex  &= 7;

	if( Forced || CPUState.Registers.Regs[RegisterIndex] != Value )
		{
		if( RegisterIndex == 7 )
			RefreshAll	= true;
		}

	if( Forced )
		CPUState.Registers.Regs[RegisterIndex]			= Value;
	}
//==============================================================================
void wxCesarFrame::Update_RegisterHexa( wxTextCtrl *TextCtrl, int RegisterIndex, bool Forced )
	{
	wxString	Str;
	unsigned	Value;
	bool		OK	= false;

    Str					= TextCtrl->GetValue();

    int	i = 0;

	if( isxdigit( Str[i] ))
		{
		for( ; isxdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'H' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

    if( sscanf( Str, "%X", &Value ) != 1 )
		OK	= false;

	if( Value < 0 || Value > 65535 )
		OK	= false;

	if( OK )
		UpdateRegister( RegisterIndex, Value, Forced );

	RefreshRegister( RegisterIndex, *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_RegisterHexa( wxCommandEvent& event )
	{
	int			RegisterIndex;

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

    RegisterIndex		= (( event.GetId() - wcID_R0_Hexa ) / 3 ) & 7;

    Update_RegisterHexa( RegHexa[RegisterIndex], RegisterIndex, true );

	RegHexa[RegisterIndex+1]->SetFocus();
	}
//==============================================================================
void wxCesarFrame::Update_RegisterUnsigned( wxTextCtrl *TextCtrl, int RegisterIndex, bool Forced )
	{
	wxString	Str;
	unsigned	Value;
	bool		OK	= false;

    Str					= TextCtrl->GetValue();

    int	i = 0;

	if( isdigit( Str[i] ))
		{
		for( ; isdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'D' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

	if( sscanf( Str, "%u", &Value ) != 1 )
		OK	= false;

	if( Value < 0 || Value > 65535 )
		OK	= false;

	if( OK )
		UpdateRegister( RegisterIndex, Value, Forced );

	RefreshRegister( RegisterIndex, *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_RegisterUnsigned( wxCommandEvent& event )
	{
	int			RegisterIndex;

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

    RegisterIndex		= (( event.GetId() - wcID_R0_Hexa ) / 3 ) & 7;

    Update_RegisterUnsigned( RegUnsigned[RegisterIndex], RegisterIndex, true );

	RegUnsigned[RegisterIndex+1]->SetFocus();
	}
//==============================================================================
void wxCesarFrame::Update_RegisterSigned( wxTextCtrl *TextCtrl, int RegisterIndex, bool Forced )
	{
	wxString	Str;
	int			Value;
	bool		OK	= false;

    Str					= TextCtrl->GetValue();

    int	i = 0;

	if( Str[i] == '-' || Str[i] == '+' )
		i++;
	if( isdigit( Str[i] ))
		{
		for( ; isdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'D' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

    if( sscanf( Str, "%d", &Value ) != 1 )
		OK	= false;

	if( Value < -32768 || Value > 32767 )
		OK	= false;

	if( OK )
		UpdateRegister( RegisterIndex, Value, Forced );

	RefreshRegister( RegisterIndex, *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_RegisterSigned( wxCommandEvent& event )
	{
	int			RegisterIndex;

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

    RegisterIndex		= (( event.GetId() - wcID_R0_Hexa ) / 3 ) & 7;

    Update_RegisterSigned( RegSigned[RegisterIndex], RegisterIndex, true );

	RegSigned[RegisterIndex+1]->SetFocus();
	}
//==============================================================================
void wxCesarFrame::Evt_KillFocus( wxFocusEvent& event )
	{
	wxTextCtrl	*TextCtrl;
	int			RegisterIndex, GroupIndex;

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

    TextCtrl			= static_cast <wxTextCtrl*>( event.GetEventObject() );

    TextCtrl->SetSelection( 0, 0 );

	GroupIndex			= ( event.GetId() - wcID_R0_Hexa ) % 3;
    RegisterIndex		= (( event.GetId() - wcID_R0_Hexa ) / 3 ) & 7;

    switch( GroupIndex )
		{
		case 0:
			Update_RegisterHexa( TextCtrl, RegisterIndex, false );
			break;
		case 1:
			Update_RegisterUnsigned( TextCtrl, RegisterIndex, false );
			break;
		case 2:
			Update_RegisterSigned( TextCtrl, RegisterIndex, false );
			break;
		}
	}
//==============================================================================
void wxCesarFrame::Evt_SetFocusRegHex( wxFocusEvent& event )
	{
	wxTextCtrl	*TextCtrl;

    TextCtrl	= static_cast <wxTextCtrl*>( event.GetEventObject() );

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		{
		m_TECDT_Hexa->SetFocus();
		TextCtrl->SetSelection( 0, 0 );
		return;
		}

	HadFocus	= nullptr;

   	TextCtrl->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::Evt_SetFocusRegUns( wxFocusEvent& event )
	{
	wxTextCtrl	*TextCtrl;

    TextCtrl	= static_cast <wxTextCtrl*>( event.GetEventObject() );

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		{
		m_TECDT_Unsigned->SetFocus();
		TextCtrl->SetSelection( 0, 0 );
		return;
		}

	HadFocus	= nullptr;

   	TextCtrl->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::Evt_SetFocusRegSign( wxFocusEvent& event )
	{
	wxTextCtrl	*TextCtrl;

    TextCtrl	= static_cast <wxTextCtrl*>( event.GetEventObject() );

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		{
		m_TECDT_Signed->SetFocus();
		TextCtrl->SetSelection( 0, 0 );
		return;
		}

	HadFocus	= nullptr;

   	TextCtrl->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::Evt_SetFocus( wxFocusEvent& event )
	{
	wxTextCtrl	*TextCtrl;

    TextCtrl	= static_cast <wxTextCtrl*>( event.GetEventObject() );

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		{
		if( event.GetWindow() == m_ButtonPause )
			m_TECDT_Char->SetFocus();
		else
			m_ButtonPause->SetFocus();
		TextCtrl->SetSelection( 0, 0 );
		return;
		}

	HadFocus	= nullptr;

   	TextCtrl->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::Evt_SetFocusTECDT( wxFocusEvent& event )
	{
	wxTextCtrl	*TextCtrl;

	HadFocus	= nullptr;

    TextCtrl	= static_cast <wxTextCtrl*>( event.GetEventObject() );

   	TextCtrl->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::RefreshFlags( const wxColour &Colour, bool Forced )
	{
	static wxStaticText	*Controls[8]	= { m_Flags7, m_Flags6, m_Flags5, m_Flags4, m_FlagsN, m_FlagsZ, m_FlagsV, m_FlagsC };
	static const char	*Letters[8]		= { "*", "*", "*", "*", "N", "Z", "V", "C" };

	wxMutexLocker	Locker( App->SimulatorMutex );
	csimcpustate_t	&CPUState	= App->CPUState;
	uint8_t			ChangedMask	= CPUState.Registers.Flags ^ CPUState.PreviousRegisters.Flags;

	for( int i = 0; i < (int)LENGTH( Controls ); i++ )
		{
		Controls[i]->SetLabel(( CPUState.Registers.Flags >> ( 7 - i )) & 1 ? Letters[i] : "-" );
		if( Forced || (( ChangedMask >> ( 7 - i )) & 1 ))
			{
			if(( ChangedMask >> ( 7 - i )) & 1 )
				Controls[i]->SetBackgroundColour( Colour );
			else if( CPUState.FlagsUpdated )
				Controls[i]->SetBackgroundColour( *wxCYAN );
			else
				Controls[i]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_SCROLLBAR ));
			}
		Controls[i]->Refresh();
		}

	if( Forced )
		CPUState.PreviousRegisters.Flags	= CPUState.Registers.Flags;
	}
//==============================================================================
void wxCesarFrame::CheckFlags()
	{
	RefreshFlags( *wxRED, true );
	}
//==============================================================================
void wxCesarFrame::EvtClick_Flags( wxMouseEvent& event )
	{
	static const char	*Letters[8]		= { "*", "*", "*", "*", "N", "Z", "V", "C" };
	int					Bit;

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

    Bit		= 7 - (( event.GetId() - wcID_Flags_7 ) & 7 );

	CPUState.Registers.Flags		   ^= 1 << Bit;

	static_cast <wxStaticText*>( event.GetEventObject() )->SetLabel(( CPUState.Registers.Flags >>  Bit ) & 1 ? Letters[7-Bit] : "-" );
	static_cast <wxStaticText*>( event.GetEventObject() )->SetBackgroundColour( *wxYELLOW );

	RefreshFlags( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::RefreshTIMDT( const wxColour &Colour, bool Forced )
	{
	char	Str[16];
	uint8_t	Value;

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	if( Forced || CPUState.Registers.TIMDT != CPUState.PreviousRegisters.TIMDT )
		{
		if( CPUState.Registers.TIMDT != CPUState.PreviousRegisters.TIMDT )
			{
			m_TIMDT_Hexa->SetBackgroundColour( Colour );
			m_TIMDT_Unsigned->SetBackgroundColour( Colour );
			}
		else if( CPUState.TIMDTUpdated )
			{
			m_TIMDT_Hexa->SetBackgroundColour( *wxCYAN );
			m_TIMDT_Unsigned->SetBackgroundColour( *wxCYAN );
			}
		else
			{
			m_TIMDT_Hexa->SetBackgroundColour( BkColour );
			m_TIMDT_Unsigned->SetBackgroundColour( BkColour );
			}
		}

	Value	= CPUState.Registers.TIMDT;

    sprintf( Str, "%02hhXh", Value );
	m_TIMDT_Hexa->SetValue( Str );

    sprintf( Str, "%hhud", Value );
	m_TIMDT_Unsigned->SetValue( Str );

	m_TIMDT_Hexa->Refresh();
	m_TIMDT_Hexa->SetSelection( 0, 0 );
	m_TIMDT_Unsigned->Refresh();
	m_TIMDT_Unsigned->SetSelection( 0, 0 );

	if( Forced )
		CPUState.PreviousRegisters.TIMDT	= CPUState.Registers.TIMDT;
	}
//==============================================================================
void wxCesarFrame::CheckTIMDT()
	{
	RefreshTIMDT( *wxRED, true );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_TIMDT_Hexa( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	Update_TIMDTHexa();
	m_TIMDT_Hexa->SetSelection( -1, -1 );
	m_R0Hexa->SetFocus();
	}
//==============================================================================
void wxCesarFrame::EvtEnter_TIMDT_Unsigned( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	Update_TIMDTUnsigned();
	m_TIMDT_Unsigned->SetSelection( -1, -1 );
	m_R0Unsigned->SetFocus();
	}
//==============================================================================
void wxCesarFrame::UpdateTIMDT( uint8_t Value )
	{
	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	if( Value != CPUState.Registers.TIMDT )
		SimulatorUpdateTIMDT( &CPUState, Value );
	}
//==============================================================================
void wxCesarFrame::Update_TIMDTHexa()
	{
	wxString	Str;
	unsigned	Value;
	bool		OK	= false;

    Str		= m_TIMDT_Hexa->GetValue();

    int	i	= 0;

	if( isxdigit( Str[i] ))
		{
		for( ; isxdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'H' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

    if( sscanf( Str, "%X", &Value ) != 1 )
		OK	= false;

	if( Value < 0 || Value > 255 )
		OK	= false;

	if( OK )
		UpdateTIMDT( Value );

	RefreshTIMDT( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::Update_TIMDTUnsigned()
	{
	wxString	Str;
	unsigned	Value;
	bool		OK	= false;

    Str		= m_TIMDT_Unsigned->GetValue();

    int	i	= 0;

	if( isdigit( Str[i] ))
		{
		for( ; isdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'D' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

	if( sscanf( Str, "%u", &Value ) != 1 )
		OK	= false;

	if( Value < 0 || Value > 255 )
		OK	= false;

	if( OK )
		UpdateTIMDT( Value );

	RefreshTIMDT( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::Evt_KillFocusTIMDT( wxFocusEvent& event )
	{
	int			ControlIndex;

	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	ControlIndex		= ( event.GetId() - wcID_TIMDT_Hexa ) % 2;

    switch( ControlIndex )
		{
		case 0:
			m_TIMDT_Hexa->SetSelection( 0, 0 );
			Update_TIMDTHexa();
		case 1:
			m_TIMDT_Unsigned->SetSelection( 0, 0 );
			Update_TIMDTUnsigned();
		}

	RefreshTIMDT( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::RefreshINTE( const wxColour &Colour, bool Forced )
	{
	static wxStaticText	*Controls[8]	= { m_INTE_T, m_INTE_K, m_INTE_S, m_INTE_3, m_INTE_4, m_INTE_5, m_INTE_6, m_INTE_I };
	static const char	*Letters[8]		= { "T", "K", "S", "*", "*", "*", "*", "I" };

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	uint8_t	ChangedMask	= CPUState.Registers.INTE ^ CPUState.PreviousRegisters.INTE;

	for( int i = 0; i < (int)LENGTH( Controls ); i++ )
		{
		Controls[i]->SetLabel(( CPUState.Registers.INTE >> i ) & 1 ? Letters[i] : "-" );
		if( Forced || (( ChangedMask >> i ) & 1 ))
			{
			if(( ChangedMask >> i ) & 1 )
				Controls[i]->SetBackgroundColour( Colour );
			else if( CPUState.INTEUpdated )
				Controls[i]->SetBackgroundColour( *wxCYAN );
			else
				Controls[i]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_SCROLLBAR ));
			}
		Controls[i]->Refresh();
		}
	if( Forced )
		CPUState.PreviousRegisters.INTE		= CPUState.Registers.INTE;
	}
//==============================================================================
void wxCesarFrame::CheckINTE()
	{
	RefreshINTE( *wxRED, true );
	}
//==============================================================================
void wxCesarFrame::EvtClick_INTE( wxMouseEvent& event )
	{
	static const char	*Letters[8]	= { "T", "K", "S", "*", "*", "*", "*", "I" };
	int					Bit;

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

    Bit		= 7 - (( event.GetId() - wcID_INTE_7 ) & 7 );

	CPUState.Registers.INTE			   ^= 1 << Bit;

	static_cast <wxStaticText*>( event.GetEventObject() )->SetLabel( CPUState.Registers.INTE & ( 1 << Bit ) ? Letters[Bit] : "-" );

	RefreshINTE( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::RefreshINTS( const wxColour &Colour, bool Forced )
	{
	static wxStaticText	*Controls[8]	= { m_INTS_t, m_INTS_k, m_INTS_s, m_INTS_3, m_INTS_4, m_INTS_5, m_INTS_6, m_INTS_i};
	static const char	*Letters[8]		= { "t", "k", "s", "*", "*", "*", "*", "i" };

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	uint8_t	ChangedMask	= CPUState.Registers.INTS ^ CPUState.PreviousRegisters.INTS;

	for( int i = 0; i < (int)LENGTH( Controls ); i++ )
		{
		Controls[i]->SetLabel(( CPUState.Registers.INTS >> i ) & 1 ? Letters[i] : "-" );
		if( Forced || (( ChangedMask >> i ) & 1 ))
			{
			if(( ChangedMask >> i ) & 1 )
				Controls[i]->SetBackgroundColour( Colour );
			else if( CPUState.INTSUpdated )
				Controls[i]->SetBackgroundColour( *wxCYAN );
			else
				Controls[i]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_SCROLLBAR ));
			}
		Controls[i]->Refresh();
		}
	if( Forced )
		CPUState.PreviousRegisters.INTS		= CPUState.Registers.INTS;
	}
//==============================================================================
void wxCesarFrame::CheckINTS()
	{
	RefreshINTS( *wxRED, true );
	}
//==============================================================================
void wxCesarFrame::EvtClick_INTS( wxMouseEvent& event )
	{
	static const char	*Letters[8]	= { "t", "k", "s", "*", "*", "*", "*", "i" };
	int					Bit;

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	Bit		= 7 - (( event.GetId() - wcID_INTS_7 ) & 7 );

	CPUState.Registers.INTS			   ^= 1 << Bit;

	static_cast <wxStaticText*>( event.GetEventObject() )->SetLabel( CPUState.Registers.INTS & ( 1 << Bit ) ? Letters[Bit] : "-" );

	RefreshINTS( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::RefreshTECST( const wxColour &Colour, bool Forced )
	{
	static wxStaticText	*Controls[8]	= { m_TECST_0, m_TECST_1, m_TECST_2, m_TECST_3, m_TECST_4, m_TECST_5, m_TECST_6, m_TECST_A };
	static const char	*Letters[8]	= { "*", "*", "*", "*", "*", "*", "*", "A" };

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	uint8_t	ChangedMask	= CPUState.Registers.TECST ^ CPUState.PreviousRegisters.TECST;

	for( int i = 0; i < (int)LENGTH( Controls ); i++ )
		{
		Controls[i]->SetLabel(( CPUState.Registers.TECST >> i ) & 1 ? Letters[i] : "-" );
		if( Forced || (( ChangedMask >> i ) & 1 ))
			{
			if(( ChangedMask >> i ) & 1 )
				Controls[i]->SetBackgroundColour( Colour );
			else if( CPUState.TECSTUpdated )
				Controls[i]->SetBackgroundColour( *wxCYAN );
			else
				Controls[i]->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_SCROLLBAR ));
			}
		Controls[i]->Refresh();
		}

	if(( ChangedMask & 0x80 ) != 0 && ( CPUState.Registers.TECST & 0x80 ) == 0 )
		RefreshTECDT( *wxYELLOW, true );

	if( Forced )
		{
//@@@@		CPUState.PreviousRegisters.TECDT	= CPUState.Registers.TECDT;
		CPUState.PreviousRegisters.TECST	= CPUState.Registers.TECST;
		}

	}
//==============================================================================
void wxCesarFrame::CheckTECST()
	{
	RefreshTECST( *wxRED, true );
	}
//==============================================================================
void wxCesarFrame::EvtClick_TECST( wxMouseEvent& event )
	{
	static const char	*Letters[8]	= { "*", "*", "*", "*", "*", "*", "*", "A" };
	int					Bit;

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

    Bit		= 7 - (( event.GetId() - wcID_TECST_7 ) & 7 );

	CPUState.Registers.TECST		   ^= 1 << Bit;

	static_cast <wxStaticText*>( event.GetEventObject() )->SetLabel( CPUState.Registers.TECST & ( 1 << Bit ) ? Letters[Bit] : "-" );

	if( Bit == 7 )
		{
		CPUState.PreviousRegisters.TECDT	= CPUState.Registers.TECDT;
		RefreshTECDT( *wxYELLOW, true );
		if( LastMessageId == 5 )
			{
			LastMessageId	= 0;
			m_statusBar->SetStatusText( "", 0 );
			m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
			m_statusBar->Refresh();
			}
		}

	RefreshTECST( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_TECDT_Hexa( wxCommandEvent& event )
	{
	Update_TECDTHexa( true );	/* The input was caused by ENTER, we must force the generation of a keystroke. */
	m_TECDT_Hexa->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_TECDT_Unsigned( wxCommandEvent& event )
	{
	Update_TECDTUnsigned( true );	/* The input was caused by ENTER, we must force the generation of a keystroke. */
	m_TECDT_Unsigned->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::EvtEnter_TECDT_Signed( wxCommandEvent& event )
	{
	Update_TECDTSigned( true );	/* The input was caused by ENTER, we must force the generation of a keystroke. */
	m_TECDT_Signed->SetSelection( -1, -1 );
	}
//==============================================================================
void wxCesarFrame::EvtOnChar_TECDT_Char( wxKeyEvent& event )
	{
	wxString	Str;
	wchar_t		c;

	wxMutexLocker Locker( App->SimulatorMutex );

	c					= event.GetKeyCode();

    if( c == '\t' )
		{
		m_TECDT_Char->SetSelection( 0, 0 );
		if( event.GetModifiers() & wxMOD_SHIFT )
			m_TECDT_Signed->SetFocus();
		else
			m_TIMDT_Hexa->SetFocus();
		}
    else if( isascii( c ))
		{
		UpdateTECDT( c, true );
		RefreshTECDT( *wxYELLOW, true );
		m_TECDT_Char->SetSelection( -1, -1 );
		}
	}
//==============================================================================
void wxCesarFrame::RefreshTECDT( const wxColour &Colour, bool Forced )
	{
	char	Str[16];
	uint8_t	Value;

	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t &CPUState	= App->CPUState;

	if( Forced || CPUState.Registers.TECDT != CPUState.PreviousRegisters.TECDT )
		{
		if( CPUState.Registers.TECDT != CPUState.PreviousRegisters.TECDT )
			{
			m_TECDT_Hexa->SetBackgroundColour( Colour );
			m_TECDT_Unsigned->SetBackgroundColour( Colour );
			m_TECDT_Signed->SetBackgroundColour( Colour );
			m_TECDT_Char->SetBackgroundColour( Colour );
			}
		else
			{
			m_TECDT_Hexa->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ));
			m_TECDT_Unsigned->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ));
			m_TECDT_Signed->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ));
			m_TECDT_Char->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ));
			}
		}

	if( Forced )
		{
		Value	= CPUState.Registers.TECDT;

		sprintf( Str, "%02hhXh", Value );
		m_TECDT_Hexa->SetValue( Str );

		sprintf( Str, "%hhud", Value );
		m_TECDT_Unsigned->SetValue( Str );

		sprintf( Str, "%+hhdd", Value );
		m_TECDT_Signed->SetValue( Str );

		if( isprint( Value ))
			{
			char	s[2];
			s[0]	= Value;
			s[1]	= '\0';
			m_TECDT_Char->SetValue( s );
			}
		else
			m_TECDT_Char->SetValue( "." );
		}
	m_TECDT_Hexa->Refresh();
	m_TECDT_Unsigned->Refresh();
	m_TECDT_Signed->Refresh();
	m_TECDT_Char->Refresh();
	}
//==============================================================================
void wxCesarFrame::CheckTECDT()
	{
	RefreshTECDT( *wxYELLOW, false );
	}
//==============================================================================
void wxCesarFrame::UpdateTECDT( uint8_t Value, bool Forced )
	{
	wxMutexLocker Locker( App->SimulatorMutex );
	csimcpustate_t	&CPUState	= App->CPUState;

	if( Forced || Value != CPUState.Registers.TECDT )
		{
		CPUState.PreviousRegisters.TECDT	= ~CPUState.Registers.TECDT;

		if( KeyboardInput( &CPUState, Value ) && LastMessageId != 5 )
			{
			LastMessageId	= 5;
			m_statusBar->SetStatusText( "Keyboard buffer overrun.", 0 );
			m_statusBar->SetBackgroundColour( *wxYELLOW );
			m_statusBar->Refresh();
			}
		RefreshTECST( *wxRED, false );
		RefreshINTS( *wxRED, false );
		}
	}
//==============================================================================
void wxCesarFrame::Update_TECDTHexa( bool Forced )
	{
	wxString	Str;
	unsigned	Value;
	bool		OK	= false;

    Str		= m_TECDT_Hexa->GetValue();

    int	i	= 0;

	if( isxdigit( Str[i] ))
		{
		for( ; isxdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'H' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

    if( sscanf( Str, "%X", &Value ) != 1 )
		OK	= false;

	if( Value < 0 || Value > 255 )
		OK	= false;

	if( OK )
		UpdateTECDT( Value, Forced );

	RefreshTECDT( *wxYELLOW, true );
	}
//==============================================================================
void wxCesarFrame::Update_TECDTUnsigned( bool Forced )
	{
	wxString	Str;
	unsigned	Value;
	bool		OK	= false;

    Str		= m_TECDT_Unsigned->GetValue();

    int	i	= 0;

	if( isdigit( Str[i] ))
		{
		for( ; isdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'D' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

	if( sscanf( Str, "%u", &Value ) != 1 )
		OK	= false;

	if( Value < 0 || Value > 255 )
		OK	= false;

	if( OK )
		UpdateTECDT( Value, Forced );

	RefreshTECDT( *wxYELLOW, true );
	}
//==============================================================================
void wxCesarFrame::Update_TECDTSigned( bool Forced )
	{
	wxString	Str;
	int			Value;
	bool		OK	= false;

    Str		= m_TECDT_Signed->GetValue();

    int	i	= 0;

	if( Str[i] == '-' || Str[i] == '+' )
		i++;
	if( isdigit( Str[i] ))
		{
		for( ; isdigit( Str[i] ); i++ )
			{}
		if( toupper( Str[i] ) == 'D' )
			Str[i++]	= '\0';
		if( Str[i] == '\0' )
			OK	= true;
		}

    if( sscanf( Str, "%d", &Value ) != 1 )
		OK	= false;

	if( Value < -128 || Value > 127 )
		OK	= false;

	if( OK )
		UpdateTECDT( Value, Forced );

	RefreshTECDT( *wxYELLOW, true );
	}
//==============================================================================
void wxCesarFrame::Evt_KillFocusTECDT( wxFocusEvent& event )
	{
	int			ControlIndex;

	ControlIndex		= ( event.GetId() - wcID_TECDT_Hexa ) % 4;

    switch( ControlIndex )
		{
		case 0:
			m_TECDT_Hexa->SetSelection( 0, 0 );
			Update_TECDTHexa( false );	/* The input was caused by TAB or CLICK, we generate a keystroke only if the value is valid and new. */
			break;
		case 1:
			m_TECDT_Unsigned->SetSelection( 0, 0 );
			Update_TECDTUnsigned( false );	/* The input was caused by TAB or CLICK, we generate a keystroke only if the value is valid and new. */
			break;
		case 2:
			m_TECDT_Signed->SetSelection( 0, 0 );
			Update_TECDTSigned( false );	/* The input was caused by TAB or CLICK, we generate a keystroke only if the value is valid and new. */
			break;
		case 3:
			m_TECDT_Char->SetSelection( 0, 0 );
			m_TECDT_Char->SetSelection( 0, 0 );
			break;	/* The input was caused by TAB or CLICK, we generate a keystroke only if the value is valid and new. */
		}

	RefreshTECDT( *wxYELLOW, true );
	}
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorPause( wxCommandEvent& event )
	{
	App->SimulatorThread->Pause();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorRun( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	if( FindFocus() == m_ButtonRun )
		HadFocus	= m_ButtonRun;
	else
		HadFocus	= nullptr;

	DisableControls();
	LastMessageId	= 0;
	m_statusBar->SetStatusText( "", 0 );
	m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_statusBar->Refresh();
	App->SimulatorThread->Go();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatotAnimate( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	if( FindFocus() == m_ButtonAnimate )
		HadFocus	= m_ButtonAnimate;
	else
		HadFocus	= nullptr;

	DisableControls();
	LastMessageId	= 0;
	m_statusBar->SetStatusText( "", 0 );
	m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_statusBar->Refresh();
	App->SimulatorThread->Animate();
	}
//==============================================================================
void wxCesarFrame::EvtClick_RunMaxSpeed( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	if( FindFocus() == m_ButtonMaxSpeed )
		HadFocus	= m_ButtonMaxSpeed;
	else
		HadFocus	= nullptr;

	DisableControls();
	LastMessageId	= 0;
	m_statusBar->SetStatusText( "", 0 );
	m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_statusBar->Refresh();
	App->SimulatorThread->MaxSpeed();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorStepInto( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	LastMessageId	= 0;
	m_statusBar->SetStatusText( "", 0 );
	m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_statusBar->Refresh();
	App->SimulatorThread->StepInto();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorStepOver( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	LastMessageId	= 0;
	m_statusBar->SetStatusText( "", 0 );
	m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_statusBar->Refresh();
	if( App->SimulatorThread->StepOver() )
		DisableControls();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorFaster( wxCommandEvent& event )
	{
	if( !App->SimulatorThread->SpeedUp() )
		{
		m_ButtonSlowDown->SetFocus();
		m_ButtonSpeedUp->Disable();
		}
	m_ButtonSlowDown->Enable();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorSlower( wxCommandEvent& event )
	{
	if( !App->SimulatorThread->SlowDown() )
		{
		m_ButtonSpeedUp->SetFocus();
		m_ButtonSlowDown->Disable();
		}
	m_ButtonSpeedUp->Enable();
	}
//==============================================================================
void wxCesarFrame::EvtClick_SimulatorReset( wxCommandEvent& event )
	{
	RefreshAll	= true;

	App->SimulatorThread->Reset();
	m_TECDT_Hexa->SetValue( "00h" );
	m_TECDT_Unsigned->SetValue( "0d" );
	m_TECDT_Signed->SetValue( "+0d" );
	m_TECDT_Char->SetValue( "." );
	SomethingChanged	= 1;
	RefreshAll			= 1;

	for( int i = 0; i < (int)LENGTH( Painted ); i++ )
		Painted[i]		= 1;

	LastMessageId	= 0;
	m_statusBar->SetStatusText( "", 0 );
	m_statusBar->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	m_statusBar->Refresh();

	m_PreviousChangedMem	= false;

	EnableControls();
	}
//==============================================================================
void wxCesarFrame::EvtClick_ClearTrace( wxCommandEvent& event )
	{
	if( pCPUState->RunningState != RUNNINGSTATE_STOPPED )
		return;

	App->SimulatorThread->ClearTrace();
	SomethingChanged	= 1;
	RefreshAll			= 1;
	}
//==============================================================================
void wxCesarFrame::evtScroll_MemoryDump( wxScrollEvent& event )
	{
	DumpStartAddress	= 16 * event.GetPosition();
//	m_MemoryDump->ShowMemory( DumpStartAddress, wxGetApp().CPUState.Memory, 65500 );
	}
//==============================================================================
void wxCesarFrame::ShowStatusBarMessage( int Code, const wxString &Message, const wxColour &Colour )
	{
	LastMessageId	= Code;
	m_statusBar->SetStatusText( Message, 0 );
	m_statusBar->SetBackgroundColour( Colour );
	m_statusBar->Refresh();
	}
//==============================================================================
void wxCesarFrame::ClearStatusBarMessage( int Code )
	{
	if( Code == 0 || LastMessageId == Code )
		ShowStatusBarMessage( 0, "", wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
	}
//==============================================================================
static bool ValidateHexa( const wxString &Str )
	{
	int	Length	= Str.Length();

	if( Length < 1 || Length > 2 )
		return false;

	for( int i = 0; i < Length; i++ )
		if( !isxdigit( Str[i] ))
			return false;

	return true;
	}
//==============================================================================
#if 0
void wxCesarFrame::ValueChanged( wxDataViewEvent& event )
	{
    event.Skip();
    return;

	int			Column	= event.GetColumn();

	if( Column < 1 || Column > 16 )
		return;

	wxString	Str	= event.GetValue();

	if( ValidateHexa( Str ))
		ClearStatusBarMessage( 7 );
	else
		{
		event.Veto();
		ShowStatusBarMessage( 7, "Invalid Hexadecimal Value", *wxRED );
		}

    event.Skip();
	}
#endif
//==============================================================================
