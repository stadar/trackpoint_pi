/***************************************************************************
 *
 * Project:  OPENCPN
 * Purpose:  NMEA0183 PORE Class
 * Author:   Ilias Daradimos
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Ilias Daradimos                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.         *
 ***************************************************************************
 *
 *   S Blackburn's original source license:                                *
 *         "You can use it any way you like."                              *
 *   More recent (2010) license statement:                                 *
 *         "It is BSD license, do with it what you will"                   *
 */


#include "nmea0183.h"
#include <iostream>

PORE::PORE()
{
    Mnemonic = _T("PORE");
    Empty();
}

PORE::~PORE()
{
    Mnemonic.Empty();
    Empty();
}

void PORE::Empty( void )
{
    TargetNumber=1;
    HMS="000000";
    Heading=0;
    HeadingIdentifier='T';
    TargetBearing=0.0;
    X=0.0;
    Y=0.0;
    Z=0.0;
    Roll=0.0;
    Pitch=0.0;
    WarningCode=0;
    QualityFactor=1;
}

bool PORE::Parse( const SENTENCE& sentence )
{
    /*
    ** First we check the checksum...
    */

    if ( sentence.IsChecksumBad( 13 ) == TRUE )
    {
        SetErrorMessage( _T("Invalid Checksum") );
        return( FALSE );
    }

    TargetNumber=sentence.Integer(1);
    HMS=sentence.Field(2);
    Heading = sentence.Double( 3 );
    HeadingIdentifier=sentence.Double( 4 );
    TargetBearing = sentence.Double( 5 );
    X = sentence.Double( 6 );
    Y = sentence.Double( 7 );
    Z = sentence.Double( 8 );
    Roll = sentence.Double( 9 );
    Pitch = sentence.Double( 10 );
    WarningCode=sentence.Integer(11);
    QualityFactor=sentence.Integer(12);

    return( TRUE );
}

bool PORE::Write( SENTENCE& sentence )
{
    /*
    ** Let the parent do its thing
    */

    RESPONSE::Write( sentence );

    sentence.Finish();

    return( TRUE );
}

const PORE& PORE::operator = ( const PORE& source )
{
    //TargetNumber = source.TargetNumber;

    return( *this );
}

void PORE::Dump(void)
{
// TODO (drid#1#): Use OpenCPN console log

    std::cout << "Target Number:" << TargetNumber <<std::endl;
    std::cout << "HMS:" << HMS <<std::endl;
    std::cout << "Heading:" << Heading << HeadingIdentifier << std::endl;
    std::cout << "TargetBearing:" << TargetBearing <<std::endl;
    std::cout << "X Y Z:" << X << " " << Y << " " << Z << std::endl;
    std::cout << "Roll / Pitch:" << Roll << " " << Pitch <<std::endl;
    std::cout << "WarningCode:" << WarningCode <<std::endl;
    std::cout << "QualityFactor:" << QualityFactor <<std::endl<<std::endl;
}
