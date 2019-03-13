#ifndef INF_H
#define INF_H

#include <QMetaType>
#include <QString>
class account
{
  QString url;
  QString login;
  QString password;
public:
  QString geturl();
  QString getlogin();
  QString getpassword();
  void set(QString u, QString l, QString p);
  void seturl(QString u);
  void setlogin(QString l);
  void setpassword(QString p);
};

class inf_appt
{
  QString mId;
  QString mname;
  QString mloc;
  QString mdur;
  QString malarmInstStart;
  QString mnextAlarm;
  QString mdueDate;


public:
  inf_appt();
  inf_appt(const inf_appt &A);

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
  bool operator ==(inf_appt );
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

class mresult
{
  int size ;
  int append;
  int remove;
  int black;
public:
  void setresult(int sz = 0, int ap = 0, int rm = 0, int bl = 0);
  int getsize();
  int getappend();
  int getremove();
  int getblack();
};

//Q_DECLARE_METATYPE(inf)
#endif // INF_H
