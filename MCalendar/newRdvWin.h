#ifndef RDVWINDOW_H
#define RDVWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include <QDate>
#include <QCheckBox>
#include <QTimeEdit>
#include <QTime>
#include <QPoint>
#include <QButtonGroup>
#include <QMessageBox>
#include <QRadioButton>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QMultiMap>
#include <QDebug>

#define MINIMUM_PLAINTEXT_WIDTH 55
#define MINIMUM_PLAINTEXT_HEIGHT 90

class rdvTable;

class PlainTextEdit: public QPlainTextEdit {

    using super = QPlainTextEdit;
    Q_OBJECT

int index;
bool firstClickOccured;
rdvTable *parentTable;

rdvTable * getParentTable();


private slots:
    void selectThisOne();
    void textHasChanged();
    void buttonOkClicked();
    void buttonClearClicked();
    void mTimeChanged(QTime qt);
public:

    explicit PlainTextEdit (QWidget *parent = nullptr): super(parent)
    {

        init();
    }

    explicit PlainTextEdit(QString str,QWidget *parent = nullptr): super(parent)
    {

        appendPlainText(str);
        init();
    }
    int getIndex();
    void setIndex(int i);
    bool firstClicked();
    void setFirstClickOccured(bool b);
    void init();
    void resizeMe();
    void connect_buttons();

    void setParentTable(rdvTable *parentTable);


    virtual QSize sizeHint() const override
    {
        QSize s(document()->size().toSize());
        s.rwidth() = std::max(MINIMUM_PLAINTEXT_WIDTH, s.width());
        s.rheight() = std::max(MINIMUM_PLAINTEXT_HEIGHT, s.height());
        return(s);
    }
protected:

    virtual void resizeEvent (QResizeEvent *event) override
    {
        //  If the widget has been resized then the size hint will
        // also have changed.  Call updateGeometry to make sure
        // any layouts are notified of the change.
        //qDebug() << "This is a resizeEvent size : " << event->size() <<  "oldsize : " << event->oldSize();
        updateGeometry();
        super::resizeEvent(event);
    }
    void mousePressEvent(QMouseEvent* event);

signals:
    void clicked();
};

struct rowCt
{
    QWidget *widPeriod;
    QWidget *widgTime;
    QWidget *widgText;
    QWidget *wigBtnOk;
    QWidget *wigBtnClear;

    QVBoxLayout  *qhbxPeriod;
    QButtonGroup *qbg;
    QRadioButton *qrad0;
    QRadioButton *qrad1;
    QRadioButton *qrad2;
    QRadioButton *qrad3;
    QRadioButton *qrad4;

    QTimeEdit *qte;
    QPushButton *sqrh;
    PlainTextEdit *qpte;

    QVBoxLayout *qhbxTm;
    QHBoxLayout *qhbxtxt;
    QHBoxLayout *qhbxOk;
    QHBoxLayout *qhbxClearButton;

    QPushButton *okButton;
    QPushButton *clearButton;
};

class rdvTable : public QTableWidget
{
     inline void init();
private slots:
    void sqrhButtonClicked();

public:

    QList <rowCt *> rct_list;

    explicit rdvTable();
    explicit rdvTable(const QDate &d,QString &rdvf);
    void setDate(const QDate &d);
    rowCt *addArow(int nRow,const QTime &qtm);
    void autoresize();
    void buttonOkClicked(int b);
    void buttonClearClicked(int b);
    void plainTextChanged(int n);
    void openMeAndCloseOthers(PlainTextEdit *me);
    void resizeOthers(PlainTextEdit *me);
    void resizeOthers(PlainTextEdit *me,int size);
    QDate getSelectedDate();
    void setSelectedDate(QDate &d);

private:
    Q_OBJECT

    rowCt *createWidget(const QTime &qtm, QString &initial_text);
    rowCt *addRowInTable(int nRow, rowCt *c);
    int whichRadButtonClicked();
    QString rdvfile;

    QDate selectedDate;
     QPushButton *sqrh;
    int m_rowCount;
    int m_rowOpened;
    int tmRotate;
    QMultiMap<int,int> qpte_width;

};


#endif // RDVWINDOW_H
