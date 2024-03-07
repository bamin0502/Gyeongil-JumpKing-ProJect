#include "pch.h"
#include "GameScene.h"

#include "Map.h"
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
    FRAMEWORK.SetTimeScale(1.f);
    FRAMEWORK.GetWindow().setSize({1920, 1080}); // FHD 기준
    FRAMEWORK.GetWindow().setView(worldView);
    
    background = new SpriteGo("background");
    background->SetTexture("graphics/map1.png");
    background->SetOrigin(Origins::MC);
    background->SetPosition({0.f, -1250.f});
    AddGo(background);
    
    introText = new TextGo("intro");
    introText->SetFont("fonts/Galmuri11-Bold.ttf");
    introText->SetString(str);
    introText->SetScale({1.5f, 1.5f});
    introText->SetOrigin(Origins::MC);
    introText->SetPosition({0.f, 0.f});
    AddGo(introText);
    
    player = new Player("player");
    AddGo(player);

    Map* map = new Map;
    AddGo(map);

    
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
    worldView.setCenter({ 0.f, 0.f});
    //player->SetPosition({mapTile->GetPosition().x, mapTile->GetPosition().y-100.f});
    // sf::Vector2f windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
    //
    // worldView.setSize(windowSize);
    // worldView.setCenter({ 0.f, 0.f });
    
    introText->SetAlpha(255);
    fadeoutElapsedTime=0.f;
    fadeoutDuration=3.0f;
    isFadingOut=true;
    player->Reset();
}

void GameScene::Exit()
{
    Scene::Exit();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
    //background->SetPosition(player->GetPosition());
    //페이드아웃 만들기
    if(isFadingOut)
    {
        fadeoutElapsedTime += dt;
        float alpha=255.f*(fadeoutElapsedTime/fadeoutDuration);
        introText->SetAlpha(255-alpha);

        if(fadeoutElapsedTime>=fadeoutDuration)
        {
            isFadingOut=false;
            fadeoutElapsedTime=0.f;
        }
    }

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
