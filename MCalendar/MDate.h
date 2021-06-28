#ifndef MDATE_H
#define MDATE_H

#include <QDate>
#include <QLabel>
#include <QFont>
#include <QPixmap>
#include <QPalette>

#define DAY_ONE_LETTER 0
#define DAY_THREE_LETTER 1
#define DAY_LONG_NAME 2

namespace globalNamespace
{
   extern int firstHolly;
   extern int secondHolly;
   extern int defaultDayFormat;
   extern QList<QFont> NormalDayFont;
   extern QList<QFont> firstPublidayFont;
   extern QList<QFont> secondPublicDayFont;
   extern QList<QFont> fearFont;
   extern QList<QFont> selectionFont;
   extern QList<QPalette> NormalDayPalette;
   extern QList<QPalette> firstPublicDayPalette;
   extern QList<QPalette> secondPublicDayPalette;
   extern QList<QPalette> fearPalette;
   extern QList<QPalette> selectionPalette;
   extern QString headerLabelFormat;
   extern QPalette headersPalette;
   extern QFont headersFont;
   extern bool gridVisible;
   extern bool showMoon;
   extern bool showMoonTime;
   extern bool showFear;
}

class MDate : public  QDate
{
    int index;
    int moontype=0;

    QTime moonTime;
    bool hasMoon;
    bool paques;
    bool lundiPaques;
    bool rameaux;
    bool ascension;
    bool pentecote;
    bool lundiPentecote;
    bool vendrediSaint;
    QString moonTimeDefaultFormat;

    bool isPaques();
    void initColumnStyleSheet();

public:
    explicit MDate();
    explicit MDate(const QDate &d);
    explicit MDate(int y,int m , int d);
    bool operator!=(const QDate &d) const;
    bool operator<(const QDate &d) const;
    bool operator<=(const QDate &d) const;
    bool operator==(const QDate &d) const;
    bool operator>(const QDate &d) const;
    bool operator>=(const QDate &d) const;

    bool operator!=(const MDate &d) const;
    bool operator<(const  MDate &d) const;
    bool operator<=(const MDate &d) const;
    bool operator==(const MDate &d) const;
    bool operator>(const  MDate &d) const;
    bool operator>=(const MDate &d) const;

    void setMoon(int type, const QTime &dt);
    void setMoonTimeDefaultFormat(const QString &fmt);
    QString getMoonTimeDefaultFormat();
    QString getNumDay();

    QPalette genPalette(int column);
    QFont    genFont(int column);
    QPixmap * genMoonPixmap();
    QString getFear();
    QString getDay();
    bool isFearDay();
    QString fearDay();
    bool hasmoon();
    QTime getMoonTime();
    QString getMoonTextTime();

    int getMoonType();
    void setIndex(int i);
    int getIndex();
    void setEaster(bool easter);
    void setMondayEaster(bool measter);
    void setRameaux(bool ram);
    void setAscension(bool asc);
    void setPentecote(bool pent);
    void setLundiPent(bool lpent);
    void setVendrediSaint(bool vendredi);

};

#endif
