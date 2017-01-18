#pragma once

//#pragma comment(lib, \"assimp.lib\") 

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla
#include "headers/Utility.h"
#include "headers/boilerplate.h"

class CAssimpModel
{
public:
	bool LoadModelFromFile(char* sFilePath);
	bool LoadModelFromFileBumpMap(char* sFilePath, char* sColorMap, char* sNormalMap);

	static void FinalizeVBO();
	static void BindModelsVAO();

	void RenderModel();
	void RenderModelBumpMap(CShaderProgram* spProgram);

	CAssimpModel();
private:
	bool bLoaded;
	static CVertexBufferObject vboModelData;
	static UINT uiVAO;
	static vector<MyTexture> tTextures;
	vector<int> iMeshStartIndices;
	vector<int> iMeshSizes;
	vector<int> iMaterialIndices;
	int iNumMaterials;
};

};

