#pragma once
#include <iostream>
#include <string>
#include <mysql/mysql.h>

class DBHelper
{
public:
	DBHelper();
	~DBHelper();
	// 连接数据库
	bool Connect(
		const std::string& user, const std::string& password,
		const std::string& dbName, const std::string& host = "localhost",
		unsigned int port = 3306
	);
	// 改变当前连接的数据库
	// 执行SQL语句
	// 获取查询结果
private:
	// 显示数据库错误信息
	void ShowError();

private:
	MYSQL m_mysql;
};