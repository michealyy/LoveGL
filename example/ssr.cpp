#include <lovegl.h>

using namespace kd;
using namespace glm;

class AppSSR : public kd::App
{
  public:
    void Setup() override
    {
        scnMgr = new SceneManager();
        auto engine = Engine::GetInstance();
        engine->sceneManager = scnMgr;
        engine->exposure = 1.f;

        scnMgr->LoadGLTF("assets/scenes/ssr/ssr.gltf");
        scnMgr->LoadIBL("night");

        //主摄像机
        auto camera = scnMgr->CreateNode<Camera>();
        camera->name = "MainCamera";
        camera->AttachPostProcessing(new Bloom());
        //camera->AttachPostProcessing(new ToneMapping());
        camera->AttachController(new FreeCameraController());
        camera->position = vec3(6, 7.3, 2.8);
        camera->eulerAngles = vec3(-37, 67, 0);
        Engine::GetInstance()->mainCamera = camera;
    }

    void Update(float deltaTime) override
    {
    }

  private:
    kd::SceneManager *scnMgr = nullptr;
};

int main(int, char **)
{
    kd::Main main;
    main.InitWindow(1200, 900)->SetApp(new AppSSR());
    main.MainLoop();
    return 0;
}