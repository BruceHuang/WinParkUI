#ifndef NETWORKCONTROLLER_H
#define NETWORKCONTROLLER_H

#include "NetworkLibrary_global.h"
#include <QObject>
#include "qlistenerthread.h"

class NetworkController : public QObject
{
    Q_OBJECT
public:
    static NetworkController* GetController( QObject* parent = NULL );
    static void ReleaseController( );

    void StartListen( quint16 nPort, qint32 nMaxConn );
    void StopListen( );

private:
    NetworkController( QObject* parent );
    ~NetworkController( );

    QListenerThread* pListenerThread;
    static NetworkController* pController;

signals:
    void Log( QString strLog, bool bStatic );

public slots:

private slots:
    void HandleLog( QString strLog, bool bStatic );
};

#endif // NETWORKCONTROLLER_H
