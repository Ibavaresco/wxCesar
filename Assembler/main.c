/*============================================================================*/
/*
 casm - Assembler for the abstract computer 'Cesar'
 <https://www.inf.ufrgs.br/arq/wiki/doku.php?id=cesar>
================================================================================
 Copyright (c) 2007-2024, Isaac Marino Bavaresco
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
*/
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
/*============================================================================*/
#define	LENGTH(a)	(sizeof(a)/sizeof(a)[0])
/*============================================================================*/
#define	NUM_PASSES		2
/*============================================================================*/
#define	OPCODE_NOP		0x00
#define	OPCODE_HLT		0x01

#define	OPCODE_JMP		0x40

#define	OPCODE_BR		0x30
#define	OPCODE_BNE		0x31
#define	OPCODE_BEQ		0x32
#define	OPCODE_BPL		0x33
#define	OPCODE_BMI		0x34
#define	OPCODE_BVC		0x35
#define	OPCODE_BVS		0x36
#define	OPCODE_BCC		0x37
#define	OPCODE_BCS		0x38
#define	OPCODE_BGE		0x39
#define	OPCODE_BLT		0x3a
#define	OPCODE_BGT		0x3b
#define	OPCODE_BLE		0x3c
#define	OPCODE_BHI		0x3d
#define	OPCODE_BLS		0x3e

#define	OPCODE_CLR		0x80
#define	OPCODE_NOT		0x81
#define	OPCODE_INC		0x82
#define	OPCODE_DEC		0x83
#define	OPCODE_NEG		0X84
#define	OPCODE_TST		0X85
#define	OPCODE_ROR		0X86
#define	OPCODE_ROL		0X87
#define	OPCODE_ASR		0X88
#define	OPCODE_ASL		0X89
#define	OPCODE_ADC		0X8a
#define	OPCODE_SBC		0X8b

#define	OPCODE_MOV		0x90
#define	OPCODE_ADD		0xa0
#define	OPCODE_SUB		0xb0
#define	OPCODE_CMP		0xc0
#define	OPCODE_AND		0xd0
#define	OPCODE_OR		0xe0

#define	OPCODE_JSR		0x60
#define	OPCODE_RTS		0x70

#define	OPCODE_RTI		0x78

#define	OPCODE_CCC		0x10
#define	OPCODE_SCC      0x20

#define	OPCODE_SOB		0x50
/*============================================================================*/
typedef enum
	{
	TOKEN_NONE,

	TOKEN_INVALID,

	TOKEN_EOF,
	TOKEN_EOL,

	TOKEN_COLON,
	TOKEN_COMMA,
	TOKEN_OPENPAR,
	TOKEN_CLOSEPAR,
	TOKEN_OPEN_BRACKET,
	TOKEN_CLOSE_BRACKET,
	TOKEN_SHARP,
	TOKEN_EQUAL_TO,
	TOKEN_NOT_EQUAL_TO,
	TOKEN_SHIFT_LEFT,
	TOKEN_SHIFT_RIGHT,
	TOKEN_BINARY_OR,
	TOKEN_BINARY_AND,
	TOKEN_BINARY_XOR,
	TOKEN_LOGICAL_OR,
	TOKEN_LOGICAL_AND,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_ASTERISK,
	TOKEN_SLASH,
	TOKEN_PERCENT,
	TOKEN_CURRENCY,
	TOKEN_LESS_THAN,
	TOKEN_LESS_THAN_OR_EQUAL,
	TOKEN_GREATER_THAN,
	TOKEN_GREATER_THAN_OR_EQUAL,

	TOKEN_REGISTER,
	TOKEN_INSTRUCTION,
	TOKEN_DIRECTIVE,
	TOKEN_IDENTIFIER,

	TOKEN_NUMBER,
	TOKEN_NUMBERHEX,
	TOKEN_CHARACTER,

	TOKEN_STRING
	} token_t;
/*============================================================================*/
typedef enum
	{
	/*========================================================================*/
	INDEX_NONE			= -1,
	/*========================================================================*/
	INSTRUCTION_NONE	= -1,
	/*========================================================================*/
	/* Keep the values below together and in the same order */
	/*------------------------------------------------------------------------*/
	INSTRUCTION_NOP,
	INSTRUCTION_HLT,
	INSTRUCTION_RTI,
	/*------------------------------------------------------------------------*/
	INSTRUCTION_JMP,
	/*------------------------------------------------------------------------*/
	/* Keep the values below together and in the same order */
	/*------------------------------------------------------------------------*/
	INSTRUCTION_BR,
	INSTRUCTION_BNE,
	INSTRUCTION_BEQ,
	INSTRUCTION_BPL,
	INSTRUCTION_BMI,
	INSTRUCTION_BVC,
	INSTRUCTION_BVS,
	INSTRUCTION_BCC,
	INSTRUCTION_BCS,
	INSTRUCTION_BGE,
	INSTRUCTION_BLT,
	INSTRUCTION_BGT,
	INSTRUCTION_BLE,
	INSTRUCTION_BHI,
	INSTRUCTION_BLS,
	/*------------------------------------------------------------------------*/
	INSTRUCTION_JSR,
	/*------------------------------------------------------------------------*/
	INSTRUCTION_RTS,
	/*------------------------------------------------------------------------*/
	/* Keep the values below together and in the same order */
	/*------------------------------------------------------------------------*/
	INSTRUCTION_CLR,
	INSTRUCTION_NOT,
	INSTRUCTION_INC,
	INSTRUCTION_DEC,
	INSTRUCTION_NEG,
	INSTRUCTION_TST,
	INSTRUCTION_ROR,
	INSTRUCTION_ROL,
	INSTRUCTION_ASR,
	INSTRUCTION_ASL,
	INSTRUCTION_ADC,
	INSTRUCTION_SBC,
	/*------------------------------------------------------------------------*/
	/* Keep the values below together and in the same order */
	/*------------------------------------------------------------------------*/
	INSTRUCTION_MOV,
	INSTRUCTION_ADD,
	INSTRUCTION_SUB,
	INSTRUCTION_CMP,
	INSTRUCTION_AND,
	INSTRUCTION_OR,
	/*------------------------------------------------------------------------*/
	/* Keep the values below together and in the same order */
	/*------------------------------------------------------------------------*/
	INSTRUCTION_CCC,
	INSTRUCTION_SCC,
	/*------------------------------------------------------------------------*/
	INSTRUCTION_SOB,
	/*========================================================================*/
	DIRECTIVE_NONE		= -1,
	/*========================================================================*/
	DIRECTIVE_EQU,
	DIRECTIVE_ORG,
	DIRECTIVE_DB,
	DIRECTIVE_DAB,
	DIRECTIVE_DW,
	DIRECTIVE_DAW,
	/*========================================================================*/
	REGISTER_NONE		= -1,
	/*========================================================================*/
	REGISTER_R0,
	REGISTER_R1,
	REGISTER_R2,
	REGISTER_R3,
	REGISTER_R4,
	REGISTER_R5,
	REGISTER_R6,
	REGISTER_R7
	/*========================================================================*/
	} index_t;
/*============================================================================*/
typedef enum
	{
	TYPE_INVALID	= -1,

	TYPE_UNDEFINED,

	TYPE_NUMBER,
	TYPE_ADDRESS,

	TYPE_IMMEDIATE,
	TYPE_DIRECT,

	TYPE_REGISTER,
	TYPE_REGISTER_POSTINC,
	TYPE_REGISTER_PREDEC,
	TYPE_REGISTER_INDEXED,
	TYPE_REGISTER_INDIRECT,
	TYPE_REGISTER_POSTINC_INDIRECT,
	TYPE_REGISTER_PREDEC_INDIRECT,
	TYPE_REGISTER_INDEXED_INDIRECT
	} type_t;
/*============================================================================*/
typedef struct
	{
	char		*Name;
	token_t		Token;
	index_t		Index;
	} pairs_t;
/*============================================================================*/
typedef struct
	{
	char		Name[64];
	int			Value;
	type_t		Type;
	} symbol_t;
/*============================================================================*/
typedef struct
	{
	uint32_t	CurrentLocation;
	uint32_t	MaximumAddress;
	uint8_t		*Memory;
	symbol_t	*SymbolTable;
	unsigned	NumSymbols;
	unsigned	SymbolTableLength;
	unsigned	Pass;
	} state_t;
/*============================================================================*/
typedef struct
	{
	type_t			Type;
	union
		{
		struct
			{
			int		Offset;
			int		Register;
			};
		uint32_t	Number;
		};
	} result_t;
/*============================================================================*/
static pairs_t	ReservedWords[]	=
	{
		{ "nop", TOKEN_INSTRUCTION, INSTRUCTION_NOP },
		{ "hlt", TOKEN_INSTRUCTION, INSTRUCTION_HLT },

		{ "jmp", TOKEN_INSTRUCTION, INSTRUCTION_JMP },

		{ "br",  TOKEN_INSTRUCTION, INSTRUCTION_BR  },
		{ "bne", TOKEN_INSTRUCTION, INSTRUCTION_BNE }, { "bnz", TOKEN_INSTRUCTION, INSTRUCTION_BNE },	/* Synonyms: bnz == bne */
		{ "beq", TOKEN_INSTRUCTION, INSTRUCTION_BEQ }, { "bz",  TOKEN_INSTRUCTION, INSTRUCTION_BEQ },	/* Synonyms: bz  == beq */
		{ "bpl", TOKEN_INSTRUCTION, INSTRUCTION_BPL }, { "bmi", TOKEN_INSTRUCTION, INSTRUCTION_BMI },
		{ "bvc", TOKEN_INSTRUCTION, INSTRUCTION_BVC }, { "bvs", TOKEN_INSTRUCTION, INSTRUCTION_BVS },
		{ "bcc", TOKEN_INSTRUCTION, INSTRUCTION_BCC }, { "bcs", TOKEN_INSTRUCTION, INSTRUCTION_BCS },
		{ "bge", TOKEN_INSTRUCTION, INSTRUCTION_BGE }, { "blt", TOKEN_INSTRUCTION, INSTRUCTION_BLT },
		{ "bgt", TOKEN_INSTRUCTION, INSTRUCTION_BGT }, { "ble", TOKEN_INSTRUCTION, INSTRUCTION_BLE },
		{ "bhi", TOKEN_INSTRUCTION, INSTRUCTION_BHI }, { "bls", TOKEN_INSTRUCTION, INSTRUCTION_BLS },

		{ "jsr", TOKEN_INSTRUCTION, INSTRUCTION_JSR },

		{ "rts", TOKEN_INSTRUCTION, INSTRUCTION_RTS },
		{ "rti", TOKEN_INSTRUCTION, INSTRUCTION_RTI },

		{ "clr", TOKEN_INSTRUCTION, INSTRUCTION_CLR },
		{ "not", TOKEN_INSTRUCTION, INSTRUCTION_NOT },
		{ "inc", TOKEN_INSTRUCTION, INSTRUCTION_INC }, { "dec", TOKEN_INSTRUCTION, INSTRUCTION_DEC },
		{ "neg", TOKEN_INSTRUCTION, INSTRUCTION_NEG },
		{ "tst", TOKEN_INSTRUCTION, INSTRUCTION_TST },
		{ "ror", TOKEN_INSTRUCTION, INSTRUCTION_ROR }, { "rol", TOKEN_INSTRUCTION, INSTRUCTION_ROL },
		{ "asr", TOKEN_INSTRUCTION, INSTRUCTION_ASR }, { "asl", TOKEN_INSTRUCTION, INSTRUCTION_ASL },
		{ "adc", TOKEN_INSTRUCTION, INSTRUCTION_ADC }, { "sbc", TOKEN_INSTRUCTION, INSTRUCTION_SBC },

		{ "mov", TOKEN_INSTRUCTION, INSTRUCTION_MOV },
		{ "add", TOKEN_INSTRUCTION, INSTRUCTION_ADD }, { "sub", TOKEN_INSTRUCTION, INSTRUCTION_SUB },
		{ "cmp", TOKEN_INSTRUCTION, INSTRUCTION_CMP },
		{ "and", TOKEN_INSTRUCTION, INSTRUCTION_AND },
		{ "or",  TOKEN_INSTRUCTION, INSTRUCTION_OR  },

		{ "ccc", TOKEN_INSTRUCTION, INSTRUCTION_CCC },
		{ "scc", TOKEN_INSTRUCTION, INSTRUCTION_SCC },

		{ "sob", TOKEN_INSTRUCTION, INSTRUCTION_SOB },

		{ "r0",  TOKEN_REGISTER, REGISTER_R0 }, { "r1",  TOKEN_REGISTER, REGISTER_R1 },
		{ "r2",  TOKEN_REGISTER, REGISTER_R2 }, { "r3",  TOKEN_REGISTER, REGISTER_R3 },
		{ "r4",  TOKEN_REGISTER, REGISTER_R4 }, { "r5",  TOKEN_REGISTER, REGISTER_R5 },
		{ "r6",  TOKEN_REGISTER, REGISTER_R6 }, { "r7",  TOKEN_REGISTER, REGISTER_R7 },
		{ "sp",  TOKEN_REGISTER, REGISTER_R6 }, { "pc",  TOKEN_REGISTER, REGISTER_R7 },	/* Synonyms: sp == r6, pc == r7 */

		{ "equ", TOKEN_DIRECTIVE, DIRECTIVE_EQU },
		{ "org", TOKEN_DIRECTIVE, DIRECTIVE_ORG },
		{ "db",  TOKEN_DIRECTIVE, DIRECTIVE_DB  }, { "dab", TOKEN_DIRECTIVE, DIRECTIVE_DAB },
		{ "dw",  TOKEN_DIRECTIVE, DIRECTIVE_DW  }, { "daw", TOKEN_DIRECTIVE, DIRECTIVE_DAW }
	};
/*============================================================================*/
typedef struct
	{
	FILE		*File;
	unsigned	LineNumber;
	unsigned	Column;
	unsigned	TabWidth;
	char		Buffer[1024];
	} cookie_t;
/*============================================================================*/
typedef struct
	{
	long		FilePos;
	unsigned	LineNumber;
	unsigned	Column;
	} savepos_t;
/*============================================================================*/
static void SavePosition( cookie_t *Cookie, savepos_t *SaveBuffer )
	{
	SaveBuffer->FilePos		= ftell( Cookie->File );
	SaveBuffer->LineNumber	= Cookie->LineNumber;
	SaveBuffer->Column		= Cookie->Column;
	}
/*============================================================================*/
static void RestorePosition( cookie_t *Cookie, savepos_t *SaveBuffer )
	{
	fseek( Cookie->File, SaveBuffer->FilePos, SEEK_SET );
	Cookie->LineNumber		= SaveBuffer->LineNumber;
	Cookie->Column			= SaveBuffer->Column;
	}
/*============================================================================*/
static int GetChar( cookie_t *Cookie )
	{
	return getc( Cookie->File );
	}
/*============================================================================*/
static int UngetChar( cookie_t *Cookie, int c )
	{
	return ungetc( c, Cookie->File );
	}
/*============================================================================*/
static int Error( cookie_t *Cookie, const char *Msg )
	{
	fprintf( stderr, "\nError in line %u, column %u: %s\n", Cookie->LineNumber, Cookie->Column, Msg );
	return -1;
	}
/*============================================================================*/
static int Warning( cookie_t *Cookie, const char *Msg )
	{
	fprintf( stderr, "\nWarning in line %u, column %u: %s\n", Cookie->LineNumber, Cookie->Column, Msg );
	return -1;
	}
/*============================================================================*/
#if			defined __linux__
/*============================================================================*/
static int strnicmp( const char *a, const char *b, size_t n )
    {
    for( ; n > 0 && *a != '\0' && *b != '\0' && toupper( *a ) == toupper( *b ); a++, b++, n-- )
        {}

    return toupper( *a ) - toupper( *b );
    }
/*============================================================================*/
static int stricmp( const char *a, const char *b )
    {
    for( ; *a != '\0' && *b != '\0' && *a == *b; a++, b++ )
        {}

    return *a - *b;
    }
/*============================================================================*/
#endif	/*	defined __linux__ */
/*============================================================================*/
static type_t FindSymbol( cookie_t *Cookie, state_t *State, const char * const Name, int *Value, type_t *Type )
	{
	int i;

	for( i = 0; i < State->NumSymbols && strnicmp( Name, State->SymbolTable[i].Name, sizeof State->SymbolTable[i].Name ) != 0; i++ )
		{}

	/* We did not find the symbol in the symbol table... */
	if( i >= State->NumSymbols )
		{
		/* ...but we are doing the first pass... */
		if( State->Pass < NUM_PASSES )
			{
			/* ...so we will fake the symbol with a value of zero. */
			if( Value != NULL )
				*Value	= 0;
			if( Type != NULL )
				*Type	= TYPE_UNDEFINED;
			return TYPE_UNDEFINED;
			}
		/* ...that's an irrecoverable error. */
		Error( Cookie, "Symbol not found" );
		return TYPE_INVALID;
		}

	if( Value != NULL )
		*Value	= State->SymbolTable[i].Value;
	if( Type != NULL )
		*Type	= State->SymbolTable[i].Type;

	return State->SymbolTable[i].Type;
	}
/*============================================================================*/
static int InsertSymbol( cookie_t *Cookie, state_t *State, const char * const Name, int Value, type_t Type )
	{
	int	i;

	for( i = 0; i < State->NumSymbols; i++ )
		if( strnicmp( Name, State->SymbolTable[i].Name, sizeof State->SymbolTable[i].Name ) == 0 )
			{
			if( State->SymbolTable[i].Value != Value )
				return Error( Cookie, "Symbol redefined with a different value" );
			if( State->SymbolTable[i].Type != Type )
				return Error( Cookie, "Symbol redefined with a different type" );
			//if( State->Pass < NUM_PASSES )
			//	Warning( Cookie, "Symbol redefined with same type and value" );
			return 2;
			}

	if( i >= State->SymbolTableLength )
		return Error( Cookie, "No room in Symbol Table for new symbol" );

	strncpy( State->SymbolTable[i].Name, Name, sizeof State->SymbolTable[i].Name );

	State->SymbolTable[i].Name[sizeof State->SymbolTable[i].Name-1]	= '\0';
	State->SymbolTable[i].Type										= Type;
	State->SymbolTable[i].Value										= Value;

	State->NumSymbols++;

	return 1;
	}
/*============================================================================*/
token_t GetToken( cookie_t *Cookie, index_t *Index, char *Buffer, size_t BufferLength )
	{
	int	c;

	do
		{
		c	= GetChar( Cookie );
		switch( c )
			{
			case ' ':
				Cookie->Column++;
				break;
			case '\r':
				c	= GetChar( Cookie );
				if( c != '\n' )
					{
					UngetChar(	Cookie, c );
					c	= '\r';
					}
				Cookie->LineNumber++;
				Cookie->Column	= 1;
				return TOKEN_EOL;
			case '\n':
				c	= GetChar( Cookie );
				if( c != '\r' )
					{
					UngetChar( Cookie, c );
					c	= '\n';
					}
				Cookie->LineNumber++;
				Cookie->Column	= 1;
				return TOKEN_EOL;
			case '\t':
				Cookie->Column	= ((( Cookie->Column - 1 ) / Cookie->TabWidth ) + 1 ) * Cookie->TabWidth + 1;
				break;
			case ';':
				while(( c = GetChar( Cookie )) != '\0' && c != '\r' && c != '\n' )
					{}
				if( c == '\r' || c == '\n' )
					UngetChar( Cookie, c );
				break;
			}
		}
	while( isspace( c ));

	switch( c )
		{
		case -1:
		case '\0':
			return TOKEN_EOF;
		case ':':
			return TOKEN_COLON;
		case ',':
			return TOKEN_COMMA;
		case '(':
			return TOKEN_OPENPAR;
		case ')':
			return TOKEN_CLOSEPAR;
		case '[':
			return TOKEN_OPEN_BRACKET;
		case ']':
			return TOKEN_CLOSE_BRACKET;
		case '#':
			return TOKEN_SHARP;
		case '+':
			return TOKEN_PLUS;
		case '-':
			return TOKEN_MINUS;
		case '*':
			return TOKEN_ASTERISK;
		case '/':
			return TOKEN_SLASH;
		case '%':
			return TOKEN_PERCENT;
		case '$':
			return TOKEN_CURRENCY;
		case '&':
			if(( c = GetChar( Cookie )) == '&' )
				return TOKEN_LOGICAL_AND;
			else
				UngetChar( Cookie, c );
			return TOKEN_BINARY_AND;
		case '|':
			if(( c = GetChar( Cookie )) == '|' )
				return TOKEN_LOGICAL_OR;
			else
				UngetChar( Cookie, c );
			return TOKEN_BINARY_OR;
		case '<':
			if(( c = GetChar( Cookie )) == '<' )
				return TOKEN_SHIFT_LEFT;
			else if( c == '=' )
				return TOKEN_LESS_THAN_OR_EQUAL;
			else
				UngetChar( Cookie, c );
			return TOKEN_LESS_THAN;
		case '>':
			if(( c = GetChar( Cookie )) == '>' )
				return TOKEN_SHIFT_RIGHT;
			else if( c == '=' )
				return TOKEN_GREATER_THAN_OR_EQUAL;
			else
				UngetChar( Cookie, c );
			return TOKEN_GREATER_THAN;
		}

	if( isdigit( c ))
		{
		int			i;

		Buffer[0]	= c;
		i			= c;
		c	= GetChar( Cookie );

		if( i == '0' && ( c == 'x' || c == 'X' ))
			{
			Buffer[0]	= 'h';
			c	= GetChar( Cookie );
			for( i = 1; isxdigit( c ); i++, c = GetChar( Cookie ))
				if( Buffer != NULL && i < BufferLength )
					Buffer[i]	= c;
			Buffer[i]	= '\0';
			UngetChar( Cookie, c );

			return TOKEN_NUMBERHEX;
			}

		for( i = 1; isdigit( c ); i++, c = GetChar( Cookie ))
			if( Buffer != NULL && i < BufferLength )
				Buffer[i]	= c;
		Buffer[i]	= '\0';
		UngetChar( Cookie, c );

		return TOKEN_NUMBER;
		}
	else if( isalpha( c ) || c == '_' )
		{
		char	Temp[1024];
		int		i, Length;

		for( Length = 0; isalnum( c ) || c == '_'; c = GetChar( Cookie ))
			if( Length < BufferLength - 1 )
				Temp[Length++]	= c;
		Temp[Length]	= '\0';
		UngetChar( Cookie, c );

		if( Buffer != NULL )
			{
			strncpy( Buffer, Temp, BufferLength - 1 );
			Buffer[BufferLength - 1]	= '\0';
			}

		if( Buffer[0] == 'h' || Buffer[0] == 'H' )
			{
			for( i = 1; isxdigit( Buffer[i] ); i++ )
				{}
			if( Buffer[i] == '\0' )
				return TOKEN_NUMBERHEX;
			}

		for( i = 0; i < LENGTH( ReservedWords ); i++ )
			if( stricmp( Temp, ReservedWords[i].Name ) == 0 )
				{
				if( Index != NULL )
					*Index	= ReservedWords[i].Index;
				return ReservedWords[i].Token;
				}

		return TOKEN_IDENTIFIER;
		}
	else if( c == '\'' )
		{
		int	i;

		for( i = 0; i < BufferLength - 3 && isprint( c = GetChar( Cookie )) && c != '\''; i++ )
			{
			if( c == '\\' )
				{
				c = GetChar( Cookie );
				switch( c )
					{
					default:
						Error( Cookie, "Invalid escape code" );
						return TOKEN_INVALID;
					case '\\': case '\'': case '\"':
						break;
					case 'a':
						c	= 0x07;
						break;
					case 'b':
						c	= 0x08;
						break;
					case 't':
						c	= 0x09;
						break;
					case 'n':
						c	= 0x0a;
						break;
					case 'v':
						c	= 0x0b;
						break;
					case 'f':
						c	= 0x0c;
						break;
					case 'r':
						c	= 0x0d;
						break;
					}
				}
			Buffer[i]	= c;
			}
		if( c != '\'' )
			{
			Error( Cookie, "Invalid string" );
			return TOKEN_INVALID;
			}
		Buffer[i]	= '\0';
		if( i == 1 )
			return TOKEN_CHARACTER;
		return TOKEN_STRING;
		}
	else
		{
		Error( Cookie, "Invalid syntax" );
		return TOKEN_INVALID;
		}
	return TOKEN_INVALID;
	}
/*============================================================================*/
static void UpdateHighWaterLine( state_t *State )
	{
	if( State->CurrentLocation >  State->MaximumAddress )
		State->MaximumAddress	= State->CurrentLocation;
	}
/*============================================================================*/
static type_t ParseExpression( cookie_t *Cookie, state_t *State, result_t *Result );
/*============================================================================*/
static type_t ParseValueExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t	Index;
	token_t	Token;
	type_t	Type1;

	Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
	switch( Token )
		{
		case TOKEN_IDENTIFIER:
			return FindSymbol( Cookie, State, Cookie->Buffer, &Result->Number, &Result->Type );

		case TOKEN_CURRENCY:
			Result->Number	= State->CurrentLocation;
			Result->Type	= TYPE_ADDRESS;
			return Result->Type;

		case TOKEN_PLUS:
			/* A unary plus sign does nothing, just return the expression that folows it. */
			Type1 = ParseValueExpression( Cookie, State, Result );
			if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS )
				{
				Error( Cookie, "Invalid expression" );
				return TYPE_INVALID;
				}
			Result->Type	= Type1;
			return Result->Type;

		case TOKEN_MINUS:
			/* A unary minus sign inverts the sign of the expression. */
			Type1	= ParseValueExpression( Cookie, State, Result );
			if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS )
				{
				Error( Cookie, "Invalid expression" );
				return TYPE_INVALID;
				}
			Result->Number	= -Result->Number;
			Result->Type	= TYPE_NUMBER;
			return Result->Type;

		case TOKEN_CHARACTER:
			Result->Number	= Cookie->Buffer[0];
			Result->Type	= TYPE_NUMBER;
			return Result->Type;

		case TOKEN_NUMBER:
			Result->Number	= atoi( Cookie->Buffer );
			Result->Type	= TYPE_NUMBER;
			return Result->Type;

		case TOKEN_NUMBERHEX:
			sscanf( &Cookie->Buffer[1], "%x", &Result->Number );
			Result->Type	= TYPE_NUMBER;
			return Result->Type;

		case TOKEN_OPENPAR:
			Type1	= ParseExpression( Cookie, State, Result );
			Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_CLOSEPAR )
				{
				Error( Cookie, "Missing \")\"" );
		default:
				Result->Type	= TYPE_INVALID;
				return Result->Type;
				}
			Result->Type	= Type1;
			return Result->Type;
		}

	return Type1;
	}
/*============================================================================*/
static type_t ParseMultiplicativeExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseValueExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_ASTERISK || Token == TOKEN_SLASH || Token == TOKEN_PERCENT )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2	= ParseValueExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			switch( Token )
				{
				default:
					Error( Cookie, "Internal error" );
					return TYPE_INVALID;

				case TOKEN_ASTERISK:
					Result->Number	*= Operand2.Number;
					break;

				case TOKEN_SLASH:
					if( Operand2.Number == 0 )
						{
						Error( Cookie, "Divide by zero" );
						return TYPE_INVALID;
						}
					Result->Number	/= Operand2.Number;
					break;

				case TOKEN_PERCENT:
					if( Operand2.Number == 0 )
						{
						Error( Cookie, "Divide by zero" );
						return TYPE_INVALID;
						}
					Result->Number	%= Operand2.Number;
					break;
				}
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseAdditiveExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseMultiplicativeExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_PLUS || Token == TOKEN_MINUS )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2	= ParseMultiplicativeExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			switch( Token )
				{
				default:
					Error( Cookie, "Internal error" );
					return TYPE_INVALID;

				case TOKEN_PLUS:
					Result->Number += Operand2.Number;
					break;
				case TOKEN_MINUS:
					Result->Number -= Operand2.Number;
					break;
				}
			Result->Type	= Type1 == Type2 ? TYPE_NUMBER : TYPE_ADDRESS;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseShiftExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseAdditiveExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_SHIFT_LEFT || Token == TOKEN_SHIFT_RIGHT )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2	= ParseAdditiveExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			if( Operand2.Number > 16 )
				Warning( Cookie, "Shift more than the data width" );

			switch( Token )
				{
				default:
					Error( Cookie, "Internal error" );
					return TYPE_INVALID;

				case TOKEN_SHIFT_LEFT:
					Result->Number	<<= Operand2.Number;
					break;

				case TOKEN_SHIFT_RIGHT:
					Result->Number	>>= Operand2.Number;
					break;
				}
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseBinaryAndExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseShiftExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_BINARY_AND )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2			= ParseShiftExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			Result->Number &= Operand2.Number;
			Result->Type	= Type1 == Type2 ? TYPE_NUMBER : TYPE_ADDRESS;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseBinaryXOrExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseBinaryAndExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_BINARY_XOR )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2			= ParseBinaryAndExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			Result->Number ^= Operand2.Number;
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseBinaryOrExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseBinaryXOrExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_BINARY_OR )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2			= ParseBinaryXOrExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			Result->Number |= Operand2.Number;
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseInequalityExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseBinaryOrExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_LESS_THAN || Token == TOKEN_LESS_THAN_OR_EQUAL || Token == TOKEN_GREATER_THAN_OR_EQUAL || Token == TOKEN_GREATER_THAN )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2	= ParseBinaryOrExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			switch( Token )
				{
				default:
					Error( Cookie, "Internal error" );
					return TYPE_INVALID;

				case TOKEN_LESS_THAN:
					Result->Number	= Result->Number < Operand2.Number;
					break;

				case TOKEN_LESS_THAN_OR_EQUAL:
					Result->Number	= Result->Number <= Operand2.Number;
					break;

				case TOKEN_GREATER_THAN_OR_EQUAL:
					Result->Number	= Result->Number >= Operand2.Number;
					break;

				case TOKEN_GREATER_THAN:
					Result->Number	= Result->Number > Operand2.Number;
					break;
				}
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseEqualityExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseInequalityExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_EQUAL_TO || Token == TOKEN_NOT_EQUAL_TO )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2	= ParseInequalityExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			switch( Token )
				{
				default:
					Error( Cookie, "Internal error" );
					return TYPE_INVALID;

				case TOKEN_EQUAL_TO:
					Result->Number	= Result->Number == Operand2.Number;
					break;

				case TOKEN_NOT_EQUAL_TO:
					Result->Number	= Result->Number != Operand2.Number;
					break;
				}
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseLogicalAndExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseEqualityExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_LOGICAL_AND )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2			= ParseEqualityExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			Result->Number	= Result->Number && Operand2.Number;
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseExpression( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	index_t		Index;
	token_t		Token;
	type_t		Type1, Type2;
	result_t	Operand2;
	savepos_t	SavePos;

	Type1	= ParseLogicalAndExpression( Cookie, State, Result );
	SavePosition( Cookie, &SavePos );
	while(( Token = GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer )) == TOKEN_LOGICAL_OR )
		{
		if( Type1 != TYPE_NUMBER && Type1 != TYPE_ADDRESS && Type1 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		Type2			= ParseLogicalAndExpression( Cookie, State, &Operand2 );
		if( Type2 != TYPE_NUMBER && Type2 != TYPE_ADDRESS && Type2 != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid argument for operator" );
			return TYPE_INVALID;
			}

		if( Type1 == TYPE_UNDEFINED || Type2 == TYPE_UNDEFINED )
			{
			Type1			= TYPE_UNDEFINED;
			Result->Number	= 0;
			Result->Type	= TYPE_UNDEFINED;
			}
		else
			{
			Result->Number	= Result->Number || Operand2.Number;
			Result->Type	= TYPE_NUMBER;
			}
		SavePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	return Result->Type;
	}
/*============================================================================*/
static type_t ParseOperand( cookie_t *Cookie, state_t *State, result_t *Result )
	{
	char		Buffer1[1024];
	index_t		Index1;
	token_t		Token1;
	type_t		Type;
	savepos_t	SavePos;

	SavePosition( Cookie, &SavePos );
	Token1 = GetToken( Cookie, &Index1, Buffer1, sizeof Buffer1 );
	/* We have a "rx" operand. */
	if( Token1 == TOKEN_REGISTER )
		{
		Result->Register	= Index1;
		Result->Type		= TYPE_REGISTER;
		return Result->Type;
		}
	/* We have a "#<expression>" operand. */
	else if( Token1 == TOKEN_SHARP )
		{
		Type	= ParseExpression( Cookie, State, Result );
		if( Type != TYPE_ADDRESS && Type != TYPE_NUMBER && Type != TYPE_UNDEFINED )
			{
			Error( Cookie, "Invalid immediate value" );
			Result->Type	= TYPE_INVALID;
			return Result->Type;
			}
		Result->Type		= TYPE_IMMEDIATE;
		return Result->Type;
		}
	/* We *MAY* have a "(rx)" or "(rx)+ operand. */
	else if( Token1 == TOKEN_OPENPAR )
		{
		char	Buffer2[1024];
		index_t	Register2;
		token_t	Token2;
		SavePosition( Cookie, &SavePos );
		Token2 = GetToken( Cookie, &Register2, Buffer2, sizeof Buffer2 );
		if( Token2 == TOKEN_REGISTER )
			{
			index_t	Index3;
			token_t	Token3;

			Result->Register	= Register2;

			Token3 = GetToken( Cookie, &Index3, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token3 != TOKEN_CLOSEPAR )
				{
				Error( Cookie, "Missing \")\"" );
				Result->Type	= TYPE_INVALID;
				return Result->Type;
				}
			SavePosition( Cookie, &SavePos );
			Token3 = GetToken( Cookie, &Index3, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token3 == TOKEN_PLUS )
				{
				Result->Type	= TYPE_REGISTER_POSTINC;
				return Result->Type;
				}
			RestorePosition( Cookie, &SavePos );
			Result->Type	= TYPE_REGISTER_INDIRECT;
			return Result->Type;
			}
		RestorePosition( Cookie, &SavePos );
		}
	/* We *MAY* have a "-(rx)" operand. */
	else if( Token1 == TOKEN_MINUS )
		{
		char	Buffer4[1024];
		index_t	Index4;
		token_t	Token4;

		SavePosition( Cookie, &SavePos );
		Token4 = GetToken( Cookie, &Index4, Buffer4, sizeof Buffer4 );
		if( Token4 == TOKEN_OPENPAR )
			{
			char	Buffer5[1024];
			index_t	Register5;
			token_t	Token5;

			SavePosition( Cookie, &SavePos );
			Token5 = GetToken( Cookie, &Register5, Buffer5, sizeof Buffer5 );
			if( Token5 == TOKEN_REGISTER )
				{
				index_t	Index6;
				token_t	Token6;

				Result->Register	= Register5;

				Token6 = GetToken( Cookie, &Index6, Cookie->Buffer, sizeof Cookie->Buffer );
				if( Token6 != TOKEN_CLOSEPAR )
					{
					Error( Cookie, "Missing \")\"" );
					Result->Type	= TYPE_INVALID;
					return Result->Type;
					}
				Result->Type		= TYPE_REGISTER_PREDEC;
				return Result->Type;
				}
			RestorePosition( Cookie, &SavePos );
			}
		RestorePosition( Cookie, &SavePos );
		}

	RestorePosition( Cookie, &SavePos );

	Type	= ParseExpression( Cookie, State, Result );
	if( Type != TYPE_ADDRESS && Type != TYPE_NUMBER && Type != TYPE_UNDEFINED )
		{
		Error( Cookie, "Invalid argument" );
		Result->Type	= TYPE_INVALID;
		return Result->Type;
		}

	if( Type == TYPE_UNDEFINED )
		Type	= TYPE_NUMBER;

	index_t	Register7;
	token_t	Token7;

	SavePosition( Cookie, &SavePos );
	Token7 = GetToken( Cookie, &Register7, Cookie->Buffer, sizeof Cookie->Buffer );
	if( Token7 == TOKEN_OPENPAR )
		{
		Token7 = GetToken( Cookie, &Register7, Cookie->Buffer, sizeof Cookie->Buffer );
		if( Token7 != TOKEN_REGISTER )
			{
			Error( Cookie, "Expecting register name" );
			Result->Type	= TYPE_INVALID;
			return Result->Type;
			}
		Token7 = GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
		if( Token7 != TOKEN_CLOSEPAR )
			{
			Error( Cookie, "Missing \")\"" );
			Result->Type	= TYPE_INVALID;
			return Result->Type;
			}
		Result->Offset		= Result->Number;
		Result->Register	= Register7;
		Result->Type		= TYPE_REGISTER_INDEXED;
		return Result->Type;
		}

	RestorePosition( Cookie, &SavePos );

	Result->Type	= Type;
	return Result->Type;
	}
/*============================================================================*/
static void PrintWords( const uint8_t * const Memory, uint16_t StartAddress, uint8_t Length )
	{
	int	i;

	for( i = 0; Length > 0; )
		{
		printf( "\n%04x:\t", StartAddress );
		while( Length > 0 )
			{
			printf( "%04x ", ( Memory[StartAddress+0] << 8 ) | ( Memory[StartAddress+1] << 0 ));
			StartAddress += 2;
			Length--;
			i++;
			if( i % 8 == 0 )
				break;
			}
		}
	}
/*============================================================================*/
static void PrintOpCodes( const uint8_t * const Memory, uint16_t OpCodeStart, uint8_t Length )
	{
	int	i;

	for( i = 0; Length > 0; )
		{
		printf( "\n%04x:\t", OpCodeStart );
		while( Length > 0 )
			{
			printf( "%02x ", Memory[OpCodeStart] );
			OpCodeStart++;
			Length--;
			if( ++i % 16 == 0 )
				break;
			}
		}
	}
/*============================================================================*/
static int ProcessDirective( cookie_t *Cookie, state_t *State, const char * const Label, index_t Directive )
	{
	token_t		Token;
	result_t	Result;
	type_t		Type;
	uint16_t	StartAddress;
	savepos_t	SavePos;

	switch( Directive )
		{
		case DIRECTIVE_EQU:
			{
			token_t	Token;
			index_t	Index;

			if( Label[0] == '\0' )
				return Error( Cookie, "EQU directive without a label" );

			SavePosition( Cookie, &SavePos );
			Token	= GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_SHARP )
				RestorePosition( Cookie, &SavePos );

			Type	= ParseExpression( Cookie, State, &Result );
			if( Type != TYPE_UNDEFINED && InsertSymbol( Cookie, State, Label, Result.Number, Type ) < 0 )
				return -1;

			break;
			}

		case DIRECTIVE_ORG:
			{
			uint32_t	Address;

			Type	= ParseExpression( Cookie, State, &Result );
			if( Type == TYPE_UNDEFINED )
				return Error( Cookie, "Undefined Symbol" );

			Address	= Result.Number & 0xffff;

			if( Label[0] != '\0' && InsertSymbol( Cookie, State, Label, Address, TYPE_ADDRESS ) < 0 )
				return -1;

			State->CurrentLocation	= Address;
			UpdateHighWaterLine( State );
			break;
			}
		case DIRECTIVE_DW:
		case DIRECTIVE_DAW:
			{
			int		Length	= 0;
			index_t	Index;

			if( Label[0] != '\0' && InsertSymbol( Cookie, State, Label, State->CurrentLocation, TYPE_ADDRESS ) < 0 )
				return -1;

			StartAddress	= State->CurrentLocation;

			SavePosition( Cookie, &SavePos );
			Token	= GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token == TOKEN_EOL || Token == TOKEN_EOF )
				{
				State->Memory[State->CurrentLocation++]	= 0;
				State->Memory[State->CurrentLocation++]	= 0;
				Length++;
				}
			else
				{
				RestorePosition( Cookie, &SavePos );
				do
					{
					index_t	Index2;
					SavePosition( Cookie, &SavePos );
					Token	= GetToken( Cookie, &Index2, Cookie->Buffer, sizeof Cookie->Buffer );
					if( Token == TOKEN_OPEN_BRACKET )
						{
						index_t	Index3;
						Type	= ParseExpression( Cookie, State, &Result );
						if( Type != TYPE_NUMBER )
							return Error( Cookie, "Expression not valid in this context" );
						Token	= GetToken( Cookie, &Index3, Cookie->Buffer, sizeof Cookie->Buffer );
						if( Token != TOKEN_CLOSE_BRACKET )
							return Error( Cookie, "Missing \']\'" );
						//PrintWords( State->Memory, StartAddress, Length );
						StartAddress   += 2 * Length;
						Length			= 0;
						//printf( "\n%04x:\t0000 <repeats %u times>", StartAddress, Result.Number );
						memset( &State->Memory[State->CurrentLocation], 0x00, 2 * Result.Number );
						State->CurrentLocation	+= 2 * Result.Number;
						StartAddress	= State->CurrentLocation;
						}
					else if( Token == TOKEN_STRING )
						{
						int	i;

						for( i = 0; Cookie->Buffer[i] != '\0'; )
							{
							//printf( "\n%04x:\t", StartAddress + 2 * i );
							while( Cookie->Buffer[i] != '\0' )
								{
								//printf( "%04x ", Cookie->Buffer[i] );
								if( ++i % 8 == 0 )
									break;
								}
							}

						for( i = 0; Cookie->Buffer[i] != '\0' ; i++ )
							{
							State->Memory[State->CurrentLocation++]	= 0;
							State->Memory[State->CurrentLocation++]	= Cookie->Buffer[i];
							}
						StartAddress	= State->CurrentLocation;
						}
					else
						{
						RestorePosition( Cookie, &SavePos );
						Type	= ParseExpression( Cookie, State, &Result );
						if( Type != TYPE_NUMBER && Type != TYPE_ADDRESS )
							return Error( Cookie, "Expression not valid in this context" );
						if( Result.Number > 0xffff )
							Warning( Cookie, "Value larger than 16-bits will be truncated" );
						State->Memory[State->CurrentLocation++]	= (uint8_t)(Result.Number >> 8 );
						State->Memory[State->CurrentLocation++]	= (uint8_t)(Result.Number >> 0 );

						Length++;
						}
					SavePosition( Cookie, &SavePos );
					Token	= GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
					}
				while( Token == TOKEN_COMMA );
				}

			UpdateHighWaterLine( State );
			RestorePosition( Cookie, &SavePos );

			//PrintWords( State->Memory, StartAddress, Length );

			break;
			}
		case DIRECTIVE_DB:
		case DIRECTIVE_DAB:
			{
			int			Length	= 0;
			index_t		Index;
			savepos_t	SavePos;

			if( Label[0] != '\0' && InsertSymbol( Cookie, State, Label, State->CurrentLocation, TYPE_ADDRESS ) < 0 )
				return -1;

			StartAddress	= State->CurrentLocation;

			SavePosition( Cookie, &SavePos );
			Token	= GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token == TOKEN_EOL || Token == TOKEN_EOF )
				{
				State->Memory[State->CurrentLocation++]	= 0;
				Length++;
				}
			else
				{
				RestorePosition( Cookie, &SavePos );
				do
					{
					index_t	Index2;
					SavePosition( Cookie, &SavePos );
					Token	= GetToken( Cookie, &Index2, Cookie->Buffer, sizeof Cookie->Buffer );
					if( Token == TOKEN_OPEN_BRACKET )
						{
						index_t	Index3;

						Type	= ParseExpression( Cookie, State, &Result );
						if( Type != TYPE_NUMBER )
							return Error( Cookie, "Expression not valid in this context" );
						Token	= GetToken( Cookie, &Index3, Cookie->Buffer, sizeof Cookie->Buffer );
						if( Token != TOKEN_CLOSE_BRACKET )
							return Error( Cookie, "Missing \']\'" );
						//PrintOpCodes( State->Memory, StartAddress, Length );
						StartAddress   += Length;
						Length			= 0;
						//printf( "\n%04x:\t00 <repeats %u times>", StartAddress, Result.Number );
						memset( &State->Memory[State->CurrentLocation], 0x00, Result.Number );
						State->CurrentLocation	+= Result.Number;
						StartAddress	= State->CurrentLocation;
						}
					else if( Token == TOKEN_STRING )
						{
						int	i;

						for( i = 0; Cookie->Buffer[i] != '\0'; )
							{
							//printf( "\n%04x:\t", StartAddress + i );
							while( Cookie->Buffer[i] != '\0' )
								{
								//printf( "%02x ", Cookie->Buffer[i] );
								if( ++i % 16 == 0 )
									break;
								}
							}
						for( i = 0; Cookie->Buffer[i] != '\0' ; i++ )
							State->Memory[State->CurrentLocation++]	= Cookie->Buffer[i];
						StartAddress	= State->CurrentLocation;
						}
					else
						{
						RestorePosition( Cookie, &SavePos );
						Type	= ParseExpression( Cookie, State, &Result );
						if( Type != TYPE_NUMBER && Type != TYPE_ADDRESS )
							return Error( Cookie, "Expression not valid in this context" );
						if( Result.Number > 0xff )
							Warning( Cookie, "Value larger than 8-bits will be truncated" );
						State->Memory[State->CurrentLocation++]	= (uint8_t)Result.Number;

						Length++;
						}
					SavePosition( Cookie, &SavePos );
					Token	= GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
					}
				while( Token == TOKEN_COMMA );
				}

			UpdateHighWaterLine( State );
			RestorePosition( Cookie, &SavePos );

			//PrintOpCodes( State->Memory, StartAddress, Length );

			break;
			}
		default:
			return Error( Cookie, "Unknown directive" );
		}
	Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
	if( Token == TOKEN_EOL )
		return 1;
	if( Token == TOKEN_EOF )
		return 0;

	return Error( Cookie, "Extra input at end of line" );
	}
/*============================================================================*/
typedef union
	{
	uint16_t		Value;
	struct
		{
		uint16_t    RegDst	: 3;
		uint16_t    ModeDst	: 3;
		uint16_t    RegSrc	: 3;
		uint16_t    ModeSrc	: 3;
		uint16_t	OpCode4	: 4;
		};
	struct
		{
		uint16_t    		: 8;
		uint16_t	OpCode8	: 8;
		};
	} opcode2op_t;
/*============================================================================*/
typedef enum
	{
	MODE_REGISTER,
	MODE_POSTINCREMENT,
	MODE_PREDECREMENT,
	MODE_INDEXED,
	MODE_REGISTER_INDIRECT,
	MODE_POSTINCREMENT_INDIRECT,
	MODE_PREDECREMENT_INDIRECT,
	MODE_INDEXED_INDIRECT
	} addrmodes_t;
/*============================================================================*/
typedef struct
	{
	addrmodes_t	Mode;
	int			Register;
	int			HasValue;
	int			Value;
	} operand_t;
/*============================================================================*/
typedef enum
	{
	OPERAND_DESTINATION,
	OPERAND_SOURCE
	} srcdst_t;
/*============================================================================*/
static int EvaluateOperand( cookie_t *Cookie, operand_t *Result, uint8_t OpCode, srcdst_t SrcDst, type_t Type, int Register, int Value )
	{
	switch( Type )
		{
		default:
			return Error( Cookie, "Invalid Operand mode" );

		/* Instructions of the type "op #val,..." */
		case TYPE_IMMEDIATE:
			/* It doesn't make sense to have an immediate value as destination... */
			if( SrcDst == OPERAND_DESTINATION && OpCode != OPCODE_CMP )
				return Error( Cookie, "Invalid Operand mode" );
			Result->Mode		= MODE_POSTINCREMENT;
			Result->Register	= 7;
			Result->HasValue	= 1;
			Result->Value		= Value;
			break;
		/* Instructions of the type "op var,..." or "op ...,var */
		case TYPE_DIRECT:
		case TYPE_ADDRESS:
		case TYPE_NUMBER:
			Result->Mode		= MODE_POSTINCREMENT_INDIRECT;
			Result->Register	= 7;
			Result->HasValue	= 1;
			Result->Value		= Value;
			break;
		/* Instructions of the type "op rx,..." or "op ...,rx" */
		case TYPE_REGISTER:
			Result->Mode		= MODE_REGISTER;
			Result->Register	= Register;
			Result->HasValue	= 0;
			break;
		case TYPE_REGISTER_POSTINC:
			Result->Mode		= MODE_POSTINCREMENT;
			Result->Register	= Register;
			Result->HasValue	= 0;
			break;
		case TYPE_REGISTER_PREDEC:
			Result->Mode		= MODE_PREDECREMENT;
			Result->Register	= Register;
			Result->HasValue	= 0;
			break;
		case TYPE_REGISTER_INDEXED:
			Result->Mode		= MODE_INDEXED;
			Result->Register	= Register;
			Result->HasValue	= 1;
			Result->Value		= Value;
			break;
		case TYPE_REGISTER_INDIRECT:
			Result->Mode		= MODE_REGISTER_INDIRECT;
			Result->Register	= Register;
			Result->HasValue	= 0;
			break;
		case TYPE_REGISTER_POSTINC_INDIRECT:
			Result->Mode		= MODE_POSTINCREMENT_INDIRECT;
			Result->Register	= Register;
			Result->HasValue	= 0;
			break;
		case TYPE_REGISTER_PREDEC_INDIRECT:
			Result->Mode		= MODE_PREDECREMENT_INDIRECT;
			Result->Register	= Register;
			Result->HasValue	= 0;
			break;
		case TYPE_REGISTER_INDEXED_INDIRECT:
			Result->Mode		= MODE_INDEXED_INDIRECT;
			Result->Register	= Register;
			Result->HasValue	= 0;
			Result->Value		= Value;
			break;
		}
	return 1;
	}
/*============================================================================*/
static uint16_t GenerateOpCode( cookie_t *Cookie, state_t *State, uint8_t OpCode, const operand_t * const Op1, const operand_t * const Op2 )
	{
	opcode2op_t	Insn;
	//uint16_t	OpCodeStart;
	unsigned	Length	= 0;

	//OpCodeStart		= State->CurrentLocation;

	Insn.Value		= 0;

	if( Op1 != NULL )
		{
		Insn.OpCode4	= OpCode >> 4;
		Insn.ModeSrc	= Op1->Mode;
		Insn.RegSrc		= Op1->Register;
		}
	else
		Insn.OpCode8	= OpCode;

	Insn.ModeDst		= Op2->Mode;
	Insn.RegDst			= Op2->Register;

	State->Memory[State->CurrentLocation++]	= (uint8_t)( Insn.Value >> 8 );
	State->Memory[State->CurrentLocation++]	= (uint8_t)( Insn.Value >> 0 );
	Length								   += 2;

	if( Op1 != NULL && Op1->HasValue )
		{
		State->Memory[State->CurrentLocation++]	= (uint8_t)( Op1->Value >> 8 );
		State->Memory[State->CurrentLocation++]	= (uint8_t)( Op1->Value >> 0 );
		Length								   += 2;
		}

	if( Op2->HasValue )
		{
		State->Memory[State->CurrentLocation++]	= (uint8_t)( Op2->Value >> 8 );
		State->Memory[State->CurrentLocation++]	= (uint8_t)( Op2->Value >> 0 );
		Length								   += 2;
		}

	UpdateHighWaterLine( State );

	//PrintOpCodes( State->Memory, OpCodeStart, Length );

	return Insn.Value;
	}
/*============================================================================*/
static int Process1OperandIntructions( cookie_t *Cookie, state_t *State, index_t Instruction )
	{
	static const uint8_t	OpCodes[]	=
		{
		/* Do not change the order of the values below. */
		OPCODE_CLR, OPCODE_NOT, OPCODE_INC, OPCODE_DEC, OPCODE_NEG, OPCODE_TST,
		OPCODE_ROR, OPCODE_ROL, OPCODE_ASR, OPCODE_ASL, OPCODE_ADC, OPCODE_SBC
		};
	result_t	OperandIn;
	operand_t	OperandOut;
	uint8_t		OpCode;

	OpCode	= OpCodes[Instruction-INSTRUCTION_CLR];

	ParseOperand( Cookie, State, &OperandIn );
	if( EvaluateOperand( Cookie, &OperandOut, OpCode, OPERAND_DESTINATION, OperandIn.Type, OperandIn.Register, OperandIn.Offset ) < 0 )
		return -1;

	GenerateOpCode( Cookie, State, OpCode, NULL, &OperandOut );

	return 1;
	}
/*============================================================================*/
static int Process2OperandsIntructions( cookie_t *Cookie, state_t *State, index_t Instruction )
	{
	static const uint8_t	OpCodes[]	=
		{
		/* Do not change the order of the values below. */
		OPCODE_MOV, OPCODE_ADD, OPCODE_SUB, OPCODE_CMP, OPCODE_AND, OPCODE_OR
		};
	result_t	Operand1In, Operand2In;
	operand_t	Operand1Out, Operand2Out;
	token_t		Token;
	uint8_t		OpCode;

	OpCode	= OpCodes[Instruction-INSTRUCTION_MOV];

	ParseOperand( Cookie, State, &Operand1In );
	if( EvaluateOperand( Cookie, &Operand1Out, OPERAND_SOURCE, OpCode, Operand1In.Type, Operand1In.Register, Operand1In.Offset ) < 0 )
		return -1;

	Token	= GetToken( Cookie, NULL, NULL, 0 );
	if( Token != TOKEN_COMMA )
		return Error( Cookie, "Missing \",\"" );

	ParseOperand( Cookie, State, &Operand2In );
	if( EvaluateOperand( Cookie, &Operand2Out, OPERAND_DESTINATION, OpCode, Operand2In.Type, Operand2In.Register, Operand2In.Offset ) < 0 )
		return -1;

	GenerateOpCode( Cookie, State, OpCode, &Operand1Out, &Operand2Out );

	return 1;
	}
/*============================================================================*/
static int ProcessInstruction( cookie_t *Cookie, state_t *State, const char * const Label, index_t Instruction )
	{
	token_t			Token;
	//uint16_t		OpCodeStart;

	if( Label[0] != '\0' && InsertSymbol( Cookie, State, Label, State->CurrentLocation, TYPE_ADDRESS ) < 0 )
		return -1;

	switch( Instruction )
		{
	default:
			return Error( Cookie, "Unknown instruction" );
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_NOP:
		case INSTRUCTION_HLT:
		case INSTRUCTION_RTI:
			{
			static const uint8_t	OpCodes[]	=
				{
				/* Do not change the order of the values below. */
				OPCODE_NOP, OPCODE_HLT, OPCODE_RTI
				};

			//OpCodeStart	= State->CurrentLocation;

			State->Memory[State->CurrentLocation++]	= OpCodes[Instruction-INSTRUCTION_NOP];
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 1 );
			break;
			}
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_JMP:
			{
			result_t	Address;
			type_t		Type;

			//OpCodeStart	= State->CurrentLocation;

			Type	= ParseExpression( Cookie, State, &Address );
			if( Type != TYPE_NUMBER && Type != TYPE_ADDRESS  && Type != TYPE_UNDEFINED )
				return Error( Cookie, "Invalid operand I" );

			if( Address.Offset > 0xffff )
				Warning( Cookie, "Address larger than 16-bits will be truncated" );
			Address.Offset	&= 0xffff;

			State->Memory[State->CurrentLocation++]	= OPCODE_JMP;
			State->Memory[State->CurrentLocation++]	= 0x2f;
			State->Memory[State->CurrentLocation++]	= (uint8_t)( Address.Offset >> 8 );
			State->Memory[State->CurrentLocation++]	= (uint8_t)( Address.Offset >> 0 );
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 4 );
			break;
			}
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_BR:
		case INSTRUCTION_BNE:
		case INSTRUCTION_BEQ:
		case INSTRUCTION_BPL:
		case INSTRUCTION_BMI:
		case INSTRUCTION_BVC:
		case INSTRUCTION_BVS:
		case INSTRUCTION_BCC:
		case INSTRUCTION_BCS:
		case INSTRUCTION_BGE:
		case INSTRUCTION_BLT:
		case INSTRUCTION_BGT:
		case INSTRUCTION_BLE:
		case INSTRUCTION_BHI:
		case INSTRUCTION_BLS:
			{
			static const uint8_t	OpCodes[]	=
				{
				/* Do not change the order of the values below. */
				OPCODE_BR,
				OPCODE_BNE, OPCODE_BEQ, OPCODE_BPL, OPCODE_BMI, OPCODE_BVC, OPCODE_BVS, OPCODE_BCC,
				OPCODE_BCS, OPCODE_BGE, OPCODE_BLT, OPCODE_BGT, OPCODE_BLE, OPCODE_BHI, OPCODE_BLS
				};
			int						Offset;
			result_t				Address;
			type_t					Type;

			//OpCodeStart	= State->CurrentLocation;

			Type		= ParseExpression( Cookie, State, &Address );
			if( Type == TYPE_NUMBER || Type ==TYPE_UNDEFINED )
				Offset	= Address.Offset;
			else if( Type == TYPE_ADDRESS )
				Offset	= Address.Offset - State->CurrentLocation - 2;
			else
				return Error( Cookie, "Invalid operand II" );

			if( Offset < -128 || Offset > 127 )
				return Error( Cookie, "Relative jump out of range" );

			State->Memory[State->CurrentLocation++]	= OpCodes[Instruction-INSTRUCTION_BR];
			State->Memory[State->CurrentLocation++]	= (uint8_t)Offset;
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 2 );
			break;
			}
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_JSR:
			{
			index_t		Register;
			result_t	Address;
			type_t		Type;

			//OpCodeStart	= State->CurrentLocation;

			Token	= GetToken( Cookie, &Register, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_REGISTER )
				return Error( Cookie, "Expecting register name" );
			Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_COMMA )
				return Error( Cookie, "Expecting \',\'" );

			Type	= ParseExpression( Cookie, State, &Address );
			if( Type != TYPE_NUMBER && Type != TYPE_ADDRESS && Type != TYPE_UNDEFINED )
				return Error( Cookie, "Invalid operand III" );

			if( Address.Offset > 0xffff )
				Warning( Cookie, "Address larger than 16-bits will be truncated" );
			Address.Offset	&= 0xffff;

			State->Memory[State->CurrentLocation++]	= OPCODE_JSR | Register;
			State->Memory[State->CurrentLocation++]	= 0x2f;
			State->Memory[State->CurrentLocation++]	= (uint8_t)( Address.Offset >> 8 );
			State->Memory[State->CurrentLocation++]	= (uint8_t)( Address.Offset >> 0 );
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 4 );
			break;
			}
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_RTS:
			{
			index_t	Register;

			//OpCodeStart	= State->CurrentLocation;

			Token	= GetToken( Cookie, &Register, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_REGISTER )
				return Error( Cookie, "Expecting register name" );

			State->Memory[State->CurrentLocation++]	= OPCODE_RTS | Register;
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 1 );
			break;
			}
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_CLR:
		case INSTRUCTION_NOT:
		case INSTRUCTION_INC:
		case INSTRUCTION_DEC:
		case INSTRUCTION_NEG:
		case INSTRUCTION_TST:
		case INSTRUCTION_ROR:
		case INSTRUCTION_ROL:
		case INSTRUCTION_ASR:
		case INSTRUCTION_ASL:
		case INSTRUCTION_ADC:
		case INSTRUCTION_SBC:
			if( Process1OperandIntructions( Cookie, State, Instruction ) < 0 )
				return -1;
			break;
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_MOV:
		case INSTRUCTION_ADD:
		case INSTRUCTION_SUB:
		case INSTRUCTION_CMP:
		case INSTRUCTION_AND:
		case INSTRUCTION_OR:
			if( Process2OperandsIntructions( Cookie, State, Instruction ) < 0 )
				return -1;
			break;
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_CCC:
		case INSTRUCTION_SCC:
			{
			static const char * const	nzcv		= "NZCV";
			static const uint8_t		OpCodes[]	=
				{
				/* Do not change the order of the values below. */
				OPCODE_CCC, OPCODE_SCC
				};
			char						*p;
			int							i;
			uint8_t						Mask;

			//OpCodeStart	= State->CurrentLocation;

			Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_IDENTIFIER || strlen( Cookie->Buffer ) > 4 )
				return Error( Cookie, "Expecting any combination of at most one of each \'n\', \'z\', \'c\', \'v\'" );
			for( i = 0, Mask = 0; i < strlen( Cookie->Buffer ); i++, Mask |= 1 << ( p - nzcv ))
				if(( p = strchr( nzcv, toupper( Cookie->Buffer[i] ))) == NULL || ( Mask & 1 << ( p - nzcv )) != 0 )
					return Error( Cookie, "Expecting any combination of at most one of each \'n\', \'z\', \'c\', \'v\'" );

			State->Memory[State->CurrentLocation]	= OpCodes[Instruction-INSTRUCTION_CCC] | Mask;
			State->CurrentLocation++;
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 1 );
			break;
			}
		/*--------------------------------------------------------------------*/
		case INSTRUCTION_SOB:
			{
			index_t	Register;
			int		Offset;

			//OpCodeStart	= State->CurrentLocation;

			Token	= GetToken( Cookie, &Register, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_REGISTER )
				return Error( Cookie, "Expecting register name" );
			Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_COMMA )
				return Error( Cookie, "Expecting \',\'" );
			Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
			if( Token != TOKEN_NUMBER && Token != TOKEN_NUMBERHEX )
				return Error( Cookie, "Expecting address expression" );

			Offset	= atoi( Cookie->Buffer );
			if( Offset < -128 || Offset > 127 )
				return Error( Cookie, "Relative jump out of range" );

			State->Memory[State->CurrentLocation++]	= OPCODE_SOB;
			State->Memory[State->CurrentLocation++]	= Offset;
			UpdateHighWaterLine( State );

			//PrintOpCodes( State->Memory, OpCodeStart, 2 );
			}
			break;
		/*--------------------------------------------------------------------*/
		}

	Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
	if( Token == TOKEN_EOL )
		return 1;
	if( Token == TOKEN_EOF )
		return 0;

	return Error( Cookie, "Extra input at end of line" );
	}
/*============================================================================*/
static int	ProcessLine( cookie_t *Cookie, state_t *State )
	{
	index_t	Index;
	token_t	Token;
	char	Label[128];

	Token	= GetToken( Cookie, &Index, Label, sizeof Label );

	if( Token != TOKEN_IDENTIFIER )
		Label[0]	= '\0';
	else
		{
		Token	= GetToken( Cookie, NULL, Cookie->Buffer, sizeof Cookie->Buffer );
		if( Token != TOKEN_COLON )
			return Error( Cookie, "Expecting \':\'" );
		Token	= GetToken( Cookie, &Index, Cookie->Buffer, sizeof Cookie->Buffer );
		//printf( "\n%s:", Label );
		}

	if( Token == TOKEN_INSTRUCTION )
		return ProcessInstruction( Cookie, State, Label, Index );
	else if( Token == TOKEN_DIRECTIVE )
		return ProcessDirective( Cookie, State, Label, Index );
	else if( Token == TOKEN_EOL )
		{
		if( Label[0] != '\0' && InsertSymbol( Cookie, State, Label, State->CurrentLocation, TYPE_ADDRESS ) < 0 )
			return -1;
		return 1;
		}
	else if( Token == TOKEN_EOF )
		{
		if( Label[0] != '\0' && InsertSymbol( Cookie, State, Label, State->CurrentLocation, TYPE_ADDRESS ) < 0 )
			return -1;
		return 0;
		}

	return Error( Cookie, "Expecting label, directive or instruction" );
	}
/*============================================================================*/
static int CompareSymbols( const void *a, const void *b )
	{
	symbol_t	*A	= (symbol_t*)a;
	symbol_t	*B	= (symbol_t*)b;

	return A->Type != B->Type ? A->Type - B->Type: A->Value != B->Value ? A->Value - B->Value : strnicmp( A->Name, B->Name, sizeof A->Name );
	}
/*============================================================================*/
int ProcessFile( cookie_t *Cookie, state_t *State, const char *FileName, FILE *File )
	{
	int Result;

	for( State->Pass = 1; State->Pass <= NUM_PASSES; State->Pass++ )
		{
		fseek( File, 0, SEEK_SET );
		Cookie->LineNumber	= 1;
		Cookie->Column		= 1;

		while(( Result = ProcessLine( Cookie, State )) > 0 )
			{}

		if( Result < 0 )
			return Result;
		}

	qsort( State->SymbolTable, State->NumSymbols, sizeof State->SymbolTable[0], CompareSymbols );

#if 0
	printf( "\n       SYMBOL    TABLE       " );
	printf( "\nDec.  Hex.   Type     Value  " );
	printf( "\n----- ---- -------- ---------" );
	for( i = 0; i < State->NumSymbols; i++ )
		printf( "\n%5u %04x %-8s %s", State->SymbolTable[i].Value, State->SymbolTable[i].Value, State->SymbolTable[i].Type == TYPE_ADDRESS ? "Address" : "Constant", State->SymbolTable[i].Name );
#endif

	return Result;
	}
/*============================================================================*/
static void SplitPath( const char *pFullPath, char *pPath, char *pName, char *pExt )
	{
	char	FullPath[256], *p, *q;

	strncpy( FullPath, pFullPath, sizeof FullPath );
	FullPath[sizeof FullPath - 1]	= '\0';

	if(( p = strrchr( FullPath, '\\' )) != NULL )
		{
		*p++	= '\0';
		if( pPath != NULL )
			{
			strcpy( pPath, FullPath );
			strcat( pPath, "\\" );
			}
		}
	else if(( p = strrchr( FullPath, ':' )) != NULL )
		{
		*p++	= '\0';
		if( pPath != NULL )
			{
			strcpy( pPath, FullPath );
			strcat( pPath, ":" );
			}
		}
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
	else
		strcpy( pExt, "" );

	if( pName != NULL )
		strcpy( pName, p );
	}
/*============================================================================*/
int Process( const char *FileName )
	{
	cookie_t	Cookie;
	state_t		State;
	FILE		*File;
	int			Result;
	uint8_t		Memory[65536];
	symbol_t	SymbolTable[10000];
	char		Path[256];
	char		Name[256];
	char		Ext[256];

	if(( File = fopen( FileName, "rb" )) == NULL )
		return fprintf( stderr, "\nError: File \"%s\" not found.\n", FileName ), -1;

	Cookie.LineNumber		= 1;
	Cookie.Column			= 1;
	Cookie.File				= File;
	Cookie.TabWidth			= 8;

	State.CurrentLocation	= 0x0000;
	State.MaximumAddress	= 0x0000;
	State.Memory			= Memory;
	State.SymbolTable		= SymbolTable;
	State.SymbolTableLength	= LENGTH( SymbolTable );
	State.NumSymbols		= 0;

	memset( Memory, 0x00, sizeof Memory );

	Result	= ProcessFile( &Cookie, &State, FileName, File );

	fclose( File );

	if( Result >= 0 )
		{
		int	i;

		SplitPath( FileName, Path, Name, Ext );
		strcat( Path, Name );
		strcat( Path, ".casm.mem" );
		if(( File = fopen( Path, "wb" )) == NULL )
			{
			fprintf( stderr, "\nError: Could not create the output file \"%s\"\n", Path );
			return -1;
			}
		fwrite( "\x03" "C16", 4, 1, File );
		fwrite( Memory, 1, sizeof Memory, File );
		fclose( File );

		SplitPath( FileName, Path, Name, Ext );
		strcat( Path, Name );
		strcat( Path, ".casm.map" );
		if(( File = fopen( Path, "wb" )) == NULL )
			{
			fprintf( stderr, "\nError: Could not create map file \"%s\"\n", Path );
			return -1;
			}

		fprintf( File, "Símbolo                          Valor Decimal Valor Hexadecimal\r\n" );
		fprintf( File, "-------------------------------- ------------- -----------------" );
		for( i = 0; i < State.NumSymbols; i++ )
			fprintf( File, "\r\n%-32.32s %13u %17X", State.SymbolTable[i].Name, State.SymbolTable[i].Value, State.SymbolTable[i].Value );

		fclose( File );
		}


	return Result;
	}
/*============================================================================*/
static int ShowUsage( void )
	{
	fprintf( stderr,
		"\n\n"
		"casm v0.1 (built " __DATE__ " " __TIME__ ")"			"\n"
		"Copyright (c) 2024, Isaac Marino Bavaresco"			"\n"
		"An assembler for the abstract computer \"Cesar\" <https://www.inf.ufrgs.br/arq/wiki/doku.php?id=cesar>"	"\n\n"
		"Usage:\tcasm <source-file.ced>"						"\n\n"
		);
	return 0;
	}
/*============================================================================*/
int main( int ArgC, char *ArgV[] )
	{
	int	Result;

	if( ArgC == 1 )
		return ShowUsage();
	if( ArgC != 2 )
		return fprintf( stderr, "\nError: Wrong number of parameters\n" ), -1;

	Result	= Process( ArgV[1] );

	if( Result == 0 )
		fprintf( stderr, "\nAssembled without errors\n" );

	return Result;
	}
/*============================================================================*/
