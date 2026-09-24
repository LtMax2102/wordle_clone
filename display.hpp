#pragma once
#include "customUIV1/Screen.hpp"
#include <vector>
#include <string>
#include "file_interaction.hpp"

class Display : public Screen
{
  bool game_running = false;
  File_Interaction file;
  int character_place = 0;
  int guesses = 0;
  std::string guess = "";
  std::string answer = "jamie";
  std::vector<int> guess_marker = {};
public:
  Display();
  int MainLoop();
  void Draw();
  void HandleKey(KEY_EVENT_RECORD &key);

  void DrawGuess();
  void DrawOutcome(bool outcome);
  void DrawLine(int line_num);
  void CheckWord();
  void CheckWinLoseCondition();
  void reset_game();
};
