#include <lovegl.h>
#include <GL/glew.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
using namespace kd;

class RayTracingFinalPass : public PostProcessing
{
  public:
    RayTracingFinalPass(unsigned tex)
    {
        texture = tex;
        //shader = ResourceManager::GetInstance()->GetShader("tone_mapping");
        shader = ResourceManager::GetInstance()->GetShader("image");
    }
    void Draw() override
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        shader->Bind();
        shader->SetFloat("exposure", Engine::GetInstance()->exposure);
        shader->SetFloat("gamma", Engine::GetInstance()->gamma);
        shader->SetInt("image", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

  private:
};

class AppRayTracing : public kd::App
{
public:
    int numGroupsX = 32;
    int numGroupsY = 32;
    static const unsigned Width = 1200;
    static const unsigned Height = 900;

    void Setup() override
    {
        Engine::GetInstance()->uiRoot->show_inspector = false;

        shader = new ComputeShader("raytracing", "assets/raytracing/raytracing.glsl");
        auto shaderId = shader->GetGLProgramId();
        glUseProgram(shaderId);
        int workGroupSize[3];
        glGetProgramiv(shaderId, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
        numGroupsX = (int)ceil((float)Width / workGroupSize[0]);
        numGroupsY = (int)ceil((float)Height / workGroupSize[1]);

        unsigned tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, Width, Height);
        glBindTexture(GL_TEXTURE_2D, 0);

        finalPass = new RayTracingFinalPass(tex);
        camera = new Camera();
        camera->Setup();
        camera->position = vec3(0, 0, 5);
        camera->eulerAngles = vec3(-12, 20, 0);
        camera->AttachController(new FreeCameraController());
    }

    void Update(float deltaTime) override
    {
        camera->Update(deltaTime);
        shader->Bind();

        //最终输出结果
        auto renderTargetImage = shader->GetImageUnit("RenderTarget");
        glBindImageTexture(renderTargetImage, finalPass->texture, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

        // vec3 cameraPosition = vec3(0.0f, 0.0f, 5.0f);
        // auto viewMat = lookAt(cameraPosition, vec3(-3.0f, -2.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        // auto projectMat = perspective(radians(60.f), (float)Width / Height, 0.1f, 1000.f);
        // shader->SetVector3("cameraPosition", cameraPosition);
        // shader->SetMatrix("unProject", inverse(projectMat * viewMat));
        shader->SetVector3("cameraPosition", camera->position);
        shader->SetMatrix("unProject", camera->GetUnProjectMatrix());

        //执行CS。每个像素有一个计算单元，发射一条射线计算
        glDispatchCompute(numGroupsX, numGroupsY, 1);

        //等待写入FBO完毕
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glBindImageTexture(renderTargetImage, 0, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glUseProgram(0);
        finalPass->Draw();
    }

private:
    ComputeShader *shader = nullptr;
    RayTracingFinalPass *finalPass = nullptr;
    Camera *camera = nullptr;
};

int main(int, char **)
{
    kd::Main main;
    main.InitWindow(AppRayTracing::Width, AppRayTracing::Height)->SetApp(new AppRayTracing());
    main.MainLoop();
    return 0;
}