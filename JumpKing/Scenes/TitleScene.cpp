#include "pch.h"
#include "TitleScene.h"

TitleScene::TitleScene(SceneIds id):Scene(id)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
    title = new SpriteGo("title");
    title->SetTexture("graphics/Title.png");

    title->SetOrigin(Origins::MC);
    AddGo(title,Ui);
    
    Scene::Init();
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

    if(InputMgr::GetKeyDown(sf::Keyboard::Space))
    {
        SceneMgr::Instance().ChangeScene(SceneIds::GameScene);
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
