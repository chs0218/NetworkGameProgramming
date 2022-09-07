/*** 여기서부터 이 책의 모든 예제에서 공통으로 포함하여 사용하는 코드이다. ***/

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

/*** 여기까지가 이 책의 모든 예제에서 공통으로 포함하여 사용하는 코드이다. ***/
/*** 2장 이후의 예제들은 Common.h를 포함하는 방식으로 이 코드를 사용한다.  ***/

#define SERVERPORT 9000
#define BUFSIZE    512

// TCP 서버(IPv4)
DWORD WINAPI TCPServer4(LPVOID arg)
{
	int retval;

	// 소켓 생성
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 받은 데이터 출력
			buf[retval] = '\0';
			printf("%s", buf);
		}

		// 소켓 닫기
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// 소켓 닫기
	closesocket(listen_sock);
	return 0;
}

// TCP 서버(IPv6)
DWORD WINAPI TCPServer6(LPVOID arg)
{
	int retval;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// 듀얼 스택을 끈다. [Windows는 꺼져 있음(기본값). UNIX/Linux는 OS마다 다름]
	int no = 1;
	setsockopt(listen_sock, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&no, sizeof(no));

	// bind()
	struct sockaddr_in6 serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_addr = in6addr_any;
	serveraddr.sin6_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in6 clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char ipaddr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, ipaddr, sizeof(ipaddr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			ipaddr, ntohs(clientaddr.sin6_port));

		// 클라이언트와 데이터 통신
		while (1) {
			// 데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// 받은 데이터 출력
			buf[retval] = '\0';
			printf("%s", buf);
		}

		// 소켓 닫기
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			ipaddr, ntohs(clientaddr.sin6_port));
	}

	// 소켓 닫기
	closesocket(listen_sock);
	return 0;
}

int main(int argc, char* argv[])
{
	// WSAStartup은 윈도우 소켓을 초기화하는 함수, 반환이 0이면 초기화에 성공한 것이며 그 이외의 값은 초기화에 실패한 것
	// 첫번째 인자는 윈도우 소켓의 버전, 두번째 인자는 윈도우 소켓이 초기화된 상태를 저장하는 변수
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 멀티스레드를 이용하여 두 개의 서버를 동시에 구동한다.
	// CreateThread는 쓰레드를 생성하는 함수, 인자는 (보안속성, 쓰레드 생성시할당되는 스택 사이즈, 
	//				쓰레드가 실행하는 함수 주소, ???, 쓰레드 함수에 전달할 인자, 생성되는 쓰레드의 ID를 얻기위한 주소)
	// 반환되는 인자는 커널 오브젝트의 핸들이며 후에 생성한 쓰레드에 대해 CloseHandle을 통해 핸들을 닫아야한다.

	// 생성된 커널 오브젝트는 Non-Signaled 상태, 프로세스가 종료되면 Signaled 상태가 되는데 
	// WaitForMultipleObjects 함수는 첫번째 인자만큼의 쓰레드가 전부 Signaled 상태가 되는 순간까지 기다리는 함수
	// 인자는 차례대로 (배열에 저장되어있는 핸들의 개수, 핸들을 저장하고 있는 배열의 주소 정보, 
	//		관찰대상이 전부 Signaled 상태가 되기를 기다리는지 혹은 하나라도 Signaled가 되면 반환하는지 결정, 커널 오브젝트가 Signaled가 될 때까지 기다릴 수 있는 최대시간)

	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, TCPServer4, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, TCPServer6, NULL, 0, NULL);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	// WSACleanup은 윈도우 소켓을 종료할때 호출하는 함수
	WSACleanup();
	return 0;
}
