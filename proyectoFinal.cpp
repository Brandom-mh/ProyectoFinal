/*
Animación por keyframes
La textura del skybox fue conseguida desde la página https ://opengameart.org/content/elyvisions-skyboxes?page=1
y edité en Gimp rotando 90 grados en sentido antihorario la imagen  sp2_up.png para poder ver continuidad.
Fuentes :
	https ://www.khronos.org/opengl/wiki/Keyframe_Animation
	http ://what-when-how.com/wp-content/uploads/2012/07/tmpcd0074_thumb.png
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//Para archivo
#include <fstream>
#include <sstream>

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float angulovaria = 0.0f;
float alaAngulo = 0.0f;

//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture ArcoTexture;
Texture PuertaTexture;
Texture lumbreraTexture;
Texture aguilaTexture;
Texture habitat_ajoloteTexture;
Texture nidoTexture;
Texture troncoTexture;
Texture jaguarTexture;
Texture arbolTexture;
Texture piedrasTexture;
Texture ringTexture;
Texture axoloteTexture;
Texture piramideTexture;
Texture arbolAviarioTexture;
Texture pajaroTexture;


Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Arco_M;
Model Letrero_M;
Model Puerta_Derecha_M;
Model Puerta_Izquierda_M;
Model lumbrera;
Model aguila_M;
Model alaDer_M;
Model alaIzq_M;
Model habitat_ajolote_M;
Model nido_M;
Model tronco_M;
Model jaguar_M;
Model arbol_M;
Model piedras_M;
Model ring_M;
Model piramide_M;

Model axolote_M;
Model axoloteCola_M;
Model axolotePataDerecha_M;
Model axolotePataIzquierda_M;
Model axolotePataTraseraDerecha_M;
Model axolotePataTraseraIzquierda_M;

Model arbolAviario_M;
Model pajaro_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

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


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;

//Número de cuadros máximos
//Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
struct FRAME {
	float movX;
	float movY;
	float movXInc;
	float movYInc;
	float giro;
	float giroInc;
};

#define MAX_FRAMES 100
// Variables globales de control
std::string archivoActual = "keyframes.txt";

// Avión
FRAME KeyFrameAvion[MAX_FRAMES];
int FrameIndexAvion = 0;
float movAvion_x = 0, movAvion_y = 0, giroAvion = 0;
int playIndexAvion = 0, i_curr_stepsAvion = 0;
bool playAvion = false;

// Águila
FRAME KeyFrameAguila[MAX_FRAMES];
int FrameIndexAguila = 0;
float movAguila_x = 0, movAguila_y = 0, giroAguila = 0;
int playIndexAguila = 0, i_curr_stepsAguila = 0;
bool playAguila = false;

// Control de teclas
int reproducirAnimacion = 0;
bool modoEdicion = false;
bool modoRotacion = false;

int i_max_steps = 100;
bool play = false;
int playIndex = 0;

// --- REINICIAR AL PRIMER FRAME ---
void resetElements(FRAME* KeyFrame, float& movX, float& movY, float& giro)
{
	movX = KeyFrame[0].movX;
	movY = KeyFrame[0].movY;
	giro = KeyFrame[0].giro;
}

// --- CALCULAR INCREMENTOS ---
void interpolation(FRAME* KeyFrame, int playIndex, int i_max_steps)
{
	KeyFrame[playIndex].movXInc = (KeyFrame[playIndex + 1].movX - KeyFrame[playIndex].movX) / i_max_steps;
	KeyFrame[playIndex].movYInc = (KeyFrame[playIndex + 1].movY - KeyFrame[playIndex].movY) / i_max_steps;
	KeyFrame[playIndex].giroInc = (KeyFrame[playIndex + 1].giro - KeyFrame[playIndex].giro) / i_max_steps;
}

// --- ANIMAR ---
void animate(FRAME* KeyFrame, int& playIndex, int& FrameIndex, int& i_curr_steps,
	int i_max_steps, bool& play,
	float& movX, float& movY, float& giro)
{
	if (!play) return;

	if (i_curr_steps >= i_max_steps) // fin de interpolación
	{
		playIndex++;
		if (playIndex > FrameIndex - 2) // fin de animación total
		{
			printf("terminó la animación\n");
			playIndex = 0;
			play = false;
		}
		else
		{
			i_curr_steps = 0;
			interpolation(KeyFrame, playIndex, i_max_steps);
		}
	}
	else
	{
		movX += KeyFrame[playIndex].movXInc;
		movY += KeyFrame[playIndex].movYInc;
		giro += KeyFrame[playIndex].giroInc;
		i_curr_steps++;
	}
}

///////////////* FIN KEYFRAMES*////////////////////////////

/*
============================= Archivo =============================
*/


// --- GUARDAR / AÑADIR ---
void appendKeyframeToFile(const std::string& archivo, const FRAME& frame, int index)
{
	std::ofstream file(archivo, std::ios::app);
	if (!file.is_open()) {
		printf("Error al abrir %s para escribir.\n", archivo.c_str());
		return;
	}
	file << index << " " << frame.movX << " " << frame.movY << " " << frame.giro << "\n";
	file.close();
	printf("Frame %d agregado al archivo %s.\n", index, archivo.c_str());
}

void saveFrame(const std::string& archivo, FRAME* KeyFrame, int& FrameIndex,
	float movX, float movY, float giro)
{
	KeyFrame[FrameIndex].movX = movX;
	KeyFrame[FrameIndex].movY = movY;
	KeyFrame[FrameIndex].giro = giro;

	appendKeyframeToFile(archivo, KeyFrame[FrameIndex], FrameIndex);
	FrameIndex++;
}

// --- LEER DESDE ARCHIVO ---
void leerArchivo(const std::string& archivo, FRAME* KeyFrame, int& FrameIndex)
{
	std::ifstream file(archivo);
	if (!file.is_open()) {
		printf("No se encontró %s, se creará al guardar.\n", archivo.c_str());
		return;
	}

	FrameIndex = 0;
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int idx;
		FRAME temp;
		if (iss >> idx >> temp.movX >> temp.movY >> temp.giro) {
			KeyFrame[idx] = temp;
			FrameIndex = std::max(FrameIndex, idx + 1);
		}
	}
	file.close();
	printf("Se cargaron %d frames desde %s\n", FrameIndex, archivo.c_str());
}

// --- MODIFICAR ---
void actualizarArchivo(const std::string& archivo, int index, const FRAME& newFrame)
{
	std::ifstream fileIn(archivo);
	if (!fileIn.is_open()) {
		printf("No se puede abrir %s para modificar.\n", archivo.c_str());
		return;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(fileIn, line)) lines.push_back(line);
	fileIn.close();

	std::ofstream fileOut(archivo);
	if (!fileOut.is_open()) {
		printf("No se puede escribir %s\n", archivo.c_str());
		return;
	}

	bool found = false;
	for (auto& l : lines) {
		std::istringstream iss(l);
		int idx; float x, y, g;
		if (iss >> idx >> x >> y >> g) {
			if (idx == index) {
				fileOut << index << " " << newFrame.movX << " " << newFrame.movY << " " << newFrame.giro << "\n";
				found = true;
			}
			else fileOut << l << "\n";
		}
	}
	fileOut.close();
	if (found)
		printf("Frame %d modificado correctamente en %s.\n", index, archivo.c_str());
	else
		printf("Frame %d no encontrado en %s.\n", index, archivo.c_str());
}

// --- BORRAR ---
void borrarFrame(const std::string& archivo, int index)
{
	std::ifstream fileIn(archivo);
	if (!fileIn.is_open()) {
		printf("No se puede abrir %s para borrar.\n", archivo.c_str());
		return;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(fileIn, line)) lines.push_back(line);
	fileIn.close();

	std::ofstream fileOut(archivo);
	if (!fileOut.is_open()) {
		printf("No se puede escribir %s\n", archivo.c_str());
		return;
	}

	bool found = false;
	for (auto& l : lines) {
		std::istringstream iss(l);
		int idx;
		if (iss >> idx) {
			if (idx == index) {
				found = true;
				continue;
			}
			fileOut << l << "\n";
		}
	}
	fileOut.close();
	if (found)
		printf("Frame %d eliminado correctamente de %s.\n", index, archivo.c_str());
	else
		printf("Frame %d no encontrado en %s.\n", index, archivo.c_str());
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	pisoTexture = Texture("Textures/Nordic_Moss_se4rwei_1K_BaseColor.jpg");
	pisoTexture.LoadTextureA();
	ArcoTexture = Texture("Textures/Mossy_Stone_Floor_veilfjxn_1K_BaseColor.jpg");
	ArcoTexture.LoadTextureA();
	PuertaTexture = Texture("Textures/Rusty_Metal_Sheet_tj4kedvcw_1K_BaseColor.jpg");
	PuertaTexture.LoadTextureA();
	lumbreraTexture = Texture("Textures/lumbrera.png");
	lumbreraTexture.LoadTextureA();
	aguilaTexture = Texture("Textures/Fogel_Mat_Diffuse_Color.png");
	aguilaTexture.LoadTextureA();
	habitat_ajoloteTexture = Texture("Textures/rock_wall_13_diff_4k.jpg");
	habitat_ajoloteTexture.LoadTextureA();
	nidoTexture = Texture("Textures/wood_table_diff_4k.jpg");
	nidoTexture.LoadTextureA();
	troncoTexture = Texture("Textures/pine_bark_diff_4k.png");
	troncoTexture.LoadTextureA();
	jaguarTexture = Texture("Textures/Jaguar_BaseColor.png");
	jaguarTexture.LoadTextureA();
	arbolTexture = Texture("Textures/bark_willow_diff_4k.png");
	arbolTexture.LoadTextureA();
	piedrasTexture = Texture("Textures/CavePlatform4_Base_Specular.png");
	piedrasTexture.LoadTextureA();
	ringTexture = Texture("Textures/Lona.png");
	ringTexture.LoadTextureA();
	axoloteTexture = Texture("Textures/ajoloteTexture.png");
	axoloteTexture.LoadTextureA();
	piramideTexture = Texture("Textures/Stone_Wall_tinifauaw_1K_BaseColor.jpg");
	piramideTexture.LoadTextureA();
	arbolAviarioTexture = Texture("Textures/Tree_Bark_vimmdcofw_1K_BaseColor.jpg");
	arbolAviarioTexture.LoadTextureA();
	pajaroTexture = Texture("Textures/12213_bird_diffuse.jpg");
	pajaroTexture.LoadTextureA();

	Arco_M = Model();
	Arco_M.LoadModel("Models/Arco.fbx");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero.fbx");
	Puerta_Derecha_M = Model();
	Puerta_Derecha_M.LoadModel("Models/Puerta_Der.fbx");
	Puerta_Izquierda_M = Model();
	Puerta_Izquierda_M.LoadModel("Models/Puerta_Izq.fbx");
	lumbrera = Model();
	lumbrera.LoadModel("Models/Lumbrera.fbx");
	aguila_M = Model();
	aguila_M.LoadModel("Models/aguila.fbx");
	alaDer_M = Model();
	alaDer_M.LoadModel("Models/alaDerecha.fbx");
	alaIzq_M = Model();
	alaIzq_M.LoadModel("Models/alaIzquierda.fbx");
	habitat_ajolote_M = Model();
	habitat_ajolote_M.LoadModel("Models/habitat_ajolote.fbx");
	nido_M = Model();
	nido_M.LoadModel("Models/nido.fbx");
	tronco_M = Model();
	tronco_M.LoadModel("Models/tronco.fbx");
	jaguar_M = Model();
	jaguar_M.LoadModel("Models/jaguar.fbx");
	arbol_M = Model();
	arbol_M.LoadModel("Models/arbol.fbx");
	piedras_M = Model();
	piedras_M.LoadModel("Models/piedras.fbx");
	ring_M = Model();
	ring_M.LoadModel("Models/Ring.fbx");
	axolote_M = Model();
	axolote_M.LoadModel("Models/axolote.fbx");
	axoloteCola_M = Model();
	axoloteCola_M.LoadModel("Models/colaAxolote.fbx");
	axolotePataDerecha_M = Model();
	axolotePataDerecha_M.LoadModel("Models/pataDerechaAxolote.fbx");
	axolotePataIzquierda_M = Model();
	axolotePataIzquierda_M.LoadModel("Models/pataIzquierdaAxolote.fbx");
	axolotePataTraseraDerecha_M = Model();
	axolotePataTraseraDerecha_M.LoadModel("Models/pataDerechaTraseraAxolote.fbx");
	axolotePataTraseraIzquierda_M = Model();
	axolotePataTraseraIzquierda_M.LoadModel("Models/pataIzquierdaTraseraAxolote.fbx");
	piramide_M = Model();
	piramide_M.LoadModel("Models/piramide.fbx");
	arbolAviario_M = Model();
	arbolAviario_M.LoadModel("Models/arbolAves.fbx");
	pajaro_M = Model();
	pajaro_M.LoadModel("Models/pajaro.fbx");




	std::vector<std::string> skyboxFaces;
	/*skyboxFaces.push_back("Textures/Skybox/sp2_lf.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_rt.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_dn.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_up.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_bk.png");
	skyboxFaces.push_back("Textures/Skybox/sp2_ft.png");*/

	skyboxFaces.push_back("Textures/Skybox/nx.png");
	skyboxFaces.push_back("Textures/Skybox/px.png");
	skyboxFaces.push_back("Textures/Skybox/ny.png");
	skyboxFaces.push_back("Textures/Skybox/py.png");
	skyboxFaces.push_back("Textures/Skybox/nz.png");
	skyboxFaces.push_back("Textures/Skybox/pz.png");
	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	// Luz direccional tipo sol de mediodía
	mainLight = DirectionalLight(
		1.0f, 0.98f, 0.92f,   // color ligeramente cálido
		0.8f, 0.6f,           // intensidad difusa y especular más alta
		0.0f, -1.0f, -0.3f    // dirección: desde arriba hacia abajo con ligera inclinación
	);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ


	//Se agregan nuevos frames 


	printf("\n=====================================\n");
	printf("      CONTROLES DE KEYFRAMES\n");
	printf("=====================================\n");
	printf(" [F1]		 Utilizar el helicoptero\n");
	printf(" [F2]		 Utilizar el aguila\n");
	printf(" [ESPACIO]   Reproducir animación desde keyframes.txt\n");
	printf(" [0]         Reiniciar animación para volver a reproducir\n");
	printf(" [1]         Activar/Desactivar modo edición\n");
	printf("    ├─ H / K Mover en eje X  (H = +X / K = -X)\n");
	printf("    ├─ U / J Mover en eje Y  (U = +Y / J = -Y)\n");
	printf("    └─ R     Activar/Desactivar modo rotación\n");
	printf(" [Flecha Izq/Der]	Rotar el modelo en eje Y\n");
	printf(" [L]         Guardar frame actual en keyframes.txt\n");
	printf(" [M]         Modificar último frame guardado\n");
	printf(" [B]         Borrar último frame del archivo\n");
	printf("=====================================\n\n");



	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);

	float radio = 5.0f;             // radio del movimiento circular
	float velocidadGiro = 0.1f;     // velocidad del giro
	float centroX = -25.0f;          // centro de la circunferencia
	float centroZ = 270.0f;         // centro de la circunferencia
	float centro2X = 25.0f;          // centro de la circunferencia
	float centro2Z = 270.0f;         // centro de la circunferencia
	float posX;
	float posZ;
	float pos2x;
	float pos2z;
	float rotY;

	//leerArchivo();


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;
		alaAngulo = 15.0f * sin(angulovaria / 1.6);

		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
		}
		rotllanta += rotllantaOffset * deltaTime;


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());

		animate(KeyFrameAvion, playIndexAvion, FrameIndexAvion, i_curr_stepsAvion,
			i_max_steps, playAvion, movAvion_x, movAvion_y, giroAvion);

		animate(KeyFrameAguila, playIndexAguila, FrameIndexAguila, i_curr_stepsAguila,
			i_max_steps, playAguila, movAguila_x, movAguila_y, giroAguila);


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//Entrada

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, -110.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ArcoTexture.UseTexture();
		Arco_M.RenderModel();

		//Puertas
		//Puerta derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.1f, 0.0f, 3.4f));
		//model = glm::rotate(model, -glm::radians(anguloPuerta), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		Puerta_Derecha_M.RenderModel();
		//Puerta izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.1f, 0.0f, 3.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		Puerta_Izquierda_M.RenderModel();

		//Letrero
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(26.0, -2.0f, 7.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lumbreraTexture.UseTexture();
		lumbrera.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lumbreraTexture.UseTexture();
		lumbrera.RenderModel();

		//aguila
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, 48.0f, 200.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movAguila_x, movAguila_y, 0.0f));
		model = glm::rotate(model, glm::radians(giroAguila), glm::vec3(0, 0, 1));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguilaTexture.UseTexture();
		aguila_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, -1.0f, 1.9f));
		model = glm::rotate(model, glm::radians(alaAngulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, -1.0f, 2.0f));
		model = glm::rotate(model, -glm::radians(alaAngulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaIzq_M.RenderModel();





		//nido
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, -2.0f, 200.0f));
		model = glm::scale(model, glm::vec3(2.0f, 6.0f, 2.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		nidoTexture.UseTexture();
		nido_M.RenderModel();


		//habitat ajolote
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 260.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		habitat_ajoloteTexture.UseTexture();
		habitat_ajolote_M.RenderModel();

		//ajolote
		posX = centroX + radio * cos(angulovaria * velocidadGiro);
		posZ = centroZ + radio * sin(angulovaria * velocidadGiro);
		pos2x = centro2X + 3 * cos(angulovaria * velocidadGiro + 3.14f);
		pos2z = centro2Z + 3 * sin(angulovaria * velocidadGiro + 3.14f);
		
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(25.0f, 1.0f, 190.0f));
		model = glm::translate(model, glm::vec3(posX, 1.0f, posZ));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolote_M.RenderModel();

		//cola
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axoloteCola_M.RenderModel();

		//pata derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataDerecha_M.RenderModel();

		//pata izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, -0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataIzquierda_M.RenderModel();

		//pata trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraDerecha_M.RenderModel();

		//pata trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, -0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraIzquierda_M.RenderModel();

		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(25.0f, 1.0f, 190.0f));
		model = glm::translate(model, glm::vec3(pos2x, 1.0f, pos2z));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolote_M.RenderModel();


		//cola
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axoloteCola_M.RenderModel();

		//pata derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f*sin(angulovaria/10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataDerecha_M.RenderModel();

		//pata izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, -0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataIzquierda_M.RenderModel();

		//pata trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f*sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraDerecha_M.RenderModel();

		//pata trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, -0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f*sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraIzquierda_M.RenderModel();



		//jaguar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, 2.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		troncoTexture.UseTexture();
		tronco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 30.0f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();
		model = glm::translate(model, glm::vec3(6.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(185.0f, -2.0f, 85.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolTexture.UseTexture();
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolTexture.UseTexture();
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 10.0f, 50.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piedrasTexture.UseTexture();
		piedras_M.RenderModel();

		//Piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -2.0f, 215.0f));
		model = glm::scale(model, glm::vec3(55.0f, 55.0f, 55.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piramideTexture.UseTexture();
		piramide_M.RenderModel();



		//Ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 80.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ringTexture.UseTexture();
		ring_M.RenderModel();


		//Aviario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolAviarioTexture.UseTexture();
		arbolAviario_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-125.0f, 20.0f, 105.0f));
		//model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pajaroTexture.UseTexture();
		pajaro_M.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	// --- CAMBIAR MODELO ACTIVO ---
	if (keys[GLFW_KEY_F1]) {
		archivoActual = "keyframes.txt";

	}
	if (keys[GLFW_KEY_F2]) {
		archivoActual = "keyframes_aguila.txt";
	}
	FRAME* KeyFrameActivo;
	int* FrameIndexActivo;
	float* movX;
	float* movY;
	float* giro;
	int* playIndex;
	int* i_curr_steps;
	bool* play;

	if (archivoActual == "keyframes.txt") {
		KeyFrameActivo = KeyFrameAvion;
		FrameIndexActivo = &FrameIndexAvion;
		movX = &movAvion_x;
		movY = &movAvion_y;
		giro = &giroAvion;
		playIndex = &playIndexAvion;
		i_curr_steps = &i_curr_stepsAvion;
		play = &playAvion;
	}
	else {
		KeyFrameActivo = KeyFrameAguila;
		FrameIndexActivo = &FrameIndexAguila;
		movX = &movAguila_x;
		movY = &movAguila_y;
		giro = &giroAguila;
		playIndex = &playIndexAguila;
		i_curr_steps = &i_curr_stepsAguila;
		play = &playAguila;
	}
	static bool pressed1 = false;
	if (keys[GLFW_KEY_1]) {
		if (!pressed1) {
			modoEdicion = !modoEdicion;
			modoRotacion = false;
			printf(modoEdicion ? "Modo edicion activado.\n" : "Modo edición desactivado.\n");
			pressed1 = true;
		}
	}
	else pressed1 = false;

	static bool pressedR = false;
	if (modoEdicion && keys[GLFW_KEY_R]) {
		if (!pressedR) {
			modoRotacion = !modoRotacion;
			printf(modoRotacion ? "Modo rotacion activado.\n" : "Modo traslación activado.\n");
			pressedR = true;
		}
	}
	else pressedR = false;

	if (modoEdicion) {
		float vel = 0.05f, velRot = 1.5f;
		if (!modoRotacion) {
			if (keys[GLFW_KEY_H]) *movX += vel;
			if (keys[GLFW_KEY_K]) *movX -= vel;
			if (keys[GLFW_KEY_U]) *movY += vel;
			if (keys[GLFW_KEY_J]) *movY -= vel;
		}
		else {
			if (keys[GLFW_KEY_LEFT]) *giro -= velRot;
			if (keys[GLFW_KEY_RIGHT]) *giro += velRot;
		}
	}

	// --- GUARDAR FRAME ---
	static bool pressedL = false;
	if (keys[GLFW_KEY_L]) {
		if (!pressedL) {
			saveFrame(archivoActual, KeyFrameActivo, *FrameIndexActivo, *movX, *movY, *giro);
			pressedL = true;
		}
	}
	else pressedL = false;

	// --- MODIFICAR FRAME ---
	static bool pressedM = false;
	if (keys[GLFW_KEY_M]) {
		if (!pressedM && *FrameIndexActivo > 0) {
			int modIndex = *FrameIndexActivo - 1;
			(KeyFrameActivo)[modIndex].movX = *movX;
			(KeyFrameActivo)[modIndex].movY = *movY;
			(KeyFrameActivo)[modIndex].giro = *giro;
			actualizarArchivo(archivoActual, modIndex, KeyFrameActivo[modIndex]);
			pressedM = true;
		}
	}
	else pressedM = false;

	// --- BORRAR FRAME ---
	static bool pressedB = false;
	if (keys[GLFW_KEY_B]) {
		if (!pressedB && *FrameIndexActivo > 0) {
			borrarFrame(archivoActual, *FrameIndexActivo - 1);
			(*FrameIndexActivo)--;
			pressedB = true;
		}
	}
	else pressedB = false;

	// --- REPRODUCIR ANIMACIÓN ---
	if (keys[GLFW_KEY_SPACE]) {
		if (reproducirAnimacion < 1) {
			if (!*play) {
				leerArchivo(archivoActual, KeyFrameActivo, *FrameIndexActivo);
				if (*FrameIndexActivo > 1) {
					resetElements(KeyFrameActivo, *movX, *movY, *giro);
					interpolation(KeyFrameActivo, *playIndex, i_max_steps);
					*play = true;
					*playIndex = 0;
					*i_curr_steps = 0;
					reproducirAnimacion++;
				}
				else
					printf("No hay suficientes frames en %s para reproducir.\n", archivoActual.c_str());
			}
		}
	}
	else reproducirAnimacion = 0;
}

