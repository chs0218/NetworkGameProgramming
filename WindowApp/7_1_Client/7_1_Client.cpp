#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
//#include <windows.h>
//#include <tchar.h>
//#include <stdio.h>
#include "..\..\Common.h"
#include "resource.h"
#include <shobjidl.h>
#include <fstream>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512

#pragma pack(1)
struct DataSet {
	int len = 0;	// ������ ����
	char name[50] = "\0";	// ������ ���� �̸�
	char buf[BUFSIZE + 1];	// ������ ����
};
#pragma pack()

// ��ȭ���� ���ν���
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// ����Ʈ ��Ʈ�� ��� �Լ�
void DisplayText(const char* fmt, ...);
void DisplayFileName();
HANDLE hReadEvent, hWriteEvent; // �̺�Ʈ
HWND hSelectButton; // ������ ��ư
HWND hEdit1, hEdit2; // ����Ʈ ��Ʈ��
HWND hWnd;

// ������ ��ſ� ����� ����
DataSet data;
char pFilePath[500];

// ���� ��� ������ �Լ�
DWORD WINAPI ClientMain(LPVOID arg);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// �̺�Ʈ ����
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// ���� ��� ������ ����
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	// ��ȭ���� ����
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// �̺�Ʈ ����
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	// ���� ����
	WSACleanup();
	return 0;
}

// ��ȭ���� ���ν���
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
		hSelectButton = GetDlgItem(hDlg, IDSELECT);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDSELECT:
			DisplayFileName();
			SetFocus(hEdit1); // Ű���� ��Ŀ�� ��ȯ
			SendMessage(hEdit1, EM_SETLIMITTEXT, BUFSIZE, 0);
			return TRUE;
		case IDOK:
			WaitForSingleObject(hReadEvent, INFINITE); // �б� �Ϸ� ���
			EnableWindow(hSelectButton, FALSE); // ������ ��ư ��Ȱ��ȭ
			SetEvent(hWriteEvent); // ���� �Ϸ� �˸�				
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

// ����Ʈ ��Ʈ�� ��� �Լ�
void DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[BUFSIZE * 2];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(hEdit1);
	SendMessage(hEdit1, EM_SETSEL, nLength, nLength);
	SendMessageA(hEdit1, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}

void DisplayFileName()
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		// use ofn.lpstrFile
		wcstombs(pFilePath, ofn.lpstrFile, 500);
		SetWindowText(hEdit1, L"");

		char* FileName = strrchr(pFilePath, '\\');
		(FileName) ? strcpy(data.name, FileName + 1) : strcpy(data.name, pFilePath);

		DisplayText("%s\r\n", pFilePath);
	}
}

// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	while (1) {
		WaitForSingleObject(hWriteEvent, INFINITE); // ���� �Ϸ� ���

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

		std::ifstream in;

		in.open(pFilePath, std::ios::binary);

		in.seekg(0, std::ios::end);
		data.len = in.tellg();
		in.seekg(0, std::ios::beg);

		retval = send(sock, (char*)&data.len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		retval = send(sock, (char*)&data.name, 50, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		while (in.read(data.buf, BUFSIZE))
		{
			retval = send(sock, data.buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		retval = send(sock, data.buf, data.len % BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		EnableWindow(hSelectButton, TRUE); // ������ ��ư Ȱ��ȭ

		in.close();

		closesocket(sock); // ���� �ݱ�
		SetEvent(hReadEvent); // �б� �Ϸ� �˸�
	}

	return 0;
}