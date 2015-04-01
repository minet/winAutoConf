#include "log.h"
#include <QApplication>

#include <QDate>
Log::Log(QString const &path, QObject *parent) : QObject(parent), mLogs(path)
{
    mLogs.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    mStream.setDevice(&mLogs);
}

QString Log::write(QString const &step, const QString &str)
{
    mStream << QDate::currentDate().toString(Qt::ISODate) << " : " << step << str << "\n";
    return str;
}

Log::~Log()
{
    mStream << "\n\n";
}

