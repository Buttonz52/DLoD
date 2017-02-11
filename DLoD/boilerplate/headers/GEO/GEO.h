#ifndef GEO_H
#define GEO_H
#include "..\Game\Utility.h"
#include "..\GameEngine\Mesh.h"
#include "..\GameEngine\Shader.h"
#include "..\GameEngine\Texture.h"
#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "../GEO/GEO.h"

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
	double getRadius();
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

    mat4 getRotation();

	mat4 getModelMatrix();
	void setModelMatrix(mat4 m);
	void updateModelMatrix();

	physx::PxShape& getShape();
	physx::PxRigidDynamic& getBody();
	void setShape(physx::PxShape &s);
	void setBody(physx::PxRigidDynamic &b);

private:
	physx::PxShape* shape;
	physx::PxRigidDynamic* body;
	mat4 modelMatrix;
	vec3 scale;  
    double xRotation, yRotation, zRotation;
	vec3 position;
	double radius;
	string filename;
	Mesh mesh;
	Shader shader;
	Texture texture;
	string audioFile;
};
#endif