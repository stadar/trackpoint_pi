/******************************************************************************
 * $Id: trackpoint_pi.cpp, v0.3 2017/01/21 SethDart Exp $
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


#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <typeinfo>
#include "trackpoint_pi.h"
#include "icons.h"
#include <math.h>


// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new trackpoint_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}



//---------------------------------------------------------------------------------------------------------
//
//    TRACKPOINT PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

trackpoint_pi::trackpoint_pi(void *ppimgr)
    :opencpn_plugin_16(ppimgr), wxTimer(this),
     m_generate_ais(true)
{
    // Create the PlugIn icons
    initialize_images();
}

int trackpoint_pi::Init(void)
{
    AddLocaleCatalog( _T("opencpn-trackpoint_pi") );

    //    Get a pointer to the opencpn configuration object
    m_pconfig = GetOCPNConfigObject();

    //    And load the configuration items
    LoadConfig();

    //    This PlugIn needs one toolbar icon
    m_tb_item_id_settings = InsertPlugInTool(_T(""), _img_trackpoint_settings, _img_trackpoint_settings, wxITEM_CHECK,
                                           _("Trackpoint Settings"), _T(""), NULL, TRACKPOINT_TOOL_POSITION, 0, this);
    // 88 for testing

    return (
               WANTS_TOOLBAR_CALLBACK    |
               INSTALLS_TOOLBAR_TOOL     |
               WANTS_CONFIG              |
               WANTS_NMEA_SENTENCES      |
               WANTS_PREFERENCES
           );
}

bool trackpoint_pi::DeInit(void)
{
    SaveConfig();

    RemovePlugInTool( m_tb_item_id_settings );
    return true;
}

int trackpoint_pi::GetAPIVersionMajor()
{
    return MY_API_VERSION_MAJOR;
}

int trackpoint_pi::GetAPIVersionMinor()
{
    return MY_API_VERSION_MINOR;
}

int trackpoint_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int trackpoint_pi::GetPlugInVersionMinor()
{
    return PLUGIN_VERSION_MINOR;
}

wxBitmap *trackpoint_pi::GetPlugInBitmap()
{
    return _img_trackpoint_pi;
}

wxString trackpoint_pi::GetCommonName()
{
    return _("TRACKPOINT");
}


wxString trackpoint_pi::GetShortDescription()
{
    return _("Trackpoint II plugin for OpenCPN");
}

wxString trackpoint_pi::GetLongDescription()
{
    return _("Trackpoint II plug-in for OpenCPN\n\
Accepts PORE NMEA strings and translates them to AIS ARPA targets and Depth\n\
Target numbers can be assigned names for better readability.\n\
Copyright (C) 2017 by Ilias Daradimos");

}

void trackpoint_pi::SetNMEASentence(wxString &sentence)
{
    m_NMEA0183 << sentence;
    wxString TTM,Depth;

    if(m_NMEA0183.PreParse())
    {
        if(m_NMEA0183.LastSentenceIDReceived == "PORE")
        {
            if(m_NMEA0183.Parse())
            {
                //m_NMEA0183.Pore.Dump();

                if (m_generate_ais)
                {
                    TTM=PORE_2_TTM(m_NMEA0183, m_target_name[m_NMEA0183.Pore.TargetNumber-1]);
                    PushNMEABuffer(TTM);
                }

                if (m_generate_depth && m_NMEA0183.Pore.TargetNumber == m_depth_target)
                {
                    Depth = PORE_2_Depth(m_NMEA0183);
                    PushNMEABuffer(Depth);
                }
            }
        }
    }
}


wxString trackpoint_pi::PORE_2_TTM(NMEA0183 nmea, wxString name)
{
    wxString TTM;
    double distance;

    distance = sqrt(pow(nmea.Pore.X, 2) + pow(nmea.Pore.Y, 2));
    TTM += wxString::Format(wxT("$TPTTM,%02d,%1.4f,%3.1f,R,,,,,,K,%s,T,,%s,A*"),
                            nmea.Pore.TargetNumber,
                            distance,
                            nmea.Pore.TargetBearing,
                            name,
                            nmea.Pore.HMS
                           );
    TTM += wxString::Format(wxT("%02X"),(ComputeChecksum(TTM)));
    TTM+="\r\n";
    return TTM;
}

wxString trackpoint_pi::PORE_2_Depth(NMEA0183 nmea)
{
    wxString Depth;

    Depth += wxString::Format(wxT("$TPDBT,,f,%1.1f,M,,F*"),
                            nmea.Pore.Z
                           );
    Depth += wxString::Format(wxT("%02X"),(ComputeChecksum(Depth)));
    Depth+="\r\n";
    return Depth;
}

unsigned char trackpoint_pi::ComputeChecksum( wxString Sentence ) const
{
    unsigned char checksum_value = 0;

    int string_length = Sentence.Length();
    int index = 1; // Skip over the $ at the begining of the sentence

    while( index < string_length    &&
            Sentence[ index ] != '*' &&
            Sentence[ index ] != CARRIAGE_RETURN &&
            Sentence[ index ] != LINE_FEED )
    {
        checksum_value ^= (char)Sentence[ index ];
        index++;
    }

    return( checksum_value );
}

void trackpoint_pi::ShowPreferencesDialog( wxWindow* parent )
{
    wxTextCtrl *targetName;
    TP2Conf *conf = new TP2Conf( parent );
    conf->Fit();
    wxColor     cl;
    GetGlobalColor(_T("DILG1"), &cl);
    conf->SetBackgroundColour(cl);

    conf->cbAIS->SetValue(m_generate_ais);
    conf->cbDepth->SetValue(m_generate_depth);
    conf->cDepthTarget->SetSelection(m_depth_target-1);
    for(int i=0; i<9; i++) {
        targetName = (wxTextCtrl *) conf->targetList.Item(i);
        targetName->SetValue(m_target_name[i]);
    }

    if(conf->ShowModal() == wxID_OK)
    {
        m_generate_ais = conf->cbAIS->GetValue();
        m_generate_depth    = conf->cbDepth->GetValue();
        m_depth_target = conf->cDepthTarget->GetSelection()+1;
        for(int i=0; i<9; i++) {
            targetName = (wxTextCtrl *) conf->targetList.Item(i);
            m_target_name[i] = targetName->GetValue();
        }

        SaveConfig();
    }
}

int trackpoint_pi::GetToolbarToolCount(void)
{
    return 1;
}

void trackpoint_pi::OnToolbarToolCallback(int id)
{
    if ( id == m_tb_item_id_settings )
    {
        ShowPreferencesDialog(GetOCPNCanvasWindow());
        SetToolbarItemState(id, false);
    }
}

bool trackpoint_pi::LoadConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(pConf)
    {
        pConf->SetPath( _T("/PlugIns/TRACKPOINT") );

        pConf->Read( _T("GenerateAIS"), &m_generate_ais, 1 );
        pConf->Read( _T("GenerateDepth"), &m_generate_depth, 1 );
        pConf->Read( _T("DepthTarget"), &m_depth_target, 1 );

        for (int i=0; i<9; i++) {
            pConf->Read( _T("TargetName_"+wxString::Format("%d", i)), &m_target_name[i] );
        }

        return true;
    }
    else
        return false;
}

bool trackpoint_pi::SaveConfig(void)
{
    wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

    if(pConf)
    {
        pConf->SetPath( _T("/PlugIns/TRACKPOINT") );

        pConf->Write( _T("GenerateAIS"), m_generate_ais );
        pConf->Write( _T("GenerateDepth"), m_generate_depth );
        pConf->Write( _T("DepthTarget"), m_depth_target);
        for (int i=0; i<9; i++) {
            pConf->Write( _T("TargetName_"+wxString::Format("%d", i)), m_target_name[i] );
        }

        return true;
    }
    else
        return false;
}


