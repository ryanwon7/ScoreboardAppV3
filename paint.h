#ifndef PAINT_HEADER
#define PAINT_HEADER

#include "scoreboard.h"

void paintTimer( ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int minutes, int seconds, int decaseconds );

void paintLeftTimeoutTimer( ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int minutes, int seconds );

void paintRightTimeoutTimer( ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int minutes, int seconds);

void paintLeftScore(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int points);

void paintRightScore(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, int points);

void paintPeriod(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool overtime, int period);

void paintLeftName(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, const char * name);

void paintRightName(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, const char * name);

void paintLeftTimeouts(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts);

void paintRightTimeouts(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts);

void paintActiveLeftTimeout(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts, bool toggle);

void paintActiveRightTimeout(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd, bool home, int timeouts, bool toggle);

void paintLeftLogo(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd);

void paintRightLogo(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd);

void paintLogoBackground(ScoreboardDesign* scoreboard_design, HDC hdc, HWND hwnd);
#endif
