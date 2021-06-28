#ifndef DATEFILTER_H
#define DATEFILTER_H
#include <QMap>
#include <QDateTime>
#include <QMultiMap>

struct dt_per_msg
{
    QDateTime dt;
    QString message;
    int period;
    qint64 offsetDays;
};

class dateFilter
{
    QMultiMap<QDateTime,dt_per_msg> disp;
    QMap<QDateTime, QPair<QString,int> > *rendezvous;
    int days_after = 90;
    int days_before = 0;
    int compare_with_mask(QDateTime qdt, int period );

public:
    explicit dateFilter();
    explicit dateFilter(QMap<QDateTime, QPair<QString,int> > *rdvs, bool dofilter=false);
    explicit dateFilter(QMap<QDateTime, QPair<QString,int> > *rdvs,int days_after,int days_before);
    void setBounds(int days_after,int days_before);
    void setRdvs(QMap<QDateTime, QPair<QString,int> > *rdvs);
    int countForDisplay();

    void filter();
    QMultiMap<QDateTime,dt_per_msg> * getSorted();
};

#endif // DATEFILTER_H
