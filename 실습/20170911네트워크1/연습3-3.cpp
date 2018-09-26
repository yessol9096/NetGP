#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

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

// ������ �̸� -> IPv4 �ּ�
BOOL printDomain(char *name)
{
	// ������ �̸��� ip�ּҸ� ��ȣ ��ȯ ���ִ� ���� �Լ� 
	// ptr �� hostent ����ü�� �����͸� ���� 
	HOSTENT *ptr = gethostbyname(name);
	// ���� �߻��� ���� ����ϰ� false ����
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return FALSE;
	}
	// �ּ�ü�� 
	if (ptr->h_addrtype != AF_INET)
		return FALSE;

	int i = 0;
	// ����
	while (ptr->h_aliases[i] != NULL)
	{
		cout << ptr->h_aliases[i] << endl;
		++i;
	}

	i = 0;
	
	// ��Ʈ��ũ ����Ʈ ���ĵ� ip�ּ� 
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
	cout << "������ �̸��� �Է����ּ���: ";
	cin >> inputData;

	printDomain(inputData);

	WSACleanup();
	return 0;
}

