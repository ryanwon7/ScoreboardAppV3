#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include "scoreboard.h"
#include "paint.h"
#include "resource.h"

#pragma comment(lib, "winmm.lib")

/* Globals */
bool g_clock_running;
bool g_timeout_running;
int max_timeouts = 2;
int max_periods = 2;
HWND g_hDlg;

/* Static function declaration */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK SettingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static void loadConfigFileAndDefaults( ScoreboardControl* scoreboard_control, ScoreboardDesign* scoreboard_design);
static void deleteDesigns( ScoreboardDesign * design );
static void configurationDialog( HWND hwndParent );
static void soundBuzzer( void );
static void onClockTimer(HWND hwnd);
static void onTimeoutTimer(HWND hwnd);
static void onTimeoutTimer(HWND hwnd);
static void paintScoreboard(HWND hwnd);
static void userCharCommand(HWND hwnd, WPARAM wParam);
static void onResize(HWND hwnd);
static void reset( ScoreboardControl * scoreboard_control );

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

	free(scoreboard_control);
	deleteDesigns(scoreboard_design);

    return (int) msg.wParam;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch(msg)
    {
		case WM_TIMER:
			if (g_clock_running)
				onClockTimer(hwnd);
			else
				onTimeoutTimer(hwnd);
			break;
			
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

static INT_PTR CALLBACK SettingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // Handle OK button click
            // Retrieve and process settings here
            EndDialog(hwnd, IDOK);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            // Handle Cancel button click
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
        break;

    case WM_CLOSE:
        // Handle close button click
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }

    return FALSE;
}

static void loadConfigFileAndDefaults( ScoreboardControl* scoreboard_control, ScoreboardDesign* scoreboard_design)
{
	// for now im just going to populate with hardcoded defaults, shift to config file later
	scoreboard_control->keycodes.leftMinusPtKey = 97;
	scoreboard_control->keycodes.leftPlus2PtKey = 100;
	scoreboard_control->keycodes.leftPlus3PtKey = 103;
	scoreboard_control->keycodes.leftTimeoutKey = 27;
	scoreboard_control->keycodes.rightMinusPtKey = 99;
	scoreboard_control->keycodes.rightPlus2PtKey = 102;
	scoreboard_control->keycodes.rightPlus3PtKey = 105;
	scoreboard_control->keycodes.rightTimeoutKey = 106;
	scoreboard_control->keycodes.switchKey = 96;
	scoreboard_control->keycodes.resetKey = 46;
	scoreboard_control->keycodes.setKey = 98;
	scoreboard_control->keycodes.periodPlusKey = 104;
	scoreboard_control->keycodes.periodMinusKey = 101;
	scoreboard_control->keycodes.startTimeKey = 109;
	scoreboard_control->keycodes.stopTimeKey = 107;
	scoreboard_control->keycodes.timeoutClearKey = 111;
	scoreboard_control->keycodes.configureKey = 110; //todo

	scoreboard_control->paint.homeTeam = false;
	scoreboard_control->paint.awayTeam = false;
	scoreboard_control->paint.homeScore = false;
	scoreboard_control->paint.awayScore = false;
	scoreboard_control->paint.homeTimeouts = false;
	scoreboard_control->paint.awayTimeouts = false;
	scoreboard_control->paint.period = false;
	scoreboard_control->paint.timer = false;
	scoreboard_control->paint.timerTimeout = false;
	scoreboard_control->paint.timeoutFlicker = false;
	scoreboard_control->paint.leftLogo = false;
	scoreboard_control->paint.rightLogo = false;
	scoreboard_control->paint.switches = false;
	scoreboard_control->paint.all = true;

	scoreboard_control->state.switchState = 0;
	scoreboard_control->state.homeScore = 0;
	scoreboard_control->state.awayScore = 0;
	scoreboard_control->state.homeTimeouts = max_timeouts;
	scoreboard_control->state.awayTimeouts = max_timeouts;
	strcpy(scoreboard_control->state.homeName, "Honey Buns");
	strcpy(scoreboard_control->state.awayName, "Fish Taxi");
	scoreboard_control->state.period = 1;
	scoreboard_control->state.timeoutCaller = 0;
	scoreboard_control->state.toggle = false;
	scoreboard_control->state.timer_default.minutes = 12;
	scoreboard_control->state.timer_default.seconds = 30;
	scoreboard_control->state.timer_default.decaseconds = 0;
	scoreboard_control->state.timer_current.minutes = 12;
	scoreboard_control->state.timer_current.seconds = 30;
	scoreboard_control->state.timer_current.decaseconds = 0;
	scoreboard_control->state.timeout_default.minutes = 0;
	scoreboard_control->state.timeout_default.seconds = 30;
	scoreboard_control->state.timeout_default.decaseconds = 0;
	scoreboard_control->state.timeout_current.minutes = 0;
	scoreboard_control->state.timeout_current.seconds = 30;
	scoreboard_control->state.timeout_current.decaseconds = 0;

	scoreboard_design->timerFont = CreateFont(0.3 * GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->teamFont = CreateFont(0.11 * GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->scoreFont = CreateFont(0.43 * GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->periodFont = CreateFont(0.19 * GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->overtimeFont = CreateFont(0.14 * GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->smallFont = CreateFont(0.035 * GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->outlinePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
	scoreboard_design->backgroundBrush = CreateSolidBrush(RGB(60, 75, 40));
	scoreboard_design->fillBrush = CreateSolidBrush(RGB(39, 39, 39));
	scoreboard_design->fillHomeBrush = CreateSolidBrush(RGB(204, 0, 0));
	scoreboard_design->fillAwayBrush = CreateSolidBrush(RGB(255, 192, 0));

	scoreboard_design->logo = (HBITMAP)LoadImageW(NULL, L"resources/logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	scoreboard_design->logoLeft = (HBITMAP)LoadImageW(NULL, L"resources/JBA2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	scoreboard_design->logoRight = (HBITMAP)LoadImageW(NULL, L"resources/JBA.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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

static void configurationDialog(HWND hwndParent)
{
    // Create the settings dialog
    g_hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SETTINGS), hwndParent, SettingsDlgProc);

    // Show the settings dialog
    ShowWindow(g_hDlg, SW_SHOW);
}

static void onClockTimer(HWND hwnd)
{
	ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);
	if (scoreboard_control->state.timer_current.decaseconds == 0)
	{
		if (scoreboard_control->state.timer_current.seconds == 0)
		{
			if (scoreboard_control->state.timer_current.minutes == 0)
			{
    			sndPlaySound("resources/beep_long.wav", SND_FILENAME | SND_ASYNC);
				KillTimer(hwnd, 1);
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

static void onTimeoutTimer(HWND hwnd)
{
	ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwnd, 0);
	if (scoreboard_control->state.timeout_current.decaseconds == 0)
	{
		if (scoreboard_control->state.timeout_current.seconds == 0)
		{
			if (scoreboard_control->state.timeout_current.minutes == 0)
			{
    			sndPlaySound("resources/beep.wav", SND_FILENAME | SND_ASYNC);
				KillTimer(hwnd, 2);
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
		if (scoreboard_control->state.period > max_periods)
		{
			overtime = true;
			paintPeriod(scoreboard_design, hdc, hwnd, overtime, scoreboard_control->state.period);
		}
		else
		{
			overtime = false;
			paintPeriod(scoreboard_design, hdc, hwnd, overtime, scoreboard_control->state.period);
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
		if (scoreboard_control->state.period > max_periods)
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
	if (wParam == scoreboard_control->keycodes.rightPlus2PtKey)
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
	else if (wParam == scoreboard_control->keycodes.leftPlus2PtKey)
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
	else if (wParam == scoreboard_control->keycodes.rightPlus3PtKey)
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
	else if (wParam == scoreboard_control->keycodes.leftPlus3PtKey)
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
	else if (wParam == scoreboard_control->keycodes.startTimeKey)
	{
		if (!g_timeout_running)
		{
			if(!g_clock_running)
			{
				SetTimer(hwnd, 1, 100, NULL);
				g_clock_running = true;

				if (scoreboard_control->state.timeoutCaller == 0)
					scoreboard_control->paint.leftLogo = true;
				else
					scoreboard_control->paint.rightLogo = true;
				InvalidateRect(hwnd, NULL, TRUE);
			}
		}
	}
	else if (wParam == scoreboard_control->keycodes.stopTimeKey)
	{
		if (!g_timeout_running)
		{
			if(g_clock_running)
			{
				KillTimer(hwnd, 1);
				g_clock_running = false;
			}
		}
	}
	else if (wParam == scoreboard_control->keycodes.rightMinusPtKey)
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
	else if (wParam == scoreboard_control->keycodes.leftMinusPtKey)
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
	else if (wParam == scoreboard_control->keycodes.periodPlusKey)
	{
		if (scoreboard_control->state.period <= max_periods)
		{
			scoreboard_control->state.period++;
			scoreboard_control->paint.period = true;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	else if (wParam == scoreboard_control->keycodes.periodMinusKey)
	{
		if (scoreboard_control->state.period > 1)
		{
			scoreboard_control->state.period--;
			scoreboard_control->paint.period = true;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	else if (wParam == scoreboard_control->keycodes.rightTimeoutKey)
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
					scoreboard_control->state.timeout_current = scoreboard_control->state.timeout_default;

					if(g_clock_running)
					{
						KillTimer(hwnd, 1);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					SetTimer(hwnd, 2, 100, NULL);
				}
			}
			else // home right, away left
			{
				if (scoreboard_control->state.homeTimeouts > 0)
				{
					scoreboard_control->state.homeTimeouts--;
					scoreboard_control->paint.homeTimeouts = true;

					scoreboard_control->state.timeoutCaller = 1;
					scoreboard_control->state.timeout_current = scoreboard_control->state.timeout_default;

					if(g_clock_running)
					{
						KillTimer(hwnd, 1);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					SetTimer(hwnd, 2, 100, NULL);
				}
			}
		}
	}
	else if (wParam == scoreboard_control->keycodes.leftTimeoutKey)
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
					scoreboard_control->state.timeout_current = scoreboard_control->state.timeout_default;

					if(g_clock_running)
					{
						KillTimer(hwnd, 1);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					SetTimer(hwnd, 2, 100, NULL);
				}
			}
			else // home right, away left
			{
				if (scoreboard_control->state.awayTimeouts > 0)
				{
					scoreboard_control->state.awayTimeouts--;
					scoreboard_control->paint.awayTimeouts = true;

					scoreboard_control->state.timeoutCaller = 0;
					scoreboard_control->state.timeout_current = scoreboard_control->state.timeout_default;

					if(g_clock_running)
					{
						KillTimer(hwnd, 1);
						g_clock_running = false;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					g_timeout_running = true;
					SetTimer(hwnd, 2, 100, NULL);
				}
			}
		}
	}
	else if(wParam == scoreboard_control->keycodes.timeoutClearKey)
	{
		if (g_timeout_running)
		{
			
    		sndPlaySound("resources/beep.wav", SND_FILENAME | SND_ASYNC);
			KillTimer(hwnd, 2);
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
	else if(wParam == scoreboard_control->keycodes.switchKey)
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
	else if(wParam == scoreboard_control->keycodes.setKey)
	{
		if(!g_clock_running)
		{
			scoreboard_control->state.timer_current = scoreboard_control->state.timer_default;
			scoreboard_control->paint.timer = true;
    		InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	else if(wParam == scoreboard_control->keycodes.resetKey)
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
		reset(scoreboard_control);
    	InvalidateRect(hwnd, NULL, TRUE);
	}
	else if(wParam == scoreboard_control->keycodes.configureKey)
	{
		if(!g_clock_running && !g_timeout_running)
		{
			configurationDialog(hwnd);
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

static void reset( ScoreboardControl * scoreboard_control )
{
	scoreboard_control->paint.homeTeam = false;
	scoreboard_control->paint.awayTeam = false;
	scoreboard_control->paint.homeScore = false;
	scoreboard_control->paint.awayScore = false;
	scoreboard_control->paint.homeTimeouts = false;
	scoreboard_control->paint.awayTimeouts = false;
	scoreboard_control->paint.period = false;
	scoreboard_control->paint.timer = false;
	scoreboard_control->paint.timerTimeout = false;
	scoreboard_control->paint.timeoutFlicker = false;
	scoreboard_control->paint.leftLogo = false;
	scoreboard_control->paint.rightLogo = false;
	scoreboard_control->paint.switches = false;
	scoreboard_control->paint.all = true;

	scoreboard_control->state.switchState = 0;
	scoreboard_control->state.homeScore = 0;
	scoreboard_control->state.awayScore = 0;
	scoreboard_control->state.homeTimeouts = max_timeouts;
	scoreboard_control->state.awayTimeouts = max_timeouts;
	strcpy(scoreboard_control->state.homeName, "Honey Buns");
	strcpy(scoreboard_control->state.awayName, "Fish Taxi");
	scoreboard_control->state.period = 1;
	scoreboard_control->state.timeoutCaller = 0;
	scoreboard_control->state.toggle = false;
	scoreboard_control->state.timer_default.minutes = 12;
	scoreboard_control->state.timer_default.seconds = 30;
	scoreboard_control->state.timer_default.decaseconds = 0;
	scoreboard_control->state.timer_current.minutes = 0;
	scoreboard_control->state.timer_current.seconds = 6;
	scoreboard_control->state.timer_current.decaseconds = 0;
	scoreboard_control->state.timeout_default.minutes = 0;
	scoreboard_control->state.timeout_default.seconds = 30;
	scoreboard_control->state.timeout_default.decaseconds = 0;
	scoreboard_control->state.timeout_current.minutes = 0;
	scoreboard_control->state.timeout_current.seconds = 30;
	scoreboard_control->state.timeout_current.decaseconds = 0;
}
