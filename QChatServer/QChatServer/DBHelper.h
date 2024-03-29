#pragma once
#include <iostream>
#include <string>
#include <mysql/mysql.h>

class DBHelper
{
public:
	DBHelper();
	~DBHelper();
	// �������ݿ�
	bool Connect(
		const std::string& user, const std::string& password,
		const std::string& dbName, const std::string& host = "localhost",
		unsigned int port = 3306
	);
	// �ı䵱ǰ���ӵ����ݿ�
	// ִ��SQL���
	// ��ȡ��ѯ���
private:
	// ��ʾ���ݿ������Ϣ
	void ShowError();

private:
	MYSQL m_mysql;
};