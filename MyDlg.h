﻿
// MyDlg.h: файл заголовка
//

#pragma once
#include"GameDialog.h"

// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIC_TAC_TOE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox GameMode;
	afx_msg void OnSelchangeGameMode();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

	NW* p1 = nullptr;
	NW* p2 = nullptr;
	CProgressCtrl ProgBar;
	afx_msg LRESULT OnMsIncrBar(WPARAM wParam, LPARAM lParam);

	HANDLE thr;
	CButton StarCtr;
	Trainer tr;
	CStatic ProgTxtCtr;
	afx_msg void OnBnClickedButton2();
};
