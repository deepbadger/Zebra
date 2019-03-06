#ifndef INF_H
#define INF_H

#include <QMetaType>
#include <QString>

class inf
{
  QString mId;
  QString mname;
  QString mloc;
  QString mdur;
  QString malarmInstStart;
  QString mnextAlarm;
  QString mdueDate;


public:
  inf();
  inf(const inf &A);

  QString getmId();
  QString getmname();
  QString getmloc();
  QString getmdur();
  QString getmalarmInstStart();
  QString getmnextAlarm();
  QString getmdueDate();
  QString getstr();
  QString getstrMsg();
  QString setmId(const QString &A);
  QString setmname(const QString &A);
  QString setmloc(const QString &A);
  QString setmdur(const QString &A);
  QString setmdueDate(const QString &A);
  QString setmalarmInstStart(const QString &A);
  QString setmalarmInstStartsave(const QString &A);
  QString setmnextAlarm(const QString &A);
  bool operator ==(inf );
  void reset();
};


class inf_mail
{
  QString Id;
  QString name;
  QString from;
  QString date;
  QString subject;
  QString fragment;
  QString mId;
  QString size;

public:
  inf_mail();
  inf_mail(const inf_mail &A);
  QString getId();
  QString getname();
  QString getfrom();
  QString getdate();
  QString getsubject();
  QString getfragment();
  QString getmid();
  QString getsize();

  void setId(const QString &A);
  void setname(const QString &A);
  void setfrom(const QString &A);
  void setdate(const QString &A);
  void setsubject(const QString &A);
  void setfragment(const QString &A);
  void setmid(const QString &A);
  void setsize(const QString &A);

  bool operator ==(inf_mail );
  void reset();
};

//Q_DECLARE_METATYPE(inf)
#endif // INF_H
