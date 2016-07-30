///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////M系列乱数作成アルゴリズム
//#include"lcg.h"
#include<iostream>
#include"mseq.h"

unsigned long M32(unsigned long u){return 0xffffffff & u;}

void rand521()
{
	int i;
	for(i = 0;i < 32;i++)   x[i] ^= x[i+489];
	for(i = 32;i < 521;i++) x[i] ^= x[i-32];
}

void initrand(unsigned long seed)
{
	int i,j;
	unsigned long u;
	using namespace std;

	next = seed;

	//最初の16個を線形合同法により作成する.
	u = 0;
	for(i = 0;i <= 16;i++)
	{
		for(j = 0;j < 32;j++)
		{
			seed = seed * 1566083941UL + 1;
			u = (u >> 1) | (seed & (1UL << 31));
//			seed = (seed * 950706376UL) % ((1UL << 31)-1);
		}
		x[i] = u;
//		x[i] = seed;
	}
	//16個目を次のように更新する.
	x[16] = ((x[0] >> 9) ^ x[15]) | M32(x[16] << 23);

	//17個目から521個目まで以下のように更新する.
	for(i = 17;i < 521;i++)
	{
		x[i] = M32(((x[i-17] << 23) | (x[i-16] >> 9)) ^ x[i-1]);
	}
	reg_loc = 520;
	rand521(); rand521(); rand521();
}

unsigned long mrand()
{
	reg_loc++;
	//レジスタを一番最後まで参照したら,レジスタの値をずらす.
	if(reg_loc >= 521) 
	{
		rand521();
		reg_loc = 0;
	}
	return x[reg_loc];
}

void initrnd()
{
	int i;

	for(i = 0;i < poolsize;i++) pool[i] = (double)mrand()*1.0/((double)(RAND_MAX)+1.0);
}

double rnd()
{
	static int i = poolsize-1;
	double r;

	i = (int)(poolsize*pool[i]);
	r = pool[i];
	pool[i] = (double)mrand()*1.0/((double)(RAND_MAX)+1.0);
	return r;
}