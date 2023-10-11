#pragma once
#include <afxwin.h>
#include<gdiplus.h>
#include<vector>

#define MS_DETERMINEWINNER WM_USER + 1

// в дате крестик - это 1, а -1 - это нолик
class Drawer :
	public CStatic
{
	ULONG_PTR token;
	int width = 0;
	int height = 0;
public:
	Drawer();
	~Drawer();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

	HWND parent;
	bool stop = false;
	bool turn = true;
	bool init = false;
	std::vector<int>* m_data;
	DECLARE_MESSAGE_MAP()
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};




