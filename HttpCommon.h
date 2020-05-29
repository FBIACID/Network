#pragma once

#include "stdax.h"

#ifndef HTTP_STATUS_HANDLE
#define HTTP_STATUS_HANDLE

#pragma comment(lib,"Ws2_32.lib")

#define BUFFSIZE 4096
#define DEFAULT_PORT 8080
#define ROOTDIR "F:\\Temp"

struct File_Information{
	std::string filename;
	LARGE_INTEGER file_size;
	FILETIME ftLastWriteTime;    //文件最后一次修改时间
	int flag = 0;                //表示目录
};

struct HTTPCLIENT {
	SOCKET ClientSocket;
	sockaddr_in addr;
};

/* Only for test Start */

void Test(LPVOID lpCtx);

/* End */
void Post_Get_Judge(SOCKET &client,char *msg);
void SendHeaders(SOCKET &client, File_Information file);
void SendHeaders(SOCKET& client, File_Information file, string cookie);
std::string Base_HTml_GET();
void Http_Get(SOCKET &client, std::string msg,bool flag);
void Http_Post(SOCKET &client,std::string user,std::string password,bool flag);

void Save_Files(SOCKET &client,char* msg);

void Send_Files(SOCKET &client,std::string FileName);

std::vector<std::string> Split(std::string str, std::string pattern);
bool Substr(string::size_type &idx,string &prior,string &result);
std::vector<std::string> Encode(std::vector<File_Information> file, std::string url);
std::string Decode(std::string url);
void Get_File_Information(std::string Dir, std::vector<File_Information> &file);
int Get_File_Information(std::string Dir, File_Information &);

int Compare_TIME(SYSTEMTIME &A,SYSTEMTIME &B);
/*
SOCKET Setting
*/
void Set_SOCKET_In(sockaddr_in& sock_addr, int port);
void Bind(SOCKET& socket_demo, sockaddr_in& server);
void Init_SOCKET(SOCKET& socket_demo);
void Listen(SOCKET& ListenSocket, int num);
SOCKET Accept(SOCKET& ListenSocket, sockaddr_in& addr, int& addrlen);

DWORD CALLBACK file_worker(LPVOID lpCtx);

/* 待解决 URL中文解码问题 ,以及特殊符号的Url编码 */

#endif // !HTTP_STATUS_HANDLE
