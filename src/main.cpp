#pragma execution_character_set("utf-8")

#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <QString>
#include <QtNetwork/QHostInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    QHostInfo ipInfo = QHostInfo::fromName(QHostInfo::localHostName());
    qDebug()<<ipInfo.addresses();
    w.show();
    return a.exec();
}
