
#include <QDate>
#include "window.h"
#include "MCalendar.h"

#include "newRdvWin.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QFontDialog>
#include <QColorDialog>
#include <QButtonGroup>
#include <QMessageBox>
#include <QFile>
#include <QSignalBlocker>
#include <QMetaObject>

Window::Window()
{
    layout = new QHBoxLayout;

    for (int i=0;i<5;i++)
    {
        QPalette qpl1,qpl2,qpl3,qpl4,qpl5;

        qpl1.setColor(QPalette::Text,QColor("#000000"));
        qpl1.setColor(QPalette::Base,QColor("#6666FF"));
        globalNamespace::firstPublicDayPalette << qpl1;
        qpl2.setColor(QPalette::Text,QColor("#000000"));
        qpl2.setColor(QPalette::Base,QColor("#9999FF"));
        globalNamespace::secondPublicDayPalette << qpl2;
        qpl3.setColor(QPalette::Text,QColor("#FFFFFF"));
        qpl3.setColor(QPalette::Base,QColor("#AA0000"));
        globalNamespace::fearPalette << qpl3;
        qpl4.setColor(QPalette::Text,QColor("#FFFF00"));
        qpl4.setColor(QPalette::Base,QColor("#FF0000"));
        globalNamespace::selectionPalette << qpl4;
        qpl5.setColor(QPalette::Text,QColor("#000000"));
        qpl5.setColor(QPalette::Base,QColor("#FFFFFF"));
        globalNamespace::NormalDayPalette << qpl5;

        globalNamespace::NormalDayFont << QFont("Arial",9,QFont::Bold);
        globalNamespace::fearFont     << QFont("Arial",9,QFont::Bold);
        globalNamespace::firstPublidayFont << QFont("Arial",9,QFont::Bold);
        globalNamespace::secondPublicDayFont << QFont("Arial",9,QFont::Bold);
        globalNamespace::selectionFont << QFont("Arial",9,QFont::Bold);
    }
    readSettings();
    QDate qdt;
    if (startToday == true || month == -1 || year == -1)
    {
        qdt = QDate::currentDate();
        year =  qdt.year();
        month = qdt.month();
    }

    //Creation du calendrier
    grpcal = new MGroupCalendar(year,month,nbMonth,monthBefore);
    grpcal->setParent(this);
    connect(grpcal->getBtnSettings(),SIGNAL(clicked()),this,SLOT(togleSettings()));
    connect(grpcal->getBtnQuit(),SIGNAL(clicked()),this,SLOT(close()));

    //Ajout du calendrier
    layout->addWidget(grpcal);
    layout->setAlignment(Qt::AlignRight);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

}
void Window::togleSettings()
{
    if (settings_enable == false)
    {
        if (rdvs_enable == true)
        {
            removeLayout(rightlayout);
            rdvs_enable = false;
        }
        createGeneralOptionGroupBox();
        createFormatsGroupBox();
        createStylesGroupBox();
        syncSettings();
        rightlayout  = new QVBoxLayout;
        layout->setAlignment(rightlayout,Qt::AlignTop);
        rightlayout->addWidget(generalOptionsGroupBox);
        rightlayout->setAlignment(Qt::AlignRight);
        rightlayout->addWidget(formatsGroupBox);
        rightlayout->addWidget(stylesGroupBox);

        QHBoxLayout *qhbxl = new QHBoxLayout;
        QPushButton *btnSaveSettings = new QPushButton(tr("Save Settings"));
        qhbxl->setAlignment(Qt::AlignRight);
        rightlayout->addLayout(qhbxl);
        qhbxl->setAlignment(btnSaveSettings,Qt::AlignRight);
        qhbxl->addWidget(btnSaveSettings);
        connect(btnSaveSettings,SIGNAL(clicked()),this,SLOT(writeSettings()));
        layout->addLayout(rightlayout);
        settings_enable = true;
    }
    else
    {
        removeLayout(rightlayout);
        settings_enable = false;
    }
}
void Window::createRdvPanel(QDate d)
{

    QString text = d.toString("dddd d MMMM yyyy");
    if (settings_enable == true)
    {
        removeLayout(rightlayout);
        settings_enable = false;
    }
    if (rdvs_enable == true)
    {
        rdvHeaderLabel->setText(text);
        rdvt->setSelectedDate(d);
        return;
    }

    rdvHeaderLabel = new QLabel(text);
    rdvHeaderLabel->setStyleSheet("font-size: 22px;border: 1px solid;background: yellow;");
    rdvHeaderLabel->setAlignment(Qt::AlignCenter);
    QHBoxLayout *qhbxl = new QHBoxLayout;

    rightlayout = new QVBoxLayout;
    rightlayout->addLayout(qhbxl);

    layout->addLayout(rightlayout);

    qhbxl->addWidget(rdvHeaderLabel);
    rdvt = new  rdvTable(d,datafile);
    rdvt->setVisible(false);

    for (int i=0;i<24;i++)
    {
        QTime qt(i,0);
        rdvt->addArow(i,qt);
    }

    rdvt->autoresize();
    rightlayout->addWidget(rdvt);
    rdvt->setVisible(true);
    rdvs_enable = true;

}

void Window::removeLayout(QLayout *layout)
{
  while (QLayoutItem *item = layout->takeAt(0))
  {
      if (QWidget * widget = item->widget())
          widget->deleteLater();
      if (QLayout *childLayout = item->layout())
          removeLayout(childLayout);
      delete item;
  }
}
void Window::showRdvWin(int row, int col, MCalendar *cal)
{
    qDebug() << "Row :  " << row << ", col : " << col << "is double Clicked.";
    qDebug() << cal->getDate();
    QDate qd(cal->getYear(),cal->getMonth(),row+1);
    createRdvPanel(qd);
}

int Window::igetKey(const QString &key)
{
  if (mSettings->contains(key) == true)
  {
      bool ok =false;
      int value = mSettings->value(key).toInt(&ok);
      if (ok)
          return value;
  }
  return -1;
}

bool Window::bgetKey(const QString &key)
{
    if (mSettings->contains(key) == true )
    {
        if (mSettings->value(key) == "true")
            return true;
    }
    return false;
}

QString Window::sgetKey(const QString &key)
{
    if (mSettings->contains(key) == true)
    {
        QString value = mSettings->value(key).toString();
        return value;
    }
    return "";
}

void Window::setFirstHolyFont(QFont &fnt,int col)
{
    globalNamespace::firstPublidayFont[col] = fnt;
}

void Window::setSecondHolyFont(QFont &fnt,int col)
{
    globalNamespace::secondPublicDayFont[col] = fnt;
}

void Window::setNormalDayFont(QFont &fnt,int col)
{
    globalNamespace::NormalDayFont[col] = fnt;
}

void Window::setSelectionFont(QFont &fnt,int col)
{
    globalNamespace::selectionFont[col] = fnt;
}

void Window::setFearDayFont(QFont &fnt,int col)
{
    globalNamespace::fearFont[col] = fnt;
}
void Window::readFontSettings(QString funcName)
{
    QByteArray qbMethod = ("set" + funcName).toLatin1();
    const char *method = qbMethod.data();
    for(int i = 0;i < 5 ; i++)
    {
        QString key = "STYLE/" + funcName + QString::number(i);
        QString value =  sgetKey(key);
        if (value != "")
        {
            QFont font;
            bool ok = font.fromString(value);
            if (ok == true)
                QMetaObject::invokeMethod(this,method,Qt::DirectConnection,
                                          Q_ARG(QFont &,font),Q_ARG(int,i));
        }
    }
}
void Window::readPaletteSettings(QString funcName)
{
    QByteArray getMethod  = ("get" + funcName).toLatin1();
    const char *getmethod = getMethod.data();
    QByteArray setMethod =  ("set" + funcName).toLatin1();
    const char *setmethod  = setMethod.data();

    for (int i=0;i< 5 ; i++)
    {
        QString keyFg = "STYLE/foreground_" + funcName + QString::number(i);
        QString keyBg = "STYLE/background_" + funcName + QString::number(i);
        QString fgC = sgetKey(keyFg);
        QString bgC = sgetKey(keyBg);
        QPalette *pal;
        QMetaObject::invokeMethod(this,getmethod,
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(QPalette *, pal),
                                  Q_ARG(int,i)
                                  );
        if (fgC != "" )
        {
            const QColor &fgcolor = QColor(fgC);
            pal->setColor(QPalette::Text,fgcolor);
        }
        if (bgC != "")
        {
            const QColor &bgcolor = QColor(bgC);
            pal->setColor(QPalette::Base,bgcolor);
        }

        QMetaObject::invokeMethod(this,setmethod,
                                  Qt::DirectConnection,
                                  Q_ARG(QPalette &, *pal),
                                  Q_ARG(int,i));
    }
}

int Window::readSettings()
{
   QDir *qdobj = new QDir;
   QString home = qdobj->homePath();
   QString pathIni = home + "/.config/MCalendar";
   QString iniFile = pathIni + "/config.ini";
   QString pathData = home + "/etc";
   QString recordFile = pathData + "/Rdv.dat";
   try
   {
       int ret = checkPath(iniFile,qdobj);
       if (ret != 0)
           throw(ret);

   }
   catch (int err)
   {
       if (err == 2)
           qDebug() << tr("Could not create directory, error ") << "2.";
       else if (err == 3)
           qDebug() << tr("User aborted, error ")  << "3";
       return err;
   }

   mSettings = new QSettings(iniFile,QSettings::IniFormat);

   if (mSettings->contains("GLOBAL/datafile") == false)
   {
       mSettings->setValue("GLOBAL/datafile",recordFile);
       mSettings->sync();
   }
   QVariant qv = mSettings->value("GLOBAL/datafile");
   datafile = qv.toString();
   if (checkPath(datafile,qdobj) != 0)
       return 4;
   QFile *qf = new QFile(datafile);
   if ( qf->exists() == false)
   {
       if (qf->open(QIODevice::WriteOnly) == false)
       {
           QMessageBox::critical(this->parentWidget(),tr("Error"),tr("Could not create file ") + datafile,QMessageBox::Yes|QMessageBox::No);
           return 5;
       }
       qf->close();
   }

   startToday = bgetKey("THEME1/startToday");
   if (noIniFile)
       startToday=true;
    year = igetKey("THEME1/year");
    month = igetKey("THEME1/month");
    nbMonth =igetKey("THEME1/nb_month");
    monthBefore = igetKey("THEME1/month_before");
    globalNamespace::showMoon = bgetKey("THEME1/showMoon");
    globalNamespace::showMoonTime = bgetKey("THEME1/showMoonTime");
    globalNamespace::gridVisible=bgetKey("THEME1/gridVisible");
    globalNamespace::showFear = bgetKey("THEME1/showSaints");
    int firstPblcDay = igetKey("THEME1/firstPublicDay");
    int secondPblcDay = igetKey("THEME1/secondPublicDay");
    if (firstPblcDay != -1)
        grpcal->setFirstHolyDay(firstPblcDay);
    if (secondPblcDay != -1)
        grpcal->setSecondHolyDay(secondPblcDay);
    QString lblfmt = sgetKey("THEME1/headersFormat");
    grpcal->setHeaderLabelFormat(lblfmt);
    int dayFormat = igetKey("THEME1/dayFormat");
    if (dayFormat != -1)
        grpcal->setDayformat(dayFormat);
    QString headerBgcolor = sgetKey("THEME1/headerBgColor");
    QString headerColor   = sgetKey("THEME1/headerColor");
    if (headerBgcolor == "")
        headerBgcolor = "#0000FF"; //Blue
    if (headerColor == "")
        headerColor ="#FFFFFF"; //White
     const QColor hBgColor(headerBgcolor);
     const QColor hFgColor(headerColor);
    QPalette *pal = grpcal->getHeaderPalette();
    pal->setColor(QPalette::WindowText,hFgColor);
    pal->setColor(QPalette::Window,hBgColor);
    QFont *hFnt = grpcal->getHeaderFont();
    QString sHfont = sgetKey("THEME1/headerFont");
    if (sHfont == "")
        *hFnt = QFont("Arial",12,QFont::Bold);
     else
        hFnt->fromString(sHfont);

     readFontSettings("NormalDayFont");
     readFontSettings("FirstHolyFont");
     readFontSettings("SecondHolyFont");
     readFontSettings("SelectionFont");
     readFontSettings("FearDayFont");

     readPaletteSettings("NormalDayPalette");
     readPaletteSettings("FirstHolyPalette");
     readPaletteSettings("SecondHolyPalette");
     readPaletteSettings("SelectionPalette");
     readPaletteSettings("FearDayPalette");

   return 0;
}

void Window::syncSettings()
{
   if (startToday == true)
   {
      mSettings->setValue("THEME1/startToday", "true");
      QSignalBlocker blocker(ckbToday);
      ckbToday->setChecked(true);
      blocker.unblock();
   }
  else
   {
       mSettings->setValue("THEME1/year",QString::number(year));
       mSettings->setValue("THEME1/month",QString::number(month));
       QSignalBlocker blocker(qlyear);
       qlyear->setText(QString::number(year));
       blocker.unblock();
       QSignalBlocker blocker2(qlmonth);
       qlmonth->setText(QString::number(month));
       blocker2.unblock();
   }
    mSettings->setValue("THEME1/nb_month",QString::number(nbMonth));
    mSettings->setValue("THEME1/month_before",QString::number(monthBefore));

    QSignalBlocker blocker(leHowMany);
    leHowMany->setText(QString::number(nbMonth));
    blocker.unblock();
    QSignalBlocker blocker2(leMthBefor);
    leMthBefor->setText(QString::number(monthBefore));
    blocker2.unblock();
    mSettings->sync();
    QSignalBlocker blocker3(ckbAffMoonHour);
    ckbAffMoonHour->setChecked(grpcal->getHasMoonHours());
    blocker3.unblock();
    QSignalBlocker blocker4(ckbAffMoons);
    ckbAffMoons->setChecked(grpcal->getHasMoon());
    blocker4.unblock();

    QSignalBlocker blocker5(qcbJrFer);
    qcbJrFer->setCurrentIndex(grpcal->getFirstHolyDay()-1);
    blocker5.unblock();
    QSignalBlocker blocker6(qcbJrFer2);
    qcbJrFer2->setCurrentIndex(grpcal->getSecondHolyDay()-1);
    blocker6.unblock();

    QSignalBlocker blocker7(qcbMthHeaderFmt);
    QString fmt = grpcal->getHeaderLabelFormat();
    qcbMthHeaderFmt->setCurrentText(fmt);
    blocker7.unblock();
    QSignalBlocker blocker8(qcbFmtDays);
    int format = grpcal->getDayFormat();
    qcbFmtDays->setCurrentIndex(format);
    blocker8.unblock();

    QSignalBlocker blocker9(ckbSetGrid);
    ckbSetGrid->setChecked(globalNamespace::gridVisible);
    blocker9.unblock();
    QSignalBlocker blocker10(ckbSetSt);
    ckbSetSt->setChecked(globalNamespace::showFear);
    blocker10.unblock();
}

void Window::saveFont(QString funcName)
{
    for (int i=0;i<5;i++)
    {
        QString s = QString::number(i);
        QFont *font;
        QByteArray qbMethod = ("get" + funcName).toLatin1();
        const char *method = qbMethod.data();
        QMetaObject::invokeMethod(grpcal, method,Qt::DirectConnection,Q_RETURN_ARG(QFont *,font),Q_ARG(int,i));
        const QString key = "STYLE/" + funcName + s;
        mSettings->setValue(key,(*font).toString());
    }
}
void Window::savePalette(QString funcName)
{
    for (int i=0;i<5;i++)
    {
        QString s = QString::number(i);
        QPalette *pal;
        QByteArray qbMethod = ("get" + funcName).toLatin1();
        const char *method = qbMethod.data();
        QMetaObject::invokeMethod(grpcal,method,Qt::DirectConnection,Q_RETURN_ARG(QPalette *, pal),Q_ARG(int,i));
        const QColor &fg = pal->color(QPalette::Text);
        const QColor &bg = pal->color(QPalette::Base);
        const QString keyfg = "STYLE/foreground_" + funcName + s;
        const QString keybg = "STYLE/background_" + funcName + s;
        mSettings->setValue(keyfg,fg.name());
        mSettings->setValue(keybg,bg.name());
    }
}

void Window::writeSettings()
{
   if (startToday == true)
       mSettings->setValue("THEME1/startToday","true");
   else
       mSettings->setValue("THEME1/startToday","false");
   mSettings->setValue("THEME1/year",QString::number(year));
   mSettings->setValue("THEME1/month",QString::number(month));
   mSettings->setValue("THEME1/nb_month",QString::number(nbMonth));
   mSettings->setValue("THEME1/month_before",QString::number(monthBefore));
   QString s1,s2,s3;
   s1 = (globalNamespace::showMoon == true) ? ("true") : ("false");
   s2 = (globalNamespace::showMoonTime == true) ? ("true") : ("false");
   s3 = (globalNamespace::showFear == true) ? ("true") : ("false");
   mSettings->setValue("THEME1/showMoon",s1);
   mSettings->setValue("THEME1/showMoonTime",s2);
   mSettings->setValue("THEME1/showSaints",s3);

   int firstPblc, secondPblc;
   firstPblc = grpcal->getFirstHolyDay();
   secondPblc = grpcal->getSecondHolyDay();
   mSettings->setValue("THEME1/firstPublicDay",QString::number(firstPblc));
   mSettings->setValue("THEME1/secondPublicDay",QString::number(secondPblc));
   QString grid = (globalNamespace::gridVisible == true) ? ("true") : ("false");
   mSettings->setValue("THEME1/gridVisible",grid);
   mSettings->setValue("THEME1/headersFormat",grpcal->getHeaderLabelFormat());
   int dayf = grpcal->getDayFormat();
   mSettings->setValue("THEME1/dayFormat",QString::number(dayf));

   QPalette *pal =grpcal->getHeaderPalette();
   const QColor &hBgColor = pal->color(QPalette::Window);
   const QColor &hColor = pal->color(QPalette::WindowText);

   mSettings->setValue("THEME1/headerColor",hColor.name());
   mSettings->setValue("THEME1/headerBgColor",hBgColor.name());
   QFont *hfont = grpcal->getHeaderFont();
   mSettings->setValue("THEME1/headerFont",(*hfont).toString());
   saveFont("NormalDayFont");
   saveFont("FirstHolyFont");
   saveFont("SecondHolyFont");
   saveFont("SelectionFont");
   saveFont("FearDayFont");

   savePalette("FirstHolyPalette");
   savePalette("SecondHolyPalette");
   savePalette("NormalDayPalette");
   savePalette("SelectionPalette");
   savePalette("FearDayPalette");
   mSettings->sync();
}

int Window::checkPath(const QString &file,QDir *qdirObj)
{
    QFileInfo qfi(file);

    if ( QFile(file).exists() == false)
    {
        int reponse = QMessageBox::question(this,"Question",QString(QObject::tr("File "))
                                            + file
                                            + QObject::tr(" does not exist.\nDo you want me to create it with default values?"),
                                            QMessageBox::Yes|QMessageBox::No);
        if  (reponse == QMessageBox::Yes)
        {
            QString path = qfi.absolutePath();
            if ( qdirObj->mkpath(path) == false)
            {
                QMessageBox::critical(this, QString(QObject::tr("Error") + "!") , QString(QObject::tr("Could not create path ")) + path + ", " + QString(QObject::tr("is basedir readable?")));
                return 2;
            }
            noIniFile = true;
        }
        else
            return 3;
    }
    return 0;
}


void Window::createGeneralOptionGroupBox()
{
    generalOptionsGroupBox = new QGroupBox(tr("General Options"));

    QLabel    *startWhen = new QLabel(tr("Start When")+ ": ");
    ckbToday = new QCheckBox(tr("This month"));
    QLabel *lblyear = new QLabel(tr("Year") + ": ");
    QLabel *lblmonth = new QLabel(tr("Month ")+ ": ");
    lblyear->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lblmonth->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    qlyear   = new QLineEdit;
    qlyear->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    qlyear->setInputMask("0000");
    qlyear->setCursorPosition(0);
    QFontMetrics fontM(qlyear->font());
    int qlyearWidth = fontM.width("888888");
    qlyear->setMaximumWidth(qlyearWidth);
    qlmonth = new QLineEdit;
    qlmonth->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    qlmonth->setInputMask("00");
    qlmonth->setCursorPosition(0);
    QFontMetrics fontM2(qlmonth->font());
    int qlmonthWidth = fontM2.width("888") ;
    qlmonth->setMaximumWidth(qlmonthWidth);
    QLabel    *lblMth = new QLabel(tr("Month") + ": ");
    QHBoxLayout *hbxMth = new QHBoxLayout;
    QLabel    *lblHowManyMth = new QLabel(tr("How many month to display") + ": ");
    leHowMany = new QLineEdit;


    QLabel    *lblMthBefor = new QLabel(tr("how many Month Before initial date") + ": ");
    leMthBefor = new QLineEdit;
    leMthBefor->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lblHowManyMth->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QFontMetrics fontM3(leMthBefor->font());
    int leMthhWidth = fontM3.width("88") ;
    leMthBefor->setMaximumWidth(leMthhWidth);
    qlmonth->setMaximumWidth(qlmonthWidth);
    leHowMany->setMaximumWidth(leMthhWidth);

    lblMthBefor->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QHBoxLayout *hbxShow = new QHBoxLayout;
    QLabel *lblSetGrid = new QLabel(tr("Show") + ": ");
    ckbSetGrid = new QCheckBox(tr("Grid"));
    ckbSetSt = new QCheckBox(tr("Saints"));
    hbxShow->addWidget(ckbSetGrid);
    hbxShow->addWidget(ckbSetSt);

    hbxMth->addWidget(lblHowManyMth);
    hbxMth->addWidget(leHowMany);
    hbxMth->addWidget(lblMthBefor);
    hbxMth->addWidget(leMthBefor);

    QHBoxLayout *hbxMoon = new QHBoxLayout;
    QLabel      *lblMoon  = new QLabel(tr("Moon") + ": ");
    ckbAffMoonHour = new QCheckBox(tr("Display Moon Hours"));
    ckbAffMoons = new QCheckBox(tr("Display Moons"));
    hbxMoon->addWidget(ckbAffMoons);
    hbxMoon->addWidget(ckbAffMoonHour);

    QLabel    *lblJrFer  = new QLabel(tr("First Public holliday"));
    QHBoxLayout *hbxHoli = new QHBoxLayout;
    qcbJrFer = new QComboBox;
    QStringList days;
    days << tr("Monday");
    days << tr("Tuesday");
    days << tr("Wednesday");
    days << tr("Thursday");
    days << tr("Friday");
    days << tr("Saturday");
    days << tr("Sunday");
    qcbJrFer->addItems(days);
    qcbJrFer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QLabel    *lblHoli = new QLabel("Public Holliday:");
    QLabel    *lblJrFer2  = new QLabel(tr("Second public holliday"));
    qcbJrFer2 = new QComboBox;
    qcbJrFer2->addItems(days);
    qcbJrFer2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    QGridLayout *qgl = new QGridLayout;
    qgl->addWidget(startWhen,0,0);
    QHBoxLayout *hbxStartWhen  = new QHBoxLayout;

    hbxHoli->addWidget(lblJrFer);
    hbxHoli->addWidget(qcbJrFer);
    hbxHoli->addWidget(lblJrFer2);
    hbxHoli->addWidget(qcbJrFer2);

    hbxStartWhen->addWidget(ckbToday);
    hbxStartWhen->addWidget(lblyear);
    hbxStartWhen->addWidget(qlyear);
    hbxStartWhen->addWidget(lblmonth);
    hbxStartWhen->addWidget(qlmonth);

    qgl->addLayout(hbxStartWhen,0,1);
    qgl->addWidget(lblMth,1,0);
    qgl->addLayout(hbxMth,1,1);
    qgl->addWidget(lblMoon,2,0);
    qgl->addLayout(hbxMoon,2,1);
    qgl->addWidget(lblHoli,3,0);
    qgl->addLayout(hbxHoli,3,1);
    qgl->addWidget(lblSetGrid,4,0);
    qgl->addLayout(hbxShow,4,1);

    connect(ckbToday,SIGNAL(stateChanged(int)),this,SLOT(ckbStartDateChanged(int)));
    connect(qlyear,SIGNAL(editingFinished()),this,SLOT(leYearChanged()));
    connect(qlmonth,SIGNAL(editingFinished()),this,SLOT(leMonthChanged()));
    connect(leMthBefor,SIGNAL(editingFinished()),this,SLOT(leMonthBeforeChanged()));
    connect(leHowMany,SIGNAL(editingFinished()),this,SLOT(leHowManyChanged()));
    connect(ckbAffMoonHour,SIGNAL(stateChanged(int)),this,SLOT(ckbMoonTime(int)));
    connect(ckbAffMoons,SIGNAL(stateChanged(int)),this,SLOT(ckbMoon(int)));
    connect(qcbJrFer,SIGNAL(currentIndexChanged(int)),this,SLOT(firstPblcDayChged(int)));
    connect(qcbJrFer2,SIGNAL(currentIndexChanged(int)),this,SLOT(secondPblcDayChged(int)));
    connect(ckbSetGrid,SIGNAL(stateChanged(int)),this,SLOT(ckbShowGrid(int)));
    connect(ckbSetSt,SIGNAL(stateChanged(int)),this,SLOT(ckbShowSaints(int)));

    generalOptionsGroupBox->setLayout(qgl);
}
void Window::createFormatsGroupBox()
{
    formatsGroupBox = new QGroupBox(tr("Formats"));
    QLabel *lblMthHeaderFmt = new QLabel(tr("Month Header Format") + ":");
    qcbMthHeaderFmt = new QComboBox;
    QStringList lstHeaders;
    lstHeaders << "MM/yy";
    lstHeaders << "MMM/yy";
    lstHeaders << "MMMM/yy";
    lstHeaders << "MMMM/yyyy";
    lstHeaders << "MM/yyyy";
    lstHeaders << "MMM/yyyy";
    lstHeaders << "yy/MM";
    lstHeaders << "yy/MMM";
    lstHeaders << "yy/MMMM";
    lstHeaders << "yyyy/MM";
    lstHeaders << "yyyy/MMM";
    lstHeaders << "yyyy/MMMM";
    lstHeaders << "MM yy";
    lstHeaders << "MMM yy";
    lstHeaders << "MMMM yy";
    lstHeaders << "MMMM yyyy";
    lstHeaders << "MM yyyy";
    lstHeaders << "MMM yyyy";
    lstHeaders << "yy MM";
    lstHeaders << "yy MMM";
    lstHeaders << "yy MMMM";
    lstHeaders << "yyyy MM";
    lstHeaders << "yyyy MMM";
    lstHeaders << "yyyy MMMM";
    qcbMthHeaderFmt->addItems(lstHeaders);
    QHBoxLayout *hbxHeadFmt = new QHBoxLayout;
    hbxHeadFmt->addWidget(qcbMthHeaderFmt);
    QLabel *lblFmtDays = new QLabel(tr("Day Name Format") + ": ");
    qcbFmtDays = new QComboBox;
    qcbFmtDays->addItems(QStringList() << tr("One Letter")<<tr("3 Letters")<< tr("Full Day"));
    QHBoxLayout *hbxFmtDays = new QHBoxLayout;
    hbxFmtDays->addWidget(qcbFmtDays);
    QGridLayout *qgl = new QGridLayout;
    qgl->addWidget(lblMthHeaderFmt,0,0);
    qgl->addLayout(hbxHeadFmt,0,1);
    qgl->addWidget(lblFmtDays,1,0);
    qgl->addWidget(qcbFmtDays,1,1);

    connect(qcbMthHeaderFmt,SIGNAL(currentIndexChanged(QString)),this,SLOT(monthHeaderFmtChged(QString)));
    connect(qcbFmtDays,SIGNAL(currentIndexChanged(int)),this,SLOT(dayNameTypeChanged(int)));

    formatsGroupBox->setLayout(qgl);
}
void Window::createStylesGroupBox()
{
    stylesGroupBox = new QGroupBox(tr("Styles"));
    QLabel *lblHeaderStyle = new QLabel(tr("Header Style")+ ":");
    QHBoxLayout *hbxHeaderStyle = new QHBoxLayout;
    QPushButton *headerBtncc = new QPushButton(tr("Color"));
    QPushButton *headerBtnbgcc = new QPushButton(tr("Background color"));
    QPushButton *headerBtncp = new QPushButton(tr("Police"));
    connect(headerBtncc,SIGNAL(clicked(bool)),this,SLOT(btnColorHeaderClicked()));
    connect(headerBtnbgcc,SIGNAL(clicked(bool)),this,SLOT(btnBgColorHeaderClicked()));
    connect(headerBtncp,SIGNAL(clicked(bool)),this,SLOT(btnPoliceHeaderClicked()));

    hbxHeaderStyle->addWidget(headerBtncc);
    hbxHeaderStyle->addWidget(headerBtnbgcc);
    hbxHeaderStyle->addWidget(headerBtncp);
    headerBtncc->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    headerBtnbgcc->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    headerBtncp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QLabel *lblFirstHolyDayStyle = new QLabel(tr("First public hollyday Style")+":");
    QLabel *lblSecondHolyDayCol = new QLabel(tr("column")+":");
    QLabel *lblNormalDayStyle = new QLabel(tr("Normal Day Style")+":");
    QLabel *lblSelectionStyle = new QLabel(tr("Selection Style")+":");

    QLabel *lblSecondHolyDayStyle = new QLabel(tr("Second public hollyday Style")+ ":");
    QLabel *lblFirstHolyDayCol = new QLabel(tr("column")+":");
    QLabel *lblNormalDayCol = new QLabel(tr("column")+":");
    QLabel *lblSelectionCol = new QLabel(tr("column")+":");

    QLabel *lblFearStyle        = new QLabel(tr("Fear Style")+":");
    QLabel *lblFearCol          = new QLabel(tr("column")+":");

    QList<QCheckBox *> ckbFirstHoly;
    QList<QCheckBox *> ckbSecondHoly;
    QList<QCheckBox *> ckbNormalDay;
    QList<QCheckBox *> ckbSelection;
    QList<QCheckBox *> ckbFear;


    QHBoxLayout *hbxFirstDay = new QHBoxLayout;
    QHBoxLayout *hbxSecondDay = new QHBoxLayout;
    QHBoxLayout *hbxNormalDay = new QHBoxLayout;
    QHBoxLayout *hbxFear = new QHBoxLayout;
    QHBoxLayout *hbxSelectionDay = new QHBoxLayout;

    hbxNormalDay->addWidget(lblNormalDayCol);
    hbxFirstDay->addWidget(lblFirstHolyDayCol);
    hbxSecondDay->addWidget(lblSecondHolyDayCol);
    hbxSelectionDay->addWidget(lblSelectionCol);
    hbxFear->addWidget(lblFearCol);

    //First Holy
    btnGrpFirstHoly = new QButtonGroup;
    btnGrpFirstHoly->setExclusive(true);
    for (int i=0;i<5;i++){
        ckbFirstHoly << new QCheckBox(QString::number((i+1)));
        btnGrpFirstHoly->addButton(ckbFirstHoly[i]);
        hbxFirstDay->addWidget(ckbFirstHoly[i]);
       // connect(ckbFirstHoly[i],SIGNAL(stateChanged(int)),this,SLOT(ckbFirstHolyChanged()));
    }
    QPushButton *firstDayBtncc = new QPushButton(tr("Color"));
    QPushButton *firstDayBtnbgcc = new QPushButton(tr("Background color"));
    QPushButton *firstDayBtncp = new QPushButton(tr("Police"));
    hbxFirstDay->addWidget(firstDayBtncc);
    hbxFirstDay->addWidget(firstDayBtnbgcc);
    hbxFirstDay->addWidget(firstDayBtncp);
    connect(firstDayBtncc,SIGNAL(clicked(bool)),this,SLOT(btnColorFirstDayClicked()));
    connect(firstDayBtnbgcc,SIGNAL(clicked(bool)),this,SLOT(btnBgColorFirstDayClicked()));
    connect(firstDayBtncp,SIGNAL(clicked(bool)),this,SLOT(btnPoliceFirstDayClicked()));

    //Second Holy
    btnGrpSecondHoly = new QButtonGroup;
    btnGrpSecondHoly->setExclusive(true);
    for (int i=0;i<5;i++) {
        ckbSecondHoly << new QCheckBox(QString::number((i+1)));
        btnGrpSecondHoly->addButton(ckbSecondHoly[i]);
        hbxSecondDay->addWidget(ckbSecondHoly[i]);
       // connect(ckbSecondHoly[i],SIGNAL(stateChanged(int)),this,SLOT(ckbSecondHolyChanged()));
    }
    QPushButton *secondDayBtncc = new QPushButton(tr("Color"));
    QPushButton *secondDayBtnbgcc = new QPushButton(tr("Background color"));
    QPushButton *secondDayBtncp = new QPushButton(tr("Police"));
    connect(secondDayBtncc,SIGNAL(clicked(bool)),this,SLOT(btnColorSecondDayClicked()));
    connect(secondDayBtnbgcc,SIGNAL(clicked(bool)),this,SLOT(btnBgColorSecondDayClicked()));
    connect(secondDayBtncp,SIGNAL(clicked(bool)),this,SLOT(btnPoliceSecondDayClicked()));
    hbxSecondDay->addWidget(secondDayBtncc);
    hbxSecondDay->addWidget(secondDayBtnbgcc);
    hbxSecondDay->addWidget(secondDayBtncp);

    //NormalDay
    btnGrpNormalHoly = new QButtonGroup;
    btnGrpNormalHoly->setExclusive(true);
    for (int i=0;i<5;i++) {
        ckbNormalDay << new QCheckBox(QString::number((i+1)));
        btnGrpNormalHoly->addButton(ckbNormalDay[i]);
        hbxNormalDay->addWidget(ckbNormalDay[i]);
       // connect(ckbNormalDay[i],SIGNAL(stateChanged(int)),this,SLOT(ckbNormalDayChanged()));
    }
    QPushButton *normalDayBtncc = new QPushButton(tr("Color"));
    QPushButton *normalDayBtnbgcc = new QPushButton(tr("Background color"));
    QPushButton *normalDayBtncp = new QPushButton(tr("Police"));
    connect(normalDayBtncc,SIGNAL(clicked(bool)),this,SLOT(btnColorNormalDayClicked()));
    connect(normalDayBtnbgcc,SIGNAL(clicked(bool)),this,SLOT(btnBgColorNormalDayClicked()));
    connect(normalDayBtncp,SIGNAL(clicked(bool)),this,SLOT(btnPoliceNormalDayClicked()));
    hbxNormalDay->addWidget(normalDayBtncc);
    hbxNormalDay->addWidget(normalDayBtnbgcc);
    hbxNormalDay->addWidget(normalDayBtncp);


    //Fear
    btnGrpFear = new QButtonGroup;
    btnGrpFear->setExclusive(true);
    for (int i=0;i<5;i++) {
        ckbFear << new QCheckBox(QString::number((i+1)));
        btnGrpFear->addButton(ckbFear[i]);
        hbxFear->addWidget(ckbFear[i]);
       // connect(ckbFear[i],SIGNAL(stateChanged(int)),this,SLOT(ckbFearChanged()));
    }
    QPushButton *fearDayBtncc = new QPushButton(tr("Color"));
    QPushButton *fearDayBtnbgcc = new QPushButton(tr("Background color"));
    QPushButton *fearDayBtncp = new QPushButton(tr("Police"));
    connect(fearDayBtncc,SIGNAL(clicked(bool)),this,SLOT(btnColorFearDayClicked()));
    connect(fearDayBtnbgcc,SIGNAL(clicked(bool)),this,SLOT(btnBgColorFearDayClicked()));
    connect(fearDayBtncp,SIGNAL(clicked(bool)),this,SLOT(btnPoliceFearDayClicked()));

    hbxFear->addWidget(fearDayBtncc);
    hbxFear->addWidget(fearDayBtnbgcc);
    hbxFear->addWidget(fearDayBtncp);

    //Selection
    btnGrpSelection = new QButtonGroup;
    btnGrpSelection->setExclusive(true);
    for (int i=0;i<5;i++) {
        ckbSelection << new QCheckBox(QString::number((i+1)));
        btnGrpSelection->addButton(ckbSelection[i]);
        hbxSelectionDay->addWidget(ckbSelection[i]);
        //connect(ckbSelection[i],SIGNAL(stateChanged(int)),this,SLOT(ckbSelectionChanged()));
    }
    QPushButton *selectedDayBtncc = new QPushButton(tr("Color"));
    QPushButton *selectedDayBtnbgcc = new QPushButton(tr("Background color"));
    QPushButton *selectedDayBtncp = new QPushButton(tr("Police"));
    connect(selectedDayBtncc,SIGNAL(clicked(bool)),this,SLOT(btnColorSelectedDayClicked()));
    connect(selectedDayBtnbgcc,SIGNAL(clicked(bool)),this,SLOT(btnBgColorSelectedDayClicked()));
    connect(selectedDayBtncp,SIGNAL(clicked(bool)),this,SLOT(btnPoliceSelectedDayClicked()));

    hbxSelectionDay->addWidget(selectedDayBtncc);
    hbxSelectionDay->addWidget(selectedDayBtnbgcc);
    hbxSelectionDay->addWidget(selectedDayBtncp);


    QGridLayout *qgl = new QGridLayout;

    qgl->addWidget(lblHeaderStyle,1,0);
    qgl->addLayout(hbxHeaderStyle,1,1);
    qgl->addWidget(lblNormalDayStyle,2,0);
    qgl->addLayout(hbxNormalDay,2,1);
    qgl->addWidget(lblFirstHolyDayStyle,3,0);
    qgl->addLayout(hbxFirstDay,3,1);
    qgl->addWidget(lblSecondHolyDayStyle,4,0);
    qgl->addLayout(hbxSecondDay,4,1);

    qgl->addWidget(lblFearStyle,5,0);
    qgl->addLayout(hbxFear,5,1);

    qgl->addWidget(lblSelectionStyle,6,0);
    qgl->addLayout(hbxSelectionDay,6,1);

    stylesGroupBox->setLayout(qgl);
}

void Window::ckbStartDateChanged(int st)
{
    startToday = (st == 0) ? false : true;

    if (startToday == true)
    {
        QDate dt(QDate::currentDate());
        grpcal->jumpTo(dt);
    }
}

void Window::leYearChanged()
{
    if (ckbToday->isChecked() == true)
        return;
    //int month,year;
    QString text =qlmonth->text();
    bool ok = false;

    if (text.contains(QRegExp("^[0-9]+$")) == true)
        month = text.toInt(&ok);
    else
        return;
    if (ok == false)
        return;

    ok =false;
    text = qlyear->text();
    if (text.contains(QRegExp("^[0-9]+$")) == true)
        year = text.toInt(&ok);
    else
        return;
    if (ok == false)
        return;

    QDate dt(year,month,1);
    grpcal->jumpTo(dt);

}

void Window::leMonthChanged()
{
    if (ckbToday->isChecked() == true)
        return;
   // int month,year;
    QString text =qlyear->text();
    bool ok = false;
    if (text.contains(QRegExp("^[0-9]+$")) == true)
        year = text.toInt(&ok);
    else
        return;
    if (ok == false)
        return;
    ok = false;
    text = qlmonth->text();
    if (text.contains(QRegExp("^[0-9]+$")) == true)
        month = text.toInt(&ok);
    else
        return;
    if (ok == false)
        return;
    QDate dt(year,month,1);
    grpcal->jumpTo(dt);
}

void Window::leMonthBeforeChanged()
{
    int monthB;
    bool ok = false;
    QString text =leMthBefor->text();
    if (text.contains(QRegExp("^[0-9]+$")) == true)
        monthB=text.toInt(&ok);
    else
        return;
    if (ok == false)
        return;
    monthBefore = monthB;
    grpcal->setMonthBefore(monthB);
}

void Window::leHowManyChanged()
{
    int hmMonth;

    bool ok = false;
    QString text =leHowMany->text();
    if (text.contains(QRegExp("^[0-9]+$")) == true)
        hmMonth=text.toInt(&ok);
    else
        return;
    if (ok == false)
        return;
    nbMonth = hmMonth;
    grpcal->setNbMonth(hmMonth);
}

void Window::ckbMoonTime(int st)
{
    bool sg = (st == 0) ? false : true;
    globalNamespace::showMoonTime = sg;

    grpcal->redrawAfterDataChanged();
}

void Window::ckbMoon(int st)
{
    bool sg = (st == 0) ? false : true;
    globalNamespace::showMoon = sg;
    grpcal->redrawAfterDataChanged();
}
void Window::ckbShowGrid(int n)
{
    bool sg = (n == 0) ? false : true;
    globalNamespace::gridVisible = sg;
    grpcal->redrawAfterDataChanged();

}
void Window::ckbShowSaints(int n)
{
     bool sg = (n == 0) ? false : true;
     globalNamespace::showFear = sg;
     grpcal->redrawAfterDataChanged();
}

void Window::firstPblcDayChged(int n)
{
    grpcal->setFirstHolyDay(n + 1);
    grpcal->redrawAfterDataChanged();
}

void Window::secondPblcDayChged(int n)
{
    grpcal->setSecondHolyDay(n + 1);
    grpcal->redrawAfterDataChanged();
}

void Window::monthHeaderFmtChged(QString fmt)
{
    grpcal->setHeaderLabelFormat(fmt);
    grpcal->refreshHeaderLabel();
}

void Window::dayNameTypeChanged(int n)
{
    grpcal->setDayformat(n);
    grpcal->redrawAfterDataChanged();
}

void Window::btnColorHeaderClicked()
{
    QPalette *pal = grpcal->getHeaderPalette();
    const QColor &color = pal->color(QPalette::WindowText);
    QColor newColor = QColorDialog::getColor(color);
    if (newColor.isValid() == true)
        grpcal->setHeadersFgColor(newColor);
}

void Window::btnBgColorHeaderClicked()
{
    QPalette *pal = grpcal->getHeaderPalette();
    const QColor &color = pal->color(QPalette::Window);
    QColor newColor = QColorDialog::getColor(color);
    if (newColor.isValid() == true)
        grpcal->setHeadersBgColor(newColor);

}

void Window::btnPoliceHeaderClicked()
{
    bool ok = false;
    QFont *font = grpcal->getHeaderFont();
    const QFont &fnt = *font;
    QFont newFont = QFontDialog::getFont(&ok,fnt);
    if (ok == true)
        grpcal->setHeadersFont(newFont);
}

//Start Here

int Window::getCheckedCol(QButtonGroup *qbtn)
{
    QList <QAbstractButton *> buttons = qbtn->buttons();

    for(int i = 0;i< buttons.count();i++)
    {
        QCheckBox *ckb = dynamic_cast<QCheckBox *>(buttons[i]);
        if (ckb != 0)
        {
            if (ckb->isChecked() == true)
                return i;
        }
    }
   return -1;
}

void Window::genericColumnsColorChange(QButtonGroup *qbtn,QString id,QPalette::ColorRole palType)
{
    int i = getCheckedCol(qbtn);
     const QString getident = "get" + id;
     const QString setidend = "set" + id;
     QByteArray ba1,ba2;
     ba1 = getident.toLatin1();
     ba2  = setidend.toLatin1();
     const char * get_identifiant = ba1.data();
     const  char * set_identifiant = ba2.data();
    if (i != -1)
    {

        QPalette *pal;
        QMetaObject::invokeMethod(grpcal,get_identifiant,
                                  Qt::DirectConnection,
                                  Q_RETURN_ARG(QPalette *, pal),
                                  Q_ARG(int,i)
                                  );
        const QColor &color = pal->color(palType);
        QColor newColor = QColorDialog::getColor(color);
        if (newColor.isValid()) {
            pal->setColor(palType,newColor);
            QMetaObject::invokeMethod(grpcal,set_identifiant,
                         Qt::DirectConnection,
                         Q_ARG(QPalette &,*pal),
                         Q_ARG(int,i)
                         );
        }
    }
    else
    {
        QColor color = QColorDialog::getColor();
        if (color.isValid())
        {
            for (int n=0;n<5;n++)
            {
                QPalette *pal;
                QMetaObject::invokeMethod(grpcal,get_identifiant,
                                          Qt::DirectConnection,
                                          Q_RETURN_ARG(QPalette *, pal),
                                          Q_ARG(int,n)
                                          );
                pal->setColor(palType,color);
                QMetaObject::invokeMethod(grpcal,set_identifiant,
                             Qt::DirectConnection,
                             Q_ARG(QPalette &,*pal),
                             Q_ARG(int,n),
                             Q_ARG(bool,false)
                             );
            }
            grpcal->redrawAfterDataChanged();
        }
    }
}

void Window::btnColorFirstDayClicked()
{
    genericColumnsColorChange(btnGrpFirstHoly,"FirstHolyPalette",QPalette::Text);
}

void Window::btnBgColorFirstDayClicked()
{
    genericColumnsColorChange(btnGrpFirstHoly,"FirstHolyPalette",QPalette::Base);
}

void Window::btnColorSecondDayClicked()
{
    genericColumnsColorChange(btnGrpSecondHoly,"SecondHolyPalette",QPalette::Text);
}

void Window::btnBgColorSecondDayClicked()
{
    genericColumnsColorChange(btnGrpSecondHoly,"SecondHolyPalette",QPalette::Base);

}

void Window::btnColorNormalDayClicked()
{
    genericColumnsColorChange(btnGrpNormalHoly,"NormalDayPalette",QPalette::Text);
}


void Window::btnBgColorNormalDayClicked()
{
    genericColumnsColorChange(btnGrpNormalHoly,"NormalDayPalette",QPalette::Base);
}


void Window::btnColorFearDayClicked()
{
    genericColumnsColorChange(btnGrpFear,"FearDayPalette",QPalette::Text);
}


void Window::btnBgColorFearDayClicked()
{
    genericColumnsColorChange(btnGrpFear,"FearDayPalette",QPalette::Base);
}

void Window::btnColorSelectedDayClicked()
{
    genericColumnsColorChange(btnGrpSelection,"SelectionPalette",QPalette::Text);
}


void Window::btnBgColorSelectedDayClicked()
{
     genericColumnsColorChange(btnGrpSelection,"SelectionPalette",QPalette::Base);
}
void Window::genericColumnsPoliceChange(QButtonGroup *qbtn,QString id)
{
    int i = getCheckedCol(qbtn);
    const QString getident = "get" + id;
    const QString setidend = "set" + id;
    QByteArray ba1,ba2;
    ba1 = getident.toLatin1();
    ba2  = setidend.toLatin1();
    const char * get_identifiant = ba1.data();
    const  char * set_identifiant = ba2.data();

    if (i != -1)
    {
       QFont *fnt;
       QMetaObject::invokeMethod(grpcal,get_identifiant,
                                 Qt::DirectConnection,
                                 Q_RETURN_ARG(QFont *, fnt),
                                 Q_ARG(int,i)
                                 );
       const QFont &font = *fnt;
       bool ok = false;
       QFont newFont = QFontDialog::getFont(&ok,font);
       if (ok == true)
           QMetaObject::invokeMethod(grpcal,set_identifiant,
                        Qt::DirectConnection,
                        Q_ARG(QFont &,newFont),
                        Q_ARG(int,i)
                        );
    }
    else
    {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok);
        if (ok == true)
        {
            for (int n=0;n<5;n++)
            {
                QMetaObject::invokeMethod(grpcal,set_identifiant,
                             Qt::DirectConnection,
                             Q_ARG(QFont &,font),
                             Q_ARG(int,n),
                             Q_ARG(bool,false)
                             );
            }
            grpcal->redrawAfterDataChanged();
        }
    }
}

void Window::btnPoliceNormalDayClicked()
{
    genericColumnsPoliceChange(btnGrpNormalHoly,"NormalDayFont");
}

void Window::btnPoliceFirstDayClicked()
{
    genericColumnsPoliceChange(btnGrpFirstHoly,"FirstHolyFont");
}
//Marque
void Window::btnPoliceSecondDayClicked()
{
    genericColumnsPoliceChange(btnGrpSecondHoly,"SecondHolyFont");
}

void Window::btnPoliceFearDayClicked()
{
   genericColumnsPoliceChange(btnGrpFear,"FearDayFont");
}

void Window::btnPoliceSelectedDayClicked()
{
   genericColumnsPoliceChange(btnGrpSelection,"SelectionFont");
}

void Window::setFirstHolyPalette(QPalette &pal,int col)
{
  globalNamespace::firstPublicDayPalette[col] = pal;
}

void Window::setSecondHolyPalette(QPalette &pal,int col)
{
  globalNamespace::secondPublicDayPalette[col] = pal;
}

void Window::setNormalDayPalette(QPalette &pal,int col)
{
   globalNamespace::NormalDayPalette[col] = pal;
}

void Window::setSelectionPalette(QPalette &pal,int col)
{
   globalNamespace::selectionPalette[col] = pal;
}

void Window::setFearDayPalette(QPalette &pal,int col)
{
  globalNamespace::fearPalette[col] = pal;
}

QPalette * Window::getFirstHolyPalette(int col)
{
  return &globalNamespace::firstPublicDayPalette[col];
}

QPalette * Window::getSecondHolyPalette(int col)
{
  return &globalNamespace::secondPublicDayPalette[col];
}

QPalette * Window::getNormalDayPalette(int col)
{
  return &globalNamespace::NormalDayPalette[col];
}

QPalette * Window::getSelectionPalette(int col)
{
  return &globalNamespace::selectionPalette[col];
}

QPalette * Window::getFearDayPalette(int col)
{
  return &globalNamespace::fearPalette[col];
}
