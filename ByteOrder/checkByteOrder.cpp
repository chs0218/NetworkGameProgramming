#include "Common.h"
/*
bool IsLittleEndian();
���ϰ�: ��Ʋ ������̸� true, �׷��� ������ false�� �����Ѵ�.

bool IsBigEndian();
���ϰ�: �� ������̸� true, �׷��� ������ false�� �����Ѵ�.
*/

bool IsLittleEndian()
{
	u_short s1 = 0x1234;
	u_short s2 = htons(s1);

	if (s1 != s2)
		return true;
	return false;
}

bool IsBigEndian()
{
	u_short s1 = 0x1234;
	u_short s2 = htons(s1);

	if (s1 = s2)
		return true;
	return false;
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	if (IsLittleEndian())
		printf("I'm Little Endian\n");
	if (IsBigEndian())
		printf("I'm Big Endian\n");
	WSACleanup();
}