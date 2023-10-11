#pragma once
#include "afxdialogex.h"

#include"Drawer.h"
#include"Neuron.h"
// Диалоговое окно GameDialog
#define DRAW							1000
#define MS_BOTTURN						WM_USER + 2
#define MS_DETERMINEWINNERONBOTTURN		WM_USER + 3
#define MS_DONEURONTURN					WM_USER + 4
class GameDialog : public CDialogEx
{
	DECLARE_DYNAMIC(GameDialog)

public:
	GameDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~GameDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Drawer drw;
	std::vector<int>data;
	int zeros = 9;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	enum GameMode
	{
		pvp,
		pve,
		evp,
		pvn,
		nvp
	};
	GameMode gm;


	NW* P1 = nullptr;
	NW* P2 = nullptr;
protected:
	afx_msg LRESULT OnMsDeterminewinner(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsBotturn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsDoBotTurn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT GameDialog::OnMsDeterminewinnerOnBotTurn(WPARAM wParam, LPARAM lParam);
};


int FindWinner(std::vector<int>& data, int zeros);
int minmax(std::vector<int> data, int zeros, int turn, bool bot, int deep);