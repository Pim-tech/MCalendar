/*
Module : AAMOONPHASES.H
Purpose: Implementation for the algorithms which obtain the dates for the phases of the Moon
Created: PJN / 11-01-2004

Copyright (c) 2004 - 2009 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise)
when your product is released in binary form. You are allowed to modify the source code in any way you want
except you cannot modify the copyright details at the top of each module. If you want to distribute source
code with your application, then you are only allowed to distribute versions released by the author. This is
to maintain a single distribution point for the source code.

*/


/////////////////////// Macros / Defines //////////////////////////////////////


#ifndef __AAMOONPHASES_H__
#define __AAMOONPHASES_H__

#ifndef AAPLUS_EXT_CLASS
#define AAPLUS_EXT_CLASS
#endif

#define FULLMOON 1
#define LASTQUATER 2
#define NEWMOON 3
#define FIRSTQUATER  4

/////////////////////// Classes ///////////////////////////////////////////////


struct  ln_dt
{
    double dday;
    int year;
    int month;
    int day;
    int hour;
    int min;
    int secs;
};

class AAPLUS_EXT_CLASS MoonPhases
{
    ln_dt newmoon;
    ln_dt fullmoon;
    ln_dt firstquater;
    ln_dt lastquater;


    double E;
    double E2;

    double M;
    double Mdash;
    double F;
    double omega;
    double A1;
    double A2;
    double A3;
    double A4;
    double A5;
    double A6;
    double A7;
    double A8;
    double A9;
    double A10;
    double A11;
    double A12;
    double A13;
    double A14;


  double deltaNewMoon(double jd);
  double deltaQuaters(double jd, int moontype);
  double deltaFullMoon(double jd);
  double TruePhase(double k,int moontype);
  void reset();
public:
//Static methods
  MoonPhases();
  MoonPhases(int year,int month,int day = -1);
    static double K(double Year);
    static double MeanPhase(double k);

    void computeMoonPhases(int year, int month, int dateday=-1);

    ln_dt * getnewMoon();
    ln_dt * getfullMoon();
    ln_dt * getfirstQuater();
    ln_dt * getlastQuater();
};

#endif //__AAMOONPHASES_H__
