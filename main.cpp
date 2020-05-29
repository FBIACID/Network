#include "HttpCommon.h"


/*   实现目标

MYSQL存储用户信息实现登录验证
文件下载
用户注册/登录

文件上传

*/
int main()
{
	WSADATA wsaData;
	auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		std::cerr << "WSADATA failed :" << iResult << std::endl;
		exit(1);
	}
	SOCKET ListenSocket;
	Init_SOCKET(ListenSocket);
	sockaddr_in server;
	Set_SOCKET_In(server, DEFAULT_PORT);
	Bind(ListenSocket, server);
	Listen(ListenSocket, 5);

	SOCKET ACCEPTSOCKET = INVALID_SOCKET;
	sockaddr_in client_addr;
	int addrlen = sizeof(client_addr);
	std::cout << "Server have allready:" << std::endl;

	while (true) {
		ACCEPTSOCKET = Accept(ListenSocket, client_addr, addrlen);
		HTTPCLIENT http_status;
		http_status.ClientSocket = ACCEPTSOCKET;
		http_status.addr = client_addr;
		char tmp_buff[16] = { 0 };
		// std::cout << "Have a new Client :" << inet_ntop(AF_INET, &client_addr.sin_addr, tmp_buff, sizeof(tmp_buff)) << ":" << ntohs(client_addr.sin_port) << std::endl;
		Test((LPVOID*)& http_status);
		// HANDLE hthread = CreateThread(NULL, 0, file_worker, (LPVOID*)&http_status, 0, NULL);
		/* 
		
		开启多线程后反而加载缓慢 由于界面需要加载多个内容，cpu核心有限线程被阻塞所以执行缓
		文件下载展示界面所受影响较少，由于所有文件一次性发送
		由于时间问题，暂不解决

		*/
	}
	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

