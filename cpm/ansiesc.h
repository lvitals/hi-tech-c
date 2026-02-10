#pragma once

#include <stdio.h>

enum Colors {
    RESET_COLOR,
    BLACK_TXT = 30,
    RED_TXT,
    GREEN_TXT,
    YELLOW_TXT,
    BLUE_TXT,
    MAGENTA_TXT,
    CYAN_TXT,
    WHITE_TXT,

    BLACK_BKG = 40,
    RED_BKG,
    GREEN_BKG,
    YELLOW_BKG,
    BLUE_BKG,
    MAGENTA_BKG,
    CYAN_BKG,
    WHITE_BKG    
};

enum ClearCodes {
    CLEAR_FROM_CURSOR_TO_END,
    CLEAR_FROM_CURSOR_TO_BEGIN,
    CLEAR_ALL
};

void setupConsole(void);
void restoreConsole(void);
void getCursorPosition(int *row, int *col);

static void setTextColor(int code) {
    printf("\x1b[3%dm", code);
}

static void setTextColorBright(int code) {
    printf("\x1b[3%d;1m", code);
}

static void setBackgroundColor(int code) {
    printf("\x1b[4%dm", code);
}

static void setBackgroundColorBright(int code) {
    printf("\x1b[4%d;1m", code);
}
	
void resetColor(void) {
    printf("\x1b[%dm", RESET_COLOR);
}

static void clearScreen(void) {
    printf("\x1b[%dJ", CLEAR_ALL);
}

static void clearScreenToBottom(void) {
    printf("\x1b[%dJ", CLEAR_FROM_CURSOR_TO_END);
}

static void clearScreenToTop(void) {
    printf("\x1b[%dJ", CLEAR_FROM_CURSOR_TO_BEGIN);
}

static void clearLine(void) {
    printf("\x1b[%dK", CLEAR_ALL);
}

static void clearLineToRight(void) {
    printf("\x1b[%dK", CLEAR_FROM_CURSOR_TO_END);
}

static void clearLineToLeft(void) {
    printf("\x1b[%dK", CLEAR_FROM_CURSOR_TO_BEGIN);
}

static void moveUp(int positions) {
    printf("\x1b[%dA", positions);
}

static void moveDown(int positions) {
    printf("\x1b[%dB", positions);
}

static void moveRight(int positions) {
    printf("\x1b[%dC", positions);
}

static void moveLeft(int positions) {
    printf("\x1b[%dD", positions);
}

static void moveTo(int row, int col) {
    printf("\x1b[%d;%df", row, col);
}

static void saveCursorPosition(void) {
    printf("\x1b[s");		/* "\x1b7" */
}

static void restoreCursorPosition(void) {
    printf("\x1b[u");		/* "\x1b8" */
}

static void setTextInverted() { 
    printf("\x1b[7m");
}

static void setTextNoInverted() { 
    printf("\x1b[27m");
}

static void setTextNormal() { 
    printf("\x1b[0m");
}


static void setTextBold() {
    printf("\x1b[1m");
}

static void setTextNoBold() {
    printf("\x1b[22m");
}

static void setTextUnderlined() {
    printf("\x1b[4m");
}

static void setTextNoUnderlined() {
    printf("\x1b[24m");
}

static void insLine() {
    printf("\x1b[L");
}

static void delLine() {
    printf("\x1b[M");
}

static void setCursorOn() {
   printf("\x1b[?25h");
}

static void setCursorOff() {
   printf("\x1b[?25l");
}

static void setTextBlinking() {
   printf("\x1b[5m");
}

static void setTextNoBlinking() {
   printf("\x1b[25m");
}

