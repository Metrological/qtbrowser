#include "sslhandler.h"

#include <QDebug>

SSLSlotHandler::SSLSlotHandler(){}
SSLSlotHandler::~SSLSlotHandler(){}

void SSLSlotHandler::sslError(QNetworkReply* qnr, const QList<QSslError> & errlist)
{
    foreach (QSslError err, errlist)
        qDebug() << "[ssl error]" << err;

    qnr->ignoreSslErrors();
}


