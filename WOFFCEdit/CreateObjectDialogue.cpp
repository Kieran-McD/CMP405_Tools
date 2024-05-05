#include "CreateObjectDialogue.h"

IMPLEMENT_DYNAMIC(CreateObjectDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(CreateObjectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &CreateObjectDialogue::End)					//ok button
	ON_BN_CLICKED(IDCREATE, &CreateObjectDialogue::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_COMBO2, &CreateObjectDialogue::Select)	//listbox
END_MESSAGE_MAP()

CreateObjectDialogue::CreateObjectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	m_sceneGraph = SceneGraph;
}

CreateObjectDialogue::CreateObjectDialogue(CWnd* pParent)
	: CDialogEx(IDD_DIALOG3, pParent)
{
}

CreateObjectDialogue::~CreateObjectDialogue()
{
}

void CreateObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_modelComboBox);
	DDX_Control(pDX, IDC_COMBO3, m_textureComboBox);
	DDX_Control(pDX, IDC_EDIT1, m_scaleXEdit);
	DDX_Control(pDX, IDC_EDIT2, m_scaleYEdit);
	DDX_Control(pDX, IDC_EDIT3, m_scaleZEdit);
	DDX_Control(pDX, IDC_EDIT4, m_idNumEdit);
}

void CreateObjectDialogue::End()
{
}

void CreateObjectDialogue::Select()
{
}

BOOL CreateObjectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return NULL;
}

void CreateObjectDialogue::PostNcDestroy()
{
}


void CreateObjectDialogue::RetrieveData(std::vector<SceneObject>* SceneGraph)
{
	m_sceneGraph = SceneGraph;

	CFileFind finder;
	CFile file;
	//Finds folders with dds files
	BOOL bWorking = finder.FindFile(_T("database/data\\*"));
	while (bWorking)
	{
		//Gets next file
		bWorking = finder.FindNextFile();
		int f = finder.GetFileName().Find(_T(".cmo"));
		CString s = finder.GetFileName();
		//Checks if the current file is dds
		if (f < 0) {
			continue;
		}

		if (!finder.IsDots())
		{
			CString name = finder.GetFileName();
			m_modelComboBox.AddString(name);
			m_modelPath.push_back("database/data/" + name);
		}
	}

	//Finds folders with dds files
	bWorking = finder.FindFile(_T("database/data\\*"));
	while (bWorking)
	{
		//Gets next file
		bWorking = finder.FindNextFile();
		int f = finder.GetFileName().Find(_T(".dds"));
		CString s = finder.GetFileName();
		//Checks if the current file is dds
		if (f < 0) {
			continue;
		}

		if (!finder.IsDots())
		{
			CString name = finder.GetFileName();
			m_textureComboBox.AddString(name);
			m_texturePaths.push_back("database/data/" + name);
		}
	}

	m_modelComboBox.SetCurSel(0);
	m_textureComboBox.SetCurSel(0);


	m_idNumEdit.SetLimitText(5);
	m_idNumEdit.SetWindowTextW(_T("0"));

	m_scaleXEdit.SetLimitText(5);
	m_scaleXEdit.SetWindowTextW(_T("1"));
	m_scaleYEdit.SetLimitText(5);
	m_scaleYEdit.SetWindowTextW(_T("1"));
	m_scaleZEdit.SetLimitText(5);
	m_scaleZEdit.SetWindowTextW(_T("1"));
}

void CreateObjectDialogue::DeSelect()
{
}

bool CreateObjectDialogue::DeleteSelected()
{
	return false;
}

void CreateObjectDialogue::OnBnClickedOk()
{
	SceneObject NewSceneObject;
	CString text;
	m_modelComboBox.GetWindowTextW(text);

	text = "database/data/" + text;

	//m_modelComboBox.GetWindowTextW(m_modelPath.at(m_modelComboBox.GetCurSel()));

	

	CT2CA pszConvertedAnsiString(text);
	NewSceneObject.model_path = pszConvertedAnsiString;
	NewSceneObject.tex_diffuse_path = "database/data/placeholder.dds";

	m_textureComboBox.GetWindowTextW(text);
	text = "database/data/" + text;

	CT2CA pszConvertedAnsiString2(text);
	NewSceneObject.tex_diffuse_path = pszConvertedAnsiString2;


	m_idNumEdit.GetWindowTextW(text);
	NewSceneObject.ID = _ttof(text);

	m_scaleXEdit.GetWindowTextW(text);
	NewSceneObject.scaX = _ttof(text);
	m_scaleYEdit.GetWindowTextW(text);
	NewSceneObject.scaY = _ttof(text);
	m_scaleZEdit.GetWindowTextW(text);
	NewSceneObject.scaZ = _ttof(text);

	m_sceneGraph->push_back(NewSceneObject);
	RebuildScene = true;
}
