#include "..\..\Common.h"
#include <Windows.h>
#include <string>
#include <fstream>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512

#pragma pack(1)
struct DataSet {
	int len = 0;	// ������ ����
	char name[50] = "\0";	// ������ ���� �̸�
	char buf[BUFSIZE] = "\0";	// ������ ����
};
#pragma pack()

int main(int argc, char* argv[])
{
	int retval = 0;

	if (argc == 1)
		return 1;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	DataSet data;
	for (int i = 1; i < argc; ++i)
	{
		std::ifstream in{ argv[i], std::ios::binary };
		in.seekg(0, std::ios::end);
		data.len = in.tellg();
		in.seekg(0, std::ios::beg);

		char* pFileName = strrchr(argv[i], '\\');
		if (pFileName != NULL)
			strcpy(data.name, pFileName + 1);
		else
			strcpy(data.name, argv[i]);

		retval = send(sock, (char*)&data.len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		retval = send(sock, (char*)&data.name, 50, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		while (in)
		{
			in.read(data.buf, BUFSIZE);
			retval = send(sock, data.buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
			"���½��ϴ�.\n", (int)sizeof(int), data.len);
	}

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}