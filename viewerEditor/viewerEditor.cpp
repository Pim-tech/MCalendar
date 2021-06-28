
#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMessageBox>
#include <QLineEdit>
#include <QDir>
#include <QApplication>
#include <QTimeEdit>
#include <QComboBox>


#include "viewerEditor.h"
#include "TextEdit.h"
#include "recorder.h"
#include "TimeEdit.h"
#include "Combo.h"

viewerEditor::viewerEditor()
{
    setAttribute(Qt::WA_QuitOnClose);

    QDir *qdobj = new QDir;
    QString home = qdobj->homePath();
    QString pathIni = home + "/.config/myqtcalendar";
    QString iniFile = pathIni + "/config.ini";
    QString pathData = home + "/etc";
    QString recordFile = pathData + "/Rdv.dat";

    int ret = checkPath(iniFile,qdobj);
    if (ret != 0)
    {
        isFinished_ = true;
        return;
    }
    mSettings = new QSettings(iniFile,QSettings::IniFormat);
    if (mSettings->contains("GLOBAL/datafile") == false)
        mSettings->setValue("GLOBAL/datafile",recordFile);
    mSettings->sync();
    QVariant qv = mSettings->value("GLOBAL/datafile");
    QString qdatafile = qv.toString();
    if (checkPath(qdatafile,qdobj) != 0)
    {
        isFinished_ = true;
        return;
    }
    QFile *qf = new QFile(qdatafile);
    if ( qf->exists() == false)
    {
        if (qf->open(QIODevice::WriteOnly) == false)
        {
            QMessageBox::critical(this,tr("Error"),tr("Could not create file ") + qdatafile,QMessageBox::Yes|QMessageBox::No);
            isFinished_ = true;
            return;
        }
        qf->close();
    }
    this->record = new recorder(qdatafile);

    init();
}

recorder * viewerEditor::getRecord()
{
    return this->record;
}

int viewerEditor::checkPath(const QString &file,QDir *qdirObj)
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
                QMessageBox::critical(this, QString(QObject::tr("Error!")) , QString(QObject::tr("Could not create path ")) + path +  QString(QObject::tr(" : is basedir readable?")));
                return 2;
            }
        }
        else
            return 3;
    }
    return 0;
}

void viewerEditor::init()
{
    offsetDaysBeforeToday = 0;
    offsetDaysAfterToday =30;


    if (  mSettings->contains("VIEWEREDITOR/daysbefore") == false)
        mSettings->setValue("VIEWEREDITOR/daysbefore",QString::number(offsetDaysBeforeToday));
    if (  mSettings->contains("VIEWEREDITOR/daysafter") == false)
        mSettings->setValue("VIEWEREDITOR/daysafter",QString::number(offsetDaysAfterToday));

    mSettings->sync();

    //creating layout
    QVariant offsetDaysBefore, offsetDaysAfter;
    offsetDaysBefore =   mSettings->value("VIEWEREDITOR/daysbefore");
    offsetDaysAfter  =   mSettings->value("VIEWEREDITOR/daysafter");

    offsetDaysBeforeToday = offsetDaysBefore.toInt();
    offsetDaysAfterToday  = offsetDaysAfter.toInt();

    qsm = new QSignalMapper(this);
    connect(qsm,SIGNAL(mapped(int)),this,SLOT(btnSuppClicked(int)));

    layout = new QVBoxLayout;
    bottomLayout = new QHBoxLayout;
    qtw = new QTableWidget;
    drawBottomLayout();
    layout->addWidget(qtw);
    drawSettingsLayout();
    layout->addLayout(settings_layout);
    layout->addLayout(bottomLayout);
    //Window Auto-rezize itself when its contents grows or decreases
    layout->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(layout);

    //Initialize table properties
    qtw->verticalHeader()->hide();
    qtw->horizontalHeader()->hide();
    qtw->setStyleSheet("background-color: #CCFFFF;");
    qtw->setColumnCount(3);
    qtw->setAutoScroll(false);
    qtw->setVisible(false);

    // Records reading
    rdvs = record->getData();
    // DateFiltering for print
    dt_f = new dateFilter(rdvs,false);
    dt_f->setBounds(offsetDaysAfterToday,offsetDaysBeforeToday);
    dt_f->filter();
    int count  = dt_f->countForDisplay();
    disp  = dt_f->getSorted();
    qtw->setRowCount(count);
    display();
}

bool viewerEditor::eventFilter(QObject *target, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space)
        {
            focusNextChild();
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}

void viewerEditor::drawSettingsLayout()
{
    settings_layout = new QHBoxLayout;

    QLabel *lbl0 = new QLabel(tr("Display "));
    QLabel *lbl1 = new QLabel(tr(" day(s) before and "));
    QLabel *lbl2 = new QLabel(tr(" day(s) after today "));
    QLineEdit *ql1,*ql2;
    ql1 = new QLineEdit;
    ql2 = new QLineEdit;
    ql1->setObjectName("ql1");
    ql2->setObjectName("ql2");
    ql1->setVisible(true);

    QPushButton *btnSaveSett  = new QPushButton(tr("Save this Setting"));
    QPushButton *btnApply = new QPushButton(tr("Apply"));
    btnSaveSett->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    //qpb.setStyleSheet....
    QFontMetrics fontT(btnSaveSett->font());
    int qpbWidth = fontT.width("A") * btnSaveSett->text().length();
    btnSaveSett->setMaximumWidth(qpbWidth);

    ql1->setInputMask("000");
    ql2->setInputMask("000");
    ql1->setStyleSheet("font: bold 12px;color: #FFFFFF; background-color: #0000FF;");
    ql2->setStyleSheet("font: bold 12px;color: #FFFFFF; background-color: #0000FF;");

    ql1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    ql2->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    ql1->setCursorPosition(0);
    ql2->setCursorPosition(0);
    QFontMetrics fontM(ql1->font());
    int textWidth = fontM.width("A") * (3+1) ;

    ql1->setMaximumWidth(textWidth);
    ql2->setMaximumWidth(textWidth);

    ql1->setText(QString::number(offsetDaysBeforeToday));
    ql2->setText(QString::number(offsetDaysAfterToday));
    connect(btnApply,SIGNAL(clicked()),this,SLOT(apply_bounds()));
    connect(btnSaveSett,SIGNAL(clicked()),this,SLOT(save_bounds()));

    settings_layout->setAlignment(btnApply,Qt::AlignLeft);
    settings_layout->setAlignment(btnSaveSett,Qt::AlignRight);
    settings_layout->setAlignment(ql1,Qt::AlignLeft);
    settings_layout->setAlignment(ql2,Qt::AlignLeft);

    settings_layout->addWidget(lbl0);
    settings_layout->addWidget(ql1);
    settings_layout->addWidget(lbl1);
    settings_layout->addWidget(ql2);
    settings_layout->addWidget(lbl2);
    settings_layout->addWidget(btnApply);
    settings_layout->addWidget(btnSaveSett);
}

QLineEdit * viewerEditor::get_linedit(int nth)
{
    QLineEdit *ql;
    QLayoutItem *item1 = settings_layout->itemAt(nth);
    if (item1 != nullptr)
    {
        if (QWidget * widget = item1->widget() )
        {
            ql = dynamic_cast<QLineEdit*>( widget);
            if (ql != nullptr)
                return ql;
        }
    }
    return nullptr;
}

void viewerEditor::apply_bounds()
{
    QLineEdit * ql1 = get_linedit(1);
    QLineEdit * ql2 = get_linedit(3);

    QRegExp rgx("([0-9]+)");

    if (ql1 != nullptr)
    {
        QString text = ql1->text();
        if (rgx.indexIn(text) > -1)
        {
            bool converted;
            QString num = rgx.cap(1);
            offsetDaysBeforeToday =  num.toInt(&converted,10);
            if (converted == false)
            {
                //Avertir QmsgBox
                this->close();
                return;
            }
        }
    }
    if (ql2 != nullptr)
    {
        QString texte = ql2->text();
        if (rgx.indexIn(texte) > -1)
        {
            bool converted;
            QString num = rgx.cap(1);
            offsetDaysAfterToday = num.toInt(&converted,10);
            if (converted == false)
            {
                //Avertir QmsgBox
                this->close();
                return;
            }
        }
    }
    refresh();
}

void viewerEditor::save_bounds()
{
    mSettings->setValue("VIEWEREDITOR/daysbefore",QString::number(offsetDaysBeforeToday));
    mSettings->setValue("VIEWEREDITOR/daysafter",QString::number(offsetDaysAfterToday));
    mSettings->sync();
}

void viewerEditor::drawBottomLayout()
{
    QPushButton *btn_refresh = new QPushButton(tr("Refresh"));
    QPushButton *btn_exit = new QPushButton(tr("Exit"));
    QPushButton *btn_delete_rows = new QPushButton(tr("Delete selected rows"));
    connect(btn_exit,SIGNAL(clicked()),this,SLOT(close()));
    connect(btn_refresh,SIGNAL(clicked(bool)),this,SLOT(refresh()));
    connect(btn_delete_rows,SIGNAL(clicked()),this,SLOT(delete_selected_rows()));
    bottomLayout->setAlignment(Qt::AlignBottom);
    bottomLayout->setAlignment(btn_exit,Qt::AlignRight);
    bottomLayout->setAlignment(btn_refresh,Qt::AlignLeft);
    bottomLayout->setAlignment(btn_delete_rows,Qt::AlignRight);
    bottomLayout->addWidget(btn_refresh);
    bottomLayout->addWidget(btn_exit);
    bottomLayout->addWidget(btn_delete_rows);
}

int viewerEditor::offsetToIndex(int offsetDays)
{
    int index = offsetDays + seek_display;
    index = (int) ((index<0) ? 0 : (index));
    if ((index ) > (annonces.count() - seek_display))
        index = (annonces.count() - seek_display) + 1;
    return index;
}

void viewerEditor::delete_selected_rows()
{
    for (int i=0;i<qtw->rowCount();i++)
    {
        QWidget *wCkb = qtw->cellWidget(i,2);
        QWidget *wPte = qtw->cellWidget(i,0);
        QCheckBox *ckb = wCkb->findChild<QCheckBox *>("ckb");
        TextEdit  *te  = wPte->findChild<TextEdit *>("content");
        QDateTime *dt = te->getDateTime();
        if (ckb != nullptr)
            if (ckb->isChecked() == true)
                record->delRecord(*dt);
    }
    record->save();
    refresh();
}

void viewerEditor::refresh()
{
    if (record->reload() == ALLISOK)
    {

        disp->clear();

        int ret=record->reload(&rdvs);

        if (ret == ALLISOK && rdvs != nullptr)
        {
            dt_f->setBounds(offsetDaysAfterToday,offsetDaysBeforeToday);
            dt_f->filter();
            int count = dt_f->countForDisplay();
            disp  = dt_f->getSorted();
            qtw->setRowCount(count);
            display();
        }
    }
}

QString * viewerEditor::centerString(const QString str, int len)
{
    int lenstr = str.size();
    int l = (lenstr >= len) ? (0) : (len - lenstr);
    int add=0;
    if ((l % 2)!= 0)
        add = 1;
    int before = l / 2;
    int after  = (l/2) + add;
    return new QString(QString(before,' ') + str + QString(after, ' '));
}

void viewerEditor::display()
{
    int inc_t=0;
    int n=0;
    qtw->setVisible(false);

    QMultiMap<QDateTime,dt_per_msg>::const_iterator k;
    for(QMultiMap<QDateTime,dt_per_msg>::const_iterator i = disp->constBegin();i!=disp->constEnd();++i)
    {
        const QDateTime &dt = i.key();
        const dt_per_msg *dtp =  &(i.value());
        int offset = dtp->offsetDays;
        int index = offsetToIndex(offset);
        int periode = dtp->period;

        if (n==0 || (k.key().daysTo(i.key())) > 0)
        {
            const ann_ppts *qps = get_annonce_from_index(index);
            QWidget *wdga = new QWidget;
            QHBoxLayout *qla = new QHBoxLayout;
            QString *qlblname = centerString( QString(qps->msg + dt.toString("dddd dd MMMM yyyy")),120);
            QLabel *qlbla = new QLabel(*qlblname);
            qlbla->setStyleSheet(qps->ppt);
            qla->setAlignment(qlbla,Qt::AlignRight);
            qla->addWidget(qlbla);
            qla->setAlignment(Qt::AlignCenter);
            qla->setSpacing(0);
            qla->setMargin(0);
            wdga->setLayout(qla);
            wdga->setStyleSheet(qps->ppt);
            QWidget *wBtn = qtw->cellWidget(inc_t,1);
            QPushButton *push = wBtn->findChild<QPushButton *>("btnDelete");
            QWidget *wCkb = qtw->cellWidget(inc_t,2);
            QCheckBox *ckb = wCkb->findChild<QCheckBox *>("ckb");
            if (ckb != nullptr)
            {
                QLayout  *lCkb = wCkb->layout();
                ckb->setVisible(false);
                lCkb->removeWidget(ckb);
            }
            if (push != nullptr)
            {
                QLayout *lBtn = wBtn->layout();
                disconnect(push,SIGNAL(clicked()),0,0);
                push->setVisible(false);
                lBtn->removeWidget(push);
            }
            qtw->setCellWidget(inc_t++,0,wdga);
        }

        QWidget *wdg1 = new QWidget;
        wdg1->setStyleSheet("background-color: #000000;");
        QHBoxLayout *ql1 = new QHBoxLayout;

        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QString message = dtp->message.left(dtp->message.length() - 1);
        TextEdit *pte_text = new TextEdit(message,this);
        pte_text->setObjectName("content");
        pte_text->setStyleSheet("margin-top: 0px; color: #ffffff; background-color: black; font: bold 12px;");
        pte_text->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        pte_text->setReadOnly(true);
        pte_text->setMyIndex(inc_t);
        pte_text->setDateTime(dtp->dt);

        QVBoxLayout *hbxHour = new QVBoxLayout;


        TimeEdit *qte = new TimeEdit(dt.time(),inc_t,this);
        qte->setObjectName("m_time");
        qte->setStyleSheet("margin-top:0px; color: #66ffff; background-color: black; font: bold 12px;");
        hbxHour->setAlignment(Qt::AlignTop);
        hbxHour->addWidget(qte);
        ql1->addLayout(hbxHour);
        ql1->setAlignment(qte,Qt::AlignTop);
        qte->setMaximumWidth(50);

        if (periode != -1)
        {
            QStringList lst;
            lst << record->getsCper(0) << record->getsCper(1) << record->getsCper(2) << record->getsCper(3);
            Combo *cb_per = new Combo(inc_t, this);
            cb_per->setObjectName("m_periode");
            cb_per->setStyleSheet("color: #00FF00; background-color: black; font: bold 12px;");

            QSignalBlocker blocker(cb_per);
            cb_per->addItems(lst);
            cb_per->setCurrentIndex(periode);
            blocker.unblock();
            hbxHour->addWidget(cb_per);
            hbxHour->setAlignment(cb_per,Qt::AlignTop);
            QLabel *lbl = new QLabel(dtp->dt.toString("dd/MM/yy"));
            lbl->setStyleSheet("color: #00FF00; background-color: black; font: bold 12px;");
            cb_per->setMaximumWidth(75);
            lbl->setMaximumWidth(70);
            hbxHour->addWidget(lbl);

        }
        ql1->addWidget(pte_text);

        ql1->setAlignment(Qt::AlignTop);

        ql1->setAlignment(pte_text,Qt::AlignTop);
        ql1->setAlignment(Qt::AlignLeft);
        ql1->setSpacing(0);
        ql1->setMargin(0);

        wdg1->setLayout(ql1);
        qtw->setCellWidget(inc_t,0,wdg1);
        pte_text->resizeMe();

        QWidget *wdg2 = new QWidget;
        QHBoxLayout *ql2 = new QHBoxLayout;
        QPushButton *btnDelete = new QPushButton(tr("Delete!"));
        btnDelete->setObjectName("btnDelete");
        qsm->setMapping(btnDelete,inc_t);
        connect(btnDelete,SIGNAL(clicked()),qsm,SLOT(map()) );

        btnDelete->setStyleSheet("color: #ffffff; background-color: blue;");

        btnDelete->setMaximumWidth(50);

        ql2->addWidget(btnDelete);
        ql2->setAlignment(Qt::AlignLeft);
        ql2->setSpacing(0);
        ql2->setMargin(0);
        wdg2->setLayout(ql2);

        qtw->setCellWidget(inc_t,1,wdg2);

        QWidget *wdg3 = new QWidget;
        QHBoxLayout *ql3 = new QHBoxLayout;
        QCheckBox *ckb = new QCheckBox;
        ckb->setObjectName("ckb");
        ql3->addWidget(ckb);
        wdg3->setLayout(ql3);
        qtw->setCellWidget(inc_t,2,wdg3);
        inc_t++;
        k = i;
        n++;
    }
    qtw->resizeColumnsToContents();
    qtw->resizeRowsToContents();
    qtw->setVisible(true);
    autoresizeTable();
}
int viewerEditor::changebtnDelete2Cancel(int i,QObject *obj)
{
    QModelIndex qmi;
    QWidget *m_widget = qtw->cellWidget(i,1);
    QPushButton *qpb = m_widget->findChild<QPushButton*>("btnDelete");
    if (qpb != 0)
    {
        qpb->disconnect();
        qpb->setText(tr("Cancel"));
        qpb->setStyleSheet("background-color: #EEBB00;");
        connect(qpb,SIGNAL(clicked()),obj,SLOT(btnCancelClicked()));
        qpb->installEventFilter(this);
        qmi.child(0,0);
        qtw->selectionModel()->select(qmi,QItemSelectionModel::Clear);
        return 0;
    }
    qmi.child(0,0);
    qtw->selectionModel()->select(qmi,QItemSelectionModel::Clear);
    return 1;
}

int viewerEditor::createBtnSaveChg(int n,QObject *obj)
{
    QWidget *m_widget =  qtw->cellWidget(n,1);
    QPushButton *qpb = m_widget->findChild <QPushButton *>("btnSaveChanges");
    TextEdit *te = dynamic_cast<TextEdit *>(obj);
    qtw->setVisible(false);
    if (qpb == 0)
    {
        QLayout *ql = m_widget->layout();
        QPushButton *btn_save = new QPushButton(tr("Save Changes"));
        btn_save->setObjectName("btnSaveChanges");
        connect(btn_save,SIGNAL(clicked()),obj,SLOT(btnSaveClicked()));
        btn_save->setStyleSheet("background-color: #EEBB00;");
        ql->addWidget(btn_save);
        qtw->resizeRowsToContents();
        qtw->resizeColumnsToContents();
    }
    qtw->setVisible(true);
    if (te != 0)
        te->resizeMe();
    autoresizeTable();

    QModelIndex qmi;
    qmi.child(0,0);
    qtw->selectionModel()->select(qmi,QItemSelectionModel::Clear);
    return 0;
}

void viewerEditor::btnCancelClicked(int n,QWidget *which)
{
    QWidget *m_widget = qtw->cellWidget(n,1);
    QWidget *m_wgt    = qtw->cellWidget(n,0);

    TextEdit *te =  m_wgt->findChild<TextEdit *>("content");
    TimeEdit *tme = m_wgt->findChild<TimeEdit *>("m_time");
    Combo    *cb  = m_wgt->findChild<Combo *>("m_periode");
    if (te != which && te != 0)
    {
        QString s =tme->getInitialStyleSheet();
        if (s != "")
        {
            QSignalBlocker blocker(te);
            te->setStyleSheet(te->getInitialStyleSheet());
            te->setPlainText(te->getInitialMessage());
            blocker.unblock();
        }
    }

    if (tme != which && tme != 0)
    {
        QString s =tme->getInitialStyleSheet();
        if (s != "")
        {
            QSignalBlocker blocker(tme);
            tme->setStyleSheet(s);
            tme->setTime(tme->getInitialTime());
            blocker.unblock();
        }
    }

    if (cb != which && cb != 0)
    {
        QString s =cb->getInitialStyleSheet();
        if (s != "")
        {
            QSignalBlocker blocker(cb);
            cb->setStyleSheet(cb->getInitialStyleSheet());
            cb->setCurrentIndex(cb->getInitialSelected());
            blocker.unblock();
        }
    }

    QPushButton *qpb = m_widget->findChild<QPushButton*>("btnDelete");
    if (qpb != 0)
    {
        qpb->setText(tr("Delete!"));
        qpb->setStyleSheet("color: #ffffff; background-color: blue;");
        qpb->disconnect();
        connect(qpb,SIGNAL(clicked()),qsm,SLOT(map()) );
    }

    QLayout *ql = m_widget->layout();
    QPushButton *button_save = m_widget->findChild<QPushButton*>("btnSaveChanges");
    if (button_save != nullptr)
    {
        button_save->setVisible(false);
        button_save->setObjectName("");
        ql->removeWidget(button_save);
    }
    qtw->resizeRowsToContents();
    qtw->resizeColumnsToContents();
    autoresizeTable();

    QModelIndex qmi;
    qmi.child(0,0);
    qtw->selectionModel()->select(qmi,QItemSelectionModel::Clear);
}

void viewerEditor::btnSaveChgClicked(int n)
{
 QWidget *widget = qtw->cellWidget(n,0);
 QWidget *m_widget = qtw->cellWidget(n,1);
 TextEdit *te =  widget->findChild<TextEdit *>("content");
 TimeEdit *tme = widget->findChild<TimeEdit *>("m_time");
 Combo    *cb  = widget->findChild<Combo *>("m_periode");
 if (te == 0)
     return;
 if (tme == 0)
     return;

bool time_changed = false;
 QDateTime qdt( *(te->getDateTime()) );
 if(tme->getInitialTime() != tme->time())
 {
     record->delRecord(*te->getDateTime());
     qdt.setTime(tme->time());
     te->setDateTime(qdt);
     time_changed = true;
 }

QString content;
bool content_modified = false;
if (te->getInitialStyleSheet() != "")
{
    if (te->getInitialMessage() != te->toPlainText())
    {
        content = te->toPlainText() + "\n";
        content_modified = true;;
    }
}
int per;
bool per_changed = false;
if (cb != 0)
{
    per = cb->currentIndex();
    if (cb->getInitialSelected() != per)
        per_changed = true;
}
else
{
    per = -1;

}
 if (time_changed == false)
 {
     if (content_modified == true)
     {
         if (per_changed == true)
             record->changeTxtPerAndSave(qdt,content,per);
         else
             record->changetxtRecordAndSave(qdt,content );
     }
     else if (per_changed == true)
         record->changePerAndSave(qdt,per);
 }
  else
     record->addRecordAndSave(qdt,te->toPlainText() + "\n",per);

 if (te->getInitialStyleSheet() != "" )
     te->setStyleSheet(te->getInitialStyleSheet());
 if (tme->getInitialStyleSheet() != "")
     tme->setStyleSheet(tme->getInitialStyleSheet());
 if (cb != 0 && cb->getInitialStyleSheet() != "")
     cb->setStyleSheet(cb->getInitialStyleSheet());

    QPushButton *qpb = m_widget->findChild<QPushButton*>("btnDelete");
    if (qpb != 0)
    {
        qpb->setText(tr("Delete!"));
        qpb->setStyleSheet("color: #ffffff; background-color: blue;");
        qpb->disconnect();
        connect(qpb,SIGNAL(clicked()),qsm,SLOT(map()) );
    }
    QLayout *ql = m_widget->layout();
    QPushButton *button_save = m_widget->findChild<QPushButton*>("btnSaveChanges");
    if (button_save != nullptr)
    {
        button_save->setVisible(false);
        button_save->setObjectName("");
        ql->removeWidget(button_save);
    }
    refresh();
    qtw->resizeRowsToContents();
    qtw->resizeColumnsToContents();
    autoresizeTable();

    QModelIndex qmi;
    qmi.child(0,0);
    qtw->selectionModel()->select(qmi,QItemSelectionModel::Clear);
}

QTableWidget * viewerEditor::getParentTable()
{
    return qtw;
}

void viewerEditor::btnSuppClicked(int row)
{
    QWidget *wPte = qtw->cellWidget(row,0);
    TextEdit  *te  = wPte->findChild<TextEdit *>("content");
    if (te != nullptr)
    {
        QDateTime *dt = te->getDateTime();
        int reponse = QMessageBox::question(this, tr("Confirm!") , tr("Do you really want to delete rendezvous for ")
                                            + dt->toString("dddd dd MMMM yyyy HH:mm"),
                                            QMessageBox::Yes|QMessageBox::No);
        if (reponse == QMessageBox::Yes)
        {
            record->delRecord(*dt);
            record->save();
            refresh();
        }
    }
}

void viewerEditor::autoresizeTable()
{
    qtw->setVisible(false);

    int width=0;
    for (int i=0;i<qtw->columnCount();i++)
        width += qtw->columnWidth(i);
    width += 10;

    if (width > 800)
        width = 800;
    qtw->setMinimumWidth(width);

    int minHeigt = 0;

    for (int i = 0;i<qtw->rowCount();i++)
        minHeigt += qtw->rowHeight(i);

    minHeigt = (minHeigt > 800) ? 800 : minHeigt;
    minHeigt +=50;
    qtw->setVisible(true);
    qtw->setVisible(false);
    qtw->setMinimumHeight(minHeigt);

    QRect vporig = qtw->viewport()->geometry();
    QRect vpnew = vporig;
    vpnew.setWidth(std::numeric_limits<int>::max());
    qtw->viewport()->setGeometry(vpnew);
    qtw->resizeColumnsToContents();
    qtw->resizeRowsToContents();
    qtw->viewport()->setGeometry((vporig));

    qtw->setVisible(true);
}

const  ann_ppts * viewerEditor::get_annonce_from_index(int index)
{
    if ((index +1) > annonces.count())
        return nullptr;
    return &(annonces[index]);

}


