#ifndef MESSMODEL_H
#define MESSMODEL_H


#include <QAbstractTableModel>
#include "inf.h"

class Messmodel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Messmodel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int check(const QList<inf_mail> &list);
    QString getId(int row);
    QString getFrom(int row);
    
signals:
    
public slots:


private:
    QList<inf_mail> Infmail;
    QList<inf_mail> Black;
    
};

#endif // MESSMODEL_H
