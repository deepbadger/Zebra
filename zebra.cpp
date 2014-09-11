#include "zebra.h"
#include "ui_zebra.h"
//#ifdef  Q_OS_WIN32
#include "win.h"
//#endif
#include "alarm.h"
#include "messages.h"
#include "inf.h"
#include "tabledelegate.h"
#include <QMessageBox>
#include <QDebug>


#include <QtGui/QScrollBar>
#include <QtGui/QStyle>
#include <QtNetwork/QSslCipher>
#include <QDebug>
#include <QtXml/QXmlStreamReader>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>
#include <QCryptographicHash>







Zebra::Zebra(QWidget *parent): QDialog(parent),
    ui(new Ui::Zebra), socket(0), configs()
{

    ui->setupUi(this);

    alarm = new Alarm(this);
    mess = new Messages(this);
    //mess->show();





    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIconAppt, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivatedAppt(QSystemTrayIcon::ActivationReason)));

    connect (this, SIGNAL ( addFilter (QString)), mess, SLOT ( filterOK(QString )));

    timerIcon = new QTimer(this);
    connect(timerIcon , SIGNAL(timeout()), this, SLOT(updateTrayIcon()));

    setIcon(0);
    trayIcon->show();
    //trayIconAppt->show();
    this->hide();


    QRect geo =  trayIcon->geometry();
    //qDebug () << "tray"<<geo.left() << geo.right() << geo.top() << geo.bottom();


    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами єкрана
    QPoint center = rect.center(); //координаты центра экрана
    //qDebug () << "tray"<<center.x() << center.y() << geo.x() << geo.y() ;
    int x,y,w,h;

    // tray 1490 1513 1012 1049
    // tray 839 504

    if((center.x() > geo.x())&&(center.y() > geo.y())){ //left up
        x = geo.right() ;
        y = geo.bottom();
    }
    if((center.x() < geo.x())&&(center.y() < geo.y())){//right down
        x = geo.right() - alarm->width();
        y = geo.bottom() - alarm->height() - 60;
    }
    if((center.x() > geo.x())&&(center.y() < geo.y())){//left down
        x = geo.right() + 60 ;
        y = geo.bottom() - alarm->height();
    }
    if((center.x() < geo.x())&&(center.y() > geo.y())){//right up
        x = geo.right() - alarm->width();
        y = geo.bottom();
    }
    alarm->setGeometry(x,y ,alarm->width(), alarm->height());
    initSettings();

    QIcon icon = QIcon(":/images/zebraAppt.svg");
    trayIconAppt->setIcon(icon);


    //sendAuth();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerupdate()));
    timer->start(ui->spinBox->value() * 1000);


    trayIconAppt->setIcon(QIcon(":/images/zebraAppt.svg"));

    //    timerIcon->start(800); // update it every 2 seconds
    //    timerIcon->stop();


    sendMess();

}

Zebra::~Zebra()
{
    saveSettings();
    delete ui;
}

void Zebra::setVisible(bool visible)
{
#ifdef DBG
    qDebug() << "setVisible(bool visible)";
#endif
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);
}

void Zebra::closeEvent(QCloseEvent *event)
{
#ifdef DBG
    qDebug() << "closeEvent";
#endif
    if (trayIcon->isVisible()) {
        //QMessageBox::information(this, tr("Systray"),
        //                                 tr("The program will keep running in the "
        //                                    "system tray. To terminate the program, "
        //                                    "choose <b>Quit</b> in the context menu "
        //                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

//bool Zebra::eventFilter(QObject *obj, QEvent *ev)
//{
//    qDebug() << obj->objectName();



//}

void Zebra::timerupdate()
{
#ifdef DBG
    qDebug() << "Timer:";
#endif
    if (_error_cnt < 3)
        sendMess();

    if(alarm->isVisible())
        alarm->upd();


}

void Zebra::setIconAppt(int index)
{
    //qDebug() << "setIcon";
    QIcon icon = QIcon(":/images/zebraAppt.svg");
    trayIconAppt->setIcon(icon);

    if (index > 0)
        trayIconAppt->show();
    else
        trayIconAppt->hide();

    //setWindowIcon(icon);


}

void Zebra::setIconAppt(QString str)
{
#ifdef DBG
    qDebug() << "setIconAppt: " << str;
#endif
    trayIconAppt->setToolTip(str);
    if (str.isEmpty()){

        if (trayIconAppt->isVisible())
            trayIconAppt->hide();
        if(!alarm->isHidden())
            alarm->hide();
    }else{
        if (!trayIconAppt->isVisible()){
            if(alarm->isHidden())
                alarm->show();
            trayIconAppt->show();
        }
    }
}

void Zebra::showMessages(bool l)
{
    if (l){
        if(mess->isHidden())
            mess->show();
        emit addFilter(ui->rxEdit->toPlainText());
    }else{
        if (mess->isVisible()){
            mess->hide();
            emit addFilter(ui->rxEdit->toPlainText());
        }
    }
}


void Zebra::setIcon(int index)
{
#ifdef DBG
    qDebug() << "setIcon" << index;
#endif
    IconN = index;
    QIcon icon;
    if((index != 7) && (index != 5))
        if (timerIcon->isActive())
            timerIcon->stop();
    switch (index) {
    case 0:
        icon = QIcon(":/images/zebra00.svg");//black
        break;
    case 1:
        icon = QIcon(":/images/zebra01.svg");//blue
        break;
    case 2:
        icon = QIcon(":/images/zebra03.svg");//green
        break;
    case 5:
        trayIconAppt->setIcon(QIcon(":/images/zebraAppt.svg"));//appointment
        break;
    case 7:
        icon = QIcon(":/images/zebra07.svg");//new-mail
        break;
    case 9:
        icon = QIcon(":/images/zebra09.svg");
        break;
    default:
        break;
    }
    trayIcon->setIcon(icon);
    setWindowIcon(icon);


}

void Zebra::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
#ifdef DBG
    qDebug() << "iconActivated";
    qDebug() << reason;
#endif
    switch (reason) {
    case QSystemTrayIcon::Trigger:

        sendMess();
        break;
    case QSystemTrayIcon::DoubleClick:
        //showMessage();
        viewMail();
        break;
    case QSystemTrayIcon::MiddleClick:
        //showNormal();
        showMessage();
        break;
    default:
        ;
    }
}

void Zebra::iconActivatedAppt(QSystemTrayIcon::ActivationReason reason)
{
#ifdef DBG
    qDebug() << "iconActivatedAppt";
    qDebug() << reason;
#endif
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        alarm->setVisible(alarm->isHidden());
        //sendMess();
        break;
    case QSystemTrayIcon::DoubleClick:
        //showMessage();
        //viewMail();
        break;
    case QSystemTrayIcon::MiddleClick:
        //showNormal();
        //showMessage();
        break;
    default:
        ;
    }
}

void Zebra::showMessage()
{
#ifdef DBG
    qDebug() << "showMessage";
#endif
    //QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
    //QIcon(":/images/zebrablack.svg").);
    trayIcon->showMessage("", "");
}

void Zebra::messageClicked()
{
#ifdef DBG
    qDebug() << "messageClicked";
#endif
    if (_authToken != ""){
        //QDesktopServices::openUrl(QUrl("http://*/service/preauth?isredirect=1&authtoken=" + _authToken, QUrl::TolerantMode));
    }
    //QMessageBox::information(0, tr("Systray"),
    //tr("Sorry, I already gave what help I could.\n"
    //"Maybe you should try asking a human?"));
}

void Zebra::changeRXlist(bool checked)
{
    if (checked){
        ui->pushButton_RX->setText(tr("White list"));
        settings["stateRX"]= "true";
    }
    else{
        ui->pushButton_RX->setText(tr("Black list"));
        settings["stateRX"]= "false";
    }
    settings["hashmessages"] = "";
    settings["countmsg"] = "0";
}

void Zebra::changeRX(QString str)
{

    if (str.length() <= 0 ){
        ui->rxEdit->setPalette(QPalette());
        ui->labelInfo->setText("");
        return;
    }
    QRegExp rx;
    rx.setPattern(str);
    rx.setPatternSyntax(QRegExp::RegExp2);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    if (!rx.isValid()){
        ui->rxEdit->setPalette(QPalette());
        ui->labelInfo->setText(rx.errorString());
        return;
    }
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 255, 192));
    ui->rxEdit->setPalette(palette);
    ui->labelInfo->setText("");


}

void Zebra::changeRX()
{
    changeRX(ui->rxEdit->toPlainText());
}

void Zebra::createActions()
{
#ifdef DBG
    qDebug() << "createActions";
#endif
    minimizeAction = new QAction(tr("&Hide"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Settings"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    viewAction = new QAction(tr("&View browser"), this);
    connect(viewAction, SIGNAL(triggered()), this, SLOT(viewMail()));

    messviewAction = new QAction(tr("&View mail"), this);
    connect(messviewAction, SIGNAL(triggered()), this, SLOT(viewmessMail()));

}


void Zebra::createTrayIcon()
{
#ifdef DBG
    qDebug() << "createTrayIcon";
#endif
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(viewAction);
    trayIconMenu->addAction(messviewAction);
    trayIconMenu->addAction(minimizeAction);
    //trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIconMenu->setDefaultAction(viewAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIconAppt = new QSystemTrayIcon(this);
}

bool Zebra::secureConnect()
{

    if (!socket) {
        socket = new QSslSocket(this);
        connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        connect(socket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
        connect(socket, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
    }

    if (socket && socket->state() == QAbstractSocket::UnconnectedState){
        socket->connectToHostEncrypted(settings.value("URL", ""), 443);
    }
    updateEnabledState();

    return socket && socket->state() == QAbstractSocket::ConnectedState;
}

void Zebra::updateEnabledState()
{

    bool unconnected = !socket || socket->state() == QAbstractSocket::UnconnectedState;

    bool connected = socket && socket->state() == QAbstractSocket::ConnectedState;
#ifdef DBG
    qDebug() << "updateEnabledState - +"<< unconnected << connected;
#endif

}

void Zebra::socketStateChanged(QAbstractSocket::SocketState state)
{
#ifdef DBG
    qDebug() << "SocketState:" << state;
#endif

    if (state == QAbstractSocket::HostLookupState) {
        //_state = "HostLookupState";

    }

    if (state == QAbstractSocket::ConnectingState) {
        //_state = "ConnectingState";
    }

    if (state == QAbstractSocket::ClosingState ) {
        //_state = "ClosingState";
    }

    updateEnabledState();
    if (state == QAbstractSocket::UnconnectedState) {
        ui->lineSrv->setPalette(QPalette());
        ui->lineSrv->setFocus();
        ui->labelInfo->setText(tr(""));
        socket->deleteLater();
        socket = 0;
        if (!(_state == "ConnectedState")){
            _state = "UnconnectedState";
            setIcon(0);
        }
        _state = "UnconnectedState";

    }


    if (state == QAbstractSocket::ConnectedState) {
        _state = "ConnectedState";
        sendMess();
    }
}

void Zebra::socketEncrypted()
{
#ifdef DBG
    qDebug() << "socketEncrypted";
#endif
    if (!socket)
        return;                 // might have disconnected already

    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 255, 192));
    ui->lineSrv->setPalette(palette);

    QSslCipher ciph = socket->sessionCipher();
    QString cipher = QString("%1, %2 (%3/%4)").arg(ciph.authenticationMethod())
            .arg(ciph.name()).arg(ciph.usedBits()).arg(ciph.supportedBits());;
    ui->labelInfo->setText(cipher);


}



void Zebra::socketReadyRead()
{
#ifdef DBG
    qDebug() << "socketReadyRead: bytesAvailable  _ContentLength " << socket->bytesAvailable() <<  _ContentLength;
#endif
    //appendString(QString::fromUtf8(socket->readAll()), socket->bytesAvailable());
    appendString(socket->readAll(), socket->bytesAvailable());
}

void Zebra::sendAuth()
{

    if (settings.value("Account").isEmpty() || settings.value("Password").isEmpty())
        return;


    if (!secureConnect())
        return;


    QDateTime current = QDateTime::currentDateTime().toUTC();
    int calExpandInstStart = QDateTime::currentDateTime().addDays(-7).toUTC().toTime_t() * 1000;
    int calExpandInstEnd = QDateTime::currentDateTime().addDays(+1).toUTC().toTime_t() * 1000;

    //qDebug() << "time" << current.toTime_t()<< " "<<calExpandInstStart<< " " <<calExpandInstEnd ;

    QString BodySearch = QString("<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\">"
                                 "<env:Header><context xmlns=\"urn:zimbra\"><authToken>%1</authToken></context></env:Header>"
                                 "<env:Body><BatchRequest xmlns=\"urn:zimbra\"><SearchRequest xmlns=\"urn:zimbraMail\">"
                                 "<query>is:unread</query></SearchRequest><SearchRequest xmlns=\"urn:zimbraMail\""
                                 "calExpandInstEnd=\"%2\" calExpandInstStart=\"%3\" sortBy=\"dateAsc\" types=\"appointment\">"
                                 "<query>inid:10</query></SearchRequest></BatchRequest></env:Body></env:Envelope>").arg(_authToken).arg(calExpandInstEnd).arg(calExpandInstStart);




    QString BodyAuth = QString("<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\">"
                               "<env:Header/><env:Body><AuthRequest xmlns=\"urn:zimbraAccount\">"
                               "<account by=\"name\">%1</account>"
                               "<password>%2</password>"
                               "</AuthRequest></env:Body></env:Envelope>").arg(settings.value("Account")).arg(settings.value("Password"));

    QString Header = QString("POST http://%1/service/soap/ HTTP/1.1\r\n"
                             "User-Agent: Java/1.7.0_09\r\n"
                             "Host: %1\r\n"
                             "Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\n"
                             "Connection: keep-alive\r\n"
                             "Content-type: application/x-www-form-urlencoded\r\n"
                             "Content-Length: %3\r\n\r\n").arg(settings.value("URL")).arg(BodyAuth.length());
    //qDebug() << "Auth";
    QString input = Header + BodyAuth;

    //if (secureConnect()){
#ifdef DBG
    qDebug() << "sendAuth - " << input;
#endif
    socket->write(input.toUtf8() + "\r\n");
    //}

}

void Zebra::sendMess()
{
    if (_authToken == ""){
        sendAuth();
        return;
    }

    if (!secureConnect())
        return;
    QDateTime current = QDateTime::currentDateTime().toUTC();
    qint64 calExpandInstStart = current.addSecs(-(60)).toMSecsSinceEpoch() ;
    qint64 calExpandInstEnd = current.addDays(+7).toMSecsSinceEpoch() ;

    //qDebug() << "time" << current.currentMSecsSinceEpoch() << " "<<calExpandInstStart  << " " <<calExpandInstEnd;


    QString BodySearch = QString("<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\">"
                                 "<env:Header><context xmlns=\"urn:zimbra\"><authToken>%1</authToken></context></env:Header>"
                                 "<env:Body><BatchRequest xmlns=\"urn:zimbra\" onerror=\"continue\"><SearchRequest limit=\"%2\" offset=\"0\" xmlns=\"urn:zimbraMail\">"
                                 "<query>is:unread</query></SearchRequest> "
                                 "<SearchRequest xmlns=\"urn:zimbraMail\" types=\"appointment\" sortBy=\"dateAsc\" calExpandInstStart=\"%4\" calExpandInstEnd=\"%3\" >"
                                 "<query>appt-start: &lt;+7day AND (is:local OR is:remote)</query>"
                                 "</SearchRequest>"
                                 "<SearchRequest xmlns=\"urn:zimbraMail\" types=\"task\" sortBy=\"dateAsc\" calExpandInstStart=\"%4\" calExpandInstEnd=\"%3\" >"
                                 "<query>appt-start: &lt;+7day AND (is:local OR is:remote)</query>"
                                 "</SearchRequest>"
                                 //                                 "<SearchRequest xmlns=\"urn:zimbraMail\" calExpandInstEnd=\"%3\" calExpandInstStart=\"%4\" "
                                 //                                 "sortBy=\"dateAsc\" types=\"appointment\">"
                                 //                                 "<query>inid:10</query></SearchRequest>"
                                 "</BatchRequest></env:Body></env:Envelope>").arg(_authToken).arg(_msgget).arg(calExpandInstEnd).arg(calExpandInstStart);







    QString Header = QString("POST http://%1/service/soap/ HTTP/1.1\r\n"
                             "User-Agent: Java/1.7.0_09\r\n"
                             "Host: %1\r\n"
                             "Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\n"
                             "Connection: keep-alive\r\n"
                             "Content-type: application/x-www-form-urlencoded\r\n"
                             "Content-Length: %3\r\n\r\n").arg(settings.value("URL")).arg(BodySearch.length());
#ifdef DBG
    qDebug() << "Search" << BodySearch;
#endif
    QString input = Header + BodySearch;
    //qDebug() << input;
    if (secureConnect()){
#ifdef DBG
        qDebug() << "sendSearch";
#endif
        socket->write(input.toUtf8() + "\r\n");

    }

}

void Zebra::sendDismiss(QString invId)
{
    if(invId.isEmpty()){
#ifdef DBG
        qDebug() << "Dismiss empty";
#endif
        return;
    }

    if (_authToken == ""){
        sendAuth();
        return;
    }

    if (!secureConnect())
        return;

    qint64 now = QDateTime::currentMSecsSinceEpoch();




    QString BodyDismiss = QString("<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\">"
                                  "<env:Header><context xmlns=\"urn:zimbra\"><authToken>%1</authToken></context></env:Header>"
                                  "<env:Body><DismissCalendarItemAlarmRequest xmlns=\"urn:zimbraMail\">"
                                  "<appt dismissedAt=\"%3\" id=\"%2\"/></DismissCalendarItemAlarmRequest>"
                                  "</env:Body></env:Envelope>").arg(_authToken).arg(invId).arg(now);

    QString Header = QString("POST http://%1/service/soap/ HTTP/1.1\r\n"
                             "User-Agent: Java/1.7.0_09\r\n"
                             "Host: %1\r\n"
                             "Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\n"
                             "Connection: keep-alive\r\n"
                             "Content-type: application/x-www-form-urlencoded\r\n"
                             "Content-Length: %3\r\n\r\n").arg(settings.value("URL")).arg(BodyDismiss.length());
#ifdef DBG
    qDebug() << "Dismiss" << BodyDismiss;
#endif
    QString input = Header + BodyDismiss;
    //qDebug() << input;
    if (secureConnect()){
#ifdef DBG
        qDebug() << "sendDismiss";
#endif
        socket->write(input.toUtf8() + "\r\n");
    }

}

void Zebra::sendRead(QString invId)
{
    if(invId.isEmpty()){
#ifdef DBG
        qDebug() << "Read empty";
#endif
        return;
    }

    if (_authToken == ""){
        sendAuth();
        return;
    }

    if (!secureConnect())
        return;

    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QString id = invId;
    if (invId.startsWith('-'))
        id = invId.mid(1);



    QString BodyRead = QString("<env:Envelope xmlns:env=\"http://www.w3.org/2003/05/soap-envelope\">"
                               "<env:Header><context xmlns=\"urn:zimbra\"><authToken>%1</authToken></context></env:Header>"
                               "<env:Body><MsgActionRequest xmlns=\"urn:zimbraMail\">"
                               "<action id=\"%2\" op=\"read\"/></MsgActionRequest>"
                               "</env:Body></env:Envelope>").arg(_authToken).arg(id);

    QString Header = QString("POST http://%1/service/soap/ HTTP/1.1\r\n"
                             "User-Agent: Java/1.7.0_09\r\n"
                             "Host: %1\r\n"
                             "Accept: text/html, image/gif, image/jpeg, *; q=.2, */*; q=.2\r\n"
                             "Connection: keep-alive\r\n"
                             "Content-type: application/x-www-form-urlencoded\r\n"
                             "Content-Length: %3\r\n\r\n").arg(settings.value("URL")).arg(BodyRead.length());
#ifdef DBG
    qDebug() << "BodyRead" << BodyRead;
#endif
    QString input = Header + BodyRead;
    //qDebug() << input;
    if (secureConnect()){
#ifdef DBG
        qDebug() << "sendRead";
#endif
        socket->write(input.toUtf8() + "\r\n");
    }

}

void Zebra::addtoFilter(QString s)
{
    if(s.isEmpty())
        return;
    QString x = ui->rxEdit->toPlainText();

    if(x.contains(s))
        return;


    if(! ui->rxEdit->toPlainText().trimmed().isEmpty())
        x = x +"|";

    ui->rxEdit->setPlainText(x+s);
    settings["RX"] = ui->rxEdit->toPlainText();

    emit addFilter(ui->rxEdit->toPlainText());

}



void Zebra::sslErrors(const QList<QSslError> &errors)
{
    if (ui->checkBoxSSL->checkState() == Qt::Checked){
        socket->ignoreSslErrors();
        if (socket->state() != QAbstractSocket::ConnectedState)
            socketStateChanged(socket->state());
        return;
    }

    QString errorstr;
#ifdef DBG
    qDebug() << "sslErrors";
#endif
    foreach (const QSslError &error, errors)
        errorstr = errorstr + error.errorString() + ". ";
#ifdef DBG
    qDebug() << errorstr;
#endif
    trayIcon->showMessage(tr("Error:"), errorstr, QSystemTrayIcon::Critical, 15000);
    ui->labelInfo->setText(errorstr);

}




void Zebra::appendString(const  QByteArray &line, const qint64 &size)
{
    if (line.length() == 0){
        return;
    }
    //qDebug() << "\n>>>>>" << line<<"<<<<<\n";

    QString result;
    qint64 sz = size;

    buffer = buffer.append(line);
    if (buffer.contains("\r\n\r\n")){
#ifdef DBG
        qDebug() << "\n\n\n%%%%%%%%%%%%%%%%\n\n";
#endif
        QString buff = QString::fromUtf8(buffer);
        if (buff.startsWith("HTTP/1.1 ",Qt::CaseInsensitive)){
            QStringList header =  buff.trimmed().split("\n");
            QStringList::const_iterator i;
            _ContentLength = 0;
            for (i = header.constBegin(); i != header.constEnd(); ++i){
                QString str = *i;
                //qDebug() << "******************"<<str;
                if (str.startsWith("Set-Cookie:"))
                    _cookie = str.remove("Set-Cookie:").trimmed() ;
                if (str.startsWith("Content-Length:")){
                    _chunked = 0;
                    _ContentLength = str.remove("Content-Length:").trimmed().toInt();
#ifdef DBG
                    qDebug() << "_ContentLength:" << _ContentLength;
#endif
                }
                if (str.startsWith("Transfer-Encoding: chunked")){
                    QString s = header.last();
                    _chunked = 1;
#ifdef DBG
                    qDebug() << "_ContentLength--:" << _ContentLength << s ;
#endif
                }
            }
#ifdef DBG
            qDebug() << "\nBEGIN HEADER++++++++++++++++++++++++++++++++++++++++\n"<< buffer<< "\nEND HEADER++++++++++++++++++++++++++++++++++++++++\n";
#endif
            buffer = buffer.mid(buffer.indexOf("\r\n\r\n")).mid(4);
            sz = size -(size - buffer.length());
#ifdef DBG
            qDebug() << "\nDROP1: ostatok" << buffer.length() << " allbytes"<< size -(size - buffer.length()) << " " << _ContentLength;
#endif
            //return;
        }else if (buff.contains("0\r\n\r\n")){
            qDebug() << "\n\n\nFINISH\n\n )))" << buffer;
            QStringList header =  buff.trimmed().split("\r\n");
            QStringList::const_iterator n;
            bool ok;
            int bytes;
            for (n = header.constBegin(); n != header.constEnd(); ++n){
                QString str = *n;
                if (str.trimmed() == "")
                    continue;
                bytes = str.trimmed().toInt(&ok, 16);
                if(ok){
#ifdef DBG
                    qDebug()<<str <<" hex " << bytes;
#endif
                    _ContentLength = size;
                }else{
                    result.append(str);
                }
            }
            buffer = buffer.mid(buffer.indexOf("0\r\n\r\n")).mid(5);
            sz = size -(size - buffer.length());
#ifdef DBG
            qDebug() << "\nDROP0: " << buffer.length();
#endif
        }


    }

    bool startauth = false;
    _ContentLength = _ContentLength - sz;

    if ((_ContentLength == 0) && (size != 0) && (_chunked == 0)){
        result = QString::fromUtf8(buffer);
    }

    if (!result.isNull()){

        QString rxtext = ui->rxEdit->toPlainText().trimmed();
        QRegExp rx;
        if (rxtext.length() > 0 ){
            rx.setPattern(rxtext);
            rx.setPatternSyntax(QRegExp::RegExp2);
            //rx.setCaseSensitivity(Qt::CaseInsensitive);

        }
        //trayIcon->showMessage("read", buffer, QSystemTrayIcon::Information, 15000);
        QXmlStreamReader xml;
        //ui->plainTextEdit->setPlainText( result);
#ifdef DBG
        qDebug() << "Result: " << result;
#endif
        xml.addData(result);
        int count_mess = 0;
        bool flagStateRX = false;
        QString id = "", messageA = "",messageB = "", hashid = "", msg;
        inf apptinf;
        inf_mail message;
        QList <inf> listApptinf;
        QList <inf_mail> listMessage;
        int DismissCalendarItemAlarmResponse = 0;
        int MsgActionResponse = 0;
        int SearchResponse = 0;


        while (!xml.atEnd()) {//xml parser
            xml.readNext();
            //******isStartElement
            if (xml.isStartElement()) {
                //---authToken
                if (xml.name().contains("authToken",Qt::CaseInsensitive)){
                    QString auth_token = xml.readElementText().trimmed();
                    if (auth_token.length() > 0){
                        startauth = true;
                        _authToken = auth_token;
                    }
#ifdef DBG
                    qDebug() << "_authToken:"<<_authToken;
#endif
                    break;
                }
                //---DismissCalendarItemAlarmResponse
                if (xml.name() == "DismissCalendarItemAlarmResponse"){
                    DismissCalendarItemAlarmResponse = 1;
                    break;
                }
                //---MsgActionResponse
                if (xml.name() == "MsgActionResponse"){
                    MsgActionResponse = 1;
                    break;
                }
                //---SearchResponse
                //                if (xml.name() == "SearchResponse"){


                //                }


                //---subject
                if ((xml.name() == "su")){
                    QString s = xml.readElementText().trimmed();
                    message.subject("<b>"+s+"</b>");
                    messageA = messageA +tr("Subject: ")+s + "\n";
                }
                //---fragment
                if ((xml.name() == "fr")){
                    QString s = xml.readElementText().trimmed();
                    message.fragment(s);
                    messageA = messageA + s + "\n";;
                }
                //---message
                if ((xml.name() == "m")){

                    message.dir(xml.attributes().value("id").toString());
                }
                //---e adress
                if (xml.name() == "e"){
#ifdef DBG
                    qDebug() << ""<< flagStateRX << xml.attributes().value("a").toString() ;
#endif
                    message.name(xml.attributes().value("p").toString());
                    message.from(xml.attributes().value("a").toString());
                    messageB = messageB + xml.attributes().value("p").toString() + " <"+xml.attributes().value("a").toString()+ ">; ";
                    if (!rxtext.isEmpty() && (rxtext.length() > 0)
                            &&(( rx.indexIn(xml.attributes().value("a").toString()) >= 0 )
                               || ( rx.indexIn(xml.attributes().value("p").toString()) >= 0 )
                               ||( rx.indexIn(xml.attributes().value("d").toString()) >= 0 ))){
                        flagStateRX = (settings.value("stateRX","false")== "false")?false:true;
#ifdef DBG
                        qDebug() << "((((((((((((march)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<< flagStateRX << xml.attributes().value("a").toString() ;
#endif
                    }
                }
                //---root mail id
                if (xml.name() == "c"){
                    if(!message.gId().isEmpty() && !message.gdate().isEmpty()){//заполнена prev, значит сохраняем prev в список, обнуляем prev, и заполняем next структуру
                        if (flagStateRX){//проверка на черный список
                            listMessage.append(message);
                            count_mess++;
                            hashid = hashid + id;
                            if (messageA.length()>0 || messageB.length()>0){
                                msg = msg + tr("From: %1\n%2\n").arg(messageB).arg(messageA);
                            }
                        }
                        message.reset();
                    }
                    message.mId(xml.attributes().value("id").toString());
                    message.date(xml.attributes().value("d").toString());
                    messageA = "";messageB = "";
                    id = xml.attributes().value("id").toString() + xml.attributes().value("d").toString() + xml.attributes().value("sf").toString();
                    id = QString(QCryptographicHash::hash(id.toUtf8(),QCryptographicHash::Md5).toHex());
                    flagStateRX = (settings.value("stateRX","false")== "false")?true:false;
                }

                if (xml.name() == "SearchResponse"){
                    _error = "";
                    _error_cnt = 0;
                    SearchResponse = 1;
                }
                if ((xml.name() == "appt")||(xml.name() == "task")){
                    if(!apptinf.getmId().isEmpty()){
                        listApptinf.append(apptinf);
                        apptinf.reset();
                    }
                    apptinf.setmId(xml.attributes().value("id").toString());
                    apptinf.setmdur(xml.attributes().value("dur").toString());
                    apptinf.setmname(xml.attributes().value("name").toString());
                    apptinf.setmloc(xml.attributes().value("loc").toString());
                }
                if ((xml.name() == "inst")){
                    apptinf.setmalarmInstStartsave(xml.attributes().value("s").toString());

                    apptinf.setmdueDate(xml.attributes().value("dueDate").toString());
                }

                if ((xml.name() == "alarmData")){

                    apptinf.setmalarmInstStart(xml.attributes().value("alarmInstStart").toString());
                    apptinf.setmnextAlarm(xml.attributes().value("nextAlarm").toString());
                }

                if ((xml.name() == "Text")){
                    _authToken = "";
                    if (_error_cnt == 0){
                        startauth = true;
                    }
                    _error_cnt++;
                    if (_error.contains("authentication failed for")){
                        startauth = false;
                    }
                    _error = xml.readElementText().trimmed();

                    if (_error.contains("auth credentials have expired")){
                        startauth = true;
                        break;
                    }

                    trayIcon->showMessage(tr("Error:"), tr("%1").arg(_error), QSystemTrayIcon::Critical, 10000);
                    //}

                }


                //qDebug() << xml.name();
            }//******isStartElement
        }//while (!xml.atEnd())

        if(!apptinf.getmId().isEmpty()){
            listApptinf.append(apptinf);
            apptinf.reset();
        }

        if(!message.gId().isEmpty() && !message.gdate().isEmpty()){//проверяем заполнена ли last структура
            if (flagStateRX){//проверка на черный список
                listMessage.append(message);
                count_mess++;
                hashid = hashid + id;
                if (messageA.length()>0 || messageB.length()>0){
                    msg = msg + tr("From: %1\n%2\n").arg(messageB).arg(messageA);
                }
            }
        }


        if (xml.hasError()) {
#ifdef DBG
            qDebug() << "ERROR: xml " << xml.errorString();
#endif
            // do error handling
        }
        //******************************************************************
        //SearchResponse
        //******************************************************************
        if (SearchResponse) {
            qDebug() << "//SearchResponse" ;
            qDebug() << "//***   count_mess =" << count_mess << "******************************************************* ";
            if (count_mess > 0) { //***   count_mess > 0  *******************************************************

                setIcon(7);
                if (!timerIcon->isActive())
                    timerIcon->start(500);
                QString hashmessages = QString(QCryptographicHash::hash(hashid.toUtf8(),QCryptographicHash::Md5).toHex());
                if (settings.value("hashmessages", "") != hashmessages) { //***   hashmessages  *******************************************************
                    settings["hashmessages"] = hashmessages;
                    qDebug() << "//***   old_count_mess =" << settings.value("countmsg", "0").toInt() << "******************************************************* ";
                    if (settings.value("countmsg", "0").toInt() <= count_mess) {//***   count_mess > privios count_mess
                        if (ui->checkBox_2->checkState() == Qt::Checked) {//***   play sound
                            QSound::play("mail.wav");
                        }
                        QString more = "";
                        if (count_mess >= _msgget) {
                            more = tr("more than");
                        }
                        msg.truncate(1000);
                        trayIcon->setToolTip(
                                    tr("New messages %2 %1\n%3").arg(count_mess).arg(more).arg(
                                        msg));
                        if (ui->checkBox->checkState() == Qt::Checked) {
                            if (mess->check(listMessage))
                                showMessages(true);
                        }
                        else
                            trayIcon->showMessage(tr("New messages %2 %1").arg(count_mess).arg(more), tr("%1").arg(msg), QSystemTrayIcon::Information, 10000);
                    } //***   count_mess > previos count_mess
                    else
                        mess->check(listMessage);
                } //***   hashmessages  *******************************************************
            } else { //***   count_mess > 0  *******************************************************

                trayIcon->setToolTip(tr("Zimbra notification"));
                if (timerIcon->isActive())
                    timerIcon->stop();
                setIcon(2);
                mess->check(listMessage);
                showMessages(false);
                //                if (!mess->check(listMessage)) {
                //                    showMessages(false);
                //                }
            } //***   count_mess > 0  *******************************************************
            qDebug() << "//***   listApptinf.size() = " << listApptinf.size() << "******************************************************* ";
            if (listApptinf.size() > 0) { //***   есть напоминания  *******************************************************
                if (alarm->checkAppt(listApptinf) > 0) { //проверка на наличие их в таблице напоминаний
                    setIconAppt(alarm->getMsg());
                } else {
                    setIconAppt("");
                }
            } else {//***   есть напоминания  *******************************************************
                setIconAppt("");
            }//***   есть напоминания  *******************************************************
        }
        //******************************************************************
        //SearchRequest
        //******************************************************************
        //******************************************************************
        //DismissCalendarItemAlarmResponse
        //******************************************************************
        if (DismissCalendarItemAlarmResponse) {
            qDebug() << "//***   DismissCalendarItemAlarmResponse " << "******************************************************* ";
            if (alarm->checkCount()) { //есть напоминания
                setIconAppt(alarm->getMsg());
            } else {
                //                if (settings.value("countmsg", "0").toInt()) {
                //                    if (!timerIcon->isActive())
                //                        timerIcon->start(500);
                //                    setIcon(7);
                //                } else {
                //                    if (timerIcon->isActive())
                //                        timerIcon->stop();
                //                    setIcon(2);
                //                }
                setIconAppt("");
            }
        }
        //******************************************************************
        //DismissCalendarItemAlarmResponse
        //******************************************************************
        //******************************************************************
        //MsgActionResponse
        //******************************************************************
        if (MsgActionResponse) {
            qDebug() << "//***   MsgActionResponse " << "******************************************************* ";

            //            if (settings.value("countmsg", "0").toInt()) {
            //                if (!timerIcon->isActive())
            //                    timerIcon->start(500);
            //                setIcon(7);
            //            } else {
            //                if (timerIcon->isActive())
            //                    timerIcon->stop();
            //                setIcon(2);
            //            }
        }
        //******************************************************************
        //MsgActionResponse
        //******************************************************************


        if (!_error.isEmpty()){
            setIcon(1);
        }
        //qDebug() << "cont_mess:"<< count_mess;
        buffer = "";
        _ContentLength = 0;
        if(!DismissCalendarItemAlarmResponse)
            settings["countmsg"] = QString::number(count_mess);
    }
    if (startauth)
        sendMess();

}

void Zebra::acceptOK()
{
    if ((settings["URL"] != ui->lineSrv->text().trimmed())||
            (settings["Account"] != ui->lineLgn->text().trimmed())||
            (settings["Password"] != ui->linePwd->text().trimmed())){
        _authToken = "";
        _cookie = "";
        _error = "";
        _error_cnt = 0;
        if (socket){
            if (!(socket->state() == QAbstractSocket::UnconnectedState)) {
                socket->abort();
                setIcon(0);
            }
        }
    }
    //    settings["URL"] = ui->lineSrv->text().trimmed();
    //    settings["Account"] = ui->lineLgn->text().trimmed();
    //    settings["Password"] = ui->linePwd->text().trimmed();
    saveSettings();
    sendMess();

    //    if ((settings["Browser"] != ui->lineBrw->text().trimmed())||
    //            (settings["RX"] != ui->rxEdit->text())||
    //            (settings["Timeout"] != QString("%1").arg(ui->spinBox->value()))){
    //        saveSettings();

    //    }




    //    if (ui->checkBoxSSL->checkState() == Qt::Checked){
    //        settings["SSL"] = "true";
    //    }else{
    //        settings["SSL"] = "false";
    //    }

    //    if (ui->checkBox_2->checkState() == Qt::Checked){
    //        settings["Sound"] = "true";
    //    }else{
    //        settings["Sound"] = "false";
    //    }


}

void Zebra::acceptNO()
{

    ui->lineSrv->setText(settings.value("URL", ""));
    ui->lineLgn->setText(settings.value("Account", ""));
    ui->linePwd->setText(settings.value("Password", ""));
}

void Zebra::about()
{
    //ui->widget->hide();

    if (ui->stackedWidget->currentIndex() == 0){
        ui->stackedWidget->setCurrentIndex(1);
        viewAbout();
    }else{
        ui->stackedWidget->setCurrentIndex(0);
    }


}

void Zebra::viewAbout()
{

    if (settings.value("Browser", "").isEmpty()){
        QDesktopServices::openUrl(QUrl("http://zebra.ermoler.ru", QUrl::TolerantMode));
    }else{
        QProcess process;
        process.startDetached(settings.value("Browser", ""),QStringList() <<  "http://zebra.ermoler.ru");
    }

}
void Zebra::runBrw(const QString &auth)
{
    QString authstr = "https://"+ settings.value("URL") + auth;


    if(settings.value("Browser", "").trimmed().isEmpty()){
        QDesktopServices::openUrl(QUrl(authstr, QUrl::TolerantMode));
        return;
    }
#ifdef DBG
    qDebug() << settings.value("Browser", "");
#endif
    bool mail_server = true;
    QString brw = settings.value("Browser", "");
    if (brw.contains("$mail_server$",Qt::CaseInsensitive)){
        brw = brw.replace("$mail_server$",authstr,Qt::CaseInsensitive);
        mail_server = false;
    }
    QStringList opt = brw.replace(" -", " -$%-").split(" -");
    QStringList options;
    QString str;





#ifdef DBG
    qDebug() << opt;
#endif

    if (1 == opt.size()){ // параметры отсутсвуют
        if (mail_server)
            options.append(authstr);

    }else// есть параметры
        for (int i = 1; i < opt.size(); ++i){
            str = opt.at(i);
            if (i == opt.size() - 1){
                options.append(str.replace("$%-", " -").trimmed().split(' '));
                if (mail_server)
                    options.append(authstr);
            }else{
                options.append(str.replace("$%-", " -").trimmed().split(' '));
            }
        }
#ifdef DBG
    qDebug() << opt.at(0).toLocal8Bit()<< options;
#endif

    QProcess process;
    //process.startDetached(settings.value("Browser", ""),QStringList() << " -k " << "https://"+ settings.value("URL") +auth);
    process.startDetached(opt.at(0).toLocal8Bit(),options);
}


void Zebra::viewMail()
{
#ifdef Q_OS_WIN32
    if (showBrowser())
        return;
#endif
    if (_authToken != ""){
        runBrw("/service/preauth?isredirect=1&authtoken=" + _authToken);
    }else{
        runBrw("");
    }
}

void Zebra::viewmessMail()
{

        emit addFilter(ui->rxEdit->toPlainText());
        mess->setVisible(mess->isHidden());

}



void Zebra::changeBrowser()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.exe");

    if (!str.isEmpty())
    {
        ui->lineBrw->setText(str);

    }
}

void Zebra::updateTrayIcon()
{
    //qDebug() << ord;
    if (ord){
        trayIcon->setIcon(QIcon(":/images/zebra03.svg"));
        ord = 0;
    }
    else{
        trayIcon->setIcon(QIcon(":/images/zebra07.svg"));
        ord = 1;
    }

}

void Zebra::initSettings()
{

    _authToken = "";
    _cookie = "";
    _ContentLength = 0;
    _state = "ConnectedState";
    _msgget = 40;
    _error = "";
    _error_cnt = 0;

    //Заносим в settings значения из реестра
    settings["URL"] = QString(QByteArray::fromBase64(
                                  configs.value("URL",
                                                QString(QString("").toUtf8().toBase64())).toByteArray()));
    settings["Account"] = QString(QByteArray::fromBase64( configs.value("Account", "").toByteArray()));
    settings["Password"] = QString(QByteArray::fromBase64( configs.value("Password", "").toByteArray()));
    settings["Browser"] = QString(QByteArray::fromBase64( configs.value("Browser", "").toByteArray()));
    settings["RX"] = QString(QByteArray::fromBase64( configs.value("RX", "").toByteArray()));
    settings["stateRX"] = QString(QByteArray::fromBase64(
                                      configs.value("stateRX",
                                                    QString(QString("false").toUtf8().toBase64())).toByteArray()));
    settings["SSL"] = configs.value("SSL", "true").toString();
    settings["Sound"] = configs.value("Sound", "false").toString();
    settings["Window"] = configs.value("Window", "false").toString();
    settings["Timeout"] = configs.value("Timeout", "60").toString();
    settings["hashmessages"] = configs.value("hashmessages", "").toString();
    settings["countmsg"] = configs.value("countmsg", "0").toString();


    //Запоняем значениями элементы управления окна
    ui->lineSrv->setText(settings.value("URL", ""));
    ui->lineLgn->setText(settings.value("Account", ""));
    ui->linePwd->setText(settings.value("Password", ""));
    ui->lineBrw->setText(settings.value("Browser", ""));
    ui->rxEdit->setPlainText(settings.value("RX", ""));
    ui->spinBox->setValue(settings.value("Timeout", "60").toInt());

    if ((settings.value("Window", "false") == "true")){
        ui->checkBox->setChecked(true);
    }
    if ((settings.value("Sound", "false") == "true")){
        ui->checkBox_2->setChecked(true);
    }
    if ((settings.value("SSL", "true") == "false")){
        ui->checkBoxSSL->setChecked(false);
    }
    if ((settings.value("stateRX", "false") == "true")){
        ui->pushButton_RX->setText(tr("White list"));
        ui->pushButton_RX->setChecked(true);
    }else{
        ui->pushButton_RX->setChecked(false);
    }
    if ((settings.value("URL", "") == "") || (settings.value("Account", "") == "")){
        showNormal();
    }

    trayIcon->setToolTip(tr("Zimbra notification"));
    ui->labelInfo->setText("");

    QSettings *autorun = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(autorun->value( "ZebraTray","").toString() != "")
    {
        ui->autoRun->setChecked(true);
    }
    restoreGeometry(configs.value("Geometry","").toByteArray());
    alarm->restoreGeometry(configs.value("GeometryAppt","").toByteArray());
    mess->restoreGeometry(configs.value("GeometryMess","").toByteArray());

}

void Zebra::saveSettings()
{
    settings["URL"] = ui->lineSrv->text().trimmed();
    settings["Account"] = ui->lineLgn->text().trimmed();
    settings["Password"] = ui->linePwd->text().trimmed();
    settings["Browser"] = ui->lineBrw->text();
    settings["RX"] = ui->rxEdit->toPlainText();
    settings["Timeout"] = QString("%1").arg(ui->spinBox->value());

    if (ui->checkBoxSSL->checkState() == Qt::Checked){
        settings["SSL"] = "true";
    }else{
        settings["SSL"] = "false";
    }

    if (ui->checkBox_2->checkState() == Qt::Checked){
        settings["Sound"] = "true";
    }else{
        settings["Sound"] = "false";
    }

    if (ui->checkBox->checkState() == Qt::Checked){
        settings["Window"] = "true";
    }else{
        settings["Window"] = "false";
    }

    configs.setValue("URL",settings.value("URL", "").toUtf8().toBase64());
    configs.setValue("Account",settings.value("Account", "").toUtf8().toBase64());
    configs.setValue("Password",settings.value("Password", "").toUtf8().toBase64());
    configs.setValue("Browser",settings.value("Browser", "").toUtf8().toBase64());
    configs.setValue("RX",settings.value("RX", "").toUtf8().toBase64());
    configs.setValue("stateRX",settings.value("stateRX", "false").toUtf8().toBase64());
    configs.setValue("Timeout",settings.value("Timeout", "60"));
    configs.setValue("SSL",settings.value("SSL", "true"));
    configs.setValue("Sound",settings.value("Sound", "false"));
    configs.setValue("Window",settings.value("Window", "false"));
    configs.setValue("Geometry", saveGeometry());
    configs.setValue("GeometryAppt", alarm->saveGeometry());
    configs.setValue("GeometryMess", mess->saveGeometry());

}



void Zebra::autoRun_slot()
{
#ifdef Q_OS_WIN32
    //Добавляем в автозагрузку пользователя
    QSettings *autorun = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    if(ui->autoRun->isChecked())
    {
        autorun->setValue("ZebraTray", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
        autorun->sync();

        ui->autoRun->setToolTip(tr("Disable autorun"));
    }
    else
    {
        autorun->remove("ZebraTray");
        ui->autoRun->setToolTip(tr("Enable autorun"));
    }
    delete autorun;
#endif
}
