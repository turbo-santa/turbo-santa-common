#ifndef TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_
#define TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_

struct GB_CPU {
	union flag_union {
		struct flag_struct {
			unsigned char rA;
			// flags register
			struct rF {
				unsigned int Z : 1;
				unsigned int N : 1;
				unsigned int H : 1;
				unsigned int C : 1;
				unsigned int reserved : 4;
			};
		};
		unsigned short rAF;
	};

	union bc_union {
		struct bc_struct {
			unsigned char rB;
			unsigned char rC;
		};
		unsigned short rBC;
	};

	union de_union {
		struct de_struct {
			unsigned char rD;
			unsigned char rE;
		};
		unsigned short rDE;
	};

	union hl_union {
		struct hl_struct {
			unsigned char rH;
			unsigned char rL;
		};
		unsigned short rHL;
	};

	unsigned short rPC;
	unsigned short rSP;	
};
GB_CPU cpu;
#endif