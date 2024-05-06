#include "CreateObjectDialogue.h"

IMPLEMENT_DYNAMIC(CreateObjectDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(CreateObjectDialogue, CDialogEx)
	//ok button
	ON_BN_CLICKED(IDCREATE, &CreateObjectDialogue::OnBnClickedOk)

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
	//Finds folders with cmo files
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
		{	//Gets name of file
			CString name = finder.GetFileName();
			//Adds model name to combo box
			m_modelComboBox.AddString(name);
			//Pushes model path to vector
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
			//Gets name of file
			CString name = finder.GetFileName();
			//Adds text name to combo box
			m_textureComboBox.AddString(name);
			//Stores texture path in vector
			m_texturePaths.push_back("database/data/" + name);
		}
	}

	//Sets the currently selected item for the combo boxes
	m_modelComboBox.SetCurSel(0);
	m_textureComboBox.SetCurSel(0);
	
	//Sets the inital state for the edit control boxes
	m_idNumEdit.SetLimitText(7);
	m_idNumEdit.SetWindowTextW(_T("0"));
	m_scaleXEdit.SetLimitText(7);
	m_scaleXEdit.SetWindowTextW(_T("1"));
	m_scaleYEdit.SetLimitText(7);
	m_scaleYEdit.SetWindowTextW(_T("1"));
	m_scaleZEdit.SetLimitText(7);
	m_scaleZEdit.SetWindowTextW(_T("1"));
}

void CreateObjectDialogue::OnBnClickedOk()
{
	SceneObject NewSceneObject;
	CString text;

	//Sets up model for scene object
	m_modelComboBox.GetWindowTextW(text);
	text = "database/data/" + text;
	CT2CA pszConvertedAnsiString(text);
	NewSceneObject.model_path = pszConvertedAnsiString;

	//Sets up texture for scene object
	m_textureComboBox.GetWindowTextW(text);
	text = "database/data/" + text;
	CT2CA pszConvertedAnsiString2(text);
	NewSceneObject.tex_diffuse_path = pszConvertedAnsiString2;

	//Sets up id for scene object
	m_idNumEdit.GetWindowTextW(text);
	NewSceneObject.ID = _ttof(text);

	//Sets up scale for scene object
	m_scaleXEdit.GetWindowTextW(text);
	NewSceneObject.scaX = _ttof(text);
	m_scaleYEdit.GetWindowTextW(text);
	NewSceneObject.scaY = _ttof(text);
	m_scaleZEdit.GetWindowTextW(text);
	NewSceneObject.scaZ = _ttof(text);

	//Pushed new scene object to storage
	m_sceneGraph->push_back(NewSceneObject);

	//Check to rebuild scene
	RebuildScene = true;
}
