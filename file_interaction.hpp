#pragma once
#include <fstream>
#include <random>
#include <string>

struct File_Interaction
{
  int amount_of_words = 0;
  std::mt19937 ran;

  std::fstream MyFile;

  File_Interaction(const char* filename);
  void get_total_words();
  void initialise_seed();
  unsigned int get_random_number();
  std::fstream& go_to_line(unsigned int line);
  std::string get_random_word();
};
