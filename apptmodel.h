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

  mresult checkAppt(const QList<QString> mId);
  mresult checkAppt(const QList<inf_appt> &listAppt);
  int dismissAppt( inf_appt infor);

  int appendAppt(const QString Id, QString nextAlarm, QString alarmInstStart, QString invId, QString name, QString loc);

  void getAppt(int row, inf_appt &infor);
  QString getMsg();
  void upd();

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
  QList<inf_appt> Appt;
  QList<inf_appt> black;
  QList<QString> mId;
  QMap <QString,QString> mnextAlarm;
  QMap <QString,QString> malarmInstStart;
  QMap <QString,QString> minvId;
  QMap <QString,QString> mname;
  QMap <QString,QString> mloc;


};

#endif // APPTMODEL_H
