#ifndef INF_H
#define INF_H

#include <QMetaType>

class inf
{
    QString mId;
    QString mname;
    QString mloc;
    QString mdur;
    QString malarmInstStart;
    QString mnextAlarm;
    QString mdueDate;


public:
    inf();
    inf(const inf &A);

    QString getmId();
    QString getmname();
    QString getmloc();
    QString getmdur();
    QString getmalarmInstStart();
    QString getmnextAlarm();
    QString getmdueDate();
    QString getstr();
    QString getstrMsg();

    QString setmId(const QString &A);
    QString setmname(const QString &A);
    QString setmloc(const QString &A);
    QString setmdur(const QString &A);
    QString setmdueDate(const QString &A);
    QString setmalarmInstStart(const QString &A);
    QString setmalarmInstStartsave(const QString &A);
    QString setmnextAlarm(const QString &A);

    bool operator ==(inf );

    void reset();
};


class inf_mail
{
    QString Id;
    QString nm;
    QString fr;
    QString dt;
    QString sub;
    QString frm;
    QString d;

public:
    inf_mail();
    inf_mail(const inf_mail &A);


    QString gId();
    QString gname();
    QString gfrom();
    QString gdate();
    QString gsubject();
    QString gfragment();
    QString gdir();

    void mId(const QString &A);
    void name(const QString &A);
    void from(const QString &A);
    void date(const QString &A);
    void subject(const QString &A);
    void fragment(const QString &A);
    void dir(const QString &A);

    bool operator ==(inf_mail );

    void reset();
};

//Q_DECLARE_METATYPE(inf)
#endif // INF_H
