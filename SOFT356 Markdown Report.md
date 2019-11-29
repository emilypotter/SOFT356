# SOFT356 Markdown Report
## Project Aims
In this project, the aim was to create a model loader from scratch that is able to render 3D objects from a given model file.
## Code
I have used Visual Studio 2019 and OpenGL 4.5.

First, the shaders are initialised and loaded. The model loader then works by first reading in a file. My parser then splits each line by spaces and checks what the first element is. If the first element is a 'v', the following three numbers must be vertices so these are put into a temp_vertices vector. The same is true if the line starts with a 'vn', except the following three numbers are normals so will go into the temp_normals vector. If the line starts with a 'vt', the following two numbers are textures so these go into the temp_uvs vector. Lines that begin with an 'f' are first split into individual faces by the space between them and then split further on the slash that separates the three numbers. If there are four faces on the line, this means it is a quad and needs to be split into two triangles. Once this is done the vertex indices, uv indices and texture indices are all put into their respective vec2/vec3 vectors.

OpenGL then generates the buffers for the indices, textures, normals and colours and binds them. The texture is then loaded from the given file and OpenGL wraps it. It then creates the matrices and finally the display method binds the textures and draws the elements to the screen.

