#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>


unsigned long long cpu_cycles = 0;

#define ENUM_TO_STR(REG_NAME) #REG_NAME
#define REGS_SIZE   32
#define IMEM_SIZE   32*1024 // 4 KiB of instruction memory (I$)
#define DMEM_SIZE   32*1024 // 4 KiB of data memory (D$)

long        reg[REGS_SIZE];  //(1) -> 32 (general-purpose) registers
unsigned long   PC = 0;   //(2) -> Let's assume that program segment starts from 0x00000000
unsigned long   inst_mem[IMEM_SIZE];
unsigned long   data_mem[DMEM_SIZE];

char            DMEM_flag[DMEM_SIZE];   // a set of flags for checking the data memory is touched or not

unsigned long IF(unsigned long addr);
unsigned long ID(unsigned addr_rs1, unsigned addr_rs2);
unsigned long EX(unsigned long inputA, unsigned long inputB, int ALUSel);
unsigned long MEM(unsigned long addr, unsigned long dataW, int MemRW);
unsigned long WB(unsigned long dataD, unsigned addr_rd1, int RegWEn);
unsigned long Control_Unit(unsigned long inst, int BrEq, int BrLT);

void init_states(std::ifstream& binary_input);
void print_statistics(); // print processor's clock cycles
void dump_registers();   // print all contents of registers
void dump_memory();      // print all used data in (data) memory

std::string g_REG_NAMEs[32] = {
	"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
	"s0", "s1", "a0", "a1", "a2", "a3", "a4","a5", "a6", "a7",
	"s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
	"t3", "t4", "t5", "t6"
};
enum RegType
{
	zero, ra, sp, gp, tp, t0, t1, t2,
	s0, s1, a0, a1, a2, a3, a4, a5, a6, a7,
	s2, s3, s4, s5, s6, s7, s8, s9, s10, s11,
	t3, t4, t5, t6
};
enum OpType
{
	I, S, B, J, U, R
};

enum ExeState
{
	ADD, SUB, MUL, DIV, REM, OR, XOR, AND, SLL, SRL, ALU_B
};
enum RwType
{
	MEM_READ, MEM_WRITE
};
enum InputA {
	SRC_REG1, PC_REG
};
enum InputB {
	SRC_REG2, IMM_VAL
};
enum BoolState
{
	FALSE, TRUE
};

enum PCInput
{
	PC_PLUS_4, PC_PLUS_IMM
};
int main(int argc, char** argv)
{
	bool should_halt = false;

	std::ifstream _exe;
	if (argc != 2)
	{
		std::cout << "Any argument isn't found..So run test t2.s, instead" << std::endl;
		_exe.open("t2.s");
	}
	else
		_exe.open(argv[1]);
	init_states(_exe);
	while (!should_halt)
	{
		unsigned long  opcode_register = IF(PC);
		if (opcode_register == 0)
		{
			std::cout << "No more codes!" << std::endl;
			break;
		}
		unsigned ImmSel = (Control_Unit(opcode_register, TRUE, TRUE) >> 1) & 0b111;
		unsigned long immediate_val = 0;
		unsigned long next_PC = 0;
		switch (ImmSel)
		{
		case I:
			//20~31번 비트: imm[11:0]
			immediate_val = (opcode_register >> 20) & 0b111111111111;
			if ((opcode_register >> 31 & 0b1) == 0b1) //음수 일 경우
				immediate_val |= 0b11111111111111111111 << 12;

			break;
		case S:
			//25~31번 비트: imm[11:5] 7~ 11번 비트: imm[4:0]
			immediate_val |= (opcode_register >> 7) & 0b11111;
			immediate_val |= ((opcode_register >> 25) & 0b1111111) << 5;
			if ((opcode_register >> 31 & 0b1) == 0b1) //음수 일 경우
				immediate_val |= 0b11111111111111111111 << 12;
			break;
		case B:
			//31번 비트: imm[12] 25 ~ 30번 비트: imm[10:5] 8~11번 비트: imm[4:1] 7번 비트: imm[11]
			immediate_val |= ((opcode_register >> 7) & 0b1) << 11;
			immediate_val |= ((opcode_register >> 8) & 0b1111) << 1;
			immediate_val |= ((opcode_register >> 25) & 0b111111) << 5;
			immediate_val |= (opcode_register >> 31) << 12;
			if (((immediate_val >> 12) & 0b1) == 0b1) //음수 일 경우
				immediate_val |= 0b1111111111111111111 << 13;
			break;
		case J:
			//31번 비트: imm[20] 21~30번 비트: imm[10:1] 20번 비트: imm[11] 12~19번 비트: imm[19:12]
			immediate_val |= ((opcode_register >> 12) & 0b11111111) << 12;
			immediate_val |= ((opcode_register >> 20) & 0b1) << 11;
			immediate_val |= ((opcode_register >> 21) & 0b1111111111) << 1;
			immediate_val |= ((opcode_register >> 31) & 0b1) << 20;
			if (((immediate_val >> 20) & 0b1) == 0b1) //음수 일 경우
				immediate_val |= 0b11111111111 << 21;
			break;
		case U:
			//12~31번 비트: imm[31:12] 
			immediate_val = ((opcode_register >> 12) & 0b11111111111111111111) << 12;
			break;
		default:
			break;
		}

		unsigned ASel = (Control_Unit(opcode_register, TRUE, TRUE) >> 5) & 0b1;
		unsigned long val_A = 0;
		unsigned long src1_reg_val = ID(0, (opcode_register >> 15) & 0b11111);
		if (ASel == PC_REG)//ASel == 1 -> PC for branch
		{
			val_A = PC;
		}
		else //ASel == 0 -> Source Register 1
		{
			val_A = src1_reg_val;
		}

		unsigned BSel = (Control_Unit(opcode_register, TRUE, TRUE) >> 6) & 0b1;
		unsigned long val_B = 0;
		unsigned long src2_reg_val = ID((opcode_register >> 20) & 0b11111, 0);
		if (BSel == IMM_VAL) //BSel == 1 -> Imm
		{
			val_B = immediate_val;
		}
		else //BSel == 0 -> Source Register 2 
		{
			val_B = src2_reg_val;
		}

		unsigned BrUn = (Control_Unit(opcode_register, TRUE, TRUE) >> 4) & 0b1;
		int BrLt = TRUE;
		int BrEq = TRUE;
		if (src1_reg_val == src2_reg_val)
			BrEq = TRUE;
		else
			BrEq = FALSE;

		if (BrUn == TRUE)
		{
			if (src1_reg_val < src2_reg_val)
				BrLt = TRUE;
			else
				BrLt = FALSE;
		}
		else
		{
			long* ptr_A = reinterpret_cast<long*>(&src1_reg_val);
			long* ptr_B = reinterpret_cast<long*>(&src2_reg_val);
			//unsigned 타입을 signed로 데이터 비트 유지한 채 포인터 크기만 변환 
			if (*ptr_A < *ptr_B)
				BrLt = TRUE;
			else
				BrLt = FALSE;
		}


		unsigned ALUSel = (Control_Unit(opcode_register, TRUE, TRUE) >> 7) & 0b1111;
		unsigned long result = EX(val_A, val_B, ALUSel);
		unsigned PCSel = Control_Unit(opcode_register, BrEq, BrLt) & 0b1;


		if (PCSel == PC_PLUS_IMM)
			next_PC = result;
		else
			next_PC = PC + 4;


		if (ImmSel != B)
		{
			unsigned MemRW = (Control_Unit(opcode_register, TRUE, TRUE) >> 11) & 0b1;
			unsigned WBSel = (Control_Unit(opcode_register, TRUE, TRUE) >> 13) & 0b11;
			unsigned RegWEn = (Control_Unit(opcode_register, TRUE, TRUE) >> 12) & 0b1;

			switch (WBSel)
			{
			case 0:
				//MemRW == 0 -> Read
				//MemRW == 1 -> Write
				WB(MEM(result, src2_reg_val, MemRW), (opcode_register >> 7) & 0b11111, RegWEn);
				break;
			case 1:
				WB(result, (opcode_register >> 7) & 0b11111, RegWEn);
				break;
			case 2:
				WB(PC + 4, (opcode_register >> 7) & 0b11111, RegWEn);
				break;
			default:
				break;
			}
		}

		if (reg[31] == 0xDEADBEEF)
			should_halt = true;
		PC = next_PC;
		cpu_cycles++;
	}

	print_statistics(); // print processor's clock cycles
	dump_registers();   // print all contents of registers
	dump_memory();      // print all used data in (data) memory
	_exe.close();
	return 0;
}


unsigned long str_to_bin(const std::string& binary_string)
{
	unsigned long result = 0;
	unsigned digit = 0;
	for (auto reverse_iter = binary_string.rbegin(); reverse_iter != binary_string.rend(); reverse_iter++)
	{
		if (*reverse_iter == '1')
			result += 1 << digit;
		digit++;
	}
	return result;
}
void init_states(std::ifstream& binary_input)
{
	if (!binary_input.is_open())
	{
		std::cout << "File path is missing..." << std::endl;
		exit(-1);
	}

	std::string input;
	unsigned long* p_inst = inst_mem;
	while (true)
	{
		binary_input >> input;
		*p_inst = str_to_bin(input);
		p_inst++;
		if (binary_input.eof())
			break;
	}
	memset(reg, 0, sizeof(long int) * REGS_SIZE);
	reg[sp] = DMEM_SIZE * 4 - 4;
	PC = 0;
	cpu_cycles = 0;
	memset(DMEM_flag, 0, sizeof(char) * DMEM_SIZE);
}


/* TODO: your work should be done with completing these five functions: */
unsigned long IF(unsigned long addr)
{
	return inst_mem[addr / 4];
}

unsigned long ID(unsigned addr_rs1, unsigned addr_rs2)
{

	if (addr_rs1 == 0)
		return reg[addr_rs2];
	return reg[addr_rs1];
}

unsigned long EX(unsigned long inputA, unsigned long inputB, int ALUSel)
{
	switch (ALUSel)
	{
	case ADD:
		return inputA + inputB;
	case SUB:
		return inputA - inputB;
	case MUL:
		return inputA * inputB;
	case DIV:
		return inputA / inputB;
	case REM:
		return inputA % inputB;
	case OR:
		return inputA | inputB;
	case XOR:
		return inputA ^ inputB;
	case AND:
		return inputA & inputB;
	case SLL:
		return inputA << inputB;
	case SRL:
		return inputA >> inputB;
	case ALU_B:
		return inputB;
	default:
		break;
	}
	return 0;
}

unsigned long MEM(unsigned long addr, unsigned long dataW, int MemRW)
{
	if (MemRW == MEM_READ)
	{
		return data_mem[addr / 4];
	}
	else
	{
		DMEM_flag[addr / 4] = 1;
		data_mem[addr / 4] = dataW;
		return 0;
	}
}

unsigned long WB(unsigned long dataD, unsigned addr_rd1, int RegWEn)
{
	if (RegWEn == TRUE)
		reg[addr_rd1] = dataD;

	return 0;
}

unsigned long Control_Unit(unsigned long inst, int BrEq, int BrLT)
{
	unsigned int opcode = inst & 0b1111111;
	unsigned long encoded_result = 0;
	if (opcode == 0b0000011)// Load
	{
		encoded_result |= 0; //PcSel
		encoded_result |= I << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= SRC_REG1 << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ADD << 7; //ALUSel
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 1 << 12; //RegWEn
		encoded_result |= 0 << 13; //WBSel

	}
	else if (opcode == 0b0100011) //Store
	{
		encoded_result |= 0; //PcSel
		encoded_result |= S << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= SRC_REG1 << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ADD << 7; //ALUSel
		encoded_result |= MEM_WRITE << 11; //MemRW
		encoded_result |= 0 << 12; //RegWEn
		encoded_result |= 0 << 13; //WBSel
	}
	else if (opcode == 0b0110011) //2 register ALU Operation
	{
		encoded_result |= 0; //PcSel
		encoded_result |= R << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= SRC_REG1 << 5; //ASel
		encoded_result |= SRC_REG2 << 6; //BSel
		unsigned int funct3 = (inst >> 12) & 0b111;
		unsigned int reminder = inst >> 25;

		if (funct3 == 0b000)
		{
			if (reminder == 0b0000000)
			{
				encoded_result |= ADD << 7; //ALUSel

			}
			else if (reminder == 0b0010100)
			{
				encoded_result |= SUB << 7; //ALUSel
			}
			else if (reminder == 0b0000001)
			{
				encoded_result |= MUL << 7; //ALUSel
			}
		}
		else if (funct3 == 0b100)
		{
			if (reminder == 0b0000001)
			{
				encoded_result |= DIV << 7; //ALUSel
			}
			else if (reminder == 0b0000000)
			{
				encoded_result |= XOR << 7; //ALUSel
			}
		}
		else if (funct3 == 0b110)
		{
			if (reminder == 0b0000001)
			{
				encoded_result |= REM << 7; //ALUSel
			}
			else if (reminder == 0b0000000)
			{
				encoded_result |= OR << 7; //ALUSel
			}
		}
		else if (funct3 == 0b111)
		{
			if (reminder == 0b0000000)
			{
				encoded_result |= AND << 7; //ALUSel
			}
		}
		else if (funct3 == 0b001)
		{
			if (reminder == 0b0000000)
			{
				encoded_result |= SLL << 7; //ALUSel
			}
		}
		else if (funct3 == 0b101)
		{
			if (reminder == 0b0000000)
			{
				encoded_result |= SRL << 7; //ALUSel
			}
		}
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 1 << 12; //RegWEn
		encoded_result |= 1 << 13; //WBSel
	}
	else if (opcode == 0b0010011) //1 Immediate 1 register Operation
	{
		unsigned int funct3 = (inst >> 12) & 0b111;
		encoded_result |= 0; //PcSel
		encoded_result |= I << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= SRC_REG1 << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel

		if (funct3 == 0b000) //ADDI
		{
			encoded_result |= ADD << 7; //ALUSel
		}
		else if (funct3 == 0b001) //SLLI
		{
			encoded_result |= SLL << 7;
		}
		else if (funct3 == 0b101) //SRLI
		{
			encoded_result |= SRL << 7;
		}
		else if (funct3 == 0b100) //XORI
		{
			encoded_result |= XOR << 7;
		}
		else if (funct3 == 0b110) //ORI
		{
			encoded_result |= OR << 7;
		}
		else if (funct3 == 0b111) //ANDI
		{
			encoded_result |= AND << 7;
		}
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 1 << 12; //RegWEn
		encoded_result |= 1 << 13; //WBSel

	}
	else if (opcode == 0b0110111) //LUI Operation
	{
		encoded_result |= 0; //PcSel
		encoded_result |= U << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= SRC_REG1 << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ALU_B << 7; //ALUSel
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 1 << 12; //RegWEn
		encoded_result |= 1 << 13; //WBSel
	}
	else if (opcode == 0b0010111) //AUIPC Operation
	{
		encoded_result |= 0; //PcSel
		encoded_result |= U << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= PC_REG << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ALU_B << 7; //ALUSel
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 1 << 12; //RegWEn
		encoded_result |= 1 << 13; //WBSel
	}
	else if (opcode == 0b1100011) //Branch Operation
	{

		unsigned int funct3 = (inst >> 12) & 0b111;

		if (funct3 == 0b000 && BrEq == TRUE) //BEQ
		{
			encoded_result |= 1; //PcSel
		}
		else if (funct3 == 0b001 && BrEq == FALSE) //BNE
		{
			encoded_result |= 1; //PcSel
		}
		else if (funct3 == 0b101 && BrLT == FALSE) //BGE
		{
			encoded_result |= 1; //PcSel
		}
		else if (funct3 == 0b111 && BrLT == FALSE) //BGEU
		{
			encoded_result |= 1; //PcSel
		}
		else if (funct3 == 0b100 && BrLT == TRUE) //BLT
		{
			encoded_result |= 1; //PcSel
		}
		else if (funct3 == 0b110 && BrLT == TRUE) //BLTU
		{
			encoded_result |= 1; //PcSel
		}
		else
		{
			encoded_result |= 0;
		}
		encoded_result |= B << 1; //ImmSel

		if (funct3 == 0b110 || funct3 == 0b111)
		{
			encoded_result |= 1 << 4; //BrUn
		}
		else
		{
			encoded_result |= 0 << 4; //BrUn
		}
		encoded_result |= PC_REG << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ADD << 7; //ALUSel
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 0 << 12; //RegWEn
		encoded_result |= 0 << 13; //WBSel
	}
	else if (opcode == 0b1101111) //JAL Operation
	{
		encoded_result |= 1; //PcSel
		encoded_result |= J << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= PC_REG << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ADD << 7; //ALUSel
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 1 << 12; //RegWEn
		encoded_result |= 2 << 13; //WBSel
	}
	else if (opcode == 0b1100111) //JALR Operation
	{
		encoded_result |= 1; //PcSel
		encoded_result |= I << 1; //ImmSel
		encoded_result |= 0 << 4; //BrUn
		encoded_result |= SRC_REG1 << 5; //ASel
		encoded_result |= IMM_VAL << 6; //BSel
		encoded_result |= ADD << 7; //ALUSel
		encoded_result |= MEM_READ << 11; //MemRW
		encoded_result |= 0 << 12; //RegWEn
		encoded_result |= 2 << 13; //WBSel
	}
	return encoded_result;
}
void print_statistics()
{
	std::cout << "Processor's clock cycles: " << cpu_cycles << std::endl;
}

void dump_registers()
{
	std::cout << std::endl << ">>>>>>>>>[REGISTER DUMP]<<<<<<<<<" << std::endl;
	std::cout << std::setw(10) << std::left << "Register";
	std::cout << std::setw(12) << std::left << "Hex";
	std::cout << std::setw(12) << std::left << "Decimal" << std::endl;
	std::cout << std::setw(10) << "PC: ";
	std::cout << std::setw(12) << std::hex << std::showbase << PC;
	std::cout << std::setw(12) << std::dec << PC << std::endl;
	for (unsigned int i = 0; i < REGS_SIZE; i++)
	{
		std::cout << std::setw(10) << "x" + std::to_string(i) + "(" + g_REG_NAMEs[i] + "): ";
		std::cout << std::setw(12) << std::hex << std::showbase << reg[i];
		std::cout << std::setw(12) << std::dec << reg[i] << std::endl;
	}
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
}

void dump_memory()
{
	std::cout << std::endl << ">>>>>>>>>>>[MEMORY DUMP]<<<<<<<<<<<<" << std::endl;
	std::cout << std::setw(16) << "Address";
	std::cout << std::setw(12) << "Hex";
	std::cout << std::setw(12) << "Decimal" << std::endl;
	for (int i = 0; i < DMEM_SIZE; i++)
	{
		if (DMEM_flag[i] != 0)
		{
			std::cout << std::left << std::setw(16) << std::showbase << std::hex << 4 * i;
			std::cout << std::left << std::setw(12) << std::showbase << data_mem[i];
			std::cout << std::left << std::setw(12) << std::dec << data_mem[i] << std::endl;
		}
	}
	std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;
}