#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>

#define SERVER_IP		"127.0.0.1"			// ������
#define SERVER_PORT	 9000				// ��Ʈ��ȣ
#define NAMESIZE	100

using namespace std;

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
	cout << "[" << msg << "] " << (char *)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	FILE *fp;
	char namebuf[NAMESIZE];
	char *databuf;
	int datalen;

	cout << "-----------------------------------------------------------------------------------------" << endl;
	cout << "                  T C P - C L I E N T_ F I L E T R A N S" << endl;
	cout << "-----------------------------------------------------------------------------------------" << endl;


	cout << "������ ������ ��θ� �Է��ϼ���: " << endl;
	cout << "--> ";
	gets_s(namebuf);

	// ������ ����
	fp = fopen(namebuf, "rb");
	fseek(fp, 0, SEEK_END);  // ���� ���������� ������ �̵�
	datalen = ftell(fp); // �������� ��ġ(ũ��)
	fseek(fp, 0, SEEK_SET); // ������ �ʱ�ȭ
	databuf = new char[datalen + 1];		
	fread(databuf, datalen, 1, fp); // ������ �а� ����

	cout << endl << "-----------------------------------------------------------------------------------------" << endl;


	// ������ ������ ���

	// send(������ ����)
	retval = send(sock, (char *)&datalen, sizeof(datalen), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}


	// send(���ϰ�� �� ���ϸ�)
	retval = send(sock, namebuf, sizeof(namebuf), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// datalen�� �������� ������ ũ�� ����Ǿ� ����
	// databuf ���� ������
	// send(������ ��)
	retval = send(sock, databuf, datalen, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	cout << "[TCP Client]" << sizeof(datalen) << " + " << sizeof(namebuf) << " + " << datalen << "����Ʈ�� ���½��ϴ�. "<< endl;

	cout << endl << "-----------------------------------------------------------------------------------------" << endl;

	fclose(fp);

	// closesocket()
	closesocket(sock);


	delete[] databuf;



	// ���� ����
	WSACleanup();
	return 0;
}