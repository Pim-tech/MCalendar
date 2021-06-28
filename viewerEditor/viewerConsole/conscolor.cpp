#include "conscolor.h"

conscolor::conscolor()
{
    ts = new QTextStream(stdout);
    buffer = new char[BLK];
    buffer[0]='\0';
    setuse(ESC);
}

void conscolor::setuse(int use)
{
    this->use = use;
}

QTextStream& conscolor::qStdOut()
{
    return *ts;
}
QTextStream * conscolor::getOutput()
{
    return ts;
}

char *conscolor::build_sequence(int mode,int color,int len)
{
    if (len < 5)
    {
        qStdOut() << "Minimal len must be >= 5 to build a minimal sequence" << endl;
        return nullptr;
    }
    int i=0;
    if (use == ESC)
    {
        buffer[i++]=ESC;
        buffer[i++]= '[';
    }
    else if (use == CSI)
        buffer[i++] = CSI;
    if ( mode == _ )
    {
        buffer[i++] = '0';
        buffer[i++] = 'm';
    }
    else if ( mode>0)
    {
        if (mode < 10)
            buffer[i++] = (char) (mode + '0');
        else if (mode <= 27)
        {
            int dizaine = (mode/10);
            int reste = (mode%10);
            buffer[i++] = (char) (dizaine + '0');
            buffer[i++] = (char) (reste + '0');
        }
        if ( color == NOCOLOR )
        {
            buffer[i++] = 'm';
            buffer[i++] = '\0';
            return buffer;
        }
        else buffer[i++] = ';';
    }
    //Si la valeur est > 16 c est qu un fond est invoque
    int light;
    if ( (light = ( color % 16) ) > 7 )
    {
        light-=8;      //On remene le fond a sa vraie valeur
        buffer[i++]='1';
        buffer[i++]=';'; // On invoque le fond dans la sequence ansi
    }
    buffer[i++]='3';
    buffer[i++]= (char) (light + '0');
    int atr;
    if ( (atr = ((color - (color%16)) / 16 ))>0 ){
        atr--;
        buffer[i++]=';';
        buffer[i++]='4';
        buffer[i++]= (char) (atr+ '0');
    }
    buffer[i++]='m';
    buffer[i++]='\0';
    return buffer;
}

void conscolor::print(const char *str, int mode, int color)
{
    qStdOut() << build_sequence(mode,color,BLK) << str << cl;
    qStdOut().flush();
}

void conscolor::print(const char *str, int color)
{
    qStdOut() << build_sequence(0,color,BLK) << str << cl;
    qStdOut().flush();
}

void conscolor::print(const QString &s, int mode, int color)
{
    qStdOut() << build_sequence(mode,color,BLK) << s << cl;
    qStdOut().flush();
}
void conscolor::print(const QString &s, int color)
{
    qStdOut() << build_sequence(0,color,BLK) << s << cl;
    qStdOut().flush();
}
void conscolor::print(const QString &s)
{
    qStdOut() << s;
    qStdOut().flush();
}
void conscolor::print(const char *s)
{

    qStdOut() << s ;
    qStdOut().flush();
}
void conscolor::printl(const char *str)
{
    qStdOut() << str << endl ;
    qStdOut().flush();
}


char * conscolor::int2digit(int x)
{
    char *str;
    int i = 0;
    str = new char[16];

    int dizaines,centaines;

    if(x>99)
    {
        centaines = x/100;
        str[i++]= (char) (centaines + '0');
        x -= (100 * centaines);
    }
    if (x>9)
    {
        dizaines = x/10;
        str[i++] = (char) (dizaines + '0');
        x -= (10 * dizaines);
    }
    str[i++]=(char) (x + '0');
    str[i] = '\0';

    return str;
}

void conscolor::cls()
{
    qStdOut() << char(ESC) << "[2J" ;
    qStdOut().flush();
}

void conscolor::gotoxy(int x,int y)
{
    qStdOut() << (char) ESC <<  '[' <<  int2digit(y) << ';' << int2digit(x) << 'H';
}

void conscolor::mvu(int y)
{
    qStdOut() << (char) ESC << '[' << int2digit(y) << 'A';
}
void conscolor::mvd(int y )
{
    qStdOut() <<(char) ESC << '[' << int2digit(y) << 'B';
}
void conscolor::mvr(int y)
{
    qStdOut() << (char) ESC << '[' << int2digit(y) << 'C';
}
void conscolor::mvl(int y)
{
    qStdOut() << (char) ESC << '[' << int2digit(y) << 'D';
}
void conscolor::mvdline(int y)
{
    qStdOut() << (char) ESC << '[' << int2digit(y) << 'E';
}
void conscolor::mvuline(int y)
{
    qStdOut() << (char) ESC << '[' << int2digit(y) << 'F';
}
void conscolor::gotoline(int y)
{
    qStdOut() << (char) ESC << '[' << int2digit(y) << 'G';
}
int conscolor::lenstr(const char *str,int limit)
{
    int len = 0;

    for(len=0;str[len]!='\0' ;len++)
        if ((len + 1 ) >= limit && str[len+1] != '\0')
            return -1;

    return len;
}

char * conscolor::center(const char *str, int len)
{
    int str_len = lenstr(str,2048);
    int l = len - str_len;
    bool add;
    int i;

    if ((l%2) != 0)
        add = true;

    l /= 2;

    char *output = new char[((l+2) * 2) + str_len];
    for (i=0;i<l;i++)
        output[i]=' ';
    if (add == true)
        output[i++]= ' ';

    for (int k=0;k<str_len;k++,i++)
        output[i]=str[k];

    for (int k=0;k<l;k++,i++)
        output[i]=' ';
    if (add == true)
        output[i++]= ' ';

    output[i]='\0';

    return output;

}

char * conscolor::center(const QString str, int len)
{
    QByteArray ba;
    ba = str.toLatin1();
    return center(ba.data(),len);
}

QString & conscolor::centerString(const QString str, int len)
{
   int lenstr = str.size();
   int l = (lenstr >= len) ? (0) : (len - lenstr);

   int add=0;

   if ((l % 2)!= 0)
        add = 1;
   
   int before = l / 2;
   int after  = (l/2) + add;

  static QString output(QString(before,' ') + str + QString(after, ' '));

  return output;

}

void conscolor::printcenter(const char *str, int color, int width)
{
    print(center(str,width),color);
}

void conscolor::newline()
{
    qStdOut() << endl;
}

void conscolor::printcenter(const QString str, int color, int width)
{
    print(center(str,width),color);
}

const char *conscolor::get_annonce_from_index(int index)
{
    if ((index+1) > MAX_ANNONCES)
        return nullptr;
    return annonces[index];

}

int conscolor::get_color_from_index(int index)
{
    if ((index+1) > MAX_ANNONCES)
        return -1;
    return indexes[index];
}
