#include "SelectServer.h"


SelectServer::SelectServer(const std::string& ip, unsigned short port)
	: m_serverSocket(INVALID_SOCKET)
	, m_maxFd(INVALID_SOCKET)
	, m_bServerInvalid(true)
{
	memset(&m_serverAddr, 0, sizeof(m_serverAddr));
	FD_ZERO(&m_rdset); // 使用给的宏清空
	// memset(&m_rdset, 0, sizeof(m_rdset));
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(port);
	m_serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
	if (SelectInit() == false) {
		printf("服务器初始化失败\n");
	}
}

SelectServer::~SelectServer()
{
	m_bServerInvalid = false;
	Clear();
}

bool SelectServer::SelectInit()
{
	if (m_serverSocket != INVALID_SOCKET)
		return false;

	// 创建套接字
	m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_serverSocket < 0) {
		printf("服务器套接字创建失败\n");
		m_serverSocket = INVALID_SOCKET;
		return false;
	}
	// 绑定端口
	if (bind(m_serverSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) < 0) {
		printf("服务器端口绑定失败\n");
		Clear();
		return false;
	}
	// 监听套接字，使套接字fd变为lfd
	if (listen(m_serverSocket, 5) < 0) {
		printf("服务器套接字监听失败\n");
		Clear();
		return false;
	}
	m_maxFd = m_serverSocket;
	m_fds.push_back(m_serverSocket); // 服务器套接字单独监听
	return true;
}

bool SelectServer::Start()
{
	printf("selectserver start\n");
	while (m_bServerInvalid) {
		// 设置rdset
		if (false == SetRdSet()) {
			break;
		}
		int ret = select(m_maxFd + 1, &m_rdset, NULL, NULL, NULL); // timeout为NULL表示无限等待
		if (ret < 0) {
			break;
		}
		if (FD_ISSET(m_serverSocket, &m_rdset) == true) {
			// 有连接请求
			printf("[info]:accept client\n");
			if (false == Accept()) {
				// 警告，客户端连接失败
			}
		}
		std::vector<SOCKET> activeFds;
		FindActiveClient(activeFds);
		if (activeFds.size() > 0) {
			// 表示有客户端发送消息过来
			for (size_t i = 0; i < activeFds.size(); i++) {
				if (false == Dispose(activeFds[i])) {
					// 警告，消息处理失败
				}
			}
		}
	}
	Clear();
	return false;
}

void SelectServer::Stop()
{
	m_bServerInvalid = false;
}

bool SelectServer::Dispose(SOCKET clientSocket)
{
	return false;
}

bool SelectServer::FindActiveClient(std::vector<SOCKET>& activeFds)
{
	// 服务器套接字不需要管，因为它单独处理
	return false;
}

bool SelectServer::Accept()
{
	return false;
}

void SelectServer::Clear()
{
	// 清空所有连接上来的客户端
	if (m_fds.size() > 0) {
		for (size_t i = 0; i < m_fds.size(); i++) {
			close(m_fds[i]);
		}
		m_fds.clear();
	}
	// 关闭服务器套接字
	if (m_serverSocket != INVALID_SOCKET) {
		SOCKET temp = m_serverSocket;
		m_serverSocket = INVALID_SOCKET;
		close(temp);
	}
	m_maxFd = -1;
	memset(&m_rdset, 0, sizeof(m_rdset));
	memset(&m_serverAddr, 0, sizeof(m_serverAddr));
}

bool SelectServer::SetRdSet()
{
	FD_ZERO(&m_rdset); // 使用给的宏清空
	if (m_fds.size() <= 0)return false;
	for (size_t i = 0; i < m_fds.size(); i++) {
		FD_SET(m_fds[i], &m_rdset);
	}
	return true;
}
