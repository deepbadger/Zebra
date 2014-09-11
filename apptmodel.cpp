#include "inf.h"
#include "apptmodel.h"


#include <QDebug>
#include <QDateTime>
#include <QColor>


ApptModel::ApptModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void ApptModel::setAppt(const QList<QString> Id, QMap<QString, QString> nextAlarm, QMap<QString, QString> alarmInstStart, QMap<QString, QString> invId, QMap<QString, QString> name, QMap<QString, QString> loc)
{
    for (int i = 0; i < mId.size(); ++i) {
        if (!Id.contains(mId.at(i))){
            mnextAlarm.remove(mId.at(i));
            malarmInstStart.remove(mId.at(i));
            minvId.remove(mId.at(i));
            mname.remove(mId.at(i));
            mloc.remove(mId.at(i));
        }
    }
    mId.clear();
    mId.append(Id);
    for (int i = 0; i < mId.size(); ++i) {
        if (!Id.contains(mId.at(i))){
            mnextAlarm.insert(mId.at(i),nextAlarm.value(mId.at(i))) ;
            malarmInstStart.insert(mId.at(i),alarmInstStart.value(mId.at(i))) ;
            minvId.insert(mId.at(i),invId.value(mId.at(i))) ;
            mname.insert(mId.at(i),name.value(mId.at(i))) ;
            mloc.insert(mId.at(i),loc.value(mId.at(i))) ;
        }
    }
}

int ApptModel::checkAppt(const QList<QString> Id)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList <QString> toRemove;

    for (int i = 0; i < mId.size(); ++i) {
        QString nextAlarm = mnextAlarm.value(mId.at(i));
        if (QDateTime::fromMSecsSinceEpoch(nextAlarm.toLongLong()).toMSecsSinceEpoch() > now){
            toRemove.append(mId.at(i));
        }
    }

    for (int i = 0; i < mId.size(); ++i) {
        if (!Id.contains(mId.at(i))){
            toRemove.append(mId.at(i));
        }
    }

    for (int i = 0; i < toRemove.size(); ++i) {
        qDebug() << toRemove.at(i);
        int ind = mId.indexOf(toRemove.at(i));
        beginRemoveRows(QModelIndex(), ind, ind);
        mId.removeAt(ind);
        mnextAlarm.remove(toRemove.at(i));
        malarmInstStart.remove(toRemove.at(i));
        minvId.remove(toRemove.at(i));
        mname.remove(toRemove.at(i));
        mloc.remove(toRemove.at(i));
        endRemoveRows();
    }
    return mId.size();

}

int ApptModel::checkAppt(const QList<inf> &listAppt)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList <inf> toRemove;
    QList <inf> toRemoveBlack;
    QList <inf> toAppend;
    inf A,B;
    qint64 start = 0,end = 0,alarm = 0;

    for (int i = 0; i < black.size(); ++i) {
        A = black.at(i);
        start = A.getmalarmInstStart().toLongLong();
        end = start + A.getmdur().toLongLong();
        if(!listAppt.contains(A)){
            if (now > end){
                toRemoveBlack.append(A);
                //qDebug() << "toRemoveBlack";
            }
        }
    }



    for (int i = 0; i < listAppt.size(); ++i) {
        A = listAppt.at(i);
        //qDebug() << "listApt " << A.getstr();
        start = A.getmalarmInstStart().toLongLong();
        end = start + A.getmdur().toLongLong();
        alarm = A.getmnextAlarm().toLongLong();
        if (black.contains(A)){
            toRemove.append(A);
            //qDebug() << "toRemove d";
            continue;
        }
        if(alarm > now){
            toRemove.append(A);
            //qDebug() << "toRemove a";

        }else{
            if(alarm > now){
                toAppend.append(A);
                //qDebug() << "toAppend";
            }
            if((alarm == 0)and((now < start) or (now > end))){
                toRemove.append(A);
                //qDebug() << "toRemove b";
            }else{
                toAppend.append(A);
                //qDebug() << "toAppend";
            }
        }
    }

    for (int i = 0; i < Appt.size(); ++i) {
        A = Appt.at(i);
        if (!listAppt.contains(A)){
            toRemove.append(A);
            // qDebug() << "toRemove c";
        }
    }




    //qDebug() <<toAppend.size()<<toRemove.size()<<black.size() << Appt.size();
    for (int i = 0; i < toRemove.size(); ++i) {
        inf rm = toRemove.at(i);
        if(Appt.contains(rm)){
            int ind = Appt.indexOf(rm);
            beginRemoveRows(QModelIndex(), ind, ind);
            Appt.removeAll(rm);
            endRemoveRows();
        }
    }




    int k = 0;
    for (int i = 0; i < toAppend.size(); ++i) {
        A = toAppend.at(i);
        k = 0;
        if(!Appt.contains(A)){
            for ( int j = 0; j < Appt.size(); ++j) {
                B = Appt.at(j);
                //qDebug() << A.getmname() <<A.getmalarmInstStart() << B.getmalarmInstStart()<< B.getmname();
                if(A.getmalarmInstStart().toLongLong() < B.getmalarmInstStart().toLongLong()){
                    k = j + 1;
                }
            }
            beginInsertRows(QModelIndex(), k, k );
            //qDebug() << k<< A.getmname();
            Appt.insert(k,A);
            endInsertRows();
        }
    }


//    for (int i = 0; i < toAppend.size(); ++i) {
//        inf in = toAppend.at(i);
//        if(!Appt.contains(in)){
//            beginInsertRows(QModelIndex(), Appt.size() + 1, Appt.size() +1 );
//            Appt.append(in);
//            endInsertRows();
//        }
//    }

    for (int i = 0; i < toRemoveBlack.size(); ++i) {
        inf rm = toRemoveBlack.at(i);
        black.removeAll(rm);
    }
    //qDebug() <<toAppend.size()<<toRemove.size()<<black.size() << Appt.size();
    return Appt.size();
}

int ApptModel::dismissAppt(inf infor)
{
    if(!black.contains(infor)){
        black.append(infor);
        //qDebug() << "dismiss black"  << infor.getstr();
    }

    //    if(Appt.contains(infor)){
    //        int ind = Appt.indexOf(infor);
    //        beginRemoveRows(QModelIndex(), ind, ind);
    //        Appt.removeOne(infor);
    //        endRemoveRows();
    //    }


    return 0;
}

int ApptModel::appendAppt(const QString Id, QString nextAlarm, QString alarmInstStart, QString invId, QString name, QString loc)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    //qDebug() << "id: " << Id << " now: " << now << " nextAlarm: " << nextAlarm << " alarmInstStart: " << alarmInstStart << invId << name << loc;

    if (QDateTime::fromMSecsSinceEpoch(nextAlarm.toLongLong()).toMSecsSinceEpoch() > now){
        return mId.size();
    }

    if (malarmInstStart.contains(Id)){
        mnextAlarm.insert(Id,nextAlarm);//Когда уведомить
        malarmInstStart.insert(Id,alarmInstStart);//Начало мероприятия
        minvId.insert(Id,invId) ;
        mname.insert(Id,name) ;
        mloc.insert(Id,loc) ;
        emit dataChanged(QModelIndex(),QModelIndex());
        return mId.size();
    }


    beginInsertRows(QModelIndex(), mId.size() + 1, mId.size() +1 );
    mId.append(Id);
    mnextAlarm.insert(Id,nextAlarm);//Когда уведомить
    malarmInstStart.insert(Id,alarmInstStart);//Начало мероприятия
    minvId.insert(Id,invId) ;
    mname.insert(Id,name) ;
    mloc.insert(Id,loc) ;
    endInsertRows();


    return mId.size();
}

void ApptModel::getAppt(int row, inf &infor)
{

    inf id = Appt.at(row);
    infor = id;

}


void ApptModel::upd()
{

    //this->reset();

    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight);

}


QString ApptModel::getMsg()
{
    inf A;
    QString res = "";
    for (int i = 0; i < Appt.size(); ++i) {
        A = Appt.at(i);
        res = res + "\n "+ A.getstrMsg();
    }
    return res;

}


int ApptModel::rowCount(const QModelIndex &parent) const
{
    return Appt.size();
}

int ApptModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ApptModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    inf row = Appt.at(index.row());
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 msecs = 0, start = 0,end = 0,alarm = 0;
    start = row.getmalarmInstStart().toLongLong();
    alarm = row.getmnextAlarm().toLongLong();
    end = start + row.getmdur().toLongLong();

    if (index.column() == 0){
        QString formattedTimeRange,formattedTime;


        if (now > start){
            msecs = now - start;
            formattedTime.append(tr("<div style=\"font-weight: bold; color: red;\">Overdue by "));
        }else{
            formattedTime.append(tr("<div style=\"font-weight: bold; color: green;\">In "));
            msecs = start - now;
        }
        int days = msecs/86400000;
        int hours = (msecs - (days * 86400000))/(1000*60*60);
        int minutes = (msecs-(hours*1000*60*60)- (days * 86400000))/(1000*60);
        int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60)- (days * 86400000))/1000;

        if(days)
            formattedTime.append(QString( tr("%1 days")).arg(days, 2, 10, QLatin1Char('0')));

        else if(hours)
            formattedTime.append(QString(tr("%1 hours,")).arg(hours, 2, 10, QLatin1Char('0')) + " " +
                                 QString( tr("%1 minutes") ).arg(minutes, 2, 10, QLatin1Char('0')));
        else
            formattedTime.append(QString( tr("%1 minutes") ).arg(minutes, 2, 10, QLatin1Char('0')));

        if(days)
            if(abs(end - start) >= 86400000)
                formattedTimeRange = QString("%1 - %2").arg( QDateTime::fromMSecsSinceEpoch(start).toString(Qt::DefaultLocaleShortDate)).arg( QDateTime::fromMSecsSinceEpoch(end).toString(Qt::DefaultLocaleShortDate));
            else
                formattedTimeRange = QString("%1 - %2").arg( QDateTime::fromMSecsSinceEpoch(start).toString(Qt::DefaultLocaleShortDate)).arg( QDateTime::fromMSecsSinceEpoch(end).toString("hh:mm"));
        else
            if(abs(end - start) >= 86400000)
                formattedTimeRange = QString("%1 - %2").arg( QDateTime::fromMSecsSinceEpoch(start).toString(Qt::DefaultLocaleShortDate)).arg( QDateTime::fromMSecsSinceEpoch(end).toString(Qt::DefaultLocaleShortDate));
            else
                formattedTimeRange = QString("%1 - %2").arg( QDateTime::fromMSecsSinceEpoch(start).toString("hh:mm")).arg( QDateTime::fromMSecsSinceEpoch(end).toString("hh:mm"));





        return QString("<div align=\"right\" style=\"font-weight: bold;\">%2</div><div style=\"font-weight: bold;\">%1</div>%3</div>")
                .arg(row.getmname())
                .arg(formattedTimeRange)
                .arg(formattedTime);
    } else if (index.column() == 1){
        QString formattedTime;
        if(abs(end - start) >= 86400000)//"dd.MM.yy hh:mm"
            formattedTime = QString("%1 - %2").arg( QDateTime::fromMSecsSinceEpoch(start).toString( Qt::SystemLocaleShortDate)).arg( QDateTime::fromMSecsSinceEpoch(end).toString("dd.MM.yy hh:mm"));
        else//"hh:mm"
            formattedTime = QString("%1 - %2").arg( QDateTime::fromMSecsSinceEpoch(start).toString( Qt::SystemLocaleShortDate)).arg( QDateTime::fromMSecsSinceEpoch(end).toString("hh:mm"));
        return formattedTime;
    }else if (index.column() == 2){
        QString formattedTime;
        if (now > start){
            msecs = now - start;
            formattedTime.append(tr("Overdue by "));
        }else{
            formattedTime.append(tr("In "));
            msecs = start - now;
        }
        int days = msecs/86400000;
        int hours = (msecs - (days * 86400000))/(1000*60*60);
        int minutes = (msecs-(hours*1000*60*60)- (days * 86400000))/(1000*60);
        int seconds = (msecs-(minutes*1000*60)-(hours*1000*60*60)- (days * 86400000))/1000;

        if(days)
            formattedTime.append(QString( tr("%1 days")).arg(days, 2, 10, QLatin1Char('0')));

        else if(hours)
            formattedTime.append(QString(tr("%1 hours,")).arg(hours, 2, 10, QLatin1Char('0')) + " " +
                                 QString( tr("%1 minutes") ).arg(minutes, 2, 10, QLatin1Char('0')));
        else
            formattedTime.append(QString( tr("%1 minutes") ).arg(minutes, 2, 10, QLatin1Char('0')));



        //        formattedTime.append(by +
        //                             QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
        //                             QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
        //                             QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')));
        return formattedTime;
    }else if (index.column() == 3)
        return row.getmloc();
    else if (index.column() == 4)
        return row.getmname();
    if (index.column() == 5)
        return row.getmloc();

    return QVariant();
}

QVariant ApptModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //if (role == Qt::SizeHintRole)

    if (role != Qt::DisplayRole) {
        return QVariant();
    }


    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return tr("Subject");
        case 1:
            return tr("When");
        case 2:
            return tr("Expires");
        case 3:
            return tr("Location");

        case 4:
            return tr("name");
        case 5:
            return tr("loc");
        default:
            return QVariant();
        }
    }else
        QString::number(section);
    return QVariant();

}
