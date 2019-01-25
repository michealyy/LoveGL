#pragma once

#include "common.h"
#include "entity_mesh.h"

namespace kd
{

class Box : public Mesh
{
  public:
    explicit Box(Material *mat, float width, float height, float length);
    virtual ~Box();

    virtual void AddVertices() override;
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;
    glm::vec3 color{1};

  private:
    DISALLOW_COPY_AND_ASSIGN(Box)
};

class Cone : public Mesh
{
  public:
    /*
    *@param segments must > 8
    */
    explicit Cone(Material *mat, float radius = 0.5f, int segments = 12, int height = 1);
    virtual ~Cone();

    virtual void AddVertices() override;
    float radius = 0.f;
    int segments = 0;
    int height = 0;
    glm::vec3 color{1};

  private:
    DISALLOW_COPY_AND_ASSIGN(Cone)
};

class Cylinder : public Mesh
{
  public:
    /*
    *@param segments must > 8
    */
    explicit Cylinder(Material *mat, float radius = 0.5f, int segments = 8, int height = 1);
    virtual ~Cylinder();

    virtual void AddVertices() override;
    float radius = 0.f;
    int segments = 0;
    int height = 0;
    glm::vec3 color{1};

  private:
    void GenerateCap(bool isTop, int startIndex);
    DISALLOW_COPY_AND_ASSIGN(Cylinder)
};

} // namespace kd