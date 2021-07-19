#ifndef SIS810_AP_H
#define SIS810_AP_H
//-------------------------------------------------------------------------
//=========================================================================
//####### DESCRIPTIONBEGIN #######
//
// sis810ap.h
//
// Author:       Chaoban Wang
// Date:         2009-03-30
// Purpose:
// Description:
//
//=========================================================================

enum ParseState
{
    _HEADID,
    _COMMAND,
    _FLAG1,
    _FLAG2,
    _FLAG3,
    _DATA,
};


void Parsefile(FILE *fpin, FILE *fpout);

int Ascii2Int(int ch);

int GetByteDate(FILE *fpin);

#endif // SIS810_AP_H
