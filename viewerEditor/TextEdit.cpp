#include <QDebug>
#include <QSignalBlocker>
#include <QSize>
#include "TextEdit.h"
#include "viewerEditor.h"


/************* PlainTextEdit methods ***************************/
TextEdit::TextEdit (QWidget *parent ): QPlainTextEdit(parent)
{
    this->m_message = "";
    this->m_styleSheet = "";
}

TextEdit::TextEdit(QString str,QWidget *parent ): QPlainTextEdit(parent)
{
    this->m_message = "";
    this->m_styleSheet = "";
    setPlainText(str);
    init();
}
TextEdit::TextEdit(QString str,viewerEditor *obj, QWidget *parent ): QPlainTextEdit(parent)
{
    setStyleSheet("font: bold 12px");
    this->m_message = "";
    this->m_styleSheet = "";
    setPlainText(str);
    this->parentObj = obj;
    init();
}

void TextEdit::init()
{
    rowIndex = -1;
    document()->setDocumentMargin(0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWordWrapMode(QTextOption::NoWrap);
    setAttribute(Qt::WA_DontShowOnScreen);
    setFixedHeight(10);
    this->parentTable = parentObj->getParentTable();
    connect(this,SIGNAL(clicked()),this,SLOT(TextEditClicked()));
    connect(this,SIGNAL(textChanged()),this,SLOT(textHasChanged()));
}

QDateTime * TextEdit::getDateTime()
{
    return &recordDatetime;
}

void TextEdit::setDateTime(const QDateTime &qdt)
{
    this->recordDatetime = qdt;
}

const QSize * TextEdit::getSize()
{
    QSize *s = new QSize(document()->size().toSize());
    return s;
}

int TextEdit::getWidth()
{
    return getSize()->width();
}
void  TextEdit::setParentObj(viewerEditor *obj)
{
    this->parentObj = obj;
}
int TextEdit::getHeight()
{
    return getSize()->height();
}

int TextEdit::getRatioLineHeight()
{
    QFont font =  document()->defaultFont();
    QFontMetrics fontM = QFontMetrics(font);
    int lines = document()->lineCount();
    int ratio = fontM.height();
    int hauteur = (ratio  * lines );
    return hauteur;
}

void TextEdit::resizeMe()
{
    QTextCursor m_cursor = textCursor();
    setTextCursor(m_cursor);
    parentTable->setVisible(false);
    QFont font =  document()->defaultFont();
    QFontMetrics fontM = QFontMetrics(font);
    int ratio = fontM.height();
    int lines = document()->lineCount();
    int hauteur = (ratio  * lines) + 5; //We add a few for cursor, etc..
    setMinimumHeight(hauteur);
    QSize s(document()->size().toSize());
    setVisible(true);
    int m_width = getWidth() + 10;
    setMinimumWidth(m_width);
    setMaximumWidth(2000);
    withPreceeding = getWidth();
    parentObj->autoresizeTable();
    parentTable->setVisible(true);
    QModelIndex qmi;
    qmi.child(0,0);
    parentTable->selectionModel()->select(qmi,QItemSelectionModel::Clear);
}
int TextEdit::getMyIndex()
{
    return this->rowIndex;
}
void TextEdit::setMyIndex(int index)
{
    this->rowIndex = index;
}


void TextEdit::setParentTable(QTableWidget * table)
{
    this->parentTable = table;
}

/*******************************PlainTextEdit SLOTS  **************************************/

void TextEdit::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouseButton:" << event->button() ;
    emit clicked();
}

void  TextEdit::textHasChanged()
{
    parentObj->createBtnSaveChg(rowIndex,this);
}

void TextEdit::btnSaveClicked()
{
  parentObj->btnSaveChgClicked(rowIndex);
}

QString TextEdit::getInitialMessage()
{
    return this->m_message;
}

QString TextEdit::getInitialStyleSheet()
{
  return this->m_styleSheet;
}
void TextEdit::setPlainText(const QString &text)
{
    if (m_message == "")
        m_message = (QString) text;
     QPlainTextEdit::setPlainText(text);
}

void TextEdit::setStyleSheet(const QString &styleSheet)
{
    if (m_styleSheet == "")
        m_styleSheet = styleSheet;
    QPlainTextEdit::setStyleSheet(styleSheet);
}

void  TextEdit::TextEditClicked()
{
    setReadOnly(false);
    setStyleSheet("margin-top: 0px; font: bold 12px;color: #FFFFFF; background-color: blue;");
    parentObj->changebtnDelete2Cancel(rowIndex,this);
}
void TextEdit::btnCancelClicked()
{
    setStyleSheet(m_styleSheet);
    QSignalBlocker blocker(this);
    setPlainText(m_message);
    setReadOnly(true);
    blocker.unblock();
    parentObj->btnCancelClicked(rowIndex,this);
}
