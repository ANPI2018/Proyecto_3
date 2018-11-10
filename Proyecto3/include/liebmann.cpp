/*
 * libmann.cpp
 *
 *  Created on: Nov 4, 2018
 *      Author: Emilly Sancho Murillo
 */

#ifndef LIBMANN_CPP_
#define LIBMANN_CPP_

#include <iostream>
#include "../include/Matrix.hpp"
#include <vector>
#include <math.h>
#include <boost/math/constants/constants.hpp>
#include "../include/CubicTracers.cpp"

using namespace std;

#ifdef ANPI_ENABLE_OpenMP
const bool OMP = 1;
#else
const bool OMP = 0;
#endif

namespace anpi {

enum borders{top, bottom, left, right};

/**
 * @brief Este metodo imprime una matriz en consola
 * @author Juan Brenes.
 * @param anpi::Matrix<T>& m
 */
template<typename T>
void printMatrix(anpi::Matrix<T>& m){
	for(size_t i=0;i<m.rows();++i){
		for(size_t j=0;j<m.cols();++j){
			std::cout<<m[i][j]<<"\t";
		}std::cout<<"\n";
	}
}

/**
 * @brief Este metodo imprime un vector en consola
 * @author Juan Brenes.
 * @param std::vector<T>& v
 */
template<typename T>
void printVector(std::vector<T>& v){
	for(size_t i=0;i<v.size();++i){
			printf("%g  ",v[i]);
		}printf("\n");
}

/**
 * @brief Rellena un borde con un unico valor.
 * @details Este es un metodo que funciona como auxiliar del metodo borde simple,
 * toma el valor del lado y lo ubica en cada celda del borde de la matriz,
 * segun se especifique con el int lado.
 * Para este metodo se espera que el vector borde contenga unicamente un valor.
 * @author Emily Sancho.
 * @param anpi::Matrix<T> &matriz
 * @param std::vector<T> &borde
 * @param int lado
 */
template<typename T>
void bordeUnValor(anpi::Matrix<T> &matriz, std::vector<T> &borde, int lado){
// 1 top, 2 izq, 3 der, 4 bot

	if(lado == 1){//top
		for(size_t j=1; j<matriz.cols()-1;++j){
			matriz(0,j) = borde[0];
		}
	}
	if(lado == 2){//izq
		for(size_t i=1;i<matriz.rows()-1;++i){
			matriz(i,0) = borde[0];
		}
	}
	if(lado == 3){//der
		for(size_t i=1;i<matriz.rows()-1;++i){
			matriz(i,matriz.cols()-1) = borde[0];
		}
	}
	if(lado == 4){//bot
		for(size_t j=1;j<matriz.cols()-1;++j){
			matriz(matriz.rows()-1,j) = borde[0];
		}
	}
}


/**
 * @brief Rellena un borde con uno o dos valores.
 * @details Este metodo recibe un vector que indica las temperaturas de un borde,
 * segun el lado y dependiendo del tamaño de ese vector rellena el borde,
 * llamando a bordeSimple cuando el vector es de tamaño 1 o rellenando el borde
 * in sutu cuando el vector tiene mas de dos valores.
 * @author Emily Sancho
 * @param anpi::Matrix<T> &matriz
 * @param std::vector<T> &borde
 * @param int lado
 *
 */
template<typename T>
void bordeSimple(anpi::Matrix<T> &matriz, std::vector<T> &borde, int lado){
// 1 top, 2 izq, 3 der, 4 bot

	T total=borde[0]-borde[1];
	T paso;
	if(borde.size()==1 && lado==1)bordeUnValor(matriz,borde,1);
	if(borde.size()==1 && lado==2)bordeUnValor(matriz,borde,2);
	if(borde.size()==1 && lado==3)bordeUnValor(matriz,borde,3);
	if(borde.size()==1 && lado==4)bordeUnValor(matriz,borde,4);

	if(total<0 && borde.size()>1){
		if(lado == 1){//top
			if(matriz.cols()==3){
				matriz(0,1)=(borde[0]+borde[1])/2;
			}else{
				paso = abs(total)/(matriz.cols()-3);

				matriz(0,1)=borde[0];
				for(size_t j=2; j<matriz.cols()-1;++j){
				matriz(0,j) = matriz(0,j-1)+paso;
				}

			}
		}//if top
		if(lado == 2){//left
					if(matriz.rows()==3){
									matriz(1,0)=(borde[0]+borde[1])/2;
					}else{
						paso = abs(total)/(matriz.rows()-3);

						matriz(1,0)=borde[0];
						for(size_t i=2; i<matriz.rows()-1;++i){
						matriz(i,0) = matriz(i-1,0)+paso;
						}

					}
		}//if left
		if(lado == 3){//right
					if(matriz.cols()==3){
								matriz(1,matriz.rows()-1)=(borde[0]+borde[1])/2;
					}else{
						paso = abs(total)/(matriz.rows()-3);
						matriz(1,matriz.rows()-1)=borde[0];
						for(size_t i=2; i<matriz.rows()-1;++i){
						matriz(i,matriz.rows()-1) = matriz(i-1,matriz.rows()-1)+paso;
						}
					}
		}//if right
		if(lado == 4){//bottom
					if(matriz.cols()==3){
									matriz(matriz.rows()-1,1)=(borde[0]+borde[1])/2;
					}else{
						paso = abs(total)/(matriz.cols()-3);
						matriz(matriz.rows()-1,1)=borde[0];
						for(size_t j=2; j<matriz.cols()-1;++j){
						matriz(matriz.rows()-1,j) = matriz(matriz.rows()-1,j-1)+paso;
						}

					}
		}//if bottom

	}// segundo if total > 0
	if(total>0 && borde.size()>1){
		if(lado == 1){//top
			if(matriz.cols()==3){
							matriz(0,1)=(borde[0]+borde[1])/2;
			}else{
				paso = abs(total)/(matriz.cols()-3);

				matriz(0,1)=borde[0];
				for(size_t j=2; j<matriz.cols()-1;++j){
				matriz(0,j) = matriz(0,j-1)-paso;
				}

			}
		}//if top
		if(lado == 2){//left
					if(matriz.rows()==3){
									matriz(1,0)=(borde[0]+borde[1])/2;
					}else{
						paso = abs(total)/(matriz.rows()-3);

						matriz(1,0)=borde[0];
						for(size_t i=2; i<matriz.rows()-1;++i){
						matriz(i,0) = matriz(i-1,0)-paso;
						}

					}
		}//if left
		if(lado == 3){//right
					if(matriz.cols()==3){
								matriz(1,matriz.rows()-1)=(borde[0]+borde[1])/2;
					}else{
						paso = abs(total)/(matriz.rows()-3);
						matriz(1,matriz.rows()-1)=borde[0];
						for(size_t i=2; i<matriz.rows()-1;++i){
						matriz(i,matriz.rows()-1) = matriz(i-1,matriz.rows()-1)-paso;
						}
					}
		}//if right
		if(lado == 4){//bottom
					if(matriz.cols()==3){
									matriz(matriz.rows()-1,1)=(borde[0]+borde[1])/2;
					}else{
						paso = abs(total)/(matriz.cols()-3);
						matriz(matriz.rows()-1,1)=borde[0];
						for(size_t j=2; j<matriz.cols()-1;++j){
						matriz(matriz.rows()-1,j) = matriz(matriz.rows()-1,j-1)-paso;
						}

					}
		}//if bottom
	}// primer if total < 0
}

/**
 * @brief Rellena los bordes de la matriz, con los valores correspondientes.
 * @details Este metodo toma los valores correspondientes a los bordes de la matriz y los
 * rellena con ayuda de los metodos bordeSimple y trazadores, respetando tamaño
 * del borde y orden ascendente o decreciente del mismo.
 * @author Emily Sancho.
 * @param anpi::Matrix<T>& matriz
 * @param std::vector<T> &top
 * @param std::vector<T> &right
 * @param std::vector<T> &bottom
 * @param std::vector<T> &left
 */
template<typename T>
void rellenarBorde(anpi::Matrix<T>& matriz,
		std::vector<T> &Top,
		std::vector<T> &Right,
		std::vector<T> &Bottom,
		std::vector<T> &Left){

		std::vector<T> temperaturesVectorTop(matriz.rows());
		std::vector<T> temperaturesVectorLeft(matriz.rows());
		std::vector<T> temperaturesVectorBottom(matriz.rows());
		std::vector<T> temperaturesVectorRight(matriz.rows());


#pragma omp parallel sections if((matriz.rows() >= 512) && OMP)
    {

#pragma omp section
      {
        if (Top.size() < 3) {
          bordeSimple(matriz, Top, 1);
        }
				else{
					temperaturesVectorTop = trazadores(Top, matriz.rows());
					rellenarBordeTrazador(matriz,top,temperaturesVectorTop);
				}
      }
#pragma omp section
      {
        if (Left.size() < 3) {
          bordeSimple(matriz, Left, 2);
        }
				else{
					temperaturesVectorLeft = trazadores(Left, matriz.rows());
					rellenarBordeTrazador(matriz,left,temperaturesVectorLeft);
				}
      }
#pragma omp section
      {
        if (Right.size() < 3) {
          bordeSimple(matriz, Right, 3);
        }
				else{
					temperaturesVectorRight = trazadores(Right, matriz.rows());
					rellenarBordeTrazador(matriz,right,temperaturesVectorRight);
				}
      }
#pragma omp section
      {
        if (Bottom.size() < 3) {
          bordeSimple(matriz, Bottom, 4);
        }
				else{
					temperaturesVectorBottom = trazadores(Bottom, matriz.rows());
					rellenarBordeTrazador(matriz,bottom,temperaturesVectorBottom);
				}
      }
    }
  }

/**
 * @brief Toma matriz y copia el valor de cada una de sus celdas en 4 celdas de la newmatriz
 * @details Cada vez que el metodo de Libman calcula una capa inferior de la placa, cada celda la capa
 * se duplica en 4 celdas de la nueva capa, este metodo se encarga de implementar este comportamiento.
 * @author Pablo Bustamante Mora.
 * @param matriz
 * @param border
 * @param temperatureVector
 */
template<typename T>
void rellenarBordeTrazador(anpi::Matrix<T>& matriz, int border, std::vector<T> temperatureVector){

	if(border == (top)){
		for(std::size_t j = 0 ; j < matriz.rows() ; ++j)
			matriz[0][j] = temperatureVector[j];
	}
	else if(border == (left)){
		for(std::size_t i = 0 ; i < matriz.rows() ; ++i)
			matriz[i][0] = temperatureVector[i];
	}
	else if(border == (bottom)){
		for(std::size_t j = 0 ; j < matriz.rows() ; ++j)
			matriz[matriz.cols()-1][j] = temperatureVector[j];
	}
	else{
		for(std::size_t i = 0 ; i < matriz.rows() ; ++i)
					matriz[i][matriz.rows()-1] = temperatureVector[i];
	}

}

/**
 * @brief Toma matriz y copia el valor de cada una de sus celdas en 4 celdas de la newmatriz
 * @details Cada vez que el metodo de Libman calcula una capa inferior de la placa, cada celda la capa
 * se duplica en 4 celdas de la nueva capa, este metodo se encarga de implementar este comportamiento.
 * @author Emily Sancho.
 * @param anpi::Matrix<T> &matriz
 * @param anpi::Matrix<T> &newmatriz
 */
template<typename T>
void bajarCapa(anpi::Matrix<T> &matriz,anpi::Matrix<T> &newmatriz){

#pragma omp parallel for collapse(2) if( (matriz.rows() >= 512) && OMP)
	for(size_t i = 1; i<matriz.rows()-1;++i){//for i
		for(size_t j = 1; j<matriz.cols()-1;++j ){//for j
				newmatriz(2*i-1,2*j)=matriz(i,j);
				newmatriz(2*i-1,2*j-1)=matriz(i,j);
				newmatriz(2*i,2*j)=matriz(i,j);
				newmatriz(2*i,2*j-1)=matriz(i,j);
		}//for j
	}//for i
}

/**
 * @brief Calcula el valor de cada una de las celdas de una capa hasta que todas convergan.
 * @author Emily Sancho.
 * @param anpi::Matrix<T> &matriz
 * @param bool &converge
 */
template<typename T>
void calcularCapa(anpi::Matrix<T> &matriz, bool &converge){
	float umbral = 0.2;
#pragma omp parallel if ((matriz.rows() >= 512) && OMP)
    for (size_t i = 1; i < matriz.rows() - 1; ++i) {

#pragma omp for schedule(static)
      for (size_t j = 1; j < matriz.cols() - 1; ++j) {
        T nvalor = (matriz(i + 1, j) + matriz(i - 1, j) + matriz(i, j + 1)
            + matriz(i, j - 1)) / 4;
        converge = (abs((matriz(i, j) - nvalor)) < umbral) ? true : false;
        matriz(i, j) = nvalor;
      }
    }
  }

/**
 * @brief Determina el entero menor en un vector.
 * @author Emily Sancho.
 * @param std::vector<T>& v
 * @param int& n
 */
template<typename T>
void get_minaux(std::vector<T>& v,int& n){
	for(size_t i=0; i<v.size();++i){
		if(v[i]<n){
			n=v[i];
		}
	}
}

/**
 * @brief Determina el entero menor de un conjunto de 4 vectores.
 * @author Emily Sancho.
 * @param std::vector<T>& v
 * @param int& n
 * @return min (entero)
 */
template<typename T>
int get_min(std::vector<T> &top,
		std::vector<T> &right,
		std::vector<T> &bottom,
		std::vector<T> &left){
	int min = top[0];
	get_minaux(top,min);
	get_minaux(right,min);
	get_minaux(bottom,min);
	get_minaux(left,min);
	return min;
}

/**
 * @brief Determina el entero mayor en un vector.
 * @author Emily Sancho.
 * @param std::vector<T>& v
 * @param int& n
 */
template<typename T>
void get_maxaux(std::vector<T>& v,int& n){
	for(size_t i=0; i<v.size();++i){
		if(v[i]>n){
			n=v[i];
		}
	}
}

/**
 * @brief Determina el entero mayor de un conjunto de 4 vectores.
 * @author Emily Sancho.
 * @param std::vector<T>& v
 * @param int& n
 * @return max(entero)
 */
template<typename T>
int get_max(std::vector<T> &top,
		std::vector<T> &right,
		std::vector<T> &bottom,
		std::vector<T> &left){
	int max = top[0];
	get_maxaux(top,max);
	get_maxaux(right,max);
	get_maxaux(bottom,max);
	get_maxaux(left,max);
	return max;
}

/**
 * @brief Calcula la distribucion de temperatura en una placa.
 * @details Considera la placa como una capa de celdas, en cada iteracion del metodo
 * este calcula las temperaturas de todas la celdas de la placa y cuando
 * estas convergen aumenta el tamaño de la placa.
 * @author Emily Sancho.
 * @param std::vector<T> &top
 * @param std::vector<T> &right
 * @param std::vector<T> &bottom
 * @param std::vector<T> &left
 * @param std::vector<int>& plaqueSize
 * @return anpi::Matrix<T>
 */
template <typename T>
anpi::Matrix<T> liebmann(std::vector<T> &top,
							std::vector<T> &right,
							std::vector<T> &bottom,
							std::vector<T> &left,
							std::vector<int>& plaqueSize){
    anpi::Matrix<T> matriz;     // Return matrix
    anpi::Matrix<T> newmatriz;  // Matrix to create the layers
    size_t max = 5;             // Max iterations per layer
    size_t contador = 0;        // Counter of iterations
    bool converge = false;      // Convergence flag
    int size = 1;               // Initial size of the matrix

    size_t maxCapas = ceil(log(plaqueSize[0]) / log(2));
	matriz.allocate(size+2,size+2);

	 // n number of layers
	for(size_t n=0; n < maxCapas;++n){
		rellenarBorde(matriz,top,right,bottom,left);
		//Computes one layer until it converges
		while(converge!=true && contador<max){
			calcularCapa(matriz,converge);
			++contador;
	}//while

		contador = 0;
		converge = false;
		size=size*2;
		newmatriz.allocate(size+2,size+2);
		bajarCapa(matriz,newmatriz);
		matriz=newmatriz;
	}
	rellenarBorde(matriz,top,right,bottom,left);
	return matriz;
}

/**
 * @brief Calcula las variables secundarias del metodo del Libman.
 * @details Toma la matriz que se obtiene de Libman y calcula magnitudd y angulo
 * que representan el flujo de calor en cada uno de los puntos de la matriz,
 * retorna un vector de vectores de <magnitud,angulo>
 * @author Emily Sancho.
 * @param anpi::Matrix<T> &matriz
 * @param T k (coeficiente de tranferencia de calor)
 * @return std::vector<std::vector<T>>
 */


template<typename T>
std::vector<std::vector<T>> calcularFlujo(anpi::Matrix<T> &matriz, T k){
	T qx;
	T qy;
	T pi = boost::math::constants::pi<T>();
	std::vector<std::vector<T>> newmatriz;
	std::vector<T> valor={};
	valor.resize(2);
	cout << "---------" << endl;
	for(size_t i=1; i<matriz.rows()-1;++i ){
		for(size_t j=1; j<matriz.cols()-1;++j){
			qx = -k*((matriz(i+1,j)-matriz(i-1,j)))/(2*(matriz(i,j-1)-matriz(i,j+1)));
			qy = -k*((matriz(i,j+1)-matriz(i,j-1)))/(2*(matriz(i-1,j)-matriz(i+1,j)));
			valor[0]= sqrt((qx*qx)+(qy*qy));//magnitud
			valor[1]= atan(qx/qy)*180/pi; //angulo
			newmatriz.push_back(valor);
			//printVector(valor);

			cout <<"qx "<<qx<<"\n"<<endl;
			cout <<"qy "<<qy<<"\n"<<endl;
			cout <<"magnitud "<<valor[0]<<"\n"<<endl;
			cout <<"angulo "<<atan(qx/qy)*180/pi<<"\n"<<endl;
			cout << "~~~~~~~" << endl;

		}
	}

	return newmatriz;
}


}//cierra el namespace
#endif /* LIBMANN_CPP_ */
