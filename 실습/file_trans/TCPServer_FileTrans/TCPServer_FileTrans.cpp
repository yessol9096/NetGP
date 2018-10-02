#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <fstream> 

#define SERVER_PORT	9000
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
	cout << "["  << msg << "] "<< (char *)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;
	int downSize = len / 10;
	// ���۷� ���� ���ؼ� 

	while (left > 0) {
		if (left - downSize < 0)
			downSize = left;
		received = recv(s, ptr, downSize, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
		cout << "���� ���۷�: [ "  << ( ((float)len - (float)left) / (float)len) * 100 <<  "% ] " << endl;
	}

	return (len - left);
}


int main(int argc, char *argv[])
{
	int retval;


	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	// socket() 
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind() ������ ���� ip�ּҿ� ���� ��Ʈ ��ȣ�� ���� 
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	//0 ���� �ʱ�ȭ �����Ⱚ ������� 
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);		// ������Ʈ 9000 ����Ʈ ���� ���氪 �־��־���� 
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr)); 
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen() Ŭ�� ������ �޾Ƶ��� �� �ִ� ���� 
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char namebuf[NAMESIZE];
	char *databuf;
	int datalen;
	char* fileName;	// ���� �̸�
	char filePath[256];


	cout  << "-----------------------------------------------------------------------------------------" << endl;
	cout << "                        T C P -  S E R V E R _ F I L E T R A N S" << endl;
	cout << "-----------------------------------------------------------------------------------------" << endl;


	cout << "������ ������ ��θ� �Է��ϼ���(ex: C:\\server\\)" << endl;
	cout << "--> ";
	gets_s(filePath);

	cout << endl << "-----------------------------------------------------------------------------------------" << endl;

	while (1) {
		// accept() Ŭ���̾�Ʈ�� ����ϰ� ���ο� ���� ������ ����
		// Ŭ���̾�Ʈ ip+��Ʈ��ȣ �˷��� 
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		cout << endl;
		// inet_ntoa 32��Ʈ ���� ip �ּҰ����� ��ȯ // ntohs ��Ʈ��ũ(�򿣵��)�� ȣ��Ʈ����Ʈ(��Ʋ�����) ���� 
		cout << "[TCP Server] client ���� : IP �ּ�=" << inet_ntoa(clientaddr.sin_addr) << "��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port);

		cout << endl << "-----------------------------------------------------------------------------------------" << endl;

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			//(char *)&datalen (������ ���� ���� ) ������ �������α׷� ������ �ּ� 
			// ������ �ޱ�(������ ���� ����)
			retval = recv(client_sock, (char *)&datalen, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv1()");
				break;
			}
			else if (retval == 0)
				break;


			// ������ �ޱ�(���ϰ�� �� ���ϸ�)
			retval = recv(client_sock, namebuf, sizeof(namebuf), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv2()");
				break;
			}
			else if (retval == 0)
				break;


			//datalen �� �������� ������ ũ�� ����Ǿ�����
			// ������ �ޱ�(������ ��)
			databuf = new char[datalen + 1];			// ������ ���������ִ�  NULL����

			// �������� 
			retval = recvn(client_sock, databuf, datalen, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv3()");
				break;
			}
			else if (retval == 0)
				break;

			cout << "[TCP/" << inet_ntoa(clientaddr.sin_addr) << ": " << ntohs(clientaddr.sin_port) << "]" << sizeof(datalen) << " + " << sizeof(namebuf) << " + " << datalen << "����Ʈ�� �޾ҽ��ϴ�.";

			cout << endl << "-----------------------------------------------------------------------------------------" << endl;
			// ���ϸ� ����
			unsigned short pathLen = strlen(namebuf);	// ���ϸ� ���� ��ü����
			unsigned short idxfileNameEndPos = 0;			// ���ϰ��(���ϸ� ����)�� ������ �ε���

			for (int i = pathLen; i > 0; i--) {			// ���� ��� �� ��ġ ����
				if (namebuf[i] == '\\') {
					idxfileNameEndPos = i;
					break;
				}
			}

			unsigned short fileNameLength = pathLen - idxfileNameEndPos - 1;	// ���ϸ� ���� ��� / NULL�����Ͱ� ��																													
			fileName = new char[fileNameLength + 1];
			// src�� ����Ʈ�� dest�� �����մϴ�. ���� ��ü ���� 
			memcpy_s(fileName, fileNameLength, namebuf + (idxfileNameEndPos + 1), fileNameLength);
			fileName[fileNameLength] = '\0';		//�ƽ�Ű�ڵ� nul ����


			strcat(filePath, fileName);	// filePath + fileName
			cout << "������ ����� ��ġ : " << filePath << endl;

			cout << endl << "-----------------------------------------------------------------------------------------" << endl;

			// ���� ������ ����
			databuf[retval] = '\0';

			FILE *fp = fopen(filePath, "wb");
			fwrite(databuf, datalen, 1, fp);			// ������ ����
			fclose(fp);
		}

		// closesocket()
		closesocket(client_sock);
		cout << "[TCP Server] client ����: IP �ּ�=" << inet_ntoa(clientaddr.sin_addr) << ", ��Ʈ ��ȣ=" << ntohs(clientaddr.sin_port) << endl;

		delete[] fileName;
		delete[] databuf;
	}

	// closesocket()
	closesocket(listen_sock);


	// ���� ����
	WSACleanup();
	return 0;
}