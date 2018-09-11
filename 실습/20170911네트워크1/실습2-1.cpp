#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


int main(int argc, char *argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	int HB = 0;
	HB = (wsa.wVersion / (pow(2, 8)));
	int LB = 0;
	LB = wsa.wVersion - (pow(2, 7+ HB)); 
	int HighHB = 0;
	HighHB = (wsa.wHighVersion / (pow(2, 8)));
	int HighLB = 0;
	HighLB = wsa.wHighVersion - pow(2, 7 + HighHB);

	cout << "wVersion:" << HB << "." << LB << endl;
	cout << "wHighVersion:" << HighHB << "." << HighLB << endl;
	cout << "szDescription:" << wsa.szDescription << endl;
	cout << "szSystemStatus:" << wsa.szSystemStatus << endl;
	MessageBox(NULL, "�����ʱ�ȭ����", "�˸�", MB_OK);

	// ���� ����
	WSACleanup();
	return 0;
}