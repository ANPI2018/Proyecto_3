/*
 * trazadores.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Pablo
 */



#include <iostream>
#include <vector>
#include "../include/Matrix.hpp"

/*
	The steps to calculate the cubic tracers are:
		1. Calculate the solution vector with the values given by the user.
		2. Calculate the values of the matrix.
		3. Apply the Thomas algorithm to solve the system:
			3.1 Decompose the matrix and recalculate its values.
			3.2 Apply a forward sustitution of the results values.
			3.3 Apply a backward sustitution to find the variable values.
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

template<class T> 
void makeSolutionVector(std::vector<T> dataVector,
		std::vector<T>& solutionVector, std::size_t stepSize){ //Solution vector of the cubic tracers method.
	for (std::size_t i = 0 ; i < solutionVector.size() ; ++i)
		solutionVector[i] = 6*((dataVector[i+2]-dataVector[i+1])/(stepSize))-
							6*((dataVector[i+1]-dataVector[i])/(stepSize));
}

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

// Initiates all the values of a vector to 0.
template<class T> 
void initialiceVector(std::vector<T>& vector){
	for (std::size_t i = 0 ; i < vector.size() ; ++i)
		vector[i] = T(0);
}


