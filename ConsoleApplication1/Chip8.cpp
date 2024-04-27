#define MEMORY 0xFFF
#define STACK_SIZE 16
#define REG 16
#define START_BLOCK 0x200
#define SCREEN_SIZE 64*32

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstdio>
#include <string>
#include <cerrno>
#include <sstream>

namespace{
typedef unsigned char uch;
typedef unsigned short ush;
};

/*
	TO DO LIST:
	INIT REGISTERS AND MEMORY: COMPLETED
	LOAD ROM: COMPLETED
	READ FROM ROM: COMPLETED
	FETCH OPCODES: COMPLETED
	STORE OPCODES: COMPLETED
	DECODE OPCODES: IN PROGRESS
	EXECUTE OPCODES: NOT ATTEMPTED
	UPDATE TIMERS: NOT ATTEMPTED
	LOOP: WIP (Program will not close, not even at EOF)
*/

/*
Memory Map: (BY COWGOD)
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  | <--empty in this case
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
*/

uch _mem[MEMORY];
//each place in memory is exactly one byte

ush _stack[STACK_SIZE];
//an array of 16, 16-bit values used to represent the stack

uch V[REG];
// 16 general purpose registers
//do not use VF (V[16]) for any program, it's used as a carry flag sometimes

int opcode = 0;
//current opcode

ush I;
//one 16 bit register, for storing memory addresses
//though usually only the right most 12 bits are used...

ush PC = START_BLOCK;
//a program counter, stores the currently executing address

uch *SP;
//stack pointer, which points to the topmost level of the stack
//chip 8 also has two 8bit delay and sound timers, not created here

bool run = true;
//for running the loop

std::string _gamename;
uch _buf1;
uch _buf2;
ush H;
size_t _readsize1;
size_t _readsize2;


ush readop(FILE* filename,unsigned char _buffer1,unsigned char _buffer2){
	_readsize1 = fread(&_buffer1,1,1,filename);
	if(_readsize1 != 1){
		printf("Read error: %s\n", stderr);
		}

	_readsize2 = fread(&_buffer2,1,1,filename);
	if(_readsize2 != 1){
		printf("Read error: %s\n", stderr);
		}

	_mem[PC] = _buffer1;
	_mem[PC+1] = _buffer2;

	printf("Start position for this op is %d or %#04x\n",PC,PC);

	//printf does not have this issue
	printf("1st hex value %#02x written at %#04x\n",_mem[PC],PC);
	printf("2nd hex value %#02x written at %#04x\n",_mem[PC+1],PC+1);

	H = _buffer1 << 8 | _buffer2;
	PC += 2;
return H;
}

int Fet(FILE* filename){
	//opcode fetching COMPLETED
	opcode = readop(filename,_buf1,_buf2);

	printf("Opcode: %#04x\n",opcode);
	
	printf("Next position: %#04x\n",PC);
	
	return 0;
}

int decode_op(){
	unsigned short top = 4 & _mem[PC];
	printf("%hu \n",top);

return 0;
}

int main(){
	printf("Enter a game filename to start.\n");
	std::cin>>_gamename;

	//append an extension
	_gamename = _gamename + ".c8";

	//open game with error handling
	FILE* _game = fopen(_gamename.c_str(),"rb");
	if(_game == NULL){
		printf("Game did not open. %s\n", strerror(errno));
	}else{
	printf("Game successfully opened. \n");
	printf("Starting block for this game is %d or %#04x\n",PC,PC);
	}

	//set file read position using fseek
	if(fseek(_game,0,SEEK_SET) == 0){
	printf("Set file stream position success.\n\n");
	}else{
	printf("Set file stream position failure.\n");
	}


	while (run == true){
		Fet(_game);
		decode_op();

		std::cin.ignore();
		std::cin.get();
	}

return 0;
}