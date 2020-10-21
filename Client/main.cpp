#include <QCoreApplication>
#include "client.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client( "127.0.0.1", 1234);
    QString l_service_name = QString("calculater");
    QString l_method_name = QString("sumGreaterThan100");
    QVector<int> values = {15, 20, 35, 12, 45};
    QString hata;
    bool flag;
    int sum;
    client.beginCall(l_service_name, l_method_name, values);
    client.endCall(flag, hata, sum);
    qDebug() << "hata:" << hata;
    qDebug() << "flag:" << flag;
    qDebug() << "sum:" << sum;

    return a.exec();
}
