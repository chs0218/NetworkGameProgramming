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
    us = htons(us);
    return HIBYTE(us);
}

bool IsBigEndian()
{
    u_short us = 0x0001;
    us = htons(us);
    return LOBYTE(us);
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