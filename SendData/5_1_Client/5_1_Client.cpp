#include "..\..\Common.h"
#include <iostream>
#include <fstream>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000

#pragma pack()
struct DataSet {
	int len;	// 데이터 길이
	char name[50];	// 데이터 파일 이름
	char* buf;	// 데이터 버퍼
};

int main(int argc, char* argv[])
{
	int retval = 0;

	// 명령행 인수가 있으면 IP 주소로 사용
	if (argc > 1) SERVERIP = argv[1];

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
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

	// 데이터 통신에 사용할 변수
	DataSet data;
	std::ifstream in{ "tmp.txt", std::ios::binary };
	
	in.seekg(0, std::ios::end);
	data.len = in.tellg();
	in.seekg(0, std::ios::beg);

	data.buf = new char[data.len];
	in.read(data.buf, data.len);

	std::cout << data.len << std::endl;
	strncpy(data.name, "test.txt", strlen("test.txt"));

	// 서버와 데이터 통신
	
	// 데이터 보내기(고정 길이)
	retval = send(sock, (char*)&data.len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	retval = send(sock, (char*)&data.name, 50, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// 데이터 보내기(가변 길이)
	retval = send(sock, data.buf, data.len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	
	delete[] data.buf;
	printf("[TCP 클라이언트] %d+%d바이트를 "
		"보냈습니다.\n", (int)sizeof(int), retval);

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}