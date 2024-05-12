#include "CFloatEdit.h"


BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


CFloatEdit::CFloatEdit()
{

}

CFloatEdit::~CFloatEdit()
{
}

//Returns the float value of the edit box
float CFloatEdit::GetFloatValue()
{
	CString text;
	GetWindowTextW(text);
	return _ttof(text);;
}
//When a character is inputed into the edit box do this
void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//Used to accept only valid keys
	if (nChar >= _T('0') && nChar <= _T('9' || nChar == _T('.') || nChar == VK_BACK)) {
		//Call CEdit On Char
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	int cursorSlot;
	GetSel(cursorSlot, cursorSlot);
	//Only able to put a minus at the start of the edit box
	if (nChar == _T('-') && cursorSlot == 0) {
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}



#pragma region  OldMethod
	//CString editBoxString = "";
	////Sets up id for scene object
	//GetWindowTextW(editBoxString);
	////Store Previous Strng
	//CString previousString = editBoxString;
	//bool period = false;

	//int cursorLoc;
	//GetSel(cursorLoc, cursorLoc);

	//for (int i = 0; i < editBoxString.GetLength(); i++) {
	//	if (std::isdigit(editBoxString.GetAt(i))) {
	//		continue;
	//	}
	//	if (!editBoxString.GetAt(i) == '.') {
	//		editBoxString.Remove(i);
	//		cursorLoc--;
	//	}
	//	else if (editBoxString.GetAt(i) == '.' && period == false) {
	//		period = true;
	//	}
	//	else {
	//		editBoxString.Delete(i);
	//		cursorLoc--;
	//	}
	//}

	//if (previousString != editBoxString) {
	//	SetWindowTextW(editBoxString);
	//}
	//SetSel(cursorLoc, cursorLoc);
#pragma endregion

	
}

//Handles when the user exits the edit box
void CFloatEdit::OnKillFocus(CWnd* cwnd)
{
	CEdit::OnKillFocus(cwnd);
	//Checks if there is a value inside the box
	if (GetWindowTextLengthW() == 0) {
		SetWindowTextW(_T("0"));
	}
	//Checks to see if there is multiple periods in the box
	else {
		CString text;
		GetWindowTextW(text);
		int totalPeriods = 0;
		//Finds total amount of periods
		for (int i = 0; i < text.GetLength(); i++) {
			if (text.GetAt(i) == '.') totalPeriods++;
		}
		//If there is only 1 or less return
		if (totalPeriods < 2) return;

		//Loop through until deleted appropriate amount of periodss
		for (int i = text.GetLength()-1; i > -1; i--) {
			if (text.GetAt(i) == '.') {
				text.Delete(i);
				totalPeriods--;
				if (totalPeriods == 1) break;
			}
		}
		SetWindowTextW(text);
	}

}

#
