#ifndef RENDEZVOUS_H
#define RENDEZVOUS_H

#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QMap>
#include <QChar>


#define ALLISOK        0
#define CANTOPENFILE_R 1
#define CANTOPENFILE_W 2
#define FILE_HAS_NOLINES 4
#define FILE_BAD_FORMAT 8
#define YETRECORDED    16
#define KEY_NOT_EXIST  32

enum period
{
    DIARY,
    WEEKLY,
    MONTLY,
    YEARLY
};



class recorder
{
    void init();
public:
    recorder();
    recorder(QString filename);

    int addRecord(QDateTime &qdt,const QString &content);
    int addRecordAndSave(QDateTime &qdt,const QString &content);
    int addRecord(QDateTime &qdt,const QString &content,int p);
    int addRecordAndSave(QDateTime &qdt,const QString &content,int p);
    int delRecord(QDateTime &qdt);
    int delRecordAndSave(QDateTime &qdt);
    int changetxtRecord(QDateTime &qdt, QString txt);
    int changetxtRecordAndSave(QDateTime &qdt,QString txt);
    int changeTxtPer(QDateTime &qdt,QString txt,int per);
    int changeTxtPerAndSave(QDateTime &qdt,QString txt,int per);
    int changePer(QDateTime &qdt,int per);
    int changePerAndSave(QDateTime &qdt,int per);

    int save();
    int load();
    int load(QMap<QDateTime,QPair<QString,int>> **qmd );
    void unload();
    int reload();
    int reload(QMap<QDateTime,QPair<QString,int>> **qmd);
    QChar getCper(int n);
    QString getsCper(int n);
    void print_console();
    bool isLoaded();
    QMap<QDateTime, QPair<QString, int> > *getData();
    QPair<QString,int> *getRecord(const QDateTime &qdt);


private:
    const QChar cper[4] = {'D','W','M','Y'};
    const QString str_per[4] =
    {
        QObject::tr("Daily"),
        QObject::tr("Weekly"),
        QObject::tr("Montly"),
        QObject::tr("Yearly")
    };
    const QMap<QString, int>  perc = { {"D",DIARY},{"W",WEEKLY},{"M",MONTLY},{"Y",YEARLY}};
    bool loaded;
    QMap<QDateTime,QPair<QString,int>> qmdt;
    QFile *file_rdv_data;
    QRegExp rgxDatTm;
    QRegExp rgxStart;
    QRegExp rgxEnd;
    QString rdvfile;

    QTextStream& qStdOut();


    void setRdvfile(QString &filename);

};

#endif // RENDEZVOUS_H
