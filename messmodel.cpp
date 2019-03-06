#include "messmodel.h"

#include <QDateTime>
#include <QDebug>

Messmodel::Messmodel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


int Messmodel::rowCount(const QModelIndex &parent) const
{
    return Infmail.size();
}

int Messmodel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant Messmodel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    inf_mail row = Infmail.at(index.row());

    if (index.column() == 0){

        return row.getdate().toLongLong();
    }else if (index.column() == 1){
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 messageDate = row.getdate().toLongLong();
        QString str;
        if( (now - messageDate) >= 86400000)
            str = QString("<b>%1</b>").arg( QDateTime::fromMSecsSinceEpoch(messageDate).toString("dd.MM.yy hh:mm"));
        else
            str = QString("<b>%1</b>").arg( QDateTime::fromMSecsSinceEpoch(messageDate).toString("hh:mm"));

        return QString("<div align=\"right\" style=\"text-align: center;\">%5</div>"
                       "<div style=\"font-weight: bold;\">%1 <span style=\"color: #666666;\">%2</span></div>"
                       "<span style=\" font-family:'Arial';\">%3<br/>%4</span>")
                        .arg(row.getname())
                        .arg(row.getfrom())
                        .arg(row.getsubject())
                        .arg(row.getfragment())
                .arg(str);


        //return QString("%1").arg(row.gdate());
    }else if (index.column() == 2){
        return QString("<b>%1 <span style=\"color: #666666;\">%2</b></span><br/><span style=\" font-family:'Arial';\">%3<br/>%4</span>")
                .arg(row.getname())
                .arg(row.getfrom())
                .arg(row.getsubject())
                .arg(row.getfragment());
    }else if (index.column() == 3){
        return QString("%1<br/><span style=\" font-family:'Arial';\">%2</span>").arg(row.getsubject()).arg(row.getfragment());
    }else if (index.column() == 4)
        return QString("<span style=\" font-family:'Arial';\">%1</span>").arg(row.getfragment());

    return QVariant();
}

QVariant Messmodel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return tr("№");
        case 1:
            return tr("Date");
        case 2:
            return tr("From/Subject/Fragment");
        case 3:
            return tr("Subject - fragment");
        case 4:
            return tr("Fragment");

        default:
            return QVariant();
        }
    }else
        QString::number(section);
    return QVariant();

}


int Messmodel::check(const QList<inf_mail> &list)
{

    //qint64 now = QDateTime::currentMSecsSinceEpoch();
    QList <inf_mail> toRemove;
    QList <inf_mail> toAppend;
    inf_mail A,B;


    for (int i = 0; i < list.size(); ++i) {
        A = list.at(i);
        if(!Infmail.contains(A)){

            toAppend.append(A);
            qDebug() << "add"<< A.getsubject();
        }
    }

    for (int i = 0; i < Infmail.size(); ++i) {
        A = Infmail.at(i);
        if (!list.contains(A)){
            toRemove.append(A);
            qDebug() << "rm"<< A.getsubject();
        }
    }

    for (int i = 0; i < toRemove.size(); ++i) {
        A = toRemove.at(i);
        if(Infmail.contains(A)){
            int ind = Infmail.indexOf(A);
            beginRemoveRows(QModelIndex(), ind, ind);
            Infmail.removeAll(A);
            endRemoveRows();
        }
    }
    int k = 0;
    for (int i = 0; i < toAppend.size(); ++i) {
        A = toAppend.at(i);
        k = 0;
        if(!Infmail.contains(A)){
            for ( int j = 0; j < Infmail.size(); ++j) {
                B = Infmail.at(j);
                qDebug() << B.getsubject();
                if(A.getdate().toLongLong() < B.getdate().toLongLong()){
                    k = j + 1;
                    qDebug() << "+++" << Infmail.size() << k;
                }
            }
            beginInsertRows(QModelIndex(), k, k );
            Infmail.insert(k,A);
            endInsertRows();
        }
    }
    return Infmail.size();
}

QString Messmodel::getId(int row)
{
    if (row < Infmail.size()){
        inf_mail id = Infmail.at(row);
        return id.getmid();
    }
    return "";

}

QString Messmodel::getFrom(int row)
{
    if (row < Infmail.size()){
        inf_mail id = Infmail.at(row);
        return id.getfrom();
    }
    return "";
}

