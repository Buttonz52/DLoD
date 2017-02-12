#include "GameEngine/Shader.h"

// Functions to set up OpenGL shader programs for rendering


// load, compile, and link shaders, returning true if successful
GLuint Shader::InitializeShaders(const string &vert, const string &frag)
{
	// load shader source from files
	string vertexSource = LoadSource(vert);
	string fragmentSource = LoadSource(frag);
	if (vertexSource.empty() || fragmentSource.empty()) return false;

	// compile shader source into shader objects
	vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
	fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// link shader program
	program = LinkProgram(vertex, fragment);

	// check for OpenGL errors and return false if error occurred
	if (CheckGLErrors()) {
		cout << "ERROR INITIALIZING SHADERS." << endl;
	}
	return program;
}

// deallocate shader-related objects
void Shader::DestroyShaders()
{
	// unbind any shader programs and destroy shader objects
	glUseProgram(0);
	glDeleteProgram(program);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a string
string Shader::LoadSource(const string &filename)
{
	string source;

	ifstream input(filename.c_str());
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint Shader::CompileShader(GLenum shaderType, const string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
	}

	return shaderObject;
}


// creates and returns a program object linked from vertex and fragment shaders
GLuint Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
	}

	return programObject;
}