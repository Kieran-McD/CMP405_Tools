// SelectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SelectDialogue.h"

// SelectDialogue dialog

IMPLEMENT_DYNAMIC(SelectDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(SelectDialogue, CDialogEx)
	ON_COMMAND(IDOK, &SelectDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &SelectDialogue::OnBnClickedOk)		
	ON_LBN_SELCHANGE(IDC_LIST1, &SelectDialogue::Select)	//listbox
END_MESSAGE_MAP()


SelectDialogue::SelectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_DIALOG1, pParent)
{
	m_sceneGraph = SceneGraph;
}

SelectDialogue::SelectDialogue(CWnd * pParent)			//constructor used in modeless
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

SelectDialogue::~SelectDialogue()
{
	
}

///pass through pointers to the data in the tool we want to manipulate
void SelectDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, int * selection)
{
	m_sceneGraph = SceneGraph;
	m_currentSelection = selection;

	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (int i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}
}


void SelectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
}

void SelectDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

void SelectDialogue::Select()
{
	//m_listBox.SetSel(0, 1);
	m_listBox.SetCurSel(1);
	int currentTotalCells = 0;
	int currentCell = 0;
	for (int i = 0; i < m_listBox.GetCount()-1; i++) 
	{
		if (m_listBox.GetSel(i) > 0) {
			int index = i + 1;
			CString currentSelectionValue;

			m_listBox.GetText(currentTotalCells, currentSelectionValue);

			*m_currentSelection = _ttoi(currentSelectionValue);

			m_sceneGraph->at(index - 1).scaX = 1;
			m_sceneGraph->at(index - 1).scaY = 1;
			m_sceneGraph->at(index - 1).scaZ = 1;

			m_sceneGraph->at(index - 1).display_object->m_scale = DirectX::SimpleMath::Vector3(5, 5, 5);
			currentTotalCells++;
		}
		else {
			int index = i + 1;
			CString currentSelectionValue;

			m_listBox.GetText(index, currentSelectionValue);

			m_sceneGraph->at(index - 1).scaX = 1;
			m_sceneGraph->at(index - 1).scaY = 1;
			m_sceneGraph->at(index - 1).scaZ = 1;

			m_sceneGraph->at(index - 1).display_object->m_scale = DirectX::SimpleMath::Vector3(1, 1, 1);
		}
	}

	return;
	

	//m_sceneGraph->erase(m_sceneGraph->begin() + index);	
	//m_listBox.DeleteString(index);

	m_sceneGraph->at(*m_currentSelection - 5).scaX = 1;
	m_sceneGraph->at(*m_currentSelection - 5).scaY = 1;
	m_sceneGraph->at(*m_currentSelection - 5).scaZ = 1;

	m_sceneGraph->at(*m_currentSelection-1).display_object->m_scale = DirectX::SimpleMath::Vector3(5, 5, 5);

}

BOOL SelectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//uncomment for modal only
/*	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = m_sceneGraph->size();
	for (size_t i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.
		std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
		m_listBox.AddString(listBoxEntry.c_str());
	}*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SelectDialogue::PostNcDestroy()
{
}

void SelectDialogue::DeSelect()
{
	
	int currentTotalCells = 0;
	int currentCell = 0;
	int totalCells = m_listBox.GetSelCount();

	while (currentTotalCells < totalCells)
	{
		
		if (m_listBox.GetSel(currentCell) > 0) {
			m_listBox.SetSel(currentCell, 0);
			int index = currentCell;
			CString currentSelectionValue;

			m_listBox.GetText(index, currentSelectionValue);

			m_sceneGraph->at(index).scaX = 1;
			m_sceneGraph->at(index).scaY = 1;
			m_sceneGraph->at(index).scaZ = 1;

			m_sceneGraph->at(index).display_object->m_scale = DirectX::SimpleMath::Vector3(1, 1, 1);
			currentTotalCells++;
		}
		else {

		}

		currentCell++;
	}
}

void SelectDialogue::DeleteSelected()
{
	if (m_sceneGraph->size() == 1) return;
	int currentTotalCells = 0;
	int currentCell = 0;
	int totalCells = m_listBox.GetSelCount();
	std::vector<int> indexToRemove;
	//Loops through until it finds all selected cells
	while (currentTotalCells < totalCells)
	{
		if (m_listBox.GetSel(currentCell) > 0) {
			m_listBox.SetSel(currentCell, 0);
			int index = currentCell;
			CString currentSelectionValue;

			m_listBox.GetText(index, currentSelectionValue);

			indexToRemove.push_back(index);

			//m_sceneGraph->erase(m_sceneGraph->begin() + (index));	
			//m_listBox.DeleteString(index);

			currentTotalCells++;
		}
		else {

		}

		currentCell++;
	}
	for (int i = indexToRemove.size() - 1; i >= 0; i--) {
		if (m_sceneGraph->size() == 1) return;
		m_sceneGraph->erase(m_sceneGraph->begin() + (indexToRemove.at(i)));
		m_listBox.DeleteString(indexToRemove.at(i));
	}

}


// SelectDialogue message handlers callback   - We only need this if the dailogue is being setup-with createDialogue().  We are doing
//it manually so its better to use the messagemap
//INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{	
//	switch (uMsg)
//	{
//	case WM_COMMAND:
//		switch (LOWORD(wParam))
//		{
//		case IDOK:
//		//	EndDialog(hwndDlg, wParam);
//			DestroyWindow(hwndDlg);
//			return TRUE;
//			
//
//		case IDCANCEL:
//			EndDialog(hwndDlg, wParam);
//			return TRUE;
//			break;
//		}
//	}
//	
//	return INT_PTR();
//}


void SelectDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

