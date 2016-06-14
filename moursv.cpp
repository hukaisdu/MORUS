#include <iostream>

using namespace std;

typedef unsigned long unit64;
int b[5] = {5,31,7,22,13};
int w[5] = {32,64,96,64,32};
void Rotl_128_32(unit64 s[2], int b){
long tem = (-1l + (1l<<b));
tem ^= tem << 32; 
s[0] = (s[0] << b & ~tem) ^ (s[0] >> (32-b) & tem);
s[1] = (s[1] << b & ~tem) ^ (s[1] >> (32-b) & tem);
}

void StateUpdate(unit64 s[10]){
unit64 temp;
// round1
s[0] ^= s[2] & s[4] ^ s[6];
s[1] ^= s[3] & s[5] ^ s[7]; 
Rotl_128_32(s, 5);
// s3 << 32
temp = s[6] << 32 ^ s[7] >> 32;
s[7] = s[7] << 32 ^ s[6] >> 32;
s[6] = temp;
// round2
s[2] ^= s[4] & s[6] ^ s[8];
s[3] ^= s[5] & s[7] ^ s[9]; 
Rotl_128_32(s+2, 31);
// s4 << 64
temp = s[8];
s[8]= s[9];
s[9] = temp;

// round3
s[4] ^= s[6] & s[8] ^ s[0];
s[5] ^= s[7] & s[9] ^ s[1]; 
Rotl_128_32(s+4, 7);
// s0 << 96
temp  = s[0] >> 32 ^ s[1] << 32;
s[1] = s[1] >> 32 ^ s[0] << 32;
s[0] = temp;
// round4
s[6] ^= s[8] & s[0] ^ s[2];
s[7] ^= s[9] & s[1] ^ s[3]; 
Rotl_128_32(s+6, 22);
// s1 << 64
temp = s[2];
s[2] = s[3];
s[3] = temp;

// round5
s[8] ^= s[0] & s[2] ^ s[4];
s[9] ^= s[1] & s[3] ^ s[5]; 
Rotl_128_32(s+8, 13);
// s2 << 32;
temp = s[4] << 32 ^ s[5] >> 32;
s[5] = s[5] << 32 ^ s[4] >> 32;
s[4] = temp;
}

int mainn(){

unit64 cipher[2]={0};
unit64 result[2]={0};
long number = 1ul << 6;
for(int i = 0; i < 1; i++){
unit64 s[10] = {0x0000000000000000,0xffffffffffffffff,0x0000000000000000,0xffffffffffffffff,
                0xffffffffffffffff,0xffffffffffffffff,0x000101020305080d,0x1522375990e97962,
                0xdb3d18556dc22ff1,0x2011314273b528dd};
s[0] ^= i;
StateUpdate(s);


cout << hex <<s[0] <<endl;
cout << hex <<s[1] <<endl;
cout << hex <<s[2] <<endl;
cout << hex <<s[3] <<endl;
cout << hex <<s[4] <<endl;
cout << hex <<s[5] <<endl;
cout << hex <<s[6] <<endl;
cout << hex <<s[7] <<endl;
cout << hex <<s[8] <<endl;
cout << hex <<s[9] <<endl;
cipher[0] = s[0] ^ (s[2] >> 32 ^ s[3] << 32) ^ (s[4] & s[6]);
cipher[1] = s[1] ^ (s[3] >> 32 ^ s[2] << 32) ^ (s[5] & s[7]);
result[0] ^= cipher[0];
result[1] ^= cipher[1];
}

//cout << hex <<result[0] <<endl;
//cout << hex <<result[1] <<endl;
return 0;
}

int ain(){
unit64 a = -1;
cout << sizeof(a) << endl;
cout << hex << (a >> 3) << endl;
unit64 s[2] = {0x2222222211111111, 0x3333333322222222};
Rotl_128_32(s,5);
cout << hex <<s[0] <<endl;
cout << hex <<s[1] <<endl;
return 0;
}
