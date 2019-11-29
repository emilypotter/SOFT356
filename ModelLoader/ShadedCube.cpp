//////////////////////////////////////////////////////////////////////////////
//
//  SHaded Cube.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GML
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

enum VAO_IDs { Cube, NumVAOs = 1 };
enum Buffer_IDs { Triangles, Colours, Normals, Textures, Indices, NumBuffers = 5 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint texture1;

GLuint shader;

//----------------------------------------------------------------------------
//
// init
//
#define BUFFER_OFFSET(a) ((void*)(a))


void
init(void)
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Cube]);

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	shader = LoadShaders(shaders);
	glUseProgram(shader);
	
	//
    // configuring lighting
    //

    // ambient light
	glm::vec4 ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	//adding the Uniform to the shader
	GLuint aLoc = glGetUniformLocation(shader, "ambient");
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	// light object
	glm::vec3 lightPos = glm::vec3(100.0f, 25.0f, 100.0f);
	GLuint dLightPosLoc = glGetUniformLocation(shader, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));


	// diffuse light
	glm::vec3 diffuseLight = glm::vec3(0.5f, 0.2f, 0.7f);
	GLuint dLightLoc = glGetUniformLocation(shader, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));
	
	// specular light
	glm::vec3 specularLight = glm::vec3(0.7f);
	GLfloat shininess = 256; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(shader, "sLight");
	GLuint sShineLoc = glGetUniformLocation(shader, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);


	// setting up the cube

	//GLfloat vertices[] = {
	//-0.5f, -0.5f, -0.5f, //0 b l
	// 0.5f, -0.5f, -0.5f, //1 b r
	// 0.5f,  0.5f, -0.5f, //2 t r
	// 0.5f,  0.5f, -0.5f, //3 t r
	//-0.5f,  0.5f, -0.5f, //4 t l 
	//-0.5f, -0.5f, -0.5f, //5 b l

	//-0.5f, -0.5f,  0.5f, //6 b l
	// 0.5f, -0.5f,  0.5f, //7 b r
	// 0.5f,  0.5f,  0.5f, //8 t r
	// 0.5f,  0.5f,  0.5f, //9 t r 
	//-0.5f,  0.5f,  0.5f, //10 t l
	//-0.5f, -0.5f,  0.5f, //11 b l

	//-0.5f,  0.5f,  0.5f,
	//-0.5f,  0.5f, -0.5f,
	//-0.5f, -0.5f, -0.5f,
	//-0.5f, -0.5f, -0.5f,
	//-0.5f, -0.5f,  0.5f,
	//-0.5f,  0.5f,  0.5f,

	// 0.5f,  0.5f,  0.5f,
	// 0.5f,  0.5f, -0.5f,
	// 0.5f, -0.5f, -0.5f,
	// 0.5f, -0.5f, -0.5f,
	// 0.5f, -0.5f,  0.5f,
	// 0.5f,  0.5f,  0.5f,

	//-0.5f, -0.5f, -0.5f, // b l b
	// 0.5f, -0.5f, -0.5f, // b r b
	// 0.5f, -0.5f,  0.5f, // b r f
	// 0.5f, -0.5f,  0.5f, // b r b
	//-0.5f, -0.5f,  0.5f, // b l f
	//-0.5f, -0.5f, -0.5f, // b l b

	//-0.5f,  0.5f, -0.5f,
	// 0.5f,  0.5f, -0.5f,
	// 0.5f,  0.5f,  0.5f,
	// 0.5f,  0.5f,  0.5f,
	//-0.5f,  0.5f,  0.5f,
	//-0.5f,  0.5f, -0.5f,
	//};

	/*GLfloat normals[]{
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,
	0.0f,  0.0f, -1.0f,

	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,
	0.0f,  0.0f, 1.0f,

	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,
	1.0f,  0.0f,  0.0f,

	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,
	0.0f, -1.0f,  0.0f,

	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f,
	0.0f,  1.0f,  0.0f};*/

	//GLuint indices[][3] = {  // note that we start from 0!
	//	{0, 1, 2},  // first Triangle front
	//	{3, 4, 5},   // second Triangle
	//	
	//	{8, 7, 6 },
	//	{11, 10, 9 },
	//	
	//	{14, 13, 12 },
	//	{17, 16, 15 },
	//	
	//	{18, 19, 20 },
	//	{21, 22, 23 },
	//	
	//	{26, 25, 24 },
	//	{29, 28, 27 },
	//	
	//	{30, 31, 32 },  // first Triangle back
	//	{33, 34, 35 }   // second Triangle
	//};

	GLfloat  colours[][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },
	    { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },
	    
		{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
		
		{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
		
		{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f },
		
		{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f },
		
		{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f },
		
	};
	/*GLfloat  texture_coords[] = {
		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		1.0f,0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};*/

	// file load and parse

	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_textures;
	vector<glm::vec3> temp_normals;
	vector<int> vertex_indices, normal_indices, uv_indices, indices;

	vector<glm::vec3> vertices;
	vector<glm::vec2> textures;
	vector<glm::vec3> normals;

	std::string line;
	ifstream myFile("media/Creeper.obj");

	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			vector<std::string> strings;

			//split string by spaces
			std::string delimiter = " ";

			size_t pos = 0;
			std::string token;
			while ((pos = line.find(delimiter)) != std::string::npos) { // there is a string
				token = line.substr(0, pos);
				line.erase(0, pos + delimiter.length());
				strings.push_back(token);
			}
			strings.push_back(line); // push last substring in line

			if (strings[0] == "v") //first sub-string starts with v
			{
				//convert sub-strings 1-3 to glfloat
				float x = std::stof(strings[1]);
				float y = std::stof(strings[2]);
				float z = std::stof(strings[3]);

				//feed those into the vector vertices
				glm::vec3 vertex;
				vertex.x = x;
				vertex.y = y;
				vertex.z = z;

				temp_vertices.push_back(vertex);
			}
			else if (strings[0] == "vt") //first sub-string starts with vt
			{
				//convert sub-strings 1-3 to glfloat
				float x = std::stof(strings[1]);
				float y = std::stof(strings[2]);

				//feed those into the vector vertices
				glm::vec2 texture;
				texture.x = x;
				texture.y = y;

				temp_textures.push_back(texture);
			}
			else if (strings[0] == "vn") //first sub-string starts with vn
			{
				//convert sub-strings 1-3 to glfloat
				float x = std::stof(strings[1]);
				float y = std::stof(strings[2]);
				float z = std::stof(strings[3]);

				//feed those into the vector vertices
				glm::vec3 normal;
				normal.x = x;
				normal.y = y;
				normal.z = z;

				temp_normals.push_back(normal);
			}
			else if (strings[0] == "f") { // faces if substring 0 is f {
				if (strings.size() == 5) { // quad

					// initialise temp quads vector
					vector<unsigned int> vertex_indices_quads;

					// for each string in strings split on slash
					for (int i = 1; i < strings.size(); i++) { // ignore f(i=0)
						string face = strings[i];
						vector<std::string> face_strings;

						//split string by slashes
						std::string delimiter = "/";

						size_t pos = 0;
						std::string token;
						while ((pos = face.find(delimiter)) != std::string::npos) { // there is a string
							token = face.substr(0, pos);
							face.erase(0, pos + delimiter.length());
							face_strings.push_back(token);
						}
						face_strings.push_back(face); // push last substring in face

						// convert to int (MAYBE GLFLOAT???)
						float vertex = std::stoi(face_strings[0]);
						float uv = std::stoi(face_strings[1]);
						float normal = std::stoi(face_strings[2]);

						// put into quads array(THINK DO THIS FOR ALL???)
						vertex_indices_quads.push_back(vertex);
						uv_indices.push_back(uv);
						normal_indices.push_back(normal);
					}

					// convert to triangles and put into vertex_indices
					int one = vertex_indices_quads[0];
					int two = vertex_indices_quads[1];
					int three = vertex_indices_quads[2];
					int four = vertex_indices_quads[3];

					vertex_indices.push_back(one);
					vertex_indices.push_back(two);
					vertex_indices.push_back(three);

					vertex_indices.push_back(three);
					vertex_indices.push_back(four);
					vertex_indices.push_back(one);

					// put all indices into single vec3 and index
					for (unsigned int i = 0; i < vertex_indices.size(); i++) {
						unsigned int vertexIndex = vertex_indices[i];
						glm::vec3 vertex = temp_vertices[vertexIndex - 1];
						vertices.push_back(vertex);
						indices.push_back(i);
					}
					for (unsigned int i = 0; i < uv_indices.size(); i++) {
						unsigned int uvIndex = uv_indices[i];
						glm::vec2 uv = temp_textures[uvIndex - 1];
						textures.push_back(uv);
					}
					for (unsigned int i = 0; i < normal_indices.size(); i++) {
						unsigned int normalIndex = normal_indices[i];
						glm::vec3 normal = temp_normals[normalIndex - 1];
						normals.push_back(normal);
					}
				}
			}
			//{
			//	//if there are 5 substrings it means we have a quad
			//	//else if there are 4 substrings we have a triangle
			//	// else ignore and give warning
			// std::cout << line << "\n";
		}


	}
	//
	//		//vector<GLuint> fix_indices;
	//		//vector<GLuint> fix_vertices; //will be length 24 for creeper instead of 8
	//
	//		////for (length of vector indices)
	//		//if index > length of vertices
	//
	//
	//}

	glGenBuffers(NumBuffers, Buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	

	glVertexAttribPointer(Triangles, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	
	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);

	glVertexAttribPointer(Colours, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));


	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferStorage(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], 0);


	glVertexAttribPointer(Normals, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Textures]);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(Textures, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	// load and create a texture 
	// -------------------------
	
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("media/textures/Texture.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glUniform1i(glGetUniformLocation(shader, "texture1"), 0);


	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mv = view * model;


	//adding the Uniform to the shader
	int mvLoc = glGetUniformLocation(shader, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	//adding the Uniform to the shader
	int pLoc = glGetUniformLocation(shader, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));





	glEnableVertexAttribArray(Triangles);
	glEnableVertexAttribArray(Colours); 
	glEnableVertexAttribArray(Textures);
	glEnableVertexAttribArray(Normals);
}


//----------------------------------------------------------------------------
//
// display
//

void
display(GLfloat delta)
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind textures on corresponding texture units
	// glFrontFace(GL_CW);
	// glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(delta * 5.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // GET RID OF * 0.5f AND SEE IF WORKS BECAUSE COPIED


	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mv = view * model;


	//adding the Uniform to the shader
	int mvLoc = glGetUniformLocation(shader, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	//adding the Uniform to the shader
	int pLoc = glGetUniformLocation(shader, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));




	glBindVertexArray(VAOs[Cube]);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	
}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Model Loader", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	init();
	GLfloat timer= 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe 
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		display(timer);
		glfwSwapBuffers(window);
		glfwPollEvents();
		timer += 0.1f;
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
