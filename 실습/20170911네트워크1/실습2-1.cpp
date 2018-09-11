#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    512

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
	// 윈속 초기화
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
	MessageBox(NULL, "윈속초기화성공", "알림", MB_OK);

	// 윈속 종료
	WSACleanup();
	return 0;
}