/*===============================================================================
Copyright (c) 2004, Isaac Marino Bavaresco
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
#include <stdlib.h>
#include "BitVector.h"
/*============================================================================*/
typedef struct bitvector_tag
	{
	size_t		Length;
	size_t		LengthQWords;
	uint64_t	Storage[];
	} bitvector_t;
//==============================================================================
bitvector_t *BitVectorCreate( size_t Length )
	{
	bitvector_t *BitVector;
	size_t		LengthQWords, i;

	LengthQWords	= ( Length + 8 * sizeof BitVector->Storage[0] - 1 ) / sizeof BitVector->Storage[0];

	if(( BitVector = malloc( sizeof( bitvector_t ) + LengthQWords * sizeof BitVector->Storage[0] )) == NULL )
		return NULL;

	BitVector->Length		= Length;
	BitVector->LengthQWords	= LengthQWords;

	for( i = 0; i < BitVector->LengthQWords; i++ )
		BitVector->Storage[i]	= 0;

	return BitVector;
	}
/*============================================================================*/
void BitVectorDelete( bitvector_t *BitVector )
	{
	if( BitVector != NULL )
		free( BitVector );
	}
//==============================================================================
int BitVectorQueryBit( const bitvector_t *BitVector, size_t BitIndex )
	{
	if( BitVector == NULL || BitIndex < 0 || BitIndex >= BitVector->Length )
		return 0;

	return (int)(( BitVector->Storage[BitIndex / ( 8 * sizeof BitVector->Storage[0] )] >> ( BitIndex & ( 8 * sizeof BitVector->Storage[0] - 1 ))) & 1 );
	}
//==============================================================================
int BitVectorSetBit( bitvector_t *BitVector, size_t BitIndex )
	{
	if( BitVector == NULL || BitIndex < 0 || BitIndex >= BitVector->Length )
		return 0;

	BitVector->Storage[BitIndex / ( 8 * sizeof BitVector->Storage[0] )]   |= (uint64_t)1 << ( BitIndex & ( 8 * sizeof BitVector->Storage[0] - 1 ));

	return 1;
	}
//==============================================================================
int BitVectorClearBit( bitvector_t *BitVector, size_t BitIndex )
	{
	if( BitVector == NULL || BitIndex < 0 || BitIndex >= BitVector->Length )
		return 0;

	BitVector->Storage[BitIndex / ( 8 * sizeof BitVector->Storage[0] )]   &= ~( (uint64_t)1 << ( BitIndex & ( 8 * sizeof BitVector->Storage[0] - 1 )));

	return 1;
	}
//==============================================================================
int BitVectorToggleBit( bitvector_t *BitVector, size_t BitIndex )
	{
	if( BitVector == NULL || BitIndex < 0 || BitIndex >= BitVector->Length )
		return 0;

	BitVector->Storage[BitIndex / ( 8 * sizeof BitVector->Storage[0] )]   ^= (uint64_t)1 << ( BitIndex & ( 8 * sizeof BitVector->Storage[0] - 1 ));

	return 1;
	}
//==============================================================================
int BitVectorSetAllBits( bitvector_t *BitVector )
	{
	size_t	i;

	if( BitVector == NULL || BitVector->Length == 0 || BitVector->LengthQWords == 0 )
		return 0;

	for( i = 0; i < BitVector->LengthQWords; i++ )
		BitVector->Storage[i]	= (uint64_t)-1;

	return 1;
	}
//==============================================================================
int BitVectorClearAllBits( bitvector_t *BitVector )
	{
	size_t	i;

	if( BitVector == NULL || BitVector->Length == 0 || BitVector->LengthQWords == 0 )
		return 0;

	for( i = 0; i < BitVector->LengthQWords; i++ )
		BitVector->Storage[i]	= 0;

	return 1;
	}
//==============================================================================
int BitVectorToggleAllBits( bitvector_t *BitVector )
	{
	size_t	i;

	if( BitVector == NULL || BitVector->Length == 0 || BitVector->LengthQWords == 0 )
		return 0;

	for( i = 0; i < BitVector->LengthQWords; i++ )
		BitVector->Storage[i] ^= (uint64_t)-1;

	return 1;
	}
//==============================================================================
