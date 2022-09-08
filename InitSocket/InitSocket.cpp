#include <string>
#include "Common.h"

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	printf("====================WSADATA INFORMATION====================\n");
	printf("Major version number(WSADATA::wVersion) - %d\nMinor Version number(WSADATA::wVersion) - %d\n\n", LOBYTE(wsa.wVersion), HIBYTE(wsa.wVersion));
	printf("Major version number(WSADATA::wHighVersion) - %d\nMinor Version number(WSADATA::wHighVersion) - %d\n\n", LOBYTE(wsa.wHighVersion), HIBYTE(wsa.wHighVersion));
	printf("Description of Window Socket(WSADATA::szDescription) - %s\n\n", &wsa.szDescription);
	printf("SystemStatus(WSADATA::szSystemStatus) - %s\n\n", &wsa.szSystemStatus);\
	
	printf("[�˸�] ���� �ʱ�ȭ ����\n");

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("sock()");
	printf("[�˸�] ���� ���� ����\n");

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}