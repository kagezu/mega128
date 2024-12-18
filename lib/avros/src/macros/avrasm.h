/*
 * avrasm.h
 *
 *  Created on: 17.10.2012
 *      Author: Andrey Sharoyko <vanyamboe@gmail.com>
 */

#ifndef AVRASM_H_
#define AVRASM_H_

#include <inttypes.h>

inline void __ADC(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("adc %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __ADD(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("add %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __ADIW(register uint16_t Rdl, char K)
{
  __asm__ __volatile__("adiw %0, %1" : "=w" (Rdl) : "I" (K));
}

inline void __SUB(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("sub %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __SUBI(register uint8_t Rd, uint8_t K)
{
  __asm__ __volatile__("subi %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __SBC(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("sbc %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __SBCI(register uint8_t Rd, uint8_t K)
{
  __asm__ __volatile__("sbci %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __SBIW(register uint16_t Rdl, char K)
{
  __asm__ __volatile__("sbiw %0, %1" : "=w" (Rdl) : "I" (K));
}

inline void __AND(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("and %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __ANDI(register uint16_t Rd, uint8_t K)
{
  __asm__ __volatile__("andi %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __OR(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("or %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __ORI(register uint16_t Rd, uint8_t K)
{
  __asm__ __volatile__("ori %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __EOR(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("eor %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __COM(register uint8_t Rd)
{
  __asm__ __volatile__("com %0" : "=r" (Rd));
}

inline void __NEG(register uint8_t Rd)
{
  __asm__ __volatile__("neg %0" : "=r" (Rd));
}

inline void __SBR(register uint16_t Rd, uint8_t K)
{
  __asm__ __volatile__("sbr %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __CBR(register uint16_t Rd, uint8_t K)
{
  __asm__ __volatile__("cbr %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __INC(register uint8_t Rd)
{
  __asm__ __volatile__("inc %0" : "=r" (Rd));
}

inline void __DEC(register uint8_t Rd)
{
  __asm__ __volatile__("dec %0" : "=r" (Rd));
}

inline void __TST(register uint8_t Rd)
{
  __asm__ __volatile__("tst %0" : "=r" (Rd));
}

inline void __CLR(register uint8_t Rd)
{
  __asm__ __volatile__("clr %0" : "=r" (Rd));
}

inline void __SER(register uint8_t Rd)
{
  __asm__ __volatile__("ser %0" : "=r" (Rd));
}

inline void __MUL(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("mul %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __MULS(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("muls %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __MULSU(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("mulsu %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __FMUL(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("fmul %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __FMULS(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("fmuls %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __FMULSU(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("fmulsu %0, %1" : "=r" (Rd) : "r" (Rr));
}

#define __LABEL(label) __asm__ __volatile__ (#label ":")
#define __RJMP(label) __asm__ __volatile__ ("rjmp " #label)
#define __IJMP __asm__ __volatile__ ("ijmp")
#define __JMP(label) __asm__ __volatile__ ("jmp " #label)
#define __RCALL(label) __asm__ __volatile__ ("rcall " #label)
#define __ICALL __asm__ __volatile__ ("icall")
#define __CALL(label) __asm__ __volatile__ ("call " #label)
#define __RET __asm__ __volatile__ ("ret")
#define __RETI __asm__ __volatile__ ("reti")

inline void __CPSE(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("cpse %0, %1" : : "r" (Rd), "r" (Rr));
}

inline void __CP(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("cp %0, %1" : : "r" (Rd), "r" (Rr));
}

inline void __CPC(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("cpc %0, %1" : : "r" (Rd), "r" (Rr));
}

inline void __CPI(register uint8_t Rd, uint8_t K)
{
  __asm__ __volatile__("cp %0, %1" : : "d" (Rd), "M" (K));
}

inline void __SBRC(register uint8_t Rd, char K)
{
  __asm__ __volatile__("sbrc %0, %1" : : "r" (Rd), "I" (K));
}

inline void __SBRS(register uint8_t Rd, char K)
{
  __asm__ __volatile__("sbrs %0, %1" : : "r" (Rd), "I" (K));
}

inline void __SBIC(char P, char K)
{
  __asm__ __volatile__("sbic %0, %1" : : "I" (P), "I" (K));
}

inline void __SBIS(char P, char K)
{
  __asm__ __volatile__("sbis %0, %1" : : "I" (P), "I" (K));
}

#define __BRBS(bit, label) __asm__ __volatile__ ("brbs " #bit ", " #label)
#define __BRBC(bit, label) __asm__ __volatile__ ("brbc " #bit ", " #label)
#define __BREQ(label) __asm__ __volatile__ ("breq " #label)
#define __BRNE(label) __asm__ __volatile__ ("brne " #label)
#define __BRCS(label) __asm__ __volatile__ ("brcs " #label)
#define __BRCC(label) __asm__ __volatile__ ("brcc " #label)
#define __BRSH(label) __asm__ __volatile__ ("brsh " #label)
#define __BRLO(label) __asm__ __volatile__ ("brlo " #label)
#define __BRMI(label) __asm__ __volatile__ ("brmi " #label)
#define __BRPL(label) __asm__ __volatile__ ("brpl " #label)
#define __BRGE(label) __asm__ __volatile__ ("brge " #label)
#define __BRLT(label) __asm__ __volatile__ ("brlt " #label)
#define __BRHS(label) __asm__ __volatile__ ("brhs " #label)
#define __BRHC(label) __asm__ __volatile__ ("brhc " #label)
#define __BRTS(label) __asm__ __volatile__ ("brts " #label)
#define __BRTC(label) __asm__ __volatile__ ("brtc " #label)
#define __BRVS(label) __asm__ __volatile__ ("brvs " #label)
#define __BRVC(label) __asm__ __volatile__ ("brvc " #label)
#define __BRIE(label) __asm__ __volatile__ ("brie " #label)
#define __BRID(label) __asm__ __volatile__ ("brid " #label)

inline void __SBI(char P, char K)
{
  __asm__ __volatile__("sbi %0, %1" : : "I" (P), "I" (K));
}

inline void __CBI(char P, char K)
{
  __asm__ __volatile__("cbi %0, %1" : : "I" (P), "I" (K));
}

inline void __LSL(register uint8_t Rd)
{
  __asm__ __volatile__("lsl %0" : "=r" (Rd));
}

inline void __LSR(register uint8_t Rd)
{
  __asm__ __volatile__("lsr %0" : "=r" (Rd));
}

inline void __ROL(register uint8_t Rd)
{
  __asm__ __volatile__("rol %0" : "=r" (Rd));
}

inline void __ROR(register uint8_t Rd)
{
  __asm__ __volatile__("ror %0" : "=r" (Rd));
}

inline void __ASR(register uint8_t Rd)
{
  __asm__ __volatile__("asr %0" : "=r" (Rd));
}

inline void __SWAP(register uint8_t Rd)
{
  __asm__ __volatile__("swap %0" : "=r" (Rd));
}

inline void __BSET(char s)
{
  __asm__ __volatile__("bset %0" : : "I" (s));
}

inline void __BCLR(char s)
{
  __asm__ __volatile__("bclr %0" : : "I" (s));
}

inline void __BST(register uint8_t Rd, char b)
{
  __asm__ __volatile__("bst %0, %1" : "=r" (Rd) : "I" (b));
}

inline void __BLD(register uint8_t Rd, char b)
{
  __asm__ __volatile__("bld %0, %1" : "=r" (Rd) : "I" (b));
}

#define __SEC __asm__ __volatile__ ("sec")
#define __CLC __asm__ __volatile__ ("clc")
#define __SEN __asm__ __volatile__ ("sen")
#define __CLN __asm__ __volatile__ ("cln")
#define __SEZ __asm__ __volatile__ ("sez")
#define __CLZ __asm__ __volatile__ ("clz")
#define __SEI __asm__ __volatile__ ("sei")
#define __CLI __asm__ __volatile__ ("cli")
#define __SES __asm__ __volatile__ ("ses")
#define __CLS __asm__ __volatile__ ("cls")
#define __SEV __asm__ __volatile__ ("sev")
#define __CLV __asm__ __volatile__ ("clv")
#define __SET __asm__ __volatile__ ("set")
#define __CLT __asm__ __volatile__ ("clt")
#define __SEH __asm__ __volatile__ ("seh")
#define __CLH __asm__ __volatile__ ("clh")

inline void __MOV(register uint8_t Rd, register uint8_t Rr)
{
  __asm__ __volatile__("mov %0, %1" : "=r" (Rd) : "r" (Rr));
}

inline void __MOVW(register uint16_t Rd, register uint16_t Rr)
{
  __asm__ __volatile__("movw %0, %1" : "=w" (Rd) : "w" (Rr));
}

inline void __LDI(register uint8_t Rd, uint8_t K)
{
  __asm__ __volatile__("ldi %0, %1" : "=d" (Rd) : "M" (K));
}

inline void __LD(register uint8_t Rd, register uint16_t ptr)
{
  __asm__ __volatile__("ld %0, %a1" : "=r" (Rd) : "e" (ptr));
}

inline void __LDP(register uint8_t Rd, register uint16_t ptr)
{
  __asm__ __volatile__("ld %0, %a1+" : "=r" (Rd) : "e" (ptr));
}

inline void __LDM(register uint8_t Rd, register uint16_t ptr)
{
  __asm__ __volatile__("ld %0, -%a1" : "=r" (Rd) : "e" (ptr));
}

inline void __LDD(register uint8_t Rd, register  uint16_t ptr, char q)
{
  __asm__ __volatile__("ldd %0, %a1+%2" : "=r" (Rd) : "b" (ptr), "I" (q));
}

#define __LDS(Rr, K) __asm__ __volatile__ ("lds %0, " #K : "=r" (Rr))

inline void __ST(register uint16_t ptr, register uint8_t Rr)
{
  __asm__ __volatile__("st %a0, %1" : "=e" (ptr) : "r" (Rr));
}

inline void __STP(register uint16_t ptr, register uint8_t Rr)
{
  __asm__ __volatile__("st %a0+, %1" : "=e" (ptr) : "r" (Rr));
}

inline void __STM(register uint16_t ptr, register uint8_t Rr)
{
  __asm__ __volatile__("st -%a0, %1" : "=e" (ptr) : "r" (Rr));
}

inline void __STD(register uint16_t ptr, char q, register uint8_t Rr)
{
  __asm__ __volatile__("std %a0+%2, %1" : "=b" (ptr) : "r" (Rr), "I" (q));
}

#define __STS(K, Rr) __asm__ __volatile__ ("sts " #K ", %0" : : "r" (Rr))

inline void _LPM()
{
  __asm__ __volatile__("lpm");
}

inline void __LPMZ(register uint8_t Rd)
{
  __asm__ __volatile__("lpm %0, Z" : "=r" (Rd));
}

inline void __LPMZP(register uint8_t Rd)
{
  __asm__ __volatile__("lpm %0, Z+" : "=r" (Rd));
}

inline void __SPM()
{
  __asm__ __volatile__("spm");
}

inline void __IN(register uint8_t Rd, char P)
{
  __asm__ __volatile__("in %0, %1" : "=r" (Rd) : "I" (P));
}

inline void __OUT(char P, register uint8_t Rd)
{
  __asm__ __volatile__("out %1, %0" : "=r" (Rd) : "I" (P));
}

inline void __PUSH(register uint8_t Rd)
{
  __asm__ __volatile__("push %0" : : "r" (Rd));
}

inline void __POP(register uint8_t Rd)
{
  __asm__ __volatile__("pop %0" : "=r" (Rd));
}

#define __NOP __asm__ __volatile__ ("nop")
#define __SLEEP __asm__ __volatile__ ("sleep")
#define __WDR __asm__ __volatile__ ("wdr")
#define __BREAK __asm__ __volatile__ ("break")

#endif /* AVRASM_H_ */
