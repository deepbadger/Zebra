#include <QtGui>

#include "zebra.h"

//#include <QtDebug>
//#include <QFile>
//#include <QTextStream>

//void myMessageHandler(QtMsgType type, const char *msg)
//{
//QString txt;
//switch (type) {
//case QtDebugMsg:
//txt = QString("Debug: %1").arg(msg);
//break;
//case QtWarningMsg:
//txt = QString("Warning: %1").arg(msg);
//break;
//case QtCriticalMsg:
//txt = QString("Critical: %1").arg(msg);
//break;
//case QtFatalMsg:
//txt = QString("Fatal: %1").arg(msg);
//abort();
//}
//QFile outFile("log.log");
//outFile.open(QIODevice::WriteOnly | QIODevice::Append);
//QTextStream ts(&outFile);
//ts << txt << endl;
//}


int main(int argc, char *argv[])
{

    Q_INIT_RESOURCE(systray);

    QApplication app(argc, argv);
       // qInstallMsgHandler(myMessageHandler);

    //qRegisterMetaType<inf>();

    QCoreApplication::setOrganizationName("BadgerSoftware");
    QCoreApplication::setOrganizationDomain("chtz.ru");
    QCoreApplication::setApplicationName("ZebraTray");

//    QTranslator qtTranslator;
//    qtTranslator.load("qt_" + QLocale::system().name(),
//                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//    app.installTranslator(&qtTranslator);



    QTranslator translator;
    if (!translator.load("zebra_"+QLocale::system().name(), ":/lang"))
        translator.load("zebra_"+QLocale::system().name());
    app.installTranslator(&translator);


//    QMessageBox::critical(0, QObject::tr("Systray"),
//                          QLocale::system().name());

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Zebra window;
    //window.show();
    return app.exec();
}
