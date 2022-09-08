#include <string>
#include "Common.h"

int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	
	printf("====================WSADATA INFORMATION====================\n");
	printf("Major version number(WSADATA::wVersion) - %d\nMinor Version number(WSADATA::wVersion) - %d\n\n", LOBYTE(wsa.wVersion), HIBYTE(wsa.wVersion));
	printf("Major version number(WSADATA::wHighVersion) - %d\nMinor Version number(WSADATA::wHighVersion) - %d\n\n", LOBYTE(wsa.wHighVersion), HIBYTE(wsa.wHighVersion));
	printf("Description of Window Socket(WSADATA::szDescription) - %s\n\n", &wsa.szDescription);
	printf("SystemStatus(WSADATA::szSystemStatus) - %s\n\n", &wsa.szSystemStatus);\
	
	printf("[알림] 윈속 초기화 성공\n");

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("sock()");
	printf("[알림] 소켓 생성 성공\n");

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}