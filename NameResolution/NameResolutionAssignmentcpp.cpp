#include "Common.h"
#include <windows.h>

void printAddr_list(hostent* target)
{
	int i = 0;
	while (target->h_addr_list[i])
	{
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &target->h_addr_list[i++], str, sizeof(str));
		printf("%s\n", str);
	}
}

void printH_aliases(hostent* target)
{
	int i = 0;
	while (target->h_aliases[i])
		printf("%s\n", target->h_aliases[i++]);
}

int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	while (1)
	{
		char domainName[256];
		printf("도메인 이름을 입력해주세요: ");
		scanf_s("%s", domainName);

		hostent* ptr = gethostbyname(domainName);

		printAddr_list(ptr);
		printH_aliases(ptr);

		system("nslookup www.google.co.kr");

		
	}
	// 윈속 종료
	WSACleanup();
	return 0;
}