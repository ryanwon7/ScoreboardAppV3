#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include "dialog.h"

DialogData * user_data;

static void initUserData(HWND hwnd);
static void readUserData(HWND hwnd);
static void saveUserDataAndPaint(HWND hwnd);
static void setTabOne(HWND hwnd);
static void setTabTwo(HWND hwnd);
static INT_PTR CALLBACK SettingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

static void setTabOne(HWND hwnd)
{
    ShowWindow(GetDlgItem(hwnd, ID_TIME_TITLE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_TIME_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MIN_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MIN_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_SEC_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_SEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DSEC_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DSEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_TEAM_TITLE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_HM_NAME_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_HM_NAME_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_AW_NAME_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_AW_NAME_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_HM_SCORE_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_HM_SCORE_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_AW_SCORE_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_AW_SCORE_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_HM_TO_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_HM_TO_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_AW_TO_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_AW_TO_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_TO_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_TXT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_TITLE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_MIN ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_MIN_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_SEC ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_SEC_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_DSEC ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_DSEC_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_TITLE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_MIN ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_MIN_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_SEC ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_SEC_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_DSEC ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_DSEC_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_TITLE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_MIN ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_MIN_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_SEC ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_SEC_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_DSEC ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_DSEC_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_PER ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_PER_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_PER_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_TO ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_TO_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_TO_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_WARN ), SW_HIDE);
}

static void setTabTwo(HWND hwnd)
{
    ShowWindow(GetDlgItem(hwnd, ID_TIME_TITLE), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_TIME_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MIN_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_MIN_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_SEC_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_SEC_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DSEC_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DSEC_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_TEAM_TITLE), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HM_NAME_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HM_NAME_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AW_NAME_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AW_NAME_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HM_SCORE_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HM_SCORE_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AW_SCORE_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AW_SCORE_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HM_TO_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_HM_TO_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AW_TO_EDIT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_AW_TO_TXT), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_TO_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_TXT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_EDIT ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_PERIOD_NOTE ), SW_HIDE);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_TITLE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_MIN ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_MIN_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_SEC ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_SEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_DSEC ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TIMER_DSEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_TITLE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_MIN ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_MIN_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_SEC ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_SEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_DSEC ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_TO_DSEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_TITLE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_MIN ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_MIN_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_SEC ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_SEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_DSEC ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_DEF_OT_DSEC_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_PER ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_PER_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_PER_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_TO ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_TO_EDIT ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_MAX_TO_NOTE ), SW_SHOW);
    ShowWindow(GetDlgItem(hwnd, ID_WARN ), SW_SHOW);
}

static INT_PTR CALLBACK SettingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {        
        case WM_INITDIALOG:
            initUserData(hwnd);

            // Initialize the common controls (necessary for tab control)
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_TAB_CLASSES;
            InitCommonControlsEx(&icex);

            // Create the tab control
            HWND hTab = GetDlgItem(hwnd, IDC_TABCONTROL);

            // Add tabs dynamically
            TC_ITEM tie;
            tie.mask = TCIF_TEXT;
            tie.pszText = "Current State";
            TabCtrl_InsertItem(hTab, 0, &tie);

            tie.pszText = "Configure Defaults";
            TabCtrl_InsertItem(hTab, 1, &tie);

            // Set the initial tab
            TabCtrl_SetCurSel(hTab, 0);

            // get and modify text boxes
            HWND hwndEdit;

            char currMinutesBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_MIN_EDIT);
            sprintf(currMinutesBuf, "%d", user_data->timer_current.minutes);
            SetWindowText(hwndEdit, currMinutesBuf);
            
            char currSecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_SEC_EDIT);
            sprintf(currSecondsBuf, "%d", user_data->timer_current.seconds);
            SetWindowText(hwndEdit, currSecondsBuf);
            
            char currDsecondsBuf[2];
            hwndEdit = GetDlgItem(hwnd, ID_DSEC_EDIT);
            sprintf(currDsecondsBuf, "%d", user_data->timer_current.decaseconds);
            SetWindowText(hwndEdit, currDsecondsBuf);

            hwndEdit = GetDlgItem(hwnd, ID_HM_NAME_EDIT);
            SetWindowText(hwndEdit, user_data->homeName);

            hwndEdit = GetDlgItem(hwnd, ID_AW_NAME_EDIT);
            SetWindowText(hwndEdit, user_data->awayName);

            char homeScoreBuf[8];
            hwndEdit = GetDlgItem(hwnd, ID_HM_SCORE_EDIT);
            sprintf(homeScoreBuf, "%d", user_data->homeScore);
            SetWindowText(hwndEdit, homeScoreBuf);
            
            char awayScoreBuf[8];
            hwndEdit = GetDlgItem(hwnd, ID_AW_SCORE_EDIT);
            sprintf(awayScoreBuf, "%d", user_data->awayScore);
            SetWindowText(hwndEdit, awayScoreBuf);
            
            char homeTimeoutBuf[2];
            hwndEdit = GetDlgItem(hwnd, ID_HM_TO_EDIT);
            sprintf(homeTimeoutBuf, "%d", user_data->homeTimeouts);
            SetWindowText(hwndEdit, homeTimeoutBuf);
            
            char awayTimeoutBuf[2];
            hwndEdit = GetDlgItem(hwnd, ID_AW_TO_EDIT);
            sprintf(awayTimeoutBuf, "%d", user_data->awayTimeouts);
            SetWindowText(hwndEdit, awayTimeoutBuf);
            
            char periodBuf[2];
            hwndEdit = GetDlgItem(hwnd, ID_PERIOD_EDIT);
            sprintf(periodBuf, "%d", user_data->period);
            SetWindowText(hwndEdit, periodBuf);
            
            char periodNoteBuf[2];
            hwndEdit = GetDlgItem(hwnd, ID_PERIOD_NOTE);
            sprintf(periodNoteBuf, "Set as %d for OT.", user_data->max_periods+1);
            SetWindowText(hwndEdit, periodNoteBuf);
            
            char defMinutesBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TIMER_MIN_EDIT);
            sprintf(defMinutesBuf, "%d", user_data->timer_default.minutes);
            SetWindowText(hwndEdit, defMinutesBuf);
            
            char defSecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TIMER_SEC_EDIT);
            sprintf(defSecondsBuf, "%d", user_data->timer_default.seconds);
            SetWindowText(hwndEdit, defSecondsBuf);
            
            char defDsecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TIMER_DSEC_EDIT);
            sprintf(defDsecondsBuf, "%d", user_data->timer_default.decaseconds);
            SetWindowText(hwndEdit, defDsecondsBuf);
            
            char defToMinutesBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TO_MIN_EDIT);
            sprintf(defToMinutesBuf, "%d", user_data->timeout_default.minutes);
            SetWindowText(hwndEdit, defToMinutesBuf);
            
            char defToSecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TO_SEC_EDIT);
            sprintf(defToSecondsBuf, "%d", user_data->timeout_default.seconds);
            SetWindowText(hwndEdit, defToSecondsBuf);
            
            char defToDsecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TO_DSEC_EDIT);
            sprintf(defToDsecondsBuf, "%d", user_data->timeout_default.decaseconds);
            SetWindowText(hwndEdit, defToDsecondsBuf);
            
            char defOtMinutesBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_OT_MIN_EDIT);
            sprintf(defOtMinutesBuf, "%d", user_data->ot_default.minutes);
            SetWindowText(hwndEdit, defOtMinutesBuf);
            
            char defOtSecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_OT_SEC_EDIT);
            sprintf(defOtSecondsBuf, "%d", user_data->ot_default.seconds);
            SetWindowText(hwndEdit, defOtSecondsBuf);
            
            char defOtDsecondsBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_DEF_OT_DSEC_EDIT);
            sprintf(defOtDsecondsBuf, "%d", user_data->ot_default.decaseconds);
            SetWindowText(hwndEdit, defOtDsecondsBuf);
            
            char maxPeriodBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_MAX_PER_EDIT);
            sprintf(maxPeriodBuf, "%d", user_data->max_periods);
            SetWindowText(hwndEdit, maxPeriodBuf);
            
            char maxOtBuf[3];
            hwndEdit = GetDlgItem(hwnd, ID_MAX_TO_EDIT);
            sprintf(maxOtBuf, "%d", user_data->max_timeouts);
            SetWindowText(hwndEdit, maxOtBuf);
            
            setTabOne(hwnd);
            return TRUE;

        case WM_NOTIFY:
            {
                NMHDR* pnmhdr = (NMHDR*)lParam;
                if (pnmhdr->code == TCN_SELCHANGE) {
                    // Tab selection changed
                    int tabIndex = TabCtrl_GetCurSel(GetDlgItem(hwnd, IDC_TABCONTROL));
                    switch(tabIndex)
                    {
                        case 0:
                            setTabOne(hwnd);
                            break;
                        case 1:
                            setTabTwo(hwnd);
                            break;
                        default:
                            break;
                    }
                }
                break;
            }

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                // still need checks if change? for now just auto set
                readUserData(hwnd);
                saveUserDataAndPaint(hwnd);
                free(user_data);
                EndDialog(hwnd, IDOK);

                return TRUE;
            }
            else if (LOWORD(wParam) == IDCANCEL)
            {
                // No changes need to be made, simply exit
                free(user_data);
                EndDialog(hwnd, IDCANCEL);
                return TRUE;
            }
            break;

        case WM_CLOSE:
            // Handle close button click
            free(user_data);
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
    }

    return FALSE;
}

static void initUserData(HWND hwnd)
{
    HWND hwndParent = GetParent(hwnd);
    ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwndParent, 0);

    user_data = (DialogData*)malloc(sizeof(DialogData));

    user_data->timer_current = scoreboard_control->state.timer_current;
    strcpy(user_data->homeName, scoreboard_control->state.homeName);
	strcpy(user_data->awayName, scoreboard_control->state.awayName);
    user_data->homeScore = scoreboard_control->state.homeScore;
    user_data->awayScore = scoreboard_control->state.awayScore;
    user_data->homeTimeouts = scoreboard_control->state.homeTimeouts;
    user_data->awayTimeouts = scoreboard_control->state.awayTimeouts;
    user_data->period = scoreboard_control->state.period;
	user_data->timer_default = scoreboard_control->defaults.data.timer_default;
	user_data->timeout_default = scoreboard_control->defaults.data.timeout_default;
	user_data->ot_default = scoreboard_control->defaults.data.ot_default;
	strcpy(user_data->defaultHomeName, scoreboard_control->defaults.data.defaultHomeName);
	strcpy(user_data->defaultAwayName, scoreboard_control->defaults.data.defaultAwayName);
    user_data->max_timeouts = scoreboard_control->defaults.data.max_timeouts;
    user_data->max_periods = scoreboard_control->defaults.data.max_periods;
}

static void readUserData(HWND hwnd)
{
    HWND hwndRead;
    char * buf;
    buf = malloc(sizeof(char)*28);
    
    hwndRead = GetDlgItem(hwnd, ID_HM_NAME_EDIT);
    GetWindowText(hwndRead, buf, 28);
    strcpy(user_data->homeName, buf);
    
    hwndRead = GetDlgItem(hwnd, ID_AW_NAME_EDIT);
    GetWindowText(hwndRead, buf, 28);
    strcpy(user_data->awayName, buf);
    
    hwndRead = GetDlgItem(hwnd, ID_HM_SCORE_EDIT);
    GetWindowText(hwndRead, buf, 4);
    user_data->homeScore = atoi(buf);
    
    hwndRead = GetDlgItem(hwnd, ID_AW_SCORE_EDIT);
    GetWindowText(hwndRead, buf, 4);
    user_data->awayScore = atoi(buf);
    
    hwndRead = GetDlgItem(hwnd, ID_HM_TO_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->homeTimeouts = atoi(buf);
    
    hwndRead = GetDlgItem(hwnd, ID_AW_TO_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->awayTimeouts = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_MIN_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->timer_current.minutes = atoi(buf);
 
    hwndRead = GetDlgItem(hwnd, ID_SEC_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->timer_current.seconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DSEC_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->timer_current.decaseconds = atoi(buf);
    
    hwndRead = GetDlgItem(hwnd, ID_PERIOD_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->period = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_TIMER_MIN_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->timer_default.minutes = atoi(buf);
 
    hwndRead = GetDlgItem(hwnd, ID_DEF_TIMER_SEC_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->timer_default.seconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_TIMER_DSEC_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->timer_default.decaseconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_TO_MIN_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->timeout_default.minutes = atoi(buf);
 
    hwndRead = GetDlgItem(hwnd, ID_DEF_TO_SEC_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->timeout_default.seconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_TO_DSEC_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->timeout_default.decaseconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_OT_MIN_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->ot_default.minutes = atoi(buf);
 
    hwndRead = GetDlgItem(hwnd, ID_DEF_OT_SEC_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->ot_default.seconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_OT_DSEC_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->ot_default.decaseconds = atoi(buf);
 
    hwndRead = GetDlgItem(hwnd, ID_DEF_OT_SEC_EDIT);
    GetWindowText(hwndRead, buf, 3);
    user_data->ot_default.seconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_DEF_OT_DSEC_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->ot_default.decaseconds = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_MAX_PER_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->max_periods = atoi(buf);

    hwndRead = GetDlgItem(hwnd, ID_MAX_TO_EDIT);
    GetWindowText(hwndRead, buf, 2);
    user_data->max_timeouts = atoi(buf);
}

static void saveUserDataAndPaint(HWND hwnd)
{
    HWND hwndParent = GetParent(hwnd);
    ScoreboardControl* scoreboard_control = (ScoreboardControl*)GetWindowLongPtr(hwndParent, 0);
    
    scoreboard_control->state.timer_current = user_data->timer_current;
    strcpy(scoreboard_control->state.homeName, user_data->homeName);
	strcpy(scoreboard_control->state.awayName, user_data->awayName);
    scoreboard_control->state.homeScore = user_data->homeScore;
    scoreboard_control->state.awayScore = user_data->awayScore;
    scoreboard_control->state.homeTimeouts = user_data->homeTimeouts;
    scoreboard_control->state.awayTimeouts = user_data->awayTimeouts;
    scoreboard_control->state.period = user_data->period;
	scoreboard_control->defaults.data.timer_default = user_data->timer_default;
	scoreboard_control->defaults.data.timeout_default = user_data->timeout_default;
	scoreboard_control->defaults.data.ot_default = user_data->ot_default;
	strcpy(scoreboard_control->defaults.data.defaultHomeName, user_data->defaultHomeName);
	strcpy(scoreboard_control->defaults.data.defaultAwayName, user_data->defaultAwayName);
    scoreboard_control->defaults.data.max_timeouts = user_data->max_timeouts;
    scoreboard_control->defaults.data.max_periods = user_data->max_periods;

    scoreboard_control->paint.all = true;
    InvalidateRect(hwndParent, NULL, TRUE);
}

void configurationDialog(HWND hwndParent)
{
    // Create the settings dialog
    HWND g_hDlg = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONFIGURE), hwndParent, SettingsDlgProc);

    // Show the settings dialog
    ShowWindow(g_hDlg, SW_SHOW);
}
