#ifndef TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_
#define TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_

struct GB_CPU {
	union {
		struct {
			unsigned char rA;
			// flags register
			struct {
				unsigned int Z : 1;
				unsigned int N : 1;
				unsigned int H : 1;
				unsigned int C : 1;
				unsigned int reserved : 4;
			} rF;
		} flag_struct;
		unsigned short rAF;
	};

	union {
		struct {
			unsigned char rB;
			unsigned char rC;
		} bc_struct;
		unsigned short rBC;
	};

	union {
		struct {
			unsigned char rD;
			unsigned char rE;
		} de_struct;
		unsigned short rDE;
	};

	union {
		struct {
			unsigned char rH;
			unsigned char rL;
		} hl_struct;
		unsigned short rHL;
	};

	unsigned short rPC;
	unsigned short rSP;	
};
GB_CPU cpu;
#endif
