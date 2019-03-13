#include "inf.h"


#include <QString>
#include <QDateTime>

inf_appt::inf_appt()
{
  mId.clear();
  mname.clear();
  mloc.clear();
  mdur.clear();
  malarmInstStart.clear();
  mnextAlarm.clear();
  mdueDate.clear();

}

inf_appt::inf_appt(const inf_appt &A)
{
  mId = A.mId;
  mname = A.mname;
  mloc = A.mloc;
  mdur = A.mdur;
  malarmInstStart = A.malarmInstStart;
  mnextAlarm = A.mnextAlarm;
  mdueDate = A.mdueDate;

}

QString inf_appt::getmId()
{
  return mId;
}

QString inf_appt::getmname()
{
  return mname;
}

QString inf_appt::getmloc()
{
  return mloc;
}

QString inf_appt::getmdur()
{
  if (mdueDate.isEmpty())
    return mdur;
  else
    return QString("%1").arg(mdueDate.toLongLong() - malarmInstStart.toLongLong());

  return "";
}

QString inf_appt::getmalarmInstStart()
{
  return malarmInstStart;
}

QString inf_appt::getmnextAlarm()
{
  return mnextAlarm;
}

QString inf_appt::getmdueDate()
{
  return mdueDate;
}

QString inf_appt::getstr()
{
  qint64 end = malarmInstStart.toLongLong() + mdur.toLongLong();
  return QString("name: %2 inst: %5 end: %7 id: %1 loc: %3 dur: %4  na: %6").arg(mId)
         .arg(mname).arg(mloc).arg(mdur).arg(QDateTime::fromMSecsSinceEpoch(malarmInstStart.toLongLong()).toString())
         .arg(QDateTime::fromMSecsSinceEpoch(mnextAlarm.toLongLong()).toString())
         .arg(QDateTime::fromMSecsSinceEpoch(end).toString());
}

QString inf_appt::getstrMsg( )
{
  qint64 now = QDateTime::currentMSecsSinceEpoch();
  qint64 msecs = 0, start = 0,end = 0,alarm = 0;
  start = malarmInstStart.toLongLong();
  end = start + mdur.toLongLong();
  alarm = mnextAlarm.toLongLong();

  QString formattedTime;
  if((abs(now - start) > 86400000)||(abs(now - end) > 86400000))
    formattedTime = QString("%1-%2").arg( QDateTime::fromMSecsSinceEpoch(start).toString("dd.MM.yy hh:mm")).arg( QDateTime::fromMSecsSinceEpoch(end).toString("dd.MM.yy hh:mm"));
  else
    formattedTime = QString("%1-%2").arg( QDateTime::fromMSecsSinceEpoch(start).toString("hh:mm")).arg( QDateTime::fromMSecsSinceEpoch(end).toString("hh:mm"));

  //    if (now > start){
  //        msecs = now - start;
  //        formattedTime.append(" ");
  //    }else{
  //        formattedTime.append(" ");
  //        msecs = start - now;
  //    }
  //    int days = msecs/86400000;
  //    int hours = (msecs - (days * 86400000))/(1000*60*60);
  //    int minutes = (msecs-(hours*1000*60*60))/(1000*60);
  //    int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60))/1000;

  //    if(days)
  //        formattedTime.append(QString( "%1 days").arg(days, 2, 10, QLatin1Char('0')));

  //    else if(hours)
  //        formattedTime.append(QString("%1 hours").arg(hours, 2, 10, QLatin1Char('0')));
  //    else
  //        formattedTime.append(QString( "%1 minutes" ).arg(minutes, 2, 10, QLatin1Char('0')));
  return mname + ": " + formattedTime + "";
}

QString inf_appt::setmId(const QString &A)
{
  mId = A;
  return mId;
}

QString inf_appt::setmname(const QString &A)
{
  mname = A;
  return mId;
}
QString inf_appt::setmloc(const QString &A)
{
  mloc = A;
  return mId;
}
QString inf_appt::setmdur(const QString &A)
{
  mdur = A;
  return mId;
}

QString inf_appt::setmdueDate(const QString &A)
{

  mdueDate = A;

  return mId;
}

QString inf_appt::setmalarmInstStart(const QString &A)
{
  //if(malarmInstStart.isEmpty())
  malarmInstStart = A;
  return mId;
}
QString inf_appt::setmalarmInstStartsave(const QString &A)
{
  if(malarmInstStart.isEmpty())
    malarmInstStart = A;

  return mId;
}
QString inf_appt::setmnextAlarm(const QString &A)
{
  if(mnextAlarm.isEmpty())
    mnextAlarm = A;
  return mId;
}

bool inf_appt::operator ==(inf_appt A)
{
  if ((mId == A.mId)&&(mname == A.mname)&&(mloc == A.mloc)&&(mdur == A.mdur)&&(malarmInstStart == A.malarmInstStart))
    return true;
  return false;
}

void inf_appt::reset()
{
  mId.clear();
  mname.clear();
  mloc.clear();
  mdur.clear();
  malarmInstStart.clear();
  mnextAlarm.clear();
  mdueDate.clear();

}
//inf_mail-----------------------------------------------------------------------------------------------------------------
//inf_mail-----------------------------------------------------------------------------------------------------------------
//inf_mail-----------------------------------------------------------------------------------------------------------------
inf_mail::inf_mail()
{
  Id.clear();
  name.clear();
  from.clear();
  date.clear();
  subject.clear();
  from.clear();
  mId.clear();
}

inf_mail::inf_mail(const inf_mail &A)
{
  Id = A.Id;
  name = A.name;
  from = A.from;
  date = A.date;
  subject= A.subject;
  fragment = A.fragment;
  mId=A.mId;
}

QString inf_mail::getId()
{
  return Id;
}
QString inf_mail::getname()
{
  return name;
}
QString inf_mail::getfrom()
{
  return from;
}
QString inf_mail::getdate()
{
  return date;
}
QString inf_mail::getsubject()
{
  return subject;
}
QString inf_mail::getfragment()
{
  return fragment;
}
QString inf_mail::getmid()
{
  return mId;
}

void inf_mail::setId(const QString &A)
{
  Id = A;
}
void inf_mail::setname(const QString &A)
{
  name = A;
}
void inf_mail::setfrom(const QString &A)
{
  from = A;
}
void inf_mail::setdate(const QString &A)
{
  date = A;
}
void inf_mail::setsubject(const QString &A)
{
  subject = A;
}
void inf_mail::setfragment(const QString &A)
{
  fragment = A;
}
void inf_mail::setmid(const QString &A)
{
  if (!mId.isEmpty())
    mId = mId +","+ A  ;
  else
    mId = A;
}

bool inf_mail::operator ==(inf_mail A)
{

  if ((Id == A.Id)&&(name == A.name)&&(from == A.from)&&(date == A.date)&&(subject == A.subject)&&(fragment == A.fragment)&&(mId == A.mId))
    return true;
  return false;

}

void inf_mail::reset()
{
  Id.clear();
  name.clear();
  from.clear();
  date.clear();
  subject.clear();
  fragment.clear();
  mId.clear();
}

void mresult::setresult(int sz, int ap, int rm, int bl )
{
  this->size = sz;
  this->append = ap;
  this->remove = rm;
  this->black = bl;
}

int mresult::getsize()
{
  return this->size;
}
int mresult::getappend()
{
  return this->append;
}
int mresult::getremove()
{
  return this->remove;
}
int mresult::getblack()
{
  return this->black;
}

QString account::geturl()
{
  return this->url;
}
QString account::getlogin()
{
  return this->login;
}
QString account::getpassword()
{
  return this->password;
}
void account::set(QString u, QString l, QString p)
{
  this->url = u;
  this->login = l;
  this->password = p;
}
void account::seturl(QString u)
{
  this->url = u;
}
void account::setlogin(QString l)
{
  this->login = l;
}
void account::setpassword(QString p)
{
  this->password = p;
}

