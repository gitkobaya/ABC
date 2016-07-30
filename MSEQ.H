#ifndef _MSEQ_H_
#define _MSEQ_H_

#undef RAND_MAX

static unsigned long next = 1;
static unsigned long reg_loc = 0;
static unsigned long x[521];  

const unsigned long RAND_MAX = (1UL << 32)-1;

extern void initrand(unsigned long seed);
extern unsigned long  mrand();
void rand521();

const int poolsize = 100;
static double pool[poolsize];

extern void initrnd();
extern double rnd();

extern unsigned long M32(unsigned long u);


#endif /*_MSEQ_H_*/