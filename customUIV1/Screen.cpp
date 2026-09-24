#include "Screen.hpp"

// All code from this section is based on the tutorial by https://www.benryves.com/tutorials/winconsole/3, refer to that when stuck, it deals with introduciton to this type of programming and using the windows library

SHORT Screen::width = 0;
SHORT Screen::height = 0;

HANDLE Screen::wHand = nullptr;
HANDLE Screen::rHand = nullptr;

SMALL_RECT Screen::windowSize = {};
COORD Screen::bufferSize = {};
CHAR_INFO *Screen::consoleBuffer = nullptr;

////////////////////////////////////////////////////////////////////////////
// 								HOW TO USE
////////////////////////////////////////////////////////////////////////////
// Firstly create child class of screen and impliment the overridden methods with their own implimentation (Draw and Handle key, there are pre created ones below to follow)
// Use Screen::Initialise to create the screen which you will be using
// Create a draw method which will be the main mehtod used to push things into the buffer, must have present at the bottom to actually move things to screen
// Create a variable of type <your screen class> 
// All <your screen class> MainLoop to see the program run
// During runtime when buffer changes make sure to update the refresh variable so that the program knows the refresh the screen
// Call Screen::ShutDown() when the program exits so that it exists gracefully 
// 
////////////////////////////////////////////////////////////////////////////

/// DRAW EXAMPLE
/// Draw()
/// {
///     drawBox(0, 0, width, height);
/// 
///     drawBox(0, 2, width, 3);
/// 
///     drawText(getCentre(1, "Graveyard Database Debug Console"), 1, "Graveyard Database Debug Console");
/// 
///     const char *options[2];
///     options[0] = "View Graveyards";
///     options[1] = "Quit";
/// 
///     SHORT v_centre = getCentre(0) - 4;
///     SHORT h_centre;
/// 
///     for (int i = 0; i < 2; i++)
///     {
///         h_centre = getCentre(1, options[i]);
///         if (i == selected)
///         {
///             drawText(h_centre - 2, v_centre + (2 * i), "[");
///             drawText(h_centre + (strlen(options[i]) + 1), v_centre + (2 * i), "]");
///         }
/// 
///         drawText(h_centre, v_centre + (2 * i), options[i]);
///     }
/// 
///     present();
/// }

/// HANDLE KEY EXAMPLE 
///
/// Only works when key is released can be changed based on your implimentation 
/// For keys that are pre defined like the enter, arrow, and other main keys use key.wVirtualKeyCode, for character keys use key.uChar.AsciiChar
/// HandleKey(KEY_EVENT_RECORD &key)
/// 
///    if (!key.bKeyDown)
///      return;
/// 
///    refresh = true;
///    switch (key.wVirtualKeyCode)
///    {
///    case VK_ESCAPE:
///        screenRunning = false;
///        returnVal = ScreenCodes::None;
///        break;
/// 
///    case VK_UP:
///        if (selected - 1 < 0)
///            selected = 1;
///        else
///            selected--;
///        break;
/// 
///    case VK_DOWN:
///        if (selected + 1 > 1)
///            selected = 0;
///        else
///            selected++;
///        break;
///    case VK_RETURN:
///        HandleEnter();
///        break;
///
///    	switch (key.uChar.AsciiChar)
///    	{
///    	case 'a':
///    		break;
///    	
///    	case 'e':
///    		break;
///    	
///    	case 'd':
///    		break;
///    	}
///    }

/// @brief Initialises all of the variables to create the window and buffer for the app
/// @param Width Width of the window, how many columns it will create on the window
/// @param Height Height of the window, how many rows it will create within the window
/// @param consoleTitle The title you want to give the window
void Screen::Initialise(SHORT Width, SHORT Height, const char *consoleTitle)
{
    width = Width;
    height = Height;
    wHand = GetStdHandle(STD_OUTPUT_HANDLE); // WRITING
    rHand = GetStdHandle(STD_INPUT_HANDLE);  // READING
    windowSize = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
    bufferSize = {width, height};

    consoleBuffer = new CHAR_INFO[width * height];

    SetConsoleTitleA(consoleTitle);

    SetConsoleScreenBufferSize(wHand, bufferSize);

    SetConsoleWindowInfo(wHand, TRUE, &windowSize);
}

/// @brief Deletes the consoleBuffer before leaving the application
void Screen::ShutDown()
{
    delete[] consoleBuffer;
}

/// @brief Mainloop for the screen, will draw what is pushed to the buffer defaultly and handle input. Can be overwritten for more control. Functions inside need to be defined too (draw, and the handle key method for the input handle or nothing will happen
int Screen::MainLoop()
{
    Draw();
    while (screenRunning)
    {
        if (refresh)
        {
            present();
            refresh = false;
        }
        handleInput();
    }
    return 0;
};

/// @brief Pushes buffer to screen, anything done to buffer will not appear until this is called
void Screen::present()
{
    COORD charBuffSize = {width, height};
    COORD characterPos = {0, 0};
    SMALL_RECT writeArea = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};
    WriteConsoleOutputA(wHand, consoleBuffer, charBuffSize, characterPos, &writeArea);
}

/// @brief Draws text to the screen using positions 
/// @param left position from the left of window where you want text to start
/// @param Top position from the top of window where you want text to start
/// @param The text you want output to the screen (is a c string, if using c++ string use c_str()
/// @param foregroundColour What colour the foreground should be, default uses FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE for white but can use FOREGROUND_ANYTHING
void Screen::drawText(SHORT left, SHORT top, const char *text, WORD backgroundColour, WORD foregroundColour)
{
    SHORT startPos = left + width * top;
    size_t textLen = strlen(text);

    for (int i = 0; i < textLen; i++)
    {
        consoleBuffer[startPos + i].Char.AsciiChar = text[i];
		consoleBuffer[startPos + i].Attributes = backgroundColour | foregroundColour;
    }

}

/// @brief Fills based on the inputted coordinates
/// @param left Position from the left of window where you want box to begin
/// @param top Position from the top of window where you want box to begin
/// @param right Position from the right of window where you want box to end
/// @param bottom Position from the bottom of window where you want box to end
/// @param charToFill The char you want to fill with, by default is ' ' for empty so it will just clear what's currently there
/// @param backgroundColour What colour the background should be, default uses 0 for black but can use BLACKGROUND_ANYTHING
/// @param foregroundColour What colour the foreground should be, default uses FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE for white but can use FOREGROUND_ANYTHING
void Screen::fillBox(
    SHORT left,
    SHORT top,
    SHORT right,
    SHORT bottom,
    char charToFill,
    WORD backgroundColour,
    WORD foregroundColour)
{

    for (int y = top; y < bottom; y++)
    {
        for (int x = left; x < right; x++)
        {
            (backgroundColour > 0) ? consoleBuffer[x + width * y].Attributes = foregroundColour | backgroundColour | BACKGROUND_INTENSITY : consoleBuffer[x + width * y].Attributes = foregroundColour | backgroundColour;
            consoleBuffer[x + width * y].Char.AsciiChar = charToFill;
        }
    }

}

/// @brief Uses fillBox to clear the screen
void Screen::clearScreen()
{
    fillBox(0, 0, width, height);
}

/// @brief Is called in order to get user inputs, when one is detected it will store and send to process event 
void Screen::handleInput()
{
    DWORD numEvents = 0;
    DWORD numEventsRead = 0;
    GetNumberOfConsoleInputEvents(rHand, &numEvents);

    if (numEvents != 0)
    {
        INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

        ReadConsoleInput(rHand, eventBuffer, numEvents, &numEventsRead);

        for (int i = 0; i < numEventsRead; i++)
        {
            processEvent(eventBuffer[i]);
        }

        delete[] eventBuffer;
    }
}

/// @brief Takes a user input and based on the event calls the relevant handling method
/// @param The event which has been captured
void Screen::processEvent(INPUT_RECORD &event)
{
    if (event.EventType == KEY_EVENT)
    {
        HandleKey(event.Event.KeyEvent);
    }
}

/// @brief Gets the centre of the window, both x and y axis
/// @param axis TRUE (1) = X, FALSE (0) = Y
/// @param text optional text, for example with titles. Mainly for X axis
SHORT Screen::getCentre(bool axis, const char *text)
{
    size_t textLen = strlen(text);
    SHORT centre = 0;

    if (axis)
    {
        centre = (width - textLen) / 2;
    }
    else
    {
        centre = height / 2;
    }

    return centre;
}

/// @brief Draws a box based on inputted coordinates, can be used to draw a single line as well both vertically and horizontally
/// @param left Position from the left of window where you want box to begin
/// @param top Position from the top of window where you want box to begin
/// @param right Position from the right of window where you want box to end
/// @param bottom Position from the bottom of window where you want box to end
/// @param backgroundColour What colour the background should be, default uses 0 for black but can use BLACKGROUND_ANYTHING
/// @param foregroundColour What colour the foreground should be, default uses FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE for white but can use FOREGROUND_ANYTHING
void Screen::drawBox(
    SHORT left,
    SHORT top,
    SHORT right,
    SHORT bottom,
    WORD backgroundColour,
    WORD foregroundColour)
{
    for (int y = top; y < bottom; y++)
    {
        for (int x = left; x < right; x++)
        {
            (backgroundColour > 0) ? consoleBuffer[x + width * y].Attributes = foregroundColour | backgroundColour | BACKGROUND_INTENSITY : consoleBuffer[x + width * y].Attributes = foregroundColour | backgroundColour;
            if ((x == left && (y == top || y == bottom - 1)) || (x == right - 1 && (y == top || y == bottom - 1)))
                consoleBuffer[x + width * y].Char.AsciiChar = '+';
            else if ((x == left && y != top) || (x == right - 1 && y != top))
                consoleBuffer[x + width * y].Char.AsciiChar = '|';
            else if (y == top || y == bottom - 1)
                consoleBuffer[x + width * y].Char.AsciiChar = '-';
            else
                consoleBuffer[x + width * y].Char.AsciiChar = ' ';
        }
    }
}


void Screen::hideCursor()
{
	CONSOLE_CURSOR_INFO curInf;
	GetConsoleCursorInfo(wHand, &curInf);
	curInf.bVisible = false;
	SetConsoleCursorInfo(wHand, &curInf);
}

void Screen::showCursor()
{
	CONSOLE_CURSOR_INFO curInf;
	GetConsoleCursorInfo(wHand, &curInf);
	curInf.bVisible = true;
	SetConsoleCursorInfo(wHand, &curInf);
}

/// @brief Small drawing app, was created using the tutorial above and thought it would be funny to keep in the code as an easter egg
void Screen::funDrawingApp()
{
    WORD white =
        BACKGROUND_BLUE |
        BACKGROUND_GREEN |
        BACKGROUND_RED |
        BACKGROUND_INTENSITY;

    for (int i = 0; i < width * height; i++)
    {
        consoleBuffer[i].Char.AsciiChar = ' ';
        consoleBuffer[i].Attributes =
            BACKGROUND_BLUE |
            BACKGROUND_GREEN |
            BACKGROUND_RED |
            BACKGROUND_INTENSITY;
    }
    COORD charBufSize = {width, height};
    COORD characterPos = {0, 0};
    SMALL_RECT writeArea = {0, 0, (SHORT)(width - 1), (SHORT)(height - 1)};

    WriteConsoleOutputA(wHand, consoleBuffer, charBufSize, characterPos, &writeArea);

    DWORD numEvents = 0;
    DWORD numEventsRead = 0;

    // required to enable mouse inputs
    DWORD mode = 0;
    GetConsoleMode(rHand, &mode);

    mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode |= ENABLE_MOUSE_INPUT;
    mode |= ENABLE_EXTENDED_FLAGS;

    SetConsoleMode(rHand, mode);
    //

    bool appRunning = true;

    while (appRunning)
    {
        GetNumberOfConsoleInputEvents(rHand, &numEvents);

        if (numEvents != 0)
        {
            INPUT_RECORD *eventBuffer = new INPUT_RECORD[numEvents];

            ReadConsoleInput(rHand, eventBuffer, numEvents, &numEventsRead);

            for (DWORD i = 0; i < numEventsRead; i++)
            {
                if (eventBuffer[i].EventType == KEY_EVENT)
                {
                    KEY_EVENT_RECORD key = eventBuffer[i].Event.KeyEvent;
                    if (key.wVirtualKeyCode == VK_ESCAPE)
                    {
                        appRunning = !appRunning;
                    }
                    else if (key.uChar.AsciiChar == 'c')
                    {
                        for (int i = 0; i < width * height; i++)
                        {
                            consoleBuffer[i].Attributes = white;
                        }
                        WriteConsoleOutputA(wHand, consoleBuffer, charBufSize, characterPos, &writeArea);
                    }
                }
                else if (eventBuffer[i].EventType == MOUSE_EVENT)
                {
                    int curPos = eventBuffer[i].Event.MouseEvent.dwMousePosition.X + width * eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;

                    if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        // consoleBuffer[curPos].Char.AsciiChar = (char)0xDB; // This makes the character that is printed black but i close to use changing the background as i believe that makes more sense to me
                        consoleBuffer[curPos].Attributes = 0x0000;
                        WriteConsoleOutputA(wHand, consoleBuffer, charBufSize, characterPos, &writeArea);
                    }
                    else if (eventBuffer[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
                    {
                        consoleBuffer[curPos].Attributes = white;
                        WriteConsoleOutputA(wHand, consoleBuffer, charBufSize, characterPos, &writeArea);
                    }
                }
            }

            delete[] eventBuffer;

            // There exists 4 event types which do:
            // KEY_EVENT - keyboard event
            // MOUSE_EVENT - mouse event
            // WINDOW_BUFFER_SIZE_EVENT - handles events dealing with resizing of window
            // MENU_EVENT and FOCUS_EVENT - internal events which should be ignored
            // futher information can be sourced from this webpage https://www.benryves.com/tutorials/winconsole/3
        }
    }
}

