#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFont>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDir>
#include <QSettings>
#include <QComboBox>
#include "MGroupCalendar.h"

class rdvTable;
class Window : public QWidget
{
    Q_OBJECT
    QHBoxLayout *layout;
   bool settings_enable = false;
   bool rdvs_enable = false;
   rdvTable *rdvt;
   MGroupCalendar *grpcal;
   QGroupBox *generalOptionsGroupBox;
   QGroupBox *formatsGroupBox;
   QGroupBox *stylesGroupBox;
   QButtonGroup *btnGrpFirstHoly;
   QButtonGroup *btnGrpSecondHoly;
   QButtonGroup *btnGrpNormalHoly;
   QButtonGroup *btnGrpSelection;
   QButtonGroup *btnGrpFear;
   QCheckBox * ckbSetGrid ;
   QCheckBox * ckbSetSt;
   QCheckBox *ckbToday;
   QLineEdit    *qlyear;
   QLineEdit    *qlmonth;
   QLineEdit *leHowMany;
   QLineEdit *leMthBefor;
   QSettings *mSettings;
   QCheckBox *ckbAffMoonHour;
   QCheckBox *ckbAffMoons;
   QComboBox *qcbJrFer,*qcbJrFer2;
   QComboBox *qcbMthHeaderFmt;
   QComboBox *qcbFmtDays;
   QVBoxLayout *rightlayout;
   QLabel *rdvHeaderLabel;
   QString datafile;

   void createGeneralOptionGroupBox();
   void genericColumnsColorChange(QButtonGroup *qbtn, QString id, QPalette::ColorRole palType);
   void genericColumnsPoliceChange(QButtonGroup *qbtn,QString id);
   void saveFont(QString funcName);
   void savePalette(QString funcName);
   void readFontSettings(QString funcName);
   void readPaletteSettings(QString funcName);
   void removeLayout(QLayout *layout);
   void createRdvPanel(QDate d);


   Q_INVOKABLE void setFirstHolyFont(QFont &fnt,int col);
   Q_INVOKABLE void setSecondHolyFont(QFont &fnt, int col);
   Q_INVOKABLE void setNormalDayFont(QFont &fnt, int col);
   Q_INVOKABLE void setSelectionFont(QFont &fnt, int col);
   Q_INVOKABLE void setFearDayFont(QFont &fnt, int col);


   Q_INVOKABLE QPalette * getFirstHolyPalette(int col);
   Q_INVOKABLE QPalette * getSecondHolyPalette(int col);
   Q_INVOKABLE QPalette * getNormalDayPalette(int col);
   Q_INVOKABLE QPalette * getSelectionPalette(int col);
   Q_INVOKABLE QPalette * getFearDayPalette(int col);

   Q_INVOKABLE void setFirstHolyPalette(QPalette &pal, int col);
   Q_INVOKABLE void setSecondHolyPalette(QPalette &pal, int col);
   Q_INVOKABLE void setNormalDayPalette(QPalette &pal, int col);
   Q_INVOKABLE void setSelectionPalette(QPalette &pal, int col);
   Q_INVOKABLE void setFearDayPalette(QPalette &pal,int col);


   void createFormatsGroupBox();
   void createStylesGroupBox();
   int readSettings();
   int checkPath(const QString &file,QDir *qdirObj);
   void syncSettings();
   int getCheckedCol(QButtonGroup *qbtn);

   int igetKey(const QString &key);
   bool bgetKey(const QString &key);
   QString sgetKey(const QString &key);
   bool startToday = false;
   int year=-1;
   int month=-1;
   int nbMonth=4;
   int monthBefore=0;
   bool noIniFile=false;


   QTextStream& qStdOut()
   {
       static QTextStream ts( stdout );
       return ts;
   }

public:
    Window();
    void showRdvWin(int row, int col, MCalendar *cal);

private slots:

    void ckbStartDateChanged(int st);
    void leYearChanged();
    void leMonthChanged();
    void leHowManyChanged();
    void leMonthBeforeChanged();

    void ckbMoonTime(int st);
    void ckbMoon(int st);
    void ckbShowGrid(int n);
    void ckbShowSaints(int n);
    void firstPblcDayChged(int n);
    void secondPblcDayChged(int n);
    void monthHeaderFmtChged(QString fmt);
    void dayNameTypeChanged(int n);

    void btnColorHeaderClicked();
    void btnBgColorHeaderClicked();
    void btnPoliceHeaderClicked();

    void btnColorFirstDayClicked();
    void btnBgColorFirstDayClicked();
    void btnPoliceFirstDayClicked();
    void btnColorSecondDayClicked();
    void btnBgColorSecondDayClicked();
    void btnPoliceSecondDayClicked();
    void btnColorNormalDayClicked();
    void btnBgColorNormalDayClicked();
    void btnPoliceNormalDayClicked();
    void btnColorFearDayClicked();
    void btnBgColorFearDayClicked();
    void btnPoliceFearDayClicked();
    void btnColorSelectedDayClicked();
    void btnBgColorSelectedDayClicked();
    void btnPoliceSelectedDayClicked();
    void writeSettings();
    void togleSettings();


};

#endif
