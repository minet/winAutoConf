#include "wiredconnectionwindow.h"
#include "mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QNetworkInterface>
#include <QList>
#include <iostream>

WiredConnectionWindow::WiredConnectionWindow(Log *log, QWidget *parent) : QWidget(parent)
{
    /// Layout, button and text zones for user
    mLog = log;
    mLayout = new QGridLayout;
    mOkButton = new QPushButton(tr("Launch"));
    mLogin = new QLineEdit;
    mPassword = new QLineEdit;

    setLayout(mLayout);
    mLayout->addWidget(new QLabel(tr("Login")), 0, 0);
    mLayout->addWidget(mLogin, 0, 1);

    mLayout->addWidget(new QLabel(tr("Password")));
    mLayout->addWidget(mPassword, 1, 1);

    mLayout->addWidget(mOkButton, 2, 0, 1, 2);

    mPassword->setEchoMode(QLineEdit::Password); /// asterisks for password

    connect(mOkButton, SIGNAL(clicked()), this, SLOT(launched())); /// Click button call launched
    connect(mPassword, SIGNAL(returnPressed()), this, SLOT(launched()));
}

void WiredConnectionWindow::launched(void)
{
    QString login = mLogin->text();
    QString password = mPassword->text();
    this->hide();

    /** Configure PEAP authentification:
     *  Disable certificate
     *  Only users
     *  Fast reconnect */
    QString xmlConfiguration = QString(
            "<?xml version=\"1.0\" encoding=\"US-ASCII\"?>\n"
            "<LANProfile xmlns=\"http://www.microsoft.com/networking/LAN/profile/v1\">\n"
                "<MSM>\n"
                    "<security>\n"
                        "<OneXEnforced>false</OneXEnforced>\n"
                        "<OneXEnabled>true</OneXEnabled>\n"
                        "<OneX xmlns=\"http://www.microsoft.com/networking/OneX/v1\">\n"
                            "<cacheUserData>true</cacheUserData>"
                            "<authMode>user</authMode>"
                            "<EAPConfig>\n"
                                "<EapHostConfig xmlns=\"http://www.microsoft.com/provisioning/EapHostConfig\"\n"
                                               "xmlns:eapCommon=\"http://www.microsoft.com/provisioning/EapCommon\"\n"
                                               "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapMethodConfig\">\n"
                                    "<EapMethod>\n"
                                        "<eapCommon:Type>25</eapCommon:Type>\n"
                                        "<eapCommon:AuthorId>0</eapCommon:AuthorId>\n"
                                   "</EapMethod>\n"
                                   "<Config xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapConnectionPropertiesV1\"\n"
                                           "xmlns:msPeap=\"http://www.microsoft.com/provisioning/MsPeapConnectionPropertiesV1\"\n"
                                           "xmlns:msChapV2=\"http://www.microsoft.com/provisioning/MsChapV2ConnectionPropertiesV1\">\n"
                                       "<baseEap:Eap>\n"
                                           "<baseEap:Type>25</baseEap:Type>\n"
                                           "<msPeap:EapType>\n"
                                               "<msPeap:FastReconnect>true</msPeap:FastReconnect>\n"
                                               "<msPeap:InnerEapOptional>0</msPeap:InnerEapOptional>\n"
                                               "<baseEap:Eap>\n"
                                                   "<baseEap:Type>26</baseEap:Type>\n"
                                                   "<msChapV2:EapType>\n"
                                                       "<msChapV2:UseWinLogonCredentials>false</msChapV2:UseWinLogonCredentials>\n"
                                                   "</msChapV2:EapType>\n"
                                               "</baseEap:Eap>\n"
                                               "<msPeap:EnableQuarantineChecks>false</msPeap:EnableQuarantineChecks>\n"
                                               "<msPeap:RequireCryptoBinding>false</msPeap:RequireCryptoBinding>\n"
                                               "<msPeap:PeapExtensions />\n"
                                           "</msPeap:EapType>\n"
                                       "</baseEap:Eap>\n"
                                   "</Config>\n"
                               "</EapHostConfig>\n"
                            "</EAPConfig>\n"
                        "</OneX>\n"
                    "</security>\n"
                "</MSM>\n"
            "</LANProfile>\n");

    /** Configure Username and Password for authentification **/
    QString xmlUserPassword = QString(
              "<?xml version=\"1.0\" ?>\n"
              "<EapHostUserCredentials xmlns=\"http://www.microsoft.com/provisioning/EapHostUserCredentials\"\n"
                "xmlns:eapCommon=\"http://www.microsoft.com/provisioning/EapCommon\"\n"
                "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapMethodUserCredentials\">\n"
                "<EapMethod>\n"
                  "<eapCommon:Type>25</eapCommon:Type>\n"
                  "<eapCommon:AuthorId>0</eapCommon:AuthorId>\n"
                "</EapMethod>\n"
                "<Credentials xmlns:eapUser=\"http://www.microsoft.com/provisioning/EapUserPropertiesV1\"\n"
                  "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                  "xmlns:baseEap=\"http://www.microsoft.com/provisioning/BaseEapUserPropertiesV1\"\n"
                  "xmlns:MsPeap=\"http://www.microsoft.com/provisioning/MsPeapUserPropertiesV1\"\n"
                  "xmlns:MsChapV2=\"http://www.microsoft.com/provisioning/MsChapV2UserPropertiesV1\">\n"
                  "<baseEap:Eap>\n"
                    "<baseEap:Type>25</baseEap:Type>\n"
                    "<MsPeap:EapType>\n"
                      "<MsPeap:RoutingIdentity>") + login + "</MsPeap:RoutingIdentity>\n" + QString(
                      "<baseEap:Eap>\n"
                        "<baseEap:Type>26</baseEap:Type>\n"
                        "<MsChapV2:EapType>\n"
                          "<MsChapV2:Username>") + login + QString("</MsChapV2:Username>\n"
                          "<MsChapV2:Password>") + password + QString("</MsChapV2:Password>\n"
                          "<MsChapV2:LogonDomain></MsChapV2:LogonDomain>\n"
                        "</MsChapV2:EapType>\n"
                      "</baseEap:Eap>\n"
                    "</MsPeap:EapType>\n"
                  "</baseEap:Eap>\n"
                "</Credentials>\n"
              "</EapHostUserCredentials>\n");

    /// Enable service dot3svc and set to automatic mode
    exec_command("powershell Start-Service dot3svc", mLog);
    exec_command("powershell Set-Service dot3svc -StartupType Automatic", mLog);

    /// Get informations of the wired connection
    QString interfacesLan = exec_command("netsh lan show interface", mLog);

    QStringList interfacesToConfigure; // The list of interfaces to configure

    /// All Network Interface on this computer
    QList<QNetworkInterface> interfaces(QNetworkInterface::allInterfaces());

    /// Get usefull interfaces
    for(QNetworkInterface &interface : interfaces)
        if(interfacesLan.indexOf(interface.humanReadableName()) != -1)
            interfacesToConfigure << '"' + interface.humanReadableName() + '"';

    /// For each interfaces : balékouy
    for(QString const &name : interfacesToConfigure)
    {
        /// Dynamic Addressses and DNS Server
        exec_command("netsh interface ipv4 set address name=" + name + " source=dhcp", mLog);
        exec_command("netsh interface ipv4 set dnsserver name=" + name + " source=dhcp", mLog);

        QFile fileConfiguration("C:\\tmpConfiguration.xml");
        QFile fileUserPassword("C:\\tmpUserPassword.xml");
        fileConfiguration.open(QIODevice::WriteOnly);
        fileUserPassword.open(QIODevice::WriteOnly);

        QTextStream streamConfiguration(&fileConfiguration);
        QTextStream streamUserPassword(&fileUserPassword);

        streamConfiguration << xmlConfiguration;
        streamUserPassword << xmlUserPassword;

        /// Flush and close
        fileConfiguration.close();
        fileUserPassword.close();

        /// Set the configuration and add username / password
        exec_command("netsh lan add profile filename=C:\\tmpConfiguration.xml interface=" + name, mLog);
        exec_command("netsh lan set eapuserdata filename=C:\\tmpUserPassword.xml allusers=yes interface=" + name, mLog);

        /// Reboot network card
        exec_command("netsh interface set interface " + name + " DISABLED", mLog);
        exec_command("netsh interface set interface " + name + " ENABLED", mLog);

        /// remove configuration files
        remove("C:\\tmpConfiguration.xml");
        remove("C:\\tmpUserPassword.xml");
    }
}

WiredConnectionWindow::~WiredConnectionWindow()
{
}

