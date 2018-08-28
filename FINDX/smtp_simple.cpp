
#include "stdafx.h"
#include <stdio.h>
#include "base64.h"
#include <winsock.h>

//#pragma  comment(  linker,  "/subsystem:\"windows\"  /entry:\"mainCRTStartup\""  )

#pragma comment(lib, "Ws2_32.lib")
//

int init_socket();
void socket_close();
SOCKET socket_connect(const char * hostname, int port);
void get_sockbuf();
void send_socket(const char *data);
 
SOCKET sockfd = 0;

static unsigned int test_start = 0;
static char sub_buffer[200];
int main()
{
	while(1)
	{
		char data[1024] = { 0 };
		char base64_data[2048] = { 0 };
		int i = 0;
		init_socket();
		sockfd = socket_connect("smtp.exmail.qq.com", 25);
 
		//接收服务器信息
		get_sockbuf();
 
		//发送helo信息
		send_socket("helo smtp.exmail.qq.com\r\n");
		//接收服务器应答
		get_sockbuf();
 
		//发送登录请求
		send_socket("auth login\r\n");
		//接收服务器应答
		get_sockbuf();
 
		//发送编码后的邮箱账号
		memset(data, 0, sizeof(data));
		strcpy(data, "niuzl@feimarobotics.com");
		base64_encode(data, strlen(data), base64_data);
		i = strlen(base64_data);
		base64_data[i] = '\r';
		base64_data[i+1] = '\n';
		send_socket(base64_data);
		//接收服务器应答
		get_sockbuf();
 
		//发送编码后的邮箱密码
		memset(data, 0, sizeof(data));
		memset(base64_data, 0, sizeof(base64_data));
		strcpy(data, "3361100niu");
		base64_encode(data, strlen(data), base64_data);
		i = strlen(base64_data);
		base64_data[i] = '\r';
		base64_data[i + 1] = '\n';
		send_socket(base64_data);
		//接收服务器应答
		get_sockbuf();
 
		//mail from
		send_socket("MAIL FROM:<niuzl@feimarobotics.com>\r\n");
		//接收服务器应答
		get_sockbuf();
 
		//rcpt to
		send_socket("rcpt to:<niuzl@feimarobotics.com>\r\n");
		//接收服务器应答
		get_sockbuf();
 
		//正文
		send_socket("data\r\n");

		memset(sub_buffer,0,sizeof(sub_buffer));

		sprintf(sub_buffer,"subject:上线通知 %s %d\r\n",__TIME__,test_start++);

		send_socket(sub_buffer);
		send_socket("\r\n上线机器为：niu!\r\n.\r\n");
		//接收服务器应答
		get_sockbuf();
 
		//退出
		send_socket("quit\r\n");
		//接收服务器应答
		get_sockbuf();
		socket_close();
		/*----------*/
		Sleep(60*1000);
	}
}
 
void send_socket(const char *data)
{
	if (send(sockfd, data, strlen(data), 0) < 0)
	{
		printf("发送失败!\n");
		return;
	}
	printf("%s", data);
}
void get_sockbuf()
{
	static char buf[1024] = { 0 };
	memset(buf, 0, sizeof(buf));
	if (recv(sockfd, buf, sizeof(buf), 0) < 0)
	{
		return;
	}
	printf("%s", buf);
}
 
int init_socket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}
 
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	return 0;
}
 
SOCKET socket_connect(const char * hostname, int port)//连接套接字
{
	struct hostent*host;
	unsigned long ip = 0;
 
	if (init_socket() == -1)
		return 0;
	SOCKET st = socket(AF_INET, SOCK_STREAM, 0);
	if (st == 0)
		return 0;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
 
	
	ip = inet_addr(hostname);
	if (ip == INADDR_NONE)
	{
		host = gethostbyname(hostname);
		addr.sin_addr = *((struct in_addr*)(host->h_addr_list[0]));
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(hostname);
	}
 
	if (connect(st, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("连接失败\n");
		return 0;
	}
	else
	{
		return st;
	}
}
 
void socket_close()
{
	closesocket(sockfd);
	WSACleanup();
}
