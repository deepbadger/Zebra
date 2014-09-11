#ifndef MESSAGES_H
#define MESSAGES_H

#include <QWidget>
#include <QItemSelection>

namespace Ui {
class Messages;
}

class inf_mail;
class Messmodel;
class tableDelegate;

class Messages : public QWidget
{
    Q_OBJECT
    
public:
    explicit Messages(QWidget *parent = 0);
    ~Messages();

    int check(const QList<inf_mail> list);
    int cnt();


signals:
    void emul(QString);
    void filter(QString);
    void open();

private slots:
    void slotLoadTransaction(const QItemSelection & sel, const QItemSelection & desel);
    void acceptRead();
    void acceptOpen();
    void acceptFilter();
    void filterOK(QString s);
    
private:
    Ui::Messages *ui;
    Messmodel *Model;
    tableDelegate *delegat;
};

#endif // MESSAGES_H
