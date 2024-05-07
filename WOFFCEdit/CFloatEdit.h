#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>

//CFloat Edit used for float specific edit boxes
class CFloatEdit : public CEdit
{
public:
	CFloatEdit();
	~CFloatEdit();

	float GetFloatValue();
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd*);
private:
	
UINT m_editControlBox;
bool hasPeriod;
};

