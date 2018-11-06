/**
 * @file FileParser.cpp
 *
 * @author Juan Pablo Brenes Coto
 *
 * @date 5-11-18
 *
 * @brief Contains the methods for read and parse the text file
 *
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <regex>

enum Borders {
  Top, Bottom, Left, Right
};

/**
 * Converts a string to lower case
 *
 * @param s String
 *
 * @return String in lower case
 */
std::string tolower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
      [](unsigned char c) -> unsigned char {
        return std::tolower(c);
      });
  return s;
}

/**
 * Splits a line of text into individual strings
 *
 * @param s String to be split
 * @param delim Character used as delimiter
 * @param elems Vector with individual strings
 */
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (item.length() > 0) {
      elems.push_back(item);
    }
  }
}


/**
 * Extract the data of a text file
 *
 * @param[in] path Path where the file is located
 * @param[out] data e
 * @return
 */
bool extractFileData(const std::string path,
    std::vector<std::vector<std::string>>& data) {
  std::ifstream file;
  file.open(path);
  if (!file) {
    std::cout << "File not open" << std::endl;
    return false;
  }
  std::string regExp = "(top|bottom|left|right)(\\s?)+=(\\s?)+"
      "-?[[:digit:]]+(\\s?)+((-?[[:digit:]]+(\\s?)+)?)+";
  std::regex expression(regExp);

  while (!file.eof()) {
    std::string line;
    std::getline(file, line);
    if (line.empty()) continue;
    line = tolower(line);
    if (!std::regex_match(line, expression)) continue;
    std::vector<std::string> elems;
    split(line, ' ', elems);
    data.push_back(elems);
  }
  if (data.size() == 0) return false;
  return true;
}

bool readThermalFile(const std::string path,
    std::vector<std::vector<float>>& temps) {

  std::vector<std::vector<std::string>> lines;
  if (!extractFileData(path, lines)) {
    return false;
  }

  std::vector<bool> borders = { 0, 0, 0, 0 };
  for (std::size_t i = 0; i < lines.size(); ++i) {
    std::vector<std::string> line = lines[i];
    std::vector<float> temperatures;

    for (std::size_t j = 2; j < line.size(); ++j)
      temperatures.push_back(std::stof(line[j]));

    if (lines[i][0] == "top") {
      temps[Top] = temperatures;
    }
    else if (lines[i][0] == "bottom")
      temps[Bottom] = temperatures;
    else if (lines[i][0] == "left")
      temps[Left] = temperatures;
    else temps[Right] = temperatures;
  }

  return true;
}

