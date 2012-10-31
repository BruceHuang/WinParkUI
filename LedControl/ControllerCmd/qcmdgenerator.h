#ifndef QCMDGENERATOR_H
#define QCMDGENERATOR_H

#include <QObject>
#include "../ControllerCommon/CommonHead.h"

class QCmdGenerator : public QObject
{
    Q_OBJECT
public:
    explicit QCmdGenerator(QObject *parent = 0);

    void GetCmdBody( QByteArray& body, LedControll::ECommand eCmd, qint32 nParam = 0 );

private:
    inline void GetFlashStateAlwaysRadianceChane( QByteArray& body );
    inline void GetFlashFrenquencyIntensityTune( const qint16 nTune, QByteArray& body );
    inline void GetFlashGearSet( const qint8 nGear, QByteArray& body );
    inline void GetFrenquencyGearSet( const qint8 nGear, QByteArray& body );
    inline void GetFlashFrenquencyGearClose( const qint32 nClose, QByteArray& body );
    inline void GetFlashGearAlwaysRadianceClose( const qint32 nClose, QByteArray& body );
    inline void GetFlashGearAlwaysRadianceOpen( const qint32 nOpen, QByteArray& body );
    inline void GetFlashFrenquencyGearWorkTimeSet( const qint32 nTime, QByteArray& body );
    inline void GetFlashFrenquencyLightSensitiveIfWork( const qint32 nWork, QByteArray& body );
    inline void GetSyncModeCmd( const qint8 nMode, QByteArray& body );
    
signals:
    
public slots:
    
};

#endif // QCMDGENERATOR_H
