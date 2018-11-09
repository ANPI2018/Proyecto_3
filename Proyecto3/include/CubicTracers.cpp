/*
 * trazadores.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Pablo
 */



#include <iostream>
#include <vector>
#include "../include/Matrix.hpp"

/**
 *
 * @param dataVector
 * @param size
 * @brief Método de control general de trazadores cúbicos.
 * @details Se inicia con la creación de las variables globales a utilizar,
 * después se llama a los métodos en el orden apropiado.
 * @author Pablo Bustamante Mora
 */

template<class T> 
std::vector<T> trazadores(std::vector<T> dataVector, std::size_t size) {

	std::size_t stepSize = size/dataVector.size(); //The distance between the Xs.
	std::size_t newSize = dataVector.size();
	std::vector<T> solutionVector(newSize-2); //Until x3.
	std::vector<T> secondDerivatesVector(newSize-2); //Without the first and last second derivates.
	std::vector<T> secondDerivatesComplete(newSize); //[0,secondDerivatesVector,0]
	anpi::Matrix<T> tridiagonalMatrix[newSize-1][newSize-1];


	initialiceVector(solutionVector);
	initialiceVector(secondDerivatesVector);
	makeSolutionVector(dataVector, solutionVector, stepSize);
	makeMatrix(tridiagonalMatrix, stepSize);

	thomas(tridiagonalMatrix,secondDerivatesVector,solutionVector);

	for (std::size_t i = 0 ; i < secondDerivatesComplete.size() ; ++i){ //Adds the first and second derivates. =0.
		if (i == 0 || i == secondDerivatesComplete.size() -1)
			secondDerivatesComplete[i] = 0;
		else
			secondDerivatesComplete[i] = secondDerivatesVector[i-1];
	}
	std::vector<T> finalTemperatureVector(size);
	initialiceVector(finalTemperatureVector);
	for (int i = 0 ; i <= size ; ++i)
		finalTemperatureVector[i] = calculateTemperature(i,stepSize,secondDerivatesComplete, dataVector);
	return finalTemperatureVector;

}

/**
 * @brief Método que genera el vector solución del sistema de trazadores cúbicos.
 * @details Utiliza la fórmula obtenida por el despeje de la integración de segundo
 * orden de los polinomios de Lagrange.
 * @param dataVector
 * @param solutionVector
 * @param stepSize
 * @author Pablo Bustamante Mora
 */
template<class T> 
void makeSolutionVector(std::vector<T> dataVector,
		std::vector<T>& solutionVector, std::size_t stepSize){ //Solution vector of the cubic tracers method.
	for (std::size_t i = 0 ; i < solutionVector.size() ; ++i)
		solutionVector[i] = 6*((dataVector[i+2]-dataVector[i+1])/(stepSize))-
							6*((dataVector[i+1]-dataVector[i])/(stepSize));
}

/**
 * @brief Método que calcula la matriz tridiagonal del método de trazadores cúbicos.
 * @details Utiliza los valores de la cantidad de filas y columnas de la matriz para iterar
 * y poder llenar la matriz. En la diagonal se tiene el doble del tamaño de paso entre dos puntos;
 * en la diagonal superior e inferior se tiene únicamente el tamaño de paso, y en las otras posiciones
 * se tiene 0.
 * @param dataVector
 * @param matriz
 * @param solutionVector
 * @author Pablo Bustamante Mora
 */
template<class T> 
void makeMatrix(anpi::Matrix<T>& matriz, std::size_t stepSize) { //Matrix of the cubic tracerse method.
	std::size_t maxRows = matriz.size1();
	std::size_t maxCols = matriz.size2();
	for(std::size_t i = 0 ; i < maxRows ; ++i){
		for (std::size_t j = 0 ; j < maxCols; ++j){
			if(j == i-1 || j == i+1) {matriz[i][j] = stepSize;} //(xi-x(i-1))
			else if (j == i){ matriz[i][j] = 2*(2*stepSize);} //2(xi-x(i-2))
			else {
				matriz[i][j] = T(0); //It has to be a tridiagonal matrix.
			}
		}
	}
}

/**
 * @brief Resuelve una matriz tridiagonal por medio del método de Thomas.
 * @param matriz
 * @param variablesVector
 * @param solutionVector
 * @author Pablo Bustamante Mora
 */
template<class T> 
void thomas(anpi::Matrix<T>& matriz, std::vector<T> variablesVector,
			std::vector<T>& solutionVector){

	std::size_t n = solutionVector.size();

	for(std::size_t k = 1; k < n ; ++k){ //Decomposition of the matrix
		std::size_t j = k-1;
		matriz[k][j] /= matriz[k-1][j];	//ek /= f(k-1)
		matriz[k][k] -= matriz[k][j]*matriz[k-1][j+1];	//fk -= ek*g(k-1)
	}

	for (std::size_t k = 1; k < n ;++k){	//Forward substitution
		std::size_t j = k-1;
		solutionVector[k] -= matriz[k][j]*solutionVector[k-1]; //rk −= ek*r(k−1)
	}
	
	variablesVector[n-1] = solutionVector[n-1]/matriz(n-1,n-1); //xn = rn/fn
	for (std::size_t k = n-2 ; k > 0 ; --k){	//Backward substitution
		variablesVector[k] = (solutionVector[k]-(matriz(k,k+1)*variablesVector[k+1]))/matriz(k,k); //xk = (rk − gk*x(k+1))/fk
	}
	variablesVector[0] = (solutionVector[0]-(matriz(0,1)*variablesVector[1]))/matriz(0,0); //First value of the variables vector
}

/**
 * @brief Método que calcula la temperatura solicitada.
 * @details Utiliza el valor a evaluar para determinar cuál polinomio utilizar. Asimismo utiliza
 * el vector de segundas derivadas, el vector de datos ingresados
 * y calcula el vector de pasos.
 * poder determinar
 * @param value
 * @param stepSize
 * @param secondDerivates
 * @param dataVector
 * @return valor de la temperatura en cuestión.
 * @author Pablo Bustamante Mora.
 */
template<class T> 
int calculateTemperature(std::size_t value, std::size_t stepSize,
		std::vector<T>& secondDerivates, std::vector<T>& dataVector){
	std::vector<T> stepVector(dataVector.size());
	for (std::size_t i = 0 ; i < stepVector.size() ; ++i) //Creates a vector with the Xs.
		stepVector[i] = stepSize*i;
	int i = 1;
	while (value > i*stepSize)	++i; //Determines which polynomial has to be used.
	int operation1,operation2, operation3, operation4;
	operation1 = -secondDerivates[i-1]*(pow(value - stepVector[i],3)/(6*stepSize));
	operation2 = secondDerivates[i]*(pow(value - stepVector[i-1],3)/(6*stepSize));
	operation3 = ((-dataVector[i-1]/stepSize)+((secondDerivates[i-1]*stepSize)/6))*(value-stepVector[i]);
	operation4 = ((dataVector[i]/stepSize)-((secondDerivates[i]*stepSize)/6))*(value-stepVector[i-1]);
	int temperatura = operation1 + operation2 + operation3 + operation4;
	return temperatura;

}

/**
 * @brief Initialices a vector.
 * @details Sets all the elements of a vector to 0.
 * @param vector
 * @author Pablo Bustamante Mora.
 */
template<class T> 
void initialiceVector(std::vector<T>& vector){
	for (std::size_t i = 0 ; i < vector.size() ; ++i)
		vector[i] = T(0);
}


