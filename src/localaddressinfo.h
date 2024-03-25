#ifndef LOCALADDRESSINFO_H
#define LOCALADDRESSINFO_H

#include <QWidget>

namespace Ui {
class LocalAddressInfo;
}
// �ͻ��˵ĵ�ַ��Ϣ
class LocalAddressInfo : public QWidget
{
    Q_OBJECT

public:
    explicit LocalAddressInfo(QWidget *parent = nullptr);
    ~LocalAddressInfo();
    // ����IP����
    void SetIP(const QString& ip);
    // ��ȡIP
    QString GetIP() const;
    // ���ö˿�ֵ
    void SetAddrPort(const QString& port);
    // ��ȡ�˿�ֵ
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
