#include "localaddressinfo.h"
#include "ui_localaddressinfo.h"
#include <QDebug>
#include <QString>
#include <QtNetwork/QHostInfo>

LocalAddressInfo::LocalAddressInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalAddressInfo)
{
    ui->setupUi(this);
    // QHostInfo ipInfo = (QHostInfo::fromName(QHostInfo::localHostName()));
    // qDebug()<<ipInfo.addresses();
    ui->TEIpInfo->setText("0.0.0.0");
    ui->TEPort->setText("12345");
}

LocalAddressInfo::~LocalAddressInfo()
{
    delete ui;
}

void LocalAddressInfo::SetIP(const QString &ip)
{
    m_ip = ip;
    ui->TEIpInfo->setText(m_ip);
}

QString LocalAddressInfo::GetIP() const
{
    return m_ip;
}

void LocalAddressInfo::SetAddrPort(const QString &port)
{
    // TODO:处理比如说IP地址不能大于5位
    for(int i=0;i<port.size();i++){
        if(port[i]<'0'||port[i]>'9'){
            qDebug() << "port info is bad";
            // qDebug("端口内容不符合要求");
            return;
        }
    }
    m_port = port.toUShort();
    ui->TEPort->setText(port);
}

unsigned short LocalAddressInfo::GetPort() const
{
    return m_port;
}

void LocalAddressInfo::on_TEPort_textChanged()
{
    // 执行只能输入数字的检查
    QString port = ui->TEPort->toPlainText();
    if(port.size()<=0)return;
    QString _port;
    int index = 0; // 保存错误输入时光标的位置
    for(int i = 0; i < port.size(); i++){
        if(port[i] >= '0' && port[i] <= '9'){
            _port += port[i];
        }
        else {
            index = i;
        }
    }
    if(port != _port){
        // 如果有非数字字符，设置文本框中的值，并设置光标位置
        QTextCursor tmpCursor = ui->TEPort->textCursor();
        ui->TEPort->setText(_port);
        tmpCursor.setPosition(index); // 设置光标位置为输入非数字字符时的位置
        ui->TEPort->setTextCursor(tmpCursor);
    }
}


void LocalAddressInfo::on_BTNConfirm_clicked(bool checked)
{
    QString ip = ui->TEIpInfo->toPlainText();
    unsigned short port = ui->TEPort->toPlainText().toUShort();
    if(ip == m_ip && port == m_port){
        // 没有改变
        emit UpdateAddr(false);
        return;
    }
    m_ip = ip;
    m_port = port;
    // 这里要自定义信号和槽，去通知主对话框
    emit UpdateAddr(true);
}


void LocalAddressInfo::on_BTNCancel_clicked(bool checked)
{
    emit UpdateAddr(false);
}

