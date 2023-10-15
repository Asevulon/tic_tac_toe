
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "tic_tac_toe.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIC_TAC_TOE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, GameMode);
	DDX_Control(pDX, IDC_PROGRESS1, ProgBar);
	DDX_Control(pDX, IDOK, StarCtr);
	DDX_Control(pDX, IDC_PROGTXT, ProgTxtCtr);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &MyDlg::OnSelchangeGameMode)
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
	ON_MESSAGE(MS_INCRBAR, &MyDlg::OnMsIncrBar)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	GameMode.SetCurSel(GameMode.AddString(L"2 игрока"));
	GameMode.AddString(L"Игрок против Minmax");
	GameMode.AddString(L"Minmax против Игрока");
	GameMode.AddString(L"Игрок против Нейросети");
	GameMode.AddString(L"Нейросеть против Игрока");
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnSelchangeGameMode()
{
	// TODO: добавьте свой код обработчика уведомлений

}


void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = GameMode.GetCurSel();
	if (id == -1)return;

	CString str;
	GameMode.GetLBText(id, str);
	GameDialog gd;

	if (str == L"2 игрока")gd.gm = GameDialog::pvp;
	else
		if (str == L"Игрок против Minmax")gd.gm = GameDialog::pve;
		else
			if (str == L"Minmax против Игрока")gd.gm = GameDialog::evp;
			else
				if (str == L"Игрок против Нейросети")
				{
					gd.gm = GameDialog::pvn;
					if (p2 == nullptr)return;
					gd.P2 = p2;
				}
				else
					if (str == L"Нейросеть против Игрока")
					{
						gd.gm = GameDialog::nvp;
						if (p1 == nullptr)return;
						gd.P1 = p1;
					}
	gd.DoModal();
}

DWORD WINAPI ThreadFunc(LPVOID p)
{
	MyDlg* dlg = (MyDlg*)p;
	dlg->tr = Trainer();
	dlg->ProgBar.SetRange(0, dlg->tr.TrainLimit + 1);
	dlg->ProgBar.SetStep(1);
	dlg->ProgBar.SetPos(0);
	dlg->tr.parent = dlg->GetSafeHwnd();
	dlg->tr.train();
	dlg->tr.GetBest(dlg->p1, dlg->p2);
	dlg->StarCtr.EnableWindow(TRUE);
	return 0;
}
void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	StarCtr.EnableWindow(FALSE);

	TerminateThread(thr, 0);
	thr = CreateThread(NULL, NULL, ThreadFunc, this, NULL, NULL);

}

afx_msg LRESULT MyDlg::OnMsIncrBar(WPARAM wParam, LPARAM lParam)
{
	
	ProgBar.StepIt();
	CString str;
	pair<double, double>*avscr = (pair<double,double>*)lParam;
	str.Format(L"%d, %.2lf, %.2lf", ProgBar.GetPos(), avscr->first, avscr->second);
	ProgTxtCtr.SetWindowTextW(str);
	return 0;
}