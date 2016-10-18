/*

  ScanManager class to process scanning functionality.

*/

#include <QProcess>
//#include <QDebug>
#include <QRegExp>
#include <QErrorMessage>

#include "ScanManager.h"

ScanManager::ScanManager(QObject *parent)
    : QObject( parent )
    , mMode( LIST )
{
}

ScanManager::~ScanManager()
{
}

void ScanManager::list()
{
    mProc = new QProcess(this);

    connect(mProc, SIGNAL(readyReadStandardOutput()),this, SLOT(onReadyReadStandardOutput()) );
    connect(mProc, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()) );
    connect(mProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SIGNAL(listFinished(int,QProcess::ExitStatus)));
    connect(mProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));

    mSourceList.clear();
    mMode = LIST;

    QStringList arguments;
    arguments << "cdsm" << "lds" << "q";

#ifdef WIN32
    mProc->start("ScanImage.exe", arguments);
#else
    mProc->start("./ScanImage", arguments);
#endif
}

void ScanManager::scan()
{
    mProc = new QProcess(this);

    connect(mProc, SIGNAL(readyReadStandardOutput()),this, SLOT(onReadyReadStandardOutput()) );
    connect(mProc, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()) );
    connect(mProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SIGNAL(scanFinished(int,QProcess::ExitStatus)));
    connect(mProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));

    mMode = SCAN;

    QStringList arguments;
    arguments << "cdsm" << QString("cds%1").arg(mCurrentSourceId) << "scan" << "q";

#ifdef WIN32
    mProc->start("ScanImage.exe", arguments);
#else
    mProc->start("./ScanImage", arguments);
#endif
}

void ScanManager::onReadyReadStandardOutput()
{
    if(mMode == LIST)
    {
        QString arr = (QString)mProc->readAllStandardOutput();
        QRegExp regex("\\d:[^\\n]*");
        regex.setCaseSensitivity(Qt::CaseInsensitive);
        regex.setPatternSyntax(QRegExp::RegExp2);

        int pos = 0;
        while((pos = regex.indexIn(arr, pos)) != -1)
        {
            mSourceList.append(regex.cap(0));
            pos += regex.matchedLength();
        }
    }
}

void ScanManager::onReadyReadStandardError()
{
    //qDebug()<<mProc->readAllStandardError();
}

void ScanManager::onFinished(int,QProcess::ExitStatus)
{
    QByteArray msg = mProc->readAllStandardError();
    if(msg.size() > 0)
    {
        QErrorMessage err((QWidget*)this->parent());
        err.showMessage(msg);
        err.exec();
    }
    delete mProc;
}
