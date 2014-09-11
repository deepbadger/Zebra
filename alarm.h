#ifndef ALARM_H
#define ALARM_H

#include <QWidget>

#include <QItemSelection>



namespace Ui {
class Alarm;
}

class inf;
class ApptModel;
class tableDelegate;


class Alarm : public QWidget
{
    Q_OBJECT
    
public:
    explicit Alarm(QWidget *parent = 0);
    ~Alarm();

    int appendAppt(const QString Id, QString nextAlarm, QString alarmInstStart, QString invId, QString name, QString loc);
    int checkAppt(const QList<QString> mId);
    int checkAppt(const QList<inf> listAppt);
    int checkCount();
    QString getMsg();
    void upd();
    //void setGeo(int l,int r,int t,int b);

//protected:
    //bool eventFilter(QObject *obj, QEvent *ev);
signals:
    void emul(QString);
    void open();

private slots:
    void slotLoadTransaction(const QItemSelection & sel, const QItemSelection & desel);
    void acceptDismiss();
    void acceptOpen();
    //void acceptOK();
    //void acceptNO();

private:
    Ui::Alarm *ui;
    ApptModel *apptModel;
    tableDelegate *delegat;
    int _cnt;
};

#endif // ALARM_H
