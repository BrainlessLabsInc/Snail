#ifndef __BRAINLESS_CONSTS_DEFINES__
#define __BRAINLESS_CONSTS_DEFINES__
/*
Author : Shakti Misra
Copy Right: 2011
All are welcome to use these files, distribute, modify and release. But This notice should be included.
This software is given as is, for use and modification.
*/

//Data types
typedef unsigned long ulong;

#ifndef DIM
#define DIM( a ) ( sizeof(a)/sizeof(a[0]) )
#endif

#define BCHAR( a ) #a
#define WBCHAR( a ) L ## #a

#define VM_VERSION v0.4

const ulong REGISTER_FILE = 4;

const ulong TAPE_SIZE = 64;
const ulong ARR_SIZE_128 = 128;
const ulong BUFFER_SIZE = 256;

const unsigned int BLOCK_NAME_SIZE = 256;

typedef int BOOL;
#define TRUE 1
#define FALSE 0

//Hex constants
const ulong HEX_0 = 0x00000000;
const ulong HEX_1 = 0x00000001;
const ulong HEX_2 = 0x00000002;
const ulong HEX_3 = 0x00000003;
const ulong HEX_4 = 0x00000004;

const ulong COMPARE_FLAG_MASK = 0x00000003;

#endif //__BRAINLESS_CONSTS_DEFINES__