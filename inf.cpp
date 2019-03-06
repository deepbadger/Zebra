#include "inf.h"


#include <QString>
#include <QDateTime>

inf::inf()
{
  mId.clear();
  mname.clear();
  mloc.clear();
  mdur.clear();
  malarmInstStart.clear();
  mnextAlarm.clear();
  mdueDate.clear();

}

inf::inf(const inf &A)
{
  mId = A.mId;
  mname = A.mname;
  mloc = A.mloc;
  mdur = A.mdur;
  malarmInstStart = A.malarmInstStart;
  mnextAlarm = A.mnextAlarm;
  mdueDate = A.mdueDate;

}

QString inf::getmId()
{
  return mId;
}

QString inf::getmname()
{
  return mname;
}

QString inf::getmloc()
{
  return mloc;
}

QString inf::getmdur()
{
  if (mdueDate.isEmpty())
    return mdur;
  else
    return QString("%1").arg(mdueDate.toLongLong() - malarmInstStart.toLongLong());

  return "";
}

QString inf::getmalarmInstStart()
{
  return malarmInstStart;
}

QString inf::getmnextAlarm()
{
  return mnextAlarm;
}

QString inf::getmdueDate()
{
  return mdueDate;
}

QString inf::getstr()
{
  qint64 end = malarmInstStart.toLongLong() + mdur.toLongLong();
  return QString("name: %2 inst: %5 end: %7 id: %1 loc: %3 dur: %4  na: %6").arg(mId)
         .arg(mname).arg(mloc).arg(mdur).arg(QDateTime::fromMSecsSinceEpoch(malarmInstStart.toLongLong()).toString())
         .arg(QDateTime::fromMSecsSinceEpoch(mnextAlarm.toLongLong()).toString())
         .arg(QDateTime::fromMSecsSinceEpoch(end).toString());
}

QString inf::getstrMsg( )
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

QString inf::setmId(const QString &A)
{
  mId = A;
  return mId;
}

QString inf::setmname(const QString &A)
{
  mname = A;
  return mId;
}
QString inf::setmloc(const QString &A)
{
  mloc = A;
  return mId;
}
QString inf::setmdur(const QString &A)
{
  mdur = A;
  return mId;
}

QString inf::setmdueDate(const QString &A)
{

  mdueDate = A;

  return mId;
}

QString inf::setmalarmInstStart(const QString &A)
{
  //if(malarmInstStart.isEmpty())
  malarmInstStart = A;
  return mId;
}
QString inf::setmalarmInstStartsave(const QString &A)
{
  if(malarmInstStart.isEmpty())
    malarmInstStart = A;

  return mId;
}
QString inf::setmnextAlarm(const QString &A)
{
  if(mnextAlarm.isEmpty())
    mnextAlarm = A;
  return mId;
}

bool inf::operator ==(inf A)
{
  if ((mId == A.mId)&&(mname == A.mname)&&(mloc == A.mloc)&&(mdur == A.mdur)&&(malarmInstStart == A.malarmInstStart))
    return true;
  return false;
}

void inf::reset()
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
