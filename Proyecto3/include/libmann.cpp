/*
 * libmann.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: emi
 */

#ifndef LIBMANN_CPP
#define LIBMANN_CPP

#include <iostream>
#include "../include/Matrix.hpp"
#include <vector>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/math/constants/constants.hpp>
#include <chrono>

using namespace std;

#ifdef ANPI_ENABLE_OpenMP
const bool OMP = 1;
#else
const bool OMP = 0;
#endif


namespace anpi {

  /*
   * Imprime una matriz en consola
   */
  template<typename T>
  void printMatrix(anpi::Matrix<T>& m) {
    for (size_t i = 0; i < m.rows(); ++i) {
      for (size_t j = 0; j < m.cols(); ++j) {
        std::cout << m[i][j] << "\t";
      }
      std::cout << "\n";
    }
  }

  /*
   * Imprime un vector en consola
   */
  template<typename T>
  void printVector(std::vector<T>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
      printf("%g  ", v[i]);
    }
    printf("\n");
  }

  template<typename T>
  void printVectorMatrix(anpi::Matrix<T>& m) {
    for (size_t i = 0; i < m.rows(); ++i) {
      for (size_t j = 0; j < m.cols(); ++j) {
        printVector(m[i][j]);
        //std::cout<<m[i][j]<<"\t";
      }
      std::cout << "\n";
    }
  }

  template<typename T>
  void bordeUnValor(anpi::Matrix<T> &matriz, std::vector<T> &borde, int lado) {
    // 1 top, 2 izq, 3 der, 4 bot

    if (lado == 1) {  //top
      for (size_t j = 1; j < matriz.cols() - 1; ++j) {
        matriz(0, j) = borde[0];
      }
    }
    if (lado == 2) {  //izq
      for (size_t i = 1; i < matriz.rows() - 1; ++i) {
        matriz(i, 0) = borde[0];
      }
    }
    if (lado == 3) {  //der
      for (size_t i = 1; i < matriz.rows() - 1; ++i) {
        matriz(i, matriz.cols() - 1) = borde[0];
      }
    }
    if (lado == 4) {  //bot
      for (size_t j = 1; j < matriz.cols() - 1; ++j) {
        matriz(matriz.rows() - 1, j) = borde[0];
      }
    }
  }

  template<typename T>
  void bordeSimple(anpi::Matrix<T> &matriz, std::vector<T> &borde, int lado) {
    // 1 top, 2 izq, 3 der, 4 bot

    T total = borde[0] - borde[1];
    T paso;
    if (borde.size() == 1 && lado == 1) bordeUnValor(matriz, borde, 1);
    if (borde.size() == 1 && lado == 2) bordeUnValor(matriz, borde, 2);
    if (borde.size() == 1 && lado == 3) bordeUnValor(matriz, borde, 3);
    if (borde.size() == 1 && lado == 4) bordeUnValor(matriz, borde, 4);

    if (total < 0 && borde.size() > 1) {
      if (lado == 1) {  //top
        if (matriz.cols() == 3) {
          matriz(0, 1) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.cols() - 3);

          matriz(0, 1) = borde[0];
          for (size_t j = 2; j < matriz.cols() - 1; ++j) {
            matriz(0, j) = matriz(0, j - 1) + paso;
          }

        }
      }  //if top
      if (lado == 2) {  //left
        if (matriz.rows() == 3) {
          matriz(1, 0) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.rows() - 3);

          matriz(1, 0) = borde[0];
          for (size_t i = 2; i < matriz.rows() - 1; ++i) {
            matriz(i, 0) = matriz(i - 1, 0) + paso;
          }

        }
      }  //if left
      if (lado == 3) {  //right
        if (matriz.cols() == 3) {
          matriz(1, matriz.rows() - 1) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.rows() - 3);
          matriz(1, matriz.rows() - 1) = borde[0];
          for (size_t i = 2; i < matriz.rows() - 1; ++i) {
            matriz(i, matriz.rows() - 1) = matriz(i - 1, matriz.rows() - 1)
                + paso;
          }
        }
      }  //if right
      if (lado == 4) {  //bottom
        if (matriz.cols() == 3) {
          matriz(matriz.rows() - 1, 1) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.cols() - 3);
          matriz(matriz.rows() - 1, 1) = borde[0];
          for (size_t j = 2; j < matriz.cols() - 1; ++j) {
            matriz(matriz.rows() - 1, j) = matriz(matriz.rows() - 1, j - 1)
                + paso;
          }

        }
      }  //if bottom

    }  // segundo if total > 0
    if (total > 0 && borde.size() > 1) {
      if (lado == 1) {  //top
        if (matriz.cols() == 3) {
          matriz(0, 1) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.cols() - 3);

          matriz(0, 1) = borde[0];
          for (size_t j = 2; j < matriz.cols() - 1; ++j) {
            matriz(0, j) = matriz(0, j - 1) - paso;
          }

        }
      }  //if top
      if (lado == 2) {  //left
        if (matriz.rows() == 3) {
          matriz(1, 0) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.rows() - 3);

          matriz(1, 0) = borde[0];
          for (size_t i = 2; i < matriz.rows() - 1; ++i) {
            matriz(i, 0) = matriz(i - 1, 0) - paso;
          }

        }
      }  //if left
      if (lado == 3) {  //right
        if (matriz.cols() == 3) {
          matriz(1, matriz.rows() - 1) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.rows() - 3);
          matriz(1, matriz.rows() - 1) = borde[0];
          for (size_t i = 2; i < matriz.rows() - 1; ++i) {
            matriz(i, matriz.rows() - 1) = matriz(i - 1, matriz.rows() - 1)
                - paso;
          }
        }
      }  //if right
      if (lado == 4) {  //bottom
        if (matriz.cols() == 3) {
          matriz(matriz.rows() - 1, 1) = (borde[0] + borde[1]) / 2;
        }
        else {
          paso = abs(total) / (matriz.cols() - 3);
          matriz(matriz.rows() - 1, 1) = borde[0];
          for (size_t j = 2; j < matriz.cols() - 1; ++j) {
            matriz(matriz.rows() - 1, j) = matriz(matriz.rows() - 1, j - 1)
                - paso;
          }

        }
      }  //if bottom
    }  // primer if total < 0
  }

  template<typename T>
  void rellenarBorde(anpi::Matrix<T> &matriz, std::vector<T> &top,
      std::vector<T> &right, std::vector<T> &bottom, std::vector<T> &left) {

#   pragma omp parallel sections if((matriz.rows() >= 512) && OMP)
    {

#     pragma omp section
      {
        if (top.size() < 3) {
          bordeSimple(matriz, top, 1);
        }
      }
#     pragma omp section
      {
        if (left.size() < 3) {
          bordeSimple(matriz, left, 2);
        }
      }
#     pragma omp section
      {
        if (right.size() < 3) {
          bordeSimple(matriz, right, 3);
        }
      }
#     pragma omp section
      {
        if (bottom.size() < 3) {
          bordeSimple(matriz, bottom, 4);
        }
      }
    }
  }

  template<typename T>
  void bajarCapa(anpi::Matrix<T> &matriz, anpi::Matrix<T> &newmatriz) {

#   pragma omp parallel for collapse(2) if( (matriz.rows() >= 512) && OMP)
    for (size_t i = 1; i < matriz.rows() - 1; ++i) {  //for i
      for (size_t j = 1; j < matriz.cols() - 1; ++j) {  //for j
        newmatriz(2 * i - 1, 2 * j) = matriz(i, j);
        newmatriz(2 * i - 1, 2 * j - 1) = matriz(i, j);
        newmatriz(2 * i, 2 * j) = matriz(i, j);
        newmatriz(2 * i, 2 * j - 1) = matriz(i, j);
      }  //for j
    }  //for i
  }

  template<typename T>
  void calcularCapa(anpi::Matrix<T> &matriz, bool &converge) {
    float umbral = 0.2;

#   pragma omp parallel if ((matriz.rows() >= 512) && OMP)
    for (size_t i = 1; i < matriz.rows() - 1; ++i) {

#     pragma omp for schedule(static)
      for (size_t j = 1; j < matriz.cols() - 1; ++j) {

        T nvalor = (matriz(i + 1, j) + matriz(i - 1, j) + matriz(i, j + 1)
            + matriz(i, j - 1)) / 4;

        converge = (abs((matriz(i, j) - nvalor)) < umbral) ? true : false;
        matriz(i, j) = nvalor;

      }
    }
  }



  template<typename T>
  anpi::Matrix<T> liebmann(std::vector<T> &top, std::vector<T> &right,
      std::vector<T> &bottom, std::vector<T> &left,
      std::vector<int>& plaqueSize) {

    anpi::Matrix<T> matriz;     // Return matrix
    anpi::Matrix<T> newmatriz;  // Matrix to create the layers
    size_t max = 5;             // Max iterations per layer
    size_t contador = 0;        // Counter of iterations
    bool converge = false;      // Convergence flag
    int size = 1;               // Initial size of the matrix

    size_t maxCapas = round(log(plaqueSize[0]) / log(2));

    matriz.allocate(size + 2, size + 2);

    // n number of layers
    for (size_t n = 0; n < maxCapas; ++n) {
      rellenarBorde(matriz, top, right, bottom, left);

      //Computes one layer until it converges
      while (converge != true && contador < max) {
        calcularCapa(matriz, converge);
        //printMatrix(matriz);
        ++contador;
        //cout << "*****while*****"<<"\n";
      }  //while

      contador = 0;
      converge = false;
      size = size * 2;
      newmatriz.allocate(size + 2, size + 2);
      //rellenarBorde(newmatriz,top,right,bottom,left);
      bajarCapa(matriz, newmatriz);
      matriz = newmatriz;
      //printMatrix(matriz);
      //cout << "-------for---------"<<"\n";
    }

    rellenarBorde(matriz, top, right, bottom, left);
    return matriz;
  }

}  // anpi


/*
template<typename T>
anpi::Matrix<std::vector<T>> calcularFlujo(anpi::Matrix<T> &matriz, T k){
	T qx;
	T qy;
	T pi = boost::math::constants::pi<T>();
	anpi::Matrix<std::vector<T>> newmatriz;
	std::vector<T> valor={};
	valor.resize(2);
	cout << "---------" << endl;
	for(size_t i=1; i<matriz.rows()-1;++i ){
		for(size_t j=1; j<matriz.cols()-1;++j){
			qx = -k*((matriz(i,j+1)-matriz(i,j-1)))/(2*(matriz(i,j-1)-matriz(i,j+1)));
			qy = -k*((matriz(i+1,j)-matriz(i-1,j)))/(2*(matriz(i-1,j)-matriz(i+1,j)));
			valor[0]= sqrt((int(qx)^2)+(int(qy)^2));//magnitud
			valor[1]= atan(qx/qy)*180/pi; //angulo
			//newmatriz(i,j) = valor;
			printVector(valor);
		}cout << "xxxxxxxxxxxxx" << endl;
	}
	cout << "***********" << endl;

	return newmatriz;

}
*/


#endif /* LIBMANN_CPP_ */