#include "qdatadispactherthread.h"

QDataDispactherThread::QIntProcessorHash QDataDispactherThread::hashProcessor;
QMutex QDataDispactherThread::mutexHashProcessor;

QDataDispactherThread::QDataDispactherThread(QObject *parent) :
    QThread(parent)
{
    pConfig = CDbConfigurator::GetConfigurator( );
    nDbThreadCount = pConfig->GetDbThreadCount( );
    nDbThreadOperationCount = pConfig->GetDbThreadOperationCount( );
    nDbThreadInvalidWaitTime = pConfig->GetDbThreadInvalidWaitTime( );
    setObjectName( QString( "[Data Dispatcher Thread ID = %1]" ).arg( qrand( ) ) );
}
QDataDispactherThread::~QDataDispactherThread( )
{
    FreeDbThread( );
}

QDataDispactherThread* QDataDispactherThread::CreateThread( QObject *parent )
{
    QDataDispactherThread* pThread = new QDataDispactherThread( parent );

    pThread->start( );
    pThread->moveToThread( pThread );

    qDebug( ) << Q_FUNC_INFO << endl;

    return pThread;
}

void QDataDispactherThread::HandleLog( QString strLog, bool bStatic )
{
    SendLog( strLog, bStatic );
}

void QDataDispactherThread::SendLog( QString &strLog, bool bStatic )
{
    emit Log( strLog, bStatic );
}

void QDataDispactherThread::run( )
{
    QString strLog = objectName( ) + " Started.";
    SendLog( strLog, true );
    InitializeSubThread( );
    exec( );
}

void QDataDispactherThread::customEvent( QEvent *event )
{
    QDbThreadEvent* pEvent = ( QDbThreadEvent* ) event;
    QDbThreadEvent::EventType evtType = ( QDbThreadEvent::EventType ) pEvent->type( );

    if ( QDbThreadEvent::EventDispatchData == evtType ) {
        ProcessDispatchDataEvent( pEvent );
    }
}

void QDataDispactherThread::FreeDbThread( )
{
    static bool bDestroyed = false;
    if ( bDestroyed ) {
        return;
    }

    bDestroyed = true;

    QProcessorList lstThread;
    QDatabaseProcessor* pThread = NULL;

    for ( qint32 nIndex = nDbThreadOperationCount; nIndex >= 0; nIndex-- ) {
        lstThread = hashProcessor.values( nIndex );
        if ( 0 == lstThread.length( ) ) {
            continue;
        }

        foreach ( const QDatabaseProcessor* p, lstThread ) {
            pThread = const_cast< QDatabaseProcessor* >( p );
            pThread->AcquireProcessor( nDbThreadOperationCount ); // Fish All Operations
            pThread->deleteLater( );
        }
    }

    hashProcessor.clear( );
}

QDatabaseProcessor* QDataDispactherThread::CreateOneDbThread( )
{
    QDatabaseProcessor* pThread = QDatabaseProcessor::CreateThread( );
    connect( pThread, SIGNAL( Log( QString, bool ) ),
             this, SLOT( HandleLog( QString, bool ) ) );
    hashProcessor.insertMulti( nDbThreadOperationCount, pThread );
    //pThread->PostDbConnectEvent( );

    sleep( 2 );

    return pThread;
}

void QDataDispactherThread::CreateDbThread(  )
{
    static bool bCreated = false;
    if ( bCreated ) {
        return;
    }

    bCreated = true;
    QDatabaseProcessor* pThread = NULL;

    // Hash Key 0 to nDbThreadCount - 1
    for ( qint32 nIndex = 0; nIndex < nDbThreadCount; nIndex ++ ) {
        pThread = CreateOneDbThread( );
    }
    Q_UNUSED( pThread )
}

// GetProcessor call
void QDataDispactherThread::ChangeProcessorHash( qint32 nIndex, QDatabaseProcessor *pThread )
{
    hashProcessor.remove( nIndex, pThread );
    qint32 nCount = pThread->GetFreeOperationCount( );
    hashProcessor.insertMulti( nCount, pThread );
}

void QDataDispactherThread::ChangeProcessorHash( )
{
    QProcessorList lstThread;
    QDatabaseProcessor* pThread = NULL;
    qint32 nCount = 0;

    for ( qint32 nIndex = 0; nIndex < nDbThreadOperationCount; nIndex++ ) {
       lstThread = hashProcessor.values( nIndex );
       if ( 0 == lstThread.length( ) ) {
           continue;
       }

       foreach ( const QDatabaseProcessor* pInvalidedThread, lstThread ) {
           pThread = const_cast< QDatabaseProcessor* >( pInvalidedThread );
           nCount = pThread->GetFreeOperationCount( );

           if ( nIndex == nCount ) {
               continue;
           }

           ChangeProcessorHash( nIndex, pThread );
       }
    }
}

QDatabaseProcessor* QDataDispactherThread::GetProcessor( )
{
    QMutexLocker mutexLocker( &mutexHashProcessor ); // Locked

    QDatabaseProcessor* pThread = NULL;
    QProcessorList lstThread;

    ChangeProcessorHash( );

    for ( qint32 nIndex = nDbThreadOperationCount; nIndex >= 0; nIndex-- ) {
        lstThread = hashProcessor.values( nIndex );
        if ( 0 == lstThread.length( ) ) {
            continue;
        }

        pThread = lstThread.at( 0 );

        if ( 0 < nIndex ) {
            pThread->AcquireProcessor( 1 );
            ChangeProcessorHash( nIndex, pThread );
            break;
        } else {
            foreach ( const QDatabaseProcessor* pInvalidedThread, lstThread ) {
                pThread = const_cast< QDatabaseProcessor* >( pInvalidedThread );

                if ( pThread->TryAcquireProcessor( nDbThreadInvalidWaitTime ) ) {
                    ChangeProcessorHash( nIndex, pThread );
                    break;
                }

                pThread= NULL;
            }
        }
    }

    // Unlocked when return / Descontructor

    if ( NULL == pThread ) {
        // when pThread is NULL, no processor may be used
        pThread = CreateOneDbThread( );
    }

    pThread->SetDataDispatcher( this );

    return pThread;
}

void QDataDispactherThread::ProcessDispatchDataEvent( QDbThreadEvent *pEvent )
{
    QTcpSocket* pSocket = pEvent->GetPeerSocket( );
    qint32 nPackageType = pEvent->GetDataPackageType( );
    QByteArray& byData = pEvent->GetByteArray( );

    // Producer
    // MySQL Server else ThreadPool
    QDatabaseProcessor* pThread = GetProcessor( );
    pThread->PostDbDataProcessEvent( pSocket, nPackageType, byData );
}

void QDataDispactherThread::InitializeSubThread( )
{
    CreateDbThread( );
}

void QDataDispactherThread::PostEvent( QDbThreadEvent *pEvent )
{
    qApp->postEvent( this, pEvent );
}

void QDataDispactherThread::PostDispatchDataEvent( QTcpSocket* pSocket, qint32 nPackageType, QByteArray &byData )
{
    QDbThreadEvent* pEvent = QDbThreadEvent::CreateThreadEvent( QDbThreadEvent::ThreadDispatcher, QDbThreadEvent::EventDispatchData );

    pEvent->SetDataPackageType( nPackageType );
    pEvent->SetByteArray( byData );
    pEvent->SetPeerSocket( pSocket );

    PostEvent( pEvent );
}
