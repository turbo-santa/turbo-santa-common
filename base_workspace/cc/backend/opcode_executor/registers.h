#ifndef TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_
#define TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_

namespace backend {
namespace registers {

struct GB_CPU {
	union {
		struct {
			// flags register
			struct {
				unsigned int Z : 1;
				unsigned int N : 1;
				unsigned int H : 1;
				unsigned int C : 1;
				unsigned int reserved : 4;
			} rF;
      unsigned char rA;
		} flag_struct;
		unsigned short rAF;
	};

	union {
		struct {
			unsigned char rC;
			unsigned char rB;
		} bc_struct;
		unsigned short rBC;
	};

	union {
		struct {
			unsigned char rE;
			unsigned char rD;
		} de_struct;
		unsigned short rDE;
	};

	union {
		struct {
			unsigned char rL;
			unsigned char rH;
		} hl_struct;
		unsigned short rHL;
	};

	unsigned short rPC;
	unsigned short rSP;	
};

} // namespace registers
} // namespace backend
#endif // TURBO_SANTA_COMMON_BACK_END_REGISTERS_H_
