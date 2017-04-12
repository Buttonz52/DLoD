#include "GEO\item\ItemSpawner.h"

vector<string> itempickModels =
{
  "/ObjModels/healthPack.obj",
  "/ObjModels/armourPack.obj"
};

ItemSpawner::ItemSpawner(vector<int> types, vec3 pos)
{
  position = pos;
  itemTypes = types;

  spawnTime = (15 + rand() % 5) * 1000;
}


ItemSpawner::~ItemSpawner()
{
}

void ItemSpawner::spawnItem(GEO * skybox)
{
  timer.stop();
  timer.reset();

  int i = rand() % itemTypes.size();
  item = new Item(ItemType(itemTypes[i]), this);

  mat4 m = mat4(1);
  m[3] = vec4(position, 1);
  item->setModelMatrix(m);

  item->setScale(vec3(2));
  item->setColour(vec3(0, 1, 0));

  if (!item->initMesh(itempickModels[i]))
    cout << "Failed to initialize item mesh. " << endl;

  item->addShaders("shaders/toon.vert", "shaders/toon.frag");

  mat3 scaleM = mat3(1);
  scaleM[0][0] = item->getScale().x;
  scaleM[1][1] = item->getScale().y;
  scaleM[2][2] = item->getScale().z;

  for (int i = 0; i < item->getMesh().vertices.size(); ++i)
    item->getMesh().vertices[i] = scaleM * item->getMesh().vertices[i];

  if (!item->initBuffers()) 
    cout << "Could not initialize buffers for initialized item." << endl;
  
  item->setEnvironmentMap(skybox->getTexture());
  skybox->children.push_back(item);
}

