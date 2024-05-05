#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>

class TextureDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TextureDialogue)

public:
	TextureDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph);   // modal // takes in out scenegraph in the constructor
	TextureDialogue(CWnd* pParent = NULL);
	virtual ~TextureDialogue();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	afx_msg void Select();	//Item has been selected

	int* m_currentSelection;

	//Data Storages
	std::vector<CString> m_texturePaths;
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_selectedObjectID;
	std::string m_currentTexturePath;

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient access of the listbox
	CListBox m_listBox;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();

	//Functionality
	void FindTextures(std::vector<SceneObject>* SceneGraph, std::vector<int>* IDData);
	void DeSelect();
	bool DeleteSelected();

	afx_msg void OnBnClickedOk();

	//Variables for checks
	bool RebuildScene;
};


INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
