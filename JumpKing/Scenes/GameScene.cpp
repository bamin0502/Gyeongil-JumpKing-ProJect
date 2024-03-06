#include "pch.h"
#include "GameScene.h"
#include "rapidcsv.h"
#include "Player.h"

GameScene::GameScene(SceneIds id):Scene(id)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
    Scene::Init();
    AddGo(new Player());
    

    introText = new TextGo("intro");
    introText->SetFont("fonts/Galmuri11-Bold.ttf");
    introText->SetString(str);
    introText->SetScale({1.5f, 1.5f});
    introText->SetOrigin(Origins::MC);
    introText->SetPosition({0.f, 0.f});
    AddGo(introText);

    for (GameObject* obj : gameObjects)
    {
        obj->Init();
    }
}

void GameScene::Release()
{
    Scene::Release();
}

void GameScene::Enter()
{
    Scene::Enter();



    
}

void GameScene::Exit()
{
    Scene::Exit();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);

    if(InputMgr::GetKeyDown(sf::Keyboard::Escape))
    {
        //여기다가 일시정지 창을 나오게 하고 일단 게임을 멈추게 하자
        
    }
}

void GameScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
}

void GameScene::FixedUpdate(float dt)
{
    Scene::FixedUpdate(dt);
}

void GameScene::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
    
}
