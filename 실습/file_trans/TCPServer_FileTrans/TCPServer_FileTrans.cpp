#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <fstream> 

#define SERVER_PORT	9000
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
	cout << "["  << msg << "] "<< (char *)lpMsgBuf;
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;
	int downSize = len / 10;
	// 전송률 보기 위해서 

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
		cout << "파일 전송률: [ "  << ( ((float)len - (float)left) / (float)len) * 100 <<  "% ] " << endl;
	}

	return (len - left);
}


int main(int argc, char *argv[])
{
	int retval;


	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	// socket() 
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind() 소켓의 지역 ip주소와 지역 포트 번호를 결정 
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));	//0 으로 초기화 쓰레기값 있을까봐 
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);		// 지역포트 9000 바이트 정렬 변경값 넣어주어야함 
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr)); 
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen() 클라 접속을 받아들일 수 있는 상태 
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char namebuf[NAMESIZE];
	char *databuf;
	int datalen;
	char* fileName;	// 파일 이름
	char filePath[256];


	cout  << "-----------------------------------------------------------------------------------------" << endl;
	cout << "                        T C P -  S E R V E R _ F I L E T R A N S" << endl;
	cout << "-----------------------------------------------------------------------------------------" << endl;


	cout << "저장할 파일의 경로를 입력하세요(ex: C:\\server\\)" << endl;
	cout << "--> ";
	gets_s(filePath);

	cout << endl << "-----------------------------------------------------------------------------------------" << endl;

	while (1) {
		// accept() 클라이언트와 통신하게 새로운 소켓 생성과 리턴
		// 클라이언트 ip+포트번호 알려줌 
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		cout << endl;
		// inet_ntoa 32비트 값을 ip 주소값으로 변환 // ntohs 네트워크(빅엔디안)을 호스트바이트(리틀엔디안) 변경 
		cout << "[TCP Server] client 접속 : IP 주소=" << inet_ntoa(clientaddr.sin_addr) << "포트 번호 = " << ntohs(clientaddr.sin_port);

		cout << endl << "-----------------------------------------------------------------------------------------" << endl;

		// 클라이언트와 데이터 통신
		while (1) {
			//(char *)&datalen (데이터 고정 길이 ) 저장할 응용프로그램 버퍼의 주소 
			// 데이터 받기(데이터 고정 길이)
			retval = recv(client_sock, (char *)&datalen, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv1()");
				break;
			}
			else if (retval == 0)
				break;


			// 데이터 받기(파일경로 및 파일명)
			retval = recv(client_sock, namebuf, sizeof(namebuf), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv2()");
				break;
			}
			else if (retval == 0)
				break;


			//datalen 에 가변길이 데이터 크기 저장되어있음
			// 데이터 받기(데이터 값)
			databuf = new char[datalen + 1];			// 데이터 마지막에있는  NULL문자

			// 가변길이 
			retval = recvn(client_sock, databuf, datalen, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv3()");
				break;
			}
			else if (retval == 0)
				break;

			cout << "[TCP/" << inet_ntoa(clientaddr.sin_addr) << ": " << ntohs(clientaddr.sin_port) << "]" << sizeof(datalen) << " + " << sizeof(namebuf) << " + " << datalen << "바이트를 받았습니다.";

			cout << endl << "-----------------------------------------------------------------------------------------" << endl;
			// 파일명 추출
			unsigned short pathLen = strlen(namebuf);	// 파일명 포함 전체길이
			unsigned short idxfileNameEndPos = 0;			// 파일경로(파일명 제외)가 끝나는 인덱스

			for (int i = pathLen; i > 0; i--) {			// 파일 경로 끝 위치 추출
				if (namebuf[i] == '\\') {
					idxfileNameEndPos = i;
					break;
				}
			}

			unsigned short fileNameLength = pathLen - idxfileNameEndPos - 1;	// 파일명 길이 계산 / NULL포인터값 빼																													
			fileName = new char[fileNameLength + 1];
			// src의 바이트를 dest로 복사합니다. 버퍼 자체 복사 
			memcpy_s(fileName, fileNameLength, namebuf + (idxfileNameEndPos + 1), fileNameLength);
			fileName[fileNameLength] = '\0';		//아스키코드 nul 정의


			strcat(filePath, fileName);	// filePath + fileName
			cout << "파일이 저장될 위치 : " << filePath << endl;

			cout << endl << "-----------------------------------------------------------------------------------------" << endl;

			// 받은 데이터 저장
			databuf[retval] = '\0';

			FILE *fp = fopen(filePath, "wb");
			fwrite(databuf, datalen, 1, fp);			// 데이터 저장
			fclose(fp);
		}

		// closesocket()
		closesocket(client_sock);
		cout << "[TCP Server] client 종료: IP 주소=" << inet_ntoa(clientaddr.sin_addr) << ", 포트 번호=" << ntohs(clientaddr.sin_port) << endl;

		delete[] fileName;
		delete[] databuf;
	}

	// closesocket()
	closesocket(listen_sock);


	// 윈속 종료
	WSACleanup();
	return 0;
}