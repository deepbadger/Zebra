#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QStyledItemDelegate>

class QPainter;

class tableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit tableDelegate(QObject *parent = 0);

    void
    paint(QPainter *painter, const QStyleOptionViewItem &option,
          const QModelIndex &index) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    
signals:
    
public slots:
    
};

#endif // TABLEDELEGATE_H
