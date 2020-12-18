#pragma once
#ifndef ASPA_H_
#define ASPA_H_
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader_m.h>
#include <iostream>


class cubo{

public:
	cubo(GLfloat inf, GLfloat sup);
	void init();
	void render();
	virtual ~cubo();

private:
	float inf;
	float sup;
	GLuint aspa_VAO[4], aspa_VBO[4];

};

#endif // !ASPA_H_

// CLASS METHODS 

//constructor
cubo::cubo(GLfloat inf, GLfloat sup) : inf(inf), sup(sup) {
	aspa_VAO[4] = (-1, -1, -1, -1);
	aspa_VBO[4] = (-1, -1, -1, -1);
}

//Destructor
cubo::~cubo() {
	glDeleteVertexArrays(4, aspa_VAO);
	glDeleteBuffers(4, aspa_VBO);
}

//Render primitive
void cubo::render() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBindVertexArray(aspa_VAO[0]);
	glDrawArrays(GL_QUADS, 0, 72);
	glBindVertexArray(0);
}

void cubo::init() {


	GLfloat vertices[] = {
		//Position				//Color

//ASPA (VELA)

	//Frontal
		-0.5f, 0.5f, 0.1f,		1.0f, 0.0f, 0.0f,	
		0.5f, 0.5f, 0.1f,		1.0f, 0.0f, 0.0f,	
		0.5f, 0.0f, 0.1f,		1.0f, 0.0f, 0.0f,	
		-0.5f, 0.0f, 0.1f,		1.0f, 0.0f, 0.0f,

	//Posterior
		0.5f, 0.5f, -0.1f,		1.0f, 1.0f, 0.0f,	
		-0.5f, 0.5f, -0.1f,		1.0f, 1.0f, 0.0f,	
		-0.5f, 0.0f, -0.1f,		1.0f, 1.0f, 0.0f,	
		0.5f, 0.0f, -0.1f,		1.0f, 1.0f, 0.0f,

	//Izquierda
		-0.5f, 0.5f, 0.1f,		0.0f, 0.0f, 1.0f,	
		-0.5f, 0.5f, -0.1f,		0.0f, 0.0f, 1.0f,	
		-0.5f, 0.0f, -0.1f,		0.0f, 0.0f, 1.0f,	
		-0.5f, 0.0f, 0.1f,		0.0f, 0.0f, 1.0f,	
	
	//Deracha
		0.5f, 0.5f, 0.1f,		0.0f, 1.0f, 0.0f,	
		0.5f, 0.0f, 0.1f,		0.0f, 1.0f, 0.0f,	
		0.5f, 0.0f, -0.1f,		0.0f, 1.0f, 0.0f,	
		0.5f, 0.5f, -0.1f,		0.0f, 1.0f, 0.0f,	
	
	//Superior
		-0.5f, 0.5f, 0.1f,		1.0f, 0.0f, 1.0f,	
		0.5f, 0.5f, 0.1f,		1.0f, 0.0f, 1.0f,	
		0.5f, 0.5f, -0.1f,		1.0f, 0.0f, 1.0f,	
		-0.5f, 0.5f, -0.1f,		1.0f, 0.0f, 1.0f,	

	//Inferior
		-0.5f, 0.0f, 0.1f,		1.0f, 1.0f, 1.0f,	
		-0.5f, 0.0f, -0.1f,		1.0f, 1.0f, 1.0f,	
		0.5f, 0.0f, -0.1f,		1.0f, 1.0f, 1.0f,	
		0.5f, 0.0f, 0.1f,		1.0f, 1.0f, 1.0f,	

	};



	glGenVertexArrays(1, &aspa_VAO[0]);
	glBindVertexArray(aspa_VAO[0]);

	glGenBuffers(1, &aspa_VBO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, aspa_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}






