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
#include "../include/perfiltermico.cpp"
#include "FileParser.cpp"

#include <boost/program_options.hpp>
#include <boost/type_traits/is_complex.hpp>
#include "../include/liebmann.cpp"

typedef std::chrono::high_resolution_clock::time_point Time;


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
  int thermalFlowSize = 0;
  float thermalConductivity;
  bool show = 1;
  bool thermalFlow = 0;
  bool measureTime = 0;
};


/**
 * @brief Initialize the vector of isolated borders
 *
 * @details Sets the vector that specifies which borders
 * are isolated, according to the specified with the
 * option "-i" in terminal. \n
 *
 * @param[in] borders Borders that are isolated
 * @param[out] conf Struct with the parameters
 *
 */
void checkIsolatedBorders(std::string borders, config& conf) {

  for (char& c : borders) {
    switch (c) {
      case 't':
        conf.isolated[Top] = 1;
        conf.topTemp = {0}; break;
      case 'b' : conf.isolated[Bottom] = 1; conf.bottomTemp = {0}; break;
      case 'l' : conf.isolated[Left] = 1; conf.leftTemp = {0}; break;
      case 'r' : conf.isolated[Right] = 1; conf.rightTemp = {0}; break;
        default : std::cout << "Unknown border: '" << c << "' ignored in command \"-i\"\n" <<
        "Borders are: 't' -> Top 'b' -> Bottom 'l' -> Left 'r' -> Right"<< std::endl;
    }
  }
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
   * @param[out] conf Struct with the parameters
 * @param[in] tempsInFile Vector of temperatures in the
 * borders, extracted from the text file.
 */
void checkPriority(config& conf, std::vector<std::vector<float>> tempsInFile) {

  if (conf.topTemp.size() == 0) {
    if (tempsInFile[Top].size() == 0) {
    conf.isolated[Top] = 1;
    conf.topTemp = {0};
    }
    else {
      if (conf.isolated[Top]) conf.topTemp = {0};
      else conf.topTemp = tempsInFile[Top];
    }
  }
  if (conf.bottomTemp.size() == 0) {
    if (tempsInFile[Bottom].size() == 0) {
    conf.isolated[Bottom] = 1;
    conf.bottomTemp = {0};
    }
    else {
      if(conf.isolated[Bottom]) conf.bottomTemp = {0};
      else conf.bottomTemp = tempsInFile[Bottom];
    }
  }
  if (conf.leftTemp.size() == 0) {
    if (tempsInFile[Left].size() == 0) {
    conf.isolated[Left] = 1;
    conf.leftTemp = {0};
    }
    else {
      if(conf.isolated[Left]) conf.leftTemp = {0};
      else conf.leftTemp = tempsInFile[Left];
    }
  }
  if (conf.rightTemp.size() == 0) {
    if (tempsInFile[Right].size() == 0) {
    conf.isolated[Right] = 1;
    conf.rightTemp = {0};
    }
    else {
      if(conf.isolated[Right]) conf.rightTemp = {0};
      else conf.rightTemp = tempsInFile[Right];
    }
  }
}

/**
 * @brief Obtains the minimum and maximum values of 4 vectors
 *
 * @param[in] conf Struct that holds the vectors
 * @param[out] min Value with minimum
 * @param[out] max Value with maximum
 */
void getMinMax(const config conf, int& min, int& max) {

  auto minMaxT = std::minmax_element(conf.topTemp.begin(), conf.topTemp.end());
  auto minMaxB = std::minmax_element(conf.bottomTemp.begin(),
      conf.bottomTemp.end());
  auto minMaxL = std::minmax_element(conf.leftTemp.begin(),
      conf.leftTemp.end());
  auto minMaxR = std::minmax_element(conf.rightTemp.begin(),
      conf.rightTemp.end());

  std::vector<float> mins = {
      conf.topTemp[minMaxT.first - conf.topTemp.begin()],
      conf.bottomTemp[minMaxB.first - conf.bottomTemp.begin()],
      conf.leftTemp[minMaxL.first - conf.leftTemp.begin()],
      conf.rightTemp[minMaxR.first - conf.rightTemp.begin()] };

  std::vector<float> maxs = {
      conf.topTemp[minMaxT.second - conf.topTemp.begin()],
      conf.bottomTemp[minMaxB.second - conf.bottomTemp.begin()],
      conf.leftTemp[minMaxL.second - conf.leftTemp.begin()],
      conf.rightTemp[minMaxR.second - conf.rightTemp.begin()] };

  auto minIdx = std::min_element(mins.begin(), mins.end());
  auto maxIdx = std::max_element(maxs.begin(), maxs.end());

  min = mins[std::distance(std::begin(mins), minIdx)];
  max = maxs[std::distance(std::begin(maxs), maxIdx)];

}

/**
 * @brief Call the Liebmann method with the configuration given
 * in the terminal
 *
 * @details Call the Liebmann method to obtain the heat distribution
 * in the plaque. \n If the flag to measure the execution time is on,
 * the Liebmann algorithm will be executed but no results will be shown,
 * only prints the execution time in terminal. \n If the flag is off, the
 * the algorithm will be executed and if the flag to show results is on,
 * the plot of the result will be shown in a new window.
 *
 * @param[in] configuration Struct with the parameters entered
 * in terminal
 */
void callLiebmann(config& configuration) {

  ::anpi::Matrix<float> matrix;

  if (configuration.measureTime) {
    Time t1 = std::chrono::high_resolution_clock::now();

    matrix = ::anpi::liebmann(configuration.topTemp, configuration.rightTemp,
        configuration.bottomTemp, configuration.leftTemp,
        configuration.solutionSize);

    Time t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        t2 - t1).count();

    std::cout << "\n--- Execution time: " << duration << "us" << std::endl;
  }

  else {
    matrix = ::anpi::liebmann(configuration.topTemp, configuration.rightTemp,
        configuration.bottomTemp, configuration.leftTemp,
        configuration.solutionSize);

    if (configuration.show) {
      int min, max;
      getMinMax(configuration, min, max);
      mostrarPerfil(matrix, min, max);
    }
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

  std::cout << "Measure Time: " << configuration.measureTime << std::endl;


}



namespace po = boost::program_options;

/**
 * @brief Command line interface
 *
 * @param argc Argument count
 * @param argv Argument vector
 *
 * @return program exit state
 */
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
        "Size of the grid for the visualization of the heat flow")

    ("measureTime,m",
        "Measure the execution time in microseconds");

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

      checkIsolatedBorders(isolate, configuration);
    }

    if (vm.count("horizontalPix")) {
      int hSize = vm["horizontalPix"].as<int>();
      if (hSize <= 0)
        throw po::error("Horizontal dimension has to be greater than 0");
      else configuration.solutionSize[0] = hSize;
    }

    if (vm.count("verticalPix")) {
      int vSize = vm["verticalPix"].as<int>();
      if (vSize <= 0)
        throw po::error("Vertical dimension has to be greater than 0");
      else
        configuration.solutionSize[1] = vSize;
    }

    if (vm.count("deactivateVisual")) {
      configuration.show = 0;
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

    if (vm.count("measureTime")) {
      configuration.measureTime = 1;
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
