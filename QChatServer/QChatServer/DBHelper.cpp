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
	m_user = user;
	m_password = password;
	m_dbName = dbName;
	return true;
}

bool DBHelper::ChangeUser(const char* user, const char* password, const char* dbName)
{
	if (user == NULL && dbName == NULL)return false;
	// 临时当前用户和数据库，防止调用者要改变的用户或者数据库不成功，导致类中成员出现问题所置
	std::string _user(m_user), _password(m_password), _dbName(m_dbName);
	bool change = false; // 用户及数据库是否改变 false没有改变 true发生改变
	if (user && std::string(user) != _user) {
		// 需要更新用户
		if (password) {
			_user = user;
			_password = password;
			change = true;
		}
	}
	if (dbName && std::string(dbName) != _dbName) {
		_dbName = dbName;
		change = true;
	}
	if (change) {
		if (0 == mysql_change_user(&m_mysql, _user.c_str(), _password.c_str(), _dbName.c_str())) {
			m_user = _user;
			m_password = _password;
			m_dbName = _dbName;
		}
		else {
			// 改变用户或者数据库失败
			ShowError();
			return false;
		}
	}
	return true;
}

bool DBHelper::Execute(const std::string& sql)
{
	if (sql.size() <= 0)return false;
	if (mysql_real_query(&m_mysql, sql.c_str(), sql.size())) {
		// 执行失败
		ShowError();
		return false;
	}
	return true;
}

const ResultRecord* const DBHelper::GetResult()
{
	// 获取结果集
	MYSQL_RES* res = mysql_store_result(&m_mysql);
	if (res == NULL) {
		ShowError();
		return NULL;
	}
	ResultRecord* resRecord = new ResultRecord();
	if (resRecord == NULL)return NULL;
	resRecord->LoadResult(res);
	mysql_free_result(res);
	return resRecord;
}

void DBHelper::FreeResult(const ResultRecord* const result)
{
	if (result == NULL)return;
	delete result;
}

void DBHelper::ShowError()
{
	unsigned int Errno = mysql_errno(&m_mysql);
	if (Errno) {
		printf("[error]:(%d)<%s>\r\n", Errno, mysql_error(&m_mysql));
	}
}
