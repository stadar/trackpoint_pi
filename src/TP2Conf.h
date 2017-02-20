#ifndef TP2CONF_H
#define TP2CONF_H
#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/choice.h>

class TP2Conf : public wxDialog
{
    public:
        wxCheckBox       *m_pGenerateAIS;
        wxCheckBox       *m_pGenerateDepth;
        wxTextCtrl       *m_pTargetName;
        //wxComboBox       *m_pDepthTarget;
        wxChoice* cDepthTarget;
		wxCheckBox* cbDepth;
		wxCheckBox* cbAIS;
        wxArrayPtrVoid targetList;

        TP2Conf(wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxString& = _("Trackpoint Preferences"),
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxCAPTION|wxDEFAULT_DIALOG_STYLE );
        ~TP2Conf();
    protected:
        static const long ID_CB_AIS;
		static const long ID_CB_DEPTH;
		static const long ID_C_DEPTH_TARGET;
		static const long ID_PANEL1;

    private:
};

#endif // TP2CONF_H
