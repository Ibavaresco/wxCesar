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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "wxCesarSimulator.h"
/*============================================================================*/
#define	ADDRESS_IVET	0xffbe
#define	ADDRESS_TIMDT	0xffd7
#define	ADDRESS_INTS	0xffd8
#define	ADDRESS_INTE	0xffd9
#define	ADDRESS_TECST	0xffda
#define	ADDRESS_TECDT	0xffdb
/*============================================================================*/
#if         defined __WXMSW__
#include <windows.h>
#define PATH_SEPARATOR      '\\'
#define PATH_SEPARATOR_STR  "\\"
#else   //  defined __WXMSW__
#define PATH_SEPARATOR      '/'
#define PATH_SEPARATOR_STR  "/"
#endif  //  defined __WXMSW__
/*============================================================================*/
static void CSIMSplitPath( const char *pFullPath, char *pPath, char *pName, char *pExt )
	{
	char	FullPath[256], *p, *q;

	strncpy( FullPath, pFullPath, sizeof FullPath );
	FullPath[sizeof FullPath - 1]	= '\0';

	if(( p = strrchr( FullPath, PATH_SEPARATOR )) != NULL )
		{
		*p++	= '\0';
		if( pPath != NULL )
			{
			strcpy( pPath, FullPath );
			strcat( pPath, PATH_SEPARATOR_STR );
			}
		}
#if         defined __WXMSW__
	else if(( p = strrchr( FullPath, ':' )) != NULL )
		{
		*p++	= '\0';
		if( pPath != NULL )
			{
			strcpy( pPath, FullPath );
			strcat( pPath, ":" );
			}
		}
#endif  //  defined __WXMSW__
	else
		{
		p		= FullPath;
		if( pPath != NULL )
			strcpy( pPath, "" );
		}

	if(( q = strrchr( p, '.' )) != NULL )
		{
		if( pExt != NULL )
			strcpy( pExt, q );
		*q		= '\0';
		}
	else if( pExt != NULL )
		strcpy( pExt, "" );

	if( pName != NULL )
		strcpy( pName, p );
	}
/*============================================================================*/
static int CSIMCompareSymbols( const void *a, const void *b )
	{
	symbol_t	*A	= (symbol_t*)a;
	symbol_t	*B	= (symbol_t*)b;

	/* Order by address, if the addresses are the same then order by name. */
	return ( A->Address != B->Address) ? A->Address - B->Address : strcmp( A->Name, B->Name );
	}
/*============================================================================*/
int SimulatorLoadSymbolTable( csimcpustate_t *CPUState, const char *const FileName )
	{
	static const char	*PredefSymNames[]	= { "IVET", "TIMDT", "INTS", "INTE", "TECST", "TECDT" };
	static const int	PredefSymAddrs[]	= { 0xffbe, 0xffd7,  0xffd8, 0xffd9, 0xffda,  0xffdb  };
	char				Path[256], Name[256];
	FILE				*File;
	size_t				NumSymbols;
	int					i, j;

	CSIMSplitPath( FileName, Path, Name, NULL );
	strcat( Path, Name );
	strcat( Path, ".map" );

	if( CPUState->SymbolTable != NULL )
		free( CPUState->SymbolTable );
	if( CPUState->SymbolIndex != NULL )
		free( CPUState->SymbolIndex );
	CPUState->SymbolTable	= NULL;
	CPUState->SymbolIndex	= NULL;

	if(( File = fopen( Path, "rb" )) == NULL )
		return 0;

	/* First we will count the number of lines in the file. */
	for( NumSymbols = 0; ( i = fgetc( File )) != EOF; )
		{
		if( i == '\r' )
			{
			/* This 'r' means it is an end-of-line. */
			NumSymbols++;
			/* Let's read one more character to skip an eventual '\n' (Windows style end-of-line). */
			i	= fgetc( File );
			/* Even if we get another '\r' here, it means we have an empty line, no need to count it. */
			}
		else if( i == '\n' )
			{
			/* This 'n' means it is an end-of-line (Linux style end-of-line). */
			NumSymbols++;
			i	= fgetc( File );
			/* Even if we get another '\n' here, it means we have an empty line, no need to count it. */
			}
		}
	NumSymbols += LENGTH_LCD + LENGTH( PredefSymNames );

	if(( CPUState->SymbolTable = malloc( NumSymbols * sizeof( symbol_t ))) == NULL )
		{
		fclose( File );
		return 0;
		}

	if(( CPUState->SymbolIndex = malloc( MEMORY_SIZE * sizeof( symbol_t* ))) == NULL )
		{
		free( CPUState->SymbolTable );
		fclose( File );
		return 0;
		}

	/* Rewind the file. */
	fseek( File, 0, SEEK_SET );

	for( i = 0; i < LENGTH( PredefSymNames ); i++ )
		{
		strcpy( CPUState->SymbolTable[i].Name, PredefSymNames[i] );
		CPUState->SymbolTable[i].Address	= PredefSymAddrs[i];
		CPUState->SymbolTable[i].Type		= 0;
		}
	for( j = 0; j < LENGTH_LCD; j++, i++ )
		{
		sprintf( CPUState->SymbolTable[i].Name, "LCD_COL%02u", j + 1 );
		CPUState->SymbolTable[i].Address	= 0xffdc + j;
		CPUState->SymbolTable[i].Type		= 0;
		}

	/* Let's skip the two header lines. */
	fscanf( File, "%*s %*s %*s %*s %*s\n" );
	fscanf( File, "%*s %*s %*s\n" );

	for( ; !feof( File ) && i < NumSymbols; i++ )
		if( fscanf( File, "%255s %*s %255s\n", Name, Path ) == 2 )
			{
			strncpy( CPUState->SymbolTable[i].Name, Name, sizeof CPUState->SymbolTable[i].Name );
			CPUState->SymbolTable[i].Name[sizeof CPUState->SymbolTable[i].Name - 1]	= '\0';
			sscanf( Path, "%hX", &CPUState->SymbolTable[i].Address );
			CPUState->SymbolTable[i].Type		= 1;
			}

	/* We don't need the file anymore. */
	fclose( File );

	/* If we found no symbols, there is no reason to keep the tables. */
	if( i == 0 )
		{
		free( CPUState->SymbolTable );
		CPUState->SymbolTable	= NULL;
		free( CPUState->SymbolIndex );
		CPUState->SymbolIndex	= NULL;
		return 0;
		}

	/* Sort the symbol table in ascending order of address/value and by name. */
	qsort( CPUState->SymbolTable, i, sizeof CPUState->SymbolTable[0], CSIMCompareSymbols );

	/* Fill the index with NULL pointers. */
	memset( CPUState->SymbolIndex, 0x00, MEMORY_SIZE * sizeof( symbol_t* ));

	/* Save the number of symbols. */
	CPUState->NumSymbols	= i;

	for( i = 0, j = 0; i < MEMORY_SIZE && j < CPUState->NumSymbols; i++ )
		{
		if( i == CPUState->SymbolTable[j].Address )
			CPUState->SymbolIndex[i]	= &CPUState->SymbolTable[j];
		while( j < CPUState->NumSymbols && i == CPUState->SymbolTable[j].Address )
			j++;
		}

	BitVectorClearAllBits( CPUState->BreakPointsExec );
	BitVectorClearAllBits( CPUState->BreakPointsRead );
	BitVectorClearAllBits( CPUState->BreakPointsWrite );

	return CPUState->NumSymbols;
	}
/*============================================================================*/
static int FetchByte( csimcpustate_t *CPUState )
	{
	/* The processor is trying to fetch a byte of an instruction from the I/O area...*/
	if( CPUState->Registers.R7 >= RAM_SIZE )
		/* ...that doesn't make sense and we will abort the execution. */
		return -1;

	/* We are profiling...*/
	if( CPUState->Map != NULL )
		{
		/* ...let's mark this address as instruction. */
		CPUState->Map[CPUState->Registers.R7].Visited		= 1;
		CPUState->Map[CPUState->Registers.R7].IsCode		= 1;
		CPUState->Map[CPUState->Registers.R7].IsInstruction	= 1;
		if( CPUState->RunningState != RUNNINGSTATE_PROFILING )
			{
			if( !CPUState->Map[CPUState->Registers.R7].Executed )
				CPUState->PreviousRegisters.Regs[7]	= ~CPUState->Registers.Regs[7];
			CPUState->Map[CPUState->Registers.R7].Executed	= 1;
			if( CPUState->Registers.i )
				CPUState->Map[CPUState->Registers.R7].InISR		= 1;
			else
				CPUState->Map[CPUState->Registers.R7].InThread	= 1;
			if( !CPUState->Registers.I )
				CPUState->Map[CPUState->Registers.R7].IntDisabled = 1;
			else
				CPUState->Map[CPUState->Registers.R7].IntEnabled = 1;
			}
		}

	return (int)(unsigned)CPUState->Memory[CPUState->Registers.R7++];
	}
/*============================================================================*/
static int FetchWord( csimcpustate_t *CPUState )
	{
	uint16_t	Temp;

	/* The processor is trying to fetch a word of an instruction from the I/O area...*/
	if( CPUState->Registers.R7 >= RAM_SIZE - 1 )
		/* ...that doesn't make sense and we will abort the execution. */
		return -1;

	/* We are profiling...*/
	if( CPUState->Map != NULL )
		{
		/* ...let's mark those two addresses as instructions. */
		CPUState->Map[CPUState->Registers.R7+0].Visited			= 1;
		CPUState->Map[CPUState->Registers.R7+0].IsCode			= 1;
		CPUState->Map[CPUState->Registers.R7+0].IsInstruction	= 1;
		CPUState->Map[CPUState->Registers.R7+1].IsCode			= 1;
		CPUState->Map[CPUState->Registers.R7+1].Visited			= 1;
		CPUState->Map[CPUState->Registers.R7+1].IsInstruction	= 1;
		if( CPUState->RunningState != RUNNINGSTATE_PROFILING )
			{
			if( !CPUState->Map[CPUState->Registers.R7+0].Executed )
				CPUState->PreviousRegisters.Regs[7]	= ~CPUState->Registers.Regs[7];
			CPUState->Map[CPUState->Registers.R7+0].Executed	= 1;
			CPUState->Map[CPUState->Registers.R7+1].Executed	= 1;
			if( CPUState->Registers.i )
				CPUState->Map[CPUState->Registers.R7].InISR		= 1;
			else
				CPUState->Map[CPUState->Registers.R7].InThread	= 1;
			if( !CPUState->Registers.I )
				CPUState->Map[CPUState->Registers.R7].IntDisabled = 1;
			else
				CPUState->Map[CPUState->Registers.R7].IntEnabled = 1;
			}
		}

	Temp	= (uint16_t)CPUState->Memory[CPUState->Registers.R7++] << 8;
	Temp   |= (uint16_t)CPUState->Memory[CPUState->Registers.R7++] << 0;

	return Temp;
	}
/*============================================================================*/
static int MarkCodeWord( csimcpustate_t *CPUState, uint16_t Address )
	{
	/* The processor is trying to read a data word from the I/O area...*/
	if( Address >= RAM_SIZE - 1 )
		return -1;

	/* We are profiling...*/
	if( CPUState->Map != NULL )
		{
		/* ...let's mark those two addresses as instructions. */
		CPUState->Map[CPUState->Registers.R7+0].Visited			= 1;
		CPUState->Map[CPUState->Registers.R7+0].IsCode			= 1;
		CPUState->Map[CPUState->Registers.R7+1].Visited			= 1;
		CPUState->Map[CPUState->Registers.R7+1].IsCode			= 1;
		if( CPUState->RunningState != RUNNINGSTATE_PROFILING )
			{
			if( !CPUState->Map[CPUState->Registers.R7+0].Executed )
				CPUState->PreviousRegisters.Regs[7]	= ~CPUState->Registers.Regs[7];
			CPUState->Map[CPUState->Registers.R7+0].Executed	= 1;
			CPUState->Map[CPUState->Registers.R7+1].Executed	= 1;
			if( CPUState->Registers.i )
				CPUState->Map[CPUState->Registers.R7].InISR		= 1;
			else
				CPUState->Map[CPUState->Registers.R7].InThread	= 1;
			if( !CPUState->Registers.I )
				CPUState->Map[CPUState->Registers.R7].IntDisabled = 1;
			else
				CPUState->Map[CPUState->Registers.R7].IntEnabled = 1;
			}
		}

	return 0;
	}
/*============================================================================*/
static int ReadWord( csimcpustate_t *CPUState, uint16_t Address, uint16_t *Value )
	{
	uint16_t	Temp;

	CPUState->LastWriteAccess	= Address;

	/* The processor is trying to read a data word from the I/O area...*/
	if( Address >= RAM_SIZE - 1 )
		{
		if( CPUState->BreakPointsRead != NULL && BitVectorQueryBit( CPUState->BreakPointsRead, Address ) != 0 )
			{
			CPUState->SimulatorStatus	= STATUS_BREAKPOINT_READ;
			CPUState->Running			= 0;
			CPUState->RunningState		= RUNNINGSTATE_STOPPED;
			}
		switch( Address )
			{
			case ADDRESS_TIMDT:
				*Value	= (uint16_t)CPUState->Registers.TIMDT;
				return -1;
			case ADDRESS_INTS:
				*Value	= (uint16_t)CPUState->Registers.INTS;
				return -1;
			case ADDRESS_INTE:
				*Value	= (uint16_t)CPUState->Registers.INTE;
				return -1;
			case ADDRESS_TECST:
				*Value	= (uint16_t)CPUState->Registers.TECST;
				return -1;
			case ADDRESS_TECDT:
				*Value	= (uint16_t)CPUState->Registers.TECDT;
				//CPUState->PreviousRegisters.TECDT	= CPUState->Registers.TECDT;
				return -1;
			default:
				if( Address < ADDRESS_LCD || Address >= ADDRESS_LCD + LENGTH_LCD )
					return -2;
				Temp   = (uint16_t)CPUState->Memory[Address];
				return -1;
			}
		}

	if( CPUState->BreakPointsRead != NULL && ( BitVectorQueryBit( CPUState->BreakPointsRead, Address ) != 0 || BitVectorQueryBit( CPUState->BreakPointsRead, Address + 1 ) != 0 ))
		{
		CPUState->SimulatorStatus	= STATUS_BREAKPOINT_READ;
		CPUState->Running			= 0;
		CPUState->RunningState		= RUNNINGSTATE_STOPPED;
		}

	Temp	= (uint16_t)CPUState->Memory[Address++] << 8;
	Temp   |= (uint16_t)CPUState->Memory[Address++] << 0;

	*Value	= Temp;
	return 0;
	}
/*============================================================================*/
static int StoreWord( csimcpustate_t *CPUState, uint16_t Address, uint16_t Value )
	{
	CPUState->LastWriteAccess	= Address;

	/* The processor is trying to write a data word to the I/O area...*/
	if( Address >= RAM_SIZE - 1 )
		{
		if( CPUState->BreakPointsWrite != NULL && BitVectorQueryBit( CPUState->BreakPointsWrite, Address ) != 0 )
			{
			CPUState->SimulatorStatus	= STATUS_BREAKPOINT_WRITE;
			CPUState->Running			= 0;
			CPUState->RunningState		= RUNNINGSTATE_STOPPED;
			}
		switch( Address )
			{
			case ADDRESS_TIMDT:
				CPUState->Registers.TIMDT	= (uint8_t)Value;
				CPUState->TIMDTUpdated		= 1;
				return -1;
			case ADDRESS_INTS:
				/* The bit INTS:7 can only be changed by the (simulated) hardware, it is the "Interrupt Being Serviced" flag. */
				CPUState->Registers.INTS	= ( CPUState->Registers.INTS & 0x80 ) | ( (uint8_t)Value & 0x7f );
				CPUState->INTSUpdated		= 1;
				return -1;
			case ADDRESS_INTE:
				CPUState->Registers.INTE	= (uint8_t)Value & 0x87;
				CPUState->INTEUpdated		= 1;
				return -1;
			case ADDRESS_TECST:
				CPUState->Registers.TECST	= (uint8_t)Value & 0x80;
				CPUState->TECSTUpdated		= 1;
				return -1;
			case ADDRESS_TECDT:
				/* CPUState->Registers.TECDT	= (uint8_t)Value; */
				return -1;
			default:
				if( Address < ADDRESS_LCD || Address >= ADDRESS_LCD + LENGTH_LCD )
					return -2;
				/* If the character is invalid, we will store a space instead. */
				CPUState->Memory[Address]	= isprint( (uint8_t)Value ) ? (uint8_t)Value : ' ';
				CPUState->ChangedLCD		= 1;
				return -1;
			}
		}

	if( CPUState->BreakPointsWrite != NULL && ( BitVectorQueryBit( CPUState->BreakPointsWrite, Address ) != 0 || BitVectorQueryBit( CPUState->BreakPointsWrite, Address + 1 ) != 0 ))
		{
		CPUState->SimulatorStatus	= STATUS_BREAKPOINT_WRITE;
		CPUState->Running			= 0;
		CPUState->RunningState		= RUNNINGSTATE_STOPPED;
		}

	BitVectorSetBit( CPUState->MemoryUpdated, Address + 0 );
	BitVectorSetBit( CPUState->MemoryUpdated, Address + 1 );

	if( CPUState->Memory[Address+0] != (uint8_t)( Value >> 8 ))
		BitVectorSetBit( CPUState->MemoryChanged, Address + 0 );

	if( CPUState->Memory[Address+1] != (uint8_t)( Value >> 0 ))
		BitVectorSetBit( CPUState->MemoryChanged, Address + 1 );

	CPUState->Memory[Address+0]	= (uint8_t)( Value >> 8 );
	CPUState->Memory[Address+1]	= (uint8_t)( Value >> 0 );

	if( Address >= CPUState->MemWindowStart && Address <= CPUState->MemWindowEnd + 1 )
		CPUState->ChangedMem	= 1;

	return 0;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionBxx( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	int			Offset;
	uint16_t	NewAddress;

	if(( Offset = FetchByte( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	if(( NewAddress = CPUState->Registers.R7 + (int8_t)(uint8_t)Offset ) >= RAM_SIZE )
		return STATUS_ERROR_INVALID_ADDRESS;

	/* We are profiling and the instructions is a conditional branch... */
	if( CPUState->RunningState == RUNNINGSTATE_PROFILING && ( OpCode & 0x0f ) != 0x00 && CPUState->ProfilerStack != NULL )
		{
		/* ...let's push the destination address into the profiler's stack so we can visit it later. */
		CPUState->ProfilerStack[CPUState->ProfilerStackPointer++]	= NewAddress;
		return STATUS_TO_CONTINUE;
		}

	switch( OpCode & 0x0f )
		{
		case 0x00:	/* BR */
			break;
		case 0x01:	/* BNE */
			if( CPUState->Registers.Z )
				return STATUS_TO_CONTINUE;
			break;
		case 0x02:	/* BEQ */
			if( !CPUState->Registers.Z )
				return STATUS_TO_CONTINUE;
			break;
		case 0x03:	/* BPL */
			if( CPUState->Registers.N )
				return STATUS_TO_CONTINUE;
			break;
		case 0x04:	/* BMI */
			if( !CPUState->Registers.N )
				return STATUS_TO_CONTINUE;
			break;
		case 0x05:	/* BVC */
			if( CPUState->Registers.V )
				return STATUS_TO_CONTINUE;
			break;
		case 0x06:	/* BVS */
			if( !CPUState->Registers.V )
				return STATUS_TO_CONTINUE;
			break;
		case 0x07:	/* BCC */
			if( CPUState->Registers.C )
				return STATUS_TO_CONTINUE;
			break;
		case 0x08:	/* BCS */
			if( !CPUState->Registers.C )
				return STATUS_TO_CONTINUE;
			break;
		case 0x09:	/* BGE */
			if( CPUState->Registers.N != CPUState->Registers.V )
				return STATUS_TO_CONTINUE;
			break;
		case 0x0a:	/* BLT */
			if( CPUState->Registers.N == CPUState->Registers.V )
				return STATUS_TO_CONTINUE;
			break;
		case 0x0b:	/* BGT */
			if( CPUState->Registers.Z || CPUState->Registers.N != CPUState->Registers.V )
				return STATUS_TO_CONTINUE;
			break;
		case 0x0c:	/* BLE */
			if( !CPUState->Registers.Z && CPUState->Registers.N == CPUState->Registers.V )
				return STATUS_TO_CONTINUE;
			break;
		case 0x0d:	/* BHI */
			if( CPUState->Registers.C || CPUState->Registers.Z )
				return STATUS_TO_CONTINUE;
			break;
		case 0x0e:	/* BLS */
			if( !CPUState->Registers.C && !CPUState->Registers.Z )
				return STATUS_TO_CONTINUE;
			break;
		default:	/* UNDEFINED (Probably "Branch Never" = NOP) */
			if( CPUState->Pedantic )
				return STATUS_ERROR_INVALID_INSTRUCTION;
			return STATUS_TO_CONTINUE;
		}

	CPUState->Registers.R7		= NewAddress;
	CPUState->RegsUpdated[7]	= 1;

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionJMP( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	int	NewAddress;

	if(( NewAddress = FetchByte( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	if( CPUState->Pedantic && NewAddress != 0x2f )
		return STATUS_ERROR_INVALID_INSTRUCTION;

	if(( NewAddress = FetchWord( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	if( NewAddress >= RAM_SIZE )
		return STATUS_ERROR_INVALID_ADDRESS;

	CPUState->Registers.R7		= (uint16_t)NewAddress;
	CPUState->RegsUpdated[7]	= 1;

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionJSR( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	int	NewAddress;
	int	RegNumber;

	if(( NewAddress = FetchByte( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	if( CPUState->Pedantic && NewAddress != 0x2f )
		return STATUS_ERROR_INVALID_INSTRUCTION;

	if(( NewAddress = FetchWord( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	if( NewAddress >= RAM_SIZE )
		return STATUS_ERROR_INVALID_ADDRESS;

	if( CPUState->RunningState == RUNNINGSTATE_PROFILING && CPUState->Map != NULL && CPUState->Map[NewAddress].Visited )
		return STATUS_TO_CONTINUE;

	CPUState->JSRCount++;

	RegNumber							= OpCode & 0x07;
	CPUState->Registers.R6			   -= 2;
	CPUState->RegsUpdated[6]			= 1;

	if( StoreWord( CPUState, CPUState->Registers.R6, CPUState->Registers.Regs[RegNumber] ) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	CPUState->Registers.Regs[RegNumber]	= CPUState->Registers.R7;
	CPUState->RegsUpdated[RegNumber]	= 1;
	CPUState->Registers.R7				= (uint16_t)NewAddress;
	CPUState->RegsUpdated[7]			= 1;

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionRTS_RTI( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	uint16_t	NewAddress, FlagsTemp;
	int			RegNumber;

	/* Instruction RTI */
	if( OpCode >= 0x78 )
		{
		if( CPUState->Pedantic && OpCode > 0x78 )
			return STATUS_ERROR_INVALID_INSTRUCTION;

		if( ReadWord( CPUState, CPUState->Registers.R6, &FlagsTemp ) < 0 )
			return STATUS_ERROR_INVALID_ADDRESS;
		CPUState->Registers.R6			   += 2;
		CPUState->RegsUpdated[6]			= 1;

		if( ReadWord( CPUState, CPUState->Registers.R6, &NewAddress ) < 0 )
			return STATUS_ERROR_INVALID_ADDRESS;
		CPUState->Registers.R6			   += 2;
		CPUState->RegsUpdated[6]			= 1;

		CPUState->Registers.Flags			= (uint8_t)FlagsTemp & 0xff;
		CPUState->FlagsUpdated				= 1;
		CPUState->Registers.R7				= NewAddress;
		CPUState->RegsUpdated[7]			= 1;
		CPUState->Registers.i				= 0;
		CPUState->INTSUpdated				= 1;
		CPUState->Latency					= 1;
		}
	/* Instruction RTS */
	else
		{
		if( ReadWord( CPUState, CPUState->Registers.R6, &NewAddress ) < 0 )
			return STATUS_ERROR_INVALID_ADDRESS;

		CPUState->Registers.R6			   += 2;
		CPUState->RegsUpdated[6]			= 1;

		RegNumber							= OpCode & 0x07;
		CPUState->Registers.R7				= CPUState->Registers.Regs[RegNumber];
		CPUState->RegsUpdated[7]			= 1;
		CPUState->Registers.Regs[RegNumber]	= NewAddress;
		CPUState->RegsUpdated[RegNumber]	= 1;
		if( CPUState->RunningState == RUNNINGSTATE_STEPOVER && CPUState->Registers.R7 == CPUState->StepOverR7 && CPUState->Registers.R6 == CPUState->StepOverR6 )
			{
			CPUState->RunningState			= RUNNINGSTATE_STOPPED;
			CPUState->Busy					= 0;
			CPUState->Running				= 0;
			}
		}


	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionCCC( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	CPUState->Registers.Flags  &= ~( OpCode & 0x0f );
	CPUState->FlagsUpdated		= 1;

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionSCC( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	CPUState->Registers.Flags  |=  ( OpCode & 0x0f );
	CPUState->FlagsUpdated		= 1;

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructionSOB( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	int			Offset;
	int			RegNumber;
	uint16_t	NewAddress;

	if(( Offset = FetchByte( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	if(( NewAddress = CPUState->Registers.R7 - (int8_t)(uint8_t)Offset ) >= RAM_SIZE )
		return STATUS_ERROR_INVALID_ADDRESS;

	RegNumber					= OpCode & 0x07;

	CPUState->RegsUpdated[RegNumber]	= 1;
	if( --CPUState->Registers.Regs[RegNumber] != 0 )
		{
		/* We are profiling... */
		if( CPUState->RunningState == RUNNINGSTATE_PROFILING && CPUState->ProfilerStack != NULL )
			/* ...let's push the destination address into the profiler's stack so we can visit it later. */
			CPUState->ProfilerStack[CPUState->ProfilerStackPointer++]	= NewAddress;
		else
			{
			CPUState->Registers.R7		= NewAddress;
			CPUState->RegsUpdated[7]	= 1;
			}
		}

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
typedef enum
	{
	MODE_REGISTER,
	MODE_REGISTER_POSTINC,
	MODE_REGISTER_PREDEC,
	MODE_REGISTER_INDEXED,
	MODE_REGISTER_INDIRECT,
	MODE_REGISTER_POSTINC_INDIRECT,
	MODE_REGISTER_PREDEC_INDIRECT,
	MODE_REGISTER_INDEXED_INDIRECT
	} addrmode_t;
/*============================================================================*/
typedef union
	{
	uint16_t		Value;
	struct
		{
		uint16_t	RegDst		: 3;
		uint16_t	ModeDst		: 3;
		uint16_t	RegSrc		: 3;
		uint16_t	ModeSrc		: 3;
		uint16_t	OpCode4		: 4;
		};
	struct
		{
		uint16_t				: 6;
		uint16_t	Undefined	: 2;
		uint16_t				: 8;
		};
	struct
		{
		uint16_t	OpCodeLow	: 8;
		uint16_t	OpCode8		: 8;
		};
	} opcode2op_t;
/*============================================================================*/
int SimulatorDisassemble( csimcpustate_t *CPUState, uint16_t *Offset, char * const Buffer, size_t BufferSize, int *Skip )
	{
	uint8_t		OpCode1, OpCode2;
	uint16_t	OffsetTemp;

	*Skip		= 0;
	Buffer[0]	= '\0';

	OffsetTemp	= *Offset;
	OpCode1		= CPUState->Memory[OffsetTemp++];

	switch(( OpCode1 >> 4 ) & 0x0f )
		{
		case 0x00:	/* NOP */
			if( /*CPUState->Pedantic &&*/ ( OpCode1 & 0x0f ) != 0 )
//				return STATUS_ERROR_INVALID_INSTRUCTION;
				sprintf( Buffer, "%04X: %02X                  NOP ; *INVALID*", *Offset, (unsigned)OpCode1 );
			else
				sprintf( Buffer, "%04X: %02X                  NOP", *Offset, (unsigned)OpCode1 );
			break;
		case 0x01:	/* CCC */
			sprintf( Buffer, "%04X: %02X                  CCC %s%s%s%s%s", *Offset, (unsigned)OpCode1, OpCode1 & 0x08 ? "N" : "", OpCode1 & 0x04 ? "Z" : "", OpCode1 & 0x02 ? "V" : "", OpCode1 & 0x01 ? "C" : "", ( OpCode1 & 0x0f ) == 0 ? "; *POINTLESS*" : "" );
			break;
		case 0x02:	/* SCC */
			sprintf( Buffer, "%04X: %02X                  SCC %s%s%s%s%s", *Offset, (unsigned)OpCode1, OpCode1 & 0x08 ? "N" : "", OpCode1 & 0x04 ? "Z" : "", OpCode1 & 0x02 ? "V" : "", OpCode1 & 0x01 ? "C" : "", ( OpCode1 & 0x0f ) == 0 ? "; *POINTLESS*" : "" );
			break;
		case 0x03:	/* BR, Bxx */
			{
			static const char	*Conditions[]	= { "R ", "NE", "EQ", "PL", "MI", "VC", "VS", "CC", "CS", "GE", "LT", "GT", "LE", "HI", "LS", "??" };
			uint16_t			Address;
			int					Index;

			OpCode2	= CPUState->Memory[OffsetTemp++];
			Address	= OffsetTemp + (int8_t)OpCode2;
			if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
				Index	= sprintf( Buffer, "%04X: %02X %02X               B%s %s ; h%04X = $%+d", *Offset, (unsigned)OpCode1, (unsigned)OpCode2, Conditions[OpCode1&0x0f], CPUState->SymbolIndex[Address]->Name, Address, (int8_t)OpCode2 + 2 );
			else
				Index	= sprintf( Buffer, "%04X: %02X %02X               B%s h%04X    ; $%+d", *Offset, (unsigned)OpCode1, (unsigned)OpCode2, Conditions[OpCode1&0x0f], Address, (int8_t)OpCode2 + 2 );
			if( /*CPUState->Pedantic &&*/ OpCode1 == 0x3f )
				sprintf( &Buffer[Index], " ; *INVALID*" );
//				return STATUS_ERROR_INVALID_INSTRUCTION;
			if(( OpCode1 & 0x0f ) != 0 )
				*Skip	= 1;
			else
				*Skip	= 2;
			break;
			}
		case 0x04:	/* JMP */
			{
			uint16_t	Address;
			int			Index;

			OpCode2		= CPUState->Memory[OffsetTemp++];
			Address		= (uint16_t)CPUState->Memory[OffsetTemp++] << 8;
			Address	   |= (uint16_t)CPUState->Memory[OffsetTemp++] << 0;
			if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
				Index	= sprintf( Buffer, "%04X: %02X %02X %02X %02X         JMP %s ; h%04X", *Offset, (unsigned)OpCode1, (unsigned)OpCode2, (unsigned)(uint8_t)( Address >> 8 ), (unsigned)(uint8_t)( Address >> 0 ), CPUState->SymbolIndex[Address]->Name, Address );
			else
				Index	= sprintf( Buffer, "%04X: %02X %02X %02X %02X         JMP h%04X", *Offset, (unsigned)OpCode1, (unsigned)OpCode2, (unsigned)(uint8_t)( Address >> 8 ), (unsigned)(uint8_t)( Address >> 0 ), Address );
			if(( CPUState->Pedantic && ( OpCode1 & 0x0f ) != 0 ) || ( CPUState->Pedantic && OpCode2 != 0x2f ))
				sprintf( &Buffer[Index], " ; *INVALID*" );
//				return STATUS_ERROR_INVALID_INSTRUCTION;
			*Skip		= 2;
			break;
			}
		case 0x05:	/* SOB */
			{
			uint16_t	Address;

			OpCode2	= CPUState->Memory[OffsetTemp++];

			Address	= OffsetTemp - (int8_t)OpCode2;
			if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
				sprintf( Buffer, "%04X: %02X %02X               SOB R%u,%s ; h%04X = $%+d", *Offset, OpCode1, OpCode2, OpCode1 & 0x07, CPUState->SymbolIndex[Address]->Name, Address, -(int8_t)OpCode2 + 2 );
			else
				sprintf( Buffer, "%04X: %02X %02X               SOB R%u,h%04X ; $%+d", *Offset, OpCode1, OpCode2, OpCode1 & 0x07, Address, -(int8_t)OpCode2 + 2 );
			*Skip	= 1;
			break;
			}
		case 0x06:	/* JSR */
			{
			uint16_t	Address;
			int			Index;

			OpCode2		= CPUState->Memory[OffsetTemp++];
			Address		= (uint16_t)CPUState->Memory[OffsetTemp++] << 8;
			Address	   |= (uint16_t)CPUState->Memory[OffsetTemp++] << 0;
			if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
				{
				Index	= sprintf( Buffer, "%04X: %02X %02X %02X %02X         JSR R%u,%s ; h%04X", *Offset, OpCode1, OpCode2, (unsigned)(uint8_t)( Address >> 8 ), (unsigned)(uint8_t)( Address >> 0 ), OpCode1 & 7, CPUState->SymbolIndex[Address]->Name, Address );
				if( /*CPUState->Pedantic &&*/ OpCode2 != 0x2f )
					sprintf( &Buffer[Index], " *INVALID*" );
//					return STATUS_ERROR_INVALID_INSTRUCTION;
				}
			else
				{
				Index	= sprintf( Buffer, "%04X: %02X %02X %02X %02X         JSR R%u,h%04X", *Offset, OpCode1, OpCode2, (unsigned)(uint8_t)( Address >> 8 ), (unsigned)(uint8_t)( Address >> 0 ), OpCode1 & 7, Address );
				if( /*CPUState->Pedantic &&*/ OpCode2 != 0x2f )
					sprintf( &Buffer[Index], " ; *INVALID*" );
//					return STATUS_ERROR_INVALID_INSTRUCTION;
				}
			*Skip	= 1;
			break;
			}
		case 0x07:	/* RTS, RTI */
			{
			int Index;

			if(( OpCode1 & 0x0f ) <= 7 )
				Index	= sprintf( Buffer, "%04X: %02X                  RTS R%u", *Offset, (unsigned)OpCode1, OpCode1 & 0x07 );
			else /*if(( OpCode1 & 0x0f ) >= 8 )*/
				Index	= sprintf( Buffer, "%04X: %02X                  RTI", *Offset, (unsigned)OpCode1 );
			if( /*CPUState->Pedantic &&*/ ( OpCode1 & 0x0f ) > 8 )
				sprintf( &Buffer[Index], " ; *INVALID*" );
//				return STATUS_ERROR_INVALID_INSTRUCTION;
			*Skip	= 2;
			break;
			}
		case 0x08:	/* CLR, NOT, INC, DEC, NEG, TST, ROR, ROL, ASR, ASL, ADC, SBC */
			{
			static const char *Instructions[]	= { "CLR", "NOT", "INC", "DEC", "NEG", "TST", "ROR", "ROL", "ASR", "ASL", "ADC", "SBC", "?8C", "?8D", "?8E", "?8F" };
			opcode2op_t	OpCode16;
			int			Index;
			uint16_t	AddressDst	= 0;
			int			Warning		= 0;

			OpCode2	= CPUState->Memory[OffsetTemp++];

			OpCode16.OpCode8	= OpCode1;
			OpCode16.OpCodeLow	= OpCode2;

			Index	= sprintf( Buffer, "%04X: %02X %02X ", *Offset, OpCode1, OpCode2 );

			if( OpCode16.ModeDst == MODE_REGISTER_INDEXED || OpCode16.ModeDst == MODE_REGISTER_INDEXED_INDIRECT || (( OpCode16.ModeDst == MODE_REGISTER_POSTINC || OpCode16.ModeDst == MODE_REGISTER_POSTINC_INDIRECT ) && OpCode16.RegDst == 7 ))
				{
				AddressDst	= (uint16_t)CPUState->Memory[OffsetTemp++] << 8;
				AddressDst |= (uint16_t)CPUState->Memory[OffsetTemp++] << 0;
				Index	   += sprintf( &Buffer[Index], "%02X %02X       ", (uint8_t)( AddressDst >> 8 ), (uint8_t)( AddressDst >> 0 ));
				}
			else
				Index	   += sprintf( &Buffer[Index], "            " );

			Index  += sprintf( &Buffer[Index], "  %s ", Instructions[OpCode16.OpCode8 & 0x0f] );

			switch( OpCode16.ModeDst )
				{
				case MODE_REGISTER:
					Index  += sprintf( &Buffer[Index], "R%u", OpCode16.RegDst );
					break;

				case MODE_REGISTER_POSTINC:
					if( OpCode16.RegDst == 7 )
						{
						if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
							Index  += sprintf( &Buffer[Index], "#%s ; #h%04X", CPUState->SymbolIndex[AddressDst]->Name, AddressDst );
						else
							Index  += sprintf( &Buffer[Index], "#h%04X", AddressDst );
						Warning	= 1;
						}
					else
						Index  += sprintf( &Buffer[Index], "(R%u)+", OpCode16.RegDst );
					break;

				case MODE_REGISTER_PREDEC:
					if( OpCode16.RegDst == 7 )
						Warning	= 2;
					Index  += sprintf( &Buffer[Index], "-(R%u)", OpCode16.RegDst );
					break;

				case MODE_REGISTER_INDEXED:
					if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
						Index  += sprintf( &Buffer[Index], "%s(R%u) ; h%04X(R%u)", CPUState->SymbolIndex[AddressDst]->Name, OpCode16.RegDst, AddressDst, OpCode16.RegDst );
					else
						Index  += sprintf( &Buffer[Index], "h%04X(R%u)", AddressDst, OpCode16.RegDst );
					break;

				case MODE_REGISTER_INDIRECT:
					Index  += sprintf( &Buffer[Index], "(R%u)", OpCode16.RegDst );
					break;

				case MODE_REGISTER_POSTINC_INDIRECT:
					if( OpCode16.RegDst == 7 )
						{
						if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
							Index  += sprintf( &Buffer[Index], "%s ; h%04X", CPUState->SymbolIndex[AddressDst]->Name, AddressDst );
						else
							Index  += sprintf( &Buffer[Index], "h%04X", AddressDst );
						}
					else
						Index  += sprintf( &Buffer[Index], "((R%u)+)", OpCode16.RegDst );
					break;

				case MODE_REGISTER_PREDEC_INDIRECT:
					if( OpCode16.RegDst == 7 )
						Warning	= 2;
					Index  += sprintf( &Buffer[Index], "(-(R%u))", OpCode16.RegDst );
					break;

				case MODE_REGISTER_INDEXED_INDIRECT:
					if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
						Index  += sprintf( &Buffer[Index], "(%s(R%u)) ; h%04X(R%u)", CPUState->SymbolIndex[AddressDst]->Name, OpCode16.RegDst, AddressDst, OpCode16.RegDst );
					else
						Index  += sprintf( &Buffer[Index], "(h%04X(R%u))", AddressDst, OpCode16.RegDst );
					break;
				}

			/* We are being pedantic and the instruction have undefined bits set... */
			if( /*CPUState->Pedantic &&*/ OpCode16.Undefined != 0 )
				/* ...let's reject the instruction. */
				Index  += sprintf( &Buffer[Index], " ; *INVALID*" );
//				return STATUS_ERROR_INVALID_INSTRUCTION;
			else if( Warning == 2 )
				Index  += sprintf( &Buffer[Index], " ; WARNING" );
			else if( Warning == 1 )
				Index  += sprintf( &Buffer[Index], " ; POINTLESS" );

			break;
			}
		case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e:	/* MOV, ADD, SUB, CMP, AND, OR */
			{
			static const char *Instructions[]	= { "?90", "MOV", "ADD", "SUB", "CMP", "AND", "OR ", "?97" };
			opcode2op_t	OpCode16;
			int			Index;
			uint16_t	Address		= 0, AddressDst	= 0;
			int			Warning		= 0;
			char		AddrSrc[20]	= "";
			char		AddrDst[20]	= "";

			OpCode2				= CPUState->Memory[OffsetTemp++];

			OpCode16.OpCode8	= OpCode1;
			OpCode16.OpCodeLow	= OpCode2;

			Index	= sprintf( Buffer, "%04X: %02X %02X ", *Offset, OpCode1, OpCode2 );

			if( OpCode16.ModeSrc == MODE_REGISTER_INDEXED || OpCode16.ModeSrc == MODE_REGISTER_INDEXED_INDIRECT || (( OpCode16.ModeSrc == MODE_REGISTER_POSTINC || OpCode16.ModeSrc == MODE_REGISTER_POSTINC_INDIRECT ) && OpCode16.RegSrc == 7 ))
				{
				Address		= (uint16_t)CPUState->Memory[OffsetTemp++] << 8;
				Address	   |= (uint16_t)CPUState->Memory[OffsetTemp++] << 0;
				Index	   += sprintf( &Buffer[Index], "%02X %02X ", (uint8_t)( Address >> 8 ), (uint8_t)( Address >> 0 ));
				}

			if( OpCode16.ModeDst == MODE_REGISTER_INDEXED || OpCode16.ModeDst == MODE_REGISTER_INDEXED_INDIRECT || (( OpCode16.ModeDst == MODE_REGISTER_POSTINC || OpCode16.ModeDst == MODE_REGISTER_POSTINC_INDIRECT ) && OpCode16.RegDst == 7 ))
				{
				AddressDst	= (uint16_t)CPUState->Memory[OffsetTemp++] << 8;
				AddressDst |= (uint16_t)CPUState->Memory[OffsetTemp++] << 0;
				Index	   += sprintf( &Buffer[Index], "%02X %02X ", (uint8_t)( AddressDst >> 8 ), (uint8_t)( AddressDst >> 0 ));
				}
			else
				Index	   += sprintf( &Buffer[Index], "      " );

			if( !( OpCode16.ModeSrc == MODE_REGISTER_INDEXED || OpCode16.ModeSrc == MODE_REGISTER_INDEXED_INDIRECT || (( OpCode16.ModeSrc == MODE_REGISTER_POSTINC || OpCode16.ModeSrc == MODE_REGISTER_POSTINC_INDIRECT ) && OpCode16.RegSrc == 7 )))
				Index	   += sprintf( &Buffer[Index], "      " );

			switch( OpCode16.ModeSrc )
				{
				case MODE_REGISTER:
					Index  += sprintf( &Buffer[Index], "  %s R%u,", Instructions[OpCode16.OpCode4 & 0x07], OpCode16.RegSrc );
					break;

				case MODE_REGISTER_POSTINC:
					if( OpCode16.RegSrc == 7 )
						{
						if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
							{
							Index  += sprintf( &Buffer[Index], "  %s #%s,", Instructions[OpCode16.OpCode4 & 0x07], CPUState->SymbolIndex[Address]->Name );
							sprintf( AddrSrc, "#h%04X", Address );
							}
						else
							Index  += sprintf( &Buffer[Index], "  %s #h%04X,", Instructions[OpCode16.OpCode4 & 0x07], Address );
						}
					else
						Index  += sprintf( &Buffer[Index], "  %s (R%u)+,", Instructions[OpCode16.OpCode4 & 0x07], OpCode16.RegSrc );
					break;

				case MODE_REGISTER_PREDEC:
					if( OpCode16.RegSrc == 7 )
						Warning	= 2;
					Index  += sprintf( &Buffer[Index], "  %s -(R%u),", Instructions[OpCode16.OpCode4 & 0x07], OpCode16.RegSrc );
					break;

				case MODE_REGISTER_INDEXED:
					if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
						{
						Index  += sprintf( &Buffer[Index], "  %s %s(R%u),", Instructions[OpCode16.OpCode4 & 0x07], CPUState->SymbolIndex[Address]->Name, OpCode16.RegSrc );
						sprintf( AddrSrc, "h%04X ", Address );
						}
					else
						{
						if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
							{
							Index  += sprintf( &Buffer[Index], "  %s %s(R%u),", Instructions[OpCode16.OpCode4 & 0x07], CPUState->SymbolIndex[Address]->Name, OpCode16.RegSrc );
							sprintf( AddrSrc, "h%04X(R%u)", Address, OpCode16.RegSrc );
							}
						else
							Index  += sprintf( &Buffer[Index], "  %s h%04X(R%u),", Instructions[OpCode16.OpCode4 & 0x07], Address, OpCode16.RegSrc );
						}
					break;

				case MODE_REGISTER_INDIRECT:
					Index  += sprintf( &Buffer[Index], "  %s (R%u),", Instructions[OpCode16.OpCode4 & 0x07], OpCode16.RegSrc );
					break;

				case MODE_REGISTER_POSTINC_INDIRECT:
					if( OpCode16.RegSrc == 7 )
						{
						if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
							{
							Index  += sprintf( &Buffer[Index], "  %s %s,", Instructions[OpCode16.OpCode4 & 0x07], CPUState->SymbolIndex[Address]->Name );
							sprintf( AddrSrc, "h%04X", Address );
							}
						else
							Index  += sprintf( &Buffer[Index], "  %s h%04X,", Instructions[OpCode16.OpCode4 & 0x07], Address );
						}
					else
						Index  += sprintf( &Buffer[Index], "  %s ((R%u)+),", Instructions[OpCode16.OpCode4 & 0x07], OpCode16.RegSrc );
					break;

				case MODE_REGISTER_PREDEC_INDIRECT:
					if( OpCode16.RegSrc == 7 )
						Warning	= 2;
					Index  += sprintf( &Buffer[Index], "  %s (-(R%u)),", Instructions[OpCode16.OpCode4 & 0x07], OpCode16.RegSrc );
					break;

				case MODE_REGISTER_INDEXED_INDIRECT:
					if( Address != 0 && CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[Address] != NULL )
						{
						Index  += sprintf( &Buffer[Index], "  %s (%s(R%u)),", Instructions[OpCode16.OpCode4 & 0x07], CPUState->SymbolIndex[Address]->Name, OpCode16.RegSrc );
						sprintf( AddrSrc, "(h%04X(R%u)) ", Address, OpCode16.RegSrc );
						}
					else
						Index  += sprintf( &Buffer[Index], "  %s (h%04X(R%u)),", Instructions[OpCode16.OpCode4 & 0x07], Address, OpCode16.RegSrc );
					break;
				}

			switch( OpCode16.ModeDst )
				{
				case MODE_REGISTER:
					Index  += sprintf( &Buffer[Index], "R%u", OpCode16.RegDst );
					break;

				case MODE_REGISTER_POSTINC:
					if( OpCode16.RegDst == 7 )
						{
						Index  += sprintf( &Buffer[Index], "#h%04X", AddressDst );
						if( OpCode16.OpCode4 != 0x0c && Warning < 1 )
							Warning	= 1;
						}
					else
						Index  += sprintf( &Buffer[Index], "(R%u)+", OpCode16.RegDst );
					break;

				case MODE_REGISTER_PREDEC:
					if( OpCode16.RegDst == 7 )
						Warning	= 2;
					Index  += sprintf( &Buffer[Index], "-(R%u)", OpCode16.RegDst );
					break;

				case MODE_REGISTER_INDEXED:
					if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
						{
						Index  += sprintf( &Buffer[Index], "%s(R%u)", CPUState->SymbolIndex[AddressDst]->Name, OpCode16.RegDst );
						sprintf( AddrDst, "h%04X(R%u)", AddressDst, OpCode16.RegDst );
						}
					else
						Index  += sprintf( &Buffer[Index], "h%04X(R%u)", AddressDst, OpCode16.RegDst );
					break;

				case MODE_REGISTER_INDIRECT:
					Index  += sprintf( &Buffer[Index], "(R%u)", OpCode16.RegDst );
					break;

				case MODE_REGISTER_POSTINC_INDIRECT:
					if( OpCode16.RegDst == 7 )
						{
						if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
							{
							Index  += sprintf( &Buffer[Index], "%s", CPUState->SymbolIndex[AddressDst]->Name );
							sprintf( AddrDst, "h%04X", AddressDst );
							}
						else
							Index  += sprintf( &Buffer[Index], "h%04X", AddressDst );
						}
					else
						Index  += sprintf( &Buffer[Index], "((R%u)+)", OpCode16.RegDst );
					break;

				case MODE_REGISTER_PREDEC_INDIRECT:
					if( OpCode16.RegDst == 7 )
						Warning	= 2;
					Index  += sprintf( &Buffer[Index], "(-(R%u))", OpCode16.RegDst );
					break;

				case MODE_REGISTER_INDEXED_INDIRECT:
					if( CPUState->SymbolIndex != NULL && CPUState->SymbolIndex[AddressDst] != NULL )
						{
						Index  += sprintf( &Buffer[Index], "(%s(R%u))", CPUState->SymbolIndex[AddressDst]->Name, OpCode16.RegDst );
						sprintf( AddrDst, "h%04X(R%u)", AddressDst, OpCode16.RegDst );
						}
					else
						Index  += sprintf( &Buffer[Index], "(h%04X(R%u))", AddressDst, OpCode16.RegDst );
					break;
				}

			if( Warning != 0 || AddrSrc[0] != '\0' || AddrDst[0] != '\0' )
				Index  += sprintf( &Buffer[Index], " ; " );

			Index  += sprintf( &Buffer[Index], "%s", AddrSrc );
			if( AddrSrc[0] != '\0' && AddrDst[0] != '\0' )
				Index  += sprintf( &Buffer[Index], " -> " );
			Index  += sprintf( &Buffer[Index], "%s", AddrDst );

			if( Warning == 2 )
				Index  += sprintf( &Buffer[Index], " ; WARNING" );
			else if( Warning == 1 )
				Index  += sprintf( &Buffer[Index], " ; POINTLESS" );

			break;
			}
		case 0x0f:	/* HLT */
			if( CPUState->Pedantic && ( OpCode1 & 0x0f ) != 0 )
//				return STATUS_ERROR_INVALID_INSTRUCTION;
				sprintf( Buffer, "%04X: %02X                  HLT; *INVALID*", *Offset, (unsigned)OpCode1 );
			else
				sprintf( Buffer, "%04X: %02X                  HLT", *Offset, (unsigned)OpCode1 );
			*Skip	= 3;
			break;
		}

	*Offset	= OffsetTemp;

	return 0;
	}
/*============================================================================*/
static csimstatuscodes_t ProcessOperand( csimcpustate_t *CPUState, addrmode_t Mode, int Reg, uint16_t *Value, uint16_t *Address, int DontRead )
	{
	uint16_t	Offset;

	switch( Mode )
		{
		case MODE_REGISTER:
			if( !DontRead )
				*Value	= CPUState->Registers.Regs[Reg&0x07];
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_POSTINC:
			*Address	= CPUState->Registers.Regs[Reg&0x07];
			if( !DontRead && ReadWord( CPUState, CPUState->Registers.Regs[Reg&0x07], Value ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			if( /*CPUState->RunningState == RUNNINGSTATE_PROFILING &&*/ CPUState->Map != NULL && Reg == 7 )
				MarkCodeWord( CPUState, CPUState->Registers.R7 );
			CPUState->Registers.Regs[Reg&0x07] += 2;
			CPUState->RegsUpdated[Reg&0x07]		= 1;
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_PREDEC:
			CPUState->Registers.Regs[Reg&0x07] -= 2;
			CPUState->RegsUpdated[Reg&0x07]		= 1;
			*Address	= CPUState->Registers.Regs[Reg&0x07];
			if( !DontRead && ReadWord( CPUState, CPUState->Registers.Regs[Reg&0x07], Value ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_INDEXED:
			if( ReadWord( CPUState, CPUState->Registers.R7, &Offset ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			/* We are profiling...*/
			if( /*CPUState->RunningState == RUNNINGSTATE_PROFILING &&*/ CPUState->Map != NULL )
				MarkCodeWord( CPUState, CPUState->Registers.R7 );
			CPUState->Registers.R7	   += 2;
			CPUState->RegsUpdated[7]	= 1;
			*Address	= CPUState->Registers.Regs[Reg&0x07]  + Offset;
			if( !DontRead && ReadWord( CPUState, CPUState->Registers.Regs[Reg&0x07] + Offset, Value ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_INDIRECT:
			*Address	= CPUState->Registers.Regs[Reg&0x07];
			if( !DontRead && ReadWord( CPUState, CPUState->Registers.Regs[Reg&0x07], Value ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_POSTINC_INDIRECT:
			if( ReadWord( CPUState, CPUState->Registers.Regs[Reg&0x07], &Offset ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			if( /*CPUState->RunningState == RUNNINGSTATE_PROFILING &&*/ CPUState->Map != NULL && Reg == 7 )
				MarkCodeWord( CPUState, CPUState->Registers.R7 );
			CPUState->Registers.Regs[Reg&0x07] += 2;
			CPUState->RegsUpdated[Reg&0x07]		= 1;
			*Address	= Offset;
			if( !DontRead && ReadWord( CPUState, Offset, Value ) < -1 && CPUState->Pedantic )
				return STATUS_ERROR_INVALID_ADDRESS;
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_PREDEC_INDIRECT:
			CPUState->Registers.Regs[Reg&0x07] -= 2;
			CPUState->RegsUpdated[Reg&0x07]		= 1;
			if( ReadWord( CPUState, CPUState->Registers.Regs[Reg&0x07], &Offset ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			*Address	= Offset;
			if( !DontRead && ReadWord( CPUState, Offset, Value ) < -1 && CPUState->Pedantic )
				return STATUS_ERROR_INVALID_ADDRESS;
			return STATUS_TO_CONTINUE;

		case MODE_REGISTER_INDEXED_INDIRECT:
			if( ReadWord( CPUState, CPUState->Registers.R7, &Offset ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			if( /*CPUState->RunningState == RUNNINGSTATE_PROFILING*/ CPUState->Map != NULL )
				MarkCodeWord( CPUState, CPUState->Registers.R7 );
			CPUState->Registers.R7	   += 2;
			CPUState->RegsUpdated[7]	= 1;
			Offset += CPUState->Registers.Regs[Reg&0x07];
			if( ReadWord( CPUState, Offset, Address ) < -1 && CPUState->Pedantic )
				return STATUS_ERROR_INVALID_ADDRESS;
			if( !DontRead && ReadWord( CPUState, *Address, Value ) < -1 && CPUState->Pedantic )
				return STATUS_ERROR_INVALID_ADDRESS;
			return STATUS_TO_CONTINUE;
		}

	return STATUS_ERROR_INTERNAL_ERROR;
	}
/*============================================================================*/
static csimstatuscodes_t StoreResult( csimcpustate_t *CPUState, addrmode_t Mode, int Reg, uint16_t Value, uint16_t Address )
	{
	csimstatuscodes_t Status;

	if( Mode == 0 )
		{
		CPUState->Registers.Regs[Reg&7]	= Value;
		CPUState->RegsUpdated[Reg&7]	= 1;
		}
	else if(( Status = StoreWord( CPUState, Address, Value )) < -1 )
		return Status;

	return STATUS_TO_CONTINUE;
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructions1Operand( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	csimstatuscodes_t 	Status;
	opcode2op_t			OpCode16;
	uint8_t				Operation;
	int					Temp;
	uint16_t			Operand, Result, Offset;

	/* Get the second byte of the instruction. */
	if(( Temp = FetchByte( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	OpCode16.OpCode8	= OpCode;
	OpCode16.OpCodeLow	= (uint8_t)Temp;

	/* We are being pedantic and the instruction have undefined bits set... */
	if( CPUState->Pedantic && OpCode16.Undefined != 0 )
		/* ...let's reject the instruction. */
		return STATUS_ERROR_INVALID_INSTRUCTION;

	Operation	= OpCode & 0x0f;

	/* For instruction CLR we don't need to read anything... */
	if(( Status = ProcessOperand( CPUState, OpCode16.ModeDst, OpCode16.RegDst, &Operand, &Offset, Operation == 0x00 )) < STATUS_TO_CONTINUE )
		return Status;

	switch( Operation )
		{
		case 0x00:	/* CLR */
			Result	= 0;
			CPUState->Registers.C	= 0;
			CPUState->Registers.V	= 0;
			break;
		case 0x01:	/* NOT */
			Result	= ~Operand;
			CPUState->Registers.C	= 0;
			CPUState->Registers.V	= 0;
			break;
		case 0x02:	/* INC */
			Result	= Operand + 1;
			CPUState->Registers.C	= Result < Operand;
			CPUState->Registers.V	= (int16_t)Result < (int16_t)Operand;
			break;
		case 0x03:	/* DEC */
			Result	= Operand - 1;
			CPUState->Registers.C	= Result > Operand;
			CPUState->Registers.V	= (int16_t)Result > (int16_t)Operand;
			break;
		case 0x04:	/* NEG */
			Result	= -Operand;
			CPUState->Registers.C	= 1;
			CPUState->Registers.V	= 0;
			break;
		case 0x05:	/* TST */
			CPUState->Registers.Z	= Operand == 0;
			CPUState->Registers.N	= (int16_t)Operand < 0;
			CPUState->Registers.C	= 0;
			CPUState->Registers.V	= 0;
			CPUState->FlagsUpdated	= 1;
			/* There is no need to store the result. */
			return STATUS_TO_CONTINUE;
		case 0x06:	/* ROR */
			Result	= ( CPUState->Registers.C ? 0x8000 : 0x0000 ) | (( Operand >> 1 ) & 0x7fff );
			CPUState->Registers.C	= ( Operand & 0x0001 ) ? 1 : 0;
			CPUState->Registers.V	= (( Result ^ Operand ) & 0x8000 ) ? 1 : 0;
			break;
		case 0x07:	/* ROL */
			Result	= (( Operand << 1 ) & 0xfffe ) | ( CPUState->Registers.C ? 0x0001 : 0x0000 );
			CPUState->Registers.C	= ( Operand & 0x8000 ) ? 1 : 0;
			CPUState->Registers.V	= (( Result ^ Operand ) & 0x8000 ) ? 1 : 0;
			break;
		case 0x08:	/* ASR */
			Result	= ( Operand & 0x8000 ) | ( Operand >> 1 );
			CPUState->Registers.C	= ( Operand & 0x0001 ) ? 1 : 0;
			CPUState->Registers.V	= (( Result ^ Operand ) & 0x8000 ) ? 1 : 0;
			break;
		case 0x09:	/* ASL */
			Result	= Operand << 1;
			CPUState->Registers.C	= ( Operand & 0x8000 ) ? 1 : 0;
			CPUState->Registers.V	= (( Result ^ Operand ) & 0x8000 ) ? 1 : 0;
			break;
		case 0x0a:	/* ADC */
			Result	= Operand + CPUState->Registers.C;
			CPUState->Registers.C	= Result < Operand;
			CPUState->Registers.V	= (int16_t)Result < (int16_t)Operand;
			break;
		case 0x0b:	/* SBC */
			Result	= Operand - CPUState->Registers.C;
			CPUState->Registers.C	= Result > Operand;
			CPUState->Registers.V	= (int16_t)Result > (int16_t)Operand;
			break;
		default:
			if( CPUState->RunningState != RUNNINGSTATE_PROFILING )
				return STATUS_ERROR_INVALID_INSTRUCTION;
			return STATUS_TO_CONTINUE;
		}

	CPUState->Registers.Z	= Result == 0;
	CPUState->Registers.N	= (int16_t)Result < 0;
	CPUState->FlagsUpdated	= 1;

	return StoreResult( CPUState, OpCode16.ModeDst, OpCode16.RegDst, Result, Offset );
	}
/*============================================================================*/
static csimstatuscodes_t CSIMInstructions2Operands( csimcpustate_t *CPUState, uint8_t OpCode )
	{
	csimstatuscodes_t 	Status;
	opcode2op_t			OpCode16;
	uint8_t				Operation;
	int					Temp;
	uint16_t			Operand1, Operand2, Result, Offset;

	/* Get the second byte of the instruction. */
	if(( Temp = FetchByte( CPUState )) < 0 )
		return STATUS_ERROR_INVALID_ADDRESS;

	OpCode16.OpCode8	= OpCode;
	OpCode16.OpCodeLow	= (uint8_t)Temp;

	if(( Status = ProcessOperand( CPUState, OpCode16.ModeSrc, OpCode16.RegSrc, &Operand1, &Offset, 0 )) < STATUS_TO_CONTINUE )
		return Status;

	Operation	= ( OpCode >> 4 ) & 0x0f;

	if(( Status = ProcessOperand( CPUState, OpCode16.ModeDst, OpCode16.RegDst, &Operand2, &Offset, Operation == 0x09 )) < STATUS_TO_CONTINUE )
		return Status;

	switch( Operation )
		{
		case 0x09:	/* MOV */
			/* wCesar clears overflow flag, WHY? */
			Result	= Operand1;
			CPUState->Registers.V	= 0;
			break;
		case 0x0a:	/* ADD */
			Result	= Operand2 + Operand1;
			CPUState->Registers.C	= Result < Operand2;
			CPUState->Registers.V	= (( Operand2 ^ Operand1 ) & 0x8000 ) == 0 && (( Operand2 ^ Result ) & 0x8000 ) != 0 ;
			break;
		case 0x0b:	/* SUB */
			Result	= Operand2 - Operand1;
			CPUState->Registers.C	= Result > Operand2;
			CPUState->Registers.V	= (( Operand2 ^ Operand1 ) & 0x8000 ) != 0 && (( Operand2 ^ Result ) & 0x8000 ) != 0 ;
			break;
		case 0x0c:	/* CMP */
			Result	= Operand1 - Operand2;
			CPUState->Registers.Z	= Result == 0;
			CPUState->Registers.N	= (int16_t)Result < 0;
			CPUState->Registers.C	= Result > Operand1;
			CPUState->Registers.V	= (( Operand2 ^ Operand1 ) & 0x8000 ) != 0 && (( Operand2 ^ Result ) & 0x8000 ) != 0 ;
			CPUState->FlagsUpdated	= 1;
			/* We should not write anything back. */
			return STATUS_TO_CONTINUE;
		case 0x0d:	/* AND */
			Result	= Operand2 & Operand1;
			/* wCesar clears overflow flag, WHY? */
			CPUState->Registers.V	= 0;
			break;
		case 0x0e:	/* OR */
			Result	= Operand2 | Operand1;
			/* wCesar clears overflow flag, WHY? */
			CPUState->Registers.V	= 0;
			break;
		default:	/* Should never happen. */
			return STATUS_ERROR_INVALID_INSTRUCTION;
		}

	CPUState->Registers.Z	= Result == 0;
	CPUState->Registers.N	= (int16_t)Result < 0;
	CPUState->FlagsUpdated	= 1;

	return StoreResult( CPUState, OpCode16.ModeDst, OpCode16.RegDst, Result, Offset );
	}
/*============================================================================*/
static csimstatuscodes_t CSIMSimulatorLoop( csimcpustate_t *CPUState )
	{
	while( 1 )
		{
		csimstatuscodes_t	Status	= STATUS_TO_CONTINUE;
		int					OpCode;
		uint16_t			InstrStartAddress;

		if( /*CPUState->RunningState == RUNNINGSTATE_PROFILING &&*/ CPUState->Map != NULL )
			{
			if( CPUState->RunningState == RUNNINGSTATE_PROFILING && CPUState->Map[CPUState->Registers.R7].Visited )
				{
				CPUState->RunningState	= RUNNINGSTATE_STOPPED;
				return STATUS_TO_CONTINUE;
				}
			CPUState->Map[CPUState->Registers.R7].Visited		= 1;
			CPUState->Map[CPUState->Registers.R7].IsFirstByte	= 1;
			if( CPUState->RunningState != RUNNINGSTATE_PROFILING )
				{
				if( !CPUState->Map[CPUState->Registers.R7].Executed )
					CPUState->PreviousRegisters.Regs[7]	= CPUState->Registers.Regs[7];
				CPUState->Map[CPUState->Registers.R7].Executed	= 1;
				}
			}

		/* We only call the ISR if the global 'Interupt Enable' ('I') is active and the
		  'Interrupt Being Served' ('i') is inactive, together with any combination
		  of 'T & t', 'K & k' and 'S & s' true.

		  We include a latency so the CPU can execute at least one instruction between
		  two calls to the ISR. That allows software single-step code execution. */
		if( CPUState->RunningState != RUNNINGSTATE_PROFILING && !CPUState->Registers.i && CPUState->Registers.I && ( CPUState->Registers.INTE & CPUState->Registers.INTS & 0x07 ) != 0 && ( CPUState->Latency == 0 || CPUState->Latency-- == 0 ))
			{
			CPUState->Registers.R6		   -= 2;
			CPUState->RegsUpdated[6]		= 1;
			if( StoreWord( CPUState, CPUState->Registers.R6, CPUState->Registers.R7 ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			CPUState->Registers.R6		   -= 2;
			CPUState->RegsUpdated[6]		= 1;
			if( StoreWord( CPUState, CPUState->Registers.R6, (uint16_t)CPUState->Registers.Flags ) < 0 )
				return STATUS_ERROR_INVALID_ADDRESS;
			ReadWord( CPUState, ADDRESS_IVET, &CPUState->Registers.R7 );
			/* We must set the 'Interrupt Being Served' ('i') flag, so the ISR is not called recursively.
			   This flag can only be cleared by the 'RTI' instruction. */
			CPUState->Registers.i			= 1;
			return STATUS_TO_CONTINUE;
			}

		InstrStartAddress	= CPUState->Registers.R7;

		if(( OpCode = FetchByte( CPUState )) < 0 )
			{
			CPUState->Registers.R7			= InstrStartAddress;
			CPUState->RegsUpdated[7]		= 1;
			return STATUS_ERROR_INVALID_ADDRESS;
			}

		switch( OpCode >> 4 & 0x0f )
			{
			case 0x00:	/* NOP */
				if( CPUState->Pedantic && ( OpCode & 0x0f ) != 0 )
					return STATUS_ERROR_INVALID_INSTRUCTION;
				Status	= STATUS_TO_CONTINUE;
				break;
			case 0x01:	/* CCC */
				Status	= CSIMInstructionCCC( CPUState, (uint8_t)OpCode );
				break;
			case 0x02:	/* SCC */
				Status	= CSIMInstructionSCC( CPUState, (uint8_t)OpCode );
				break;
			case 0x03:	/* BR, Bxx */
				Status	= CSIMInstructionBxx( CPUState, (uint8_t)OpCode );
				break;
			case 0x04:	/* JMP */
				if( CPUState->Pedantic && ( OpCode & 0x0f ) != 0 )
					return STATUS_ERROR_INVALID_INSTRUCTION;
				Status	= CSIMInstructionJMP( CPUState, (uint8_t)OpCode );
				break;
			case 0x05:	/* SOB */
				Status	= CSIMInstructionSOB( CPUState, (uint8_t)OpCode );
				break;
			case 0x06:	/* JSR */
				Status	= CSIMInstructionJSR( CPUState, (uint8_t)OpCode );
				break;
			case 0x07:	/* RTS, RTI */
				Status	= CSIMInstructionRTS_RTI( CPUState, (uint8_t)OpCode );
				break;
			case 0x08:	/* CLR, NOT, INC, DEC, NEG, TST, ROR, ROL, ASR, ASL, ADC, SBC */
				Status	= CSIMInstructions1Operand( CPUState, (uint8_t)OpCode );
				break;	/* MOV, ADD, SUB, CMP, AND, OR */
			case 0x09: case 0x0a: case 0x0b: case 0x0c: case 0x0d: case 0x0e:
				Status	= CSIMInstructions2Operands( CPUState, (uint8_t)OpCode );
				break;
			case 0x0f:	/* HLT */
				/* Let's put R7 back at the beginning of the instruction, so even if we try
				   we cannot continue past this instruction. */
				CPUState->Registers.R7		   -= 1;
				CPUState->RegsUpdated[7]		= 1;
				if( CPUState->Pedantic && ( OpCode & 0x0f ) != 0 )
					return STATUS_ERROR_INVALID_INSTRUCTION;
				return STATUS_HALTED;
			}

		if( Status < STATUS_HALTED )
			{
			CPUState->Registers.R7			= InstrStartAddress;
			CPUState->RegsUpdated[7]		= 1;
			return Status;
			}
		else if( Status == STATUS_HALTED )
			return Status;

		return STATUS_TO_CONTINUE;
		}
	}
/*============================================================================*/
static void CSIMDoProfileLoop( csimcpustate_t *CPUState, uint16_t StartAddress )
	{
	CPUState->ProfilerStackPointer								= 0;
	CPUState->ProfilerStack[CPUState->ProfilerStackPointer++]	= StartAddress;

	while( CPUState->ProfilerStackPointer > 0 )
		{
		CPUState->Registers.R7			= CPUState->ProfilerStack[--CPUState->ProfilerStackPointer];
		CPUState->RegsUpdated[7]		= 1;
		CPUState->RunningState			= RUNNINGSTATE_PROFILING;
		do
			CSIMSimulatorLoop( CPUState );
		while( CPUState->RunningState == RUNNINGSTATE_PROFILING );
		}
	}
/*============================================================================*/
int SimulatorDoProfile( csimcpustate_t *CPUState )
	{
	uint16_t	IVEC;
	int			Pedantic;

	Pedantic			= CPUState->Pedantic;
	CPUState->Pedantic	= 0;

	if( !CPUState->CPUStateOK )
		{
		CPUState->Pedantic	= Pedantic;
		return -1;
		}

	if(( CPUState->ProfilerStack = malloc( MEMORY_SIZE * sizeof *CPUState->ProfilerStack )) == NULL )
		{
		CPUState->Pedantic	= Pedantic;
		return -1;
		}

	if( CPUState->Map == NULL && ( CPUState->Map = malloc( MEMORY_SIZE * sizeof *CPUState->Map )) == NULL )
		{
		free( CPUState->ProfilerStack );
		CPUState->ProfilerStack	= NULL;
		CPUState->Pedantic		= Pedantic;
		return -1;
		}

	memset( CPUState->ProfilerStack, 0x00, MEMORY_SIZE * sizeof *CPUState->ProfilerStack );
	memset( CPUState->Map, 0x00, MEMORY_SIZE * sizeof *CPUState->Map );

	CSIMDoProfileLoop( CPUState, 0x0000 );
	IVEC	= ( (uint16_t)CPUState->Memory[0xffbe] << 8 ) | ( (uint16_t)CPUState->Memory[0xffbf] << 0 );
	if( IVEC != 0 )
		CSIMDoProfileLoop( CPUState, IVEC );

	free( CPUState->ProfilerStack );
	CPUState->ProfilerStack	= NULL;

	CPUState->Pedantic	= Pedantic;
	return 1;
	}
/*============================================================================*/
int SimulatorResetCPU( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return -1;

	if( CPUState->Memory == NULL || CPUState->PristineMemory == NULL )
		return -1;

	memcpy( CPUState->Memory, CPUState->PristineMemory, MEMORY_SIZE );

	memset( &CPUState->PreviousRegisters, 0x00, sizeof CPUState->Registers );
	memset( &CPUState->Registers, 0x00, sizeof CPUState->Registers );
	memset( &CPUState->Memory[ADDRESS_LCD], ' ', LENGTH_LCD );
	memset( &CPUState->RegsUpdated, 0x00, sizeof CPUState->RegsUpdated );
	if( CPUState->MemoryChanged != NULL )
		BitVectorClearAllBits( CPUState->MemoryChanged );
	if( CPUState->MemoryUpdated != NULL )
		BitVectorClearAllBits( CPUState->MemoryUpdated );

	CPUState->FlagsUpdated	=   0;
	CPUState->TIMDTUpdated	=   0;
	CPUState->TECSTUpdated	=   0;
	CPUState->INTSUpdated	=   0;
	CPUState->INTEUpdated	=   0;

	CPUState->ChangedStep	=   1;
	CPUState->Latency		=   0;
	CPUState->JSRCount		=   0;
	CPUState->StepOverR6	=   0;
	CPUState->StepOverR7	=   0;
	CPUState->Delay			= 398;
	CPUState->InsnCount		=   0;
	CPUState->RunningState	= RUNNINGSTATE_STOPPED;
	CPUState->ChangedMem	=   1;
	CPUState->ChangedLCD	=   1;

	return 1;
	}
/*============================================================================*/
#if         defined __WXGTK__
static uint64_t GetTickCount()
    {
    struct timespec ts;

    clock_gettime( CLOCK_MONOTONIC, &ts );

    return (uint64_t)( ts.tv_nsec / 1000000 ) + ( (uint64_t)ts.tv_sec * 1000ull );
    }
#endif  //  defined __WXGTK__
/*============================================================================*/
csimstatuscodes_t SimulatorLoop( csimcpustate_t *CPUState )
	{
	uint32_t	t2	= GetTickCount();
	int			Status;

	if( !CPUState->CPUStateOK )
		return CPUState->SimulatorStatus = STATUS_ERROR_INTERNAL_ERROR;

	if( CPUState->RunningState == RUNNINGSTATE_STOPPED )
		{
		CPUState->TimerTick				= t2 + ( CPUState->Registers.TIMDT != 0 ? CPUState->Registers.TIMDT : 100 );
		CPUState->t1					= t2;
		CPUState->Busy					= 0;
		CPUState->Running				= 0;
		CPUState->InsnCount				= 0;
		Sleep( 1 );
		return STATUS_STOPPED;
		}
	else if( CPUState->RunningState == RUNNINGSTATE_ANIMATING )
		{
		CPUState->TimerTick				= t2 + ( CPUState->Registers.TIMDT != 0 ? CPUState->Registers.TIMDT : 100 );
		if(( t2 - CPUState->t1 ) < CPUState->Delay )
			{
			CPUState->Busy				= 0;
			return STATUS_TO_CONTINUE;
			}
		/* We are running too slow for real time, let's generate our timer interrupt by counting instructions. */
		if( ++CPUState->InsnCount > 100 * ( CPUState->Registers.TIMDT != 0 ? CPUState->Registers.TIMDT : 100 ))
			{
			CPUState->InsnCount			= 0;
			if( CPUState->Registers.t )
				CPUState->TimerOverRun	= 1;
			CPUState->Registers.t		= 1;
			}
		/* Let's keep our reference time updated to be prepared for a change in running mode. */
		CPUState->t1					= t2;
		}
	else if( CPUState->RunningState == RUNNINGSTATE_RUNNINGFAST || CPUState->RunningState == RUNNINGSTATE_RUNNING || CPUState->RunningState == RUNNINGSTATE_STEPOVER )
		{
		/* ...So let's generate our timer interrupt from the system tick. */
		if( (int)( CPUState->TimerTick - t2 ) <= 0 )
			{
			CPUState->TimerTick			= t2 + ( CPUState->Registers.TIMDT != 0 ? CPUState->Registers.TIMDT : 100 );
			if( CPUState->Registers.t )
				CPUState->TimerOverRun	= 1;
			CPUState->Registers.t		= 1;
			}
		/* Let's keep the instruction count at zero to be prepared for a change in running mode. */
		CPUState->InsnCount				= 0;
		CPUState->t1					= t2;
		}

	CPUState->Busy		= 1;

	if( CPUState->RunningState < RUNNINGSTATE_RUNNINGFAST )
		{
		uint8_t	Temp						= CPUState->PreviousRegisters.TECDT;
		memcpy( &CPUState->PreviousRegisters, &CPUState->Registers, sizeof CPUState->PreviousRegisters );
		memset( &CPUState->RegsUpdated, 0x00, sizeof CPUState->RegsUpdated );
		CPUState->FlagsUpdated				= 0;
		CPUState->TIMDTUpdated				= 0;
		CPUState->TECSTUpdated				= 0;
		CPUState->INTSUpdated				= 0;
		CPUState->INTEUpdated				= 0;
		CPUState->PreviousRegisters.TECDT	= Temp;
		if( CPUState->MemoryChanged != NULL )
			BitVectorClearAllBits( CPUState->MemoryChanged );
		if( CPUState->MemoryUpdated != NULL )
			BitVectorClearAllBits( CPUState->MemoryUpdated );
		}

	Status	= CSIMSimulatorLoop( CPUState );
	if( Status < STATUS_TO_CONTINUE )
		{
		CPUState->Running	= 0;
		CPUState->RunningState	= RUNNINGSTATE_STOPPED;
		}
	else if( CPUState->BreakPointsExec != NULL && BitVectorQueryBit( CPUState->BreakPointsExec, CPUState->Registers.R7 ) != 0 )
		{
		Status					= STATUS_BREAKPOINT_EXEC;
		CPUState->Running		= 0;
		CPUState->RunningState	= RUNNINGSTATE_STOPPED;
		}

	CPUState->ChangedStep	= 1;

	if( CPUState->RunningState == RUNNINGSTATE_SINGLESTEP )
		CPUState->RunningState = RUNNINGSTATE_STOPPED;

	CPUState->Busy			= 0;

	return CPUState->SimulatorStatus = Status;
	}
/*============================================================================*/
void SimulatorClearTraceHistory( csimcpustate_t *CPUState )
	{
	int	i;

	if( !CPUState->CPUStateOK )
		return;

	if( CPUState->Map == NULL )
		return;

	for( i = 0; i < MEMORY_SIZE; i++ )
		{
		CPUState->Map[i].Executed	= 0;
		CPUState->Map[i].InISR		= 0;
		CPUState->Map[i].InThread	= 0;
		}
	}
/*============================================================================*/
int SimulatorInitialize( csimcpustate_t *CPUState )
	{
	memset( CPUState, 0x00, sizeof *CPUState );

	if(( CPUState->Memory = malloc( MEMORY_SIZE )) == NULL )
		return -1;

	if(( CPUState->PristineMemory = malloc( MEMORY_SIZE )) == NULL )
		{
		free( CPUState->Memory );
		CPUState->Memory			= NULL;
		return -1;
		}

	if(( CPUState->Map = malloc( MEMORY_SIZE * sizeof *CPUState->Map )) == NULL )
		{
		free( CPUState->Memory );
		CPUState->Memory			= NULL;
		free( CPUState->PristineMemory );
		CPUState->PristineMemory	= NULL;
		return -1;
		}

	CPUState->MemoryUpdated		= BitVectorCreate( RAM_SIZE );
	CPUState->MemoryChanged		= BitVectorCreate( RAM_SIZE );
	CPUState->BreakPointsRead	= BitVectorCreate( RAM_SIZE );
	CPUState->BreakPointsWrite	= BitVectorCreate( RAM_SIZE );
	CPUState->BreakPointsExec	= BitVectorCreate( RAM_SIZE );

	memset( CPUState->Memory, 0x00, MEMORY_SIZE );
	memset( CPUState->PristineMemory, 0x00, MEMORY_SIZE );
	memset( CPUState->Map, 0x00, MEMORY_SIZE * sizeof *CPUState->Map );
	CPUState->Delay	= 398;

	SimulatorResetCPU( CPUState );

	CPUState->CPUStateOK	= 1;

	return 0;
	}
/*============================================================================*/
int SimulatorFinalize( csimcpustate_t *CPUState )
	{
	if( CPUState->PristineMemory != NULL )
		{
		free( CPUState->PristineMemory );
		CPUState->PristineMemory	= NULL;
		}
	if( CPUState->Memory != NULL )
		{
		free( CPUState->Memory );
		CPUState->Memory			= NULL;
		}
	if( CPUState->Map != NULL )
		{
		free( CPUState->Map );
		CPUState->Map				= NULL;
		}
	if( CPUState->ProfilerStack != NULL )
		{
		free( CPUState->ProfilerStack );
		CPUState->ProfilerStack		= NULL;
		}
	if( CPUState->SymbolIndex != NULL )
		{
		free( CPUState->SymbolIndex );
		CPUState->SymbolIndex		= NULL;
		}
	if( CPUState->SymbolTable != NULL )
		{
		free( CPUState->SymbolTable );
		CPUState->SymbolTable		= NULL;
		}
	if( CPUState->MemoryChanged != NULL )
		{
		free( CPUState->MemoryChanged );
		CPUState->MemoryChanged		= NULL;
		}
	memset( CPUState, 0x00, sizeof *CPUState );

	return 1;
	}
/*============================================================================*/
int SimulatorLoadFile( csimcpustate_t *CPUState, const char * const FileName )
	{
	FILE	*File;
	uint8_t	Temp[4];

	if( !CPUState->CPUStateOK )
		return -1;

	if( CPUState->Memory == NULL || CPUState->PristineMemory == NULL )
		return -1;

	if(( File = fopen( FileName, "rb" )) == NULL )
		return -1;

	if( fread( Temp, 1, 4, File ) != 4 )
		{
		fclose( File );
		return -1;
		}

	if( memcmp( Temp, "\x03" "C16", sizeof Temp ) != 0 )
		{
		fclose( File );
		return -1;
		}

	if( fread( CPUState->PristineMemory, 1, MEMORY_SIZE, File ) != MEMORY_SIZE )
		{
		fclose( File );
		return -2;
		}

	memcpy( CPUState->Memory, CPUState->PristineMemory, MEMORY_SIZE );
	if( CPUState->Map != NULL )
		memset( CPUState->Map, 0x00, MEMORY_SIZE * sizeof *CPUState->Map );
	if( CPUState->MemoryChanged != NULL )
		BitVectorClearAllBits( CPUState->MemoryChanged );
	if( CPUState->MemoryUpdated != NULL )
		BitVectorClearAllBits( CPUState->MemoryUpdated );

	fclose( File );
	return 0;
	}
/*============================================================================*/
int SimulatorGetLCDContents( csimcpustate_t *CPUState, char *Buffer )
	{
	if( !CPUState->CPUStateOK )
		return -1;

	memcpy( Buffer, &CPUState->Memory[ADDRESS_LCD], LENGTH_LCD );
	Buffer[LENGTH_LCD]	= '\0';

	return 0;
	}
/*============================================================================*/
int KeyboardInput( csimcpustate_t *CPUState, uint8_t Char )
	{
	int	OverRun;

	if( !CPUState->CPUStateOK )
		return -1;

	CPUState->PreviousRegisters.INTS	= CPUState->Registers.INTS;
	CPUState->PreviousRegisters.TECST	= CPUState->Registers.TECST;
	OverRun								= CPUState->Registers.A != 0;

	CPUState->Registers.TECDT			= Char;
	CPUState->Registers.A				= 1;
	CPUState->Registers.k				= 1;

	return OverRun;
	}
/*============================================================================*/
int SimulatorTimerElapsed( csimcpustate_t *CPUState )
	{
	int	OverRun;

	if( !CPUState->CPUStateOK )
		return -1;

	OverRun						= CPUState->Registers.t != 0;
	CPUState->KeyboardOverRun	= OverRun;

//	if( CPUState->Registers.t == 0 )
//		CPUState->ChangedINTS	= 1;
	CPUState->Registers.t		= 1;

	return OverRun;
	}
/*============================================================================*/
void SimulatorRun( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return;

	memcpy( &CPUState->PreviousRegisters, &CPUState->Registers, sizeof CPUState->PreviousRegisters );
	CPUState->RunningState	= RUNNINGSTATE_RUNNING;
	CPUState->Busy			= 1;
	CPUState->Running		= 1;
	}
/*============================================================================*/
void SimulatorMaxSpeed( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return;

	memcpy( &CPUState->PreviousRegisters, &CPUState->Registers, sizeof CPUState->PreviousRegisters );
	CPUState->RunningState	= RUNNINGSTATE_RUNNINGFAST;
	CPUState->Busy			= 1;
	CPUState->Running		= 1;
	}
/*============================================================================*/
void SimulatorPause( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return;

	CPUState->RunningState	= RUNNINGSTATE_STOPPED;
	}
/*============================================================================*/
void SimulatorAnimate( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return;

	CPUState->RunningState	= RUNNINGSTATE_ANIMATING;
	CPUState->Busy			= 1;
	CPUState->Running		= 1;
	}
/*============================================================================*/
void SimulatorStepInto( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return;

	CPUState->RunningState	= RUNNINGSTATE_SINGLESTEP;
	CPUState->Busy			= 1;
	CPUState->Running		= 1;
	}
/*============================================================================*/
int SimulatorStepOver( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return 0;

	CPUState->Busy				= 1;
	CPUState->Running			= 1;

	/* We are about to execute a 'jsr' instruction, we must really do a 'Step Over'... */
	if( CPUState->Memory[CPUState->Registers.R7] == 0x67 )
		{
		memcpy( &CPUState->PreviousRegisters, &CPUState->Registers, sizeof CPUState->PreviousRegisters );
		CPUState->StepOverR6	= CPUState->Registers.R6;
		CPUState->StepOverR7	= CPUState->Registers.R7 + 4;
		CPUState->RunningState	= RUNNINGSTATE_STEPOVER;
		return 1;
		}
	/* For all other instructions, it is just a 'Step Into'... */
	else
		{
		CPUState->RunningState	= RUNNINGSTATE_SINGLESTEP;
		return 0;
		}
	}
/*============================================================================*/
void SimulatorClearTrace( csimcpustate_t *CPUState )
	{
	if( !CPUState->CPUStateOK )
		return;

	SimulatorClearTraceHistory( CPUState );
	}
/*============================================================================*/
void SimulatorUpdateTIMDT( csimcpustate_t *CPUState, uint8_t Value )
	{
	uint32_t	t1;

	if( !CPUState->CPUStateOK )
		return;

	t1	= GetTickCount();

	CPUState->Registers.TIMDT	= Value;
	CPUState->t1				= t1;
	CPUState->TimerTick			= t1 + ( CPUState->Registers.TIMDT != 0 ? CPUState->Registers.TIMDT : 100 );
	}
/*============================================================================*/
void SimulatorSetMemWindow( csimcpustate_t *CPUState, uint16_t MemWindowStart, uint16_t MemWindowEnd )
	{
	CPUState->MemWindowStart	= MemWindowStart;
	CPUState->MemWindowEnd		= MemWindowEnd;
	}
/*============================================================================*/
