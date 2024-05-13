#include "ModifyObjectDialogue.h"
#include "sstream"
IMPLEMENT_DYNAMIC(ModifyObjectDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(ModifyObjectDialogue, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, UpdateObjectScale)
	ON_EN_CHANGE(IDC_EDIT2, UpdateObjectScale)
	ON_EN_CHANGE(IDC_EDIT3, UpdateObjectScale)
	ON_EN_CHANGE(IDC_EDIT5, UpdateObjectPosition)
	ON_EN_CHANGE(IDC_EDIT6, UpdateObjectPosition)
	ON_EN_CHANGE(IDC_EDIT7, UpdateObjectPosition)
	ON_EN_CHANGE(IDC_EDIT8, UpdateObjectRotation)
	ON_EN_CHANGE(IDC_EDIT9, UpdateObjectRotation)
	ON_EN_CHANGE(IDC_EDIT10, UpdateObjectRotation)
	ON_CBN_SELCHANGE(IDC_COMBO1, UpdateSelectedObject)
	ON_CBN_SELCHANGE(IDC_COMBO2, UpdateObjectModel)
	ON_CBN_SELCHANGE(IDC_COMBO3, UpdateObjectTexture)
END_MESSAGE_MAP()

ModifyObjectDialogue::ModifyObjectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_sceneGraph = SceneGraph;
	m_currentSelection = -1;
}

ModifyObjectDialogue::ModifyObjectDialogue(CWnd* pParent)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_currentSelection = -1;
}

ModifyObjectDialogue::~ModifyObjectDialogue()
{
}

void ModifyObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_selectedObjectBox);
	DDX_Control(pDX, IDC_COMBO2, m_modelComboBox);
	DDX_Control(pDX, IDC_COMBO3, m_textureComboBox);
	DDX_Control(pDX, IDC_EDIT1, m_scaleXEdit);
	DDX_Control(pDX, IDC_EDIT2, m_scaleYEdit);
	DDX_Control(pDX, IDC_EDIT3, m_scaleZEdit);
	DDX_Control(pDX, IDC_EDIT4, m_idNumEdit);
	DDX_Control(pDX, IDC_EDIT5, m_posXEdit);
	DDX_Control(pDX, IDC_EDIT6, m_posYEdit);
	DDX_Control(pDX, IDC_EDIT7, m_posZEdit);
	DDX_Control(pDX, IDC_EDIT8, m_rotXEdit);
	DDX_Control(pDX, IDC_EDIT9, m_rotYEdit);
	DDX_Control(pDX, IDC_EDIT10, m_rotZEdit);
}

void ModifyObjectDialogue::End()
{

}

//Updates the objects position
void ModifyObjectDialogue::UpdateObjectPosition()
{
	if (m_currentSelection < 0) return;
	
	m_sceneGraph->at(m_currentSelection).posX = m_posXEdit.GetFloatValue();
	m_sceneGraph->at(m_currentSelection).posY = m_posYEdit.GetFloatValue();
	m_sceneGraph->at(m_currentSelection).posZ = m_posZEdit.GetFloatValue();


	RebuildScene = true;
}

//Updates the object scale
void ModifyObjectDialogue::UpdateObjectScale()
{
	if (m_currentSelection < 0) return;

	m_sceneGraph->at(m_currentSelection).scaX = m_scaleXEdit.GetFloatValue();
	m_sceneGraph->at(m_currentSelection).scaY = m_scaleYEdit.GetFloatValue();
	m_sceneGraph->at(m_currentSelection).scaZ = m_scaleZEdit.GetFloatValue();


	RebuildScene = true;
}

//Updates the rotation of the object
void ModifyObjectDialogue::UpdateObjectRotation()
{
	if (m_currentSelection < 0) return;

	m_sceneGraph->at(m_currentSelection).rotX = m_rotXEdit.GetFloatValue();
	m_sceneGraph->at(m_currentSelection).rotY = m_rotYEdit.GetFloatValue();
	m_sceneGraph->at(m_currentSelection).rotZ = m_rotZEdit.GetFloatValue();


	RebuildScene = true;
}

//Updates the objects models path
void ModifyObjectDialogue::UpdateObjectModel()
{
	if (m_currentSelection < 0) return;

	CString text;
	m_modelComboBox.GetWindowTextW(text);
	text = "database/data/" + text;
	CT2CA pszConvertedAnsiString(text);
	m_sceneGraph->at(m_currentSelection).model_path = pszConvertedAnsiString;
	RebuildScene = true;
}

//Updates the object texture
void ModifyObjectDialogue::UpdateObjectTexture()
{
	if (m_currentSelection < 0) return;

	CString text;
	m_textureComboBox.GetWindowTextW(text);
	text = "database/data/" + text;
	CT2CA pszConvertedAnsiString(text);
	m_sceneGraph->at(m_currentSelection).tex_diffuse_path = pszConvertedAnsiString;
	RebuildScene = true;
}

//Handles updates the edit/combo boxes to use the scene object data
void ModifyObjectDialogue::UpdateSelectedObject()
{
	int index = m_selectedObjectID->at(m_selectedObjectBox.GetCurSel());
	m_currentSelection = -1;
	CString text;

	text.Format(_T("%d"), m_sceneGraph->at(index).ID);
	m_idNumEdit.SetWindowTextW(text);	

	text.Format(_T("%f"), m_sceneGraph->at(index).posX);
	m_posXEdit.SetWindowTextW(text);
	text.Format(_T("%f"), m_sceneGraph->at(index).posY);
	m_posYEdit.SetWindowTextW(text);
	text.Format(_T("%f"), m_sceneGraph->at(index).posZ);
	m_posZEdit.SetWindowTextW(text);

	text.Format(_T("%f"), m_sceneGraph->at(index).scaX);
	m_scaleXEdit.SetWindowTextW(text);
	text.Format(_T("%f"), m_sceneGraph->at(index).scaY);
	m_scaleYEdit.SetWindowTextW(text);
	text.Format(_T("%f"), m_sceneGraph->at(index).scaZ);
	m_scaleZEdit.SetWindowTextW(text);

	text.Format(_T("%f"), m_sceneGraph->at(index).rotX);
	m_rotXEdit.SetWindowTextW(text);
	text.Format(_T("%f"), m_sceneGraph->at(index).rotY);
	m_rotYEdit.SetWindowTextW(text);
	text.Format(_T("%f"), m_sceneGraph->at(index).rotZ);
	m_rotZEdit.SetWindowTextW(text);

	


	m_currentSelection = index;
}

BOOL ModifyObjectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return NULL;
}

void ModifyObjectDialogue::PostNcDestroy()
{
}


void ModifyObjectDialogue::RetrieveData(std::vector<SceneObject>* SceneGraph, std::vector<int>* SelectedID)
{
	m_sceneGraph = SceneGraph;
	m_selectedObjectID = SelectedID;

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

	//Setups the scale edit boxes
	m_scaleXEdit.SetLimitText(7);
	m_scaleXEdit.SetWindowTextW(_T("1"));
	m_scaleYEdit.SetLimitText(7);
	m_scaleYEdit.SetWindowTextW(_T("1"));
	m_scaleZEdit.SetLimitText(7);
	m_scaleZEdit.SetWindowTextW(_T("1"));

	//Setups the position edit boxes
	m_posXEdit.SetLimitText(7);
	m_posXEdit.SetWindowTextW(_T("1"));
	m_posYEdit.SetLimitText(7);
	m_posYEdit.SetWindowTextW(_T("1"));
	m_posZEdit.SetLimitText(7);
	m_posZEdit.SetWindowTextW(_T("1"));

	//Setups the rotation edit boxes
	m_rotXEdit.SetLimitText(7);
	m_rotXEdit.SetWindowTextW(_T("1"));
	m_rotYEdit.SetLimitText(7);
	m_rotYEdit.SetWindowTextW(_T("1"));
	m_rotZEdit.SetLimitText(7);
	m_rotZEdit.SetWindowTextW(_T("1"));

	if (m_selectedObjectID->size() < 1) return;

	for (int i = 0; i < m_selectedObjectID->size(); i++) {
		CString text;
		text.Format(_T("%d"), m_sceneGraph->at(m_selectedObjectID->at(i)).ID);
		m_selectedObjectBox.AddString(text);
	}

	m_selectedObjectBox.SetCurSel(0);

	UpdateSelectedObject();
}

//Updares the listbox selected objects
void ModifyObjectDialogue::UpdateObjectList()
{
	m_selectedObjectBox.ResetContent();
	m_currentSelection = -1;
	for (int i = 0; i < m_selectedObjectID->size(); i++) {
		CString text;
		text.Format(_T("%d"), m_sceneGraph->at(m_selectedObjectID->at(i)).ID);
		m_selectedObjectBox.AddString(text);
	}
}

void ModifyObjectDialogue::UpdateParameters()
{
	int index = m_selectedObjectBox.GetCurSel();
	if (m_selectedObjectBox.GetCurSel() < 0) return;

	UpdateSelectedObject();
}

void ModifyObjectDialogue::UpdateDelete()
{
	m_currentSelection = -1;
	m_selectedObjectBox.ResetContent();

}

void ModifyObjectDialogue::OnBnClickedOk()
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

	NewSceneObject.posX = m_posXEdit.GetFloatValue();
	NewSceneObject.posY = m_posYEdit.GetFloatValue();
	NewSceneObject.posZ = m_posZEdit.GetFloatValue();

	NewSceneObject.scaX = m_scaleXEdit.GetFloatValue();
	NewSceneObject.scaY = m_scaleYEdit.GetFloatValue();
	NewSceneObject.scaZ = m_scaleZEdit.GetFloatValue();



	//Pushed new scene object to storage
	m_sceneGraph->push_back(NewSceneObject);

	//Check to rebuild scene
	RebuildScene = true;
}
