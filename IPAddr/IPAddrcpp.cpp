#include "Common.h"

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/*---------------*/
	/* IPv4 ��ȯ ���� */
	/*---------------*/

	// ������ IPv4 �ּ� ���

	const char* ipv4test = "147.46.114.70";
	printf("IPv4 �ּ�(��ȯ ��) = %s\n", ipv4test);

	// inet_pton() �Լ� ����
	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 �ּ�(��ȯ ��) = %#x\n", ipv4num.s_addr);

	// inet_ntop() �Լ� ����
	char ipv4str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));

	/*---------------*/
	/* IPv6 ��ȯ ���� */
	/*---------------*/

	// ������ IPv6 �ּ� ���
	const char* ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6 �ּ�(��ȯ ��) = %s\n", ipv6test);

	// inet_pton() �Լ� ����
	struct in_addr ipv6num;
	inet_pton(AF_INET6, ipv6test, &ipv6num);
	printf("IPv6 �ּ�(��ȯ ��) = %#x\n", ipv6num.s_addr);

	// inet_ntop() �Լ� ����
	char ipv6str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET6, &ipv6num, ipv6str, sizeof(ipv6str));

	// ���� ����
	WSACleanup();
	return 0;
}