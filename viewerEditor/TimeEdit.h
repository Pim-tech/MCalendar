#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QTimeEdit>



class TimeEdit : public QTimeEdit
{
    Q_OBJECT

    QWidget *parentObj;
    int rowIndex;
    QString m_styleSheet;
    QTime   m_time;


    void init();

public:
    explicit TimeEdit ();
    explicit TimeEdit(QTime tm,QWidget *parent = nullptr);
    explicit TimeEdit(QTime tm, int index, QWidget *parent = nullptr);
    QString getInitialStyleSheet();
    QTime   getInitialTime();
    void setStyleSheet(const QString &styleSheet);


private slots:
   void editingHasChanged();
   void btnCancelClicked();
   void btnSaveClicked();
public slots:
};

#endif // TIMEEDIT_H
