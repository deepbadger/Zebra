#include "tabledelegate.h"
#include <QPainter>
#include <QTextBrowser>
#include <QAbstractTextDocumentLayout>
#include <QDebug>


tableDelegate::tableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void tableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    //QModelIndex indexx = index.model()->index(index.row(), 2, QModelIndex());
    //QString str = index.model()->data(indexx, Qt::DisplayRole).toString();

    if (index.column() < 6)
    {
        bool isSelected = option.state & QStyle::State_Selected;
        QStyleOptionViewItemV4 options = option;
        initStyleOption(&options, index);

        painter->save();
        QTextBrowser browser;
        QColor ItemForegroundColor = index.data(Qt::ForegroundRole).value<QColor>();

//        if (isSelected )
//        {

//            if (ItemForegroundColor != option.palette.color(QPalette::WindowText))
//            {
//                qDebug()<< "valod"<< ItemForegroundColor;
//                options.palette.setColor(QPalette::HighlightedText, ItemForegroundColor);



//                QPalette palette;
//                palette = browser.palette();
//                palette.setColor(QPalette::Base, QColor(255, 255, 192));
//                QFont f;
//                f.setBold(true);
//                browser.document()->setDefaultFont(f);
//            }
//        }




        //options.palette.setColor(QPalette::HighlightedText, Qt::red);

        //browser.setLineWrapMode(QTextBrowser::NoWrap);
        QString str = index.data().value<QString>();

        if (isSelected )
        {
            str = "<div style=\"background:none repeat scroll 0 0 #F3EDDF;\"><span style=\"background:none repeat scroll 0 0 #F3EDDF;\">"+str+"</span></div>";
        }

        browser.setHtml(str);
        QAbstractTextDocumentLayout::PaintContext context;
        QRect rect = option.rect;
        browser.document()->setPageSize( rect.size());

        painter->translate(rect.x(),rect.y());

        browser.document()->documentLayout()->draw(painter, context);
        options.text = "";

        painter->restore();


        //                QStyleOptionViewItemV4 options = option;
        //                initStyleOption(&options, index);

        //                painter->save();

        //                QTextDocument doc;

        //                doc.setHtml(options.text);

        //               // options.text = "";
        //                options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

        //                painter->translate(options.rect.left(), options.rect.top());
        //                QRect clip(0, 0, options.rect.width(), options.rect.height());
        //                doc.drawContents(painter, clip);

        //                painter->restore();

    }


    //    if (index.column() == 2)
    //    {

    //        QString str = index.model()->data(index, Qt::DisplayRole).toString();
    //        QFont f;
    //        QStyleOptionViewItem opt(option);
    //        f.setBold(true);


    //        //painter->fillRect(option.rect, QColor(168, 253, 168));//green
    //        //painter->setFont(f);

    //        opt.font = f;
    //        drawDisplay(painter, opt, opt.rect, str);
    //        drawFocus(painter, opt, opt.rect);

    //    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

QSize tableDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //    QTextBrowser browser;
    //    //browser.setLineWrapMode(QTextBrowser::NoWrap);
    //    browser.setHtml(index.data().value<QString>());
    //    QRect rect = option.rect;
    //    browser.document()->setPageSize( rect.size());
    //    return QSize(browser.width(),browser.size().height());


    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    QTextDocument doc;
    doc.setHtml(options.text);
    doc.setTextWidth(options.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}
