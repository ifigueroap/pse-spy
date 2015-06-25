#ifndef MXCREATORWINDOW_H
#define MXCREATORWINDOW_H
#include <wx/frame.h>
#include <wx/timer.h>
#include "Package.h"

class wxCheckBox;
class wxTextCtrl;
class wxButton;
class wxStaticText;

class mxCreatorWindow : public wxFrame {
	wxStaticText *static1;
	wxStaticText *static2;
	wxStaticText *static3;
	wxString pseint_command;
	wxTextCtrl *input_path;
//	wxTextCtrl *output_path;
	wxTextCtrl *msg_ok;
	wxTextCtrl *msg_bad;
	wxCheckBox *chk_pass;
	wxTextCtrl *password1;
	wxTextCtrl *num_tests;
	wxCheckBox *show_errors;
	wxCheckBox *show_solutions;
	wxCheckBox *stop_on_first_error;
	wxCheckBox *shuffle_tests;
	wxCheckBox *base_psc;
	wxCheckBox *help_html;
	wxTimer timer;
	wxButton *solutions_button;
	wxButton *ok_button;
public:
	mxCreatorWindow(const wxString &cmdline);
	void OnTimer(wxTimerEvent &event);
	void OnCreate(wxCommandEvent &event);
	void OnInputPath(wxCommandEvent &event);
//	void OnOutputPath(wxCommandEvent &event);
	void OnInputChange(wxCommandEvent &event);
	void OnShowErrors(wxCommandEvent &event);
	void OnGenerateSolutions(wxCommandEvent &event);
	void OnCheckPass(wxCommandEvent &event);
	void OnReloadInput(wxCommandEvent &event);
	void EnableDisable();
	wxString GetFilePath(const wxString &name);
	DECLARE_EVENT_TABLE();
};

#endif

