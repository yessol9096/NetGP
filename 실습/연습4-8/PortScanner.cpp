#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define IPBUFSIZE		32

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
	//char ipbuf[256];
	int MinPortNum, MaxPortNum;
	int result;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// IP, 최대최소 포트번호 입력
	/*printf("IP 주소 입력 : ");
	gets_s(ipbuf);
	printf("최소, 최대 포트번호 : ");
	scanf_s("%d %d", &MinPortNum, &MaxPortNum);*/
	MinPortNum = atoi(argv[2]);
	MaxPortNum = atoi(argv[3]);

	
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	// 쓰레기값 있을까봐 초기화 
	serveraddr.sin_family = AF_INET;
	// 인터넷 주소체계 사용
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	// 입력
	for (int i = MinPortNum; i <= MaxPortNum; i++)
	{
		serveraddr.sin_port = htons(i);
		// 호스트 바이트 순서를 네트워크 바이트 순서로 
		result = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));	// connect 함수가 제대로 동작하면 0을 리턴한다.
		// 서버가 사용하는 소켓은 특정 포트번호와 결합 되어있음 
		// 이 포트 번호로 접속하는 클라만 수용이가능 == connect 가 되면 포트번호가 열려있다는 뜻 
		if (result == SOCKET_ERROR) printf("%d : not Listen\n", i);
		if (result == 0) printf("%d : Listen\n", i);
	}
	printf("End\n");

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}