/******************************************************************************
 * $Id: trackpoint_pi.h, v0.3 2017/01/21 SethDart Exp $
 *
 * Project:  OpenCPN
 * Purpose:  TRACKPOINT Plugin
 * Author:   Ilias Daradimos
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Ilias Daradimos                                 *
 *   $EMAIL$                                                               *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _TRACKPOINTPI_H_
#define _TRACKPOINTPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    2

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    6

#include <wx/fileconf.h>
#include <ocpn_plugin.h>

#include "nmea/nmea0183.h"

#define TRACKPOINT_TOOL_POSITION -1

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

class TRACKPOINTControl;

class trackpoint_pi : public opencpn_plugin_16, wxTimer
{
public:
    trackpoint_pi( void *ppimgr );

//    The required PlugIn Methods
    int Init( void );
    bool DeInit( void );

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    wxBitmap *GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

    wxString PORE_2_TTM(NMEA0183 nmea, wxString name);
    wxString PORE_2_Depth(NMEA0183 nmea);
    unsigned char ComputeChecksum( wxString Sentence ) const;
    void ShowPreferencesDialog( wxWindow* parent );

//    The optional method overrides
    void SetNMEASentence( wxString &sentence );
    int GetToolbarToolCount( void );
    void OnToolbarToolCallback( int id );

    NMEA0183        m_NMEA0183;

private:
    bool LoadConfig( void );
    bool SaveConfig( void );

    int               m_tb_item_id_settings;

    wxFileConfig     *m_pconfig;
    int TargetNumber;
    char HMS[6];
    double Heading;
    char HeadingIdentifier;
    double TargetBearing;
    double X;
    double Y;
    double Z;
    double Roll;
    double Pitch;
    int WarningCode;
    int QualityFactor;

    bool              m_generate_ais;
    wxCheckBox       *m_pGenerateAIS;
    bool              m_generate_depth;
    wxCheckBox       *m_pGenerateDepth;
    wxString          m_target_name[9];
    wxTextCtrl       *m_pTargetName;
};

#endif
