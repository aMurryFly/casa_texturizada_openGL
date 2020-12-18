/*---------------------------------------------------------*/
/* ------------- Casa de mi personaje ---------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno: Alfonso Murrieta Villegas ---------*/

#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"
#include "cone.h"
#include "cylinder.h"
#include "cubo.h"
#include "esfera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

//CALL FUNCTIONS
void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void cubeBasic(void);
void display(Shader);
void getResolution(void);
void LoadTextures(void);
void getResolution(void);


//LOAD PRIMITIVES 
cubo cubo_1(1.0, 1.0);
cone cono_1(0.5f, 1.0f);
cylinder cilincro_1(0.5f, 1.0f);
Esfera esfera_1(1.0f);


int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));

GLfloat lastX = SCR_WIDTH / 2.0f,
lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;


unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Textures
unsigned int t_towers, t_muro, t_bridge, t_cupulas, t_puerta, t_ventanas,
t_piso;

unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void LoadTextures()
{
	t_piso = generateTextures("Texturas/piso.jpg", 0);
	t_bridge = generateTextures("Texturas/castle/bridge.jpg", 0);
	t_muro= generateTextures("Texturas/castle/muro.jpg", 0);
	t_towers = generateTextures("Texturas/castle/tower.jpg", 0);
	t_cupulas = generateTextures("Texturas/castle/torre_cupula.jpg", 0);
	t_puerta = generateTextures("Texturas/castle/door.jpg", 0);
	t_ventanas = generateTextures("Texturas/castle/main_castle.jpg", 0);
}


void cubeBasic()
{
	float vertices[] = {
		
		//DADO
		0.5f,  0.5f, 0.5f,   0.5f, 0.666f, 
		 0.5f, -0.5f, 0.5f,   0.5f, 0.333f, 
		-0.5f, -0.5f, 0.5f,   0.25f, 0.333f, 
		-0.5f,  0.5f, 0.5f,   0.25f, 0.666f,  

		0.5f, 0.5f,-0.5f, 0.75f,0.66f,
		0.5f,-0.5f,-0.5f, 0.75f,0.333f,
		0.5f,-0.5f,0.5f, 0.5f, 0.333f,
		0.5f, 0.5f,0.5f, 0.5f,0.666f,

		-0.5f,0.5f,-0.5f, 1.0f,0.666f,
		-0.5f,-0.5f,-0.5f, 1.0f,0.333f,
		0.5f,-0.5f,-0.5f, 0.75f,0.333f,
		0.5f,0.5f,-0.5f, 0.75f,0.666f,

		-0.5f, 0.5f,0.5f, 0.25f,0.66f,
		-0.5f,-0.5f,0.5f, 0.25f,0.333f,
		-0.5f,-0.5f,-0.5f, 0.0f, 0.333f,
		-0.5f, 0.5f,-0.5f, 0.0f,0.666f,

		0.5f,0.5f,-0.5f, 0.5f,1.0f,
		0.5f,0.5f,0.5f,	 0.5f,0.666f,
		-0.5f,0.5f,0.5f, 0.25f,0.66f,
		-0.5f,0.5f,-0.5f, 0.25f,1.0f,

		0.5f,-0.5f,0.5f, 0.5f,0.333f,
		0.5f,-0.5f,-0.5f, 0.5f,0.0f,
		-0.5f,-0.5f,-0.5f, 0.25f,0.0f,
		-0.5f,-0.5f,0.5f, 0.25f,0.333f,
	};

	unsigned int indices[] = {
	
		//VERTICES para fan triangule

		0, 1, 3, 
		1, 2, 3,  

		4,5,7,
		5,6,7,

		8,9,11,
		9,10,11,

		12,13,15,
		13,14,15,

		16,17,19,
		17,18,19,

		20,21,23,
		21,22,23,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}


void display(Shader shader)
{
	//Enable Shader
	shader.use();

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	shader.setVec3("viewPos", camera.Position);
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);


	glBindVertexArray(VAO);


	//PISO
	model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_piso);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//Puente
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(60.0f, 0.01f, 5.0f));
	model = glm::translate(model, glm::vec3(0.8f, 0.2f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_bridge);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//MURO

	//MURO FRONTAL
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(40.0f, 10.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 20.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_muro);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Relieves de muros
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, 2.0f, 20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, 20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(12.0f, 2.0f, 20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-12.0f, 2.0f, 20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//MUROS EXTRAS

	//2
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(40.0f, 10.0f, 1.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, -20.0f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_muro);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//Relieves de muros
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(12.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-12.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//3 
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 10.0f, 40.0f));
	model = glm::translate(model, glm::vec3(20.0f, 0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_muro);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//Relieves de muros
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(6.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(12.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-12.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



	//4=> CARA PRINCIPAL
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 10.0f, 40.0f));
	model = glm::translate(model, glm::vec3(-20.0f, 0.5f, 0.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_muro);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Relieves muralla

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(8.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-8.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(13.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-13.0f, 2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//ENTRADA - BLOCK

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, -20.0f));
		model = glm::scale(model, glm::vec3(8.0f, 12.0f, 4.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_towers);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//PUERTA
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 8.0f, 5.0f));
		model = glm::translate(model, glm::vec3(14.5f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_puerta);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.5f, 8.0f, 5.0f));
		model = glm::translate(model, glm::vec3(12.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_puerta);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
	//CENTRO - MAIN CASTLE
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_towers);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	/*
	*/

	//PUERTAS 

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
	model = glm::translate(model, glm::vec3(5.0f, 0.5f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_ventanas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-5.0f, 0.5f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_ventanas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//Ventanas
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
		model = glm::translate(model, glm::vec3(4.0f, 3.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_ventanas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
		model = glm::translate(model, glm::vec3(-4.0f, 3.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_ventanas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



	//LEFT / RIGHT
	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
	model = glm::translate(model, glm::vec3(5.0f, 0.5f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_ventanas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, .0f));
	model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
	model = glm::translate(model, glm::vec3(-5.0f, 0.5f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_ventanas);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Ventanas
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
		model = glm::translate(model, glm::vec3(4.0f, 3.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_ventanas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(1.0f, 4.0f, 2.0f));
		model = glm::translate(model, glm::vec3(-4.0f, 3.0f, 0.0f));
		shader.setMat4("model", model);
		shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, t_ventanas);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



	//arriba
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, t_towers);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//tore / cupula de main castle
	glm::mat4 temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(0.0f, 18.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.75f, 10.0f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_muro);
	cilincro_1.render();

	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(0.0f, 20.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cupulas);
	esfera_1.render();

	//CUPULA AUX - ENTRADA
	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(20.0f, 11.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.4f, 4.0f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cupulas);
	esfera_1.render();

	temporal = glm::mat4(1.0f);


	// ------------------------------------------------------------------
	//MURALLA
	// ------------------------------------------------------------------


	//CILINDRO - CUERPO PRINCIPAL

	//TORRE y CUPULA
	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(20.0f, 8.0f, 20.0f));
	model = glm::scale(model, glm::vec3(4.75f, 10.0f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_towers);
	cilincro_1.render();

	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(20.0f, 10.0f, 20.0f));
	model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cupulas);
	esfera_1.render();



	//2
	temporal = glm::mat4(1.0f);
	model = glm::translate(temporal, glm::vec3(-20.0f, 8.0f, 20.0f));
	model = glm::scale(model, glm::vec3(4.75f, 10.0f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_towers);
	cilincro_1.render();

	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(-20.0f, 10.0f, 20.0f));
	model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cupulas);
	esfera_1.render();



	//Torres traseras
	temporal = glm::mat4(1.0f);
	model = glm::translate(temporal, glm::vec3(20.0f, 8.0f, -20.0f));
	model = glm::scale(model, glm::vec3(4.75f, 10.0f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_towers);
	cilincro_1.render();

	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(20.0f, 10.0f, -20.0f));
	model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cupulas);
	esfera_1.render();


	temporal = glm::mat4(1.0f);
	model = glm::translate(temporal, glm::vec3(-20.0f, 8.0f, -20.0f));
	model = glm::scale(model, glm::vec3(4.75f, 10.0f, 4.75f));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_towers);
	cilincro_1.render();

	temporal = glm::mat4(1.0f);
	temporal = model = glm::translate(temporal, glm::vec3(-20.0f, 10.0f, -20.0f));
	model = glm::scale(model, glm::vec3(2.4f, 2.4f, 2.4));
	shader.setMat4("model", model);
	shader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	glBindTexture(GL_TEXTURE_2D, t_cupulas);
	esfera_1.render();


	

glBindVertexArray(0);
}


// ------------------------------------------------------------------
// SOLO INSTANCIAS DE OBJETOS
// ------------------------------------------------------------------

int main()
{

	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Casa de mi personaje", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glewInit();
	LoadTextures();
	cubeBasic();

	cubo_1.init();
	cono_1.init();
	cilincro_1.init();
	esfera_1.init();

	glEnable(GL_DEPTH_TEST);

	Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");

	// render loop
	while (!glfwWindowShouldClose(window))
	{

		lastFrame = SDL_GetTicks();

		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		my_input(window);

		//DIBUJO DE TODAS LAS ENTIDADES
		display(projectionShader);


		deltaTime = SDL_GetTicks() - lastFrame;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}



// ------------------------------------------------------------------
// ENTRADA DE TECLADO 
// ------------------------------------------------------------------

void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}