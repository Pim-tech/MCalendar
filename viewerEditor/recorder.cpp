
#include <QtDebug>
#include <QObject>


#include "recorder.h"


recorder::recorder()
{

}

void recorder::init()
{
    rgxDatTm.setPattern("^_Date_ : (\\d{2})/(\\d{2})/(\\d{4}),(\\d{2}):(\\d{2})(?:,)?([DWMY])?\\s*$");
    rgxStart.setPattern("^\\s*StartComment\\s*$");
    rgxEnd.setPattern("^\\s*EndComment\\s*$");
}

recorder::recorder(QString filename)
{
   rdvfile = filename;
   init();
   loaded = false;

   file_rdv_data = new QFile;
   file_rdv_data->setFileName(rdvfile);
   try
   {
       int ret = load();
       if (ret != ALLISOK)
           throw ret;
   }
   catch (int coderet)
   {
       switch (coderet)
       {
       case CANTOPENFILE_R :
           qFatal(QObject::tr("Exception Error %d, could not open file %s recorder for reading\n").toLatin1().data(),coderet,rdvfile.toLatin1().data());
           break;
       case FILE_BAD_FORMAT :
           qFatal(QObject::tr("Exception Error %d, file %s has not valid format\n").toLatin1().data(),coderet,rdvfile.toLatin1().data()) ;
           break;
       default:
           break;
       }
   }
}


QTextStream& recorder::qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

QMap<QDateTime,QPair<QString,int>> * recorder::getData()
{
    if (isLoaded() == false)
        return nullptr;
    return &qmdt;
}

void recorder::setRdvfile(QString &filename)
{
    rdvfile = filename;
    file_rdv_data = new QFile;
    file_rdv_data->setFileName(filename);
}

int recorder::load()
{

    QDateTime *qfile_dt = nullptr;
    int per = -1;
    QString content="";
    bool canread =false;
    int nlines=0;
    int check_datatype=0;
    int nrecords=0;
    if (file_rdv_data->open(QIODevice::ReadOnly))
    {
        QTextStream inout(&*file_rdv_data);
        while(! inout.atEnd())
        {
            QString line =inout.readLine();

            if  ( rgxDatTm.indexIn(line) > -1)
            {
                check_datatype++;
                int jr = rgxDatTm.cap(1).toInt();
                int ms = rgxDatTm.cap(2).toInt();
                int year = rgxDatTm.cap(3).toInt();
                int hr = rgxDatTm.cap(4).toInt();
                int mn = rgxDatTm.cap(5).toInt();
                QDate dt(year,ms,jr);
                QTime tm(hr,mn);
                qfile_dt = new QDateTime(dt,tm);

                QString s = rgxDatTm.cap(6);
                if (s.isEmpty() == false)
                     per = perc[s];
                else
                    per = -1;
            }
            else if (rgxStart.indexIn(line) > -1)
            {
                check_datatype++;
                canread = true;
            }
            else if (rgxEnd.indexIn(line)> -1)
            {
                check_datatype++;
                if (check_datatype == 3)
                {
                    nrecords ++;
                    check_datatype = 0;
                }
                qmdt[*qfile_dt].first = content;
                qmdt[*qfile_dt].second = per;
                content="";
                canread = false;
            }
            else if (canread == true)
            {
                content += (line + "\n");
            }
            nlines++;
        }

    }
    else
    {
        return CANTOPENFILE_R;
    }
    if (nlines >1 && nrecords == 0 )
        return FILE_BAD_FORMAT;

    file_rdv_data->close();
    loaded = true;

    return ALLISOK;
}

int recorder::load(QMap<QDateTime, QPair<QString, int> > **qmd)
{
    int ret = load();
    *qmd = &qmdt;
    return ret;
}

int recorder::reload()
{
    qmdt.clear();
    return load();
}
int recorder::reload(QMap<QDateTime, QPair<QString, int> > **qmd)
{
    qmdt.clear();
    int ret = load();
    *qmd = &qmdt;
    return ret;
}

void recorder::unload()
{
    qmdt.clear();
}

int recorder::addRecord(QDateTime &qdt, const QString &content)
{
    if (qmdt.contains(qdt) == true)
        return YETRECORDED;    
    qmdt[qdt].first = content;
    qmdt[qdt].second = -1;
    return ALLISOK;
}
int recorder::addRecord(QDateTime &qdt, const QString &content,int p)
{
    if (qmdt.contains(qdt) == true)
        return YETRECORDED;
    qmdt[qdt].first = content;
    qmdt[qdt].second = p;
    return ALLISOK;
}

int recorder::addRecordAndSave(QDateTime &qdt,const QString &content)
{
     int rc = addRecord(qdt,content);

     if (rc != ALLISOK)
         return rc;

    return save();
}
int recorder::addRecordAndSave(QDateTime &qdt,const QString &content,int p)
{
     int rc = addRecord(qdt,content,p);

     if (rc != ALLISOK)
         return rc;

    return save();
}

int recorder::delRecord(QDateTime &qdt)
{
    if (qmdt.contains(qdt) == false)
        return KEY_NOT_EXIST;
    qmdt.remove(qdt);
    return ALLISOK;
}

 QPair<QString,int> * recorder::getRecord(const QDateTime &qdt)
{
    if (qmdt.contains(qdt) == false)
        return nullptr;
    return &qmdt[qdt];
}

int recorder::changetxtRecordAndSave(QDateTime &qdt, QString txt)
{
    if (qmdt.contains(qdt) == false)
        return KEY_NOT_EXIST;
    qmdt[qdt].first = txt;
    return save();
}
int recorder::changetxtRecord(QDateTime &qdt, QString txt)
{
    if (qmdt.contains(qdt) == false)
        return KEY_NOT_EXIST;
    qmdt[qdt].first = txt;
    return ALLISOK;
}
int recorder::changeTxtPer(QDateTime &qdt,QString txt,int per)
{
    if (qmdt.contains(qdt) == false)
        return KEY_NOT_EXIST;
    qmdt[qdt].first = txt;
    qmdt[qdt].second = per;
    return ALLISOK;
}

int recorder::changeTxtPerAndSave(QDateTime &qdt,QString txt,int per)
{
    changeTxtPer(qdt,txt,per);
    return save();
}
int recorder::changePer(QDateTime &qdt,int per)
{
    if (qmdt.contains(qdt) == false)
        return KEY_NOT_EXIST;
    qmdt[qdt].second = per;
    return ALLISOK;
}
int recorder::changePerAndSave(QDateTime &qdt,int per)
{
    changePer(qdt,per);
    return save();
}

int recorder::delRecordAndSave(QDateTime &qdt)
{
    delRecord(qdt);
    return save();
}
QChar recorder::getCper(int n)
{
    return cper[n];
}

QString recorder::getsCper(int n)
{
    return str_per[n];
}

int recorder::save()
{
    if (file_rdv_data->open(QIODevice::WriteOnly))
    {
        QTextStream out(&*file_rdv_data);
        for (QMap<QDateTime,QPair<QString,int> >::const_iterator i = qmdt.constBegin();i != qmdt.constEnd();++i)
        {
            QDateTime qdattime = i.key();
            QPair<QString,int> val = i.value();
            QString content = val.first;
            int per = val.second;
            QString fmtdate = "_Date_ : " + qdattime.toString("dd/MM/yyyy,HH:mm");
            out << fmtdate;
            if (per != -1)
                out << "," + QString(cper[per]);
            out << endl;
            out << "StartComment" <<endl;
            out << content;
            out << "EndComment" << endl << endl;
        }
        file_rdv_data->close();
    }
    else
    {
        return CANTOPENFILE_W;
    }

    return ALLISOK;
}



bool recorder::isLoaded()
{
    return loaded;
}
