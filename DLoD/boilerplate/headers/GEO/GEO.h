#ifndef GEO_H
#define GEO_H
#include "..\Game\Utility.h"
#include "..\GameEngine\Mesh.h"
#include "..\GameEngine\Shader.h"
#include "..\GameEngine\Texture.h"
#include "../GEO/GEO.h"
#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "..\Game\Audio.h"
#include <map>




//Game Entity Object
class GEO
{
public:
	GEO();
	~GEO();
	
	bool hasTexture;
	bool isSkybox;
	bool isPlane;

	double getRadius();
	string getFilename();
	void setFilename(const string &fname);

	//Scale
	vec3 &getScale();
	void setScale(const vec3 &scale);
	void updateScale(const vec3 &scale);

	//Rotation
	mat4 getRotation();
	void setRotation(const vec3 &rotation);
	void updateRotation(const vec3 &rotation);

	//Position
	vec3 &getPosition();
	void setPosition(const vec3 &pos);
	void updatePosition(const vec3 &pos);

	//Transform Matrix
	mat4 getModelMatrix();
	void setModelMatrix(mat4 m);
	void updateModelMatrix();
	virtual void Render(Shader &shader, const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);
	//shader, texture, mesh
	void addShaders(const string &vertex, const string &fragment);
	bool initBuffers();
	bool initMesh();
	bool initTexture(const string &filename, GLuint target); //generic texture
	bool initSkybox(const vector <string> &filenames);

	//get/set
	Shader &getShader();
	void setShader(const Shader &s);
	Texture &getTexture();
	void setTexture(const Texture &texture);
	Mesh& getMesh();
	void setMesh(const Mesh &m);
	void setColour(const vec3 &col);

	//PhysX
	physx::PxShape& getShape();
	physx::PxRigidDynamic* getBody();
	void setShape(physx::PxShape &s);
	void setBody(physx::PxRigidDynamic &b);

	void shutdown();		//destroy shader, texture, mesh
	void playSFX(const string &name);

protected:
	string filename;
	double radius;

	vec3 position;
	vec3 scale;  
    double xRotation, yRotation, zRotation;

	mat4 modelMatrix;
	physx::PxShape* shape;
	physx::PxRigidDynamic* body;

	Mesh mesh;
	Shader shader;
	Texture texture;

	Audio audio;
	map<string, Mix_Chunk*> sfxMap;	//map of sounds
	//Mix_Chunk* sfx = Mix_LoadWAV("sfx/carCrash.wav");

};
#endif