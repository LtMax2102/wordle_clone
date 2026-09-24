#pragma once
#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <Windows.h>

// All method comments are in the cpp file along with demonstrations of the harder to remember methods

class Screen
{
protected:
    static SHORT width, height;
    static HANDLE wHand, rHand;
    static SMALL_RECT windowSize;
    static COORD bufferSize;
    static CHAR_INFO *consoleBuffer;
    bool refresh;

    void drawBox(SHORT left, SHORT top, SHORT right, SHORT bottom, WORD backgroundColour = 0, WORD foregroundColour = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    void fillBox(SHORT left, SHORT top, SHORT right, SHORT bottom, char charToFill = ' ', WORD backgroundColour = 0, WORD foregroundColour = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    void drawText(SHORT left, SHORT top, const char *text, WORD backgroundColour = 0, WORD foregroundColour = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    void clearScreen();
    void handleInput();
    void processEvent(INPUT_RECORD &event);
    void present();
    SHORT getCentre(bool axis, const char *text = "");
	void hideCursor();
	void showCursor();

private:
    static void funDrawingApp();

public:
    bool screenRunning;
    virtual void Draw() = 0;
    virtual void HandleKey(KEY_EVENT_RECORD &key) = 0;
    int MainLoop();
    static void Initialise(SHORT width, SHORT height, const char *consoleTitle);
    static void ShutDown();
};

#endif