#include "CFloatEdit.h"


BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	//ON_EN_CHANGE(m_editControlBox, &CFloatEdit::Type)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if (nChar >= _T('0') && nChar <= _T('9' || nChar == _T('.') || nChar == VK_BACK)) {
		//Call CEdit On Char
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

void CFloatEdit::OnKillFocus(CWnd* cwnd)
{
	CEdit::OnKillFocus(cwnd);

	if (GetWindowTextLengthW() == 0) {
		SetWindowTextW(_T("0"));
	}
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
