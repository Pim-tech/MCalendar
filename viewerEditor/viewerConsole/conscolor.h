#ifndef CONSCOLOR_H
#define CONSCOLOR_H
#include <QTextStream>
#include <QString>

#define MAX_ANNONCES 20

class conscolor
{
    const int ESC = 27;
    const int CSI =  155;
    const int S0  =  55;
    const int S1  =  56;
    const int S2  =  99;
    const int GRMOD       =      14;
    const int SIMPLE    = 0;
    const int DOUBLE    = 1;
    const int  BLK = 16;
    QTextStream *ts;

public:

    explicit conscolor();
    const char * get_annonce_from_index(int index);
    int get_color_from_index(int index);
    void print (const char *str, int mode, int color);
    void print (const char *str, int color);
    void print (const char *str);
    void printl(const char *str);
    void print (const QString &);
    void print(const QString &s, int mode, int color);
    void print(const QString &s, int color);
    QTextStream * getOutput();
    void cls();
    void gotoxy(int x,int y);
    void mvu(int y);
    void mvd(int y );
    void mvr(int y);
    void mvl(int y);
    void mvdline(int y);
    void mvuline(int y);
    void gotoline(int y);
    char *center(const char *str,int len);
    char *center(const QString str,int len);
    void printcenter(const QString str,int color,int width);
    void printcenter(const char *str,int color,int width);

    void newline();
    QString & centerString(const QString str, int len);


    static const int BLACK=0;
    static const int RED=1;
    static const int GREEN=2;
    static const int BROWN=3;
    static const int BLUE=4;       //couleurs de bases
    static const int MAGENTA=5;
    static const int CYAN=6;
    static const int WHITE=7;

    static const int LBLACK=8;
    static const int LRED=9;
    static const int LGREEN=10;
    static const int LYELLOW=11;
    static const int LBLUE=12;
    static const int LMAGENTA=13;        //couleurs claires
    static const int LCYAN=14;
    static const int LWHITE=15;

    static const int BBLACK=16;        // Les couleurs de fond commencent  a  16
    static const int BRED=32;
    static const int BGREEN=48;
    static const int BYELLOW=64;        //couleurs de fond
    static const int BBLUE=80;
    static const int BMAGENTA=96;
    static const int BCYAN=112;
    static const int BWHITE=128;

    static const int NOCOLOR= 144;

    //All theses are the mode we can print into
    static const int BOLD        =      1;
    static const int LOWERED     =      2;
    static const int ITALIC      =      3;
    static const int UNDERLINE   =      4;
    static const int BLINK       =      5;     //clignote
    static const int INVERSE     =      7;
    static const int INVISIBLE   =      8;
    static const int WRONG       =      9;
    static const int REINIT      =      10;
    static const int NULLC       =      11;    //needded to display chars to build boxes
    static const int NULLCCMETA  =      12;
    static const int NORMAL1     =      21;
    static const int NORMAL2     =      22;
    static const int NOSOUL      =      24;
    static const int NOBLINK     =      25;
    static const int NOINVERSE   =      27;
    static const int _           =      144;     //This is reset mode : ESC[0m or CSI 0m

private:
    char *buffer;
    int use;
    char cl[5]= {(char) ESC,'[','0','m','\0'};
    QTextStream& qStdOut();

    //private methods
    int lenstr(const char *str, int limit);
    char *build_sequence(int mode, int color, int len);
    void setuse(int use);
    char *int2digit(int x);

    const char *annonces[MAX_ANNONCES] = {
        "RENDEZ-VOUS PASSE : ",
        "HIER : ",
        "AUJOURDHUI : ",
        "DEMAIN : ",
        "APRES-DEMAIN : ",
        "DANS TROIS JOURS : ",
        "DANS QUATRES JOURS : ",
        "DANS CINQ JOURS : ",
        "DANS SIX JOURS : ",
        "DANS SEPT JOURS : ",
        "DANS HUIT JOURS : ",
        "DANS NEUF JOURS : ",
        "DANS DIX JOURS : ",
        "DANS ONZE JOURS :",
        "DANS DOUZE JOURS : ",
        "DANS TREIZE JOURS : ",
        "DANS QUATORZE JOURS : ",
        "DANS QUINZE JOURS : ",
        "DANS SEIZE JOURS : ",
        "DANS UN TEMPS PLUS ELOIGNE : "
    };
    const int indexes[MAX_ANNONCES] ={
        LGREEN + BBLUE,
        LYELLOW + BBLUE,
        LYELLOW + BRED,
        LWHITE+BBLUE,
        LWHITE+BGREEN,
        LCYAN+BRED,
        LWHITE+BYELLOW,
        LCYAN+BYELLOW,
        LGREEN + BRED,
        LRED+BBLUE,
        LYELLOW+BYELLOW,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN,
        LWHITE+BCYAN
    };
    const int attr_passe = LCYAN + BWHITE;
    const char *str_passe = "RENDEZ-VOUS PASSE : ";

};

#endif
