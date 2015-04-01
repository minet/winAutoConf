#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include "log.h"

#include "wiredconnectionwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void helpWindow(void);

private:
    Log *mLog;
    QGridLayout *mGridLayout;
    QPushButton *mWiredConnectionButton; //!< Bouton permettant d'ouvrir une fenêtre pour gérer la connexion câblée.
    QWidget *mWiredConnectionWindow; //!< Fenêtre permettant de gérer la connexion câblée
};

QString exec_command(QString const &command, Log *log);

#endif // MAINWINDOW_H
