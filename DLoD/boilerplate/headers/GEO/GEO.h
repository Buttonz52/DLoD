#ifndef GEO_H
#define GEO_H
#include "Game\Utility.h"
#include "GameEngine\Mesh.h"
#include "GameEngine\Shader.h"
#include "GameEngine\Texture.h"
#include "GEO/GEO.h"
#include "PxPhysicsAPI.h"
#include "PxPhysics.h"
#include "PxScene.h"
#include "Game\Audio.h"
#include <map>

//Game Entity Object
class GEO
{
public:
	GEO();
	virtual ~GEO();

    vector<GEO*> children;
    void removeChild(GEO*);
	
	bool hasTexture;
	bool isSkybox;
	bool isPlane;
	bool mixColour; 
	bool hasEnvMap;

	float transparency;

	//Scale
	vec3 &getScale();
	void setScale(const vec3 &scale);
	void updateScale(const vec3 &scale);

	void UseRegularShader();

	void UseShadowShader();

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
	virtual void Render(const mat4 &_view, const mat4 &_projection, const vec3 &_lightSource);
	//shader, texture, mesh
	void addShaders(const string &vertex, const string &fragment);
	virtual bool initBuffers();
	virtual bool initMesh(const string &);
	bool initTexture(const string &filename, GLuint target); //generic texture
	bool initSkybox(const vector <string> &filenames);

	//get/set
	Shader &getShader();
	void setShader(const Shader &s);
	Texture &getTexture();
	void setTexture(const Texture &texture);
	void setExposure(const float & e);
	float getExposure();
	void setReflectance(const float & r);
	float getReflectance();
	void setEnvironmentMap(const Texture & em);
	Mesh& getMesh();
	void setMesh(const Mesh &m);
	virtual void setColour(const vec3 &col);

	vec3 * getColour();

	virtual void RenderShadow(const mat4 & _view, const mat4 & _projection, const vec3 & _lightSource);

	virtual void shutdown();		//destroy shader, texture, mesh
	void playSFX(const string & name, const int & volume, const int & channel);
	Mesh mesh;

protected:
	vec3 colour;
	float exposure, reflectance;
	vec3 position;
	vec3 scale;  
    double xRotation, yRotation, zRotation;

	mat4 modelMatrix;
	physx::PxShape *shape;
	physx::PxRigidDynamic *body;
	
	Shader *currentShader;
	Shader shader;
	Shader shadowShader;
	Texture texture, environmentMap;
	vector<vec3> tangentMesh;
	Audio audio;
	map<string, Mix_Chunk*> sfxMap;	//map of sounds

};
#endif