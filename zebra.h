#ifndef ZEBRA_H
#define ZEBRA_H

#define DBG

#include <QSystemTrayIcon>
#include <QDialog>

#include <QtNetwork>
#include <QMessageBox>
#include <QXmlQuery>
#include <QXmlResultItems>

//#include <QtNetwork/QAbstractSocket>
//#include <QtNetwork/QSslSocket>
#include <QMap>
#include <QSettings>
#include <QSet>
#include <QtSvg>


#include <QCloseEvent>
#include <QMenu>
#include <QTimer>
//#include "alarm.h"



namespace Ui
{
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
  explicit Zebra(QWidget *parent = nullptr);
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
  void sendSearch();
  void sendDismiss(QString invId);
  void sendRead(QString invId);
  void addtoFilter(QString s);
  void socketError(QAbstractSocket::SocketError error);
  void sslErrors(const QList<QSslError> &errors);
  void about();
  void viewAbout();
  void viewMail();
  void viewmessMail();
  void changeBrowser();

  void updateTrayIcon();
  void autoRun_slot();


private:
  void setupSecureSocket();
  void createTrayIcon();
  void createActions();
  void appendString(const  QByteArray &line, const qint64 &size);
  void socketReadBuffer(const  QByteArray &line, const qint64 &size);
  void soapSend(const QString &line);
  void initSettings();
  void saveSettings();
  void runBrw(const QString &auth);
  void doxml(const QString &xmldata);


  Ui::Zebra *ui = nullptr;
  QSystemTrayIcon *trayIcon = nullptr;
  QSystemTrayIcon *trayIconAppt = nullptr;
  QMenu *trayIconMenu = nullptr;
  QTimer *timer = nullptr;
  QTimer *timerIcon = nullptr;
  int IconN;



  QAction *minimizeAction = nullptr;
  QAction *maximizeAction = nullptr;
  QAction *restoreAction = nullptr;
  QAction *quitAction = nullptr;
  QAction *viewAction = nullptr;
  QAction *messviewAction = nullptr;

  QSslSocket *socket = nullptr;
  bool handlingSocketError = false;
  QString _authToken;
  QString _sessionID;
  QString _changeToken;
  QString _cookie;
  QString _state;
  QString _error;
  int _error_cnt;
  qint64 _ContentLength;
  int _chunked;
  int _msgget;
  int _prevIcon;

  QByteArray buffer;
  QMap <QString, QString> settings;
  QSettings configs;
  //QSettings *autorun;
  QMap <QString, QString> messages;
  Alarm *alarm = nullptr;
  Messages *mess = nullptr;
  int ord;
};

#endif // ZEBRA_H





