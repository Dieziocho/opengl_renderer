#include "assimp.h"
#include "draw.h"
#include "model_instance.h"
#include "shaders.h"

Model helios_model;
ModelInstance helios;

void setups(){
  helios_model = Assimp::loadModel("resources/Helios/helios.fbx");
  helios = ModelInstance(helios_model);
  helios.playAnimation("Hi");
}

void onFrame(){
  helios.update();
  drawModel(helios_model, Shaders::animated_3d, helios.getTransform(), {},
            {{helios.getBonesTransforms(), 0}}
            );
}
