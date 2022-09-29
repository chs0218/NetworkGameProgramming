#include "..\..\Common.h"
#include <iostream>
#include <fstream>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000

#pragma pack()
struct DataSet {
	int len;	// ������ ����
	char name[50];	// ������ ���� �̸�
	char* buf;	// ������ ����
};

int main(int argc, char* argv[])
{
	int retval = 0;

	// ����� �μ��� ������ IP �ּҷ� ���
	if (argc > 1) SERVERIP = argv[1];

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
	std::ifstream in{ "tmp.txt", std::ios::binary };
	
	in.seekg(0, std::ios::end);
	data.len = in.tellg();
	in.seekg(0, std::ios::beg);

	data.buf = new char[data.len];
	in.read(data.buf, data.len);

	std::cout << data.len << std::endl;
	strncpy(data.name, "test.txt", strlen("test.txt"));

	// ������ ������ ���
	
	// ������ ������(���� ����)
	retval = send(sock, (char*)&data.len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	retval = send(sock, (char*)&data.name, 50, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����)
	retval = send(sock, data.buf, data.len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	
	delete[] data.buf;
	printf("[TCP Ŭ���̾�Ʈ] %d+%d����Ʈ�� "
		"���½��ϴ�.\n", (int)sizeof(int), retval);

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}