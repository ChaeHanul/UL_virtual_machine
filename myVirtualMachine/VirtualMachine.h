#include <stdio.h>
#include <stdlib.h>

//rS,rD 둘다있는 연산 type 0 0x01~0x1F
#define ADD 0x01
#define SUB 0x02
#define MUL 0x03
#define DIV 0x04
#define OR  0x05
#define AND 0x06
#define XOR 0x07
#define SHL 0x08
#define SHR 0x09
#define CMP 0x0A

//rS 하나뿐인 연산 type 1 0x21~0x3F
#define STORE 0x21
#define LOAD 0x22
#define INC 0x23
#define DEC 0x24
#define NOT 0x25

//흐름 제어 명령어 type 2 Branch 시리즈
#define B   0x48
#define BEQ 0x49
#define BNE 0x4A
#define BL  0x4B
#define BG  0x4C

//Mask Byte
#define RS_MASK 0xF0
#define RD_MASK 0x0F
#define IMM_MASK 0x80
#define OPTYPE_MASK 0x60
#define OPNUM_MASK 0x1F
#define STATUS_MASK 0x07

//STATUS BIT
#define ZERO_BIT 0x04 //영
#define SIGN_BIT 0x02 //음수
#define OVER_BIT 0x01 //오버플로우


//메모리 크기, 공용레지스터 갯수
#define MemorySize 0x10000
#define RegNum 13

// 메모리, 레지스터, 명령어 구조체 선언
typedef struct
{
	unsigned char opHead;
	unsigned char opBody;
}Instruction;
typedef struct
{
	unsigned char InstructionArea[MemorySize / 2];
	unsigned char DataArea[MemorySize / 2];
}Memory;
typedef struct
{
	unsigned char STATUS;
	unsigned short PC;
	unsigned short Zero;
	unsigned short BP;
	unsigned short SP;
	short Reg[RegNum];
}Register;

//구조체 전역변수 생성
Instruction ins;
Memory mem;
Register reg;
//머신 실행
int run_machine();
//머신에서 할 동작들
int init();
int load();
int fetch();
int decode();
int execution(unsigned char opNum, unsigned char opBody, unsigned char opType, unsigned char isImm);

