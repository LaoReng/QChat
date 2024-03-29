#include "DBHelper.h"

DBHelper::DBHelper()
{
	if (NULL == mysql_init(&m_mysql)) {
		// mysql初始化失败
		ShowError();
	}
}

DBHelper::~DBHelper()
{
	mysql_close(&m_mysql);
}

bool DBHelper::Connect(const std::string& user, const std::string& password, const std::string& dbName, const std::string& host, unsigned int port)
{
	if (NULL == mysql_real_connect(&m_mysql, host.c_str(), user.c_str(), password.c_str(), dbName.c_str(), port, NULL, 0)) {
		// 数据库连接失败
		ShowError();
		return false;
	}
	return true;
}

void DBHelper::ShowError()
{
	unsigned int Errno = mysql_errno(&m_mysql);
	if (Errno) {
		printf("[error]:(%d)<%s>\r\n", Errno, mysql_error(&m_mysql));
	}
}
