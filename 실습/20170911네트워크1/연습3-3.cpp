#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

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

// 도메인 이름 -> IPv4 주소
BOOL printDomain(char *name)
{
	// 도메인 이름을 ip주소를 상호 변환 해주는 윈속 함수 
	// ptr 은 hostent 구조체형 포인터를 리턴 
	HOSTENT *ptr = gethostbyname(name);
	// 오류 발생시 오류 출력하고 false 리턴
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return FALSE;
	}
	// 주소체계 
	if (ptr->h_addrtype != AF_INET)
		return FALSE;

	int i = 0;
	// 별명
	while (ptr->h_aliases[i] != NULL)
	{
		cout << ptr->h_aliases[i] << endl;
		++i;
	}

	i = 0;
	
	// 네트워크 바이트 정렬된 ip주소 
	while (ptr->h_addr_list[i] != NULL)
	{
		cout << inet_ntoa(*(struct in_addr*)ptr->h_addr_list[i]) << endl;
		++i;
	}

	return TRUE;
}


int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	char *inputData = new char[30];
	cout << "도메인 이름을 입력해주세요: ";
	cin >> inputData;

	printDomain(inputData);

	WSACleanup();
	return 0;
}

