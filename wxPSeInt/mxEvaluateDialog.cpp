#include "mxEvaluateDialog.h"
#include <wx/sizer.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include "DebugManager.h"
#include <wx/msgdlg.h>
using namespace std;

BEGIN_EVENT_TABLE(mxEvaluateDialog, wxDialog)
	EVT_BUTTON(wxID_OK,mxEvaluateDialog::OnEvaluateButton)
	EVT_BUTTON(wxID_CANCEL,mxEvaluateDialog::OnCloseButton)
	EVT_CHAR_HOOK(mxEvaluateDialog::OnCharHook)
//	EVT_CLOSE(mxEvaluateDialog::OnClose)	
END_EVENT_TABLE()

	
mxEvaluateDialog::mxEvaluateDialog(wxWindow *parent) : wxDialog(parent,wxID_ANY,"Evaluar expresiones",wxDefaultPosition,wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER) {

	wxBoxSizer *mySizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *comboSizer= new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	
	mySizer->Add(new wxStaticText(this, wxID_ANY, "Introduzca una expresi�n y presione Enter:", wxDefaultPosition, wxDefaultSize, 0), wxSizerFlags().Border(wxTOP|wxLEFT|wxRIGHT,5));
	
	combo = new wxComboBox(this, wxID_ANY);
	comboSizer->Add(combo, wxSizerFlags()/*.Border(wxALL,5)*/.Proportion(1).Expand());
//	wxButton *button_evaluate = new wxButton(this,wxID_OK, " &Evaluar ",wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
//	comboSizer->Add( button_evaluate /*, wxSizerFlags().Border(wxALL,5)*/);
	
	mySizer->Add(comboSizer, wxSizerFlags().Border(wxALL,5).Proportion(0).Expand());
	
	mySizer->Add(new wxStaticText(this, wxID_ANY, "Valor actual:", wxDefaultPosition, wxDefaultSize, 0), wxSizerFlags().Border(wxTOP|wxLEFT|wxRIGHT,5));
	result = new wxTextCtrl(this, wxID_ANY, "<<Presione Evaluar para ver aqui el resultado>>", wxDefaultPosition, wxSize(200,60), wxTE_MULTILINE|wxTE_READONLY);
	mySizer->Add(result, wxSizerFlags().Border(wxLEFT|wxRIGHT,5)./*DoubleBorder(wxBOTTOM|wxLEFT|wxRIGHT).*/Proportion(1).Expand());
	tipo = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	wxBoxSizer *tipoSizer = new wxBoxSizer(wxHORIZONTAL);
	tipoSizer->Add(new wxStaticText(this,wxID_ANY,"Tipo: "),wxSizerFlags().Center());
	tipoSizer->Add(tipo, wxSizerFlags().Proportion(1).Expand());
	mySizer->Add(tipoSizer, wxSizerFlags().Border(wxALL,5)./*DoubleBorder(wxBOTTOM|wxLEFT|wxRIGHT).*/Proportion(0).Expand());
	
	wxButton *button_close = new wxButton(this,wxID_CANCEL, "&Cerrar",wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT);
	bottomSizer->Add( button_close /*, wxSizerFlags().Border(wxALL,5)*/);
	SetAffirmativeId(wxID_OK);
	SetEscapeId(wxID_CANCEL);
	mySizer->Add(bottomSizer,wxSizerFlags()./*Border(wxALL,5).*/Right());
	
	SetMinSize(GetSize());
	SetSizerAndFit(mySizer);
	SetFocusFromKbd();
	
}

mxEvaluateDialog::~mxEvaluateDialog() {
	
}

void mxEvaluateDialog::OnEvaluateButton(wxCommandEvent &evt) {
	Evaluate();
}

void mxEvaluateDialog::Evaluate() {
	if (!debug->debugging || !debug->paused) {
		result->SetValue("<<La ejecuci�n paso a paso debe estar iniciada y pausada para poder evaluar expresiones>>");
	} else if (combo->GetValue().Len()) {
//		result->SetValue("<<evaluando...>>");
		act_expression=combo->GetValue();
		if ( !last_expression.Len() || (act_expression.Len() && last_expression!=act_expression) )
			combo->Append(act_expression);
		debug->SendEvaluation(act_expression);
		combo->SetSelection(0,act_expression.Len());
		last_expression=act_expression;
	} else
		result->SetValue("<<Debe ingresar una expresi�n>>");
}

void mxEvaluateDialog::OnCloseButton(wxCommandEvent &evt) {
	Hide();
}

void mxEvaluateDialog::SetEvaluationValue(wxString val, char t) {
	result->SetValue(val);
	switch (t) {
	case '1': tipo->SetValue("L�gico"); break;
	case '2': tipo->SetValue("Num�rico"); break;
	case '3': tipo->SetValue("L�gico o Num�rico"); break;
	case '4': tipo->SetValue("Caracter"); break;
	case '5': tipo->SetValue("L�gico o Caracter"); break;
	case '6': tipo->SetValue("Num�rico o Caracter"); break;
	default: tipo->SetValue("Indefinido"); break;
	}
}

//void mxEvaluateDialog::OnClose(wxCloseEvent &evt) {
//	Destroy();
//}

	
void mxEvaluateDialog::OnCharHook(wxKeyEvent &evt)	{
	if (evt.GetKeyCode()==WXK_RETURN || evt.GetKeyCode()==WXK_NUMPAD_ENTER) {
		Evaluate();
	} else evt.Skip();
}
	
void mxEvaluateDialog::Show() {
//	result->SetValue("<<Presione Evaluar para ver aqui el resultado>>");
	wxDialog::Show();
}

void mxEvaluateDialog::EvaluateExpression (wxString exp) {
	combo->SetValue(exp);
	wxCommandEvent evt;
	OnEvaluateButton(evt);
}

