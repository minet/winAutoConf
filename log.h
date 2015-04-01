#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class Log : public QObject
{
public:
    Log(QString const &path, QObject *parent = 0);
    QString write(QString const &step, QString const &str);
    ~Log();

private:
    QFile mLogs;
    QTextStream mStream;
};

#endif // LOG_H
