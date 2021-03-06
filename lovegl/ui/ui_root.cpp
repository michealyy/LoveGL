﻿#include "ui_root.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "../engine.h"
#include "font_manager.h"
#include "button.h"
#include "slider.h"

using namespace std;
using namespace glm;

namespace kd
{
namespace ui
{

UIRoot::UIRoot()
{
    //按钮悬停变色用
    ui_button_normal_mat = new Material("ui_button_normal");
    ui_button_normal_mat->SetShader("unlit_pos_tex");
    ui_button_normal_mat->SetTexture("white");
    ui_button_hover_mat = new Material("ui_button_hover");
    ui_button_hover_mat->SetShader("unlit_pos_tex");
    ui_button_hover_mat->SetTexture("white");
    //选择框用
    ui_checkbox_rect_bg_mat = new Material("ui_checkbox_rect_bg");
    ui_checkbox_rect_bg_mat->SetShader("unlit_pos_tex");
    ui_checkbox_rect_bg_mat->SetTexture("white");
    ui_checkbox_rect_mat = new Material("ui_checkbox_rect");
    ui_checkbox_rect_mat->SetShader("unlit_pos_tex");
    ui_checkbox_rect_mat->SetTexture("white");
    //滑动条用
    ui_slider_bg_mat = new Material("ui_slider_bg");
    ui_slider_bg_mat->SetShader("unlit_pos_tex");
    ui_slider_bg_mat->SetTexture("white");
}

UIRoot::~UIRoot()
{
    SafeDelete(ui_button_normal_mat);
    SafeDelete(ui_button_hover_mat);
}

void UIRoot::Setup()
{
    int width, height;
    glfwGetWindowSize(Engine::GetInstance()->GetMainWindow(), &width, &height);

    //查看字体图集
    // auto font_texture = new UIRect();
    // font_texture->name = "font_texture";
    // font_texture->width = static_cast<float>(FontManager::GetInstance()->TEXTURE_WIDTH);
    // font_texture->height = static_cast<float>(FontManager::GetInstance()->TEXTURE_HEIGHT);
    // font_texture->material = FontManager::GetInstance()->GetMaterial(0);
    // AddChild(font_texture);

    //fps状态等
    fps_label_ = new Label();
    fps_label_->position = vec3(0, height - 12, 0);
    AddChild(fps_label_);
    draw_call_label_ = new Label();
    draw_call_label_->position = vec3(0, height - 28, 0);
    AddChild(draw_call_label_);
    ui_draw_call_label_ = new Label();
    ui_draw_call_label_->position = vec3(0, height - 45, 0);
    AddChild(ui_draw_call_label_);
    gl_renderer_label_ = new Label();
    gl_renderer_label_->position = vec3(0, height - 62, 0);
    AddChild(gl_renderer_label_);
    gl_version_label_ = new Label();
    gl_version_label_->position = vec3(0, height - 80, 0);
    AddChild(gl_version_label_);

    if (!show_inspector)
    {
        UIRect::Setup();
        return;
    }

    //标题材质
    auto ui_common_tile_mat = new Material("ui_common_tile");
    ui_common_tile_mat->SetShader("unlit_pos_tex");
    ui_common_tile_mat->SetTexture("white");
    //标题1
    auto title = new UIRect();
    title->material = ui_common_tile_mat;
    title->width = 130;
    title->height = 30;
    title->position = vec3(width - title->width, height - 30, 0);
    title->color = vec3(0.9, 0.133, 0.415);
    AddChild(title);
    auto title_label = new Label();
    title_label->SetText("Render"); //Inspector
    title_label->position = vec3(45, 10, 1);
    title_label->depth = 1;
    title->AddChild(title_label);
    //标题2
    auto title2 = new UIRect();
    title2->material = ui_common_tile_mat;
    title2->width = title->width;
    title2->height = title->height;
    title2->position = vec3(width - title2->width, height - 300, 0);
    title2->color = vec3(0.9, 0.133, 0.415);
    AddChild(title2);
    nodeNameLabel = new Label();
    nodeNameLabel->SetText("Selected Entity");
    nodeNameLabel->position = vec3(20, 10, 1);
    nodeNameLabel->depth = 1;
    title2->AddChild(nodeNameLabel);
    //选中物体信息
    nodePosXLabel = new Label();
    nodePosXLabel->position = vec3(20, -20, 0);
    title2->AddChild(nodePosXLabel);

    //背景
    auto ui_common_bg_mat = new Material("ui_common_bg");
    ui_common_bg_mat->SetShader("unlit_pos_tex");
    ui_common_bg_mat->SetTexture("white");
    auto bg = new UIRect();
    bg->material = ui_common_bg_mat;
    bg->width = title->width;
    bg->height = (float)height;
    bg->position = vec3(width - bg->width, 0, 0);
    bg->color = vec3(0, 0, 0);
    bg->alpha = 0.5f;
    bg->depth = -1;
    AddChild(bg);

    skyboxCheckBox = new CheckBox("SkyBox", false);
    skyboxCheckBox->position = vec3(10, height - 80, 0);
    bg->AddChild(skyboxCheckBox);

    auto bloomCheckBox = new CheckBox("Bloom", true);
    bloomCheckBox->position = vec3(10, height - 120, 0);
    bg->AddChild(bloomCheckBox);    

    auto slider = new Slider();
    slider->width = 110;
    slider->height = 25;
    slider->position = vec3(10, height - 180, 0);
    bg->AddChild(slider);
    auto exposure_label = new Label();
    exposure_label->SetText("Exposure:");
    exposure_label->position = vec3(10, height - 150, 0);
    exposure_label->depth = 1;
    bg->AddChild(exposure_label);

    //测试按钮
    auto btn_1 = new Button();
    btn_1->position = vec3(15, height - 240, 0);
    bg->AddChild(btn_1);

    UIRect::Setup();
}

void UIRoot::Update(float deltaTime)
{
    Node::Update(deltaTime);
    for (auto child : children_)
    {
        if (child->visible)
            child->Update(deltaTime);
    }

    if (show_stat_label)
    {
        fps_label_->SetText(std::string("FPS: ").append(std::to_string(Engine::GetInstance()->fps)));
        draw_call_label_->SetText(std::string("Draw call: ").append(std::to_string(Engine::GetInstance()->drawCall)));
        ui_draw_call_label_->SetText(std::string("UI Batch: ").append(std::to_string(Engine::GetInstance()->uiDrawCall)));

        gl_renderer_label_->SetText((char *)glGetString(GL_RENDERER));
        gl_version_label_->SetText(std::string("OpenGL ").append((char *) glGetString(GL_VERSION)));
    }

    if (!show_inspector)
        return;

    auto engine = Engine::GetInstance();
    auto scnMgr = engine->sceneManager;
    if (scnMgr == nullptr)
        return;
    scnMgr->showSkyBox = skyboxCheckBox->selected;

    auto selectedEntity = Engine::GetInstance()->selectedEntity;
    if (selectedEntity)
    {
        nodeNameLabel->SetText(selectedEntity->name);
        nodePosXLabel->SetText(string("PosX: ").append(to_string(selectedEntity->position.x)));
        //.append(to_string(selectedEntity->position.y)).append(" ").append(to_string(selectedEntity->position.z))
    }
    else
    {
        nodeNameLabel->SetText("Selected Entity");
        nodePosXLabel->SetText("");
    }
}

} // namespace ui

} // namespace kd
