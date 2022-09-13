#include "Common.h"
/*
bool IsLittleEndian();
리턴값: 리틀 엔디언이면 true, 그렇지 않으면 false를 리턴한다.

bool IsBigEndian();
리턴값: 빅 엔디언이면 true, 그렇지 않으면 false를 리턴한다.
*/

bool IsLittleEndian()
{
    u_short us = 0x0001;
    char* p = (char*)&us;
    return p[0];
}

bool IsBigEndian()
{
    u_short us = 0x0100;
    char* p = (char*)&us;
    return p[0];
}

int main() {
    if (IsLittleEndian())
        printf("I'm Little Endian\n");
    else
        printf("I'm not Little Endian\n");

    if (IsBigEndian())
        printf("I'm Big Endian\n");
    else
        printf("I'm not Big Endian\n");
}