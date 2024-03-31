#pragma once
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <string.h>     // memset头文件
#include <arpa/inet.h>  // inet_addr头文件
#include <netinet/in.h> // sockaddr_in头文件
#include <sys/select.h> // select头文件
#include <sys/socket.h> // SOCKET相关头文件
#include "DBHelper.h"   // 数据库操作头文件

typedef int SOCKET;
#define INVALID_SOCKET -1

class SelectServer
{
public:
	SelectServer(const std::string& ip = "0.0.0.0", unsigned short port = 9668);
	~SelectServer();
	// Select服务器初始化，socket初始化
	bool SelectInit();
	// 开启服务器，开启select服务
	bool Start();
	// 停止服务器运行
	void Stop();
	// 对客户端发送过来的数据解析和处理
	bool Dispose(SOCKET clientSocket);
	// 找出所有发生事件的客户端
	bool FindActiveClient(std::vector<SOCKET>& activeFds);
	// 连接客户端
	bool Accept();
private:
	// 清空所有数据信息
	void Clear();
	// 设置select的监听套接字，通过m_fds进行辅助设置
	bool SetRdSet();
private:
	SOCKET m_serverSocket;     // 服务器socket，listenSocket
	sockaddr_in m_serverAddr;  // 服务器地址绑定信息
	std::vector<SOCKET> m_fds; // 保存连接上来的客户端，包括服务器套接字
	int m_maxFd;               // 最大的文件描述符值，select所用
	fd_set m_rdset;            // 读事件，用于select监听
	bool m_bServerInvalid;     // 服务器是否有效 true表示有效 false表示无效
};