#ifndef ZEBRA_H
#define ZEBRA_H

#define DBG

#include <QSystemTrayIcon>
#include <QDialog>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QMap>
#include <QSettings>
#include <QSet>
#include <QtSvg>


#include <QCloseEvent>
#include <QMenu>
#include <QTimer>
//#include "alarm.h"



namespace Ui {
class Zebra;
}

class QSslSocket;
class Alarm;
class Messages;
class inf;




class Zebra : public QDialog
{
    Q_OBJECT
    
public:
    explicit Zebra(QWidget *parent = 0);
    ~Zebra();

    void setVisible(bool visible);

signals:
    void addFilter(QString);


protected:
    void closeEvent(QCloseEvent *event);


private slots:
    void timerupdate();
    void acceptOK();
    void acceptNO();
    void setIcon(int index);
    void setIconAppt(int index);
    void setIconAppt(QString str);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void iconActivatedAppt(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();
    void changeRXlist(bool checked);
    void changeRX(QString str);
    void changeRX();
    void showMessages(bool l);

    bool secureConnect();
    void updateEnabledState();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketEncrypted();
    void socketReadyRead();
    void sendAuth();
    void sendMess();
    void sendDismiss(QString invId);
    void sendRead(QString invId);
    void addtoFilter(QString s);
    void sslErrors(const QList<QSslError> &errors);
    void about();
    void viewAbout();
    void viewMail();
    void viewmessMail();
    void changeBrowser();

    void updateTrayIcon();
    void autoRun_slot();

    
private:
    void createTrayIcon();
    void createActions();
    void appendString(const  QByteArray &line, const qint64 &size);
    void soapSend(const QString &line);
    void initSettings();
    void saveSettings();
    void runBrw(const QString &auth);


    Ui::Zebra *ui;
    QSystemTrayIcon *trayIcon;
    QSystemTrayIcon *trayIconAppt;
    QMenu *trayIconMenu;
    QTimer *timer;
    QTimer *timerIcon;
    int IconN;



    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *viewAction;
    QAction *messviewAction;

    QSslSocket *socket;
    QString _authToken;
    QString _cookie;
    QString _state;
    QString _error;
    int _error_cnt;
    int _ContentLength;
    int _chunked;
    int _msgget;
    int _prevIcon;
    QByteArray buffer;
    QMap <QString, QString> settings;
    QSettings configs;
    //QSettings *autorun;
    QMap <QString, QString> messages;
    Alarm *alarm;
    Messages *mess;
    int ord;



};

#endif // ZEBRA_H





