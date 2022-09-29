#include "Common.h"
#include <windows.h>

void printAddr_list(const hostent* target)
{
	struct in_addr addr;
	char str[INET_ADDRSTRLEN];

	for (int i = 0; target->h_addr_list[i] != NULL; ++i)
	{
		memcpy(&addr, target->h_addr_list[i], target->h_length);
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("IP �ּ�[%d] = %s\n", i, str);
	}
	printf("\n");
}

void printH_aliases(const hostent* target)
{
	for(int i=0; target->h_aliases[i] != NULL; ++i)
		printf("Aliases[%d] = %s\n", i, target->h_aliases[i]);
	printf("\n");
}

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	char domainName[256] = { '\0' };
	for (int i = 1; i < argc; ++i)
	{
		struct hostent* ptr = gethostbyname(argv[i]);

		if (ptr == NULL) {
			err_display("gethostname()");
			return 1;
		}

		printf("\n============ myfunc =============\n");
		printH_aliases(ptr);
		printAddr_list(ptr);
		printf("===================================\n\n");

		// ���� ����
		WSACleanup();
		return 0;
	}
}