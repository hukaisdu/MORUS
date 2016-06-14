#include <iostream>
#include <cstdlib>
#include <random>
#include <pthread.h>

using namespace std;
typedef unsigned long unit64;
void StateUpdate(unit64 s[10]);

int main()
{
    int record[256][128]={0};
    int b[128][256] = {0};
    unit64 cipher[2]={0}; //restores the output result
    unit64 init[4]={0}; //restores the random value
    
    //generate random value
    random_device rd;
    mt19937_64 mt(rd());
    for (int t = 0; t < 4; t++)
    for (int j = 0; j < 64; j++)
{ if (t == 0 && j >39)
     continue;
    for (int mm = 0; mm < 256; mm++)
{
    unit64 result[2]={0}; //restores the output result
        for (int i = 0; i < 4; i++)
    	     init[i] = mt();
    int high = 1<<24;
    for (int i = 0; i < high; i++)
    {
    
        unit64 state[10] = {0x0000000000000000,0xffffffffffffffff,0x0000000000000000,0xffffffffffffffff,
                            0xffffffffffffffff,0xffffffffffffffff,0x000101020305080d,0x1522375990e97962,
                            0xdb3d18556dc22ff1,0x2011314273b528dd};
        //assigns the value of state
        for (int n = 0; n < 4; n++)
        	state[n] = init[n];
        state[0] ^= i;

        StateUpdate(state);
        StateUpdate(state);
        StateUpdate(state);
        StateUpdate(state);
        StateUpdate(state);
    
        cipher[0] = state[0] ^ (state[2] >> 32 ^ state[3] << 32) ^ (state[4] & state[6]);
        cipher[1] = state[1] ^ (state[3] >> 32 ^ state[2] << 32) ^ (state[5] & state[7]);
        result[0] ^= cipher[0];
        result[1] ^= cipher[1];

        unit64 state1[10] = {0x0000000000000000,0xffffffffffffffff,0x0000000000000000,0xffffffffffffffff,
                            0xffffffffffffffff,0xffffffffffffffff,0x000101020305080d,0x1522375990e97962,
                            0xdb3d18556dc22ff1,0x2011314273b528dd};
        //assigns the value of state
        for (int n = 0; n < 4; n++)
        	state1[n] = init[n];
        state1[0] ^= i;
        state1[t] ^= 1ul << (63 - j);

        StateUpdate(state1);
        StateUpdate(state1);
        StateUpdate(state1);
        StateUpdate(state1);
        StateUpdate(state1);
    
        cipher[0] = state1[0] ^ (state1[2] >> 32 ^ state1[3] << 32) ^ (state1[4] & state1[6]);
        cipher[1] = state1[1] ^ (state1[3] >> 32 ^ state1[2] << 32) ^ (state1[5] & state1[7]);
        result[0] ^= cipher[0];
        result[1] ^= cipher[1];
     }
        for (int k = 0; k < 2; k++)
           for (int m = 0; m < 64; m++)
                if(result[k] >> (63 -m) & 0x1)
                   record[j+64*t][m+64*k] = 1;
}
}
    
    for (int m = 40; m <64; m++)
        for (int i = 0; i < 128; i++)
             record[m][i] = 1;
    for (int m = 0; m < 128; m++)
        for (int n = 0; n < 256; n++)
            if (record[n][m] == 0)
               b[m][n]=1;
    // if result[i] != 0, print 1
    for (int m = 0; m < 128; m++){
        std::cout << "m= " << m << std::endl;
        for (int n = 0; n < 256; n++)
        if (b[m][n] == 1)
             std::cout << n << " ";
        std::cout << std::endl;
        }
    return 0;
}


