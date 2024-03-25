#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <Windows.h>
#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/qudpsocket.h> // UDP头文件
#include "localaddressinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots: // 槽函数
    // 用户点击发送按钮
    void on_BUT_SEND_clicked(bool checked);
    // 用户点击地址改变按钮
    void on_AddrChange_clicked(bool checked);
    // 用户改变客户端的IP地址信息
    void UpdateAddr(bool BChanged);
    // IP地址文本框内容发生改变
    void on_LE_IPADDR_textChanged(const QString &arg1);
    // port端口文本框内容发生改变
    void on_LE_PORT_textChanged(const QString &arg1);

    void on_BUT_SENDFILE_clicked(bool checked);

private:
    bool InitSocket(); // 初始化套接字
protected:
    static void recvThreadEny(void* arg);
    void recvThreadMain();
private:
    Ui::Widget *ui;
    LocalAddressInfo m_AddrInfo;
    HANDLE m_hThread;
    bool m_isInvalid; // 当前对话框是否有效
    bool m_isAddrUpdate; // 客户端地址信息，是否发生更新 false表示否 true表示是
    QUdpSocket m_sock; // UDP套接字
    QHostAddress m_addr; // 本客户端的IP地址
    quint16 m_port;  // 本客户端的端口
    QHostAddress m_recverAddr; // 接收者的IP地址
    quint16 m_recverPort;    // 接收者的端口
};
#endif // WIDGET_H
