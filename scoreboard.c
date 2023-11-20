#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdint.h>

#include "scoreboard.h"
#include "paint.h"
#include "resource.h"
#include "config.h"
#include "dialog.h"

#pragma comment(lib, "winmm.lib")

/* Global States */
bool g_clock_running;
bool g_timeout_running;
HANDLE g_hTimer, g_hTimeout;

/* Static function declaration */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void deleteDesigns( ScoreboardDesign * design );
static void soundBuzzer( void );
static VOID CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
static VOID CALLBACK TimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
static void paintScoreboard(HWND hwnd);
static void userCharCommand(HWND hwnd, WPARAM wParam);
static void onResize(HWND hwnd);

/* Functions */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    MSG msg;
    HWND hwnd;

    WNDCLASSW wc = {0};
    wc.cbClsExtra    = sizeof(ScoreboardDesign*);
    wc.cbWndExtra    = sizeof(ScoreboardControl*);
    wc.lpszClassName = L"ScoreboardAppV3";
    wc.hInstance     = hInstance;
    wc.lpszMenuName  = NULL;
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

    int x_pixel = GetSystemMetrics(SM_CXSCREEN);
    int y_pixel = GetSystemMetrics(SM_CYSCREEN);

    RegisterClassW(&wc);
    hwnd = CreateWindowW(wc.lpszClassName, L"ScoreboardAppV3",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0, 0, x_pixel, y_pixel, 0, 0, hInstance, 0);

	if (hwnd == NULL)
	{
		return 0;
	}

	ScoreboardControl* scoreboard_control = (ScoreboardControl*)malloc(sizeof(ScoreboardControl));
	ScoreboardDesign* scoreboard_design = (ScoreboardDesign*)malloc(sizeof(ScoreboardDesign));

	loadConfigFileAndDefaults(scoreboard_control, scoreboard_design);
	sndPlaySound("resources/silent.wav", SND_FILENAME);

	//store window data
	SetWindowLongPtr(hwnd, 0, (LONG_PTR) scoreboard_control);
	//store class data
	SetClassLongPtr(hwnd, 0, (LONG_PTR) scoreboard_design);

	g_clock_running = false;
	g_timeout_running = false;

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) 
    {        
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	//writeToConfig(scoreboard_control);

	free(scoreboard_control);
	deleteDesigns(scoreboard_design);

    return (int) msg.wParam;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch(msg)
    {	
		case WM_PAINT:
			paintScoreboard(hwnd);
			break;

        case WM_KEYUP:
            userCharCommand(hwnd, wParam);
            break;
			
		case WM_SIZE:
			onResize(hwnd);
			break;

        case WM_DESTROY:
			if (g_clock_running)
			{
				KillTimer(hwnd, 1);
			}
			else if (g_timeout_running)
			{
				KillTimer(hwnd, 2);
			}
            PostQuitMessage(0);
            break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static void deleteDesigns( ScoreboardDesign * design )
{
	if (design->timerFont != NULL) {
		DeleteObject(design->timerFont);
	}

	if (design->teamFont != NULL) {
		DeleteObject(design->teamFont);
	}

	if (design->scoreFont != NULL) {
		DeleteObject(design->scoreFont);
	}

	if (design->periodFont != NULL) {
		DeleteObject(design->periodFont);
	}

	if (design->overtimeFont != NULL) {
		DeleteObject(design->overtimeFont);
	}

	if (design->smallFont != NULL) {
		DeleteObject(design->smallFont);
	}

	if (design->outlinePen != NULL) {
		DeleteObject(design->outlinePen);
	}

	if (design->backgroundBrush != NULL) {
		DeleteObject(design->fillBrush);
	}

	if (design->fillBrush != NULL) {
		DeleteObject(design->fillBrush);
	}

	if (design->fillHomeBrush != NULL) {
		DeleteObject(design->fillHomeBrush);
	}

	if (design->fillAwayBrush != NULL) {
		DeleteObject(design->fillAwayBrush);
	}
	
	if (design->logo != NULL) {
		DeleteObject(design->logo);
	}

	free(design);
}

static VOID CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	if (lpParameter == NULL) {
        // Handle error
        return;
    }
    HWND hwnd = (HWND)(intptr_t)lpParameter;

    ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);
	if (scoreboard_control->state.timer_current.decaseconds == 0)
	{
		if (scoreboard_control->state.timer_current.seconds == 0)
		{
			if (scoreboard_control->state.timer_current.minutes == 0)
			{
    			sndPlaySound("resources/beep_long.wav", SND_FILENAME | SND_ASYNC);
				DeleteTimerQueueTimer(NULL, g_hTimer, NULL);
				g_clock_running = false;
			}
			else
			{
				scoreboard_control->state.timer_current.minutes--;
				scoreboard_control->state.timer_current.seconds = 59;
				scoreboard_control->state.timer_current.decaseconds = 9;
			}
		}
		else
		{
			scoreboard_control->state.timer_current.seconds--;
			scoreboard_control->state.timer_current.decaseconds = 9;
		}
	}
	else
	{
		scoreboard_control->state.timer_current.decaseconds--;
	}

	// call the invalidate region for the timer
	if ( (scoreboard_control->state.timer_current.minutes == 0) || (scoreboard_control->state.timer_current.decaseconds == 9) )
	{
		scoreboard_control->paint.timer = true;
    	InvalidateRect(hwnd, NULL, TRUE);
	}
}

static VOID CALLBACK TimeoutCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	if (lpParameter == NULL) {
        // Handle error
        return;
    }
    HWND hwnd = (HWND)(intptr_t)lpParameter;

    ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);
	if (scoreboard_control->state.timeout_current.decaseconds == 0)
	{
		if (scoreboard_control->state.timeout_current.seconds == 0)
		{
			if (scoreboard_control->state.timeout_current.minutes == 0)
			{
    			sndPlaySound("resources/beep.wav", SND_FILENAME | SND_ASYNC);
				DeleteTimerQueueTimer(NULL, g_hTimeout, NULL);
				g_timeout_running = false;
				scoreboard_control->paint.timeoutFlicker = true;
				scoreboard_control->state.toggle = true;
    			InvalidateRect(hwnd, NULL, TRUE);
			}
			else
			{
				scoreboard_control->state.timeout_current.minutes--;
				scoreboard_control->state.timeout_current.seconds = 59;
				scoreboard_control->state.timeout_current.decaseconds = 9;
			}
		}
		else
		{
			scoreboard_control->state.timeout_current.seconds--;
			scoreboard_control->state.timeout_current.decaseconds = 9;
		}
	}
	else
	{
		scoreboard_control->state.timeout_current.decaseconds--;
		if (scoreboard_control->state.timeout_current.decaseconds == 7 || scoreboard_control->state.timeout_current.decaseconds == 2)
		{
			scoreboard_control->paint.timeoutFlicker = true;
			scoreboard_control->state.toggle = !scoreboard_control->state.toggle;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
	}

	// call the invalidate region for the timer
	if (scoreboard_control->state.timeout_current.decaseconds == 9)
	{
		scoreboard_control->paint.timerTimeout = true;
    	InvalidateRect(hwnd, NULL, TRUE);
	}
}

static void paintScoreboard(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);
	ScoreboardDesign* scoreboard_design = (ScoreboardDesign*)GetClassLongPtr(hwnd, 0);
	
	// ordered by frequency to reduce overhead
	if (scoreboard_control->paint.timer)
	{
		paintTimer(scoreboard_design, hdc, hwnd, scoreboard_control->state.timer_current.minutes, 
			scoreboard_control->state.timer_current.seconds, scoreboard_control->state.timer_current.decaseconds);
		scoreboard_control->paint.timer = false;
	}
	else if (scoreboard_control->paint.homeScore)
	{
		if (scoreboard_control->state.switchState == 0) // home left side
		{
			paintLeftScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.homeScore);
		}
		else // home right side
		{
			paintRightScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.homeScore);
		}
		scoreboard_control->paint.homeScore = false;
	}
	else if (scoreboard_control->paint.awayScore)
	{
		if (scoreboard_control->state.switchState == 0) // away right side
		{
			paintRightScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.awayScore);
		}
		else // away left side
		{
			paintLeftScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.awayScore);
		}
		scoreboard_control->paint.awayScore = false;
	}
	else if (scoreboard_control->paint.timeoutFlicker)
	{
		if (scoreboard_control->state.timeoutCaller == 0) // paint left side
		{
			if (scoreboard_control->state.switchState == 0) // home is left side
			{
				paintActiveLeftTimeout(scoreboard_design, hdc, hwnd, true, 
					scoreboard_control->state.homeTimeouts, scoreboard_control->state.toggle);
			}
			else // away is left side
			{
				paintActiveLeftTimeout(scoreboard_design, hdc, hwnd, false, 
					scoreboard_control->state.awayTimeouts, scoreboard_control->state.toggle);
			}
		}
		else // paint right side
		{
			if (scoreboard_control->state.switchState == 0) // away is right side
			{
				paintActiveRightTimeout(scoreboard_design, hdc, hwnd, false, 
					scoreboard_control->state.awayTimeouts, scoreboard_control->state.toggle);
			}
			else // home is right side
			{
				paintActiveRightTimeout(scoreboard_design, hdc, hwnd, true, 
					scoreboard_control->state.homeTimeouts, scoreboard_control->state.toggle);
			}
		}
		scoreboard_control->paint.timeoutFlicker = false;
	}
	else if (scoreboard_control->paint.timerTimeout)
	{
		if (scoreboard_control->state.timeoutCaller == 0) // paint left side
		{
			paintLeftTimeoutTimer(scoreboard_design, hdc, hwnd, scoreboard_control->state.timeout_current.minutes, 
				scoreboard_control->state.timeout_current.seconds);
		}
		else // paint right side
		{
			paintRightTimeoutTimer(scoreboard_design, hdc, hwnd, scoreboard_control->state.timeout_current.minutes, 
				scoreboard_control->state.timeout_current.seconds);
		}
		scoreboard_control->paint.timerTimeout = false;
	}
	else if (scoreboard_control->paint.homeTimeouts)
	{
		bool home = true;
		if (scoreboard_control->state.switchState == 0) // home left side
		{
			paintLeftTimeouts(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.homeTimeouts);
		}
		else // home right side
		{
			paintRightTimeouts(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.homeTimeouts);
		}
		scoreboard_control->paint.homeTimeouts = false;
	}
	else if (scoreboard_control->paint.awayTimeouts)
	{
		bool home = false;
		if (scoreboard_control->state.switchState == 0) // away right side
		{
			paintRightTimeouts(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.awayTimeouts);
		}
		else // away left side
		{
			paintLeftTimeouts(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.awayTimeouts);
		}
		scoreboard_control->paint.awayTimeouts = false;
	}
	else if (scoreboard_control->paint.leftLogo)
	{
		if (scoreboard_control->state.switchState == 0) // home is left side
		{
			paintActiveLeftTimeout(scoreboard_design, hdc, hwnd, true, 
				scoreboard_control->state.homeTimeouts, true);
		}
		else // away is left side
		{
			paintActiveLeftTimeout(scoreboard_design, hdc, hwnd, true, 
				scoreboard_control->state.awayTimeouts, true);
		}
		paintLeftLogo(scoreboard_design, hdc, hwnd);
		scoreboard_control->paint.leftLogo = false;
	}
	else if (scoreboard_control->paint.rightLogo)
	{
		if (scoreboard_control->state.switchState == 0) // away is right side
		{
			paintActiveRightTimeout(scoreboard_design, hdc, hwnd, true, 
				scoreboard_control->state.awayTimeouts, true);
		}
		else // home is right side
		{
			paintActiveRightTimeout(scoreboard_design, hdc, hwnd, true, 
				scoreboard_control->state.homeTimeouts, true);
		}
		paintRightLogo(scoreboard_design, hdc, hwnd);
		scoreboard_control->paint.rightLogo = false;
	}
	else if (scoreboard_control->paint.period)
	{
		bool overtime;
		if (scoreboard_control->state.period > scoreboard_control->defaults.data.max_periods)
		{
			overtime = true;
			paintPeriod(scoreboard_design, hdc, hwnd, overtime, scoreboard_control->state.period);
			paintTimer(scoreboard_design, hdc, hwnd, scoreboard_control->state.timer_current.minutes, 
				scoreboard_control->state.timer_current.seconds, scoreboard_control->state.timer_current.decaseconds);
		}
		else
		{
			overtime = false;
			paintPeriod(scoreboard_design, hdc, hwnd, overtime, scoreboard_control->state.period);
			paintTimer(scoreboard_design, hdc, hwnd, scoreboard_control->state.timer_current.minutes, 
				scoreboard_control->state.timer_current.seconds, scoreboard_control->state.timer_current.decaseconds);
		}
		scoreboard_control->paint.period = false;
	}
	else if (scoreboard_control->paint.switches)
	{
		if (scoreboard_control->state.switchState == 0) // home left side, away right side
		{
			paintLeftName(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeName);
			paintRightName(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayName);
			paintLeftScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.homeScore);
			paintRightScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.awayScore);
			paintLeftTimeouts(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeTimeouts);
			paintRightTimeouts(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayTimeouts);
		}
		else // home right side, away left side
		{
			paintLeftName(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayName);
			paintRightName(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeName);
			paintLeftScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.awayScore);
			paintRightScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.homeScore);
			paintLeftTimeouts(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayTimeouts);
			paintRightTimeouts(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeTimeouts);
		}
		scoreboard_control->paint.switches = false;
	}
	else if (scoreboard_control->paint.homeTeam)
	{
		bool home = true;
		if (scoreboard_control->state.switchState == 0) // home left side
		{
			paintLeftName(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.homeName);
		}
		else // home right side
		{
			paintRightName(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.homeName);
		}
		scoreboard_control->paint.homeTeam = false;
	}
	else if (scoreboard_control->paint.awayTeam)
	{
		bool home = false;
		if (scoreboard_control->state.switchState == 0) // away right side
		{
			paintRightName(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.awayName);
		}
		else // away left side
		{
			paintLeftName(scoreboard_design, hdc, hwnd, home, scoreboard_control->state.awayName);
		}
		scoreboard_control->paint.awayTeam = false;
	}
	else if (scoreboard_control->paint.all)
	{
		paintLogoBackground(scoreboard_design, hdc, hwnd);
		paintTimer(scoreboard_design, hdc, hwnd, scoreboard_control->state.timer_current.minutes, 
			scoreboard_control->state.timer_current.seconds, scoreboard_control->state.timer_current.decaseconds);
		paintLeftLogo(scoreboard_design, hdc, hwnd);
		paintRightLogo(scoreboard_design, hdc, hwnd);
		if (scoreboard_control->state.switchState == 0) // home left side, away right side
		{
			paintLeftName(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeName);
			paintRightName(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayName);
			paintLeftScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.homeScore);
			paintRightScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.awayScore);
			paintLeftTimeouts(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeTimeouts);
			paintRightTimeouts(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayTimeouts);
		}
		else
		{
			paintLeftName(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayName);
			paintRightName(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeName);
			paintLeftScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.awayScore);
			paintRightScore(scoreboard_design, hdc, hwnd, scoreboard_control->state.homeScore);
			paintLeftTimeouts(scoreboard_design, hdc, hwnd, false, scoreboard_control->state.awayTimeouts);
			paintRightTimeouts(scoreboard_design, hdc, hwnd, true, scoreboard_control->state.homeTimeouts);
		}
		bool overtime;
		if (scoreboard_control->state.period > scoreboard_control->defaults.data.max_periods)
		{
			overtime = true;
			paintPeriod(scoreboard_design, hdc, hwnd, overtime, scoreboard_control->state.period);
		}
		else
		{
			overtime = false;
			paintPeriod(scoreboard_design, hdc, hwnd, overtime, scoreboard_control->state.period);
		}
		scoreboard_control->paint.all = false;
	}
	EndPaint(hwnd, &ps);
}

static void userCharCommand(HWND hwnd, WPARAM wParam)
{
    ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);

	// checks for if/else ordered by expected frequency to reduce checks
	if (wParam == scoreboard_control->defaults.keycodes.rightPlus2PtKey)
	{
		if (scoreboard_control->state.switchState == 0) // home left, away right
		{
			scoreboard_control->state.awayScore+=2;
			scoreboard_control->paint.awayScore = true;
		}
		else // home right, away left
		{
			scoreboard_control->state.homeScore+=2;
			scoreboard_control->paint.homeScore = true;
		}
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if (wParam == scoreboard_control->defaults.keycodes.leftPlus2PtKey)
	{
		if (scoreboard_control->state.switchState == 0) // home left, away right
		{
			scoreboard_control->state.homeScore+=2;
			scoreboard_control->paint.homeScore = true;
		}
		else // home right, away left
		{
			scoreboard_control->state.awayScore+=2;
			scoreboard_control->paint.awayScore = true;
		}
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if (wParam == scoreboard_control->defaults.keycodes.rightPlus3PtKey)
	{
		if (scoreboard_control->state.switchState == 0) // home left, away right
		{
			scoreboard_control->state.awayScore+=3;
			scoreboard_control->paint.awayScore = true;
		}
		else // home right, away left
		{
			scoreboard_control->state.homeScore+=3;
			scoreboard_control->paint.homeScore = true;
		}
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if (wParam == scoreboard_control->defaults.keycodes.leftPlus3PtKey)
	{
		if (scoreboard_control->state.switchState == 0) // home left, away right
		{
			scoreboard_control->state.homeScore+=3;
			scoreboard_control->paint.homeScore = true;
		}
		else // home right, away left
		{
			scoreboard_control->state.awayScore+=3;
			scoreboard_control->paint.awayScore = true;
		}
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if (wParam == scoreboard_control->defaults.keycodes.startTimeKey)
	{
		if (!g_timeout_running)
		{
			if(!g_clock_running)
			{
				if (!CreateTimerQueueTimer(&g_hTimer, NULL, TimerCallback, (PVOID)(intptr_t)hwnd, 100, 100, 0)) {
					// Handle error
					return;
				}
				g_clock_running = true;

				if (scoreboard_control->state.timeoutCaller == 0)
					scoreboard_control->paint.leftLogo = true;
				else
					scoreboard_control->paint.rightLogo = true;
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
	}
	else if (wParam == scoreboard_control->defaults.keycodes.stopTimeKey)
	{
		if (!g_timeout_running)
		{
			if(g_clock_running)
			{
				DeleteTimerQueueTimer(NULL, g_hTimer, NULL);
				g_clock_running = false;
			}
		}
	}
	else if (wParam == scoreboard_control->defaults.keycodes.rightMinusPtKey)
	{
		if (scoreboard_control->state.switchState == 0) // home left, away right
		{
			scoreboard_control->state.awayScore--;
			scoreboard_control->paint.awayScore = true;
		}
		else // home right, away left
		{
			scoreboard_control->state.homeScore--;
			scoreboard_control->paint.homeScore = true;
		}
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if (wParam == scoreboard_control->defaults.keycodes.leftMinusPtKey)
	{
		if (scoreboard_control->state.switchState == 0) // home left, away right
		{
			scoreboard_control->state.homeScore--;
			scoreboard_control->paint.homeScore = true;
		}
		else // home right, away left
		{
			scoreboard_control->state.awayScore--;
			scoreboard_control->paint.awayScore = true;
		}
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if (wParam == scoreboard_control->defaults.keycodes.periodPlusKey)
	{
		if(!g_clock_running && !g_timeout_running)
		{
			if (scoreboard_control->state.period <= scoreboard_control->defaults.data.max_periods)
			{
				scoreboard_control->state.period++;
				scoreboard_control->paint.period = true;
				
				if (scoreboard_control->state.period > scoreboard_control->defaults.data.max_periods)
					scoreboard_control->state.timer_current = scoreboard_control->defaults.data.ot_default;
				else
					scoreboard_control->state.timer_current = scoreboard_control->defaults.data.timer_default;

				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
	}
	else if (wParam == scoreboard_control->defaults.keycodes.periodMinusKey)
	{
		if(!g_clock_running && !g_timeout_running)
		{
			if (scoreboard_control->state.period > 1)
			{
				scoreboard_control->state.period--;
				scoreboard_control->state.timer_current = scoreboard_control->defaults.data.timer_default;
				scoreboard_control->paint.period = true;
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
	}
	else if (wParam == scoreboard_control->defaults.keycodes.rightTimeoutKey)
	{
		if (!g_timeout_running)
		{
			if (scoreboard_control->state.switchState == 0) // home left, away right
			{
				if (scoreboard_control->state.awayTimeouts > 0)
				{
					scoreboard_control->state.awayTimeouts--;
					scoreboard_control->paint.awayTimeouts = true;

					scoreboard_control->state.timeoutCaller = 1;
					scoreboard_control->state.timeout_current = scoreboard_control->defaults.data.timeout_default;

					if(g_clock_running)
					{
						DeleteTimerQueueTimer(NULL, g_hTimer, NULL);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					if (!CreateTimerQueueTimer(&g_hTimeout, NULL, TimeoutCallback, (PVOID)(intptr_t)hwnd, 100, 100, 0)) {
						// Handle error
						return;
					}
				}
			}
			else // home right, away left
			{
				if (scoreboard_control->state.homeTimeouts > 0)
				{
					scoreboard_control->state.homeTimeouts--;
					scoreboard_control->paint.homeTimeouts = true;

					scoreboard_control->state.timeoutCaller = 1;
					scoreboard_control->state.timeout_current = scoreboard_control->defaults.data.timeout_default;

					if(g_clock_running)
					{
						DeleteTimerQueueTimer(NULL, g_hTimer, NULL);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					if (!CreateTimerQueueTimer(&g_hTimeout, NULL, TimeoutCallback, (PVOID)(intptr_t)hwnd, 100, 100, 0)) {
						// Handle error
						return;
					}
				}
			}
		}
	}
	else if (wParam == scoreboard_control->defaults.keycodes.leftTimeoutKey)
	{
		if (!g_timeout_running)
		{
			if (scoreboard_control->state.switchState == 0) // home left, away right
			{
				if (scoreboard_control->state.homeTimeouts > 0)
				{
					scoreboard_control->state.homeTimeouts--;
					scoreboard_control->paint.homeTimeouts = true;

					scoreboard_control->state.timeoutCaller = 0;
					scoreboard_control->state.timeout_current = scoreboard_control->defaults.data.timeout_default;

					if(g_clock_running)
					{
						DeleteTimerQueueTimer(NULL, g_hTimer, NULL);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					if (!CreateTimerQueueTimer(&g_hTimeout, NULL, TimeoutCallback, (PVOID)(intptr_t)hwnd, 100, 100, 0)) {
						// Handle error
						return;
					}
				}
			}
			else // home right, away left
			{
				if (scoreboard_control->state.awayTimeouts > 0)
				{
					scoreboard_control->state.awayTimeouts--;
					scoreboard_control->paint.awayTimeouts = true;

					scoreboard_control->state.timeoutCaller = 0;
					scoreboard_control->state.timeout_current = scoreboard_control->defaults.data.timeout_default;

					if(g_clock_running)
					{
						DeleteTimerQueueTimer(NULL, g_hTimer, NULL);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					if (!CreateTimerQueueTimer(&g_hTimeout, NULL, TimeoutCallback, (PVOID)(intptr_t)hwnd, 100, 100, 0)) {
						// Handle error
						return;
					}
				}
			}
		}
	}
	else if(wParam == scoreboard_control->defaults.keycodes.timeoutClearKey)
	{
		if (g_timeout_running)
		{
			
    		sndPlaySound("resources/beep.wav", SND_FILENAME | SND_ASYNC);
			DeleteTimerQueueTimer(NULL, g_hTimeout, NULL);
			g_timeout_running = false;
			scoreboard_control->state.toggle = false;

			if (scoreboard_control->state.timeoutCaller == 0)
				scoreboard_control->paint.leftLogo = true;
			else
				scoreboard_control->paint.rightLogo = true;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (!g_clock_running)
		{
			if (scoreboard_control->state.timeoutCaller == 0)
				scoreboard_control->paint.leftLogo = true;
			else
				scoreboard_control->paint.rightLogo = true;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	else if(wParam == scoreboard_control->defaults.keycodes.switchKey)
	{
		if(!g_clock_running && !g_timeout_running)
		{
			if (scoreboard_control->state.switchState == 0)
			{
				scoreboard_control->state.switchState = 1;
				scoreboard_control->paint.switches = true;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			else
			{
				scoreboard_control->state.switchState = 0;
				scoreboard_control->paint.switches = true;
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
	}
	else if(wParam == scoreboard_control->defaults.keycodes.setKey)
	{
		if(!g_clock_running)
		{
			scoreboard_control->state.timer_current = scoreboard_control->defaults.data.timer_default;
			scoreboard_control->paint.timer = true;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	else if(wParam == scoreboard_control->defaults.keycodes.resetKey)
	{
		if(g_clock_running)
		{
			KillTimer(hwnd, 1);
			g_clock_running = false;
		}

		if (g_timeout_running)
		{
			KillTimer(hwnd, 2);
			g_timeout_running = false;
		}
		setDefaults(scoreboard_control);
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if(wParam == scoreboard_control->defaults.keycodes.configureKey)
	{
		if(!g_clock_running && !g_timeout_running)
		{
			configurationDialog(hwnd);
    		InvalidateRect(hwnd, NULL, TRUE);	
		}
	}
}

static void onResize(HWND hwnd)
{
	ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);
	if (scoreboard_control != NULL)
	{
		scoreboard_control->paint.all = true;
    	InvalidateRect(hwnd, NULL, TRUE);
	}
}
