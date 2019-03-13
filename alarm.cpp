#include "apptmodel.h"
#include "tabledelegate.h"
#include "alarm.h"
#include "inf.h"
#include "ui_alarm.h"
#include <QMessageBox>
#include <QDebug>
#include <QDesktopWidget>



Alarm::Alarm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Alarm)
{
  ui->setupUi(this);
  //this->setWindowFlags(Qt::Window | Qt::WindowTitleHint |    Qt::WindowStaysOnTopHint);
  //this->setWindowFlags( Qt::Window |  Qt::FramelessWindowHint );

  //this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint| Qt::WindowStaysOnTopHint );
  this->setWindowFlags( Qt::Window| Qt::WindowTitleHint| Qt::WindowStaysOnTopHint );

  connect (this, SIGNAL ( emul (QString)), this->parent(), SLOT ( sendDismiss(QString )));
  connect (this, SIGNAL ( open ()), this->parent(), SLOT ( viewMail()));


  apptModel = new ApptModel(this);
  delegat = new tableDelegate(this);

  ui->tableView->setModel(apptModel);

  ui->tableView->setItemDelegate(delegat);
  ui->tableView->setColumnWidth(0, 200);
  //    ui->tableView->setColumnWidth(1, 90);
  //    ui->tableView->setColumnWidth(2, 150);
  //    ui->tableView->setColumnWidth(3, 90);
  ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->tableView->resizeRowsToContents();

  connect(
    ui->tableView->selectionModel(),
    SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
    SLOT(slotLoadTransaction(const QItemSelection &, const QItemSelection &))
  );
  _cnt = 0;

}

Alarm::~Alarm()
{
  delete ui;
}

int Alarm::appendAppt(const QString Id, QString nextAlarm, QString alarmInstStart, QString invId, QString name, QString loc)
{
  return apptModel->appendAppt(Id,nextAlarm,alarmInstStart,invId,name,loc);
}

mresult Alarm::checkAppt(const QList<QString> mId)
{
  mresult a = apptModel->checkAppt(mId);
  ui->tableView->resizeRowsToContents();

  return a;
}


mresult Alarm::checkAppt(const QList<inf_appt> listAppt)
{
  mresult a = apptModel->checkAppt(listAppt);
  _cnt = 0;
  ui->tableView->resizeRowsToContents();
  return a;
}

int Alarm::checkCount()
{
  ui->tableView->resizeRowsToContents();
  return apptModel->rowCount() - _cnt;
}

QString Alarm::getMsg()
{

  return apptModel->getMsg();
}

//void Alarm::setGeo(int l, int r, int t, int b)
//{
//    QRect geo;
//    geo.setLeft(l);
//    geo.setRight(r);
//    geo.setTop(t);
//    geo.setBottom(b);
//    //    QRect geo =  this->parentWidget()->geometry();
//    //    qDebug () << geo.left() << geo.right() << geo.top() << geo.bottom();
//    this->setGeometry( geo );

//    //    QDesktopWidget desktop;
//    //        QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами єкрана
//    //        qDebug () << rect.left() << rect.right() << rect.top() << rect.bottom();
//    //        QPoint center = rect.center(); //координаты центра экрана
//}

void Alarm::acceptDismiss()
{
  inf_appt infor;

  QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();

  if (!selectedList.count()) {
    ui->tableView->selectAll();
    selectedList = ui->tableView->selectionModel()->selectedRows();

  }

  for( int i=0; i<selectedList.count(); i++) {
    QModelIndex mi = selectedList.at(i);
    ui->tableView->hideRow(mi.row());
    apptModel->getAppt(mi.row(), infor);
    apptModel->dismissAppt(infor);
    //qDebug() << infor.getstr();
    emit emul(infor.getmId());
    _cnt++;

  }
  ui->tableView->clearSelection();
  if(!(apptModel->rowCount() - _cnt))
    this->close();
}

void Alarm::acceptOpen()
{
  emit open();
}

void Alarm::upd()
{
  apptModel->upd();
//    QModelIndex topLeft = index(0, 0);
//    QModelIndex bottomRight = index(apptModel->rowCount() - 1, apptModel->columnCount() - 1);
//    apptModel->dataChanged(topLeft, bottomRight);
  //apptModel->layoutChanged();
}


void Alarm::slotLoadTransaction(const QItemSelection & sel, const QItemSelection & desel)
{
  QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();
  int a =  selectedList.count();
  if (a == 1 ) {
    ui->pushButton->setText(tr("Dismiss"));
    //ui->pushButton->setDisabled(true);
  } else {
    ui->pushButton->setText(tr("Dismiss all"));
    //ui->pushButton->setDisabled(false);
  }
}
