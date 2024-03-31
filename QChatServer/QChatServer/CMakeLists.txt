# CMakeList.txt: QChatServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 找到MySQL开发库
# find_library(MYSQL_LIB libmysqlclient.so /usr/lib/mysql/)
#if(NOT MUSQL_LIB)
#	message(FATAL_ERROR "not found mysqlclient.so at lib64")
#endif(NOT MYSQL_LIB)

# 包含MySQL头文件目录
# 设置库目录
#link_directories(
#	${PROJECT_SOURCE_DIR}/lib
#	/usr/lib64/mysql/
#)
# include_directories(${MYSQL_INCLUDE_DIR})

set(srcs
QChatServer.cpp
DBHelper.cpp
SelectServer.cpp
)



# 将源代码添加到此项目的可执行文件。
add_executable (QChatServer ${srcs})

# 链接MySQL客户端库
target_link_libraries(QChatServer mysqlclient)

# TODO: 如有需要，请添加测试并安装目标。
