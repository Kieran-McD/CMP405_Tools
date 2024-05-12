#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "CFloatEdit.h"

class ModifyObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ModifyObjectDialogue)

public:
	ModifyObjectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph);   // modal // takes in out scenegraph in the constructor
	ModifyObjectDialogue(CWnd* pParent = NULL);
	virtual ~ModifyObjectDialogue();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue

	int m_currentSelection;

	std::vector<CString> m_modelPath;
	std::vector<CString> m_texturePaths;
	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_selectedObjectID;

	afx_msg void UpdateObjectPosition();
	afx_msg void UpdateObjectScale();
	afx_msg void UpdateObjectRotation();
	afx_msg void UpdateObjectModel();
	afx_msg void UpdateObjectTexture();
	afx_msg void UpdateSelectedObject();

	

	DECLARE_MESSAGE_MAP()
public:
	// Control variable for more efficient access of the listbox
	CComboBox m_modelComboBox;
	CComboBox m_textureComboBox;
	CComboBox m_selectedObjectBox;

	CEdit m_idNumEdit;
	CFloatEdit m_scaleXEdit;
	CFloatEdit m_scaleYEdit;
	CFloatEdit m_scaleZEdit;

	CFloatEdit m_posXEdit;
	CFloatEdit m_posYEdit;
	CFloatEdit m_posZEdit;

	CFloatEdit m_rotXEdit;
	CFloatEdit m_rotYEdit;
	CFloatEdit m_rotZEdit;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();

	void RetrieveData(std::vector<SceneObject>* SceneGraph, std::vector<int>* SelectedID);
	void UpdateObjectList();

	afx_msg void OnBnClickedOk();

	bool RebuildScene;
};

