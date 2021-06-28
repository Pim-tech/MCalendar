#ifndef EDITOR_H
#define EDITOR_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QList>
#include <QTableWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QPlainTextEdit>
#include <QSignalMapper>
#include <QSettings>
#include <QDir>

#include "recorder.h"
#include "viewerConsole/datefilter.h"


#define CLICKED 1
#define MODIFIED 2
#define CANCEL_BUTTON 4
#define SAVE_BUTTON 8
#define CANCEL_CLICKED 16

struct ann_ppts
{
    const QString msg;
    const QString ppt;
};

class viewerEditor : public QWidget
{
     Q_OBJECT
     QString *centerString(const QString str, int len);
     void drawBottomLayout();
     int offsetToIndex(int offsetDays);
     void display();
     const ann_ppts *get_annonce_from_index(int index);
     void drawSettingsLayout();
     QLineEdit * get_linedit(int nth);
     int checkPath(const QString &file, QDir *qdirObj);
     bool eventFilter(QObject *target, QEvent *event);

public slots:
     void refresh();

private slots:
     void btnSuppClicked(int row);
     void delete_selected_rows();
     void apply_bounds();
     void save_bounds();

public:
    viewerEditor();
    void init();
    void autoresizeTable();
    QTableWidget * getParentTable();
    recorder *getRecord();
    bool isFinished() const { return isFinished_; }
    int changebtnDelete2Cancel(int i, QObject *obj);
    int createBtnSaveChg(int n, QObject *obj);
    void btnCancelClicked(int n, QWidget *which);
    void btnSaveChgClicked(int n);


private:
    const QVector<ann_ppts> annonces =
    {
        {"RENDEZ-VOUS PASSE : ","color: #66ffff; background-color: #996600; font: bold 12px;"},
        {"HIER (RENDEZ-VOUS PASSE) : ","color: #000000; background-color: #AAAAAA; font: bold 12px;"},
        {"AUJOURD'HUI : ", "color: yellow; background-color: red; background-color: red; font: bold 12px;"},
        {"DEMAIN : ","color: white; background-color: blue; font: bold 12px;"},
        {"APRES-DEMAIN : ","color: white; background-color: green; font: bold 12px;"},
        {"DANS TROIS JOURS : ","color: #66ffff; background-color: red; font: bold 12px;"},
        {"DANS QUATRES JOURS : ","color: white; background-color: #996600; font: bold 12px;"},
        {"DANS CINQ JOURS : ","color: #66ffff; background-color: #996600; font: bold 12px;"},
        {"DANS SIX JOURS : ","color: green; background-color: red; font: bold 12px;"},
        {"DANS SEPT JOURS : ","color: red; background-color: blue; font: bold 12px;"},
        {"DANS HUIT JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS NEUF JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS DIX JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS DOUZE JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS TREIZE JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS QUATORZE JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS QUINZE JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"DANS SEIZE JOURS : ","color: yellow; background-color: #996600; font: bold 12px;"},
        {"LATER : "           ,"color: #FFFFFF; background-color: #996687; font: bold 12px;" }
    };

    const int seek_display = 2;
    QVBoxLayout *layout;
    QHBoxLayout * bottomLayout;
    QHBoxLayout * settings_layout;
    QTableWidget *qtw;
    recorder *record;
    QMultiMap<QDateTime,dt_per_msg> *disp;
    QSignalMapper *qsm;
    int offsetDaysBeforeToday;
    qint64 offsetDaysAfterToday;
    QMap<QDateTime,QPair<QString,int> > *rdvs;
    dateFilter *dt_f;
    QSettings *mSettings;
    bool isFinished_ = false;
    int error=0;
};

#endif
