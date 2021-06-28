/*
Module : AAMOONPHASES.CPP
Purpose: Implementation for the algorithms which obtain the dates for the phases of the Moon
Created: PJN / 11-01-2004
History: PJN / 22-02-2004 1. Fixed a bug in the calculation of the phase type from the k value in
                          CAAMoonPhases::TruePhase.

Copyright (c) 2004 - 2009 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise)
when your product is released in binary form. You are allowed to modify the source code in any way you want
except you cannot modify the copyright details at the top of each module. If you want to distribute source
code with your application, then you are only allowed to distribute versions released by the author. This is
to maintain a single distribution point for the source code.

*/


//////////////////////////// Includes /////////////////////////////////////////

#include "MoonPhases.h"
#include "CoordinateTransformation.h"
#include <math.h>
#include <stdio.h>
#include <QDebug>
#include <QDate>
#include <iomanip>


MoonPhases::MoonPhases()
{
    this->reset();
}
MoonPhases::MoonPhases(int year, int month, int day)
{
    computeMoonPhases(year,month,day);
}

void MoonPhases::computeMoonPhases(int year, int month,int dateday)
{
    this->reset();
    QDate qd(year,1,1);
    QDate qd2(year,month,1);
    int mDay;
    if (dateday == -1)
        mDay = qd2.daysInMonth();
    else
        mDay = dateday;

    qd2 = QDate(year,month,mDay);
    int dayOfYear = qd.daysTo(qd2);
    double daysInYear = qd.daysInYear();
    float ratio = dayOfYear /  daysInYear;

    ratio = trunc(ratio * 1000) / 1000;
    double yearRatio  = year+ratio;
    double k = K(yearRatio);

    TruePhase(k,NEWMOON);
    double k2 = k;
    if ((newmoon.dday + 7.38264725*2) > mDay )
        k2 +=  (+0.5 - 1);
    else
        k2 += 0.5;
     TruePhase(k2,FULLMOON);
    double k3 = k;
    if ((newmoon.dday + 7.38264725) > mDay)
        k3 += (0.25 - 1);
    else
        k3 += 0.25;
    TruePhase(k3,FIRSTQUATER);
    double k4 = k;
    if ((newmoon.dday + 7.38264725 * 3) > mDay )
           k4  += (0.75 - 1);
       else
           k4  += 0.75;
    TruePhase(k4,LASTQUATER);
}

void MoonPhases::reset()
{
    newmoon.dday = -1;
    newmoon.year = -1;
    newmoon.month = -1;
    newmoon.day = -1;
    newmoon.hour = -1;
    newmoon.min = -1;
    newmoon.secs = -1;

    fullmoon.dday = -1;
    fullmoon.year = -1;
    fullmoon.month = -1;
    fullmoon.day = -1;
    fullmoon.hour = -1;
    fullmoon.min = -1;
    fullmoon.secs = -1;

    firstquater.dday = -1;
    firstquater.year = -1;
    firstquater.month = -1;
    firstquater.day = -1;
    firstquater.hour = -1;
    firstquater.min = -1;
    firstquater.secs = -1;

    lastquater.dday = -1;
    lastquater.year = -1;
    lastquater.month = -1;
    lastquater.day = -1;
    lastquater.hour = -1;
    lastquater.min = -1;
    lastquater.secs = -1;

    E = 0;
    E2 =0;
    M =0;
    Mdash = 0;
    F = 0;
    omega = 0;
    A1 = 0;
    A2 = 0;
    A3 = 0;
    A4 = 0;
    A5 = 0;
    A6 = 0;
    A7 = 0;
    A8 = 0;
    A9 = 0;
    A10 = 0;
    A11 = 0;
    A12 = 0;
    A13 = 0;
    A14 = 0;
}
double MoonPhases::TruePhase(double k,int moontype)
{
    //What will be the return value
    double JD = MeanPhase(k);


    //convert from K to T
    double T = k/1236.85;
    double T2 = T*T;
    double T3 = T2*T;
    double T4 = T3*T;

    E = 1 - 0.002516*T - 0.0000074*T2;

    E2 = E*E;

    M = CoordinateTransformation::MapTo0To360Range(2.5534 + 29.10535670*k - 0.0000014*T2 - 0.00000011*T3);
    M = CoordinateTransformation::DegreesToRadians(M);
    Mdash = CoordinateTransformation::MapTo0To360Range(201.5643 + 385.81693528*k + 0.0107582*T2 + 0.00001238*T3 - 0.000000058*T4);
    Mdash = CoordinateTransformation::DegreesToRadians(Mdash);
    F = CoordinateTransformation::MapTo0To360Range(160.7108 + 390.67050284*k - 0.0016118*T2 - 0.00000227*T3 + 0.00000001*T4);
    F = CoordinateTransformation::DegreesToRadians(F);
    omega = CoordinateTransformation::MapTo0To360Range(124.7746 - 1.56375588*k + 0.0020672*T2 + 0.00000215*T3);
    omega = CoordinateTransformation::DegreesToRadians(omega);
    A1 = CoordinateTransformation::MapTo0To360Range(299.77 + 0.107408*k - 0.009173*T2);
    A1 = CoordinateTransformation::DegreesToRadians(A1);
    A2 = CoordinateTransformation::MapTo0To360Range(251.88 + 0.016321*k);
    A2 = CoordinateTransformation::DegreesToRadians(A2);
    A3 = CoordinateTransformation::MapTo0To360Range(251.83 + 26.651886*k);
    A3 = CoordinateTransformation::DegreesToRadians(A3);
    A4 = CoordinateTransformation::MapTo0To360Range(349.42 + 36.412478*k);
    A4 = CoordinateTransformation::DegreesToRadians(A4);
    A5 = CoordinateTransformation::MapTo0To360Range(84.66 + 18.206239*k);
    A5 = CoordinateTransformation::DegreesToRadians(A5);
    A6 = CoordinateTransformation::MapTo0To360Range(141.74 + 53.303771*k);
    A6 = CoordinateTransformation::DegreesToRadians(A6);
    A7 = CoordinateTransformation::MapTo0To360Range(207.14 + 2.453732*k);
    A7 = CoordinateTransformation::DegreesToRadians(A7);
    A8 = CoordinateTransformation::MapTo0To360Range(154.84 + 7.306860*k);
    A8 = CoordinateTransformation::DegreesToRadians(A8);
    A9 = CoordinateTransformation::MapTo0To360Range(34.52 + 27.261239*k);
    A9 = CoordinateTransformation::DegreesToRadians(A9);
    A10 = CoordinateTransformation::MapTo0To360Range(207.19 + 0.121824*k);
    A10 = CoordinateTransformation::DegreesToRadians(A10);
    A11 = CoordinateTransformation::MapTo0To360Range(291.34 + 1.844379*k);
    A11 = CoordinateTransformation::DegreesToRadians(A11);
    A12 = CoordinateTransformation::MapTo0To360Range(161.72 + 24.198154*k);
    A12 = CoordinateTransformation::DegreesToRadians(A12);
    A13 = CoordinateTransformation::MapTo0To360Range(239.56 + 25.513099*k);
    A13 = CoordinateTransformation::DegreesToRadians(A13);
    A14 = CoordinateTransformation::MapTo0To360Range(331.55 + 3.592518*k);
    A14 = CoordinateTransformation::DegreesToRadians(A14);
    //convert to radians

    if (moontype == NEWMOON)
        JD = deltaNewMoon(JD);
    else if (moontype == FULLMOON)
        JD = deltaFullMoon(JD);
    else if (moontype == FIRSTQUATER || moontype == LASTQUATER)
        JD = deltaQuaters(JD,moontype);

    //Additional corrections for all phases
    double DeltaJD2 = 0.000325*sin(A1) +
            0.000165*sin(A2) +
            0.000164*sin(A3) +
            0.000126*sin(A4) +
            0.000110*sin(A5) +
            0.000062*sin(A6) +
            0.000060*sin(A7) +
            0.000056*sin(A8) +
            0.000047*sin(A9) +
            0.000042*sin(A10) +
            0.000040*sin(A11) +
            0.000037*sin(A12) +
            0.000035*sin(A13) +
            0.000023*sin(A14);

    JD += DeltaJD2;

    double A_,B_,C_,D_,E_,F_,Z_;
    double Z0_ = JD + 0.5;

    Z_ = floor(Z0_); Z_ = round(Z_ * 10000) / 10000;
    double A0_ = floor((Z_ - 1867216.25) / 36524.25);
    A_ = Z_ + 1 + ( A0_) - floor( A0_ / 4);
    B_ = A_ + 1524;
    C_ = floor((B_ - 122.1)/365.25);
    D_ = floor(365.25*C_);
    E_ = floor((B_ - D_)/30.6001);
    F_ = Z0_-Z_;

    double portion_day = Z0_ - (int) Z0_;
    int hour =  portion_day * 24;
    int minute =  (portion_day * 1440) - (hour * 60);
    int day =  B_ - D_ - (int) (30.6001 * E_) + F_;

    int month = (E_ < 13.5) ? (E_ - 1) : (E_ - 13);
    int year  = (month > 2) ? (C_ - 4716) :( C_ - 4715);
    int secondes = (int) (portion_day * 86400) % 60;


    double dday = B_ - D_ - floor(30.6001 * E_) + F_;



    if (moontype == NEWMOON)
    {
        newmoon.dday = dday;
        newmoon.year = year;
        newmoon.month = month;
        newmoon.day = day;
        newmoon.hour = hour;
        newmoon.min  = minute;
        newmoon.secs = secondes;
    }
    else if (moontype == FULLMOON)
    {
        fullmoon.dday = dday;
        fullmoon.year = year;
        fullmoon.month = month;
        fullmoon.day = day;
        fullmoon.hour = hour;
        fullmoon.min  = minute;
        fullmoon.secs = secondes;
    }
    else if (moontype == FIRSTQUATER)
    {
        firstquater.dday = dday;
        firstquater.year = year;
        firstquater.month = month;
        firstquater.day = day;
        firstquater.hour = hour;
        firstquater.min  = minute;
        firstquater.secs = secondes;
    }
    else if (moontype == LASTQUATER)
    {
        lastquater.dday = dday;
        lastquater.year = year;
        lastquater.month = month;
        lastquater.day = day;
        lastquater.hour = hour;
        lastquater.min  = minute;
        lastquater.secs = secondes;
    }


    return dday;
}

double MoonPhases::K(double Year)
{
    //Convert from year to k
    return floor( 12.3685* (Year - 2000));
}

double MoonPhases::MeanPhase(double k)
{
    //convert from K to T
    double T = k/1236.85;
    double T2 = T*T;
    double T3 = T2*T;
    double T4 = T3*T;

    return 2451550.09766 + 29.530588861*k + 0.0001337*T2 - 0.000000150*T3 + 0.00000000073*T4;
}


double MoonPhases::deltaNewMoon(double jd)
{
    double DeltaJD = -0.40720*sin(Mdash) +
            0.17241*E*sin(M) +
            0.01608*sin(2*Mdash) +
            0.01039*sin(2*F) +
            0.00739*E*sin(Mdash - M) +
            -0.00514*E*sin(Mdash + M) +
            0.00208*E2*sin(2*M) +
            -0.00111*sin(Mdash - 2*F) +
            -0.00057*sin(Mdash + 2*F) +
            0.00056*E*sin(2*Mdash + M) +
            -0.00042*sin(3*Mdash) +
            0.00042*E*sin(M + 2*F) +
            0.00038*E*sin(M - 2*F) +
            -0.00024*E*sin(2*Mdash - M) +
            -0.00017*sin(omega) +
            -0.00007*sin(Mdash + 2*M) +
            0.00004*sin(2*Mdash - 2*F) +
            0.00004*sin(3*M) +
            0.00003*sin(Mdash + M - 2*F) +
            0.00003*sin(2*Mdash + 2*F) +
            -0.00003*sin(Mdash + M + 2*F) +
            0.00003*sin(Mdash - M + 2*F) +
            -0.00002*sin(Mdash - M - 2*F) +
            -0.00002*sin(3*Mdash + M) +
            0.00002*sin(4*Mdash);

    return jd + DeltaJD;
}

double MoonPhases::deltaQuaters(double jd,int moontype)
{
    double DeltaJD = -0.62801*sin(Mdash) +
            0.17172*E*sin(M) +
            -0.01183*E*sin(Mdash + M) +
            0.00862*sin(2*Mdash) +
            0.00804*sin(2*F) +
            0.00454*E*sin(Mdash - M) +
            0.00204*E2*sin(2*M) +
            -0.00180*sin(Mdash - 2*F) +
            -0.00070*sin(Mdash + 2*F) +
            -0.00040*sin(3*Mdash) +
            -0.00034*E*sin(2*Mdash - M) +
            0.00032*E*sin(M + 2*F) +
            0.00032*E*sin(M - 2*F) +
            -0.00028*E2*sin(Mdash + 2*M) +
            0.00027*E*sin(2*Mdash + M) +
            -0.00017*sin(omega) +
            -0.00005*sin(Mdash - M - 2*F) +
            0.00004*sin(2*Mdash + 2*F) +
            -0.00004*sin(Mdash + M + 2*F) +
            0.00004*sin(Mdash - 2*M) +
            0.00003*sin(Mdash + M - 2*F) +
            0.00003*sin(3*M) +
            0.00002*sin(2*Mdash - 2*F) +
            0.00002*sin(Mdash - M + 2*F) +
            -0.00002*sin(3*Mdash + M);

    //printf("Delta Jd: %f\n",DeltaJD);
    double JD = (jd + DeltaJD);
    //printf("JD sans W: %f\n",JD);
    double W = 0.00306 - 0.00038*E*cos(M) + 0.00026*cos(Mdash) - 0.00002*cos(Mdash - M) + 0.00002*cos(Mdash + M) + 0.00002*cos(2*F);
    //printf("W: %f\n",W);
    W=0.002642825;
    //First quarter
    if (moontype == FIRSTQUATER)
        JD += W;
    else
        JD -= W;

    return JD;

}
double MoonPhases::deltaFullMoon(double jd)
{
    double DeltaJD = -0.40614*sin(Mdash) +
            0.17302*E*sin(M) +
            0.01614*sin(2*Mdash) +
            0.01043*sin(2*F) +
            0.00734*E*sin(Mdash - M) +
            -0.00514*E*sin(Mdash + M) +
            0.00209*E2*sin(2*M) +
            -0.00111*sin(Mdash - 2*F) +
            -0.00057*sin(Mdash + 2*F) +
            0.00056*E*sin(2*Mdash + M) +
            -0.00042*sin(3*Mdash) +
            0.00042*E*sin(M + 2*F) +
            0.00038*E*sin(M - 2*F) +
            -0.00024*E*sin(2*Mdash - M) +
            -0.00017*sin(omega) +
            -0.00007*sin(Mdash + 2*M) +
            0.00004*sin(2*Mdash - 2*F) +
            0.00004*sin(3*M) +
            0.00003*sin(Mdash + M - 2*F) +
            0.00003*sin(2*Mdash + 2*F) +
            -0.00003*sin(Mdash + M + 2*F) +
            0.00003*sin(Mdash - M + 2*F) +
            -0.00002*sin(Mdash - M - 2*F) +
            -0.00002*sin(3*Mdash + M) +
            0.00002*sin(4*Mdash);

    return (jd +DeltaJD);
}



ln_dt *MoonPhases::getnewMoon()
{
    return &newmoon;
}
ln_dt *MoonPhases::getfullMoon()
{
    return &fullmoon;
}
ln_dt *MoonPhases::getfirstQuater()
{
    return &firstquater;
}
ln_dt *MoonPhases::getlastQuater()
{
    return &lastquater;
}
