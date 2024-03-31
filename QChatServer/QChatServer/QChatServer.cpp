// QChatServer.cpp: 定义应用程序的入口点。
//

#include "QChatServer.h"
#include "SelectServer.h"

using namespace std;

int test_db()
{
	DBHelper db;
	// 连接数据库
	if (false == db.Connect("root", "123", "test")) {
		return 0;

	}
	std::cout << "连接成功" << std::endl;
	// 改变数据库连接
	/*if (true == db.ChangeUser(NULL, NULL, "mysql")) {
		std::cout << "改变成功" << std::endl;
	}*/
	if (false == db.Execute("SELECT * FROM userInfo")) {
		return 0;
	}
	std::cout << "执行成功" << std::endl;
	const ResultRecord* const res = db.GetResult();
	if (res == NULL) {
		printf("获取查询结果失败\n");
		return 0;
	}
	const FieldInfos fields = res->GetFieldInfos();
	for (size_t i = 0; i < fields.size(); i++) {
		printf("%s\t\t", fields[i].field_name.c_str());
	}
	printf("\n");
	const Records records = res->GetRecords();
	for (size_t i = 0; i < records.size(); i++) {
		for (size_t j = 0; j < records[i].size(); j++) {
			printf("%s\t\t", records[i][j].c_str());
		}
		printf("\n");
	}
	db.FreeResult(res);
	return 0;
}

int main()
{
	// test_db();
	SelectServer server;
	server.Start();
	// server.Stop();
	return 0;
}
