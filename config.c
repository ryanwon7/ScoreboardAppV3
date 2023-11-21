#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "config.h"

static void loadConfig( ScoreboardDefaults* defaults );

void setDefaults(ScoreboardControl* scoreboard_control)
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
	scoreboard_control->state.homeTimeouts = scoreboard_control->defaults.data.max_timeouts;
	scoreboard_control->state.awayTimeouts = scoreboard_control->defaults.data.max_timeouts;
	strcpy(scoreboard_control->state.homeName, scoreboard_control->defaults.data.defaultHomeName);
	strcpy(scoreboard_control->state.awayName, scoreboard_control->defaults.data.defaultAwayName);
	scoreboard_control->state.period = 1;
	scoreboard_control->state.timeoutCaller = 0;
	scoreboard_control->state.toggle = false;
	scoreboard_control->state.timer_current.minutes = scoreboard_control->defaults.data.timer_default.minutes;
	scoreboard_control->state.timer_current.seconds = scoreboard_control->defaults.data.timer_default.seconds;
	scoreboard_control->state.timer_current.decaseconds = scoreboard_control->defaults.data.timer_default.decaseconds;
	scoreboard_control->state.timeout_current.minutes = scoreboard_control->defaults.data.timeout_default.minutes;
	scoreboard_control->state.timeout_current.seconds = scoreboard_control->defaults.data.timeout_default.seconds;
	scoreboard_control->state.timeout_current.decaseconds = scoreboard_control->defaults.data.timeout_default.decaseconds;
}

void loadConfigFileAndDefaults( ScoreboardControl* scoreboard_control, ScoreboardDesign* scoreboard_design)
{
	loadConfig(&scoreboard_control->defaults );

    setDefaults(scoreboard_control);

	scoreboard_design->timerFont = CreateFont(floor(0.3 * GetSystemMetrics(SM_CYSCREEN)), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->teamFont = CreateFont(floor(0.11 * GetSystemMetrics(SM_CYSCREEN)), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->scoreFont = CreateFont(floor(0.43 * GetSystemMetrics(SM_CYSCREEN)), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->periodFont = CreateFont(floor(0.19 * GetSystemMetrics(SM_CYSCREEN)), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->overtimeFont = CreateFont(floor(0.14 * GetSystemMetrics(SM_CYSCREEN)), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,                      
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,           
        DEFAULT_PITCH | FF_SWISS, "Arial Nova");

	scoreboard_design->smallFont = CreateFont(floor(0.035 * GetSystemMetrics(SM_CYSCREEN)), 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,                      
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

static void loadConfig(ScoreboardDefaults* defaults)
{
    FILE* file = fopen( "default.conf", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[78];  // Assuming a maximum line length of 78 characters
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char key[50];
        char value[28];
        if (sscanf(line, "%49[^=]=%27[^\n]", key, value) == 2) {
            if (strcmp(key, "leftMinusPtKey") == 0) {
                defaults->keycodes.leftMinusPtKey = atoi(value);
            }
            else if (strcmp(key, "leftPlus2PtKey") == 0) {
                defaults->keycodes.leftPlus2PtKey = atoi(value);
            }
            else if (strcmp(key, "leftPlus3PtKey") == 0) {
                defaults->keycodes.leftPlus3PtKey = atoi(value);
            }
            else if (strcmp(key, "leftTimeoutKey") == 0) {
                defaults->keycodes.leftTimeoutKey = atoi(value);
            }
            else if (strcmp(key, "rightMinusPtKey") == 0) {
                defaults->keycodes.rightMinusPtKey = atoi(value);
            }
            else if (strcmp(key, "rightPlus2PtKey") == 0) {
                defaults->keycodes.rightPlus2PtKey = atoi(value);
            }
            else if (strcmp(key, "rightPlus3PtKey") == 0) {
                defaults->keycodes.rightPlus3PtKey = atoi(value);
            }
            else if (strcmp(key, "rightTimeoutKey") == 0) {
                defaults->keycodes.rightTimeoutKey = atoi(value);
            }
            else if (strcmp(key, "switchKey") == 0) {
                defaults->keycodes.switchKey = atoi(value);
            }
            else if (strcmp(key, "resetKey") == 0) {
                defaults->keycodes.resetKey = atoi(value);
            }
            else if (strcmp(key, "setKey") == 0) {
                defaults->keycodes.setKey = atoi(value);
            }
            else if (strcmp(key, "periodPlusKey") == 0) {
                defaults->keycodes.periodPlusKey = atoi(value);
            }
            else if (strcmp(key, "periodMinusKey") == 0) {
                defaults->keycodes.periodMinusKey = atoi(value);
            }
            else if (strcmp(key, "startTimeKey") == 0) {
                defaults->keycodes.startTimeKey = atoi(value);
            }
            else if (strcmp(key, "stopTimeKey") == 0) {
                defaults->keycodes.stopTimeKey = atoi(value);
            }
            else if (strcmp(key, "timeoutClearKey") == 0) {
                defaults->keycodes.timeoutClearKey = atoi(value);
            }
            else if (strcmp(key, "configureKey") == 0) {
                defaults->keycodes.configureKey = atoi(value);
            }
            else if (strcmp(key, "timer_length_min") == 0) {
                defaults->data.timer_default.minutes = atoi(value);
            }
            else if (strcmp(key, "timer_length_sec") == 0) {
                defaults->data.timer_default.seconds = atoi(value);
            }
            else if (strcmp(key, "timer_length_desc") == 0) {
                defaults->data.timer_default.decaseconds = atoi(value);
            }
            else if (strcmp(key, "timeout_length_min") == 0) {
                defaults->data.timeout_default.minutes = atoi(value);
            }
            else if (strcmp(key, "timeout_length_sec") == 0) {
                defaults->data.timeout_default.seconds = atoi(value);
            }
            else if (strcmp(key, "timeout_length_dsec") == 0) {
                defaults->data.timeout_default.decaseconds = atoi(value);
            }
            else if (strcmp(key, "overtime_length_min") == 0) {
                defaults->data.ot_default.minutes = atoi(value);
            }
            else if (strcmp(key, "overtime_length_sec") == 0) {
                defaults->data.ot_default.seconds = atoi(value);
            }
            else if (strcmp(key, "overtime_length_dsec") == 0) {
                defaults->data.ot_default.decaseconds = atoi(value);
            }
            else if (strcmp(key, "home_name") == 0) {
                strncpy(defaults->data.defaultHomeName, value, sizeof(defaults->data.defaultHomeName));
                defaults->data.defaultHomeName[sizeof(defaults->data.defaultHomeName) - 1] = '\0';  // Ensure null-terminated
            }
            else if (strcmp(key, "away_name") == 0) {
                strncpy(defaults->data.defaultAwayName, value, sizeof(defaults->data.defaultAwayName));
                defaults->data.defaultAwayName[sizeof(defaults->data.defaultAwayName) - 1] = '\0';  // Ensure null-terminated
            }
            else if (strcmp(key, "max_timeouts") == 0) {
                if (atoi(value) > 3)
                {
                    defaults->data.max_timeouts = 3;
                }
                else
                {
                    defaults->data.max_timeouts = atoi(value);
                }
            }
            else if (strcmp(key, "max_periods") == 0) {
                if (atoi(value) > 4)
                {
                    defaults->data.max_periods = 4;
                }
                else
                {
                    defaults->data.max_periods = atoi(value);
                }
            }
        }
    }

    fclose(file);
}

void writeToConfig(ScoreboardControl* scoreboard_control)
{
    FILE* file = fopen( "default.conf", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[79];

    sprintf(line, "leftMinusPtKey=%d\n", scoreboard_control->defaults.keycodes.leftMinusPtKey);
    fputs(line, file);
    sprintf(line, "leftPlus2PtKey=%d\n", scoreboard_control->defaults.keycodes.leftPlus2PtKey);
    fputs(line, file);
    sprintf(line, "leftPlus3PtKey=%d\n", scoreboard_control->defaults.keycodes.leftPlus3PtKey);
    fputs(line, file);
    sprintf(line, "leftTimeoutKey=%d\n", scoreboard_control->defaults.keycodes.leftTimeoutKey);
    fputs(line, file);
    sprintf(line, "rightMinusPtKey=%d\n", scoreboard_control->defaults.keycodes.rightMinusPtKey);
    fputs(line, file);
    sprintf(line, "rightPlus2PtKey=%d\n", scoreboard_control->defaults.keycodes.rightPlus2PtKey);
    fputs(line, file);
    sprintf(line, "rightPlus3PtKey=%d\n", scoreboard_control->defaults.keycodes.rightPlus3PtKey);
    fputs(line, file);
    sprintf(line, "rightTimeoutKey=%d\n", scoreboard_control->defaults.keycodes.rightTimeoutKey);
    fputs(line, file);
    sprintf(line, "switchKey=%d\n", scoreboard_control->defaults.keycodes.switchKey);
    fputs(line, file);
    sprintf(line, "resetKey=%d\n", scoreboard_control->defaults.keycodes.resetKey);
    fputs(line, file);
    sprintf(line, "setKey=%d\n", scoreboard_control->defaults.keycodes.setKey);
    fputs(line, file);
    sprintf(line, "periodPlusKey=%d\n", scoreboard_control->defaults.keycodes.periodPlusKey);
    fputs(line, file);
    sprintf(line, "periodMinusKey=%d\n", scoreboard_control->defaults.keycodes.periodMinusKey);
    fputs(line, file);
    sprintf(line, "startTimeKey=%d\n", scoreboard_control->defaults.keycodes.startTimeKey);
    fputs(line, file);
    sprintf(line, "stopTimeKey=%d\n", scoreboard_control->defaults.keycodes.stopTimeKey);
    fputs(line, file);
    sprintf(line, "timeoutClearKey=%d\n", scoreboard_control->defaults.keycodes.timeoutClearKey);
    fputs(line, file);
    sprintf(line, "configureKey=%d\n", scoreboard_control->defaults.keycodes.configureKey);
    fputs(line, file);

    sprintf(line, "timer_length_min=%d\n", scoreboard_control->defaults.data.timer_default.minutes);
    fputs(line, file);
    sprintf(line, "timer_length_sec=%d\n", scoreboard_control->defaults.data.timer_default.seconds);
    fputs(line, file);
    sprintf(line, "timer_length_dsec=%d\n", scoreboard_control->defaults.data.timer_default.decaseconds);
    fputs(line, file);
    sprintf(line, "timeout_length_min=%d\n", scoreboard_control->defaults.data.timeout_default.minutes);
    fputs(line, file);
    sprintf(line, "timeout_length_sec=%d\n", scoreboard_control->defaults.data.timeout_default.seconds);
    fputs(line, file);
    sprintf(line, "timeout_length_dsec=%d\n", scoreboard_control->defaults.data.timeout_default.decaseconds);
    fputs(line, file);
    sprintf(line, "overtime_length_min=%d\n", scoreboard_control->defaults.data.ot_default.minutes);
    fputs(line, file);
    sprintf(line, "overtime_length_sec=%d\n", scoreboard_control->defaults.data.ot_default.seconds);
    fputs(line, file);
    sprintf(line, "overtime_length_dsec=%d\n", scoreboard_control->defaults.data.ot_default.decaseconds);
    fputs(line, file);

    sprintf(line,"home_name=%s\n", scoreboard_control->defaults.data.defaultHomeName);
    fputs(line, file);
    sprintf(line,"away_name=%s\n", scoreboard_control->defaults.data.defaultAwayName);
    fputs(line, file);
    sprintf(line, "max_timeouts=%d\n", scoreboard_control->defaults.data.max_timeouts);
    fputs(line, file);
    sprintf(line, "max_periods=%d\n", scoreboard_control->defaults.data.max_periods);
    fputs(line, file);


    fclose(file);
}
