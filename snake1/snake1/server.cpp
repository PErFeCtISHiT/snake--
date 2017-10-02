#include<iostream>
#include<WinSock2.h>
using namespace std;
int main(){
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));

	listen(sockSrv, 5);

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	while (1)
	{
		SOCKET sock1 = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		SOCKET sock2 = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		char recvBuf1[1];
		char recvBuf2[1];
		recv(sock1, recvBuf1, 1, 0);
		recv(sock2, recvBuf2, 1, 0);
		send(sock1, recvBuf2, 1, 0);
		send(sock2, recvBuf1, 1, 0);

	}
}