#include <iostream>
#include <cstdlib>
#include <random>
#include <pthread.h>

using namespace std;
typedef unsigned long unit64;
void StateUpdate(unit64 s[10]);

void *balance_6r_6sboxes(void *args)
{
    unit64 result[2]={0}; //restores the output result
    unit64 cipher[2]={0}; //restores the output result
    unit64 init[4]={0}; //restores the random value

    //generate random value
    random_device rd;
    mt19937_64 mt(rd());
    for (int i = 0; i < 4; i++)
    	init[i] = mt();
    
    int high = 1<<8;
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
    
        cipher[0] = state[0] ^ (state[2] >> 32 ^ state[3] << 32) ^ (state[4] & state[6]);
        cipher[1] = state[1] ^ (state[3] >> 32 ^ state[2] << 32) ^ (state[5] & state[7]);
        result[0] ^= cipher[0];
        result[1] ^= cipher[1];
    }
    
    // if result[i] != 0, print 1
    for (int m = 0; m < 2; m++)
        if (result[m] != 0){
             std::cout << "1" << std::endl;
             break;
        }
}


int nthread_run(int nthreads)  //run n threads
{
    pthread_t tid[nthreads];
    for (int i = 0; i < nthreads; i++){
        //pthread_create(&tid[i], NULL, balance_5r_3sboxes, NULL);
        pthread_create(&tid[i], NULL, balance_6r_6sboxes, NULL);
    }

    for (int i = 0; i < nthreads; i++)
        pthread_join(tid[i], NULL);

    return 0;
}

int main()
{
	nthread_run(660);
	return 0;
}
