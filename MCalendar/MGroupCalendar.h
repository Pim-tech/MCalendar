#ifndef MGROUPCALENDAR_H
#define MGROUPCALENDAR_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QPalette>
#include <QColor>
#include "MCalendar.h"
#include "MDate.h"
#include "MoonPhases.h"

class Window;

class MGroupCalendar : public QWidget
{
  Q_OBJECT

    QMap<QDate,MDate *> mMdate;
    MoonPhases * mph;
    int currentYear;
    int currentMonth;
    int defaultMonthBefore = 0;
    int defaultMonthsAfter = 4;
    int index              = 0;
    int lastCalSelected    = -1;
    int lastLineSelected   = -1;
    int nbMonth;
    int remaining          = 0;
    QObject * mParent = 0;

    QList<QString> NormalDayStyleSheet;
    QList<QString> SaturdayStyleSheet;
    QList<QString> SundayStyleSheet;
    QList<QString> FearStyleSheet;
    QList<QString> selectedTimesheet;

    QHBoxLayout * calendarLayout;
    QHBoxLayout * commandsLayout;
    QVBoxLayout * mainlayout;
    QList<MCalendar *> cals;
    QList<QLabel *> labels;
    QDate firstPanelDate;
    QDate lastPanelDate;
    QDate currentSeletedDate;
    QPushButton *btnSettings;
    QPushButton *btnQuit;

    QLabel * makeHeaderLabel(int year, int month);
    QLabel * makeHeaderLabel(const QDate &qd);
    QVBoxLayout * makeVBox(int year, int month);
    QVBoxLayout * makeVBox(const QDate &d);

    void pullEasterDate();
    QDate getEasterDate(int year);
    void setMoonPhases();
    void doCalendars(const QDate &date, int mthAfter, int mthBefore);
    void doCalendars(int year,int month, int mthAfter, int mthBefore);
    void createCommandsLayout();
    void create();
    void storeOnePhase(int moontype);
    void addPanel();
    void removePanel();

    QPushButton * makeButton(const QString &name, const QString &icon);

    void monthSeek(int n);
    void init();

private slots:
    void pageMinus();
    void pagePlus();
    void yearMinus();
    void yearPlus();
    void monthMinus();
    void monthPlus();
    void dayMinus();
    void dayPlus();
    void toDay();


    void cellIsClicked(int row,int col,MCalendar *cal);
    void cellIsDoubleClicked(int row,int col,MCalendar *cal);
    void showLayoutWidgets(QLayout *layout,bool show);
    void removeLayoutWidgets(QLayout *layout, bool doIt);
    QLayout * searchLayout(QLayout *layout, int n);


public:
    explicit MGroupCalendar();
    explicit MGroupCalendar(int year, int curMonth, int mthAfter, int mthBefore=0);
    explicit MGroupCalendar(const QDate &date, int mthAfter, int mthBefore=0);

    void selectDate(const QDate &dt);

    void redrawAfterDataChanged();
    void setHeaderLabelFormat(const QString &fmt);

    void setMonthBefore(int n);
    void setNbMonth(int n);
    void jumpTo(const QDate &dt);
    void jumpTo(int year,int month);
    void setParent(QObject *mParent);
    QObject * getParent();
    int getCurrentYear();
    int getCurrentMonth();
    int getNbMonth();
    int getMonthBefore();
    void refreshHeaderLabel();

    void setHeadersBgColor(const QColor &color);
    void setHeadersFgColor(const QColor &color);
    void setHeadersFont(QFont &font);

    void setFirstHolyDay(int n);
    void setSecondHolyDay(int n);
    void setDayformat(int n);
    QPushButton * getBtnSettings();
    QPushButton * getBtnQuit();

    Q_INVOKABLE void setFirstHolyPalette(QPalette &pal, int col, bool redraw = true);
    Q_INVOKABLE void setSecondHolyPalette(QPalette &pal, int col, bool redraw = true);
    Q_INVOKABLE void setNormalDayPalette(QPalette &pal, int col, bool redraw = true);
    Q_INVOKABLE void setSelectionPalette(QPalette &pal, int col, bool redraw = true);
    Q_INVOKABLE void setFearDayPalette(QPalette &pal,int col, bool redraw = true);

    Q_INVOKABLE QPalette * getFirstHolyPalette(int col);
    Q_INVOKABLE QPalette * getSecondHolyPalette(int col);
    Q_INVOKABLE QPalette * getNormalDayPalette(int col);
    Q_INVOKABLE QPalette * getSelectionPalette(int col);
    Q_INVOKABLE QPalette * getFearDayPalette(int col);

    int getFirstHolyDay();
    int getSecondHolyDay();
    bool getHasMoonHours();
    bool getHasMoon();

    Q_INVOKABLE void setFirstHolyFont(QFont &fnt,int col,bool redraw=true);
    Q_INVOKABLE void setSecondHolyFont(QFont &fnt, int col,bool redraw=true);
    Q_INVOKABLE void setNormalDayFont(QFont &fnt, int col, bool redraw=true);
    Q_INVOKABLE void setSelectionFont(QFont &fnt, int col,bool redraw=true);
    Q_INVOKABLE void setFearDayFont(QFont &fnt, int col,bool redraw=true);

    Q_INVOKABLE QFont * getNormalDayFont(int col);
    Q_INVOKABLE QFont * getFirstHolyFont(int col);
    Q_INVOKABLE QFont * getSecondHolyFont(int col);
    Q_INVOKABLE QFont * getSelectionFont(int col);
    Q_INVOKABLE QFont * getFearDayFont(int col);

    QString getHeaderLabelFormat();
    QPalette * getHeaderPalette();
    QFont * getHeaderFont();
    int getDayFormat();

protected:
     void keyPressEvent(QKeyEvent *);
     void keyReleaseEvent(QKeyEvent *event);
     bool focusNextPrevChild(bool next) { next = false; return next;}
signals:
     void cocorico2(int row,int col,MCalendar *cal);
};

#endif // MGROUPCALENDAR_H
