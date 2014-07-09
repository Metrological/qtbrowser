#ifndef __SSLHANDLER__
#define __SSLHANDLER__

#include <QNetworkReply>
#include <QList>
#include <QSslError>

class SSLSlotHandler : public QObject
{
Q_OBJECT
public:
    SSLSlotHandler();
    virtual ~SSLSlotHandler();

public slots:
    void sslError(QNetworkReply* qnr, const QList<QSslError> & errlist);
};

#endif // __SSLHANDLER__
