#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include "dialog.h"

DialogData * user_data;
bool bContentChanged = false;

static void initUserData(HWND hwnd);
static void updateUserData(HWND hwnd, int controlId);
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
        case WM_COMMAND:
        {
            int control = LOWORD(wParam);
            int notificationCode = HIWORD(wParam);

            if (notificationCode == EN_CHANGE)
            {
                bContentChanged = true;
            }
            else if (notificationCode == EN_KILLFOCUS)
            {
                if (bContentChanged)
                {
                    updateUserData(hwnd, control);
                    bContentChanged = false;
                }
            }
            else if (control == IDOK)
            {
                saveUserDataAndPaint(hwnd);
                free(user_data);
                EndDialog(hwnd, IDOK);
                return TRUE;
            }
            else if (control == IDCANCEL)
            {
                free(user_data);
                EndDialog(hwnd, IDCANCEL);
                return TRUE;
            }
            break;
        }
   
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
            char writeBuf[8];

            hwndEdit = GetDlgItem(hwnd, ID_MIN_EDIT);
            sprintf(writeBuf, "%d", user_data->timer_current.minutes);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_SEC_EDIT);
            sprintf(writeBuf, "%d", user_data->timer_current.seconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DSEC_EDIT);
            sprintf(writeBuf, "%d", user_data->timer_current.decaseconds);
            SetWindowText(hwndEdit, writeBuf);

            hwndEdit = GetDlgItem(hwnd, ID_HM_NAME_EDIT);
            SetWindowText(hwndEdit, user_data->homeName);

            hwndEdit = GetDlgItem(hwnd, ID_AW_NAME_EDIT);
            SetWindowText(hwndEdit, user_data->awayName);

            hwndEdit = GetDlgItem(hwnd, ID_HM_SCORE_EDIT);
            sprintf(writeBuf, "%d", user_data->homeScore);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_AW_SCORE_EDIT);
            sprintf(writeBuf, "%d", user_data->awayScore);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_HM_TO_EDIT);
            sprintf(writeBuf, "%d", user_data->homeTimeouts);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_AW_TO_EDIT);
            sprintf(writeBuf, "%d", user_data->awayTimeouts);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_PERIOD_EDIT);
            sprintf(writeBuf, "%d", user_data->period);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_PERIOD_NOTE);
            sprintf(writeBuf, "Set as %d for OT.", user_data->max_periods+1);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TIMER_MIN_EDIT);
            sprintf(writeBuf, "%d", user_data->timer_default.minutes);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TIMER_SEC_EDIT);
            sprintf(writeBuf, "%d", user_data->timer_default.seconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TIMER_DSEC_EDIT);
            sprintf(writeBuf, "%d", user_data->timer_default.decaseconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TO_MIN_EDIT);
            sprintf(writeBuf, "%d", user_data->timeout_default.minutes);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TO_SEC_EDIT);
            sprintf(writeBuf, "%d", user_data->timeout_default.seconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_TO_DSEC_EDIT);
            sprintf(writeBuf, "%d", user_data->timeout_default.decaseconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_OT_MIN_EDIT);
            sprintf(writeBuf, "%d", user_data->ot_default.minutes);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_OT_SEC_EDIT);
            sprintf(writeBuf, "%d", user_data->ot_default.seconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_DEF_OT_DSEC_EDIT);
            sprintf(writeBuf, "%d", user_data->ot_default.decaseconds);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_MAX_PER_EDIT);
            sprintf(writeBuf, "%d", user_data->max_periods);
            SetWindowText(hwndEdit, writeBuf);
            
            hwndEdit = GetDlgItem(hwnd, ID_MAX_TO_EDIT);
            sprintf(writeBuf, "%d", user_data->max_timeouts);
            SetWindowText(hwndEdit, writeBuf);
            
            setTabOne(hwnd);
            return TRUE;

        case WM_CLOSE:
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

static void updateUserData(HWND hwnd, int controlId)
{
    HWND hwndRead;
    char * buf;
    buf = malloc(sizeof(char)*28);

    switch(controlId)
    {
        case ID_HM_NAME_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 28);
            strcpy(user_data->homeName, buf);
            break;

        case ID_AW_NAME_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 28);
            strcpy(user_data->awayName, buf);
            break;

        case ID_HM_SCORE_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 4);
            user_data->homeScore = atoi(buf);
            break;

        case ID_AW_SCORE_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 4);
            user_data->awayScore = atoi(buf);
            break;

        case ID_HM_TO_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->homeTimeouts = atoi(buf);
            break;

        case ID_AW_TO_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->awayTimeouts = atoi(buf);  
            break; 

        case ID_MIN_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->timer_current.minutes = atoi(buf);
            break;

        case ID_SEC_EDIT: 
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->timer_current.seconds = atoi(buf);
            break;

        case ID_DSEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->timer_current.decaseconds = atoi(buf);
            break;

        case ID_PERIOD_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->period = atoi(buf);
            break;

        case ID_DEF_TIMER_MIN_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->timer_default.minutes = atoi(buf);
            break;

        case ID_DEF_TIMER_SEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->timer_default.seconds = atoi(buf);
            break;

        case ID_DEF_TIMER_DSEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->timer_default.decaseconds = atoi(buf);
            break;

        case ID_DEF_TO_MIN_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->timeout_default.minutes = atoi(buf);
            break;

        case ID_DEF_TO_SEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->timeout_default.seconds = atoi(buf);
            break;

        case ID_DEF_TO_DSEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->timeout_default.decaseconds = atoi(buf);
            break;
        
        case ID_DEF_OT_MIN_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->ot_default.minutes = atoi(buf);
            break;

        case ID_DEF_OT_SEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 3);
            user_data->ot_default.seconds = atoi(buf);
            break;

        case ID_DEF_OT_DSEC_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->ot_default.decaseconds = atoi(buf);
            break;

        case ID_MAX_PER_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->max_periods = atoi(buf);
            break;

        case ID_MAX_TO_EDIT:
            hwndRead = GetDlgItem(hwnd, controlId);
            GetWindowText(hwndRead, buf, 2);
            user_data->max_timeouts = atoi(buf);
            break;

        default:
            free(buf);
            return;
    }
    free(buf);
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
