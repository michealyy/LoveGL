#pragma once

#include <vector>
#include <tiny_gltf.h>
#include <common.h>
#include <core/material.h>
#include <core/vertex_attribute.h>
#include <physics/ray.h>

namespace kd
{

struct MeshData
{
  std::vector<va::P_T_N_T_B> vertices;
  std::vector<unsigned> indices;
};

class SubMesh;

class Mesh : public Node
{
public:
  explicit Mesh();
  virtual ~Mesh();
  /*
  * 优先由算法生成基础集合体：Box Plane Cone
  * 否则尝试作为文件路径读取obj
  */
  void SetMesh(const std::string &name);
  virtual bool Raycast(Ray ray, RayCastHit &rayCastHit);
  void AddSubMesh(SubMesh *subMesh);

  std::vector<SubMesh *> subMeshes;

private:
  DISALLOW_COPY_AND_ASSIGN(Mesh)
};

/*
* 1.存放网格数据
* 2.绑定GL的vao
* 3.管理对应材质
*/
class SubMesh
{
public:
  explicit SubMesh();
  virtual ~SubMesh();
  void Setup();
  //void SetupFromAlgorithm();
  //void SetupFromObj();
  void SetupFromGLTF(tinygltf::Model &model, tinygltf::Primitive &primitive);
  void Draw();
  bool Raycast(Ray ray, RayCastHit &rayCastHit);

  Mesh *mesh = nullptr;
  Material *material = nullptr;
  unsigned vao = 0;
  std::vector<va::P_T_N_T_B> vertices;
  std::vector<unsigned short> indices;

private:
  unsigned vbo_ = 0;
  unsigned ebo_ = 0;

  DISALLOW_COPY_AND_ASSIGN(SubMesh)
};

} // namespace kd