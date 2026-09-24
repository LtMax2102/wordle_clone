#include "file_interaction.hpp"
#include <ios>
#include <limits>
#include <random>
#include <iostream>
#include <chrono>


File_Interaction::File_Interaction(const char* filename) : MyFile(filename, std::ios::in)
{
  get_total_words();
  initialise_seed();
}

std::string File_Interaction::get_random_word()
{
  go_to_line(get_random_number());
  std::string s;
  MyFile >> s;
  return s;
}

void File_Interaction::get_total_words()
{
  std::string line;

  while (std::getline(MyFile, line))
    ++amount_of_words;
}

void File_Interaction::initialise_seed()
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  ran = std::mt19937(seed);
}

unsigned int File_Interaction::get_random_number()
{
  std::uniform_int_distribution<> d(0, amount_of_words - 1);
  return d(ran);
}

std::fstream& File_Interaction::go_to_line(unsigned int line)
{
  MyFile.clear();
  MyFile.seekg(0, std::ios::beg);
  for (int i = 0; i < line - 1; ++i)
  {
    MyFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return MyFile;
}
