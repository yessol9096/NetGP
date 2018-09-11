#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

BOOL IsLittleEndian();
BOOL IsBIgEndian();

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	if (IsLittleEndian() == true)
		printf("리틀 엔디안 입니다.\n");
	if (IsBIgEndian() == true)
		printf("빅 엔디안 입니다.\n");

	WSACleanup();
	return 0;


}

BOOL IsLittleEndian()
{
	int n = 0x00000001;
	
	if (((char *)&n)[0])
		return true;
	else
		return false;
};
BOOL IsBIgEndian() 
{
	int n = 0x00000001;
	
	if (!((char *)&n)[0])
		return true;
	else
		return false;
};