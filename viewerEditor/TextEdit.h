#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QDateTime>
#define MINIMUM_PLAINTEXT_WIDTH 10
#define MINIMUM_PLAINTEXT_HEIGHT 20

class viewerEditor;



class TextEdit: public QPlainTextEdit
{
    QString m_styleSheet;
    QString m_message;

    using super = QPlainTextEdit;
    Q_OBJECT

    int getMyIndex();

    int rowIndex;
    int withPreceeding;

    bool firstClickOccured;
    QTableWidget *parentTable;
    viewerEditor *parentObj;

    QDateTime recordDatetime;
    QString initialText;

public slots:


private slots:
    void btnCancelClicked();

    void TextEditClicked();

    void btnSaveClicked();
    void textHasChanged();

public:
    explicit TextEdit (QWidget *parent = nullptr);
    explicit TextEdit(QString str,QWidget *parent = nullptr);
    explicit TextEdit(QString str,viewerEditor *obj, QWidget *parent = nullptr);

    void init();
    void setParentTable(QTableWidget *table);
    void resizeMe();
    const QSize *getSize();
    int getWidth();
    int getHeight();
    int getRatioLineHeight();
    void setMyIndex(int rowIndex);

    void setDateTime(const QDateTime &qdt);
    void setParentObj(viewerEditor *obj);
    void setPlainText(const QString &text);
    void setStyleSheet(const QString &styleSheet);

    QDateTime * getDateTime();

     QString getInitialMessage();
     QString getInitialStyleSheet();
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
        updateGeometry();
        super::resizeEvent(event);
    }
    void mousePressEvent(QMouseEvent* event);

signals:
    void clicked();
};

#endif
