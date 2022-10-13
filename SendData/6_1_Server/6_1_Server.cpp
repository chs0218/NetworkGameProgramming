#include "..\..\Common.h"
#include <fstream>

#define SERVERPORT 9000
#define BUFSIZE    2048

#pragma pack(1)
struct DataSet {
	int len = 0;	// ������ ����
	char name[50] = "\0";	// ������ ���� �̸�
	char buf[BUFSIZE] = "\0";	// ������ ����
};
#pragma pack()

#pragma pack(1)
struct Params {
	SOCKET client_sock;
	COORD curpos;
};
#pragma pack()

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
CRITICAL_SECTION cs;

// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	Params params;
	params.client_sock = ((Params*)arg)->client_sock;
	params.curpos = ((Params*)arg)->curpos;
	params.curpos.Y += 1;
	int retval;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	DataSet data;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(params.client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// Ŭ���̾�Ʈ�� ������ ���
	while (1) {
		retval = recv(params.client_sock, (char*)&data.len, sizeof(int), MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		retval = recv(params.client_sock, data.name, 50, MSG_WAITALL);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		std::ofstream out{ data.name, std::ios::binary };

		while (1)
		{
			retval = recv(params.client_sock, data.buf, BUFSIZE, MSG_WAITALL);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			if (retval == 0)
				break;

			out.write(data.buf, retval);
			EnterCriticalSection(&cs);
			SetConsoleCursorPosition(handle, params.curpos);
			printf("���۷�: %d%%\n", static_cast<int>(((float)(out.tellp()) / (float)data.len) * (float)100.0f));
			LeaveCriticalSection(&cs);
		}
	}

	// ���� �ݱ�
	closesocket(params.client_sock);
	params.curpos.Y += 1;

	EnterCriticalSection(&cs);
	SetConsoleCursorPosition(handle, params.curpos);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", addr, ntohs(clientaddr.sin_port));
	LeaveCriticalSection(&cs);
	return 0;

}

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	// ���� ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;
	COORD globalCurpos{ 0,-3 };

	InitializeCriticalSection(&cs);

	while (1) {
		// accept()
		Params clientParams;
		addrlen = sizeof(clientaddr);
		clientParams.client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (clientParams.client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}


		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		globalCurpos.Y += 4;

		EnterCriticalSection(&cs);
		SetConsoleCursorPosition(handle, globalCurpos);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d", addr, ntohs(clientaddr.sin_port));
		LeaveCriticalSection(&cs);

		clientParams.curpos = globalCurpos;

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)&clientParams, 0, NULL);
		if (hThread == NULL) { closesocket(clientParams.client_sock); }
		else { CloseHandle(hThread); }
	}

	DeleteCriticalSection(&cs);
	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}