#include "pch.h"
#include "TitleScene.h"
#include "SpriteGo.h"


TitleScene::TitleScene(SceneIds id):Scene(id)
{
}

void TitleScene::Init()
{
    Scene::Init();

    backgroundImage = new SpriteGo("TitleBackground");
    backgroundImage->SetTexture("graphics/title.png");
    backgroundImage->SetOrigin(Origins::MC);
    AddGo(backgroundImage);
    
    
    for (GameObject* obj : gameObjects)
    {
        obj->Init();
    }
}

void TitleScene::Release()
{
    Scene::Release();
}

void TitleScene::Enter()
{
    Scene::Enter();
    
}

void TitleScene::Exit()
{
    Scene::Exit();
}

void TitleScene::Update(float dt)
{
    Scene::Update(dt);

    if(InputMgr::GetKeyDown(sf::Keyboard::Enter))
    {
        SceneMgr::Instance().ChangeScene(SceneIds::Game);
    }
}

void TitleScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
}

void TitleScene::FixedUpdate(float dt)
{
    Scene::FixedUpdate(dt);
}

void TitleScene::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
    
}
