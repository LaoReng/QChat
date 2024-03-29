#include "packet.h"

Packet::Packet()
{

}

FileInfo::FileInfo(const char *path, const char *data, size_t len)
    : m_head(0XFEFF)
    , m_dataLength(0)
{
    if(path==NULL)return;
    memset(m_filePath, 0, sizeof(m_filePath));
    memcpy(m_filePath, path, strlen(path));
    // strcpy(m_filePath, path);
    if(data && len > 0){
        m_dataLength = len;
        m_fileData.resize(m_dataLength);
        memcpy((char*)m_fileData.c_str(), data, m_fileData.size());
    }
}

FileInfo::FileInfo(const char *pack, long long len)
    : m_head(0)
    , m_filePath("")
    , m_dataLength(0)
{
    if(pack==NULL||len<=0)return;
    int i = 0;
    for(;i<len;i++){
        if(*(unsigned short*)(pack+i)==0XFEFF){
            break;
        }
    }
    i += sizeof(m_head);
    if(i + sizeof(m_filePath) + sizeof(m_dataLength)>len){
        return;
    }
    memcpy(m_filePath, pack + i, sizeof(m_filePath));
    i += sizeof(m_filePath);
    m_dataLength = *(int*)(pack+i);
    i += sizeof(m_dataLength);
    if(i+m_dataLength > len){
        return;
    }
    if(m_dataLength>0){
        m_fileData.resize(m_dataLength);
        memcpy((char*)m_fileData.c_str(), pack+i, m_fileData.size());
    }
    m_head = 0XFEFF;
}

std::string& FileInfo::toString()
{
    m_PackData.resize(sizeof(m_head)+sizeof(m_filePath)+sizeof(m_dataLength)+m_dataLength);
    char *pData = (char*)m_PackData.c_str();
    int i=0;
    memcpy(pData + i, &m_head, sizeof(m_head));
    i += sizeof(m_head);
    memcpy(pData+i, m_filePath, sizeof(m_filePath));
    i += sizeof(m_filePath);
    memcpy(pData+i, &m_dataLength, sizeof(m_dataLength));
    i += sizeof(m_dataLength);
    if(m_dataLength>0){
        memcpy(pData+i, m_fileData.c_str(), m_fileData.size());
    }
    return m_PackData;
}

unsigned short FileInfo::GetHead() const
{
    return m_head;
}
