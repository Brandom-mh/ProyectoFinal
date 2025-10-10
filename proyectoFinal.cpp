/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture dado8Texture;

Texture parabrisasTexture;
Texture llantaTexture;
Texture cofreTexture;
Texture parrillaTexture;
Texture rinTexture;


Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model parabrisas_M;
Model llanta;
Model cofre_M;
Model parrilla_M;
Model rin;
Model gtr_M;


Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearOctaedro()
{
	// Índices: 8 triángulos (caras)
	unsigned int oct_indices[] = {
		// (0,2,4)
		0,  1,  2,
		// (2,1,4)
		3,  4,  5,
		// (1,3,4)
		6,  7,  8,
		// (3,0,4)
		9, 10, 11,
		// (2,0,5)
		12, 13, 14,
		// (1,2,5)
		15, 16, 17,
		// (3,1,5)
		18, 19, 20,
		// (0,3,5)
		21, 22, 23
	};

	// Vértices: 24 (3 por cara). UV por tri: (0,0), (1,0), (0.5,1)
	// Formato: x, y, z, s, t, nx, ny, nz
	GLfloat oct_vertices[] = {
		// Cara (0,2,4) 1
		 1.f,  0.f,  0.f,   0.24f,0.51f,   0,0,0, //derecha
		 0.f,  1.f,  0.f,   0.125f,0.99f,   0,0,0,//punta
		 0.f,  0.f,  1.f,   0.01f, 0.51f,   0,0,0,//izquierda

		 // Cara (2,1,4)4
		  0.f,  1.f,  0.f,   0.875f, 0.99f,   0,0,0,//punta
		 -1.f,  0.f,  0.f,   0.76f, 0.51f,   0,0,0,//izquierda
		  0.f,  0.f,  1.f,   0.99f, 0.51f,   0,0,0,//derecha

		 // Cara (1,3,4) 8
		 -1.f,  0.f,  0.f,   0.98f, 0.49f,   0,0,0,//derecha
		  0.f, -1.f,  0.f,   0.875f, 0.01f,    0,0,0, //punta
		  0.f,  0.f,  1.f,   0.77f, 0.49f,    0,0,0, //izquierda

		 // Cara (3,0,4) 5
		 0.f, -1.f,  0.f,   0.125f, 0.01f,  0,0,0, //punta
		 1.f,  0.f,  0.f,   0.24f, 0.49f,   0,0,0, //derecha
		 0.f,  0.f,  1.f,   0.01f, 0.49f,    0,0,0, //izquierda

		// Cara (2,0,5) 2
		 0.f,  1.f,  0.f,   0.375f, 1.0f,   0,0,0,
		 1.f,  0.f,  0.f,   0.25f, 0.5f,   0,0,0,
		 0.f,  0.f, -1.f,   0.5f, 0.5f,   0,0,0,

		// Cara (1,2,5) 3
		-1.f,  0.f,  0.f,   0.75f, 0.5f,    0,0,0,
		 0.f,  1.f,  0.f,   0.625f, 1.0f,   0,0,0,
		 0.f,  0.f, -1.f,    0.5f, 0.5f,   0,0,0,

		// Cara (3,1,5) 7
		 0.f, -1.f,  0.f,   0.625f, 0.0f,   0,0,0, //punta
		-1.f,  0.f,  0.f,   0.75f, 0.5f,   0,0,0, //derecha
		 0.f,  0.f, -1.f,   0.5f, 0.5f,   0,0,0, //izquierda

		// Cara (0,3,5) 6
		 1.f,  0.f,  0.f,   0.25f, 0.5f,   0,0,0,
		 0.f, -1.f,  0.f,   0.375f, 0.0f,    0,0,0,
		 0.f,  0.f, -1.f,    0.5f, 0.5f,    0,0,0,
	};;

	Mesh* oct = new Mesh();
	oct->CreateMesh(oct_vertices, oct_indices, 192, 24);
	meshList.push_back(oct);
}





int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearOctaedro();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado-de-numeros.png");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	dado8Texture = Texture("Textures/dado.png");
	dado8Texture.LoadTextureA();

	parabrisasTexture = Texture("Textures/ojos.png");
	parabrisasTexture.LoadTextureA();
	llantaTexture = Texture("Textures/llanta.png");
	llantaTexture.LoadTextureA();
	cofreTexture = Texture("Textures/cofre.png");
	cofreTexture.LoadTextureA();
	parrillaTexture = Texture("Textures/parrilla.png");
	parrillaTexture.LoadTextureA();
	rinTexture = Texture("Textures/fibra.png");
	rinTexture.LoadTextureA();

	

	
	
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dado_M = Model();
	Dado_M.LoadModel("Models/Dado8Texturizado.fbx");

	parabrisas_M = Model();
	parabrisas_M.LoadModel("Models/Parabrisas.fbx");
	llanta = Model();
	llanta.LoadModel("Models/caucho_FD.fbx");
	rin = Model();
	rin.LoadModel("Models/rin_FD.fbx");
	cofre_M = Model();
	cofre_M.LoadModel("Models/cofre.fbx");
	parrilla_M = Model();
	parrilla_M.LoadModel("Models/Parrilla.fbx");
	gtr_M = Model();
	gtr_M.LoadModel("Models/GTR.fbx");




	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();


		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes
		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();*/
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen dado_animales ya optimizada por ustedes
		
		


		//Dado de 8 caras por medio de código
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dado8Texture.UseTexture();
		meshList[5]->RenderMesh();
		

		
		
		/*Reporte de práctica :
		Ejercicio 1: Crear un dado de 8 caras y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo de su coche con sus 4 llantas acomodadas
		y tener texturizadas las 4 llantas (diferenciar caucho y rin)  y 
		texturizar el logo de la Facultad de ingeniería en el cofre de su propio modelo de coche
	
		*/

		//GTR
		//Instancia del coche 
		model = glm::mat4(1.0);
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		gtr_M.RenderModel();

		//llanta Frente Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//llanta Frente Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 85.0f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//llanta Trasera Derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//llanta Trasera Izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		model = glm::rotate(model, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaTexture.UseTexture();
		llanta.RenderModel();
		//rin
		rinTexture.UseTexture();
		rin.RenderModel();

		//parabrisas
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parabrisasTexture.UseTexture();
		parabrisas_M.RenderModel();

		//cofre
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofreTexture.UseTexture();
		cofre_M.RenderModel();
		
		//parrilla
		model = modelaux;
		model = glm::translate(model, glm::vec3(83.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		parrillaTexture.UseTexture();
		parrilla_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/