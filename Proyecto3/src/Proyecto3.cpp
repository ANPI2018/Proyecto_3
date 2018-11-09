/**
 * @file Proyecto3.cpp
 *
 * @author Juan Pablo Brenes Coto
 *
 * @date 30-10-18
 *
 * @brief Main file with the terminal interface
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <regex>
#include <chrono>

#include "../include/AnpiConfig.hpp"
#include "../include/liebmann.cpp"
#include "FileParser.cpp"

#include <boost/program_options.hpp>
#include <boost/type_traits/is_complex.hpp>


/**
 * @brief Configuration data entered from the terminal
 *
 * @details Struct used to store the all the parameters
 * received either from terminal or from the text file
 */
struct config {
  std::vector<float> topTemp, bottomTemp, leftTemp, rightTemp;
  std::vector<bool> isolated = { 0, 0, 0, 0 };
  std::vector<int> solutionSize = { 0, 0 };
  bool show = 1;
  bool thermalFlow = 0;
  int thermalFlowSize = 0;
  float thermalConductivity;
};


/**
 * @brief Initialize the vector of isolated borders
 *
 * @details Sets the vector that specifies which borders
 * are isolated, according to the specified with the
 * option "-i" in terminal. \n
 * Returns true if the string captured with the option
 * "-i" corresponds to the accepted syntax
 *
 * @param[in] borders Borders that are isolated
 * @param[out] configuration Struct with the parameters
 *
 * @return boolean value
 */
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

/**
 * @brief Checks the priority of the options
 * received from terminal and from the text file
 *
 * @details Ensures that the information received
 * from the terminal about the temperature in each
 * border of the plaque, have priority over the
 * temperatures specifies in the text file.
 *
 * @param[out] configuration Struct with the parameters
 * @param[in] tempsInFile Vector of temperatures in the
 * borders, extracted from the text file.
 */
void checkPriority(config& configuration,
    std::vector<std::vector<float>> tempsInFile) {

  if (configuration.topTemp.size() == 0) {
    if (tempsInFile[Top].size() == 0) {
      configuration.isolated[Top] = 1;
      configuration.topTemp = {0};
    }
    else configuration.topTemp = tempsInFile[Top];
  }
  if (configuration.bottomTemp.size() == 0) {
    if (tempsInFile[Bottom].size() == 0) {
      configuration.isolated[Bottom] = 1;
      configuration.bottomTemp = {0};
    }
    else configuration.bottomTemp = tempsInFile[Bottom];
  }
  if (configuration.leftTemp.size() == 0) {
    if (tempsInFile[Left].size() == 0) {
      configuration.isolated[Left] = 1;
      configuration.leftTemp = {0};
    }
    else configuration.leftTemp = tempsInFile[Left];
  }
  if (configuration.rightTemp.size() == 0) {
    if (tempsInFile[Right].size() == 0) {
      configuration.isolated[Right] = 1;
      configuration.rightTemp = {0};
    }
    else configuration.rightTemp = tempsInFile[Right];
  }
}


void callLiebmann(config& configuration) {

  std::chrono::high_resolution_clock::time_point t1 =
      std::chrono::high_resolution_clock::now();

  ::anpi::Matrix<float> matrix;

  matrix = ::anpi::liebmann(configuration.topTemp, configuration.rightTemp,
      configuration.bottomTemp, configuration.rightTemp,
      configuration.solutionSize);

  std::chrono::high_resolution_clock::time_point t2 =
      std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  std::cout << "Duration: " << duration << std::endl;

  ::anpi::printMatrix(matrix);
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

    if (vm.count("thermalProfile")) {
      std::string filePath = vm["thermalProfile"].as<std::string>();
      readThermalFile(filePath, tempsInFile);
    }

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
    //printConfig(configuration);

    callLiebmann(configuration);


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
