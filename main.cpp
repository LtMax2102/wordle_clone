#include "display.hpp"

int main()
{
  Screen::Initialise(80, 40, "Wordle Game");
  Display d;
  d.MainLoop();


  Screen::ShutDown();

  return 0;
}
