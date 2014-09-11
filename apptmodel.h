#ifndef APPTMODEL_H
#define APPTMODEL_H

#include <QAbstractTableModel>
#include "inf.h"
//class inf;


class ApptModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ApptModel(QObject *parent = 0);
    void setAppt(const QList<QString> mId,QMap <QString,QString> nextAlarm,
                 QMap <QString,QString> alarmInstStart,
                 QMap <QString,QString> invId,
                 QMap <QString,QString> name,
                 QMap <QString,QString> loc);

    int checkAppt(const QList<QString> mId);
    int checkAppt(const QList<inf> &listAppt);
    int dismissAppt( inf infor);

    int appendAppt(const QString Id, QString nextAlarm, QString alarmInstStart, QString invId, QString name, QString loc);

    void getAppt(int row, inf &infor);
    QString getMsg();
    void upd();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    QList<inf> Appt;
    QList<inf> black;
    QList<QString> mId;
    QMap <QString,QString> mnextAlarm;
    QMap <QString,QString> malarmInstStart;
    QMap <QString,QString> minvId;
    QMap <QString,QString> mname;
    QMap <QString,QString> mloc;

    
};

#endif // APPTMODEL_H
