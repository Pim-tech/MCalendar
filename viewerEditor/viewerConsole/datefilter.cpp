#include "datefilter.h"
#include <QDebug>

dateFilter::dateFilter()
{
}

dateFilter::dateFilter(QMap<QDateTime, QPair<QString,int> > *rdvs,bool dofilter)
{
    this->rendezvous = rdvs;
    if (dofilter == true)
        filter();
}
dateFilter::dateFilter(QMap<QDateTime, QPair<QString,int> > *rdvs,int days_after,int days_before)
{
    this->rendezvous = rdvs;
    this->days_after = days_after;
    this->days_before = days_before;
}

void dateFilter::setBounds(int days_after,int days_before)
{
    this->days_after = days_after;
    this->days_before = days_before;
}

void dateFilter::setRdvs(QMap<QDateTime, QPair<QString,int> > *rdvs)
{
    this->rendezvous = rdvs;
}

int dateFilter::compare_with_mask(QDateTime qdt, int period )
{
    QDateTime now(QDateTime::currentDateTime());
    QDateTime date2mask;
    const int month = qdt.date().month();
    const int day   = qdt.date().day();

    int m,y,d;
    int weekday_qdt,weekday_now;
    qint64 days2add;
    switch (period)
    {
    case -1:
        date2mask = qdt;
        break;
    case  0:
        date2mask = now;
        date2mask.setTime(qdt.time());
        break;
    case 1:
        weekday_qdt = qdt.date().dayOfWeek();
        weekday_now = now.date().dayOfWeek();
        days2add = weekday_qdt - weekday_now;
        if (days2add < 0 )
            days2add = 7 + days2add;
        date2mask = now;
        date2mask.setTime(qdt.time());
        date2mask = date2mask.addDays(days2add);
        break;
    case 2:
        m = now.date().month();
        y = now.date().year();
        d = now.date().day();
        date2mask = QDateTime(QDate(y,m,day),qdt.time());
        if (day < d )
            date2mask = date2mask.addMonths(1);
        break;
    case 3:
        y = now.date().year();
        date2mask = QDateTime(QDate(y,month,day),qdt.time());
        if (date2mask.date() < now.date())
            date2mask = date2mask.addYears(1);
        break;
    default: break;
    }

    qint64 offsetDays  = now.daysTo(date2mask);

    if (offsetDays < -(days_before))
        return -1;
    if (offsetDays > days_after)
        return 1;

    dt_per_msg dpm;
    dpm.dt = qdt;
    dpm.offsetDays = offsetDays;
    dpm.period =period;
    dpm.message = (*rendezvous)[qdt].first;

    disp.insert(date2mask,dpm);

    return 0;
}

int dateFilter::countForDisplay()
{
    int n=0;
    int count =0;
    QMultiMap<QDateTime,dt_per_msg>::const_iterator k;
    for(QMultiMap<QDateTime,dt_per_msg>::const_iterator i = disp.constBegin();i!=disp.constEnd();++i)
    {
            if (n==0 ||(k.key().daysTo(i.key())) > 0 )
                count++;

            count++;
             k = i;
             n++;
    }
    return count;
}

void dateFilter::filter()
{

    QMap<QDateTime,QPair<QString,int> >::const_iterator k;
    for (QMap<QDateTime,QPair<QString,int> >::const_iterator i = rendezvous->constBegin();i != rendezvous->constEnd();++i)
    {
        const QDateTime &qdt = i.key();
        compare_with_mask(qdt,i.value().second);
    }
}
QMultiMap<QDateTime,dt_per_msg> * dateFilter::getSorted()
{
    return &disp;
}
