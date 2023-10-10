
// tic_tac_toe.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CtictactoeApp:
// Сведения о реализации этого класса: tic_tac_toe.cpp
//

class CtictactoeApp : public CWinApp
{
public:
	CtictactoeApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CtictactoeApp theApp;
