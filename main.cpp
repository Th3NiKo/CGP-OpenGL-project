//GLEW GLUT AND GLM LIBRARIES
#include <GL/glew.h>
#include <GL/glut.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLUT_DISABLE_ATEXIT_HACK

#include <glm/gtx/spline.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

//NORMAL LIBRARIES
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <math.h>

//FROM CLASSES LIBRARIES
#include <Camera.h>
#include <Texture.h>
#include <Shader_Loader.h>
#include <Render_Utils.h>
#include <RigidBody.h>

//CONSTANT
#define PI 3.1415926535897932384626433832795

////////////////////////////////////////////
//              VARIABLES
////////////////////////////////////////////
Core::Shader_Loader shaderLoader;

//Camera
float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(5, 0, 25);
glm::vec3 cameraDir = {};
glm::mat4 cameraMatrix = {};
glm::mat4 perspectiveMatrix = {};

//Exercise "CAMERA MOVEMENT"

glm::vec2 mousePosition = glm::vec2(0.0f, 0.0f);

//Exercise "MOVING ON A CURVE"
float diffrence = 0;
int klatka = 0;
bool goUp = true;
int fps = 1;
float tim = 0.0f;

//Shaders
GLuint programColor;
GLuint programTexture;
GLuint programBackground;
GLuint programWater;
GLuint programSkybox;

//Light effects
glm::vec3 lightDir = glm::normalize(glm::vec3(15.0f, 0.0f, -1.0f));
glm::vec3 lightDir2 = glm::normalize(glm::vec3(0.0f, 15.0f, -1.0f));

//Models
obj::Model shipModel;
obj::Model sphere;

//Textures
GLuint shipTexture;
GLuint starsTexture;


//Exercise "Particles"
class Particle {
public:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::quat rot;

	Particle(glm::vec3 position, glm::vec3 velocity, glm::quat rotation) {
		pos = position;
		vel = velocity;
		rot = rotation;
	}
};
std::vector<Particle> miniShips;
std::vector<glm::vec3> colors;
const int numberOfShips = 50;
const int numberOfRain = 500;
float crowdValue = 0.7f;

//Rigidbodies
RigidBody * body [numberOfShips];
RigidBody * rain [numberOfRain];
float t = 0.0f;
float dt = 0.1f;

//Graph
std::vector<Particle> graph;
int resolution = 20;


//Terrain
int terrainSize = 70;
std::vector<glm::vec3> terrain;

//Mountains
std::vector<glm::vec3> mountains;

//Rain
bool doPlayRain = false;
bool pressedN = false;
bool pressedM = false;

class Tree{
public:
    glm::vec3 pos;
    int height;
    float colorGreen;

    Tree(glm::vec3 _pos, int _height){
        pos = _pos;
        height = _height;
    }
};

std::vector <Tree> trees;

//Wyklad 9 skybox
std::vector<glm::vec4> planets;
GLuint textureAsteroid;
GLuint cubemapTexture;

const float cubeVertices[] = {
	30.5f, 30.5f, 30.5f, 1.0f,
	30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,

	30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,

	30.5f, 30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,
	30.5f, -30.5f, -30.5f, 1.0f,

	30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,

	-30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,

	-30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,

	30.5f, 30.5f, 30.5f, 1.0f,
	30.5f, -30.5f, -30.5f, 1.0f,
	30.5f, -30.5f, 30.5f, 1.0f,

	30.5f, 30.5f, 30.5f, 1.0f,
	30.5f, 30.5f, -30.5f, 1.0f,
	30.5f, -30.5f, -30.5f, 1.0f,

	30.5f, 30.5f, -30.5f, 1.0f,
	30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,

	30.5f, 30.5f, -30.5f, 1.0f,
	-30.5f, 30.5f, 30.5f, 1.0f,
	-30.5f, 30.5f, -30.5f, 1.0f,

	30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
	30.5f, -30.5f, 30.5f, 1.0f,

	30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, -30.5f, 1.0f,
	-30.5f, -30.5f, 30.5f, 1.0f,
};


////////////////////////////////////////////
//              FUNCTIONS
////////////////////////////////////////////



//Graphs
glm::vec3 Ripple (float x, float z, float t) {
    glm::vec3 p;
    float d = sqrt(x * x + z * z);

    p.x = x;
    p.y = sin(PI * (4.0f * d - t));
    p.y /= 1.0f + 10.0f * d;
    p.z = z;
    return p;
}

glm::vec3 Sine2DFunction (float x, float z, float t) {
		glm::vec3 p;
		p.x = x;
		p.y = sin(PI * (x + t));
		p.y += sin(PI * (z + t));
		p.y *= 0.5f;
		p.z = z;
		return p;
	}


//Randomize
float RandomFloat(float min, float max)
{
	assert(max > min);
	float random = ((float)rand()) / (float)RAND_MAX;

	float range = max - min;
	return (random*range) + min;
}

float roll = 0.0f;
float pitch = 0.0f;
float yaw = 0.0f;




void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.2f;
	float moveSpeed = 0.5f;
	switch(key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'q': cameraPos.y += moveSpeed; break;
	case 'e': cameraPos.y -= moveSpeed; break;
	case 'p': if(doPlayRain == false){ doPlayRain = true;} else { doPlayRain = false;}; break;
	case 'n':  if(pressedN == false){ pressedN = true;} else { pressedN = false;}; break;
	case 'm':  if(pressedM == false){ pressedM = true;} else { pressedM = false;}; break;
	}

}

void mouse(int x, int y) {
	float mouseX_Sensitivity = 0.005f;
	float mouseY_Sensitivity = 0.005f;

	glm::vec2 mouse_delta = glm::vec2(x, y) - mousePosition;
	yaw += mouseX_Sensitivity * mouse_delta.x;
	pitch += mouseY_Sensitivity * mouse_delta.y;

	mousePosition = glm::vec2(x, y);
}

glm::mat4 createCameraMatrix()
{
    cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::quat RotationRoll = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat RotationPitch = glm::angleAxis(pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat RotationYaw = glm::angleAxis(yaw, glm::vec3(0.0f, 1.0f, 0.0f));


    glm::quat orientation = RotationPitch * RotationYaw;
    orientation = glm::normalize(orientation);
	glm::mat4 RotationMatrix = glm::toMat4(orientation);

    glm::mat4 cameraTranslation = glm::mat4(1.0f);
    cameraTranslation[3] = glm::vec4(-cameraPos, 1.0f);
    //cameraTranslation = glm::translate(cameraTranslation, -cameraPos);

    return RotationMatrix * cameraTranslation;
}


const int howMany = 100;
glm::vec3 points[howMany];
glm::vec3 tangents[howMany];
float angles[howMany] = {};
glm::vec3 rotateAbout[howMany] = glm::vec3(0.0f);

void init(){ //Initiliaze everything
    glEnable(GL_DEPTH_TEST);


    //Shaders
    programColor = shaderLoader.CreateProgram("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_color.vert", "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_tex.vert", "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_tex.frag");
	programBackground = shaderLoader.CreateProgram("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_tex_background.vert", "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_tex_background.frag");
    programWater = shaderLoader.CreateProgram("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_water.vert", "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_water.frag");
    programSkybox = shaderLoader.CreateProgram("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_skybox.vert", "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/shaders/shader_skybox.frag");

	//Models
    shipModel = obj::loadModelFromFile("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/models/spaceShip2.obj");
    sphere = obj::loadModelFromFile("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/models/sphere.obj");

	//Textures
    shipTexture = Core::LoadTexture("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/spaceShip2.png");
    starsTexture = Core::LoadTexture("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/stars2.png");


    //Skybox wyklad 9
    for (int i = 0; i < 10; i++)
	{
		glm::vec3 position = glm::ballRand(30.0f);
		float scale = glm::linearRand(0.5f, 5.0f);
		planets.push_back(glm::vec4(position, scale));
	}

    textureAsteroid = Core::LoadTexture("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/asteroid2.png");

    cubemapTexture = Core::setupCubeMap("C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/xpos.png",
                                        "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/xneg.png",
                                        "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/ypos.png",
                                        "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/yneg.png",
                                        "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/zpos.png",
                                        "C:/Users/Th3NiKo/Desktop/Everything/programy c++/Testing/textures/zneg.png");



    //Creating small ships
    for (int i = 0; i < numberOfShips; i++) {
		glm::vec3 tempPos = glm::vec3(RandomFloat(-4.0f, 4.0f), RandomFloat(-4.0f, 4.0f), RandomFloat(-4.0f, 4.0f));
		glm::vec3 tempVel = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat tempQuat;
		tempQuat = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
		Particle temp(tempPos, tempVel, tempQuat);
		miniShips.push_back(temp);
	}


    //Terrain
    for(int i = 0; i < 60; i++){
        for(int j = 0; j < 60; j++){
            float height = glm::perlin(glm::vec2(i*1.6f,j*1.6f) );

            glm::vec3 pos = glm::vec3(i* 1.6, height, j*1.6);
            terrain.push_back(pos);
            if(RandomFloat(0.0f, 1.0f) < 0.035f){
                Tree temp(glm::vec3(i* 1.6, height, j*1.6), rand() % 5 + 1);
                temp.colorGreen = RandomFloat(0.5f, 1.0f);
                trees.push_back(temp);
            }
        }
    }

    //Mountains
    for(int i = 30; i < 105; i++){
        for(int j = -45; j < -15; j++){
            float height = glm::perlin(glm::vec2(i*1.6f,j*1.6f )) / 1.5;
            if(height <= 0){
                height = 0.15f;
            }
            for(int c = 0; c < height * 16; c++){
                glm::vec3 pos = glm::vec3(i* 1.6, c * 1.6, j*1.6);
                mountains.push_back(pos);
            }

        }
    }

    //RIGIDBODIES
    glm::mat3 inertia = {};
    inertia[0][0] = 1.0f;
    inertia[1][1] = 0.5f;
    inertia[2][2] = 3.0f;



    for (int i = 0; i < numberOfShips; ++i) {
    // Set the initial state of the rigid bodies .
        glm::vec3 tempPos = glm::vec3(RandomFloat(-4.0f, 4.0f), RandomFloat(-4.0f, 4.0f), RandomFloat(-4.0f, 4.0f));
        glm::quat tempQuat;
        tempQuat = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
        body [i] = new RigidBody (0.1f, inertia) ;
        body[i]->ClearEverything();
        //body[i]->AddForce(glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(1,1,1));
        body[i]->AddForce(glm::vec3(0.0f,(-body[i]->m_mass) * (0.1f), 0.0f));

    }

    for(int i = 0; i < numberOfRain; i++){
        glm::vec3 tempPos = glm::vec3(RandomFloat(0.0f, 60.0f), RandomFloat(30.0f, 75.0f), RandomFloat(0.0f, 60.0f));
        glm::quat tempQuat;
        tempQuat = glm::angleAxis(roll, glm::vec3(0.0f, 0.0f, 1.0f));
        rain [i] = new RigidBody (0.1f, inertia) ;
        rain[i]->ClearEverything();
        rain[i]->m_X = tempPos;
        //body[i]->AddForce(glm::vec3(1.0f, 0.0f, 0.0f),glm::vec3(1,1,1));
        rain[i]->AddForce(glm::vec3(0.0f,-9.81f, 0.0f));
    }



    //Creating graph cubes
    float colorValue = 0.5f;
    bool flip = true;
    for(int i = 0; i < resolution * resolution; i++){
        glm::vec3 nullVector = glm::vec3(0.0f, 0.0f, 0.0f);
        Particle temp(nullVector, nullVector, nullVector);
        glm::vec3 color = glm::vec3(0.0f,0.0f,colorValue);
        colors.push_back(color);
        graph.push_back(temp);
        if(flip){
            colorValue += 0.01f;
        } else {
            colorValue -= 0.01f;
        }
        if(colorValue >= 1.0f){
            flip = !flip;
        } else if(colorValue <= 0.5f){
            flip = !flip;
        }
    }


    //HERMITE POINTS AND VECTORS
   // glm::vec4 p0 = glm::vec4(-10,10,-5,1);
    //glm::vec4 p1 = glm::vec4(20, -50, 5, 1);
    glm::vec4 p0 = glm::vec4(-40,50,-30,1);
    glm::vec4 p1 = glm::vec4(-25, 35, -25, 1);
    glm::vec4 v0 = glm::vec4(30, 0, -5, 1);
    glm::vec4 v1 = glm::vec4(30, -10, 0, 1);

    //Creating points from hermite (100)
    int licznik = 0;
    for (float t = 0.00; t < 1.00; t = t + 0.01) {
        points[licznik] = glm::hermite(p0, p1, v0, v1, t);
        licznik++;

        //Tangents vector counting
        if(licznik > 0 && licznik < 999){
            tangents[licznik] = glm::normalize(points[licznik] - points[licznik - 1]);
        }
    }



}

void shutdown() //Free memory
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
	shaderLoader.DeleteProgram(programBackground);
}

void idle() //Must be
{
	glutPostRedisplay();
}


//Drawing functions
void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);


	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectWater(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programWater;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
    glUniform1f(glGetUniformLocation(program, "time"), tim);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint IDtexture)
{
    GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);


	Core::SetActiveTexture(IDtexture, "samp", program, 0);

	Core::DrawModel(model);


	glUseProgram(0);
}

void drawSkyboxTexture( glm::mat4 modelMatrix, GLuint textureId)
{
	GLuint program = programSkybox;


	glUseProgram(program);



    glm::mat4 view = glm::mat4(glm::mat3(cameraMatrix));
	glm::mat4 transformation = perspectiveMatrix * view * modelMatrix;


	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (float*)&transformation);
    Core::SetActiveTexture(textureId, "skybox", program, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	Core::DrawVertexArray(cubeVertices, 36, 4);

	glUseProgram(0);
}



void drawBackgroundTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint IDtexture)
{

	GLuint program = programBackground;

	glUseProgram(program);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);


	Core::SetActiveTexture(IDtexture, "samp", program, 0);

	Core::DrawModel(model);


	glUseProgram(0);
}


void renderScene() //Every frame
{
    tim = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glDepthFunc ( GL_LEQUAL );
	glDepthMask(GL_FALSE);
	glm::mat4 init = glm::mat4(1.0f);
	drawSkyboxTexture(init,cubemapTexture);
	glDepthMask(GL_TRUE);


    glm::mat4 shipModelMatrix = glm::mat4(1.0f);

    shipModelMatrix = glm::translate(shipModelMatrix,points[klatka])  * glm::scale(shipModelMatrix, glm::vec3(1.0f, 0.5f, 1.0f));
    drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(1.0f, 0.2f, 0.2f));

    //Gwiazdy
    //glm::mat4 starsModelMatrix = glm::mat4(1.0f);
	//drawBackgroundTexture(&sphere, glm::translate(starsModelMatrix,glm::vec3(0, 0, 0)) * glm::scale(starsModelMatrix,glm::vec3(90.0f)), starsTexture);



	//Ground
    for(int i = 0; i < 60 * 60; i++){
        glm::mat4 Matrix = glm::mat4(1.0f);
        if(terrain.at(i).x > 30.0f && terrain.at(i).z >= 0.0f && terrain.at(i).z < 35.0f){


            //WATER SECTION

            //Diffrent types of movement
            if(terrain.at(i).x >= 75.0f){
                terrain.at(i).y = (sin(tim) / 2) - 0.8;
            } else if(terrain.at(i).x >= 60.0f){
                terrain.at(i).y = (sin(tim + terrain.at(i).x) / 2) - 0.8;
            } else if(terrain.at(i).x >= 45.0f){
                terrain.at(i).y = (sin(tim + terrain.at(i).x) / 2) + (cos(tim + terrain.at(i).z) / 2)  - 0.8;
            } else {
                terrain.at(i).y = Sine2DFunction(terrain.at(i).x,terrain.at(i).z,tim / 4).y;
            }
            //terrain.at(i).y = (sin(tim) / 2) - 0.8;
            //terrain.at(i).y = (sin(tim + terrain.at(i).x) / 2) - 0.8;
            //terrain.at(i).y = (sin(tim + terrain.at(i).x) / 2) + (cos(tim + terrain.at(i).z) / 2)  - 0.8;
            //terrain.at(i).y = Sine2DFunction(terrain.at(i).x,terrain.at(i).z,tim / 4).y;



            Matrix = glm::translate(terrain.at(i))  * glm::scale(Matrix, glm::vec3(0.8f, 0.8f, 0.8f));
            terrain.at(i).y = glm::clamp(terrain.at(i).y , 0.5f, 0.6f);
            drawObjectColor(&shipModel, Matrix, glm::vec3(0.0f, 0.0f, terrain.at(i).y  ));


        } else if(terrain.at(i).x > 25.0f && terrain.at(i).x < 75.0f && terrain.at(i).z > 35.0f && terrain.at(i).z < 55.0f){

             Matrix = glm::translate(terrain.at(i))  * glm::scale(Matrix, glm::vec3(0.8f, 0.8f, 0.8f));
            drawObjectColor(&shipModel, Matrix, glm::vec3(0.75f, glm::clamp(terrain.at(i).y, 0.5f, 0.7f), 0.41f));


        }else{
           Matrix = glm::translate(terrain.at(i))  * glm::scale(Matrix, glm::vec3(0.8f, 0.8f, 0.8f));
            drawObjectColor(&shipModel, Matrix, glm::vec3(0.0f, terrain.at(i).y + 0.6f, 0.0f));
        }

    }

    for(int i = 0; i < trees.size(); i++){
        for(int j = 0; j < trees.at(i).height; j++){
            if((terrain.at(i).x < 5.0f || terrain.at(i).x >15.0f) && (terrain.at(i).z <0.0f || terrain.at(i).z > 15.0f)){
                 if(trees.at(i).pos.x > 30.0f && trees.at(i).pos.z >= 0.0f && trees.at(i).pos.z < 35.0f){


                }  else {
                glm::mat4 Matrix = glm::mat4(1.0f);
                if(pressedN){
                    trees.at(i).pos.y += 0.05f;
                }
                if(pressedM){
                    trees.at(i).pos.y -= 0.3f;
                    if(trees.at(i).pos.y <= 0){
                        trees.at(i).pos.y = 0;
                    }
                }
                Matrix = glm::translate(glm::vec3(trees.at(i).pos.x, trees.at(i).pos.y + (j+1) * 1.6f, trees.at(i).pos.z))  * glm::scale(Matrix, glm::vec3(0.8f, 0.8f, 0.8f));
                drawObjectColor(&shipModel, Matrix, glm::vec3(0.64f, 0.16f, 0.16f));
                }
            }
        }
        //Draw leafes

        if((terrain.at(i).x < 5.0f || terrain.at(i).x >15.0f) && (terrain.at(i).z <0.0f || terrain.at(i).z > 15.0f)){
            if(trees.at(i).pos.x > 30.0f && trees.at(i).pos.z >= 0.0f && trees.at(i).pos.z < 35.0f){

            } else {
                for(int j = 1; j < 4; j++){ //3 layers up

                    for(int c = -1 ; c <= 1; c++){ //One of cube layer
                        for(int b = -1; b <= 1; b++){
                            bool draw = true;
                            if(j == 3 || j == 1){
                                if((c == -1 && b == -1) || (c == 1 && b == 1) || (c == -1 && b == 1) || (c == 1 && b == -1)){
                                    draw = false;
                                }
                            }
                            if(draw){
                                glm::mat4 Matrix = glm::mat4(1.0f);
                                Matrix = glm::translate(glm::vec3(trees.at(i).pos.x + c * 1.6f, trees.at(i).pos.y + (trees.at(i).height + j) * 1.6f, trees.at(i).pos.z + b * 1.6f))  * glm::scale(Matrix, glm::vec3(0.8f, 0.8f, 0.8f));

                                drawObjectColor(&shipModel, Matrix, glm::vec3(0.0f, trees.at(i).colorGreen, 0.0f));
                            }
                        }
                    }
                }

            }
        }
    }

    //Mountains
    float mountainHeight = 8.0f;
    for(int i = 0; i < 50 * 50; i++){
        glm::mat4 Matrix = glm::mat4(1.0f);
        Matrix = glm::translate(mountains.at(i))  * glm::scale(Matrix, glm::vec3(0.8f, 0.8f, 0.8f));
        drawObjectColor(&shipModel, Matrix, glm::vec3(0.75f, glm::clamp(mountains.at(i).y / 7, mountains.at(i).y / 8, mountains.at(i).y / 9), 0.41f));
    }


    //SMALL SHIPS
    glm::mat4 miniShipModelMatrix[numberOfShips];

    for (int i = 0; i < miniShips.size(); i++) {
        miniShipModelMatrix[i] = glm::mat4(1.0f);
        glm::vec3 v1 = points[klatka] - miniShips[i].pos; //Follow main ship

        //INIT
        glm::vec3 sumVelocity = glm::vec3(0.0f);
        glm::vec3 v2 = glm::vec3(0.0f);
        glm::vec3 v3 = glm::vec3(0.0f);

        //Determine if there is any closer ship
        for(int j = 0; j < miniShips.size(); j++){
            sumVelocity += miniShips[j].vel;
            if(glm::length(miniShips[i].pos-miniShips[j].pos) < crowdValue){
                v2 = v2 - (miniShips[i].pos - miniShips[j].pos);
            }
        }
        v3 = ((sumVelocity / (float)numberOfShips) - miniShips[i].vel);
        //Final velocity count
        miniShips[i].vel += 0.00001f * v1 + 0.0001f * v2 +  0.001f * v3;
        body[i]->AddForce(0.001f * v1);
        body[i]->AddForce(0.01f * v2);
        body[i]->AddForce(0.1f * v3);
        body[i]->AddTorque(v1);

        //Move mini ships
        miniShips[i].pos += 0.25f *  glm::normalize(miniShips[i].vel);
        miniShipModelMatrix[i] = glm::translate(miniShipModelMatrix[i],miniShips[i].pos) * glm::scale(miniShipModelMatrix[i],glm::vec3(0.1f,0.1f,0.1f));
        drawObjectColor(&shipModel, miniShipModelMatrix[i],glm::vec3(0.0f,0.0f,1.0f));
       //drawObjectTexture(&shipModel, miniShipModelMatrix[i],starsTexture);
    }


    //Rigidbodies
    // Part of the physics update step .
    for (int i = 0; i < numberOfShips; ++i) {

        body [i]->Update (t, dt);
        t += dt;

        glm::mat4 miniShipModelMatrix = glm::mat4(1.0f);
        miniShipModelMatrix = glm::translate(miniShipModelMatrix,body[i]->m_X) * glm::toMat4((glm::normalize(body[i]->m_Q))) * glm::scale(miniShipModelMatrix,glm::vec3(0.4f,0.4f,0.4f));
        drawObjectColor(&shipModel, miniShipModelMatrix,glm::vec3(0.0f,1.0f,0.0f));
    }

    //RAIN
    if(doPlayRain){
        for(int i = 0;i < numberOfRain; ++i){
            rain [i]->Update (t, dt);
            t += dt;

             glm::mat4 miniShipModelMatrix = glm::mat4(1.0f);
            miniShipModelMatrix = glm::translate(miniShipModelMatrix,rain[i]->m_X) * glm::toMat4((glm::normalize(rain[i]->m_Q))) * glm::scale(miniShipModelMatrix,glm::vec3(0.03f,0.08f,0.03f));
            drawObjectColor(&shipModel, miniShipModelMatrix,glm::vec3(0.0f,0.0f,1.0f));
            if(rain[i]->m_X.y <= 0.0f){
                glm::vec3 tempPos = glm::vec3(RandomFloat(0.0f, 60.0f), RandomFloat(30.0f, 75.0f), RandomFloat(0.0f, 60.0f));
                rain[i]->m_X = tempPos;
            }
        }
    }





    //For hermite moving to and back
    if (klatka >= 99 && goUp == true) {
        goUp = false;
    } else if (klatka <= 0 && goUp == false) {
        goUp = true;
    }
    if (goUp) {
        klatka++;
    } else {
        klatka--;
    }



    //Graph drawing "Fountain"
    /*
    float step = 2.0f / resolution;
    for (int i = 0, z = 0; z < resolution; z++) {
        float v = (z + 0.5f) * step - 1.0f;
        for (int x = 0; x < resolution; x++, i++) {
            float u = (x + 0.5f) * step - 1.0f;
            graph.at(i).pos = Ripple(u, v, tim);
            glm::mat4 matrix = glm::mat4(1.0f);
            graph.at(i).pos.y += 1.4f;
            graph.at(i).pos.x += 25.0f;
            graph.at(i).pos.z += 25.0f;
            matrix = glm::translate(graph.at(i).pos) * glm::scale(glm::vec3(0.16f, 0.16f, 0.16f));
            drawObjectColor(&shipModel, matrix,glm::vec3(colors.at(i)));
        }
    }*/

    //Wyklad 9 skybox
/*
    for (int i = 0; i < planets.size(); i++)
	{
		glm::mat4 planetModelMatrix = glm::translate(glm::vec3(planets[i])) * glm::scale(glm::vec3(planets[i].w));
		drawObjectTexture(&sphere, planetModelMatrix, textureAsteroid);
	}*/




    glutSwapBuffers();
}


////////////////////////////////////////////
//                MAIN
////////////////////////////////////////////

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(75, 50);
	glutInitWindowSize(1210, 650);
	glutCreateWindow("Computer Games Programming");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouse);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
