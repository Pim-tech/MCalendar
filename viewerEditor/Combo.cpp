#include <QDebug>
#include <QSignalBlocker>

#include "Combo.h"
#include "viewerEditor.h"

Combo::Combo()
{

}
Combo::Combo(QWidget *parent):QComboBox(parent)
{
    this->parentObj = parent;
}

Combo::Combo(int index, QWidget *parent):QComboBox(parent)
{
    this->parentObj = parent;
    this->rowIndex = index;
    init();
}
void Combo::init()
{
    connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexHasChanged(int)));
}

void Combo::btnCancelClicked()
{
    QSignalBlocker blocker(this);
    setCurrentIndex(m_selectedIndex);
    setStyleSheet(m_styleSheet);
    blocker.unblock();

    viewerEditor *par = dynamic_cast<viewerEditor *>(parentObj);
    par->btnCancelClicked(rowIndex,this);

}
void Combo::setCurrentIndex(int index)
{
    if (m_selectedIndex == -2)
        m_selectedIndex = index;
    QComboBox::setCurrentIndex(index);
}
 void Combo::setStyleSheet(const QString &styleSheet)
 {
     if (m_styleSheet == "")
         m_styleSheet = styleSheet;
     QComboBox::setStyleSheet(styleSheet);
 }

void Combo::currentIndexHasChanged(int n)
{

    viewerEditor *par = dynamic_cast<viewerEditor *>(parentObj);
    if (par != 0)
    {
        setStyleSheet("color: #ffffff; background-color: blue; font: bold 12px;");
        par->changebtnDelete2Cancel(rowIndex,this);
        par->createBtnSaveChg(rowIndex,this);
    }

}
QString Combo::getInitialStyleSheet()
{
    return this->m_styleSheet;
}

int Combo::getInitialSelected()
{
    return this->m_selectedIndex;
}
void Combo::btnSaveClicked()
{
    viewerEditor *par = dynamic_cast<viewerEditor *>(parentObj);
    if (par != 0)
    {
        par->btnSaveChgClicked(rowIndex);
    }
}
