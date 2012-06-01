/* =====================================================================
 *
 *  Copyright 2009, 2010, Freescale Semiconductor, Inc., All Rights Reserved.
 *
 *  This file contains copyrighted material. Use of this file is restricted
 *  by the provisions of a Freescale Software License Agreement, which has
 *  either accompanied the delivery of this software in shrink wrap
 *  form or been expressly executed between the parties.
 *
 *  File Name : FMCGenericError.cpp
 *  Author    : Serge Lamikhov-Center
 *
 * ===================================================================*/

#include <map>
#include <iostream>
#include "FMCGenericError.h"
#include "FMCUtils.h"

void errorFuncHandler( void * ctx, xmlErrorPtr error )
{
    char *filestr = (char*)"";
    char *msgstr  = (char*)"";

    if ( error->message != 0 ) msgstr  = error->message;

    if ( error->file != 0 ) {
        filestr = error->file;
        ((CGenericError*)ctx)->createMessage( ERR_XML_PARSE_ERROR, -1, filestr, error->line, msgstr );
    }
    else {
        ((CGenericError*)ctx)->createMessage( ERR_XML_PARSE_ERROR1, -1, msgstr );
    }
}

/*Define static members*/
int CGenericError::dontWarn = 0;


CGenericError::CGenericError(std::string error, CErrorElem elem1, CErrorElem elem2, CErrorElem elem3, CErrorElem elem4, CErrorElem elem5,
                             CErrorElem elem6,  CErrorElem elem7, CErrorElem elem8, CErrorElem elem9, CErrorElem elem10)
{
    createMessage (error, NO_LINE, elem1, elem2, elem3, elem4, elem5,
                                   elem6, elem7, elem8, elem9, elem10);
}

void CGenericError::printWarning(std::string error, CErrorElem elem1, CErrorElem elem2, CErrorElem elem3, CErrorElem elem4, CErrorElem elem5,
                                 CErrorElem elem6,  CErrorElem elem7, CErrorElem elem8, CErrorElem elem9, CErrorElem elem10)
{
    if (dontWarn)
        return;
    CGenericError tempError (error, elem1, elem2, elem3, elem4,
                             elem5, elem6, elem7, elem8, elem9, elem10);
    std::cerr << "Warning: " << tempError.msg << std::endl;
}


CGenericErrorLine::CGenericErrorLine(std::string error, int line,        CErrorElem elem1, CErrorElem elem2, CErrorElem elem3, CErrorElem elem4,
                                     CErrorElem elem5, CErrorElem elem6, CErrorElem elem7, CErrorElem elem8, CErrorElem elem9, CErrorElem elem10)
{
    createMessage (error, line, elem1, elem2, elem3, elem4, elem5,
                                elem6, elem7, elem8, elem9, elem10);
}

void CGenericErrorLine::printWarning(std::string error, int line,        CErrorElem elem1, CErrorElem elem2, CErrorElem elem3, CErrorElem elem4,
                                     CErrorElem elem5, CErrorElem elem6, CErrorElem elem7, CErrorElem elem8, CErrorElem elem9, CErrorElem elem10)
{
    if (dontWarn)
        return;
    CGenericErrorLine tempError (error, line,  elem1, elem2, elem3, elem4,
                                 elem5, elem6, elem7, elem8, elem9, elem10);
    std::cerr << "Warning: " << tempError.getErrorMsg() << std::endl;
}

void CGenericError::createMessage(std::string error, int line,         CErrorElem elem1, CErrorElem elem2, CErrorElem elem3, CErrorElem elem4, CErrorElem elem5,
                                  CErrorElem elem6,  CErrorElem elem7, CErrorElem elem8, CErrorElem elem9, CErrorElem elem10)
{
    int found = 0;
    std::string tempStr = error;
    while (found!=std::string::npos)
    {
        found = tempStr.find('%');
        msg += tempStr.substr(0, found);
        if (found==std::string::npos)
            break;

        if (tempStr.substr(found, 2) == "%1")
            msg += elem1.str;
        else if (tempStr.substr(found, 2) == "%2")
            msg += elem2.str;
        else if (tempStr.substr(found, 2) == "%3")
            msg += elem3.str;
        else if (tempStr.substr(found, 2) == "%4")
            msg += elem4.str;
        else if (tempStr.substr(found, 2) == "%5")
            msg += elem5.str;
        else if (tempStr.substr(found, 2) == "%6")
            msg += elem6.str;
        else if (tempStr.substr(found, 2) == "%7")
            msg += elem7.str;
        else if (tempStr.substr(found, 2) == "%8")
            msg += elem8.str;
        else if (tempStr.substr(found, 2) == "%9")
            msg += elem9.str;
        else if (tempStr.substr(found, 2) == "%10")
        {
            found++;
            msg += elem10.str;
        }
        else
        {
            msg += "%";
            found-=1;
        }
        found+=2;
        tempStr = tempStr.substr(found);
    }
    if (line != NO_LINE)
    {
        if (msg[msg.size()-1] != ' ')
            msg += " ";

        if (msg[msg.size()-1] == '.' || msg[msg.size()-2] == '.')
            msg += "Line " + intToString(line);
        else
            msg += "- line " + intToString(line);
    }
}

std::string CGenericError::getErrorMsg() const
{
    return msg;
}


/*-------------------------- CErrorElem ------------------------------*/

CGenericError::CErrorElem::CErrorElem(int i)
{
    str = intToString(i);
}

CGenericError::CErrorElem::CErrorElem(std::string str1)
{
    str = str1;
}

CGenericError::CErrorElem::CErrorElem(const char* chr)
{
    str = chr;
}

CGenericError::CErrorElem::CErrorElem()
{
    str = "";
}
