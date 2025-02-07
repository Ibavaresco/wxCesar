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
#if			!defined __WXCESARAPP_H__
#define __WXCESARAPP_H__
/*============================================================================*/
#include <wx/app.h>
#include "wxCesarThread.h"
/*============================================================================*/
class wxCesarApp : public wxApp
	{
    public:
							wxCesarApp			();
        virtual bool		OnInit				();

		wxMutex				SimulatorMutex;
		wxCesarThread		*SimulatorThread;
		wxSemaphore			SemaphoreAllDone;
		bool				ShuttingDown;

		csimcpustate_t		CPUState;
	};
/*============================================================================*/
#endif	//	!defined __WXCESARAPP_H__
/*============================================================================*/
