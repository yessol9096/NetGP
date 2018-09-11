#pragma commnet(lib, "ws2_32")
#include <winsock2.h>

void err_quit(char *msg)
{
	LPVOID LpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&LpMsgBuf, 0, NULL);

		MessageBox(NULL, (LPCTSTR)LpMsgBuf, msg, MB_ICONERROR);
		LocalFree(LpMsgBuf);
		exit(1);
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);	// 2 , tcp, 0
	if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "TCP 소켓 생성 성공", "알림", MB_OK);

	closesocket(tcp_sock);

	WSACleanup();
	return 0;
}
