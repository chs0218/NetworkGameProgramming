#include "Common.h"

char* SERVERIP = (char*)"223.130.195.95";
#define PORT 6463
#define BUFSIZE    512

int main(int argc, char* argv[])
{
	
	int retval;

	// ����� �μ��� ������ ����
	if (argc != 3) return 1;

	// ����� �μ��� �ΰ��� ��Ʈ ��ȣ�� ����
	int MINPORT = atoi(argv[1]);
	int MAXPORT = atoi(argv[2]);

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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
		if (retval == 0) printf("IP�ּ�: [%s] ��Ʈ��ȣ: [%d] Listening\n", SERVERIP, i);
		else printf("IP�ּ�: [%s] ��Ʈ��ȣ: [%d] Not Listening\n", SERVERIP, i);
	}
	
	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}