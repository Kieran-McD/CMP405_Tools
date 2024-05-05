#include "TextureDialogue.h"

// SelectDialogue dialog

IMPLEMENT_DYNAMIC(TextureDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(TextureDialogue, CDialogEx)
	ON_COMMAND(IDOK, &TextureDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &TextureDialogue::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &TextureDialogue::Select)	//listbox
END_MESSAGE_MAP()

TextureDialogue::TextureDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)
{
}

TextureDialogue::TextureDialogue(CWnd* pParent)
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
	for (int i = 0; i < m_sceneGraph->size(); i++) {
		CT2CA pszConvertedAnsiString(m_texturePaths.at(m_listBox.GetCurSel()));
		if (m_sceneGraph->at(i).m_selected) {
			m_sceneGraph->at(i).tex_diffuse_path = pszConvertedAnsiString;
		}		
	}
	RebuildScene = true;
}

BOOL TextureDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return NULL;
}

void TextureDialogue::PostNcDestroy()
{
}

void TextureDialogue::FindTextures(std::vector<SceneObject>* SceneGraph)
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

	//WIN32_FIND_DATA fileData;
	//memset(&fileData, 0, sizeof(WIN32_FIND_DATA));
	//HANDLE handle = FindFirstFile(_T("database/data\\*"), &fileData);
	//CStringArray strArray;

	//if (handle != INVALID_HANDLE_VALUE)
	//{
	//	do
	//	{
	//		if (_tcscmp(fileData.cFileName, _T(".")) != 0 && // ignore "." and ".."
	//			_tcscmp(fileData.cFileName, _T("..")) != 0)
	//		{
	//			strArray.Add(fileData.cFileName);
	//		}
	//	} while (FindNextFile(handle, &fileData));

	//	FindClose(handle);
	//}
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
}
