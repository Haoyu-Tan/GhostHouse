//CSCI 5607 Project4
//Maze Game

#include "../glad/glad.h"  //Include order can matter here
#ifdef __APPLE__
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_opengl.h>
#else
 #include <SDL.h>
 #include <SDL_opengl.h>
#endif
#include <cstdio>
#include <vector>

//For Visual Studios
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define GLM_FORCE_RADIANS
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include <cstdio>

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

#include "../include/MyStruct.h"
#include "../include/Player.h"
#include "../include/StaticFurniture.h"


bool loadDTextureFile(char pathName[], Frame* frame);
bool loadTextureFile(char pathName[], Model* model);
int loadModels(vector<float> *outModel, vector<float>* dOutModel, GLuint sProgram, SDL_Window* window);
int addAnimation(std::string path, int Anum, int Fnum, vector<float>* dOutModel, DModel* dmodel, int modelType);
bool parseSceneFile(std::string fileName, Scene* scene);
static char* readShaderSource(const char* shaderFile);
GLuint InitShader(const char* vShaderFileName, const char* fShaderFileName);

//path variables
char modelPath[] = "models/texture/";
char wgTextPath[] = "models/dynamic/WhiteGhost/texture/";
char bgTextPath[] = "models/dynamic/BlueGhost/texture/";
char ggTextPath[] = "models/dynamic/GreenGhost/texture/";
char gunTexPath[] = "models/dynamic/Gun/texture/";

//variables
bool fullscreen = false;
int screen_width = 800;
int screen_height = 600;

float nearPlane = 0.1f;
float farPlane = 100.0f;

char window_title[] = "My Final Game";

float avg_render_time = 0;

bool DEBUG_ON = true;

float timePast = 0;
bool saveOutput = false;



float scaleFactor = 2.0f;

vector<std::string> modelFiles{ "models/floor.obj", "models/StoneWall.obj", "models/door.obj", "models/key.obj",
"models/Bookcase.obj","models/Chest_gold1.obj", "models/smallStand.obj",
"models/Torch.obj", "models/Table.obj", "models/Bookcase_Books1.obj", "models/Bones.obj",
"models/Desk.obj", "models/standCandle.obj",
"models/health_base1.obj", "models/health_bar1.obj" };
vector<std::string> dynamicModelFiles{"models/dynamic/WhiteGhost/whiteGhost_",
									"models/dynamic/WhiteGhost/whiteGhost_forward_0000",
									"models/dynamic/BlueGhost/beforeThrow_0000",
									"models/dynamic/BlueGhost/afterThrow_0000",
									"models/dynamic/BlueGhost/blueGhost_ball",
									"models/dynamic/Gun/RayGun",
									"models/dynamic/Gun/flatBullet",
									"models/dynamic/jump_cthulhu_00"};


vector<Model*> models;
vector<DModel*> dynamicModels;
vector<float> outModel;
vector<float> dOutModel; // VBO content

//////////////////
float vertices[] = {  //The function updateVertices() changes these values to match p1,p2,p3,p4
//  X     Y     R     G     B     U    V
  0.4f/ float(screen_width) * float(screen_height),  0.6f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
  0.4f / float(screen_width) * float(screen_height), -0.2f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
  -0.4f / float(screen_width) * float(screen_height),  0.6f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left 
  -0.4f / float(screen_width) * float(screen_height), -0.2f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
  // rect 2
   0.425f,  -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
  0.425f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
  -0.425f,  -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top left 
  -0.425f, -0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
  // rect 3
   -0.4f,  -0.325f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
  -0.4f, -0.375f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
  -0.4f,  -0.325f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top left 
  -0.4f, -0.375f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
};
unsigned char* loadImage(int& img_w, int& img_h, string tn);
int img_w, img_h;
string textureName = "./textures/GPF.ppm";
unsigned char* img_data = loadImage(img_w, img_h, textureName);

const float myFaceFront = -180.0f * M_PI / 180.0f;
const float myRotateRight = 90.0f * M_PI / 180.0f;
const float myRotateLeft = -90.0f * M_PI / 180.0f;
const float myFaceBack = 0.0f;




///////////////////
unsigned char* loadImage(int& img_w, int& img_h, string tn) {

	//Open the texture image file
	ifstream ppmFile;
	ppmFile.open(tn.c_str());
	if (!ppmFile) {
		printf("ERROR: Texture file '%s' not found.\n", tn.c_str());
		exit(1);
	}

	//Check that this is an ASCII PPM (first line is P3)
	string PPM_style;
	ppmFile >> PPM_style; //Read the first line of the header    
	if (PPM_style != "P3") {
		printf("ERROR: PPM Type number is %s. Not an ASCII (P3) PPM file!\n", PPM_style.c_str());
		exit(1);
	}

	//Read in the texture width and height
	ppmFile >> img_w >> img_h;
	unsigned char* img_data = new unsigned char[4 * img_w * img_h];
	//Check that the 3rd line is 255 (ie., this is an 8 bit/pixel PPM)
	int maximum;
	ppmFile >> maximum;
	if (maximum != 255) {
		printf("ERROR: Maximum size is (%d) not 255.\n", maximum);
		exit(1);
	}

	//TODO: This loop puts in fake data, replace with the actual pixels read from the file
	//      ... see project description for a hint on how to do this
	int r, g, b;
	for (int i = img_h - 1; i >= 0; i--) {
		//float fi = i/(float)img_h;
		for (int j = 0; j < img_w; j++) {

			/*float fj = j/(float)img_w;
			img_data[i*img_w*4 + j*4] = 50;  //Red
			img_data[i*img_w*4 + j*4 + 1] = fj*150;  //Green
			img_data[i*img_w*4 + j*4 + 2] = fi*250;  //Blue*/
			ppmFile >> r >> g >> b;
			img_data[i * img_w * 4 + j * 4] = r;
			img_data[i * img_w * 4 + j * 4 + 1] = g;
			img_data[i * img_w * 4 + j * 4 + 2] = b;
			//ppmFile >> img_data[i * img_w * 4 + j * 4 + 1];
			//ppmFile >> img_data[i * img_w * 4 + j * 4 + 2];

			img_data[i * img_w * 4 + j * 4 + 3] = 255;  //Alpha
		}
	}
	ppmFile.close();

	return img_data;
}

/////


int main(int argc, char *argv[]) {

	//read command line
	if (argc != 2) {
		cout << "Usage: ./a.out scenefile\n";
		return (0);
	}
	//SDL_Init(SDL_INIT_VIDEO);

	SDL_Init(SDL_INIT_VIDEO);  //Initialize Graphics (for OpenGL)

  //Ask SDL to get a recent version of OpenGL (3.2 or greater)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


	//Create a window (offsetx, offsety, width, height, flags)
	SDL_Window* window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_OPENGL);
	float aspect = screen_width / (float)screen_height; //aspect ratio (needs to be updated if the window is resized)

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		printf("\nOpenGL loaded\n");
		printf("Vendor:   %s\n", glGetString(GL_VENDOR));
		printf("Renderer: %s\n", glGetString(GL_RENDERER));
		printf("Version:  %s\n\n", glGetString(GL_VERSION));

	}
	else {
		printf("ERROR: Failed to initialize OpenGL context.\n");
		return -1;
	}
	
	GLuint tex0;
	glGenTextures(1, &tex0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	//What to do outside 0-1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
	//TODO: TEST your understanding: Try GL_LINEAR instead of GL_NEAREST on the 4x4 test image. What is happening?
	


	//printf("Loaded Image of size (%d,%d)\n",img_w,img_h);
	//memset(img_data,0,4*img_w*img_h); //Load all zeros
	//Load the texture into memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_w, img_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	GLuint lvao;
	glGenVertexArrays(1, &lvao); //Create a VAO
	glBindVertexArray(lvao); //Bind the above created VAO to the current context


	//Allocate memory on the graphics card to store geometry (vertex buffer object)
	GLuint lvbo;
	glGenBuffers(1, &lvbo);  //Create 1 buffer called vbo
	glBindBuffer(GL_ARRAY_BUFFER, lvbo); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); //upload vertices to vbo
	const GLchar* vertexSource =
		"#version 150 core\n"
		"in vec2 position;"
		"in vec3 inColor;"
		"in vec2 inTexcoord;"
		"out vec3 Color;"
		"out vec2 texcoord;"
		"void main() {"
		"   Color = inColor;"
		"   gl_Position = vec4(position, 0.0, 1.0);"
		"   texcoord = inTexcoord;"
		"}";

	const GLchar* fragmentSource =
		"#version 150 core\n"
		"uniform sampler2D tex0;"
		"in vec2 texcoord;"
		"out vec3 outColor;"
		"in vec3 Color;"
		"uniform int ind;"
		"void main() {"
		"   if(ind < 0){"
		"       outColor = Color.rgb;"
		"       return;}"
		"   outColor = texture(tex0, texcoord).rgb;"
		"}";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		printf("Vertex Shader Compile Failed. Info:\n\n%s\n", buffer);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//Double check the shader compiled 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		printf("Fragment Shader Compile Failed. Info:\n\n%s\n", buffer);
	}

	//Join the vertex and fragment shaders together into one program
	GLuint sProgram = glCreateProgram();
	glAttachShader(sProgram, vertexShader);
	glAttachShader(sProgram, fragmentShader);
	glBindFragDataLocation(sProgram, 0, "outColor"); // set output
	glLinkProgram(sProgram); //run the linker

	glUseProgram(sProgram); //Set the active shader (only one can be used at a time)

	
    
	GLint posAttribl = glGetAttribLocation(sProgram, "position");
	// Attribute, vals/attrib., type, normalized?, stride, offset
	glVertexAttribPointer(posAttribl, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttribl); //Binds the VBO current GL_ARRAY_BUFFER 

	GLint colAttribl = glGetAttribLocation(sProgram, "inColor");
	glVertexAttribPointer(colAttribl, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colAttribl);

	GLint texAttrib = glGetAttribLocation(sProgram, "inTexcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	glClearColor(0.1f,0.1f,0.1f ,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	printf("I am here\n");

	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glUniform1i(glGetUniformLocation(sProgram, "ind"), 1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //Draw the two triangles (4 vertices) making up the square
	  //TODO: TEST your understanding: What shape do you expect to see if you change the above 4 to 3?  Guess, then try it!
	glUniform1i(glGetUniformLocation(sProgram, "ind"), -1);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); //Draw the two triangles (4 vertices) making up the square
   //TODO: TEST your understanding: What shape do you expect to see if you change the above 4 to 3?  Guess, then try it!
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4); //Draw the two triangles (4 vertices) making up the square
	SDL_GL_SwapWindow(window); //Double buffering


   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	outModel.clear();
	dOutModel.clear();

	int successload = loadModels(&outModel, &dOutModel, sProgram,window);


	/*
	for (int i = 0; i < (dOutModel.size() / 22); i++) {
		int offset = i * 22;
		if (offset != 22) {
			printf("model: %d\n", i);
			printf("position: %f, %f, %f\n", dOutModel[offset], dOutModel[offset + 1], dOutModel[offset + 2]);
			printf("Ns: %f\n", dOutModel[offset + 3]);
			printf("Ka: %f, %f, %f\n", dOutModel[offset + 4], dOutModel[offset + 5], dOutModel[offset + 6]);
			printf("Kd: %f, %f, %f\n", dOutModel[offset + 7], dOutModel[offset + 8], dOutModel[offset + 9]);
			printf("Ks: %f, %f, %f\n", dOutModel[offset + 10], dOutModel[offset + 11], dOutModel[offset + 12]);
			printf("Ke: %f, %f, %f\n", dOutModel[offset + 13], dOutModel[offset + 14], dOutModel[offset + 15]);
			printf("Ni: %f\n", dOutModel[offset + 16]);
			printf("d: %f\n", dOutModel[offset + 17]);
			printf("illum: %f\n", dOutModel[offset + 18]);
			printf("normal: %f, %f, %f\n", dOutModel[offset + 19], dOutModel[offset + 20], dOutModel[offset + 21]);
		}
	}
	*/

	/*
	for (int i = 0; i < dynamicModels.size(); i++) {
		DModel* currModel = dynamicModels.at(i);
		printf();
	}


	for (int i = 0; i < dynamicModels.size(); i++) {
		printf("===============================\n");
		printf("model: %d\n", i);

		DModel *dmodel = dynamicModels.at(i);
		printf("model start: %d\n", dmodel->startIndex);
		for (int j = 0; j < (dmodel->animations).size(); j++) {
			Animation* animation = (dmodel->animations).at(j);
			printf("animation: %d\n", j);
			printf("animation start index: %d\n", animation->startIndex);

			for (int k = 0; k < (animation->frames).size(); k++) {
				Frame* fr = (animation->frames).at(k);
				printf("frame: %d\n", k);
				printf("frame start index: %\d\n", fr->startIndex);
				printf("frame total vertices: %d\n", fr->totalVert);
				for (int l = 0; l < (fr->parts).size(); l++) {
					Part p = (fr->parts).at(l);
					printf("part: %d\n", l);
					printf("part start index: %d\n", p.startIndex);
					printf("part total vert: %d\n", p.totalVert);
				}
			}
		}

		printf("========================\n");
	}
	*/
	std::string sceneFileName = argv[1];


	Scene* scene = new Scene();
	scene->scaleFactor = scaleFactor;
	(scene->pgun) = new PGun(dynamicModels.at(gun));
	//printf("before load scene file!!!!!!!!!!\n");
	//read file and store scene data
	bool isOpen = parseSceneFile(sceneFileName, scene);

	if (!isOpen) return -1;

	scene->printSceneInfo();
	//start loading model
	

	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	GLuint vbo[1];
	glGenBuffers(1, vbo);  //Create 1 buffer called vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); //Set the vbo as the active array buffer (Only one buffer can be active at a time)
	glBufferData(GL_ARRAY_BUFFER, outModel.size()* sizeof(float), &outModel[0], GL_STATIC_DRAW); //upload vertices to vbo

	int texturedShader = InitShader("shaderSrc/textured-Vertex.glsl", "shaderSrc/textured-Fragment.glsl");

	GLint posAttrib = glGetAttribLocation(texturedShader, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint NsAttrib = glGetAttribLocation(texturedShader, "Ns");
	glVertexAttribPointer(NsAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(NsAttrib);

	GLint KaAttrib = glGetAttribLocation(texturedShader, "Ka");
	glVertexAttribPointer(KaAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(KaAttrib);

	GLint KdAttrib = glGetAttribLocation(texturedShader, "Kd");
	glVertexAttribPointer(KdAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(KdAttrib);

	GLint KsAttrib = glGetAttribLocation(texturedShader, "Ks");
	glVertexAttribPointer(KsAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(KsAttrib);

	GLint KeAttrib = glGetAttribLocation(texturedShader, "Ke");
	glVertexAttribPointer(KeAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(13 * sizeof(float)));
	glEnableVertexAttribArray(KeAttrib);

	GLint NiAttrib = glGetAttribLocation(texturedShader, "Ni");
	glVertexAttribPointer(NiAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(NiAttrib);

	GLint dAttrib = glGetAttribLocation(texturedShader, "d");
	glVertexAttribPointer(dAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(17 * sizeof(float)));
	glEnableVertexAttribArray(dAttrib);

	GLint illumAttrib = glGetAttribLocation(texturedShader, "illum");
	glVertexAttribPointer(illumAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(18 * sizeof(float)));
	glEnableVertexAttribArray(illumAttrib);

	GLint normAttrib = glGetAttribLocation(texturedShader, "inNormal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(19 * sizeof(float)));
	glEnableVertexAttribArray(normAttrib);

	GLint uniView = glGetUniformLocation(texturedShader, "view");
	GLint uniProj = glGetUniformLocation(texturedShader, "proj");

	glBindVertexArray(0); //Unbind the VAO in case we want to create a new one	

	GLuint vao2;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	DModel* dmodel = dynamicModels.at(0);
	Frame* fr = ((dmodel->animations).at(0)->frames).at(0);

	GLuint vbo2;
	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, dOutModel.size() * sizeof(float), &dOutModel[0], GL_STATIC_DRAW);

	posAttrib = glGetAttribLocation(texturedShader, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	NsAttrib = glGetAttribLocation(texturedShader, "Ns");
	glVertexAttribPointer(NsAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(NsAttrib);

	KaAttrib = glGetAttribLocation(texturedShader, "Ka");
	glVertexAttribPointer(KaAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(KaAttrib);

	KdAttrib = glGetAttribLocation(texturedShader, "Kd");
	glVertexAttribPointer(KdAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(KdAttrib);

	KsAttrib = glGetAttribLocation(texturedShader, "Ks");
	glVertexAttribPointer(KsAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(KsAttrib);

	KeAttrib = glGetAttribLocation(texturedShader, "Ke");
	glVertexAttribPointer(KeAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(13 * sizeof(float)));
	glEnableVertexAttribArray(KeAttrib);

	NiAttrib = glGetAttribLocation(texturedShader, "Ni");
	glVertexAttribPointer(NiAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(NiAttrib);

	dAttrib = glGetAttribLocation(texturedShader, "d");
	glVertexAttribPointer(dAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(17 * sizeof(float)));
	glEnableVertexAttribArray(dAttrib);

	illumAttrib = glGetAttribLocation(texturedShader, "illum");
	glVertexAttribPointer(illumAttrib, 1, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(18 * sizeof(float)));
	glEnableVertexAttribArray(illumAttrib);

	normAttrib = glGetAttribLocation(texturedShader, "inNormal");
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 22 * sizeof(float), (void*)(19 * sizeof(float)));
	glEnableVertexAttribArray(normAttrib);


	glBindVertexArray(0); //Unbind the VAO in case we want to create a new one	


	glEnable(GL_DEPTH_TEST);


	// Initialize player and disable locked cursor
	Player* player = new Player(scene);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_WarpMouseInWindow(window, screen_width / 2, screen_height / 2);

	//glm::vec3 cameraPos = glm::vec3(10.0f, 30.0f, 0.0f);
	//glm::vec3 cameraFront = glm::vec3(10.0f, 1.0f, 0.0f);
	//glm::vec3 cameraUp = glm::vec3(1.0f, 0.0f, 0.0f);
	//glm::vec4 lightDir = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//Time
	float t_start = SDL_GetTicks();
	float t_end, delta_t;
	glm::vec3 skyColor = glm::vec3(0.2f, 0.4f, 0.8f);
	bool day = true;
	glm::vec3 plpos[20];
	//Event Loop (Loop forever processing each event as fast as possible)
	SDL_Event windowEvent;
	bool quit = false;
	bool oneP = false;
	bool rightBotton = false;
	int fIndex = 0;


	delete[] img_data;
	glDeleteProgram(sProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &lvbo);

	glDeleteVertexArrays(1, &lvao);

	while (!quit) {
		if (scene->gameState == 1) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You Win", "Congratulations, you win the game!", window);
			break;
		}
		else if (scene->gameState == -1) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "You Lose!", "Ohhhhh! You Lose :(", window);
			break;

		}
		while (SDL_PollEvent(&windowEvent)) {  //inspect all events in the queue
			if (windowEvent.type == SDL_QUIT) quit = true;
			//List of keycodes: https://wiki.libsdl.org/SDL_Keycode - You can catch many special keys
			//Scancode referes to a keyboard position, keycode referes to the letter (e.g., EU keyboards)
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE)
				quit = true; //Exit event loop
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) { //If "f" is pressed
				fullscreen = !fullscreen;
				SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN : 0); //Toggle fullscreen 
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_w) { //If "w" is pressed
				player->fstart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_s) { //If "s" is pressed
				player->bstart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_a) { //If "a" is pressed
				player->lstart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_d) { //If "d" is pressed
				player->rstart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_LEFT) { //If "LEFT" is pressed
				player->rlstart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_RIGHT) { //If "RIGHT" is pressed
				player->rrstart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_UP) { //If "up" is pressed
				player->rustart();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_DOWN) { //If "down" is pressed
				player->rdstart();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_w) { //If "w" is pressed
				player->fend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_s) { //If "s" is pressed
				player->bend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_a) { //If "a" is pressed
				player->lend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_d) { //If "d" is pressed
				player->rend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_LEFT) { //If "left" is pressed
				player->rlend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_RIGHT) { //If "right" is pressed
				player->rrend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_UP) { //If "up" is pressed
				player->ruend();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_DOWN) { //If "down" is pressed
				player->rdend();
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_SPACE) {
				player->jump();
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_b) { //if b is press
				if (day) {
					skyColor = glm::vec3(0.0f, 0.0f, 0.0f);
					day = false;
				}
				else {
					skyColor = glm::vec3(0.2f, 0.4f, 0.8f);
					day = true;
				}
			}
			if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_1) {
				if (!oneP) {
					player->GunSwitch();
				}
				oneP = true;
			}
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_1) {
				
				oneP = false;
			}
			
			if (windowEvent.type == SDL_MOUSEMOTION) {
				//printf("motionx is %i, y is %i \n", windowEvent.motion.xrel, windowEvent.motion.yrel);
				player->rotate(windowEvent.motion.x - screen_width / 2, windowEvent.motion.y - screen_height / 2);
			}
			if (windowEvent.type == SDL_MOUSEBUTTONDOWN && windowEvent.button.button == SDL_BUTTON_RIGHT) {
				if (!rightBotton) {
					player->TorchONOFF();
				}
				rightBotton = true;
			}
			if (windowEvent.type == SDL_MOUSEBUTTONUP && windowEvent.button.button == SDL_BUTTON_RIGHT) {
				rightBotton = false;
			}
			if (windowEvent.type == SDL_MOUSEBUTTONDOWN && windowEvent.button.button == SDL_BUTTON_LEFT) {
				player->AttackStart();
			}
			if (windowEvent.type == SDL_MOUSEBUTTONUP && windowEvent.button.button == SDL_BUTTON_LEFT) {
				player->AttackStop();
			}
			
		}


		/////////////////////////////////
		// here we need to call the update
		t_end = SDL_GetTicks();
		delta_t = (t_end - t_start) / 1000.f;
		t_start = SDL_GetTicks();
		player->Update(delta_t);
		
		scene->Update(delta_t, player->position, player->direction);
		/*
		int keySize = (scene->keys).size();
		for (int i = 0; i < keySize; i++) {
			scene->keys.at(i)->update(delta_t);
		}

		
		for (int i = 0; i < (scene->doors).size(); i++) {
			Door* door = (scene->doors).at(i);
			if (door->isOpen) door->update(delta_t);
		}
		*/

		// also lock the cursor
		SDL_WarpMouseInWindow(window, screen_width / 2, screen_height / 2);

		// Clear the screen to default color
		//glClearColor(.2f, 0.4f, 0.8f, 1.0f);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(skyColor.x, skyColor.y, skyColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(texturedShader);
		GLint uniTorchDir = glGetUniformLocation(texturedShader, "tdir");
		glUniform3fv(uniTorchDir, 1, glm::value_ptr(player->GetDirection()));
		GLint uniTorchPos = glGetUniformLocation(texturedShader, "tpos");
		glUniform3fv(uniTorchPos, 1, glm::value_ptr(player->GetPosition()));

		
		int plsize = (scene->candels).size() + 1;
		for (int i = 0; i < plsize; i++) {
			if (i == plsize - 1) {
				plpos[i] = glm::vec3(-scene->goal_x * scaleFactor, 2.0, scene->goal_y * scaleFactor);
			}
			else {
				Candel* candel = scene->candels.at(i);
				plpos[i] = candel->position + glm::vec3(0.0, 1.4, 0.0);
			}
		}
		




		glUniform3fv(glGetUniformLocation(texturedShader, "ppos"), plsize, glm::value_ptr(plpos[0]));
		glUniform1i(glGetUniformLocation(texturedShader, "pnum"), plsize);
		glUniform1i(glGetUniformLocation(texturedShader, "pn"), plsize);
		glUniform3fv(glGetUniformLocation(texturedShader, "fc"), 1, glm::value_ptr(player->faintColor));

		
		timePast = SDL_GetTicks() / 1000.f;
		
		glm::mat4 view = player->GetView();
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 proj = glm::perspective(3.14f / 4, screen_width / (float)screen_height, nearPlane, farPlane); //FOV, aspect, near, far
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1i(glGetUniformLocation(texturedShader, "tStatus"), player->torchStatus);

		//scene->whiteGhosts.at(0)->update(delta_t);
		//drawGeometry(scene, texturedShader, startVertFloor, startVertWall, startVertDoor, startVertKey, startVertGoal, startVertFruit, totalVert, player);
		scene->drawGeometry(texturedShader, models, timePast, scaleFactor, vao,vbo);
		scene->drawDGeometry(texturedShader, vao2, vbo2);

		fIndex += 1;
		if (fIndex > 79)
			fIndex = 0;
		
		SDL_GL_SwapWindow(window); //Double buffering
	}


	//Clean Up
	glDeleteProgram(texturedShader);
	//if more shader program, delete also
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, &vao);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	delete player;
	delete scene;

	
	for (int i = 0; i < models.size(); i++) {
		Model* model = models.at(i);
		delete model;
	}
	
	for (int i = 0; i < dynamicModels.size(); i++) {
		DModel* dmodel = dynamicModels.at(i);
		delete dmodel;
	}

	return 0;
}


int loadModels(vector<float> *outModel, vector<float> *dOutModel, GLuint sProgram, SDL_Window* window) {
	//load static models
	for (int i = 0; i < modelFiles.size(); i++) {
		float percent = float(i) / float(modelFiles.size()+5.0f);
		float newx = 0.8f * percent - 0.4f;
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		vertices[56] = newx;
		vertices[63] = newx;
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glUniform1i(glGetUniformLocation(sProgram, "ind"), 1);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); //Draw the two triangles (4 vertices) making up the square
		  //TODO: TEST your understanding: What shape do you expect to see if you change the above 4 to 3?  Guess, then try it!
		glUniform1i(glGetUniformLocation(sProgram, "ind"), -1);
		glDrawArrays(GL_TRIANGLE_STRIP, 4, 4); //Draw the two triangles (4 vertices) making up the square
	   //TODO: TEST your understanding: What shape do you expect to see if you change the above 4 to 3?  Guess, then try it!
		glDrawArrays(GL_TRIANGLE_STRIP, 8, 4); //Draw the two triangles (4 vertices) making up the square
		SDL_GL_SwapWindow(window); //Double buffering




		std::string fileName = modelFiles.at(i);

		FILE* modelFile = fopen(fileName.c_str(), "r");
		if (modelFile == NULL) {
			std::cout << "Can\'t open file " << fileName << "\n" << std::endl;
			continue;
		}

		//temp vars
		vector<glm::vec3> tempVert;
		vector<glm::vec3> tempNorm;
		Material material;

		//model and part info
		Model* currModel = new Model();
		models.push_back(currModel);

		Part part;
		Material mat;

		//index info
		currModel->startIndex = outModel->size() / 22; //the start index of the model in 'vbo' !!!!!!!!!!!!!
		int modelTotalVert = 0;
		int partStartIndex = 0; //the start index of a part inside the model
		int partTotalVert = 0; 


		//reset part or not, do not add empty part to model
		bool store = false;


		while (1) {
			char line[128];

			int res = fscanf(modelFile, "%s", line);

			if (res == EOF) {
				part.startIndex = partStartIndex;
				part.totalVert = partTotalVert;
				(currModel->parts).push_back(part);
				break;
			}

			if (strcmp(line, "v") == 0) {
				
				//load vertex
				glm::vec3 vertex;
				fscanf(modelFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tempVert.push_back(vertex);

				//printf("vertex: %f, %f, %f\n", vertex.x, vertex.y, vertex.z);
			}
			else if (strcmp(line, "vn") == 0) {
				//load normal
				glm::vec3 norm;
				fscanf(modelFile, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
				tempNorm.push_back(norm);

				//printf("norm: %f, %f, %f\n", norm.x, norm.y, norm.z);
			}
			else if (strcmp(line, "mtllib") == 0) {
				//reading and loading texture file
				char path[151];
				char fn[128];
				strcpy(path, modelPath);
				fscanf(modelFile, "%s\n", fn);
				strcat(path, fn);

				bool ret = loadTextureFile(path, currModel);
				if (!ret) {
					return -1;
				}

				//printf("material size: %d\n", (currModel->materials).size());
			}
			else if (strcmp(line, "usemtl") == 0) {
				char fn[128];
				fscanf(modelFile, "%s\n", fn);

				for (int i = 0; i < (currModel->materials).size(); i++) {
					Material currMat = (currModel->materials).at(i);
					if (strcmp(currMat.mName, fn) == 0) {
						mat = currMat;
						break;
					}
				}
			}
			else if (strcmp(line, "s") == 0) {
				char isOn[5];
				fscanf(modelFile, "%s\n", isOn);

				if (strcmp(isOn, "on") == 0)
					part.s = true;
				else if (strcmp(isOn, "off") == 0)
					part.s = false;
			}
			else if (strcmp(line, "o") == 0) {
				//push previous part to model and reset counter
				if (store) {
					part.startIndex = partStartIndex;
					partStartIndex += partTotalVert;
					part.totalVert = partTotalVert;
					partTotalVert = 0;

					(currModel->parts).push_back(part);

				}
				store = true;
				
				//new part
				part = Part();

			}
			else if (strcmp(line, "f") == 0) {
				
				unsigned int vIndex[3], normIndex[3];
				int isMatch = fscanf(modelFile, "%d//%d %d//%d %d//%d\n", &vIndex[0], &normIndex[0], &vIndex[1], &normIndex[1], &vIndex[2], &normIndex[2]);

				if (isMatch != 6) {
					printf("File format does not match, please do not include texture coordinate in f\n");
					return -1;
				}

				if (vIndex[0] < 1 || vIndex[1] < 1 || vIndex[2] < 1
					|| normIndex[0] < 1 || normIndex[1] < 1 || normIndex[2] < 1) {
					printf("One of the index in f is less than 1, please check again\n");
					return -1;
				}

				//add data to outModel
				for (int j = 0; j < 3; j++) {
					int vi = vIndex[j] - 1;
					int ni = normIndex[j] - 1;


					glm::vec3 addVert = tempVert.at(vi);
					glm::vec3 addNorm = tempNorm.at(ni);

					//check bounding of the model
					currModel->max_x = max(addVert.x, currModel->max_x);
					currModel->min_x = min(addVert.x, currModel->min_x);
					currModel->max_y = max(addVert.y, currModel->max_y);
					currModel->min_y = min(addVert.y, currModel->min_y);
					currModel->max_z = max(addVert.z, currModel->max_z);
					currModel->min_z = min(addVert.z, currModel->min_z);

					//0-2: index
					outModel->push_back(addVert.x); 
					outModel->push_back(addVert.y);
					outModel->push_back(addVert.z);

					//3- 18: material
					outModel->push_back(mat.Ns);

					outModel->push_back(mat.Ka.x);
					outModel->push_back(mat.Ka.y);
					outModel->push_back(mat.Ka.z);

					outModel->push_back(mat.Kd.x);
					outModel->push_back(mat.Kd.y);
					outModel->push_back(mat.Kd.z);

					outModel->push_back(mat.Ks.x);
					outModel->push_back(mat.Ks.y);
					outModel->push_back(mat.Ks.z);

					outModel->push_back(mat.Ke.x);
					outModel->push_back(mat.Ke.y);
					outModel->push_back(mat.Ke.z);

					outModel->push_back(mat.Ni);

					outModel->push_back(mat.d);

					outModel->push_back((float)mat.illum);


					//19 - 21
					outModel->push_back(addNorm.x);
					outModel->push_back(addNorm.y);
					outModel->push_back(addNorm.z);
				}

				modelTotalVert += 3;
				partTotalVert += 3;
			}

		}
		currModel->totalVert = modelTotalVert;

		//test
		if ((currModel->totalVert) != ((outModel->size())/22 - currModel->startIndex)) {
			printf("number of vert does not match, please recheck the algorithm!!!\n");
		}

		currModel->calTrans();

		/*
		for (int m = 0; m < (currModel->parts).size(); m++) {
			Part p = (currModel->parts).at(m);
			printf("current part is: %d\n", m);
			printf("start index is: %d, total vert is: %d\n", p.startIndex, p.totalVert);
		}
		*/

		fclose(modelFile);
	}

	
	//load dynamic models

	//load white ghost
	DModel* dmodel = new DModel();
	dModelType type = whiteGhost;
	dmodel->startIndex = dOutModel->size() / 22;
	dynamicModels.push_back(dmodel);
	addAnimation(dynamicModelFiles.at(0), 1, 80, dOutModel, dmodel, type);
	addAnimation(dynamicModelFiles.at(1), 1, 30, dOutModel, dmodel, type);
	
	//load blue ghost
	DModel* dmodelB = new DModel();
	dModelType typeB = blueGhost;
	dmodelB->startIndex = dOutModel->size() / 22;
	dynamicModels.push_back(dmodelB);
	addAnimation(dynamicModelFiles.at(2), 1, 50, dOutModel, dmodelB, typeB);
	addAnimation(dynamicModelFiles.at(3), 1, 51, dOutModel, dmodelB, typeB);
	addAnimation(dynamicModelFiles.at(4), 1, 1, dOutModel, dmodelB, typeB);

	//load gun
	DModel* dmodelGun = new DModel();
	dModelType typeGun = gun;
	dmodelGun->startIndex = dOutModel->size() / 22;
	dynamicModels.push_back(dmodelGun);
	addAnimation(dynamicModelFiles.at(5), 1, 1, dOutModel, dmodelGun, typeGun);
	addAnimation(dynamicModelFiles.at(6), 1, 1, dOutModel, dmodelGun, typeGun);

	DModel* dmodelGG = new DModel();
	dModelType typeGG = greenGhost;
	dmodelGG->startIndex = dOutModel->size() / 22;
	dynamicModels.push_back(dmodelGG);
	addAnimation(dynamicModelFiles.at(7), 1, 1, dOutModel, dmodelGG, typeGG);

	//printf("blue ghost animation size is: %d\n", (dmodelB->animations).size());
	//blue ball
	/*
	DModel* dmodelBB = new DModel();
	dmodelBB->startIndex = dOutModel->size() / 22;
	dynamicModels.push_back(dmodelBB);
	addAnimation(dynamicModelFiles.at(4), 1, 1, dOutModel, dmodelBB, typeB);
	*/

	return 1;
}


int addAnimation(std::string path, int Anum, int Fnum, vector<float>* dOutModel, DModel *dmodel, int modelType) {
	for (int m = 1; m <= Anum; m++) {
		Animation *a = new Animation();
		a->startIndex = dOutModel->size() / 22;
		(dmodel->animations).push_back(a);
		//add frame
		for (int n = 1; n <= Fnum; n++) {
			char dfname[68];
			strcpy(dfname, path.c_str());
			strcat(dfname, std::to_string(n).c_str());
			strcat(dfname, ".obj");

			FILE* modelFile = fopen(dfname, "r");
			if (modelFile == NULL) {
				std::cout << "Can\'t open file " << dfname << "\n" << std::endl;
				continue;
			}
			//printf("file %s open success!\n", dfname);

			//temp vars
			vector<glm::vec3> tempVert;
			vector<glm::vec3> tempNorm;

			//frame info
			Frame *fr = new Frame();
			(a->frames).push_back(fr);
			fr->startIndex = dOutModel->size() / 22;
			int frTotalVert = 0;

			//Part info
			Part part;
			Material mat;

			int partStartIndex = 0; //the start index of a part inside the model
			int partTotalVert = 0;

			//reset part or not, do not add empty part to model
			bool store = false;

			while (1) {
				char line[128];

				int res = fscanf(modelFile, "%s", line);

				if (res == EOF) {
					if (store) {
						part.startIndex = partStartIndex;
						part.totalVert = partTotalVert;
						(fr->parts).push_back(part);
					}
					break;
				}

				if (strcmp(line, "v") == 0) {

					//load vertex
					glm::vec3 vertex;
					fscanf(modelFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					tempVert.push_back(vertex);

					//printf("vertex: %f, %f, %f\n", vertex.x, vertex.y, vertex.z);
				}
				else if (strcmp(line, "vn") == 0) {
					//load normal
					glm::vec3 norm;
					fscanf(modelFile, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
					tempNorm.push_back(norm);

					//printf("norm: %f, %f, %f\n", norm.x, norm.y, norm.z);
				}
				else if (strcmp(line, "mtllib") == 0) {
					//reading and loading texture file
					char path[200];
					char fn[128];
					if (modelType == whiteGhost) {
						strcpy(path, wgTextPath);
					}
					else if (modelType == blueGhost) {
						strcpy(path, bgTextPath);
					}
					else if (modelType == greenGhost) {
						strcpy(path, ggTextPath);
					}
					else if (modelType == gun) {
						strcpy(path, gunTexPath);
					}
					fscanf(modelFile, "%s\n", fn);
					strcat(path, fn);

					bool ret = loadDTextureFile(path, fr);
					if (!ret) {
						return -1;
					}

					//printf("material size: %d\n", (fr->materials).size());
				}
				else if (strcmp(line, "usemtl") == 0) {
					char fn[128];
					fscanf(modelFile, "%s\n", fn);

					for (int i = 0; i < (fr->materials).size(); i++) {
						Material currMat = (fr->materials).at(i);
						if (strcmp(currMat.mName, fn) == 0) {
							mat = currMat;
							break;
						}
					}
				}
				else if (strcmp(line, "s") == 0) {
					char isOn[5];
					fscanf(modelFile, "%s\n", isOn);

					if (strcmp(isOn, "on") == 0)
						part.s = true;
					else if (strcmp(isOn, "off") == 0)
						part.s = false;
				}
				else if (strcmp(line, "o") == 0) {
					//push previous part to model and reset counter
					if (store) {
						part.startIndex = partStartIndex;
						partStartIndex += partTotalVert;
						part.totalVert = partTotalVert;
						partTotalVert = 0;

						(fr->parts).push_back(part);

					}
					store = true;

					//new part
					part = Part();

				}
				else if (strcmp(line, "f") == 0) {

					unsigned int vIndex[3], normIndex[3];
					int isMatch = fscanf(modelFile, "%d//%d %d//%d %d//%d\n", &vIndex[0], &normIndex[0], &vIndex[1], &normIndex[1], &vIndex[2], &normIndex[2]);

					if (isMatch != 6) {
						printf("File format does not match, please do not include texture coordinate in f\n");
						return -1;
					}

					if (vIndex[0] < 1 || vIndex[1] < 1 || vIndex[2] < 1
						|| normIndex[0] < 1 || normIndex[1] < 1 || normIndex[2] < 1) {
						printf("One of the index in f is less than 1, please check again\n");
						return -1;
					}

					//add data to outModel
					for (int j = 0; j < 3; j++) {
						int vi = vIndex[j] - 1;
						int ni = normIndex[j] - 1;


						glm::vec3 addVert = tempVert.at(vi);
						glm::vec3 addNorm = tempNorm.at(ni);

						/*
						//check bounding of the model
						currModel->max_x = max(addVert.x, currModel->max_x);
						currModel->min_x = min(addVert.x, currModel->min_x);
						currModel->max_y = max(addVert.y, currModel->max_y);
						currModel->min_y = min(addVert.y, currModel->min_y);
						currModel->max_z = max(addVert.z, currModel->max_z);
						currModel->min_z = min(addVert.z, currModel->min_z);
						*/

						//0-2: index
						dOutModel->push_back(addVert.x);
						dOutModel->push_back(addVert.y);
						dOutModel->push_back(addVert.z);

						//3- 18: material
						dOutModel->push_back(mat.Ns);

						dOutModel->push_back(mat.Ka.x);
						dOutModel->push_back(mat.Ka.y);
						dOutModel->push_back(mat.Ka.z);

						dOutModel->push_back(mat.Kd.x);
						dOutModel->push_back(mat.Kd.y);
						dOutModel->push_back(mat.Kd.z);

						dOutModel->push_back(mat.Ks.x);
						dOutModel->push_back(mat.Ks.y);
						dOutModel->push_back(mat.Ks.z);

						dOutModel->push_back(mat.Ke.x);
						dOutModel->push_back(mat.Ke.y);
						dOutModel->push_back(mat.Ke.z);

						dOutModel->push_back(mat.Ni);

						dOutModel->push_back(mat.d);

						dOutModel->push_back((float)mat.illum);


						//19 - 21
						dOutModel->push_back(addNorm.x);
						dOutModel->push_back(addNorm.y);
						dOutModel->push_back(addNorm.z);
					}

					
					partTotalVert += 3;

					frTotalVert += 3;
				}

			}
			
			fr->totalVert = frTotalVert;

			fclose(modelFile);


		}
	}
	return 1;
}


bool loadDTextureFile(char pathName[], Frame* frame) {
	//printf("file name is %s\n", pathName);

	FILE* texFile = fopen(pathName, "r");
	if (texFile == NULL) {
		std::cout << "Can\'t open file " << pathName << "\n" << std::endl;
		return false;
	}

	//printf("successfully loading textFile: %s\n", pathName);

	Material m;
	bool add = false;
	while (1) {
		char texLine[128];
		int res = fscanf(texFile, "%s", texLine);

		//printf("texLine: %s\n", texLine);

		if (res == EOF) {
			if (add)
				(frame->materials).push_back(m);
			break;
		}

		else if (strcmp(texLine, "newmtl") == 0) {
			if (add) {
				(frame->materials).push_back(m);
				
			}
			add = true;



			//reset material
			m = Material();
			fscanf(texFile, "%s\n", &m.mName);

		}
		else if (strcmp(texLine, "Ns") == 0)
			fscanf(texFile, "%f\n", &m.Ns);


		else if (strcmp(texLine, "Ka") == 0) {
			glm::vec3 currKa;
			fscanf(texFile, "%f %f %f\n", &currKa.x, &currKa.y, &currKa.z);
			m.Ka = currKa;
		}

		else if (strcmp(texLine, "Kd") == 0) {
			glm::vec3 currKd;
			fscanf(texFile, "%f %f %f\n", &currKd.x, &currKd.y, &currKd.z);
			m.Kd = currKd;
		}

		else if (strcmp(texLine, "Ks") == 0) {
			glm::vec3 currKs;
			fscanf(texFile, "%f %f %f\n", &currKs.x, &currKs.y, &currKs.z);
			m.Ks = currKs;
		}

		else if (strcmp(texLine, "Ke") == 0) {
			glm::vec3 currKe;
			fscanf(texFile, "%f %f %f\n", &currKe.x, &currKe.y, &currKe.z);
			m.Ke = currKe;
		}

		else if (strcmp(texLine, "Ni") == 0) {
			fscanf(texFile, "%f\n", &m.Ni);
		}

		else if (strcmp(texLine, "d") == 0) {
			fscanf(texFile, "%f\n", &m.d);
		}
		else if (strcmp(texLine, "illum") == 0) {
			fscanf(texFile, "%d\n", &m.illum);
		}
	}
	return true;
}


bool loadTextureFile(char pathName[], Model *model) {
	//printf("file name is %s\n", path);

	FILE* texFile = fopen(pathName, "r");
	if (texFile == NULL) {
		std::cout << "Can\'t open file " << pathName << "\n" << std::endl;
		return false;
	}

	printf("successfully loading textFile: %s\n", pathName);

	Material m;
	bool add = false;
	while (1) {
		char texLine[128];
		int res = fscanf(texFile, "%s", texLine);

		//printf("texLine: %s\n", texLine);

		if (res == EOF) {
			(model->materials).push_back(m);
			break;
		}

		else if (strcmp(texLine, "newmtl") == 0) {
			if (add) {
				(model->materials).push_back(m);
				/*
				//print previous material
				printf("Material name: %s\n", m.mName);
				printf("Ns: %f\n", m.Ns);
				printf("Ka: %f, %f, %f\n", m.Ka.x, m.Ka.y, m.Ka.z);
				printf("Kd: %f, %f, %f\n", m.Kd.x, m.Kd.y, m.Kd.z);
				printf("Ks: %f, %f, %f\n", m.Ks.x, m.Ks.y, m.Ks.z);
				printf("Ke: %f, %f, %f\n", m.Ke.x, m.Ke.y, m.Ke.z);
				printf("Ni: %f\n", m.Ni);
				printf("d: %f\n", m.d);
				printf("illum: %d\n", m.illum);
				*/
			}
			add = true;

			

			//reset material
			m = Material();
			fscanf(texFile, "%s\n", &m.mName);

		}
		else if (strcmp(texLine, "Ns") == 0)
			fscanf(texFile, "%f\n", &m.Ns);

		
		else if (strcmp(texLine, "Ka") == 0) {
			glm::vec3 currKa;
			fscanf(texFile, "%f %f %f\n", &currKa.x, &currKa.y, &currKa.z);
			m.Ka = currKa;
		}

		else if (strcmp(texLine, "Kd") == 0) {
			glm::vec3 currKd;
			fscanf(texFile, "%f %f %f\n", &currKd.x, &currKd.y, &currKd.z);
			m.Kd = currKd;
		}

		else if (strcmp(texLine, "Ks") == 0) {
			glm::vec3 currKs;
			fscanf(texFile, "%f %f %f\n", &currKs.x, &currKs.y, &currKs.z);
			m.Ks = currKs;
		}

		else if (strcmp(texLine, "Ke") == 0) {
			glm::vec3 currKe;
			fscanf(texFile, "%f %f %f\n", &currKe.x, &currKe.y, &currKe.z);
			m.Ke = currKe;
		}
		
		else if (strcmp(texLine, "Ni") == 0) {
			fscanf(texFile, "%f\n", &m.Ni);
		}

		else if (strcmp(texLine, "d") == 0) {
			fscanf(texFile, "%f\n", &m.d);
		}
		else if (strcmp(texLine, "illum") == 0) {
			fscanf(texFile, "%d\n", &m.illum);
		}
	}
	return true;
}



bool parseSceneFile(std::string fileName, Scene* scene) {
	//std::string line;
	std::ifstream input(fileName.c_str());

	if (input.fail()) {
		std::cout << "Can't open file '" << fileName << "'" << std::endl;
		return false;
	}
	int width = 0, height = 0;
	input >> width >> height;
	if (width <= 0 || height <= 0) {
		std::cout << "Width and Height of the map should be greater than 0, please double check.\n";
		return false;
	}
	scene->width = width;
	scene->height = height;

	(scene->doors).clear();
	(scene->keys).clear();
	(scene->walls).clear();
	(scene->fruits).clear();
	//(scene->currentKeys).clear();

	int max_i = 0, max_j = 0;
	//char c;
	std::string c;
	for (int i = height - 1; i>= 0; i--) {
		for (int j = 0; j < width && (input>> c); j++) {
			//printf("c is: %s\n", c);
			sModelType modelType;
			glm::vec3 ra = glm::vec3(0.0, 0.0, 0.0);
			glm::vec3 td = glm::vec3(0.0, 0.0, 0.0);
			if (c[0] == 'W') {
				if (std::strcmp(c.c_str(), "W") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
				}
				else if (std::strcmp(c.c_str(), "WL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
				}
				else if (std::strcmp(c.c_str(), "WR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
				}
				else if (std::strcmp(c.c_str(), "WB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
				}
				glm::vec3 s = glm::vec3(0.5 * scaleFactor,4.0f, 0.5 * scaleFactor);
				(scene->walls).push_back(new Wall(j, i, scene->scaleFactor, ra,s));

			}
			else if (std::strcmp(c.c_str(), "S") == 0) {
				scene->start_x = j;
				scene->start_y = i;
			}
			/*
			else if (std::strcmp(c.c_str(), "G") == 0) {
				scene->goal_x = j;
				scene->goal_y = i;
			}
			*/
			else if (std::strcmp(c.c_str(), "f") == 0 || std::strcmp(c.c_str(), "F") == 0) {
				(scene->fruits).push_back(new Fruit(j, i, scene->scaleFactor));
			}
			//white ghost test
			else if (std::strcmp(c.c_str(), "wg") == 0) {
				glm::vec3 gpos = glm::vec3(-j * scaleFactor, 0, i * scaleFactor);
				printf("Out d size is %i\n", dynamicModels.size());
				printf("Out dOM size is %i \n", dOutModel.size());
				(scene->whiteGhosts).push_back(new WhiteGhost(gpos, dynamicModels.at(whiteGhost)));

				//////////////////////////////////////////// EIH
			}
			
			//blue ghost
			else if (std::strcmp(c.c_str(), "BG") == 0) {
				glm::vec3 gpos = glm::vec3(-j * scaleFactor, 0, i * scaleFactor);
				(scene->blueGhosts).push_back(new BlueGhost(gpos, dynamicModels.at(blueGhost)));
				//add!!!!!!!!
			}
			else if (std::strcmp(c.c_str(), "DH") == 0) {
				(scene->doors).push_back(new Door(j, i, ' ', scaleFactor,0));
			}
			else if (std::strcmp(c.c_str(), "DV") == 0) {
				(scene->doors).push_back(new Door(j, i, ' ', scaleFactor, 1));
			}
			else if (std::strcmp(c.c_str(), "key") == 0) {
				printf("in key before key\n");
				(scene->keys).push_back(new Key(j, i, ' ', scaleFactor));
				printf("in key after key\n");

			}
			else if (std::strcmp(c.c_str(), "Gun") == 0) {
				//add gun to scene, !!!!!rendering is under the static furniture!!!!!!!!!!!!!!!!!!!!!!!!!
				(scene->guns).push_back(new Gun(j, i, scaleFactor, dynamicModels.at(gun)));
			}
			else if (c[0] == 'b') {
				//bookcase with book
				modelType = bbookcase;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "b") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;
					type = 1;
				}
				else if (std::strcmp(c.c_str(), "bL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "bR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "bB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td,type));
			}
			else if (c[0] == 'B') {
				//bookcase only
				modelType = bookcase;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "B") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;
					type = 1;
				}
				else if (std::strcmp(c.c_str(), "BL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "BR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "BB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td,type));
			}
			else if (c[0] == 'G') {
				scene->goal_x = j;
				scene->goal_y = i;
				modelType = chest;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "G") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;
					type = 1;
				}
				else if (std::strcmp(c.c_str(), "GL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "GR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "GB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td, type));
			}
			else if (c[0] == 'T') {
				modelType = torch;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "T") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;
					type = 1;
				}
				else if (std::strcmp(c.c_str(), "TL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "TR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "TB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td,type));
			}
			else if (c[0] == 'K') {
				modelType = stand;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "K") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;
					type = 1;
				}
				else if (std::strcmp(c.c_str(), "KL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "KR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "KB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td, type));
			}
			else if (c[0] == 't') {
				modelType = table;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "t") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					//yind = i + 0.25;
					yind = (float)i;
					type = 1;
				}
				else if (std::strcmp(c.c_str(), "tL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					//xind = j - 0.25;
					xind = (float)j;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "tR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					//xind = j + 0.25;
					xind = (float)j;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "tB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					//yind = i - 0.25;
					yind = (float)i;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td, type));
			}
			else if (c[0] == 'p') {
				//bone
				modelType = pileBone;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "p") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.055;

					type = 1;
				}
				else if (std::strcmp(c.c_str(), "pL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.055;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "pR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.055;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "pB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.055;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td, type));
			}
			else if (c[0] == 'd') {
				//desk
				modelType = desk;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "d") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;

					type = 1;
				}
				else if (std::strcmp(c.c_str(), "dL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "dR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "dB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->staticFurnitures).push_back(new StaticFurniture(xind, yind, scene->scaleFactor, modelType, ra, td, type));
			}

			else if (c[0] == 'c') {
				//candle
				modelType = candle;
				float type = 0;
				float xind = 0.0, yind = 0.0;
				if (std::strcmp(c.c_str(), "c") == 0) {
					ra = glm::vec3(0.0f, myFaceFront, 0.0f);
					xind = (float)j;
					yind = i + 0.25;

					type = 1;
				}
				else if (std::strcmp(c.c_str(), "cL") == 0) {
					ra = glm::vec3(0.0f, myRotateLeft, 0.0f);
					xind = j - 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "cR") == 0) {
					ra = glm::vec3(0.0f, myRotateRight, 0.0f);
					xind = j + 0.25;
					yind = (float)i;
					type = 2;
				}
				else if (std::strcmp(c.c_str(), "cB") == 0) {
					ra = glm::vec3(0.0f, myFaceBack, 0.0f);
					xind = (float)j;
					yind = i - 0.25;
					type = 1;
				}
				td = glm::vec3(0.0f, 0.0f, 0.0f);
				(scene->candels).push_back(new Candel(xind, yind, scene->scaleFactor, modelType, ra, td, type));
			}

			
			
			max_i = i;
			max_j = j;
		}
	}
	scene->printSceneInfo();

	
	//add health bar to scene
	HealthBar* hb = new HealthBar();
	scene->healthBar = hb;
	
	if (max_i != 0 || max_j != width - 1) {
		std::cout << "Map width or height not match with the number of lines/rows read\n";
		return false;
	}
	
	input.close();
	cout << "success!!\n";
	return true;
}

// Create a NULL-terminated string by reading the provided file
static char* readShaderSource(const char* shaderFile) {
	FILE* fp;
	long length;
	char* buffer;

	// open the file containing the text of the shader code
	fp = fopen(shaderFile, "r");

	// check for errors in opening the file
	if (fp == NULL) {
		printf("can't open shader source file %s\n", shaderFile);
		return NULL;
	}

	// determine the file size
	fseek(fp, 0, SEEK_END); // move position indicator to the end of the file;
	length = ftell(fp);  // return the value of the current position

	// allocate a buffer with the indicated number of bytes, plus one
	buffer = new char[length + 1];

	// read the appropriate number of bytes from the file
	fseek(fp, 0, SEEK_SET);  // move position indicator to the start of the file
	fread(buffer, 1, length, fp); // read all of the bytes

	// append a NULL character to indicate the end of the string
	buffer[length] = '\0';

	// close the file
	fclose(fp);

	// return the string
	return buffer;
}

GLuint InitShader(const char* vShaderFileName, const char* fShaderFileName) {
	GLuint vertex_shader, fragment_shader;
	GLchar* vs_text, * fs_text;
	GLuint program;

	// check GLSL version
	printf("GLSL version: %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Create shader handlers
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read source code from shader files
	vs_text = readShaderSource(vShaderFileName);
	fs_text = readShaderSource(fShaderFileName);

	// error check
	if (vs_text == NULL) {
		printf("Failed to read from vertex shader file %s\n", vShaderFileName);
		exit(1);
	}
	else if (DEBUG_ON) {
		printf("Vertex Shader:\n=====================\n");
		printf("%s\n", vs_text);
		printf("=====================\n\n");
	}
	if (fs_text == NULL) {
		printf("Failed to read from fragent shader file %s\n", fShaderFileName);
		exit(1);
	}
	else if (DEBUG_ON) {
		printf("\nFragment Shader:\n=====================\n");
		printf("%s\n", fs_text);
		printf("=====================\n\n");
	}

	// Load Vertex Shader
	const char* vv = vs_text;
	glShaderSource(vertex_shader, 1, &vv, NULL);  //Read source
	glCompileShader(vertex_shader); // Compile shaders

	// Check for errors
	GLint  compiled;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		printf("Vertex shader failed to compile:\n");
		if (DEBUG_ON) {
			GLint logMaxSize, logLength;
			glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logMaxSize);
			printf("printing error message of %d bytes\n", logMaxSize);
			char* logMsg = new char[logMaxSize];
			glGetShaderInfoLog(vertex_shader, logMaxSize, &logLength, logMsg);
			printf("%d bytes retrieved\n", logLength);
			printf("error message: %s\n", logMsg);
			delete[] logMsg;
		}
		exit(1);
	}

	// Load Fragment Shader
	const char* ff = fs_text;
	glShaderSource(fragment_shader, 1, &ff, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);

	//Check for Errors
	if (!compiled) {
		printf("Fragment shader failed to compile\n");
		if (DEBUG_ON) {
			GLint logMaxSize, logLength;
			glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logMaxSize);
			printf("printing error message of %d bytes\n", logMaxSize);
			char* logMsg = new char[logMaxSize];
			glGetShaderInfoLog(fragment_shader, logMaxSize, &logLength, logMsg);
			printf("%d bytes retrieved\n", logLength);
			printf("error message: %s\n", logMsg);
			delete[] logMsg;
		}
		exit(1);
	}

	// Create the program
	program = glCreateProgram();

	// Attach shaders to program
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// Link and set program to use
	glLinkProgram(program);

	return program;
}

