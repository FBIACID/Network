#include "HttpCommon.h"
#include "file_type.h"
#include "Mysql.h"

void Post_Get_Judge(SOCKET& client, char* msg)
{
	string data = msg;
	vector<string> url = Split(data,"\r\n");
	string tmp = url[0]; url.clear();
	data.assign(tmp);
	
	string::size_type idx;
	idx = data.find("email");
	bool flag = true;
	if (idx != string::npos)
	{
		idx = data.find("nemail");
		url = Split(tmp, "=");
		string username = url[1];
		string password = url[2]; url.clear();
		url = Split(username, "&"); username.assign(url[0]); url.clear();
		url = Split(password, " "); password.assign(url[0]); url.clear();
		if (idx != string::npos) {
			flag = false;
		}
		Http_Post(client,username,password,flag);
	}
	else
	{
		url = Split(tmp, " ");
		string type = url[0];
		if (type.compare("POST") == 0) {
			Save_Files(client,msg);
		}
		else
		{
			Http_Get(client, url[1], true);
		}
	}

}
void Http_Post(SOCKET& client, std::string user, std::string password,bool flag)
{
	MYSQL DB;
	Init_Mysql(DB);
	string sql;
	if (flag == false) {
		sql.assign("INSERT INTO USER_NET VALUES(\'" + user + "\',\'" + password + "\')");
		Insert(sql, DB);
		Http_Get(client, "/Server/html/login.html",true);
		return;
	}
	sql.assign("SELECT PID FROM USER_NET WHERE UID=\'" + user + "\'");
	string MYSQL_RESULT = Select(sql,DB);
	if (MYSQL_RESULT.compare(password) == 0) {
		Http_Get(client, "/Server/data",true);
		return;
	}
	else
	{
		Http_Get(client, "/Server/html/login.html",false);
		return;
	}

}
void Http_Get(SOCKET &client, std::string url,bool flag)
{
	string tmp;
	tmp.assign(Decode(url));
	string::size_type idx;
	
	
	vector<File_Information> files;Get_File_Information(tmp,files);

	if (files.size() == 0) {
		File_Information F_D;
		int a = Get_File_Information(tmp, F_D);
		if (a == -1) {
			return;
		}
		if (!flag)
		{
			string set_cookie = "Set-Cookie: pass=\"\";expires=Thu, 01 Jan 1970 00:00:00 GMT\r\n";
			SendHeaders(client, F_D, set_cookie);
		}
		else
		{
			SendHeaders(client, F_D);
		}
		Send_Files(client,tmp);
	}
	else {
		struct File_Information F_D;
		F_D.filename = tmp;
		//int H_L = 0;
		//SendHeaders(client, F_D,H_L);
		if (!flag)
		{
			string set_cookie = "Set-Cookie: pass=\"\";expires=Thu, 01 Jan 1970 00:00:00 GMT\r\n";
			SendHeaders(client, F_D, set_cookie);
		}
		else
		{
			SendHeaders(client, F_D);
		}
		//string css = CSS_JS_GET();
		string base_html = Base_HTml_GET();

		vector<string> urls = Encode(files,url);

		idx = url.rfind("/");
		url.assign(url.substr(0, idx));

		if (urls.size() == 0) { return; }
		string html;
		html.append("<div id=\"app\"><div class=\"container\"><div><div><ol class=\"breadcrumb\">");
		html.append("<li class=\"breadcrumb-item\"><a href=\""+url);
		html.append("\" class=\"router-link-active\">" + url + "</a></li></ol></div>");
		html.append("<div data-v-75395be8=\"\" class=\"table-responsive\"><table data-v-75395be8=\"\" class=\"table table-sm table-hover sortable table-bordered\"><thead data-v-75395be8=\"\" class=\"thead-light\"><tr data-v-75395be8=\"\"><th data-v-75395be8=\"\" class=\"text-left\"><\/th><th data-v-75395be8=\"\" class=\"text-left\">文件名<\/th><th data-v-75395be8=\"\" class=\"text-right size\">文件大小<\/th><th data-v-75395be8=\"\" data-sort-default=\"\" class=\"text-left date\">上传日期<\/th><\/tr><\/thead>");

		/*  模拟测试 Version 1
		*/

		string file_picture, dir_picture;
		file_picture.assign("<td data-v-75395be8=\"\" class=\"icon\"><svg data-v-75395be8=\"\" version=\"1.1\" width=\"12\" height=\"16\" aria-hidden=\"true\"><path data-v-75395be8=\"\" fill-rule=\"evenodd\" d=\"M6 5H2V4h4v1zM2 8h7V7H2v1zm0 2h7V9H2v1zm0 2h7v-1H2v1zm10-7.5V14c0 .55-.45 1-1 1H1c-.55 0-1-.45-1-1V2c0-.55.45-1 1-1h7.5L12 4.5zM11 5L8 2H1v12h10V5z\"></path></svg></td>");
		dir_picture.assign("<td data-v-75395be8=\"\" class=\"icon\"><svg data-v-75395be8=\"\" version=\"1.1\" width=\"14\" height=\"16\" aria-hidden=\"true\"><path data-v-75395be8=\"\" fill-rule=\"evenodd\" d=\"M13 4H7V3c0-.66-.31-1-1-1H1c-.55 0-1 .45-1 1v10c0 .55.45 1 1 1h12c.55 0 1-.45 1-1V5c0-.55-.45-1-1-1zM6 4H1V3h5v1z\"></path></svg></td>");
		
		SYSTEMTIME sys;
		GetLocalTime(&sys);

		for (int i = 0; i < urls.size(); i++) {
			char* tmpA = new char[512];
			/* 增加获取文件修改时间 */

			SYSTEMTIME _File_TIME_, _STROL_TIME;
			SystemTimeToTzSpecificLocalTime(NULL,&_STROL_TIME,&_File_TIME_);
			FileTimeToSystemTime(&(files[i].ftLastWriteTime), &_STROL_TIME);

			if (files[i].flag == 0) {
				html.append(dir_picture);
				char* time_1 = new char[128];
				sprintf(time_1, "%d年%d月%d日 %d:%d", _File_TIME_.wYear, _File_TIME_.wMonth, _File_TIME_.wDay,_File_TIME_.wHour,_File_TIME_.wMinute);
				sprintf(tmpA, "<td data-v-75395be8=\"\"><a data-v-75395be8=\"\" href=\"%s\" title=\"\" target=\"_blank\" data-tooltip=\"toggle\" data-original-title=\"\">%s</a></td><td data-v-75395be8=\"\" title=\"null\" class=\"text-right\">null</td><td data-v-75395be8=\"\" title=\"%s\">%d 个月前</td></tr>", urls[i].c_str(), files[i].filename.c_str(),time_1, Compare_TIME(sys,_File_TIME_));
				delete[] time_1;
			}
			else
			{
				char* time_2 = new char[128];
				sprintf(time_2, "%d年%d月%d日 %d:%d", _File_TIME_.wYear, _File_TIME_.wMonth, _File_TIME_.wDay, _File_TIME_.wHour, _File_TIME_.wMinute);
				html.append(file_picture);
				sprintf(tmpA, "<td data-v-75395be8=\"\"><a data-v-75395be8=\"\" href=\"%s\" title=\"\" target=\"_blank\" data-tooltip=\"toggle\" data-original-title=\"\">%s</a></td><td data-v-75395be8=\"\" title=\"%lld\" class=\"text-right\">%lldMB</td><td data-v-75395be8=\"\" title=\"%s\">%d 个月前</td></tr>", urls[i].c_str(), files[i].filename.c_str(), files[i].file_size.QuadPart, files[i].file_size.QuadPart/(1024*1024)+1,time_2,Compare_TIME(sys, _File_TIME_));
				delete[] time_2;
			}
			html.append(tmpA);
			delete[] tmpA;
		}
		html.append("</tbody></table></div></div></div></div><div class=\"footer\"><p class=\"copyright\"><span>Copyright © 2008-2018 52pojie.cn</span> <span>All Rights Reserved 版权所有</span> <span>京ICP备16042023号 京公网安备11010502030087号</span></p></div></body></html>");
		/* Version 1 end*/
		/*for (int i = 0; i < urls.size(); i++) {
			char tmp1[256] = { 0 };
			if (files[i].flag == 0) {
				sprintf(tmp1, "<div class=\"dirItem\">\n<a href=\"%s\">\n%s</a>\n</div>\n", urls[i].c_str(), files[i].filename.c_str());
			}else{
				sprintf(tmp1, "<div class=\"fileItem\">\n<a href=\"%s\" target=\"tab\">\n%s <span class=\"sizeSpan\">[Size:%lld]</span></a>\n</div>\n", urls[i].c_str(), files[i].filename.c_str(), files[i].file_size.QuadPart);
			}
			html.append(tmp1);
		}
		html.append("</body></html>");*/
		char Length[128] = { 0 };

		/* 转为UTF-8编码 */
	
		wstring_convert<codecvt_byname<wchar_t, char, mbstate_t>> cv1(new codecvt_byname<wchar_t, char, mbstate_t>(".936"));
		wstring tmp_wstr = cv1.from_bytes(html);
		wstring_convert<codecvt_utf8<wchar_t>> cv2;
		string utf8_html = cv2.to_bytes(tmp_wstr);

		sprintf(Length, "Content-Length: %d\r\n\r\n", utf8_html.length()+base_html.length());
		send(client, Length, strlen(Length), 0);
		send(client, base_html.c_str(), base_html.length(), 0);
		send(client, utf8_html.c_str(), utf8_html.length(), 0);
		shutdown(client, SD_SEND);
	}
}

std::vector<std::string> Encode(std::vector<File_Information> file, std::string url)
{
	/* Return Url List */
	std::vector<std::string> result_;
	if (file.size() == 0) {
		exit(-1);
	}
	for (int i = 0; i < file.size(); i++)
	{
		result_.push_back(url + "/" + file[i].filename);
	}
	return result_;
}
std::string Decode(std::string url)
{
	using namespace std;
	vector<string> Dir = Split(url, "/");
	/* First is null */
	string result_ = ROOTDIR;
	for (int i = 1; i < Dir.size(); i++)
	{
		result_.append("\\" + Dir[i]);
	}
	return result_;
}

void SendHeaders(SOCKET &client, File_Information file)
{
	using namespace std;
	string Http_Head;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char tmp[64] = { 0 };
	sprintf(tmp, "Date: %4d/%02d/%02d %02d:%02d:%02d\r\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	if (file.flag == 0) {
		Http_Head.append("HTTP/1.0 200 OK\r\n");
		Http_Head.append("Server: NetWorkProgrambing/C++\r\n");
		Http_Head.append(tmp);
		Http_Head.append("Content-Type: text/html; charset=utf-8\r\n");
	}
	else
	{
		Http_Head.append("HTTP/1.0 200 OK\r\n");
		Http_Head.append("Server: NetWorkProgrambing/C++\r\n");
		Http_Head.append(tmp);
		string Content_Type = getContentTypeFromFileName(file.filename.c_str());
		memset(tmp, 0, 64);
		sprintf(tmp, "Content-Type: %s;\r\n", Content_Type.c_str());
		Http_Head.append(tmp);
		memset(tmp, 0, 64);
		sprintf(tmp, "Content-Length: %d\r\n\r\n", file.file_size.QuadPart);
		Http_Head.append(tmp);
	}
	send(client, Http_Head.c_str(), Http_Head.length(), 0);
}
void SendHeaders(SOCKET& client, File_Information file, string cookie)
{
	using namespace std;
	string Http_Head;
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char tmp[64] = { 0 };
	sprintf(tmp, "Date: %4d/%02d/%02d %02d:%02d:%02d\r\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	if (file.flag == 0) {
		Http_Head.append("HTTP/1.0 200 OK\r\n");
		Http_Head.append("Server: NetWorkProgrambing/C++\r\n");
		Http_Head.append(tmp);
		Http_Head.append("Content-Type: text/html; charset=utf-8\r\n");
		Http_Head.append(cookie);
	}
	else
	{
		Http_Head.append("HTTP/1.0 200 OK\r\n");
		Http_Head.append("Server: NetWorkProgrambing/C++\r\n");
		Http_Head.append(tmp);
		string Content_Type = getContentTypeFromFileName(file.filename.c_str());
		memset(tmp, 0, 64);
		sprintf(tmp, "Content-Type: %s;\r\n", Content_Type.c_str());
		Http_Head.append(tmp);
		memset(tmp, 0, 64);
		Http_Head.append(cookie);
		sprintf(tmp, "Content-Length: %d\r\n\r\n", file.file_size.QuadPart);
		Http_Head.append(tmp);
	}
	send(client, Http_Head.c_str(), Http_Head.length(), 0);
}

void Send_Files(SOCKET &client, std::string FileName) {
	using namespace std;
	ifstream infile;
	infile.open(FileName.c_str(), ios::in | ios::binary);
	if (!infile) {
		cerr << "Can't open this File:" << FileName << endl;
		return;
	}
	else
	{
		std::cout << "Get file:\t" << FileName << endl;
		char Send_Data[BUFFSIZE] = { 0 };
		while (!infile.eof()) {
			infile.read(Send_Data, BUFFSIZE - 1);
			unsigned readlen = infile.gcount();
			send(client, Send_Data, readlen, 0);
		}
	}
	infile.close();
	shutdown(client, SD_SEND);
}

int Get_File_Information(std::string Dir, File_Information &file)
{
	using namespace std;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LARGE_INTEGER size;
	hFind = FindFirstFile(Dir.c_str(), &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE) {
		return -1;
	}
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		string demo = FindFileData.cFileName;
		file.filename = demo;
	}
	else
	{
		size.LowPart = FindFileData.nFileSizeLow;
		size.HighPart = FindFileData.nFileSizeHigh;
		file.filename= FindFileData.cFileName;
		file.file_size = size;
		file.flag = 1;
	};
	FindClose(hFind);
	return 1;
}
void Get_File_Information(std::string Dir, std::vector<File_Information>& file)
{
	using namespace std;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LARGE_INTEGER size;
	Dir.append("\\*");
	hFind = FindFirstFile(Dir.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}
	do {
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			string demo = FindFileData.cFileName;
			if (demo.find(".", 0) == 0)
			{
				continue;           // 过滤隐藏文件
			}
			struct File_Information Dir_file;
			Dir_file.filename = demo;
			Dir_file.ftLastWriteTime = FindFileData.ftLastWriteTime;
			file.push_back(Dir_file);
		}
		else
		{
			size.LowPart = FindFileData.nFileSizeLow;
			size.HighPart = FindFileData.nFileSizeHigh;
			struct File_Information File;
			string PRO = FindFileData.cFileName;
			File.ftLastWriteTime = FindFileData.ftLastWriteTime;
			if (PRO.find(".", 0) == 0) {
				continue;            //¹
			}
			File.filename = PRO;
			File.file_size = size;
			File.flag = 1;
			file.push_back(File);
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);
	FindClose(hFind);
}

std::vector<std::string> Split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;
	auto size = str.size();

	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

int Compare_TIME(SYSTEMTIME& A, SYSTEMTIME& B)
{
	/* A 当前时间
	   B 文件最后一次修改时间
	*/
	int RESULT = 0;
	int Year, Month;
	if (A.wYear >= B.wYear) {
		Year = A.wYear - B.wYear;
		Month = A.wMonth - B.wMonth;
		RESULT = Year * 12 + Month;
		return RESULT;
	}
	else
	{
		return 0;
	}
}

void Set_SOCKET_In(sockaddr_in& sock_addr, int port)
{
	sock_addr.sin_family = AF_INET;
	//data_server.sin_addr.S_un.S_addr = inet_addr(INADDR_ANY);
	sock_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sock_addr.sin_port = htons(port);
}
void Bind(SOCKET& socket_demo, sockaddr_in& server)
{
	if (bind(socket_demo, (struct sockaddr*) & server, sizeof(server)) == INVALID_SOCKET)
	{
		std::cerr << "Bind Socket failed:" << WSAGetLastError() << "\n";
		closesocket(socket_demo);
		WSACleanup();
		exit(-2);
	}
}
void Init_SOCKET(SOCKET& socket_demo)
{
	socket_demo = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_demo == INVALID_SOCKET) {
		std::cerr << WSAGetLastError() << std::endl;
		exit(1);
	}
}
void Listen(SOCKET& ListenSocket, int num)
{
	if (listen(ListenSocket, 5) == SOCKET_ERROR) {
		std::cerr << "Listing on have error:\n";
		closesocket(ListenSocket);
		WSACleanup();
		exit(-2);
	}
}
SOCKET Accept(SOCKET& ListenSocket, sockaddr_in& addr, int& addrlen)
{
	SOCKET AcceptSocket = accept(ListenSocket, (struct sockaddr*) & addr, &addrlen);
	if (AcceptSocket == INVALID_SOCKET) {
		std::cerr << "Accept error:" << WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		exit(-1);
	}
	else
	{
		return AcceptSocket;
	}
}

std::string Base_HTml_GET()
{
	std::ifstream infile;
	string base_html;
	char HTML_DATA[BUFFSIZE] = { 0 };
	infile.open("F:\\Temp\\Server\\html\\base.html");
	if (!infile) {
		std::cerr << "Can't get CSS file" << std::endl;
	}
	else
	{
		while (!infile.eof())
		{
			infile.read(HTML_DATA, BUFFSIZE);
			base_html.append(HTML_DATA);
		}
	}
	infile.close();
	return base_html;
}
DWORD CALLBACK file_worker(LPVOID lpCtx)
{
	HTTPCLIENT* pClient = (HTTPCLIENT*)lpCtx;
	//HTTPCLIENT pClient = lpCtx;
	SOCKET socket_client = pClient->ClientSocket;
	sockaddr_in address = pClient->addr;
	
	// std::cout << "Have a new Client 1:" << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;

	bool flag = false;
	char* recv_msg = new char[BUFFSIZE];
	int off = 0;
	do
	{
		auto i = recv(socket_client, &recv_msg[off], BUFFSIZE - off, 0);
		if (i <= 0) {
			break;     
		}
		off += i;
		if (strstr(recv_msg, "\r\n\r\n"))
		{
			flag = true;
			break;
		}
	} while (off < BUFFSIZE);
	if (flag) {
		//Http_Get(socket_client, recv_msg);
		Post_Get_Judge(socket_client, recv_msg);
	}
	delete[] recv_msg;
	closesocket(socket_client);
	return 0;
}

bool Substr(string::size_type &idx, string& prior, string& result)
{
	if (idx != string::npos) {
		result.clear();
		result.assign(prior.substr(idx));
		return true;
	}
	else
	{
		return false;
	}
}
void Save_Files(SOCKET& client,char* msg)
{
	// Http_Get(client, "/Server/404/404.html", true);    //为了抓包看数据传输格式
	vector<string> AAA_SB = Split(msg, "\r\n\r\n");
	string::size_type idx,idx2,pos;
	string _FILE_DATA___;
	if (AAA_SB.size() >= 2) {
		for (int i = 2; i < AAA_SB.size(); i++) {
			_FILE_DATA___.append(AAA_SB[i]);
		}
		//std::cout << _FILE_DATA___.length() << endl;
		string content_type; content_type.assign(AAA_SB[1]);
		idx = content_type.find("------WebKit");
		idx2 = content_type.find("\r\n");
		pos = idx2 - idx;
		if (pos > 0) {
			content_type.assign(AAA_SB[1].substr(idx, pos));
		}
		else
		{
			return;
		}

		string file_name; file_name.assign(AAA_SB[1]);
		idx = file_name.find("filename=\"");
		idx2 = file_name.find("\"\r\nC");
		pos = idx2 - idx;
		if (pos > 0) {
			file_name.assign(AAA_SB[1].substr(idx + 10, pos-10));
		}
		else
		{
			return;
		}

		std::ofstream outfile;
		string dir = "F:\\Temp\\Server\\data\\user\\";
		dir.append(file_name);
		file_name.assign(dir);
		outfile.open(file_name, ios::out | ios::binary);
		if (!outfile) {
			Http_Get(client, "/Server/html/404.html", true);
			outfile.close();
			return;
		}
		int leng_demo = AAA_SB[0].length() + AAA_SB[1].length() + 8;
		char* SB = new char[4100];
		for (int i = leng_demo; i < 4100; i++) {
			SB[i-leng_demo] = msg[i];
		}
		pos = _FILE_DATA___.find(content_type);
		if (pos != string::npos) { // 可以用作简单txt文本的上传 
			string file_data_true;
			file_data_true.assign(_FILE_DATA___.substr(0, pos-2));
			outfile.write(file_data_true.c_str(), file_data_true.length());
			outfile.close();
			std::cout << "Already save text file: " << file_name << endl;
			shutdown(client, SD_RECEIVE);
			Http_Get(client, "/Server/data/user", true);
			return;
		}
		else
		{
			outfile.write(SB, 4096-leng_demo);
			delete[] SB;
			while (true)
			{
				int rec = 0;
				char data[1024];
				memset(data, 0, 1024);
				rec = recv(client, data, 1024, 0);
				if (rec <= 0) {
					cerr << "Receive error" << WSAGetLastError() << endl;
					break;
				}
				/* 二进制数据无法匹配表单中的字符串 ?*/
				if (rec < 1024) {
					int length = content_type.length() + 6;
					outfile.write(data, rec-length);
					outfile.close();
					std::cout << "Already save file: " << file_name << endl;
					shutdown(client, SD_RECEIVE);
					Http_Get(client, "/Server/data/user", true);
					return;
				}else{
					outfile.write(data, rec);
				}
			}
		}

	}
	else {
		Http_Get(client, "/Server/html/404.html", true);
	}
	return;
}

/*  */
void Test(LPVOID lpCtx)
{
	HTTPCLIENT* pClient = (HTTPCLIENT*)lpCtx;
	//HTTPCLIENT pClient = lpCtx;
	SOCKET socket_client = pClient->ClientSocket;
	sockaddr_in address = pClient->addr;

	std::cout << "Have a new Client :" << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl;

	bool flag = false;
	char* recv_msg = new char[BUFFSIZE];
	int off = 0;
	do
	{
		auto i = recv(socket_client, &recv_msg[off], BUFFSIZE - off, 0);
		if (i <= 0) {
			break;
		}
		off += i;
		if (strstr(recv_msg, "\r\n\r\n"))
		{
			flag = true;
			break;
		}
	} while (off < BUFFSIZE);
	if (flag) {
		// Http_Get(socket_client, recv_msg);
		Post_Get_Judge(socket_client, recv_msg);
	}
	delete[] recv_msg;
	closesocket(socket_client);
	return;
}
/**/