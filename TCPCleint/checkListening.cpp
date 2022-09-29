#include "Common.h"

char* SERVERIP = (char*)"223.130.195.95";
#define PORT 6463
#define BUFSIZE    512

int main(int argc, char* argv[])
{
	
	int retval;

	// 명령행 인수가 없으면 종료
	if (argc != 3) return 1;

	// 명령행 인수로 두개의 포트 번호를 받음
	int MINPORT = atoi(argv[1]);
	int MAXPORT = atoi(argv[2]);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	for (int i = MINPORT; i <= MAXPORT; ++i)
	{
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(i);
		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == 0) printf("IP주소: [%s] 포트번호: [%d] Listening\n", SERVERIP, i);
		else printf("IP주소: [%s] 포트번호: [%d] Not Listening\n", SERVERIP, i);
	}
	
	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}