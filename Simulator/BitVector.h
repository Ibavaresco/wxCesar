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
/*============================================================================*/
#if			!defined __BITVECTOR_H__
#define __BITVECTOR_H__
/*============================================================================*/
#include <stdint.h>
/*============================================================================*/
#ifdef	__cplusplus
extern "C"
	{
#endif
/*============================================================================*/
typedef struct bitvector_tag	bitvector_t;
//==============================================================================
bitvector_t	*BitVectorCreate		( size_t Length );
void		BitVectorDelete			( bitvector_t *BitVector );
int			BitVectorSetBit			( bitvector_t *BitVector, size_t BitIndex );
int			BitVectorClearBit		( bitvector_t *BitVector, size_t BitIndex );
int			BitVectorToggleBit		( bitvector_t *BitVector, size_t BitIndex );
int			BitVectorSetAllBits		( bitvector_t *BitVector );
int			BitVectorClearAllBits	( bitvector_t *BitVector );
int			BitVectorToggleAllBits	( bitvector_t *BitVector );
int			BitVectorChangeBit		( bitvector_t *BitVector, size_t BitIndex, int Value );
int			BitVectorQueryBit		( const bitvector_t *BitVector, size_t BitIndex );
/*============================================================================*/
#ifdef	__cplusplus
	}
#endif
/*============================================================================*/
#endif	//	!defined __BITVECTOR_H__
/*============================================================================*/
