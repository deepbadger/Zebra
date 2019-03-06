#include "messages.h"

#include "messmodel.h"
#include "tabledelegate.h"
#include "inf.h"
#include "ui_messages.h"

#include <QDebug>

Messages::Messages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Messages)
{
    ui->setupUi(this);

    this->setWindowFlags( Qt::Window| Qt::WindowTitleHint| Qt::WindowStaysOnTopHint );

    connect (this, SIGNAL ( emul (QString)), this->parent(), SLOT ( sendRead(QString )));
    connect (this, SIGNAL ( filter (QString)), this->parent(), SLOT ( addtoFilter(QString )));



    connect (this, SIGNAL ( open ()), this->parent(), SLOT ( viewMail()));



    ui->label->clear();
    Model = new Messmodel(this);
    delegat = new tableDelegate(this);

    ui->tableView->setModel(Model);
    ui->tableView->hideColumn(0);


    ui->tableView->setItemDelegate(delegat);
    ui->tableView->setColumnWidth(0, 150);
    ui->tableView->setColumnWidth(1, 70);
    ui->tableView->setColumnWidth(2, 170);
    ui->tableView->setColumnWidth(3, 200);
    ui->tableView->setColumnWidth(4, 200);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->resizeRowsToContents();
}

Messages::~Messages()
{
    delete ui;
}

int Messages::check(const QList<inf_mail> list)
{

    int a = Model->check(list);
    //ui->tableView->model()->sort(0);
    ui->tableView->resizeRowsToContents();
    return a;
}

int Messages::cnt()
{
    int hiddenrow = 0;
    for( int i=0; i<Model->rowCount(); i++)
    {
        if( ui->tableView->isRowHidden(i))
            hiddenrow++;
    }

    int a = Model->rowCount() - hiddenrow;
    ui->tableView->resizeRowsToContents();
    return a;
}

void Messages::acceptRead()
{
    inf_mail infor;

    int hiddenrow = 0;

    QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();

    if (!selectedList.count()){
        ui->tableView->selectAll();
        selectedList = ui->tableView->selectionModel()->selectedRows();

    }
    ui->tableView->setUpdatesEnabled(false);

    QString idlst,id;
    for( int i=0; i<selectedList.count(); i++)
    {
        QModelIndex mi = selectedList.at(i);
        ui->tableView->hideRow(mi.row());
        id = Model->getId(mi.row());
        if (!id.isEmpty())
            if (idlst.isEmpty())
                idlst = id;
            else
                idlst = idlst+","+id;
    }
    emit emul(idlst);
    ui->tableView->setUpdatesEnabled(true);
    ui->tableView->clearSelection();

    for( int i=0; i<Model->rowCount(); i++)
    {
        if( ui->tableView->isRowHidden(i))
            hiddenrow++;
    }


    if(!(Model->rowCount() - hiddenrow))
        this->close();
}

void Messages::acceptOpen()
{
    emit open();
}

void Messages::acceptFilter()
{

    inf_mail infor;

    QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();

    if (!selectedList.count()){
        ui->tableView->selectAll();
        selectedList = ui->tableView->selectionModel()->selectedRows();

    }

    for( int i=0; i<selectedList.count(); i++)
    {
        QModelIndex mi = selectedList.at(i);
        emit filter(Model->getFrom(mi.row()));
    }
    //ui->tableView->clearSelection();

}

void Messages::filterOK(QString s)
{
    if(s.size()>100)
        s = "..." + s.mid(s.size() - 96);
    ui->label->setText(s);
}


void Messages::slotLoadTransaction(const QItemSelection & sel, const QItemSelection & desel)
{


    //    QModelIndexList selectedList = ui->tableView->selectionModel()->selectedRows();
    //    int a =  selectedList.count();
    //    if (a == 1 ){
    //        ui->pushButton->setText(tr("Mark as read"));
    //        //ui->pushButton->setDisabled(true);
    //    }else{
    //        ui->pushButton->setText(tr("Mark as read"));

    //        //ui->pushButton->setDisabled(false);
    //    }


}
