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
#if			!defined __WXCESARSIMULATOR_H__
#define __WXCESARSIMULATOR_H__
/*============================================================================*/
#ifdef	__cplusplus
extern "C"
	{
#endif
/*============================================================================*/
#include <stdint.h>
#include "BitVector.h"
/*============================================================================*/
#define	MEMORY_SIZE	   0x10000	/* This is the total memory that the processor can address. */
#define	RAM_SIZE		0xffc0	/* This is the total amount of real RAM. */

#define	ADDRESS_LCD		0xffdc

#define	LENGTH_LCD		    36
/*============================================================================*/
#define LENGTH(a)	   ( sizeof (a) / sizeof (a)[0] )
/*============================================================================*/
typedef struct
	{
	union
		{
		uint16_t		Regs[8];
		struct
			{
			uint16_t	R0;
			uint16_t	R1;
			uint16_t	R2;
			uint16_t	R3;
			uint16_t	R4;
			uint16_t	R5;
			uint16_t	R6;
			uint16_t	R7;
			};
		};
	union
		{
		uint8_t			Flags;
		struct
			{
			uint8_t     C : 1;
			uint8_t     V : 1;
			uint8_t     Z : 1;
			uint8_t     N : 1;
			uint8_t		  : 4;
			};
		};
	union
		{
		uint8_t			INTE;
		struct
			{
			uint8_t		T : 1;
			uint8_t		K : 1;
			uint8_t		S : 1;	/* This is an addition by myself, a "software interupt". */
			uint8_t		  : 4;
			uint8_t		I : 1;
			};
		};
	union
		{
		uint8_t			INTS;
		struct
			{
			uint8_t		t : 1;
			uint8_t		k : 1;
			uint8_t		s : 1;
			uint8_t		  : 4;
			uint8_t		i : 1;
			};
		};
	union
		{
		uint8_t			TECST;
		struct
			{
			uint8_t		  : 7;
			uint8_t		A : 1;
			};
		};
	uint8_t				TECDT;
	uint8_t				TIMDT;
	} csimregisters_t;
/*============================================================================*/
typedef struct
	{
	uint16_t			Address;
	uint16_t			Type;
	char				Name[30];
	} symbol_t;
/*============================================================================*/
typedef enum
	{
	RUNNINGSTATE_STOPPED,
	RUNNINGSTATE_SINGLESTEP,
	RUNNINGSTATE_ANIMATING,
	RUNNINGSTATE_RUNNING,
	RUNNINGSTATE_RUNNINGFAST,
	RUNNINGSTATE_STEPOVER,
	RUNNINGSTATE_PROFILING
	} runningstates_t;
/*============================================================================*/
typedef struct
	{
	uint16_t			Visited			: 1;
	uint16_t			IsCode			: 1;
	uint16_t			IsInstruction	: 1;
	uint16_t			IsFirstByte		: 1;
	uint16_t			Executed		: 1;
	uint16_t			InISR			: 1;
	uint16_t			InThread		: 1;
	uint16_t			IntDisabled		: 1;
	uint16_t			IntEnabled		: 1;
	} map_t;
/*============================================================================*/
typedef enum
	{
	STATUS_ERROR_INTERNAL_ERROR			= -7,
	STATUS_ERROR_INVALID_INSTRUCTION	= -6,
	STATUS_ERROR_INVALID_ADDRESS		= -5,
	STATUS_BREAKPOINT_WRITE				= -4,
	STATUS_BREAKPOINT_READ				= -3,
	STATUS_BREAKPOINT_EXEC				= -2,
	STATUS_HALTED						= -1,
	STATUS_STOPPED,
	STATUS_TO_CONTINUE
	} csimstatuscodes_t;
/*============================================================================*/
typedef struct
	{
	csimstatuscodes_t	SimulatorStatus;
	int					CPUStateOK;
	int					Busy;
	int					Running;
	csimregisters_t		Registers;
	csimregisters_t		PreviousRegisters;
	uint8_t				RegsUpdated[8];
	uint8_t             FlagsUpdated;
	uint8_t             INTEUpdated;
	uint8_t				INTSUpdated;
	uint8_t				TECSTUpdated;
	uint8_t				TIMDTUpdated;
	bitvector_t			*MemoryUpdated;
	bitvector_t			*MemoryChanged;
	bitvector_t			*BreakPointsRead;
	bitvector_t			*BreakPointsWrite;
	bitvector_t			*BreakPointsExec;
	uint16_t			MemWindowStart;
	uint16_t			MemWindowEnd;
	uint16_t			LastReadAccess;
	uint16_t			LastWriteAccess;
	uint8_t				*Memory;
	uint8_t				*PristineMemory;
	symbol_t			*SymbolTable;
	symbol_t			**SymbolIndex;
	map_t				*Map;
	uint16_t			*ProfilerStack;
	int					ProfilerStackPointer;
	size_t				NumSymbols;
	int					Pedantic;
	unsigned			Latency;
	int					JSRCount;
	uint16_t			StepOverR6;
	uint16_t			StepOverR7;
	int					RunningState;
	int					TimerOverRun;
	int					KeyboardOverRun;
	uint32_t			InsnCount;
	int					Delay;
	uint32_t			t1;
	uint32_t			TimerTick;
	int					ChangedStep;
	int					ChangedMem;
	int					ChangedLCD;
	} csimcpustate_t;
/*============================================================================*/
int					SimulatorGetLCDContents		( csimcpustate_t *CPUState, char *Buffer );
int					SimulatorInitialize			( csimcpustate_t *CPUState );
int					SimulatorFinalize			( csimcpustate_t *CPUState );
int					SimulatorLoadFile			( csimcpustate_t *CPUState, const char * const FileName );
int					SimulatorLoadSymbolTable	( csimcpustate_t *CPUState, const char * const FileName );
int					SimulatorDoProfile			( csimcpustate_t *CPUState );
csimstatuscodes_t	SimulatorLoop				( csimcpustate_t *CPUState );
void				SimulatorClearTraceHistory	( csimcpustate_t *CPUState );
int					LoadSymbolTable				( csimcpustate_t *CPUState, const char * const FileName );
int					SimulatorDisassemble		( csimcpustate_t *CPUState, uint16_t *Offset, char * const Buffer, size_t BufferSize, int *Skip );
int					KeyboardInput				( csimcpustate_t *CPUState, uint8_t Char );

int					SimulatorResetCPU			( csimcpustate_t *CPUState );
void				SimulatorUpdateTIMDT		( csimcpustate_t *CPUState, uint8_t Value );
void				SimulatorRun				( csimcpustate_t *CPUState );
void				SimulatorPause				( csimcpustate_t *CPUState );
void				SimulatorMaxSpeed			( csimcpustate_t *CPUState );
void				SimulatorStepInto			( csimcpustate_t *CPUState );
int					SimulatorStepOver			( csimcpustate_t *CPUState );
void				SimulatorClearTrace			( csimcpustate_t *CPUState );
void				SimulatorAnimate			( csimcpustate_t *CPUState );
void				SimulatorSetMemWindow		( csimcpustate_t *CPUState, uint16_t MemWindowStart, uint16_t MemWindowEnd );
/*============================================================================*/
#ifdef	__cplusplus
	}
#endif
/*============================================================================*/
#endif	//	!defined __WXCESARSIMULATOR_H__
/*============================================================================*/
