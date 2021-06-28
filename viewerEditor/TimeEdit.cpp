#include <QDebug>
#include <QSignalBlocker>
#include "TimeEdit.h"
#include "viewerEditor.h"


TimeEdit::TimeEdit()
{
    m_styleSheet = "";
}

TimeEdit::TimeEdit(QTime tm, QWidget *parent):QTimeEdit(tm)
{
    this->parentObj = parent;
    m_styleSheet = "";
}

TimeEdit::TimeEdit(QTime tm, int index, QWidget *parent):QTimeEdit(tm)
{
    m_styleSheet = "";
    this->parentObj = parent;
    this->rowIndex = index;
    init();
}
void TimeEdit::setStyleSheet(const QString &styleSheet)
{
    if (m_styleSheet == "")
        m_styleSheet = styleSheet;
    QTimeEdit::setStyleSheet(styleSheet);
}

void TimeEdit::init()
{

    m_time = this->time();
    connect(this,SIGNAL(editingFinished()),this,SLOT(editingHasChanged()));
}
void TimeEdit::btnCancelClicked()
{
    QSignalBlocker blocker(this);

    this->setTime(m_time);
    this->setStyleSheet(m_styleSheet);
    blocker.unblock();

    viewerEditor *par = dynamic_cast<viewerEditor *>(parentObj);
    par->btnCancelClicked(rowIndex,this);

}
QString TimeEdit::getInitialStyleSheet()
{
    return this->m_styleSheet;
}

QTime TimeEdit::getInitialTime()
{
    return this->m_time;
}

void TimeEdit::editingHasChanged()
{
    viewerEditor *par = dynamic_cast<viewerEditor *>(parentObj);
    if (par != 0)
    {
        setStyleSheet("color: #ffffff; background-color: blue; font: bold 12px;");
        par->changebtnDelete2Cancel(rowIndex,this);
        par->createBtnSaveChg(rowIndex,this);
    }
}
void TimeEdit::btnSaveClicked()
{
    viewerEditor *par = dynamic_cast<viewerEditor *>(parentObj);
    if (par != 0)
    {
        par->btnSaveChgClicked(rowIndex);
    }
}
