#include "viewerconsole.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <QDebug>
#include "datefilter.h"


viewerConsole::viewerConsole()
{
    record = new recorder("/home/moi/etc/Rdv.dat");
    QMap<QDateTime,QPair<QString,int>> *rdvs = record->getData();
    cprinter = new conscolor;
    out = cprinter->getOutput();
    dateFilter *dt_f = new dateFilter(rdvs,true);
    disp = dt_f->getSorted();

}

int viewerConsole::offsetToIndex(qint64 offsetDays)
{
    int index = offsetDays + seek_display;
    index = (int) ((index<0) ? 0 : (index));
    if ((index ) > (MAX_ANNONCES - seek_display))
        index = (MAX_ANNONCES - seek_display) + 1;
    return index;
}

void viewerConsole::display()
{
    struct winsize w;
    conscolor *cprinter = new conscolor;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    cprinter->cls();
    cprinter->gotoxy(1,1);
    int n=0;

    QMap<QDateTime,dt_per_msg>::const_iterator k;

    for (QMultiMap<QDateTime,dt_per_msg >::const_iterator i = disp->constBegin();i != disp->constEnd();++i)
    {
        const QDateTime &qdt = i.key();
        int offset = i.value().offsetDays;
        int index = offsetToIndex(offset);

        QString content = i.value().message;
        if (n==0 || (k.key().daysTo(i.key())) > 0)
        {
            QString str(cprinter->get_annonce_from_index(index));
            str += qdt.toString("dddd dd MMMM yyyy");
            cprinter->printcenter(str,cprinter->get_color_from_index(index),w.ws_col - 2);
            cprinter->newline();
        }

        cprinter->print(qdt.toString("HH:mm"),conscolor::LCYAN);
        int periode = i.value().period;
        const QDateTime &qt = i.value().dt;
        if (periode != -1)
        {
            if (periode == 1 )
                cprinter->print(" " + record->getsCper(periode) + " (" + qt.toString("dddd dd/MM/yy") + ") ",conscolor::LGREEN);
            else
                cprinter->print(" " + record->getsCper(periode) + " (" + qt.toString("dd/MM/yy") + ") ",conscolor::LGREEN);
        }

        cprinter->print(" ");
        cprinter->print(content);
        k = i;
        n++;
    }
}

