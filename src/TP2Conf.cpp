#include "TP2Conf.h"

const long TP2Conf::ID_CB_AIS = wxNewId();
const long TP2Conf::ID_CB_DEPTH = wxNewId();
const long TP2Conf::ID_C_DEPTH_TARGET = wxNewId();
const long TP2Conf::ID_PANEL1 = wxNewId();

TP2Conf::TP2Conf(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style ) : wxDialog( parent, id, title, pos, size, style )
{

    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

    int border_size = 4;
    wxArrayPtrVoid targetLabels;
    wxTextCtrl *targetName;
    wxStaticText *targetLabel;
    wxString depthStr[9];

    wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* sizerSettings;

	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	sizerSettings = new wxStaticBoxSizer(wxVERTICAL, this, _("Settings"));
	cbAIS = new wxCheckBox(this, ID_CB_AIS, _("Generate AIS"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CB_AIS"));
	cbAIS->SetValue(false);
	sizerSettings->Add(cbAIS, 1, wxALL|wxALIGN_LEFT, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	cbDepth = new wxCheckBox(this, ID_CB_DEPTH, _("Generate Depth:"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CB_DEPTH"));
	cbDepth->SetValue(false);
	BoxSizer2->Add(cbDepth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	cDepthTarget = new wxChoice(this, ID_C_DEPTH_TARGET, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_C_DEPTH_TARGET"));
	cDepthTarget->SetSelection( cDepthTarget->Append(_("1")) );
	cDepthTarget->Append(_("2"));
	cDepthTarget->Append(_("3"));
	cDepthTarget->Append(_("4"));
	cDepthTarget->Append(_("5"));
	cDepthTarget->Append(_("6"));
	cDepthTarget->Append(_("7"));
	cDepthTarget->Append(_("8"));
	cDepthTarget->Append(_("9"));
	BoxSizer2->Add(cDepthTarget, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	sizerSettings->Add(BoxSizer2, 1, wxALL|wxALIGN_LEFT, 5);
	BoxSizer1->Add(sizerSettings, 0, wxALIGN_LEFT, 0);

    /*  Target Names */
    for(int i=0; i<9; i++) {
        targetList.Add(new wxTextCtrl(this, -1, wxT(""),  wxDefaultPosition, wxSize(300, 25)));
        targetName = (wxTextCtrl *) targetList.Item(i);
        targetLabel = new wxStaticText(this, -1,  wxString::Format("Target %d name", i+1), wxDefaultPosition, wxDefaultSize);

        sizerSettings->Add(targetLabel, 0, wxALL|wxALIGN_LEFT, border_size);
        sizerSettings->Add(targetName, 0, wxALL|wxALIGN_LEFT, border_size);
    }

    wxStdDialogButtonSizer* DialogButtonSizer = this->CreateStdDialogButtonSizer(wxOK|wxCANCEL);
    BoxSizer1->Add(DialogButtonSizer, 0, wxALIGN_RIGHT|wxALL, 5);

	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

}

TP2Conf::~TP2Conf()
{
}
