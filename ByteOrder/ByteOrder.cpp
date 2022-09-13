#include "Common.h"

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	u_short s1 = 0x1234;
	u_long l1 = 0x12345678;
	u_short s2;
	u_long l2;

	printf("==========Host to Network==========\n");
	printf("%#x => %#x\n", s1, s2 = htons(s1));
	printf("%#x => %#x\n", l1, l2 = htonl(l1));
	printf("==========Network to Host==========\n");
	printf("%#x => %#x\n", s2, s1 = ntohs(s2));
	printf("%#x => %#x\n", l2, l1 = ntohl(l2));
	WSACleanup();
}