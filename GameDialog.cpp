// GameDialog.cpp: файл реализации
//

#include "pch.h"
#include "tic_tac_toe.h"
#include "afxdialogex.h"
#include "GameDialog.h"
using namespace std;

// Диалоговое окно GameDialog

IMPLEMENT_DYNAMIC(GameDialog, CDialogEx)

GameDialog::GameDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME, pParent)
{

}

GameDialog::~GameDialog()
{
}

void GameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_PICURE, drw);
}


BEGIN_MESSAGE_MAP(GameDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &GameDialog::OnBnClickedOk)
	ON_MESSAGE(MS_DETERMINEWINNER, &GameDialog::OnMsDeterminewinner)
	ON_MESSAGE(MS_DETERMINEWINNERONBOTTURN, &GameDialog::OnMsDeterminewinnerOnBotTurn)
	ON_MESSAGE(MS_BOTTURN, &GameDialog::OnMsBotturn)
	ON_MESSAGE(MS_DONEURONTURN, &GameDialog::OnMsDoBotTurn)
END_MESSAGE_MAP()


// Обработчики сообщений GameDialog





void GameDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	data = vector<int>(9, 0);
	zeros = 9;
	drw.stop = false;
	drw.turn = true;
	drw.init = true;
	if (gm == evp)
	{
		drw.turn = false;
		data[4] = 1;
		zeros--;
	}
	if (gm == nvp)
	{
		drw.turn = false;
		data[4] = 1;
		zeros--;
	}
	
	drw.Invalidate();
}


BOOL GameDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	drw.m_data = &data;
	drw.parent = GetSafeHwnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


//int FindWinner(vector<int>& data, int zeros)
//{
//	if ((data[0] == data[1]) && (data[0] == data[2]) && (data[0] != 0))return data[0];
//	if ((data[3] == data[4]) && (data[3] == data[5]) && (data[3] != 0))return data[3];
//	if ((data[6] == data[7]) && (data[6] == data[8]) && (data[6] != 0))return data[6];
//
//	if ((data[0] == data[3]) && (data[0] == data[6]) && (data[0] != 0))return data[0];
//	if ((data[1] == data[4]) && (data[1] == data[7]) && (data[1] != 0))return data[1];
//	if ((data[2] == data[5]) && (data[2] == data[8]) && (data[2] != 0))return data[2];
//
//	if ((data[0] == data[4]) && (data[0] == data[8]) && (data[0] != 0))return data[0];
//	if ((data[2] == data[4]) && (data[2] == data[6]) && (data[2] != 0))return data[2];
//
//	if (zeros == 0)return DRAW;
//	return 0;
//}

afx_msg LRESULT GameDialog::OnMsDeterminewinner(WPARAM wParam, LPARAM lParam)
{
	zeros--;
	int res = FindWinner(data, zeros);
	if (res == 0)
	{
		if((gm == pve) || (gm == evp))PostMessage(MS_BOTTURN);
		if ((gm == pvn) || (gm == nvp))PostMessage(MS_DONEURONTURN);
		return 0;
	}

	if (res == DRAW)MessageBox(L"Ничья.", L"Результат", MB_OK);
	else
	{
		drw.stop = true;
		if (res == 1)MessageBox(L"Победил игрок 1.", L"Результат", MB_OK);
		else MessageBox(L"Победил игрок 2.", L"Результат", MB_OK);
	}
	return 0;
}
afx_msg LRESULT GameDialog::OnMsDeterminewinnerOnBotTurn(WPARAM wParam, LPARAM lParam)
{
	zeros--;
	int res = FindWinner(data, zeros);
	if (res == 0)
	{
		return 0;
	}

	if (res == DRAW)MessageBox(L"Ничья.", L"Результат", MB_OK);
	else
	{
		drw.stop = true;
		if (res == 1)MessageBox(L"Победил игрок 1.", L"Результат", MB_OK);
		else MessageBox(L"Победил игрок 2.", L"Результат", MB_OK);
	}
	return 0;
}


int Min(vector<int>& p)
{
	int res = 0;
	bool move = true;
	if (p[0] != INT_MAX)move = false;

	for (int i = 1; i < p.size(); i++)
	{
		if (p[i] == INT_MAX)
		{
			if (move)res++;
			continue;
		}
		else
		{
			if (move)
			{
				res++;
				move = !move;
			}
		}
		if (p[res] > p[i])res = i;
	}
	return res;
}
int Max(vector<int>& p)
{
	int res = 0;
	bool move = true;
	if (p[0] != INT_MAX)move = false;
	for (int i = 1; i < p.size(); i++)
	{
		if (p[i] == INT_MAX)
		{
			if (move)res++;
			continue;
		}
		else
		{
			if (move)
			{
				res++;
				move = !move;
			}
		}
		if (p[res] < p[i])res = i;
	}
	return res;
}
int g_id = 0;
int minmax(std::vector<int> data, int zeroes, int turn, bool bot, int deep)
{
	int winner = FindWinner(data, zeroes);
	if (bot)
	{
		if (winner == 1)return 10 - deep;
		else
			if (winner == -1)return -10 + deep;
	}
	else
	{
		if (winner == -1)return 10 - deep;
		else
			if (winner == 1)return -10 + deep;
	}
	if (winner == DRAW)return deep;


	vector<int> properties(9, INT_MAX);
	for (int i = 0; i < 9; i++)
	{
		if (data[i] == 0)
		{
			vector<int> temp = data;
			temp[i] = turn;
			int minmaxres = minmax(temp, zeroes - 1, -turn, bot, deep + 1);
			properties[i] = minmaxres;
		}
	}

	if (((turn == 1) && (bot)) || ((turn == -1) && (bot == false)))
	{
		/*int min = Min(properties);
		if (properties[min] == -8)g_id = min;
		else g_id = Max(properties);*/
		g_id = Max(properties);
	}
	else
		if (((turn == -1) && (bot)) || ((turn == 1) && (bot == false)))
		{
			/*int max = Max(properties);
			if (properties[max] == 8)g_id = max;
			else g_id = Min(properties);*/
			g_id = Min(properties);
		}

	return properties[g_id];
}

afx_msg LRESULT GameDialog::OnMsBotturn(WPARAM wParam, LPARAM lParam)
{
	int val = 1;
	if (drw.turn == 0)val = -1;
	bool bot = true;
	if (gm == pve)bot = false;
	minmax(data, zeros, val, bot, 0);
	data[g_id] = val;
	drw.turn = !drw.turn;
	PostMessage(MS_DETERMINEWINNERONBOTTURN);
	return 0;
}


afx_msg LRESULT GameDialog::OnMsDoBotTurn(WPARAM wParam, LPARAM lParam)
{
	int val = 1;
	if (drw.turn == 0)val = -1;
	bool bot = true;
	if (gm == pve)bot = false;
	minmax(data, zeros, val, bot, 0);
	data[g_id] = val;
	drw.turn = !drw.turn;
	PostMessage(MS_DETERMINEWINNERONBOTTURN);
	return 0;
}