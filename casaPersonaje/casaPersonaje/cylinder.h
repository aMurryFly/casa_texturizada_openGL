#pragma once
#ifndef CILINDRO_H_
#define CILINDRO_H_

#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Constantes
#define M_PI2 3.14159265358979323846264338327950288
#define MERIDIANOS2 50
#define PARALELOS2 20


class cylinder{

public:
	cylinder(GLfloat radio, GLfloat altura);
	void init();
	void render();
	virtual ~cylinder();

private:
	float radio;
	float altura;
	GLuint cilindro_VAO[4], cilindro_VBO[4], cilindro_index[MERIDIANOS2*(PARALELOS2 - 1) * 6];
	GLfloat cilindro_pos[PARALELOS2 * MERIDIANOS2 * 3];

};

#endif 

// CLASS METHODS 

//constructor
cylinder::cylinder(GLfloat radio, GLfloat altura) : radio(radio), altura(altura) {

	cilindro_VAO[4] = (-1, -1, -1, -1);
	cilindro_VBO[4] = (-1, -1, -1, -1);
}

//destructor
cylinder::~cylinder() {
	glDeleteVertexArrays(4, cilindro_VAO);
	glDeleteBuffers(4, cilindro_VBO);
}


//Primitive render
void cylinder::render() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(cilindro_VAO[0]);
	glDrawElements(GL_TRIANGLE_STRIP, sizeof(cilindro_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



void cylinder::init() {

	const int nn = PARALELOS2 * MERIDIANOS2 * 3;

	GLfloat cylinder_nor[nn];
	GLfloat x, y, z, b, a, da, db, auxRadio = radio, auxAltura = altura;
	GLuint i;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI2 / GLfloat(MERIDIANOS2);
	db = (GLfloat)2.0*auxAltura / GLfloat(PARALELOS2 - 1);

// MAPPING POINTS
	for (ix = 0, a = (GLfloat)-0.5*M_PI2, ib = 0; ib < PARALELOS2; ib++, a += db) {
		for (b = 0.0, ia = 0; ia < MERIDIANOS2; ia++, b += da, ix += 3) {

			// parametric functions
			x = sin(b);
			y = a;
			z = cos(b);
			cilindro_pos[ix + 0] = x * auxRadio;
			cilindro_pos[ix + 1] = y * auxRadio;
			cilindro_pos[ix + 2] = z * auxRadio;
			cylinder_nor[ix + 0] = x;
			cylinder_nor[ix + 1] = y;
			cylinder_nor[ix + 2] = z;
		}
	}


// MAPPING WITH  GL_TRIANGLE
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS2; ib++) {

		for (ia = 1; ia < MERIDIANOS2; ia++, iy++) {
			// first half of QUAD
			cilindro_index[ix] = iy;      ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS2;   ix++;
			// second half of QUAD
			cilindro_index[ix] = iy + MERIDIANOS2;   ix++;
			cilindro_index[ix] = iy + 1;    ix++;
			cilindro_index[ix] = iy + MERIDIANOS2 + 1; ix++;
		}

		// first half of QUAD
		cilindro_index[ix] = iy;       ix++;
		cilindro_index[ix] = iy + 1 - MERIDIANOS2;  ix++;
		cilindro_index[ix] = iy + MERIDIANOS2;    ix++;
		// second half of QUAD
		cilindro_index[ix] = iy + MERIDIANOS2;    ix++;
		cilindro_index[ix] = iy - MERIDIANOS2 + 1;  ix++;
		cilindro_index[ix] = iy + 1;     ix++;
		iy++;
	}



	glGenVertexArrays(4, cilindro_VAO);
	glGenBuffers(4, cilindro_VBO);
	glBindVertexArray(cilindro_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cilindro_pos), cilindro_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder_nor), cylinder_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cilindro_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cilindro_index), cilindro_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}




