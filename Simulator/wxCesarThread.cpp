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
#include "wxCesarApp.h"
#include "wxCesarThread.h"
//==============================================================================
wxDECLARE_APP( wxCesarApp );
/*============================================================================*/
const int	wxCesarThread::Delays[17]	=
	{
	2512, 1585, 1000, 631, 398, 251, 158, 100, 63, 40, 25, 16, 10, 6, 4, 2, 1
	};
/*============================================================================*/
wxCesarThread::wxCesarThread( csimcpustate_t *cpustate ) : wxThread(), CPUState( cpustate )
	{
	SimulatorInitialize( CPUState );
	SimulatorResetCPU( CPUState );
	CPUState->ChangedStep	= 1;
	CurrentSpeed			= 2;
	CPUState->Delay			= Delays[CurrentSpeed];
	}
//==============================================================================
wxCesarThread::~wxCesarThread()
	{
//	auto	&App	= wxGetApp();

	wxMutexLocker Locker( wxGetApp().SimulatorMutex );

	// signal the main thread that there are no more threads left if it is
	// waiting for us
	if( wxGetApp().ShuttingDown )
		{
		wxGetApp().ShuttingDown = false;
		wxGetApp().SemaphoreAllDone.Post();
		}
	}
//==============================================================================
wxThread::ExitCode wxCesarThread::Entry()
	{
	// Setting thread name helps with debugging, as the debugger
	// may be able to show thread names along with the list of threads.
	SetName( "wxCesarThread" );

	wxLogMessage( "Thread started (priority = %u).", GetPriority() );

	while( 1 )
		{
		// This construct may seem weird but it is necessary. We could remove the
		// 'do' and 'while(0)', but the braces must remain, because the local variable
		// 'Locker' must be created and destroyed regularly. It is an 'object' that
		// takes and releases a 'Mutex' in its constructor and destructor. Without
		// that the synchronization between the main application and the thread would
		// freeze.
		do
			{
			wxMutexLocker Locker( wxGetApp().SimulatorMutex );
			if( wxGetApp().ShuttingDown )
				return nullptr;

			// Check if this thread was asked to exit.
			if( TestDestroy() )
				break;

			SimulatorLoop( CPUState );
			}
		while( 0 );
		}

	wxLogMessage( "Thread finished." );

	return nullptr;
	}
//==============================================================================
int wxCesarThread::LoadFile( const wxString &Path )
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );

	SimulatorLoadFile( CPUState, Path );
	SimulatorLoadSymbolTable( CPUState, Path );
	SimulatorResetCPU( CPUState );
	SimulatorDoProfile( CPUState );
	SimulatorResetCPU( CPUState );
	CPUState->ChangedStep	= 1;
	CPUState->Delay			= Delays[CurrentSpeed];

	return 0;
	}
//==============================================================================
int wxCesarThread::GetCPUState( csimcpustate_t &state )
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );

	memcpy( &state, CPUState, sizeof state );

	return 0;
	}
//==============================================================================
int wxCesarThread::GetLCDContents( char *Buffer )
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );

	memcpy( Buffer, &CPUState->Memory[ADDRESS_LCD], 36 );
	Buffer[36]	= '\0';

	CPUState->ChangedLCD	= 0;
	return 0;
	}
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
int wxCesarThread::Reset()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorResetCPU( CPUState );
	CPUState->ChangedStep	= 1;
	CPUState->Delay			= Delays[CurrentSpeed];
	return 0;
	}
//==============================================================================
int wxCesarThread::ClearTrace()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorClearTrace( CPUState );
	return 0;
	}
//==============================================================================
int wxCesarThread::Go()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorRun( CPUState );
	return 0;
	}
//==============================================================================
int wxCesarThread::Pause()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorPause( CPUState );
	return 0;
	}
//==============================================================================
int wxCesarThread::Animate()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorAnimate( CPUState );
	return 0;
	}
//==============================================================================
int wxCesarThread::MaxSpeed()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorMaxSpeed( CPUState );
	return 0;
	}
//==============================================================================
int wxCesarThread::StepInto()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorStepInto( CPUState );
	return 0;
	}
//==============================================================================
int wxCesarThread::StepOver()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	return SimulatorStepOver( CPUState );
	}
//==============================================================================
bool wxCesarThread::SpeedUp()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	if( ++CurrentSpeed >= (int)LENGTH( Delays ))
		CurrentSpeed	= LENGTH( Delays ) - 1;

	CPUState->Delay	= Delays[CurrentSpeed];

	return CurrentSpeed < (int)LENGTH( Delays ) - 1;
	}
//==============================================================================
bool wxCesarThread::SlowDown()
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	if( --CurrentSpeed < 0 )
		CurrentSpeed	= 0;

	CPUState->Delay	= Delays[CurrentSpeed];

	return CurrentSpeed > 0;
	}
//==============================================================================
void wxCesarThread::SetMemoryWindow( uint16_t startaddress, uint16_t endaddress )
	{
	wxMutexLocker Locker( wxGetApp().SimulatorMutex );
	SimulatorSetMemWindow( CPUState, startaddress, endaddress );
	}
//==============================================================================
