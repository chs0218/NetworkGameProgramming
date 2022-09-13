#include "Common.h"
/*
bool IsLittleEndian();
���ϰ�: ��Ʋ ������̸� true, �׷��� ������ false�� �����Ѵ�.

bool IsBigEndian();
���ϰ�: �� ������̸� true, �׷��� ������ false�� �����Ѵ�.
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