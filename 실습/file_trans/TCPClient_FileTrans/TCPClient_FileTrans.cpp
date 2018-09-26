#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>

#define SERVER_IP		"127.0.0.1"			// 아이피
#define SERVER_PORT	 9000				// 포트번호
#define NAMESIZE	100

using namespace std;

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
	cout << "[" << msg << "] " << (char *)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
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

	// 데이터 통신에 사용할 변수
	FILE *fp;
	char namebuf[NAMESIZE];
	char *databuf;
	int datalen;

	cout << "-----------------------------------------------------------------------------------------" << endl;
	cout << "                  T C P - C L I E N T_ F I L E T R A N S" << endl;
	cout << "-----------------------------------------------------------------------------------------" << endl;


	cout << "전송할 파일의 경로를 입력하세요: " << endl;
	cout << "--> ";
	gets_s(namebuf);

	// 데이터 읽음
	fp = fopen(namebuf, "rb");
	fseek(fp, 0, SEEK_END);  // 파일 마지막까지 포인터 이동
	datalen = ftell(fp); // 포인터의 위치(크기)
	fseek(fp, 0, SEEK_SET); // 포인터 초기화
	databuf = new char[datalen + 1];		
	fread(databuf, datalen, 1, fp); // 데이터 읽고 저장

	cout << endl << "-----------------------------------------------------------------------------------------" << endl;


	// 서버와 데이터 통신

	// send(데이터 길이)
	retval = send(sock, (char *)&datalen, sizeof(datalen), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}


	// send(파일경로 및 파일명)
	retval = send(sock, namebuf, sizeof(namebuf), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// datalen에 가변길이 데이터 크기 저장되어 있음
	// databuf 내용 보내기
	// send(데이터 값)
	retval = send(sock, databuf, datalen, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	cout << "[TCP Client]" << sizeof(datalen) << " + " << sizeof(namebuf) << " + " << datalen << "바이트를 보냈습니다. "<< endl;

	cout << endl << "-----------------------------------------------------------------------------------------" << endl;

	fclose(fp);

	// closesocket()
	closesocket(sock);


	delete[] databuf;



	// 윈속 종료
	WSACleanup();
	return 0;
}