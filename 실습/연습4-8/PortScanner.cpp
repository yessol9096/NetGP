#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define IPBUFSIZE		32

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
	//char ipbuf[256];
	int MinPortNum, MaxPortNum;
	int result;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// IP, �ִ��ּ� ��Ʈ��ȣ �Է�
	/*printf("IP �ּ� �Է� : ");
	gets_s(ipbuf);
	printf("�ּ�, �ִ� ��Ʈ��ȣ : ");
	scanf_s("%d %d", &MinPortNum, &MaxPortNum);*/
	MinPortNum = atoi(argv[2]);
	MaxPortNum = atoi(argv[3]);

	
	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	// �����Ⱚ ������� �ʱ�ȭ 
	serveraddr.sin_family = AF_INET;
	// ���ͳ� �ּ�ü�� ���
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	// �Է�
	for (int i = MinPortNum; i <= MaxPortNum; i++)
	{
		serveraddr.sin_port = htons(i);
		// ȣ��Ʈ ����Ʈ ������ ��Ʈ��ũ ����Ʈ ������ 
		result = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));	// connect �Լ��� ����� �����ϸ� 0�� �����Ѵ�.
		// ������ ����ϴ� ������ Ư�� ��Ʈ��ȣ�� ���� �Ǿ����� 
		// �� ��Ʈ ��ȣ�� �����ϴ� Ŭ�� �����̰��� == connect �� �Ǹ� ��Ʈ��ȣ�� �����ִٴ� �� 
		if (result == SOCKET_ERROR) printf("%d : not Listen\n", i);
		if (result == 0) printf("%d : Listen\n", i);
	}
	printf("End\n");

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}