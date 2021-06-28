#ifndef COMBO_H
#define COMBO_H
#include <QComboBox>

class Combo: public QComboBox
{
    QWidget *parentObj;
    int rowIndex;
    int m_selectedIndex=-2;
    QString m_styleSheet="";
    Q_OBJECT
    void init();
public:
    explicit Combo();
    explicit Combo(QWidget *parent = nullptr);
    explicit Combo(int index,QWidget *parent = nullptr);
    QString getInitialStyleSheet();
    int getInitialSelected();
    void setCurrentIndex(int index);
    void setStyleSheet(const QString &styleSheet);
private slots:
    void currentIndexHasChanged(int n);
    void btnCancelClicked();
    void btnSaveClicked();
};

#endif // COMBO_H
