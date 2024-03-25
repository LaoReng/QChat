#ifndef LOCALADDRESSINFO_H
#define LOCALADDRESSINFO_H

#include <QWidget>

namespace Ui {
class LocalAddressInfo;
}
// 客户端的地址信息
class LocalAddressInfo : public QWidget
{
    Q_OBJECT

public:
    explicit LocalAddressInfo(QWidget *parent = nullptr);
    ~LocalAddressInfo();
    // 设置IP属性
    void SetIP(const QString& ip);
    // 获取IP
    QString GetIP() const;
    // 设置端口值
    void SetAddrPort(const QString& port);
    // 获取端口值
    unsigned short GetPort() const;
signals:
    void UpdateAddr(bool BChanged);

private slots:
    void on_TEPort_textChanged();

    void on_BTNConfirm_clicked(bool checked);

    void on_BTNCancel_clicked(bool checked);

private:
    Ui::LocalAddressInfo *ui;
    QString m_ip;
    unsigned short m_port;
};

#endif // LOCALADDRESSINFO_H
