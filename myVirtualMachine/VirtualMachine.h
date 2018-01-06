#include <stdio.h>
#include <stdlib.h>

//rS,rD �Ѵ��ִ� ���� type 0 0x01~0x1F
#define ADD 0x01
#define SUB 0x02
#define MUL 0x03
#define DIV 0x04
#define OR  0x05
#define AND 0x06
#define XOR 0x07
#define SHL 0x08
#define SHR 0x09


//rS �ϳ����� ���� type 1 0x21~0x3F
#define STORE 0x21
#define LOAD 0x22
#define INC 0x23
#define DEC 0x24
#define NOT 0x25

//Mask Byte
#define RS_MASK 0xF0
#define RD_MASK 0x0F
#define IMM_MASK 0x80
#define OPTYPE_MASK 0x60
#define OPNUM_MASK 0x1F
#define MemorySize 0x10000
#define RegNum 13

// �޸�, ��������, ���ɾ� ����ü ����
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
	unsigned short PC;
	unsigned short Zero;
	unsigned short BP;
	unsigned short SP;
	unsigned short Reg[RegNum];
}Register;

//����ü �������� ����
Instruction ins;
Memory mem;
Register reg;
//�ӽ� ����
int run_machine();
//�ӽſ��� �� ���۵�
int init();
int load();
int fetch();
int decode();
int execution(unsigned char opNum, unsigned char opBody, unsigned char opType, unsigned char isImm);
