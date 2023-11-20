#include <math.h>
#include "scoreboard.h"
#include "paint.h"

/* STATIC HELPER FUNCTIONS */
static int lcoord(float ratio, LONG left, LONG right)
{
	int retval;
	retval = left + floor(ratio * (right - left));

	return retval;
}

int rcoord(float ratio, LONG left, LONG right)
{
	int retval;
	retval = right - floor(ratio * (right - left));

	return retval;
}

int tcoord(float ratio, LONG top, LONG bottom)
{
	int retval;
	retval = top + floor(ratio * (bottom - top));

	return retval;
}

int bcoord(float ratio, LONG top, LONG bottom)
{
	int retval;
	retval = bottom - floor(ratio * (bottom - top));

	return retval;
}

static void roundRectCreate(HDC hdc, RECT * rect, float lratio, float rratio, float tratio, float bratio, RECT * clientRect, int ellipse)
{
	rect->left = lcoord(lratio, clientRect->left, clientRect->right);
	rect->right = rcoord(rratio, clientRect->left, clientRect->right);
	rect->top = tcoord(tratio,  clientRect->top, clientRect->bottom);
	rect->bottom = bcoord(bratio, clientRect->top, clientRect->bottom);

	RoundRect(hdc, rect->left, rect->top, rect->right, rect->bottom, ellipse, ellipse);
}

/* REGION PAINTING FUNCTIONS */
void paintTimer(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int minutes, int seconds, int decaseconds)
{
    // Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->fillBrush);
	SelectObject(hdc, scoreboard_design->outlinePen);
	SelectObject(hdc, scoreboard_design->timerFont);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.25, 0.25, 0.03055555, 0.65, &clientRect, 80);
	
	char timer_text[8];
	if (minutes > 0)
	{
		wsprintf(timer_text, "%02d:%02d", minutes, seconds);
		SetTextColor(hdc, RGB(255, 255, 0));
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, timer_text, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		wsprintf(timer_text, ":%02d.%1d", seconds, decaseconds);
		SetTextColor(hdc, RGB(255, 0, 0));
		SetBkMode(hdc, TRANSPARENT);
		DrawText(hdc, timer_text, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void paintLeftTimeoutTimer( ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int minutes, int seconds )
{
	 // Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->fillBrush);
	SelectObject(hdc, scoreboard_design->outlinePen);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.025, 0.775, 0.03055555, 0.705, &clientRect, 40);
	
	SelectObject(hdc, scoreboard_design->periodFont);
	char timer_text[8];
	wsprintf(timer_text, "%01d:%02d", minutes, seconds);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, timer_text, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, scoreboard_design->smallFont);
	const char * to_text = "TIMEOUT";
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, to_text, -1, &roundRect, DT_CENTER | DT_TOP | DT_SINGLELINE);
}

void paintRightTimeoutTimer( ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int minutes, int seconds )
{
	 // Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->fillBrush);
	SelectObject(hdc, scoreboard_design->outlinePen);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.775, 0.025, 0.03055555, 0.705, &clientRect, 40);
	
	SelectObject(hdc, scoreboard_design->periodFont);
	char timer_text[8];
	wsprintf(timer_text, "%01d:%02d", minutes, seconds);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, timer_text, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	SelectObject(hdc, scoreboard_design->smallFont);
	const char * to_text = "TIMEOUT";
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, to_text, -1, &roundRect, DT_CENTER | DT_TOP | DT_SINGLELINE);
}

void paintLeftScore(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int points)
{
    // Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->fillBrush);
	SelectObject(hdc, scoreboard_design->outlinePen);
	SelectObject(hdc, scoreboard_design->scoreFont);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.025, 0.575, 0.55694444, 0.03055555, &clientRect, 80);

	char score_text[8];
	wsprintf(score_text, "%d", points);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, score_text, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void paintRightScore(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int points)
{
    // Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->fillBrush);
	SelectObject(hdc, scoreboard_design->outlinePen);
	SelectObject(hdc, scoreboard_design->scoreFont);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect,0.575, 0.025, 0.55694444, 0.03055555, &clientRect, 80);

	char score_text[8];
	wsprintf(score_text, "%d", points);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, score_text, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void paintPeriod(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool overtime, int period)
{
    // Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->fillBrush);
	SelectObject(hdc, scoreboard_design->outlinePen);
	
	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.4453125, 0.4453125, 0.71944445, 0.0305555, &clientRect, 60);
	
	SelectObject(hdc, scoreboard_design->smallFont);
	const char * period_str = "PERIOD";
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, period_str, -1, &roundRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

	char num_str[8];
	if (overtime)
	{
		SelectObject(hdc, scoreboard_design->overtimeFont);
		wsprintf(num_str, "OT");
	}
	else
	{
		SelectObject(hdc, scoreboard_design->periodFont);
		wsprintf(num_str, "%d", period);
	}
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, num_str, -1, &roundRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void paintLeftName(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, const char * name)
{
    // Select the brush and pen into the device context
	if (home)
	{
		SelectObject(hdc, scoreboard_design->fillHomeBrush);
	}
	else
	{
		SelectObject(hdc, scoreboard_design->fillAwayBrush);
	}
	SelectObject(hdc, scoreboard_design->outlinePen);
	SelectObject(hdc, scoreboard_design->teamFont);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.025, 0.5124, 0.38055555, 0.47361, &clientRect, 40);

	char left[25];
	wsprintf(left, " %s", name);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, left, -1, &roundRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void paintRightName(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, const char * name)
{
    // Select the brush and pen into the device context
	if (home)
	{
		SelectObject(hdc, scoreboard_design->fillHomeBrush);
	}
	else
	{
		SelectObject(hdc, scoreboard_design->fillAwayBrush);
	}

	SelectObject(hdc, scoreboard_design->outlinePen);
	SelectObject(hdc, scoreboard_design->teamFont);

	RECT clientRect,roundRect;
	GetClientRect(hwnd, &clientRect);
	
	roundRectCreate(hdc, &roundRect, 0.5124, 0.025, 0.38055555, 0.47361, &clientRect, 40);
	
	char right[25];
	wsprintf(right, "%s ", name);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, right, -1, &roundRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

void paintLeftTimeouts(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts)
{
	RECT clientRect, roundRect;

	SelectObject(hdc, scoreboard_design->outlinePen);
	(home) ? SelectObject(hdc, scoreboard_design->fillHomeBrush) : SelectObject(hdc, scoreboard_design->fillAwayBrush);
	GetClientRect(hwnd, &clientRect);

	if(timeouts == 0)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	roundRectCreate(hdc, &roundRect, 0.025, 0.915, 0.32, 0.64, &clientRect, 20);

	if(timeouts == 1)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	roundRectCreate(hdc, &roundRect, 0.095, 0.845, 0.32, 0.64, &clientRect, 20);

	if(timeouts == 2)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	roundRectCreate(hdc, &roundRect, 0.165, 0.775, 0.32, 0.64, &clientRect, 20);
}

void paintRightTimeouts(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts)
{
	RECT clientRect, roundRect;

	SelectObject(hdc, scoreboard_design->outlinePen);
	(home) ? SelectObject(hdc, scoreboard_design->fillHomeBrush) : SelectObject(hdc, scoreboard_design->fillAwayBrush);
	GetClientRect(hwnd, &clientRect);

	if(timeouts == 0)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	roundRectCreate(hdc, &roundRect, 0.915, 0.025, 0.32, 0.64, &clientRect, 20);

	if(timeouts == 1)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	roundRectCreate(hdc, &roundRect, 0.845, 0.095, 0.32, 0.64, &clientRect, 20);

	if(timeouts == 2)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	roundRectCreate(hdc, &roundRect, 0.775, 0.165, 0.32, 0.64, &clientRect, 20);
}

void paintActiveLeftTimeout(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts, bool toggle)
{
	RECT clientRect, roundRect;

	SelectObject(hdc, scoreboard_design->outlinePen);
	GetClientRect(hwnd, &clientRect);

	if (toggle)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	else
		(home) ? SelectObject(hdc, scoreboard_design->fillHomeBrush) : SelectObject(hdc, scoreboard_design->fillAwayBrush);

	switch (timeouts)
	{
		case 0:
			roundRectCreate(hdc, &roundRect, 0.025, 0.915, 0.32, 0.64, &clientRect, 20);
			break;
		case 1:
			roundRectCreate(hdc, &roundRect, 0.095, 0.845, 0.32, 0.64, &clientRect, 20);
			break;
		case 2:
			roundRectCreate(hdc, &roundRect, 0.165, 0.775, 0.32, 0.64, &clientRect, 20);
			break;
		default:
			break;
	}
}

void paintActiveRightTimeout(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts, bool toggle)
{
	RECT clientRect, roundRect;

	SelectObject(hdc, scoreboard_design->outlinePen);
	GetClientRect(hwnd, &clientRect);

	if (toggle)
		SelectObject(hdc, scoreboard_design->backgroundBrush);
	else
		(home) ? SelectObject(hdc, scoreboard_design->fillHomeBrush) : SelectObject(hdc, scoreboard_design->fillAwayBrush);

	switch (timeouts)
	{
		case 0:
			roundRectCreate(hdc, &roundRect, 0.915, 0.025, 0.32, 0.64, &clientRect, 20);
			break;
		case 1:
			roundRectCreate(hdc, &roundRect, 0.845, 0.095, 0.32, 0.64, &clientRect, 20);
			break;
		case 2:
			roundRectCreate(hdc, &roundRect, 0.775, 0.165, 0.32, 0.64, &clientRect, 20);
			break;
		default:
			break;
	}
}

void paintLeftLogo(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd)
{
	// Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->backgroundBrush);

	RECT clientRect, bgRect;
	GetClientRect(hwnd, &clientRect);
	
	bgRect.left = clientRect.left;
	bgRect.right = floor(0.249 * (clientRect.right - clientRect.left));
	bgRect.bottom = floor(0.3* (clientRect.bottom - clientRect.top));
	bgRect.top = clientRect.top;

	//background
	FillRect(hdc, &bgRect, scoreboard_design->backgroundBrush);

	if (scoreboard_design->logoLeft != NULL)
	{
		BITMAP bmp;
		HDC hdcMem;
    	HGDIOBJ oldBitmap;

		hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, scoreboard_design->logoLeft);
		GetObject(scoreboard_design->logoLeft, sizeof(BITMAP), &bmp);
		
		int x = floor(0.025* (clientRect.right - clientRect.left));
		int y = floor(0.0305555* (clientRect.bottom - clientRect.top));
		int width = floor(0.1* (clientRect.right - clientRect.left));
		int height = floor(0.22 * (clientRect.bottom - clientRect.top));

		StretchBlt(hdc, x, y, width, height, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
	}
}

void paintRightLogo(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd)
{
	// Select the brush and pen into the device context
	SelectObject(hdc, scoreboard_design->backgroundBrush);

	RECT clientRect, bgRect;
	GetClientRect(hwnd, &clientRect);
	
	bgRect.left = floor(0.751 * (clientRect.right - clientRect.left));
	bgRect.right = clientRect.right;
	bgRect.bottom = floor(0.3* (clientRect.bottom - clientRect.top));
	bgRect.top = clientRect.top;

	FillRect(hdc, &bgRect, scoreboard_design->backgroundBrush);

	if (scoreboard_design->logoRight != NULL)
	{
		BITMAP bmp;
		HDC hdcMem;
    	HGDIOBJ oldBitmap;

		hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, scoreboard_design->logoRight);
		GetObject(scoreboard_design->logoRight, sizeof(BITMAP), &bmp);
		
		int x = floor(0.84 * (clientRect.right - clientRect.left));
		int y = floor(0.0305555* (clientRect.bottom - clientRect.top));
		int width = floor(0.135 * (clientRect.right - clientRect.left));
		int height = floor(0.21 * (clientRect.bottom - clientRect.top));

		StretchBlt(hdc, x, y, width, height, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
	}
}

void paintLogoBackground(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd)
{
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	SelectObject(hdc, scoreboard_design->backgroundBrush);

	FillRect(hdc, &clientRect, scoreboard_design->backgroundBrush);

	if (scoreboard_design->logo != NULL)
	{
		BITMAP bmp;
		HDC hdcMem;
    	HGDIOBJ oldBitmap;

		hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, scoreboard_design->logo);
		GetObject(scoreboard_design->logo, sizeof(BITMAP), &bmp);
		
		int x = floor(0.465* (clientRect.right - clientRect.left));
		int y = floor(0.56* (clientRect.bottom - clientRect.top));
		int width = floor(0.07* (clientRect.right - clientRect.left));
		int height = floor(0.125 * (clientRect.bottom - clientRect.top));

		StretchBlt(hdc, x, y, width, height, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
	}
}
