#include "TextureDialogue.h"

// SelectDialogue dialog

IMPLEMENT_DYNAMIC(TextureDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(TextureDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TextureDialogue::End)					//ok button
	ON_BN_CLICKED(IDAPPLY, &TextureDialogue::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &TextureDialogue::Select)	//listbox
END_MESSAGE_MAP()

TextureDialogue::TextureDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

TextureDialogue::TextureDialogue(CWnd* pParent)
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

TextureDialogue::~TextureDialogue()
{
}

void TextureDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
}

void TextureDialogue::End()
{
}

void TextureDialogue::Select()
{
	CT2CA pszConvertedAnsiString(m_texturePaths.at(m_listBox.GetCurSel()));
	m_currentTexturePath = pszConvertedAnsiString;
}

BOOL TextureDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return NULL;
}

void TextureDialogue::PostNcDestroy()
{
}

void TextureDialogue::FindTextures(std::vector<SceneObject>* SceneGraph, std::vector<int>* IDData)
{
	m_sceneGraph = SceneGraph;
	m_selectedObjectID = IDData;

	CFileFind finder;
	CFile file;
	//Finds folders with dds files
	BOOL bWorking = finder.FindFile(_T("database/data\\*"));
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
			m_listBox.AddString(name);
			m_texturePaths.push_back("database/data/" + name);
		}
	}
}


void TextureDialogue::DeSelect()
{
}

bool TextureDialogue::DeleteSelected()
{
	return false;
}

void TextureDialogue::OnBnClickedOk()
{
	if (m_currentTexturePath.empty()) return;

	for (int i = 0; i < m_selectedObjectID->size(); i++) {
		m_sceneGraph->at(m_selectedObjectID->at(i)).tex_diffuse_path = m_currentTexturePath;
	}
	RebuildScene = true;
}
