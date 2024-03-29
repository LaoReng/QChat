// QChatServer.cpp: 定义应用程序的入口点。
//

#include "QChatServer.h"
#include "DBHelper.h"

using namespace std;

int main()
{
	DBHelper db;
	if (true == db.Connect("root", "123", "test")) {
		std::cout << "连接成功" << std::endl;
	}
	return 0;
}
