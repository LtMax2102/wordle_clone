#include "display.hpp"
#include "file_interaction.hpp"
#include <string>

Display::Display() : file("words/words.txt")
{
  screenRunning = true;
  game_running = true;
  answer = file.get_random_word();
}

int Display::MainLoop()
{
  Draw();

  while (screenRunning)
  {
    while (game_running)
    {
      if (guess.size() >= 5) {
        guesses += 1;
        CheckWord();
        DrawLine(character_place);
        CheckWinLoseCondition();
      }
      present();
      handleInput();
    }
    handleInput();
  }
  return 0;
}

void Display::HandleKey(KEY_EVENT_RECORD &key)
{
  if (!key.bKeyDown)
    return;

  switch (key.wVirtualKeyCode)
  {
    case VK_ESCAPE:
      screenRunning = false;
      game_running = false;
      break;
    case VK_BACK:
      if (guess.size() > 0) guess.pop_back();
      DrawGuess();
      break;
    case VK_RETURN:
      if (game_running) break;
      reset_game();
      break;
  }

  if (key.uChar.AsciiChar)
  {
    if (key.uChar.UnicodeChar < 97 || key.uChar.UnicodeChar > 122) return;
    guess.push_back(key.uChar.AsciiChar);
    DrawGuess();
  }
}

void Display::CheckWinLoseCondition()
{
  if (guess == answer)
  {
    DrawOutcome(true);
    return;
  }
  else if (guesses >= 5)
  {
    DrawOutcome(false);
    return;
  }
  guess_marker.clear();
  guess = "";
  DrawGuess();
}

// could do occurance based search where if already found will search
// for there on wards to see if another exists
// do a frequency map to see how many letters are there which should be
// this should get rid of error where when someone puts dupe letter it doesn't say there for both
void Display::CheckWord()
{
  char buf[2] = {};
  buf[1] = '\0';
  for (size_t i = 0; i < guess.size(); i++)
  {
    buf[0] = guess[i];
    drawText(i, 16, buf);
    present();

    buf[0] = answer[i];
    drawText(i, 17, buf);
    present();
    if (guess[i] == answer[i])
    {
      guess_marker.push_back(1);
    }
    else if (answer.find(guess[i]) < answer.length())
    {
      guess_marker.push_back(-1);
    }
    else
    {
      guess_marker.push_back(0);
    }
  }
}

void Display::reset_game()
{
  game_running = true;
  character_place = 0;
  guesses = 0;
  guess = "";
  answer = file.get_random_word();
  guess_marker.clear();
  Draw();
}

void Display::DrawGuess()
{
  drawText(14, 10, "            ");
  drawText(14, 10, guess.c_str());
}

void Display::DrawOutcome(bool outcome)
{
  game_running = false;
  clearScreen();
  drawBox(0, 0, 60, 13);
  std::string message = std::string("You ") + (outcome ? "Win" : "Lose");
  drawText((60 - message.size()) / 2, 1, message.c_str());
  message = "The word was " + answer;
  drawText((60 - message.size()) / 2, 4, message.c_str());
  message = "To play again press [ENTER]";
  drawText((60 - message.size()) / 2, 7, message.c_str());
  message = "To exit press [ESCAPE]";
  drawText((60 - message.size()) / 2, 10, message.c_str());
}

void Display::DrawLine(int line_num)
{
  for (int i = 0; i < 5; i++)
  {
    char buf[2] = {guess[i], '\0'};
    if (guess_marker[i] == true)
    {
      drawText(i + (2 * i), line_num, buf, 0, FOREGROUND_GREEN);
    }
    else if (guess_marker[i] == false)
    {
      drawText(i + (2 * i), line_num, buf, 0, FOREGROUND_RED);
    }
    else
    {
      drawText(i + (2 * i), line_num, buf, 0, FOREGROUND_RED | FOREGROUND_GREEN);
    }
    // drawText(i + 1 + (2 * i), line_num, "  ");
  }
  character_place += 2;
}

void Display::Draw()
{
  clearScreen();
  for (int i = 0; i < 10; i+=2)
  {
    drawText(0, i, "-  -  -  -  -");
  }


  drawText(0, 10, "ENTER GUESS>: ");
}
