#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <regex>

#include <boost/program_options.hpp>
#include <boost/type_traits/is_complex.hpp>

//  std::regex expression(
//      "(top|bottom|left|right)(\\s?)+=(\\s?)+-?[[:digit:]]+(\\s?)+((-?[[:digit:]]+(\\s?)+)?)+");

enum Borders {
  Top, Bottom, Left, Right
};

struct config {
  std::vector<float> topTemp, bottomTemp, leftTemp, rightTemp;
  std::vector<bool> isolated = { 0, 0, 0, 0 };
  std::vector<int> solutionSize = { 0, 0 };
  bool show = 1;
  bool thermalFlow = 0;
  int thermalFlowSize = 0;
  float thermalConductivity;
};

/// Convert the given string to lowercase
std::string tolower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
      [](unsigned char c) -> unsigned char {
        return std::tolower(c);
      });
  return s;
}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (item.length() > 0) {
      elems.push_back(item);
    }
  }
}

bool extractFileData(const std::string path,
    std::vector<std::vector<std::string>>& data) {
  std::ifstream file;
  file.open(path);
  if (!file) {
    std::cout << "File not open" << std::endl;
    return false;
  }
  std::string regExp =
      "(top|bottom|left|right)(\\s?)+=(\\s?)+"
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
    else
      temps[Right] = temperatures;
  }

  return true;
}

bool checkIsolatedBorders(std::string borders, config& configuration) {
  std::regex expression("(t)?(b)?(l)?(r)?$");
  if (!std::regex_match(borders, expression)) return false;

  for (char& c : borders) {
    if (c == 't') configuration.isolated[Top] = 1;
    else if (c == 'b')
      configuration.isolated[Bottom] = 1;
    else if (c == 'l')
      configuration.isolated[Left] = 1;
    else configuration.isolated[Right] = 1;
  }
  return true;
}


void checkPriority(config& configuration,
    std::vector<std::vector<float>> tempsInFile) {

  if (configuration.topTemp.size() == 0) {
    if (tempsInFile[Top].size() == 0) {
      configuration.isolated[Top] = 1;
    }
    else configuration.topTemp = tempsInFile[Top];
  }
  if (configuration.bottomTemp.size() == 0) {
    if (tempsInFile[Bottom].size() == 0)
      configuration.isolated[Bottom] = 1;
    else configuration.bottomTemp = tempsInFile[Bottom];
  }
  if (configuration.leftTemp.size() == 0) {
    if (tempsInFile[Left].size() == 0)
      configuration.isolated[Left] = 1;
    else configuration.leftTemp = tempsInFile[Left];
  }
  if (configuration.rightTemp.size() == 0) {
    if (tempsInFile[Right].size() == 0)
      configuration.isolated[Right] = 1;
    else configuration.rightTemp = tempsInFile[Right];
  }
}

void printConfig(config configuration) {
  std::cout << "Temperatures in borders: " << std::endl;
  std::cout << "Top: ";
  for (std::size_t i = 0; i < configuration.topTemp.size(); ++i)
    std::cout << configuration.topTemp[i] << " ";

  std::cout << "\nBottom: ";
  for (std::size_t i = 0; i < configuration.bottomTemp.size(); ++i)
    std::cout << configuration.bottomTemp[i] << " ";

  std::cout << "\nLeft: ";
  for (std::size_t i = 0; i < configuration.leftTemp.size(); ++i)
    std::cout << configuration.leftTemp[i] << " ";

  std::cout << "\nRight: ";
  for (std::size_t i = 0; i < configuration.rightTemp.size(); ++i)
    std::cout << configuration.rightTemp[i] << " ";

  std::cout << "\nIsolated borders: ";
  for (std::size_t i = 0; i < configuration.isolated.size(); ++i)
    std::cout << configuration.isolated[i] << " ";

  std::cout << "\nSolution size: ";
  for (std::size_t i = 0; i < configuration.solutionSize.size(); ++i)
    std::cout << configuration.solutionSize[i] << " ";

  std::cout << "\nShow results: " << configuration.show << std::endl;

  std::cout << "Thermal Flow: " << configuration.thermalFlow << std::endl;

  std::cout << "Thermal flow size: " << configuration.thermalFlowSize
      << std::endl;

  std::cout << "Thermal conductivity: " << configuration.thermalConductivity
      << std::endl;
}



namespace bmt = boost::math::tools;
// for polynomial

namespace po = boost::program_options;

////////////////////////////////////////////////////////////////////////////
//  Main program
////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

  try {
    po::options_description desc("Allowed options");
    desc.add_options()

    ("help, h", "Help information")

    ("topTemperature,t", po::value<std::vector<float>>()->multitoken(),
        "Temperature in the upper border")

    ("bottomTemperature,b", po::value<std::vector<float>>()->multitoken(),
        "Temperature in the lower border")

    ("leftTemperature,l", po::value<std::vector<float>>()->multitoken(),
        "Temperature in the left border")

    ("rightTemperature,d", po::value<std::vector<float>>()->multitoken(),
        "Temperature in the right border")

    ("ThermalConductivity,k", po::value<float>()->required(),
        "Thermal conductivity of the material")

    ("isolate,i", po::value<std::string>()->implicit_value(""),
        "Isolate borders")

    ("thermalProfile,p", po::value<std::string>(),
        "File with a thermal profile")

    ("horizontalPix,h", po::value<int>()->required(),
        "Number of horizontal pixels of the solution")

    ("verticalPix,v", po::value<int>()->required(),
        "Number of vertical pixels of the solution")

    ("deactivateVisual,q", "Deactivate all forms of visualizations")

    ("thermalFlow,f", "Activate the visualization of the heat flow")

    ("thermalGrid,g", po::value<int>()->default_value(-1),
        "Size of the grid for the visualization of the heat flow");

    po::variables_map vm;

    po::store(parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n\n";
      std::cout << "Show info" << std::endl;

      return EXIT_SUCCESS;
    }

    po::notify(vm);
    config configuration;
    std::vector<std::vector<float>> tempsInFile = { { }, { }, { }, { } };

    if (vm.count("topTemperature")) {
      std::vector<float> top = vm["topTemperature"].as<std::vector<float>>();
      configuration.topTemp = top;
    }

    if (vm.count("bottomTemperature")) {
      std::vector<float> bottom =
          vm["bottomTemperature"].as<std::vector<float>>();
      configuration.bottomTemp = bottom;
    }

    if (vm.count("leftTemperature")) {
      std::vector<float> left = vm["leftTemperature"].as<std::vector<float>>();
      configuration.leftTemp = left;
    }

    if (vm.count("rightTemperature")) {
      std::vector<float> right =
          vm["rightTemperature"].as<std::vector<float>>();
      configuration.rightTemp = right;
    }

    if (vm.count("ThermalConductivity")) {
      float thermalConductivity = vm["ThermalConductivity"].as<float>();
      configuration.thermalConductivity = thermalConductivity;
    }

    if (vm.count("isolate")) {
      std::string isolate = vm["isolate"].as<std::string>();
      if (isolate.empty())
        throw po::error(
            "At least one border should be specified with the option \"-i\"");

      if (!checkIsolatedBorders(isolate, configuration))
        throw po::error("Unknown borders specified");
    }

    if (vm.count("thermalProfile")) {
      std::string filePath = vm["thermalProfile"].as<std::string>();
      readThermalFile(filePath, tempsInFile);
    }

    if (vm.count("horizontalPix")) {
      int hSize = vm["horizontalPix"].as<int>();
      configuration.solutionSize[0] = hSize;
    }

    if (vm.count("verticalPix")) {
      int vSize = vm["verticalPix"].as<int>();
      configuration.solutionSize[1] = vSize;
    }

    if (vm.count("deactivateVisual")) {
      configuration.show = 1;
    }

    if (vm.count("thermalFlow")) {
      configuration.thermalFlow = 1;

      if (vm.count("thermalGrid")) {
        int flowSize = vm["thermalGrid"].as<int>();
        if (flowSize <= 0)
          throw po::error(
              "The size of the grid has to be set, and couldn't be negative or zero");
        configuration.thermalFlowSize = flowSize;
      }
    }


    checkPriority(configuration, tempsInFile);
    printConfig(configuration);

  } catch (po::error& e) {
    std::cerr << "Error:\n  " << e.what() << std::endl << std::endl;
    return EXIT_FAILURE;
  } catch (std::exception& e) {
    std::cerr << "Unhandled Exception reached the top of main:\n  " << e.what()
        << "\nApplication will now exit" << std::endl;
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
