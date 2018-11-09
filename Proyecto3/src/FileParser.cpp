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

#include "../include/Matrix.hpp"

/**
 * @brief Enum with indexes of the borders
 */
enum Borders {
  Top, /*!< Top border */
  Bottom, /*!< Bottom border */
  Left, /*!< Left border */
  Right /*!< Right border */
};

/**
 * @brief Converts a string to lower case
 *
 * @param[in] s String
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
 * @brief Splits a line of text into individual strings
 *
 * @details Split a string given a delimiter character,
 * and put every individual string splitted in a vector
 *
 * @param[in] s String to be split
 * @param[in] delim Character used as delimiter
 * @param[out] elems Vector with individual strings
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
 * @brief Extract the data of a text file
 *
 * @details Checks with regex if the contents of the file
 * matches the expected syntax. If so, extracts the lines
 * that matches and splits them. \n
 * Returns true if the file was opened and at least one
 * line was extracted, returns false in case contrary
 *
 * @see split()
 *
 * @param[in] path Path where the file is located
 * @param[out] data e
 * @return boolean value
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
}  // extractFileData

/**
 * @brief Read a text file with the thermal profile
 * specifications
 *
 * @details Reads a text file with the specification
 * of a thermal profile and returns a vectors of
 * temperature in each border of the plaque. \n
 * Returns true if the file was readed, false
 * in case contrary.
 *
 * @param[in] path Path where the file is located
 * @param[out] temps Vectors where the data of the
 * thermal profile is returned
 *
 * @return boolean value
 */
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

/**
 * @brief Creates a file with the resulting heat
 * distribution data
 *
 * @param matrix Matrix result of the Liebmann method
 */
template<typename T>
void writeFile(::anpi::Matrix<T>& matrix) {

  std::ofstream file("Heat_distribution.txt");

  for (size_t i = 0; i < matrix.rows(); ++i) {
    for (size_t j = 0; j < matrix.cols(); ++j) {
      file << matrix[i][j] << " ";
    }
    file << "\n";
  }

  file.close();
}

