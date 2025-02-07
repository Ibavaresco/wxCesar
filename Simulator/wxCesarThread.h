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
#if			!defined __WXCESARTHREAD_H__
#define __WXCESARTHREAD_H__
//==============================================================================
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/thread.h"
#include "wxCesarSimulator.h"
//==============================================================================
class wxCesarThread : public wxThread
	{
	//--------------------------------------------------------------------------
	public:
						wxCesarThread	( csimcpustate_t *cpustate );// : CPUState( cpustate ) {};
		virtual			~wxCesarThread	();
	//--------------------------------------------------------------------------
	public:
		// thread execution starts here
		virtual void	*Entry			() override;
	//--------------------------------------------------------------------------
	private:
		csimcpustate_t *CPUState;
	//--------------------------------------------------------------------------
	public:
		typedef char	lcd_t[36];
	//--------------------------------------------------------------------------
	public:
		int				LoadFile		( const wxString &Path );
		int				GetCPUState		( csimcpustate_t &state );
		int				GetLCDContents	( char *Buffer );
		void			SetMemoryWindow	( uint16_t startaddress, uint16_t endaddress );
	//--------------------------------------------------------------------------
		int				Reset			();
		int				ClearTrace		();
		int				Go				();
		int				Pause			();
		int				Animate			();
		int				MaxSpeed		();
		int				StepInto		();
		int				StepOver		();
		bool			SpeedUp			();
		bool			SlowDown		();
	//--------------------------------------------------------------------------
		int				CurrentSpeed;
		static const int	Delays[17];
	//--------------------------------------------------------------------------
	};
//==============================================================================
#endif	//	!defined __WXCESARTHREAD_H__
//==============================================================================
