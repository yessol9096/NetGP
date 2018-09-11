#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	char *ipv4test = "147.46.114.70";
	printf("IPV4 주소(변환 전) = %s\n", ipv4test);

	// inet_addr() 함수 연습
	printf("IPV4 주소(변환 후) = 0x%x\n", inet_addr(ipv4test));

	//inet_addr() 함수 연습
	IN_ADDR ipv4num;
	ipv4num.s_addr = inet_addr(ipv4test);
	printf("IPv4 주소(다시 변환 후) = %s\n", inet_ntoa(ipv4num));

	printf("\n");

	///////ipv6 변환 연습
	// 원래의 ipv6 주소 출력 
	char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6 주소(변환 전) = %s\n", ipv6test);


	
}