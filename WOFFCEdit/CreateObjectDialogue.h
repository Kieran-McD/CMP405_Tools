#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>
#include "CFloatEdit.h"

class CreateObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(CreateObjectDialogue)

public:
	CreateObjectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph);   // modal // takes in out scenegraph in the constructor
	CreateObjectDialogue(CWnd* pParent = NULL);
	virtual ~CreateObjectDialogue();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue

	int* m_currentSelection;

	std::vector<CString> m_modelPath;
	std::vector<CString> m_texturePaths;
	std::vector<SceneObject>* m_sceneGraph;


	DECLARE_MESSAGE_MAP()
public:
	//Dialgoue Box controls
	CComboBox m_modelComboBox;
	CComboBox m_textureComboBox;

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

	void RetrieveData(std::vector<SceneObject>* SceneGraph);

	afx_msg void OnBnClickedOk();

	bool RebuildScene;
};



