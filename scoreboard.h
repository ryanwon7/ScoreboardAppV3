#ifndef SCOREBOARD_HEADER
#define SCOREBOARD_HEADER

#include <stdbool.h>
#include <windows.h>

typedef struct timer_t {
	int minutes;
	int seconds;
	int decaseconds;
} timer_t;

typedef struct CommandKeycodes {
	int leftMinusPtKey;
	int leftPlus2PtKey;
	int leftPlus3PtKey;
	int leftTimeoutKey;
	int rightMinusPtKey;
	int rightPlus2PtKey;
	int rightPlus3PtKey;
	int rightTimeoutKey;
	int switchKey;
	int resetKey;
	int setKey;
	int periodPlusKey;
	int periodMinusKey;
	int startTimeKey;
	int stopTimeKey;
	int timeoutClearKey;
	int configureKey;
} CommandKeycodes;

typedef struct DefaultData {
	timer_t timer_default;
	timer_t timeout_default;
	timer_t ot_default;
	char defaultHomeName[28];
	char defaultAwayName[28];
	int max_timeouts;
	int max_periods;
} DefaultData;

typedef struct ScoreboardDefaults {
	CommandKeycodes keycodes;
	DefaultData data;
} ScoreboardDefaults;

typedef struct PaintRegions {
	bool homeTeam;
	bool awayTeam;
	bool homeScore;
	bool awayScore;
	bool homeTimeouts;
	bool awayTimeouts;
	bool period;
	bool timer;
	bool timerTimeout;
	bool timeoutFlicker;
	bool leftLogo;
	bool rightLogo;
	bool switches;
	bool all;
} PaintRegions;

typedef struct ScoreboardState {
	int switchState; // 0 default, home left away right. 1=switched, home right away left
	int homeScore;
	int awayScore;
	int homeTimeouts;
	int awayTimeouts;
	char homeName[28];
	char awayName[28];
	int period;
	int timeoutCaller; // 0 for left, 1 for right
	bool toggle;
	timer_t timer_current;
	timer_t timeout_current;
} ScoreboardState;

typedef struct ScoreboardControl {
	ScoreboardDefaults defaults;
	PaintRegions paint;
	ScoreboardState state;
} ScoreboardControl;

typedef struct ScoreboardDesign {
	HFONT timerFont;
	HFONT teamFont;
	HFONT scoreFont;
	HFONT periodFont;
	HFONT overtimeFont;
	HFONT smallFont;
	HPEN outlinePen;
	HBRUSH backgroundBrush;
	HBRUSH fillBrush;
	HBRUSH fillHomeBrush;
	HBRUSH fillAwayBrush;
	HBITMAP logo;
	HBITMAP logoLeft;
	HBITMAP logoRight;
} ScoreboardDesign;

#endif
