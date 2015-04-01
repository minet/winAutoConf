#ifndef WIREDCONNECTIONWINDOW_H
#define WIREDCONNECTIONWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include "log.h"

class WiredConnectionWindow : public QWidget
{
    Q_OBJECT
public:
    WiredConnectionWindow(Log *log, QWidget *parent = 0);
    ~WiredConnectionWindow();

public slots:
    void launched(void);

private:
    Log *mLog;
    QGridLayout *mLayout;
    QPushButton *mOkButton;
    QLineEdit *mLogin;
    QLineEdit *mPassword;
};

#endif // WIREDCONNECTIONWINDOW_H
