#ifndef PACKET_H
#define PACKET_H
#include <iostream>
#include <QString>

class FileInfo
{
public:
    // 封包
    FileInfo(const char* path, const char* data = NULL,  size_t len = 0);
    // 解包
    FileInfo(const char* pack, long long len);
    // 转换为字符串形式
    std::string& toString();
    // 获取包头，如果解包后包头不是0XFEFF则表示解包失败
    unsigned short GetHead() const;
private:
    unsigned short m_head = 0XFEFF;
    char m_filePath[256];
    int m_dataLength;
    std::string m_fileData;
    std::string m_PackData;  // 整个数据包的数据
};

class Packet
{
public:
    Packet();
};

#endif // PACKET_H
