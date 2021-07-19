//-------------------------------------------------------------------------
//=========================================================================
//####### DESCRIPTIONBEGIN #######
//
// sis810ap.c
//
// Author:       Chaoban Wang
// Date:         2009-03-30
// Purpose:
// Description:
//
//=========================================================================

#include <stdio.h>
#include <stdlib.h>
#include "sis810ap.h"

int
main(int argc, char *argv[])
{
    char filename[40] = {0};

    if (argc < 2)
    {
        printf("Usage: %s [filename].\n", argv[0]);
        printf("Press any key to exit.");
        getchar();
        exit(1);
    }

    FILE *fpin  = fopen(argv[1], "rb");

    if (fpin == NULL)
    {
        printf("Can not open the file '%s'.\n", argv[1]);
        printf("Press any key to exit.");
        getchar();
        return 0;
    }

    strcpy(filename, argv[1]);
    strcat(filename, ".out");

    FILE *fpout = fopen(filename, "w");

    if (fpout == NULL)
    {
        printf("Can not create the file '%s'.\n", filename);
        printf("Press any key to exit.");
        getchar();
        return 0;
    }

    Parsefile(fpin, fpout);

    fclose(fpin);
    fclose(fpout);

    return 0;
}

void
Parsefile(FILE *fpin, FILE *fpout)
{
    int ch = 0;
    int datalen = 0;
    int state = _HEADID;

    while ((ch = getc(fpin)) != EOF)
    {
        switch (state)
        {
            case _HEADID:
                if (ch == 'P')
                {
                    if ((ch = getc(fpin)) == '8')
                    {
                        if ((ch = getc(fpin)) == '1')
                        {
                            if ((ch = getc(fpin)) == '0')
                            {
                                state = _COMMAND;
                            }
                        }
                    }
                }
                break;

            case _COMMAND:
                state = _HEADID;

                if (ch == 0xA0)
                {
                    ch = getc(fpin);

                    if (ch == 0x01)
                    {
                        state = _FLAG1;
                    }
                    else if (ch == 0x02)
                    {
                        state = _FLAG2;
                    }
                    else if (ch == 0x03)
                    {
                        state = _FLAG3;
                    }

                    if ((state == _FLAG1) || (state == _FLAG2) || (state == _FLAG3))
                    {
                        printf("Cmd_%d\n", ch);

                        datalen = GetByteDate(fpin);
                        printf("Data_len: %d\n", datalen);
                    }
                    else
                    {
                        printf("Cmd_%d error.\n", ch);
                    }
                }
                else
                {
                    printf("No command.\n");
                }
                break;

            case _FLAG1:

                state = _DATA;
                break;

            case _FLAG2:


                state = _DATA;
                break;

            case _FLAG3:

                state = _DATA;
                break;

            case _DATA:

                state = _HEADID;
                break;

            default:
                break;
        }
    }
}

int
Ascii2Int(int ch)
{
    if ((ch >= 0x30) && (ch <= 0x39))
    {
        ch &= ~0x30;
    }
    else if (((ch >= 0x41) && (ch <= 0x46)) || ((ch >= 0x61) && (ch <= 0x66)))
    {
        ch &= ~0xf0;
        ch += 9;
    }
    else
    {
        printf("ERROR value: %c\n", ch);
        return 0;
    }

    return ch;
}

int
GetByteDate(FILE *fpin)
{
    int temp = 0;

    temp = (getc(fpin) << 8) | getc(fpin);

    return temp;
}
