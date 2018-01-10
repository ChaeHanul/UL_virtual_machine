#include "VirtualMachine.h"
int run_machine()
{
	init();
	if (load())
	{
		printf("exit!\n");
		exit(0);
	}
	while ((mem.InstructionArea[reg.PC] != 0xff) && (mem.InstructionArea[reg.PC] != 0xff))
	{
		fetch();
		decode();
	}
}
int init()
{
	for (int i = 0; i < MemorySize / 2; i++)
	{
		mem.InstructionArea[i] = 0xff;
		mem.DataArea[i] = 0x0;
	}
	for (int i = 0; i < RegNum; i++)
	{
		
		reg.Reg[i] = 0x0;
	}
	reg.Zero = 0x0;
	reg.SP = 0x0;
	reg.BP = 0x0;
	reg.PC = 0x0;
	return 0;
}
int load()
{
	FILE * file;
	if (fopen_s(&file, "test.bin", "r") == 0)
	{
		int count = 0;
		while (feof(file) == 0)
		{ 
			if (count >= MemorySize / 2)
			{
				printf("file size is bigger than Memory Size...");
				return 1;
			}
			count = fread(mem.InstructionArea, sizeof(char), MemorySize / 2, file);
		}
		if (count >= MemorySize / 2)
		{
			printf("file size is bigger than Memory Size...");
			return 1;
		}
		return 0;
	}
	else
	{
		printf("file open error!!\n");
		return 1;
	}
}
int fetch()
{
	//memory�� code �������� PC~PC+1 ����Ʈ�� �о ������
	ins.opHead = mem.InstructionArea[reg.PC]; // opNum����  PC �� �ش��ϴ� ����Ʈ�� (��ɾ� �������)
	ins.opBody = mem.InstructionArea[reg.PC+1]; // opData���� PC+1 �� �ش��ϴ� ����Ʈ�� (��ɾ� ���۷���)
	//PC�� 2�� �����Ѵ�
	reg.PC += 2;
	return 0;
}
int decode()
{
	//��ǻ�� �������� ���ڴ��� ���� ALU�� ������ ������ �Ѱ��ֹǷ� �ش� �Լ����� execution �Լ��� �����ϴ� ������ ���ڴ�.
	unsigned char opType = (ins.opHead&OPTYPE_MASK)>>5;
	unsigned char isImm = (ins.opHead&IMM_MASK)>>7;
	unsigned char opNum = ins.opHead&OPNUM_MASK;
	unsigned char opBody = ins.opBody;
	if (execution(opNum, opBody, opType, isImm))
	{
		printf("exit\n");
		exit(0);
	}
}
int execution(unsigned char opNum, unsigned char opBody, unsigned char opType, unsigned char isImm)
{
	unsigned char opHead = opNum | (opType << 5);
	
	if (opType == 0)
	{
		unsigned char rS = (opBody & RS_MASK) >> 4;
		unsigned char rD = (opBody & RD_MASK);
		short SourceValue[2] = { reg.Reg[rS],(short)rS }; // imm bit �� 1 �̶�� rS�� ���� 1�� �ƴ϶�� reg.Reg[rS]�� ���� ���
		switch (opHead)
		{
		case SUB:
			reg.Reg[rD] -= SourceValue[isImm];
			break;
		case ADD:
			reg.Reg[rD] += SourceValue[isImm];
			break;
		case MUL:
			reg.Reg[rD] *= SourceValue[isImm];
			break;
		case DIV:
			reg.Reg[rD] /= SourceValue[isImm];
			break;
		case OR:
			reg.Reg[rD] |= SourceValue[isImm];
			break;
		case AND:
			reg.Reg[rD] &= SourceValue[isImm];
			break;
		case XOR:
			reg.Reg[rD] ^= SourceValue[isImm];
			break;
		case SHL:
			reg.Reg[rD] <<= SourceValue[isImm];
			break;
		case SHR:
			reg.Reg[rD] >>= SourceValue[isImm];
			break;
		case CMP:
			if (reg.Reg[rD] - SourceValue[isImm] == 0)
			{
				reg.STATUS = ZERO_BIT;
			}
			else if (reg.Reg[rD] - SourceValue[isImm] > 0)
			{
				if (reg.Reg[rD] < 0 && SourceValue[isImm]>0)
				{
					reg.STATUS = OVER_BIT;
				}
			}
			else
			{
				reg.STATUS = SIGN_BIT;
				if (reg.Reg[rD] > 0 && SourceValue[isImm] < 0)
				{
					reg.STATUS |= OVER_BIT;
				}
			}
			break;
		default:
			printf("opType 0 >> opNum Error!\n");
			return 2;
			break;
		}
		return 0;
	}
	else if (opType == 1)
	{
		unsigned char rS = (opBody & RS_MASK) >> 4;
		switch (opHead)
		{
		case STORE:
			mem.DataArea[reg.SP] = (reg.Reg[rS] >> 8) & 0xff;
			mem.DataArea[reg.SP + 1] = (unsigned char)(reg.Reg[rS] & 0xff);
			reg.SP += 2;
			break;
		case LOAD:
			reg.Reg[rS] = mem.DataArea[reg.SP - 2]; //SP�� ������ �����̱� ������ SP-2 , SP-1�� �����͸� �����;��Ѵ�.
			reg.Reg[rS] <<= 8;
			reg.Reg[rS] |= mem.DataArea[reg.SP - 1];
			reg.SP -= 2;
			break;
		case INC:
			reg.Reg[rS]++;
			break;
		case DEC:
			reg.Reg[rS]--;
			break;
		case NOT:
			reg.Reg[rS] = ~reg.Reg[rS];
			break;
		default:
			printf("opType 1 >> opNum Error!\n");
			return 2;
			break;
		}
		return 0;
	}
	else if (opType == 2)
	{
		unsigned char rD = (opBody & RD_MASK);
		unsigned char status_a;
		unsigned char status_b;
		switch (opHead)
		{
		case B:
			reg.PC += reg.Reg[rD];
			break;
		case BEQ:
			status_a = ZERO_BIT&STATUS_MASK;
			if (status_a == reg.STATUS)
				reg.PC += reg.Reg[rD];
			break;
		case BNE:
			status_a = (ZERO_BIT)&STATUS_MASK;
			if (status_a != reg.STATUS)
				reg.PC += reg.Reg[rD];
			break;
		case BL:
			status_a = (SIGN_BIT | ~ZERO_BIT | ~OVER_BIT)&STATUS_MASK;
			status_b = (~SIGN_BIT | ~ZERO_BIT | OVER_BIT)&STATUS_MASK;
			if (status_a==reg.STATUS || status_b==reg.STATUS)
				reg.PC += reg.Reg[rD];
			break;
		case BG:
			status_a = (~SIGN_BIT | ~ZERO_BIT | ~OVER_BIT)&STATUS_MASK;
			status_b = (SIGN_BIT | ~ZERO_BIT | OVER_BIT)&STATUS_MASK;
			if (status_a == reg.STATUS || status_b == reg.STATUS)
			break;
		default:
			printf("opType 2 >> opNum Error!\n");
			return 3;
			break;
		}
	}
	else
	{
		printf("opType Error!\n");
		return 1;
	}
}