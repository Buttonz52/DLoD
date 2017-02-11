#ifndef GEO_H
#define GEO_H
#include "..\Game\Utility.h"
#include "..\GameEngine\Mesh.h"
#include "..\GameEngine\Shader.h"
#include "..\GameEngine\Texture.h"
<<<<<<< HEAD
=======

>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043
//Game Entity Object
class GEO
{
public:
	GEO();
	~GEO();
	
	void setScale(const vec3 &scale);
	vec3 &getScale();
	void updateScale(const vec3 &scale);
	void setRotation(const vec3 &rotation);
	void updateRotation(const vec3 &rotation);
	vec3 &getPosition();
	void setPosition(const vec3 &pos);
	void updatePosition(const vec3 &pos);
	double GetRadius();
	void setFilename(const string &fname);
	string getFilename();
	void addShaders(const string &vertex, const string &fragment);
	void setMesh(const Mesh &m);
	void setColour(const vec3 &col);
	void setTexture(const Texture &texture);
	Texture &getTexture();
	
	Mesh& getMesh();
	Shader &getShader();
	void setShader(const Shader &s);
	bool initMesh();
	bool initBuffers();
	bool initTexture(const string &filename, GLuint target);
	bool initSkybox(const vector <string> &filenames);
	void shutdown();		//destroy shader, texture, mesh
	bool hasTexture;
	bool isSkybox;
	bool isPlane;

<<<<<<< HEAD
	vec3& getPosition();
	void setPosition(vec3 &pos);
	double getRadius();
	void setFilename(const string &fname);
	string& getFilename();
	void addMeshShader();
	Mesh& getMesh();
	Shader getShader();

	void shutdown();		//destroy shader, texture, mesh
=======
    mat4 getRotation();

	mat4 getModelMatrix();
	void setModelMatrix(mat4 m);
	void updateModelMatrix();

	//getShape()
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043

private:
	//PxShape* shape;
	//PxRigidDynamic* body;
	mat4 modelMatrix;
	vec3 scale;  
    double xRotation, yRotation, zRotation;
	vec3 position;
	double radius;
	string filename;
	Mesh mesh;
	Shader shader;
	Texture texture;
<<<<<<< HEAD
=======
	string audioFile;
>>>>>>> 7b9af403ce13b74095ac0b5e6d81f3d635f15043
};
#endif