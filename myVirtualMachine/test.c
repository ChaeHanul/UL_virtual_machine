#include "VirtualMachine.h"

void main()
{
	//테스트 케이스 생성
	FILE * file;
	if (fopen_s(&file, "test.bin", "w") == 0)
	{
		//레지스터 하나에 대한 INC,DEC
		char opcode[2];
		//increase 레지스터에 0xff 생성됨
		opcode[0] = INC;
		opcode[1] = 0xa0;

		fwrite(opcode, sizeof(opcode), 1, file);

		//add 레지스터에 0xff00 생성됨
		opcode[0] = ADD;
		opcode[1] = 0xa0 | 0x0a;
		for (int i = 0; i < 8; i++)
		{
			fwrite(opcode, sizeof(opcode), 1, file);
		}
		//xor
		opcode[0] = XOR;
		opcode[1] = 0x9a;
		fwrite(opcode, sizeof(opcode), 1, file);
		//decrease
		opcode[0] = DEC;
		opcode[1] = 0xa0;
		fwrite(opcode, sizeof(opcode), 1, file);
		//sub
		opcode[0] = SUB;
		opcode[1] = 0xa9;
		fwrite(opcode, sizeof(opcode), 1, file);

		//store
		opcode[0] = STORE;
		opcode[1] = 0x90;
		fwrite(opcode, sizeof(opcode), 1, file);
		//load
		opcode[0] = LOAD;
		opcode[1] = 0x00;
		fwrite(opcode, sizeof(opcode), 1, file);
		//xor
		opcode[0] = XOR;
		opcode[1] = 0x01;
		fwrite(opcode, sizeof(opcode), 1, file);
		//add
		opcode[0] = ADD;
		opcode[1] = 0x11;
		for (int i = 0; i<3; i++)
		{
			fwrite(opcode, sizeof(opcode), 1, file);
		}
		//not
		opcode[0] = NOT;
		opcode[1] = 0x00;
		fwrite(opcode, sizeof(opcode), 1, file);
		//shl
		opcode[0] = SHL;
		opcode[1] = 0x10;
		fwrite(opcode, sizeof(opcode), 1, file);
		//shr
		opcode[0] = SHR;
		opcode[1] = 0x10;
		fwrite(opcode, sizeof(opcode), 1, file);
		fclose(file);

		run_machine();
	}
	else
	{
		printf("file open error!!\n");
	}
}
