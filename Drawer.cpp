#include"pch.h"
#include"Drawer.h"
using namespace Gdiplus;

Drawer::Drawer()
{
	GdiplusStartupInput si{  };
	GdiplusStartup(&token, &si, NULL);
}
Drawer::~Drawer()
{
	GdiplusShutdown(token);
}
void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	Graphics ToWindow(lpDrawItemStruct->hDC);
	width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;
	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);

	std::vector<int>& data = *m_data;
	if (data.size() != 9)return;

	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	gr.TranslateTransform(3, 3);

	Pen BackgroundPen(Color::Black, 3);
	double WidthStep = width / 3. - 2;
	double HeightStep = height / 3. - 2;
	for (int i = 0; i <= 3; i++)
	{
		PointF p1(i * WidthStep, 0);
		PointF p2(i * WidthStep, height - 6);
		gr.DrawLine(&BackgroundPen, p1, p2);
	}

	for (int i = 0; i <= 3; i++)
	{
		PointF p1(0, i* HeightStep);
		PointF p2(width - 6, i* HeightStep);
		gr.DrawLine(&BackgroundPen, p1, p2);
	}

	Pen BluePen(Color::Blue, 2);
	Pen RedPen(Color::Red, 2);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (data[i * 3 + j] == 0)continue;
			if (data[i * 3 + j] == 1)
			{
				PointF p1((j + 0.1) * WidthStep, (i + 0.1) * HeightStep);
				PointF p2((j + 0.9) * WidthStep, (i + 0.9) * HeightStep);
				gr.DrawLine(&BluePen, p1, p2);

				PointF p3((j + 0.9) * WidthStep, (i + 0.1) * HeightStep);
				PointF p4((j + 0.1) * WidthStep, (i + 0.9) * HeightStep);
				gr.DrawLine(&BluePen, p3, p4);
			}
			else
			{
				PointF p1((j + 0.1) * WidthStep, (i + 0.1) * HeightStep);
				gr.DrawEllipse(&RedPen, p1.X, p1.Y, 0.8 * WidthStep, 0.8 * HeightStep);
			}
		}
	}

	BluePen.SetWidth(3);
	BluePen.SetColor(Color::BlueViolet);

	RedPen.SetWidth(3);
	RedPen.SetColor(Color::IndianRed);

	if ((data[0] == data[1]) && (data[0] == data[2]) && (data[0] != 0))
	{
		PointF p1(0, 0.5 * HeightStep);
		PointF p2(width - 6, 0.5 * HeightStep);
		if (data[0] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((data[3] == data[4]) && (data[3] == data[5]) && (data[3] != 0))
	{
		PointF p1(0, 1.5 * HeightStep);
		PointF p2(width - 6, 1.5 * HeightStep);
		if (data[3] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((data[6] == data[7]) && (data[6] == data[8]) && (data[6] != 0))
	{
		PointF p1(0, 2.5 * HeightStep);
		PointF p2(width - 6, 2.5 * HeightStep);
		if (data[6] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}

	if ((data[0] == data[3]) && (data[0] == data[6]) && (data[0] != 0))
	{
		PointF p1(0.5 * WidthStep, 0);
		PointF p2(0.5 * WidthStep, height - 6);
		if (data[0] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((data[1] == data[4]) && (data[1] == data[7]) && (data[1] != 0))
	{
		PointF p1(1.5 * WidthStep, 0);
		PointF p2(1.5 * WidthStep, height - 6);
		if (data[1] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((data[2] == data[5]) && (data[2] == data[8]) && (data[2] != 0))
	{
		PointF p1(2.5 * WidthStep, 0);
		PointF p2(2.5 * WidthStep, height - 6);
		if (data[2] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}

	if ((data[0] == data[4]) && (data[0] == data[8]) && (data[0] != 0))
	{
		PointF p1(0, 0);
		PointF p2(width - 6, height - 6);
		if (data[0] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}
	if ((data[2] == data[4]) && (data[2] == data[6]) && (data[2] != 0))
	{
		PointF p1(width - 6, 0);
		PointF p2(0, height - 6);
		if (data[2] == 1)gr.DrawLine(&BluePen, p1, p2);
		else gr.DrawLine(&RedPen, p1, p2);
	}


	ToWindow.DrawImage(&bmp, 0, 0);
}



BEGIN_MESSAGE_MAP(Drawer, CStatic)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()




void Drawer::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if(!stop)
	{
		double WidthStep = width / 3;
		double HeightStep = height / 3;
		int i = point.y / HeightStep;
		int j = point.x / WidthStep;

		std::vector<int>& data = *m_data;
		if (data[i * 3 + j] == 0)
		{
			if (turn)data[i * 3 + j] = 1;
			else data[i * 3 + j] = -1;
			turn = !turn;
		}
		Invalidate();
		PostMessageA(parent, MS_DETERMINEWINNER, NULL, NULL);
	}
	CStatic::OnRButtonUp(nFlags, point);
}
