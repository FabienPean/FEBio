// MainApp.h: interface for the CMainApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINAPP_H__5432BE41_2F50_46E1_80E3_E83BAC5BF8D8__INCLUDED_)
#define AFX_MAINAPP_H__5432BE41_2F50_46E1_80E3_E83BAC5BF8D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Flx_App.h>

#include "Wnd.h"
#include "Document.h" 
#include <FL/Fl_Preferences.H>

class CMainApp : public Flx_App 
{
public:
	CMainApp();
	virtual ~CMainApp();

	bool Init(); // initializes the application
	bool Load(const char* szfilename);
	int Run();	// runs the application

	CDocument* GetDocument() { return m_pDoc; }
	CWnd* GetMainWnd() { return m_pMainWnd; }

	Fl_Preferences& GetPreferences() { return m_prefs; }

protected:
	CWnd*		m_pMainWnd;	// main window
	CDocument*	m_pDoc;		// document class
	Fl_Preferences	m_prefs;	// FEBioTM preferences
};

CMainApp* FLXGetMainApp();
CWnd*	FLXGetMainWnd();

#endif // !defined(AFX_MAINAPP_H__5432BE41_2F50_46E1_80E3_E83BAC5BF8D8__INCLUDED_)
