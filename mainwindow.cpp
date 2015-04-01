#include "mainwindow.h"

#include <QFile>
#include <QMenuBar>
#include <QStyle>
#include "log.h"
#include <QToolBar>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowIconText("Config MiNET");
    setWindowIcon(QIcon("minet.png"));

    mLog = new Log("log.log", this);
    mGridLayout = new QGridLayout;
    mWiredConnectionButton = new QPushButton(tr("Wired Connection"));
    mWiredConnectionWindow = new WiredConnectionWindow(mLog);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mGridLayout);

    mGridLayout->addWidget(mWiredConnectionButton);

    connect(mWiredConnectionButton, SIGNAL(clicked()), mWiredConnectionWindow, SLOT(show()));

    connect(menuBar()->addAction(tr("Help")), SIGNAL(triggered()), this, SLOT(helpWindow()));
}

void MainWindow::helpWindow(void)
{
    QMessageBox::information(this, tr("Help"), tr("You have to choose between \"Wired connexion\" and \"Wireless connexion\" (not implemented yet).\n\n"
                                                  "You have to enter your MiNET Login and MiNET password, not the DISI ones, and press Launch.\n\n"
                                                  "When all prompts are closed, you can leave the application, and your computer should be configured.\n"));
}

MainWindow::~MainWindow()
{
    delete mWiredConnectionWindow;
}

QString exec_command(QString const &command, Log *log)
{
    QString newCommand = "chcp 65001 && " + command + " >tmp";
    QString ret;
    system(newCommand.toLatin1());

    QFile tmp("tmp");

    tmp.open(QIODevice::ReadOnly);
    ret = log->write(command, tmp.readAll());
    tmp.close();

    remove("tmp");

    return ret;
}
