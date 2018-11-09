/*
 * perfiltermico.cpp
 *
 *  Created on: Nov 5, 2018
 *      Author: emi
 */
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <vector>
#include <math.h>
#include "../include/Matrix.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

template<typename T>
void CelsiusToRGB(std::vector<float>& color,T punto, int min, int max){
	color.resize(3);
	int valor = int((punto*44)/max);

	switch(valor){
	case 0: color[0]=3.0; color[1]=4.0; color[2]=255.0; break;
	case 1: color[0]=1.0; color[1]=33.0 ;color[2]=255.0; break;
	case 2: color[0]=0.0; color[1]=66.0 ;color[2]=250.0; break;
	case 3: color[0]=0.0; color[1]=93.0 ;color[2]=255.0; break;
	case 4: color[0]=2.0; color[1]=120.0 ;color[2]=254.0; break;
	case 5: color[0]=0.0; color[1]=148.0 ;color[2]=254.0; break;
	case 6: color[0]=0.0; color[1]=177.0 ;color[2]=255.0; break;
	case 7: color[0]=0.0; color[1]=204.0 ;color[2]=255.0; break;
	case 8: color[0]=1.0; color[1]=210.0 ;color[2]=253.0; break;
	case 9: color[0]=1.0; color[1]=229.0 ;color[2]=254.0; break;
	case 10: color[0]=0.0; color[1]=254.0; color[2]=252.0; break;
	case 11: color[0]=0.0; color[1]=255.0; color[2]=225.0; break;
	case 12: color[0]=2.0; color[1]=255.0; color[2]=175.0; break;
	case 13: color[0]=1.0; color[1]=254.0; color[2]=148.0; break;
	case 14: color[0]=0.0; color[1]=254.0; color[2]=122.0; break;
	case 15: color[0]=0.0; color[1]=255.0; color[2]=99.0; break;
	case 16: color[0]=1.0; color[1]=255.0; color[2]=73.0; break;
	case 17: color[0]=0.0; color[1]=255.0; color[2]=50.0; break;
	case 18: color[0]=0.0; color[1]=255.0; color[2]=27.0; break;
	case 19: color[0]=0.0; color[1]=255.0; color[2]=3.0; break;
	case 20: color[0]=21.0; color[1]=255.0; color[2]=0.0; break;
	case 21: color[0]=44.0; color[1]=255.0; color[2]=0.0; break;
	case 22: color[0]=68.0; color[1]=255.0; color[2]=0.0; break;
	case 23: color[0]=90.0; color[1]=255.0; color[2]=0.0; break;
	case 24: color[0]=113.0; color[1]=255.0; color[2]=0.0; break;
	case 25: color[0]=137.0; color[1]=254.0; color[2]=0.0; break;
	case 26: color[0]=154.0; color[1]=255.0; color[2]=1.0; break;
	case 27: color[0]=178.0; color[1]=255.0; color[2]=0.0; break;
	case 28: color[0]=198.0; color[1]=253.0; color[2]=2.0; break;
	case 29: color[0]=222.0; color[1]=253.0; color[2]=1.0; break;
	case 30: color[0]=242.0; color[1]=255.0; color[2]=2.0; break;
	case 31: color[0]=254.0; color[1]=249.0; color[2]=0.0; break;
	case 32: color[0]=250.0; color[1]=227.0; color[2]=0; break;
	case 33: color[0]=255.0; color[1]=208.0; color[2]=0.0; break;
	case 34: color[0]=251.0; color[1]=187.0; color[2]=1.0; break;
	case 35: color[0]=255.0; color[1]=168.0; color[2]=0; break;
	case 36: color[0]=255.0; color[1]=148.0; color[2]=0.0; break;
	case 37: color[0]=254.0; color[1]=129.0; color[2]=0.0; break;
	case 38: color[0]=255.0; color[1]=111.0; color[2]=0; break;
	case 39: color[0]=255.0; color[1]=92.0; color[2]=1.0; break;
	case 40: color[0]=253.0; color[1]=3.0; color[2]=1.0; break;
	case 41: color[0]=255.0; color[1]=4.0; color[2]=1.0; break;
	case 42: color[0]=248; color[1]=0.0; color[2]=0.0; break;
	case 43: color[0]=251.0; color[1]=1.0; color[2]=0; break;
	case 44: color[0]=255.0; color[1]=0; color[2]=0; break;
	}
}

template<typename T>
void mostrarPerfil(anpi::Matrix<T>& matrix, int min, int max){
		std::vector<float> color;
		int alto = matrix.rows();
		int ancho = matrix.cols();

		int ini =1;
		char* init[1]={" "};

		glutInit(&ini,init);
		glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize (ancho+300,alto);
		glutInitWindowPosition (1000, 100);
		glutCreateWindow ("Perfil termico");
		glClearColor(1,1,1,1);
		glMatrixMode (GL_PROJECTION);
		gluOrtho2D(0.0, ancho+300, 0.0,alto+2);


		glPointSize(2);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_POINTS);

		//para graficar la matriz
		for(size_t i=1; i < matrix.rows()-1;++i)
		{
				for(size_t j=1; j< matrix.cols()-1;++j){
				CelsiusToRGB(color,matrix(i,j),min,max);
				glColor3ub(color[0],color[1],color[2]);
				glVertex2i(j+2,i+2);

			}
		}

		//para graficar la escala
		for(size_t i=0; i<44;++i){
				for(size_t ii=0;ii<size_t(alto*0.81);++ii){
					CelsiusToRGB(color,ii,0,alto*0.81);
					glColor3ub(color[0],color[1],color[2]);
					for(size_t j=0;j<75;++j){
						glVertex2i(j+100+ancho,ii+5);
					}
				}
		}

		glEnd();

		glColor3ub(1,1,1);


		const unsigned char* text1 = reinterpret_cast<const unsigned char*>("Perfil termico de la placa");
		glRasterPos2f(ancho+25 ,alto*0.90);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,text1);

		char t2[8]="";
		sprintf(t2,"%d",min);
		const unsigned char* text2 = reinterpret_cast<const unsigned char*>(t2);
		glRasterPos2f(ancho+185 ,alto*0.01);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,text2);

		sprintf(t2,"%d",(max-min)*1/4);
		text2 = reinterpret_cast<const unsigned char*>(t2);
		glRasterPos2f(ancho+185 ,alto*0.21);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,text2);

		sprintf(t2,"%d",(max-min)/2);
		text2 = reinterpret_cast<const unsigned char*>(t2);
		glRasterPos2f(ancho+185 ,alto*0.41);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,text2);


		sprintf(t2,"%d",(max-min)*3/4);
		text2 = reinterpret_cast<const unsigned char*>(t2);
		glRasterPos2f(ancho+185 ,alto*0.61);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,text2);


		sprintf(t2,"%d",max);
		text2 = reinterpret_cast<const unsigned char*>(t2);
		glRasterPos2f(ancho+185 ,alto*0.81);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,text2);




		glFlush();
		glutMainLoop();
}

