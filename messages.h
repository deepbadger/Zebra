#ifndef MESSAGES_H
#define MESSAGES_H

#include <QWidget>
#include <QItemSelection>
#include "inf.h"

namespace Ui
{
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

  mresult check(const QList<inf_mail> list);
  QString getMsg();
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
