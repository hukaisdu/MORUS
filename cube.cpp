#include<stdio.h>
#include<inttypes.h>
#include<random>
#include<time.h>

using namespace std;
#define OFFSET 4

int b[5] = { 5,31,7,22,13 };
int w[5] = { 32,64,96,64,32 };

void StateUpdate(uint32_t S[20],
	             uint32_t P[4]);
uint32_t Rotl_32(uint32_t integer, int bits);
void Rotl_128(uint32_t* i0,
	          uint32_t* i1,
	          uint32_t* i2,
	          uint32_t* i3,
	          int bits);

void StateUpdate(uint32_t S[20],//array of 20 elements
			     uint32_t P[4])
{
	int offset = 4;

	for (int round = 0; round<5; round++)
	{
		for (int i = 0; i<4; i++)
		{
			S[round*offset + i]
				= S[round*offset + i] ^ (S[((round + 1) % 5)*offset + i] & S[((round + 2) % 5)*offset + i]) ^ S[((round + 3) % 5)*offset + i] ^(round>0 ? P[i] : 0);
			S[round*offset+i]=Rotl_32(S[round*offset + i], b[round]);
		}
		Rotl_128(&S[((round + 3) % 5)*offset + 0],
			     &S[((round + 3) % 5)*offset + 1],
			     &S[((round + 3) % 5)*offset + 2],
		 	     &S[((round + 3) % 5)*offset + 3],
			     w[round]);
	}
}

void Rotl_128(uint32_t *i0,
	uint32_t *i1,
	uint32_t *i2,
	uint32_t *i3,
	int bits)
{
	uint32_t temp[3] = { 0,0,0 };
	if (bits == 32)
	{
		temp[0] = *i0;
		*i0 = *i1;
		*i1 = *i2;
		*i2 = *i3;
		*i3 = temp[0];
	}
	else if (bits == 64)
	{
		temp[0] = *i0;
		temp[1] = *i1;
		*i0 = *i2;
		*i1 = *i3;
		*i2 = temp[0];
		*i3 = temp[1];
	}
	else if (bits == 96)
	{
		temp[0] = *i0;
		temp[1] = *i1;
		temp[2] = *i2;
		*i0 = *i3;
		*i1 = temp[0];
		*i2 = temp[1];
		*i3 = temp[2];
	}
}
uint32_t Rotl_32(uint32_t i, int bits)
{
	return (i << bits ^ i >> (32 - bits));
}
/*
int main()
{
	uint32_t a32_stt[20]=
	{
		0,0,
		0,0,
		0xa0b1c2d3,0xe4f5a6b7,
		0,0,
		0xffffffff,0xffffffff,
		0xffffffff,0xffffffff,
		0x00010102,0x0305080d,
		0x15223759,0x90e97962,
		0xdb3d1855,0x6dc22ff1,
		0x20113142,0x73b528dd
	};
	uint32_t p[4] = { 0 };
	printf("\n==========================================Update the state=====================================\n");
	int i = 0;
	while (i++<3)
		StateUpdate(a32_stt, p);
	for (int i = 0; i < 20; i++)
		printf("%x\n", a32_stt[i]);
	return 0;
}
*/

int main()
{
/*	
	random_device rd;
	mt19937_64 mt(rd());
	//initiate the S[20]
	uint32_t S[20] = 
	{
		0,0,0,0,
		0,0,0,0,
		0xffffffff,0xffffffff,0xffffffff,0xffffffff,
		0x00010102,0x0305080d,0x15223759,0x90e97962,
		0xdb3d1855,0x6dc22ff1,0x20113142,0x73b528dd
	};
	uint32_t p[4] = { 0 };
	uint32_t Key[4] = { 0,0,0,0 };
	uint32_t BITNUMBERS=1;
	uint64_t counter=0;
*/
	int ROUND;
	for(ROUND=1;ROUND<=4;ROUND++)
	{
	time_t roundstart,roundend;
	random_device rd;
	mt19937_64 mt(rd());
	//initiate the S[20]
	uint32_t S[20] = 
	{
		0,0,0,0,
		0,0,0,0,
		0xffffffff,0xffffffff,0xffffffff,0xffffffff,
		0x00010102,0x0305080d,0x15223759,0x90e97962,
		0xdb3d1855,0x6dc22ff1,0x20113142,0x73b528dd
	};
	uint32_t p[4] = { 0 };
	uint32_t Key[4] = { 0,0,0,0 };
	uint64_t BITNUMBERS=1;
	uint64_t counter=0;
	roundstart=time(NULL);
	while(1)
	{	
	if(counter++>0xffff)
	{
		printf("\n=================================\n");
		printf("State Update:%d\n",ROUND);
		printf("the test keys : %llu \n",counter);
		printf("the possible degree is %#x\n ",BITNUMBERS);
		roundend=time(NULL);
		uint64_t time=roundend-roundstart;
		printf("The used time : %d h %d min %d s  \n ",time/3600,(time-time/3600*3600)/60,time%3600);
		break;
	}
//	printf("\n==================Test another key==========================\n");
	uint64_t random=mt()&0xffffffffffffffff;
	Key[0]=random>>32&0xffffffff;
	Key[1]=random&0xffffffff;	
	for(int i=0;i<4;i++)
		S[1*OFFSET+i]=Key[i];
//	printf("The key %x %x %x %x\n", Key[0], Key[1], Key[2], Key[3]);

	
	while(1)
	{
	uint32_t result[4] = { 0 };
	uint32_t xorSum[4] = { 0 };

	uint32_t primitive[20];
	for (int i = 0; i < 20; i++)
		primitive[i] = S[i];
	
	for (uint32_t bits = 0; bits <= BITNUMBERS; bits++)
	{
		for (int i = 0; i < 20; i++)
			S[i] = primitive[i];
		
		int i = 0;
		S[0]=bits;
		
		while (i++< ROUND)
			StateUpdate(S, p);

		Rotl_128(&S[1 * OFFSET + 0], &S[1 * OFFSET + 1], &S[1 * OFFSET + 2], &S[1 * OFFSET + 3], 96);
		for (int i = 0; i < 4; i++)
			result[i] = S[0 * OFFSET + i] ^ S[1 * OFFSET + i] ^ (S[2 * OFFSET + i] & S[3 * OFFSET + i]);
		
		for (int i = 0; i < 4; i++)
			xorSum[i] ^= result[i];	
		
	}
//	printf("the BITNUMBERS: %#x \n",BITNUMBERS);
//	printf("the sum of xor: %x,%x,%x,%x\n", xorSum[0], xorSum[1], xorSum[2], xorSum[3]);
	if(xorSum[0]==0 && xorSum[1]==0 && xorSum[2]==0 && xorSum[3]==0)
		break;
	else{
		BITNUMBERS=(BITNUMBERS<<1)+1;
		if(BITNUMBERS>0xffffffff)
		{
			printf("the max bits we can test");
			break;
		}
	}
	}
	}
	}	
	return 0;
	
}


