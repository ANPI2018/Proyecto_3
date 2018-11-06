/*
 * trazadores.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Pablo
 */

#ifndef TRAZADORES_H_
#define TRAZADORES_H_

#include <iostream>
#include <vector>

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
void makeSolutionVector(std::vector<T> dataVector,
		std::vector<T>& solutionVector, std::size_t stepSize){
	for (std::size_t i = 0 ; i < solutionVector.size() ; i++)
		solutionVector[i] = 6*((dataVector[i+2]-dataVector[i+1])/(stepSize))-6*((dataVector[i+1]-dataVector[i])/(stepSize));
}

template<class T> 
void makeMatrix(anpi::Matrix<T>& matriz, std::size_t stepSize) {
	std::size_t maxMatrixLength = matriz.cols();
	for(std::size_t i = 0 ; i < maxMatrixLength ; ++i){
		for (std::size_t j = 0 ; j < maxMatrixLength; ++j){
			if(j == i-1 || j == i+1){
				matriz[i][j] = stepSize;
			}
			else if (j == i){
				matriz[i][j] = 2*(2*stepSize);
			}
			else matriz[i][j] = T(0);	//To ensure the tridiagonallity of the matrix.
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

// Initiates all the values of a vector to 0.
template<class T> 
void initialiceVector(std::vector<T>& vector){
	for (std::size_t i = 0 ; i < vector.size() ; ++i)
		vector[i] = T(0);
}


