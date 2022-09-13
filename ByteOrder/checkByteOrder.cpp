#include "Common.h"
/*
bool IsLittleEndian();
리턴값: 리틀 엔디언이면 true, 그렇지 않으면 false를 리턴한다.

bool IsBigEndian();
리턴값: 빅 엔디언이면 true, 그렇지 않으면 false를 리턴한다.
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