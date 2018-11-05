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

template<class T> 
void cubicTracers(std::vector<T> dataVector,
 anpi::Matrix<T>& matrix, std::vector<T>& solutionVector) {
	std::size_t n = 2;
	std::size_t maximumRows = matrix.size1();
	std::size_t maximumCols = matrix.size2();
	for(std::size_t i = 0 ; i < maximumRows ; ++i){
		for (std::size_t j = 0 ; j < maximumCols; ++j){
			if(j == i-1){
				matrix(i,j) = dataVector[n-1]-dataVector[n-2];
			}
			else if (j == i){
				matrix(i,j) = 2*(dataVector[n] - dataVector[n-2]);
			}
			else if(j == i+1){
				matrix(i,j) = dataVector[n] - dataVector[n-1];
			}
			else matrix(i,j) = T(0);
		}
		++n;
	}
	for(std::size_t i = 0 ; i < solutionVector.size() ; ++i)
			solutionVector[i] = (6*(((dataVector[i+2])-(dataVector[i+1]))/((3)-(4))))-(6*(((dataVector[i+1])-(dataVector[i]))/((7)-(8))));
}

template<class T> 
void thomas(anpi::Matrix<T>& matrix, std::vector<T> variablesVector,
			std::vector<T>& solutionVector){

	std::size_t n = solutionVector.size();

	for(std::size_t k = 1; k < n ; ++k){ //Decomposition
		std::size_t j = k-1;
		matrix(k,j) /= matrix(k-1,j);	//ek /= f(k-1)
		matrix(k,k) -= matrix(k,j)*matrix(k-1,j+1);	//fk -= ek*g(k-1)
	}
	for (std::size_t k = 1; k < n ;++k){	//Forward substitution
		std::size_t j = k-1;
		solutionVector[k] -= matrix(k,j)*solutionVector[k-1];
	}

	variablesVector[n-1] = solutionVector[n-1]/matrix(n-1,n-1);
	std::size_t i = n-2;
	std::size_t j = n-1;
	for (std::size_t k = n-2 ; k > 0 ; --k){	//Backward substitution
		variablesVector[k] = (solutionVector[k]-(matrix(i,j)*variablesVector[k+1]))/matrix(k,k);
		--i;
		--j;
	}
	variablesVector[0] = (solutionVector[0]-(matrix(0,1)*variablesVector[1]))/matrix(0,0);
}
