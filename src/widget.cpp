#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include "packet.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog> // QFileDlg头文件
#include <process.h>  // 创建线程相关头文件


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_isInvalid(true)
    , m_recverAddr("127.0.0.1")
    , m_recverPort(14252)
    // , AddrInfo(this)
{
    ui->setupUi(this);
    // ui->MSG_TE->setLineWrapMode();
    ui->LE_IPADDR->setText(m_recverAddr.toString());
    ui->LE_PORT->setText("14252");
    m_AddrInfo.SetIP("0.0.0.0");
    m_AddrInfo.SetAddrPort("12345");
    m_addr = m_AddrInfo.GetIP();
    m_port = m_AddrInfo.GetPort();
    InitSocket();
    m_hThread = (HANDLE)_beginthread(recvThreadEny, 0, this);
    // 将信号和槽进行关联
    connect(&m_AddrInfo, &LocalAddressInfo::UpdateAddr, this, &Widget::UpdateAddr);
    // connect(&m_AddrInfo, SIGNAL(UpdateAddr(bool)), this, SLOT(UpdateAddr(bool)));
    connect(this, &Widget::CreateMessageDlg, this, &Widget::on_MessageDlg);
}

Widget::~Widget()
{
    m_isInvalid = false;
    delete ui;
    WaitForSingleObject(m_hThread, INFINITE);
    m_sock.close();
}

void Widget::on_BUT_SEND_clicked(bool checked)
{
    qDebug() << "" << checked;
    QString chatBuf = ui->TChat->toPlainText();
    if(chatBuf == "") {
        // QDialog InfoDlg(this);
        QMessageBox::question(nullptr, "错误", "当前没有要发送的信息", QMessageBox::Yes);
        // InfoDlg.exec();
        return;
    }
    ui->TChat->setText("");
    QNetworkDatagram Data(chatBuf.toUtf8(), m_recverAddr, m_recverPort);

    qint64 ret = m_sock.writeDatagram(Data);
    if(ret < 0){
        qDebug() << m_sock.errorString();
        // QMessageBox::question(nullptr, "", QString(""), QMessageBox::Yes);
        QMessageBox::question(nullptr, "错误", "消息发送失败，请检测网络是否连接", QMessageBox::Yes);
        qDebug() << "info send fail, ret = " << ret;
        return;
    }
    qDebug("处理完成 ret = %d\n", ret);

    return;
}


void Widget::on_AddrChange_clicked(bool checked)
{
    m_AddrInfo.show();
}

void Widget::UpdateAddr(bool BChanged)
{
    qDebug() << " BChanged=" << BChanged;
    if(BChanged){
        m_isAddrUpdate = true;
        // IP地址信息
        m_addr = m_AddrInfo.GetIP();
        m_port = m_AddrInfo.GetPort();
        InitSocket();
        m_isAddrUpdate = false;
    }
    m_AddrInfo.hide();
}

bool Widget::InitSocket()
{
    if(m_sock.isValid()){ // isValid判断套接字是否有效，true表示有效 false表示无效
        m_sock.close();
    }
    if(false == m_sock.bind(m_addr, m_port)){
        QMessageBox::question(nullptr, "错误", "当前端口已占用，运行程序后务必自行修改程序端口", QMessageBox::Yes);
        return FALSE;
    }
    return TRUE;
}

void Widget::recvThreadEny(void *arg)
{
    Widget* thiz  = (Widget*)arg;
    thiz->recvThreadMain();
    _endthread();
}

void Widget::recvThreadMain()
{
    qDebug() << "thread start";
    int count = 0;
    // Sleep(3000);
    // emit CreateMessageDlg("消息", QString("接收文件成功"), QMessageBox::Yes);

    while(m_isInvalid){
        if(!m_isAddrUpdate){
            if(m_sock.hasPendingDatagrams()){

                char buf[512] = "";
                QHostAddress recvIPAddr;
                qint64 ret = m_sock.readDatagram(buf, sizeof(buf), &recvIPAddr);
                if(ret > 0){
                    qDebug() << "index message";
                    FileInfo fi(buf, ret);
                    if(fi.GetHead()==0XFEFF){
                        // QMessageBox::question(this, "消息", QString("接收文件成功"), QMessageBox::Yes); // 这块会产生错误，因为不是GUI的线程
                        emit CreateMessageDlg("消息", QString("接收文件成功"), QMessageBox::Yes);
                    }
                    else{
                        QString info = "<";
                        info = info + recvIPAddr.toString() + ">: " + buf;
                        ui->MSG_TE->append(info);
                    }
                }
                else if(ret < 0){
                    Sleep(500);
                    count++;
                    if(count >= 10){
                        qDebug() << "read error:" << m_sock.errorString();
                        // QMessageBox::question(nullptr, "错误", QString("消息接收失败"), QMessageBox::Yes);
                        break;
                    }
                }
            }
        }
        Sleep(1);
    }
    qDebug() << "thread end";
}

void Widget::on_LE_IPADDR_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    // m_recverAddr = arg1;
    m_recverAddr.setAddress(arg1);
}


void Widget::on_LE_PORT_textChanged(const QString &arg1)
{

    m_recverPort = arg1.toUShort();
    qDebug() << m_recverPort;
}

void Dump(const std::string& str)
{
    const char* pStr = str.c_str();
    for(int i=0;i<str.size();i+=4){
        qDebug("%04X", *(pStr + i));
    }
}


void Widget::on_BUT_SENDFILE_clicked(bool checked)
{
    //    QString Path = FileDlg.getOpenFileName(this, "选择要发送的文件", "C:\\"); // 这种方式不能修改按钮的文本内容
    //    qDebug() << Path;
    QFileDialog FileDlg(this, "选择要发送的文件", "C:\\"); // 打开文件对话框，让用户选择要发送的文件
    FileDlg.setLabelText(QFileDialog::Accept, "选择");  // 设置Accept按钮的显示文本内容
    int ret = FileDlg.exec();
    qDebug("ret = %d",ret);
    if(ret){
        QStringList Paths = FileDlg.selectedFiles();
        if(Paths.isEmpty() == false){
            QString Path = Paths.at(0);
            qDebug() << Path;
            FileInfo fi(Path.toUtf8().data());
            std::string str = fi.toString();
            // Dump(fi.toString());
            QByteArray arr(str.c_str(), str.size());
            QNetworkDatagram Data(arr, m_recverAddr, m_recverPort);
            qint64 ret = m_sock.writeDatagram(Data);
            if(ret>0){

            }
            // 将文件进行发送
        }
    }

}

void Widget::on_MessageDlg(const QString &title, const QString &text, int button0)
{
    QMessageBox::question(this, title, text, button0);
}

