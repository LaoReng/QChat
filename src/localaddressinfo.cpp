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
    // TODO:�������˵IP��ַ���ܴ���5λ
    for(int i=0;i<port.size();i++){
        if(port[i]<'0'||port[i]>'9'){
            qDebug() << "port info is bad";
            // qDebug("�˿����ݲ�����Ҫ��");
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
    // ִ��ֻ���������ֵļ��
    QString port = ui->TEPort->toPlainText();
    if(port.size()<=0)return;
    QString _port;
    int index = 0; // �����������ʱ����λ��
    for(int i = 0; i < port.size(); i++){
        if(port[i] >= '0' && port[i] <= '9'){
            _port += port[i];
        }
        else {
            index = i;
        }
    }
    if(port != _port){
        // ����з������ַ��������ı����е�ֵ�������ù��λ��
        QTextCursor tmpCursor = ui->TEPort->textCursor();
        ui->TEPort->setText(_port);
        tmpCursor.setPosition(index); // ���ù��λ��Ϊ����������ַ�ʱ��λ��
        ui->TEPort->setTextCursor(tmpCursor);
    }
}


void LocalAddressInfo::on_BTNConfirm_clicked(bool checked)
{
    QString ip = ui->TEIpInfo->toPlainText();
    unsigned short port = ui->TEPort->toPlainText().toUShort();
    if(ip == m_ip && port == m_port){
        // û�иı�
        emit UpdateAddr(false);
        return;
    }
    m_ip = ip;
    m_port = port;
    // ����Ҫ�Զ����źźͲۣ�ȥ֪ͨ���Ի���
    emit UpdateAddr(true);
}


void LocalAddressInfo::on_BTNCancel_clicked(bool checked)
{
    emit UpdateAddr(false);
}

