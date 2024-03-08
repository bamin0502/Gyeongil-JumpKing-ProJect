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
    //Scene::Init();
    FRAMEWORK.SetTimeScale(1.f);
    //FRAMEWORK.GetWindow().setSize({1920, 1080}); // FHD 기준
    //FRAMEWORK.GetWindow().setView(worldView);
    background = new SpriteGo("background");
    background->SetTexture("graphics/map1.png");
    background->SetOrigin(Origins::MC);
    background->SetPosition({0.f, -700.f});
    AddGo(background,Layers::World);
    player = new Player("player");
    AddGo(player,Layers::World);
    introText = new TextGo("intro");
    introText->SetFont("fonts/Galmuri11-Bold.ttf");
    introText->SetString(str);
    introText->SetScale({1.5f, 1.5f});
    introText->SetOrigin(Origins::MC);
    introText->SetPosition({960.f, 540.f});
    AddGo(introText,Layers::Ui);
    


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
    //player->SetPosition(background->GetPosition());
    //worldView.setCenter(player->GetPosition());
    uiView.setSize(1920.f, 1080.f);
    uiView.setCenter(960.f, 540.f);
    introText->SetAlpha(255);
    fadeoutElapsedTime=0.f;
    fadeoutDuration=3.0f;
    isFadingOut=true;
    
}

void GameScene::Exit()
{
    Scene::Exit();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
    // View의 크기 설정

    sf::View worldView(sf::Vector2f(0.f, 0.f), sf::Vector2f(background->GetGlobalBounds().width, background->GetGlobalBounds().height/5));
    worldView.setSize(1980,1080);
    
    // 플레이어의 Y 좌표에 따라 View의 중심 위치를 이동
    float playerY = player->GetPosition().y;
    float viewCenterY=0.f;
    
    if(playerY < 500)
    {
        viewCenterY=250;
    }
    else if(playerY >= 500 && playerY < 1000)
    {
        viewCenterY=750;
    }
    else if(playerY >= 1000 && playerY < 1500)
    {
        viewCenterY=1250;
    }
    else if(playerY >= 1500 && playerY < 2000)
    {
        viewCenterY=1750;
    }
    else if(playerY >= 2000)
    {
        viewCenterY=2250;
    }
    
    worldView.setCenter(960.f, viewCenterY);
    worldView.zoom(2.f);
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
