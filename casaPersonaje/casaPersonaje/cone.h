#pragma once
#ifndef CONO_H_
#define CONO_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>
#define M_PI 3.14159265358979323846264338327950288
#define MERIDIANOS 30//eran 50
#define PARALELOS 10


class cone{

public:
	cone(GLfloat radio, GLfloat altura);
	void init();
	//void load();
	void render();
	virtual ~cone();

private:

	float radio;
	float height;
	GLuint cone_VAO[4], cone_VBO[4], cone_index[MERIDIANOS*(PARALELOS - 1) * 6];
	GLfloat cone_pos[PARALELOS * MERIDIANOS * 3];

};

#endif // !CONO_H_

// CLASS METHODS 

//constructor
cone::cone(GLfloat radio, GLfloat altura) :
	radio(radio), height(altura) {
	cone_VAO[4] = (-1, -1, -1, -1);
	cone_VBO[4] = (-1, -1, -1, -1);
}

//destructor
cone::~cone() {
	glDeleteVertexArrays(4, cone_VAO);
	glDeleteBuffers(4, cone_VBO);
}

//Render method
void cone::render() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(cone_VAO[0]);
	glDrawElements(GL_TRIANGLES, sizeof(cone_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


//Primitive constructor 
void cone::init() {

	const int nn = PARALELOS * MERIDIANOS * 3;
	GLfloat cone_nor[nn];
	GLfloat x, y, z, a, da, db, auxRadio = radio, auxHeight = height;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS);
	db = (GLfloat)M_PI / GLfloat(PARALELOS - 1);


//MAPPING POINTS
	for (ix = 0, a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3) {
		//parametric functions
		x = sin(a);
		y = -auxHeight;
		z = cos(a);
		cone_pos[ix + 0] = x * auxRadio;
		cone_pos[ix + 1] = y * auxRadio;
		cone_pos[ix + 2] = z * auxRadio;
		cone_nor[ix + 0] = x;
		cone_nor[ix + 1] = y;
		cone_nor[ix + 2] = z;
	}


//MAPPING MER AND PARAL
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS; ib++) {
		for (ia = 1; ia < MERIDIANOS; ia++, iy++) {

			// first half of QUAD
			cone_index[ix] = iy;      ix++;
			cone_index[ix] = iy + 1;    ix++;
			cone_index[ix] = iy + MERIDIANOS;   ix++;
			// second half of QUAD
			cone_index[ix] = iy + MERIDIANOS;   ix++;
			cone_index[ix] = iy + 1;    ix++;
			cone_index[ix] = iy + MERIDIANOS + 1; ix++;
		}

		// first half of QUAD
		cone_index[ix] = iy;       ix++;
		cone_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		cone_index[ix] = iy + MERIDIANOS;    ix++;
		// second half of QUAD
		cone_index[ix] = iy + MERIDIANOS;    ix++;
		cone_index[ix] = iy - MERIDIANOS + 1;  ix++;
		cone_index[ix] = iy + 1;     ix++;
		iy++;
	}



	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, cone_VAO);
	glGenBuffers(4, cone_VBO);
	glBindVertexArray(cone_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, cone_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cone_pos), cone_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, cone_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cone_nor), cone_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cone_index), cone_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


