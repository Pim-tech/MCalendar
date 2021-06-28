
#include <QHeaderView>
#include <QScrollBar>
#include <QSignalBlocker>
#include <limits>
#include "../viewerEditor/recorder.h"
#include "newRdvWin.h"


void PlainTextEdit::init()
{
    document()->setDocumentMargin(0);

    index=0;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWordWrapMode(QTextOption::NoWrap);
    setAttribute(Qt::WA_DontShowOnScreen);
    setFixedHeight(20);
    connect(this,SIGNAL(clicked()),this,SLOT(selectThisOne()));
    connect(this ,SIGNAL(textChanged()),this,SLOT(textHasChanged()));
    firstClickOccured = false;

}
void PlainTextEdit::connect_buttons()
{
    rowCt *r = getParentTable()->rct_list[index];
    connect(r->okButton,SIGNAL(clicked()),this,SLOT(buttonOkClicked()));
    connect(r->clearButton,SIGNAL(clicked()),this,SLOT(buttonClearClicked()));
    connect(r->qte,SIGNAL(timeChanged(QTime)),this,SLOT(mTimeChanged(QTime)));
}

void PlainTextEdit::textHasChanged()
{
    getParentTable()->plainTextChanged(this->index);
}

void PlainTextEdit::buttonOkClicked()
{
    getParentTable()->buttonOkClicked(this->index);
}
void PlainTextEdit::buttonClearClicked()
{
    getParentTable()->buttonClearClicked(this->index);
}
void PlainTextEdit::mTimeChanged(QTime qt)
{
  qDebug() << "Time is changed to: " << qt.toString("HH:mm");
}

void PlainTextEdit::setIndex(int i)
{
    this->index = i;
}
int PlainTextEdit::getIndex()
{
    return this->index;
}
bool PlainTextEdit::firstClicked()
{
    return firstClickOccured;
}
void PlainTextEdit::setFirstClickOccured(bool b)
{
    this->firstClickOccured = b;
}

void PlainTextEdit::selectThisOne()
{
    getParentTable()->openMeAndCloseOthers(this);
}
rdvTable *PlainTextEdit::getParentTable()
{
    return parentTable;
}
void PlainTextEdit::setParentTable(rdvTable *parentTable)
{
    this->parentTable = parentTable;
}

void PlainTextEdit::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouseButton:" << event->button() ;
    emit clicked();
}


void PlainTextEdit::resizeMe()
{
    QFont font =  document()->defaultFont();
    QFontMetrics fontM = QFontMetrics(font);
    int ratio = fontM.height();
    int lines = document()->lineCount();
    int hauteur = (ratio  * lines ) + 5;//we add a few for the cursor space, etc...
    setMinimumHeight(hauteur);
    parentTable->resizeOthers(this);
}

rdvTable::rdvTable()
{
  init();
}
inline void rdvTable::init()
{
    setColumnCount(5);
    setRowCount(24);
    verticalHeader()->hide();
   // horizontalHeader()->hide();
    QStringList qtextFlds;
    qtextFlds << tr(" Periodicity ") << tr(" Time ") << tr(" Message to remind ") << tr(" Confirm ") << tr(" Cancel ");
    setHorizontalHeaderLabels(qtextFlds);
    setStyleSheet("QHeaderView::section{Background-color:rgb(220,150,85);font: 14px bold}");
    setAutoScroll(false);
    setVisible(false);
    m_rowOpened = -1;
    tmRotate = 0;
    sqrh = new QPushButton("15/30/45/00");
    sqrh->setMinimumHeight(30);
    connect(sqrh,SIGNAL(clicked()),this,SLOT(sqrhButtonClicked()));
}

rdvTable::rdvTable(const QDate &d, QString &rdvf)
{
   this->rdvfile = rdvf;
   this->selectedDate = d;
   init();
}


void rdvTable::setDate(const QDate &d)
{
  this->selectedDate = d;
}
QDate rdvTable::getSelectedDate()
{
    return this->selectedDate;
}
void rdvTable::setSelectedDate(QDate &d)
{
    this->selectedDate = d;
}

void rdvTable::sqrhButtonClicked()
{

    tmRotate += 15;
    if (tmRotate == 60)
        tmRotate = 0;

    QTimeEdit *qte = rct_list[m_rowOpened]->qte;
    int time = qte->time().hour();
    qte->setTime(QTime(time,tmRotate));

}

rowCt *rdvTable::createWidget(const QTime &qtm, QString &initial_text)
{
    rowCt *ct = new rowCt;

    ct->widPeriod = new QWidget;
    ct->qhbxPeriod = new QVBoxLayout;
    ct->qhbxPeriod->setSpacing(0);
    ct->qhbxPeriod->setMargin(0);

    ct->qrad0 = new QRadioButton(tr("None"));
    ct->qrad1 = new QRadioButton(tr("Daily"));
    ct->qrad2 = new QRadioButton(tr("Weekly"));
    ct->qrad3 = new QRadioButton(tr("Monthly"));
    ct->qrad4 = new QRadioButton(tr("Yearly"));
    ct->qrad1->setObjectName("radio1");
    ct->qhbxPeriod->addWidget(ct->qrad0);

    ct->widPeriod->setLayout(ct->qhbxPeriod);

    ct->qbg = new QButtonGroup;




    ct->widgTime = new QWidget;

    ct->qhbxTm = new QVBoxLayout;
    ct->qhbxTm->setSpacing(0);
    ct->qhbxTm->setMargin(0);
    ct->qte = new QTimeEdit(qtm);

    ct->qhbxTm->setAlignment(Qt::AlignCenter);
    ct->qhbxTm->setAlignment(ct->qte,Qt::AlignCenter);

    ct->qte->setMinimumTime(qtm);
    ct->qte->setMaximumTime(QTime(qtm.hour(),59));
    ct->qte->setMinimumWidth(80);
    ct->qte->setAlignment(Qt::AlignCenter);

    ct->qhbxTm->addWidget(ct->qte);
    ct->widgTime->setLayout(ct->qhbxTm);

    ct->widgText = new QWidget;

    ct->qhbxtxt = new QHBoxLayout;
    ct->qhbxtxt->setAlignment(Qt::AlignLeft);
    ct->qhbxtxt->setSpacing(0);
    ct->qhbxtxt->setMargin(0);

    ct->widgText->setLayout(ct->qhbxtxt);
    ct->qpte = new PlainTextEdit(initial_text);
    ct->qpte->setParentTable(this);
    ct->qhbxtxt->addWidget(ct->qpte);
    ct->qhbxtxt->setAlignment(ct->qpte,Qt::AlignLeft);

    ct->wigBtnOk = new QWidget;
    ct->wigBtnClear = new QWidget;
    ct->qhbxOk = new QHBoxLayout;
    ct->qhbxOk->setAlignment(Qt::AlignLeft);
    ct->qhbxClearButton = new QHBoxLayout;
    ct->qhbxClearButton->setMargin(0);
    ct->qhbxClearButton->setSpacing(0);
    ct->qhbxClearButton->setAlignment(Qt::AlignLeft);

    ct->okButton=new QPushButton(tr("Ok"));
    ct->clearButton=new QPushButton(tr("Clear/Cancel"));
    ct->qhbxOk->setSpacing(0);
    ct->qhbxOk->setMargin(0);
    ct->qhbxOk->addWidget(ct->okButton);

    ct->qhbxClearButton->addWidget(ct->clearButton);

    ct->wigBtnOk->setLayout(ct->qhbxOk);
    ct->wigBtnClear->setLayout(ct->qhbxClearButton);

    return ct;
}
void rdvTable::resizeOthers(PlainTextEdit *me)
{
    setVisible(false);
    QSize s(me->document()->size().toSize());
    s.rwidth() = std::max(MINIMUM_PLAINTEXT_WIDTH, s.width());

    int k_width = s.width() + 5; //We add a few because of the cursor size and spaces.
    int m_width =0;
    int index = me->getIndex();

    if (qpte_width.isEmpty() == false)
    {
        int largest_width = qpte_width.lastKey();
        int largest_index = qpte_width.last();
        if (largest_index == index) //veut dire : nous sommes l'un des plus grands
        {
            if (k_width == largest_width) // veux dire qu'on ne change pas : soit on était plus grand à la mesure soit on était plus petit car il faut un évènement
            {
                m_width = k_width;             // initile de changer quoi que ce soit, juste fixer m_width
            }
            else if (k_width > largest_width) // veut dire:  on a encore grossit
            {
                qpte_width.remove(largest_width,index); // on remplace
                qpte_width.insert(k_width,index);       // on remplace
                m_width = k_width;                      // Et on impose la nouvelle valeure.
            }

            else if (k_width < largest_width) // on décroît
            {
                qpte_width.remove(largest_width,index); // Oui : marche car on précise l'index.
                if (k_width >= qpte_width.lastKey() ) // on est encore le maitre
                    m_width = k_width;
                else                                  // On est plus le maitre car trop petit.
                    m_width = qpte_width.lastKey();
                qpte_width.insert(k_width,index);     // mais on doit se re-enregistrer
            }
        }
        else
        {
            if (k_width < largest_width)
                m_width = largest_width;
            else
                m_width = k_width;
        }
    }
    else
    {
        m_width = k_width;
    }

    for (int i=0;i<rct_list.count();i++)
        rct_list[i]->qpte->setMinimumWidth(m_width);

    autoresize();
    setVisible(true);
}


void rdvTable::openMeAndCloseOthers(PlainTextEdit *me)
{

    rowCt *ct = rct_list[me->getIndex()];
    QRadioButton *radio1 = ct->widPeriod->findChild<QRadioButton *>("radio1");
    if (radio1 == 0)
    {
        ct->qrad1->setObjectName("radio1");
        ct->qhbxPeriod->addWidget(ct->qrad1);
        ct->qhbxPeriod->addWidget(ct->qrad2);
        ct->qhbxPeriod->addWidget(ct->qrad3);
        ct->qhbxPeriod->addWidget(ct->qrad4);

        QList<QRadioButton *> allRadioButtons = ct->widPeriod->findChildren<QRadioButton *>();
        for(int i = 0; i < allRadioButtons.size(); ++i)
        {
            ct->qbg->addButton(allRadioButtons[i],i);
        }
    }

    QFont font =  me->document()->defaultFont();
    QFontMetrics fontM = QFontMetrics(font);
    int ratio = fontM.height();
    int lines = me->document()->lineCount();
    int hauteur = ((ratio + 1) * lines );


    setVisible(false);
    me->setMaximumHeight(1000);
    me->setMinimumHeight(hauteur);
    int index = me->getIndex();
    m_rowOpened = index;
    QVBoxLayout *qhbx = rct_list[index]->qhbxTm;
    qhbx->addWidget(sqrh);
    qhbx->setAlignment(sqrh,Qt::AlignHCenter);
    qpte_width.clear();
    for (int i=0;i<rct_list.count();i++)
    {
        QSize s(rct_list[i]->qpte->document()->size().toSize());
        int width = std::max(MINIMUM_PLAINTEXT_WIDTH, s.width());
        //qpte_width[width] = 1;
        qpte_width.insert(width,i);
        if (rct_list[i]->qpte == me)
            continue;
        rct_list[i]->qpte->setFixedHeight(20);
    }
    if (me->firstClicked() == false)
    {
      me->clear();
      me->setFirstClickOccured(true);
    }
    autoresize();
    setVisible(true);
}

rowCt *rdvTable::addRowInTable(int nRow, rowCt *c)
{
    QString color1,color2;
    color1 = ((nRow%2) == 0) ? ("#000066") : ("#0000AA");
    color2 =  ((nRow%2) == 0) ? ("#660000") : ("#AA0000");
    QString color;
    if (nRow == 12 || nRow == 13 || nRow < 7 || nRow > 18)
        color = color1;
    else
        color = color2;

    c->widgText->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px;");
    c->widgTime->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px;");

    c->okButton->setMaximumWidth(30);
    c->okButton->setMaximumHeight(20);

    c->clearButton->setMinimumWidth(90);
    c->clearButton->setMaximumHeight(20);

    c->okButton->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px");
    c->clearButton->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px");

    c->wigBtnOk->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px");
    c->wigBtnClear->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px");

    c->widPeriod->setStyleSheet("color: #FFFFFF; background-color: " + color + "; font: bold 12px");

    setCellWidget(nRow,0,c->widPeriod);
    setCellWidget(nRow,1,c->widgTime);
    setCellWidget(nRow,2,c->widgText);
    setCellWidget(nRow,3,c->wigBtnOk);
    setCellWidget(nRow,4,c->wigBtnClear);

    return c;
}

rowCt *rdvTable::addArow(int nRow, const QTime &qtm)
{
    QString initial_text(tr("Click for new rdv"));
    rowCt *rct = createWidget(qtm,initial_text);
    addRowInTable(nRow,rct);
    rct->qpte->setIndex(nRow);
    rct_list << rct;
    rct->qpte->connect_buttons();
    rct->qpte->resizeMe();
    return rct;
}
void rdvTable::autoresize()
{
    QRect vporig = viewport()->geometry();
    QRect vpnew = vporig;
    vpnew.setWidth(std::numeric_limits<int>::max());
    viewport()->setGeometry(vpnew);
    resizeColumnsToContents();
    resizeRowsToContents();
    viewport()->setGeometry((vporig));

    int width=0;
    for (int i=0;i<columnCount();i++)
        width += columnWidth(i);
    width += 10;


    if (width > 800)
        width = 800;
    setMinimumWidth(width);

    int minHeigt = 0;

    for (int i = 0;i<rowCount();i++)
        minHeigt += rowHeight(i);

    minHeigt = (minHeigt > 800) ? 800 : minHeigt;
    minHeigt +=20;
    setMinimumHeight(minHeigt);


}
int rdvTable::whichRadButtonClicked()
{
    return 0;
}

void rdvTable::buttonOkClicked(int b)
{
    int selected_radio = rct_list[b]->qbg->checkedId();

    const QString &rdvtxt = rct_list[b]->qpte->toPlainText();
    if (rdvtxt.length()== 0)
    {
        QMessageBox::information(this,tr("Error") +"!",tr("Your message is void You must at least have one character")+".");
        return;
    }
    recorder *rec = new recorder(rdvfile);

    if (rec->isLoaded() == false)
    {
        rec->load();
    }
    QTime d(rct_list[b]->qte->time());
    QDateTime qdt(selectedDate,d);

    int coderet;
    if (selected_radio == -1 ||  selected_radio == 0)
         coderet = rec->addRecordAndSave(qdt, rdvtxt + "\n");
    else
    {
        selected_radio --;
        coderet = rec->addRecordAndSave(qdt,rdvtxt + "\n",selected_radio);
    }

    QString datestr = qdt.toString("dddd d MMMM yyyy HH:mm");
    switch (coderet)
     {
         case ALLISOK:
                QMessageBox::information(this,"Information",tr("Message recorded for ") + datestr + ".");
                break;

         case CANTOPENFILE_W :
                 QMessageBox::warning(this,tr("Error!"),tr("Could not open file recorder for writing") + ".");
                 break;

         case YETRECORDED:
                 QMessageBox::warning(this,tr("Error!"),tr("A record rendezvous for ") + datestr + tr(" is already into database\nYou must delete it before"));

         break;
     }
    rec->unload();
    delete rec;
}

void rdvTable::buttonClearClicked(int b)
{
    rowCt *ct = rct_list[b];
    ct->qhbxTm->removeWidget(sqrh);
    ct->qpte->setFixedHeight(20);
    QSignalBlocker blocker(ct->qpte);
    ct->qpte->clear();
    ct->qpte->setPlainText(tr("Click for new rdv"));
    blocker.unblock();

    QList<QRadioButton *> allRadioButtons = ct->widPeriod->findChildren<QRadioButton *>();
    for(int i = 0; i < allRadioButtons.size(); ++i)
    {
        if (i>0)
        {
            if (i == 1)
                allRadioButtons[i]->setObjectName("");
         allRadioButtons[i]->setVisible(false);
         ct->qbg->removeButton(allRadioButtons[i]);
        }
        ct->qhbxPeriod->removeWidget(allRadioButtons[i]);
    }
    ct->qpte->setFirstClickOccured(false);
    autoresize();
}

void rdvTable::plainTextChanged(int n)
{
    qDebug() << "TextEdit :" << n << " changed.";
    setVisible(false);
    rct_list[n]->qpte->resizeMe();
    autoresize();
    setVisible(true);
    QModelIndex qmi;
    qmi.child(0,0);
    selectionModel()->select(qmi,QItemSelectionModel::Clear);
}

