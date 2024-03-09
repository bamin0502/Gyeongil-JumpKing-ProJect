#include "pch.h"
#include "GameScene.h"
#include "rapidcsv.h"
#include "Player.h"

GameScene::GameScene(SceneIds id):Scene(id)
{
    
}

GameScene::~GameScene()
= default;

void GameScene::Init()
{
    Scene::Init();
    FRAMEWORK.SetTimeScale(1.f);
    background = new SpriteGo("background");
    background->SetTexture("graphics/map1.png");
    background->SetPosition({0.f, -1000.f});
    background->SetOrigin(Origins::MC);
    AddGo(background,Layers::World);
    map1Texture.loadFromFile("graphics/map1.png");
    map1PixelTexture.loadFromImage(map1Texture);
    
    
    player = new Player("player");
    player->SetPosition({0.f, 0.f});
    player->SetOrigin(Origins::BC);
    AddGo(player,Layers::World);


    introText = new TextGo("intro");
    introText->SetFont("fonts/Galmuri11-Bold.ttf");
    introText->SetString(str);
    introText->SetScale({1.5f, 1.5f});
    introText->SetOrigin(Origins::MC);
    introText->SetPosition({960.f, 540.f});
    AddGo(introText,Layers::Ui);
    
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
    
    auto windowSize = FRAMEWORK.GetWindowSize();
    auto height = 500.f;
    auto width = windowSize.x * height / windowSize.y; //
    worldView.setSize(width, height);
    uiView.setSize(windowSize.x, windowSize.y);
    
    player->SetPosition({0.f, 210.f});
    uiView.setCenter(960.f, 540.f);
    introText->SetAlpha(255);
    fadeoutElapsedTime=0.f;
    fadeoutDuration=3.0f;
    isFadingOut=true;
    player->SetActive(false);
    background->SetActive(false);
}

void GameScene::Exit()
{
    Scene::Exit();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
    // View의 크기 설정
    // 플레이어의 Y 좌표에 따라 View의 중심 위치를 이동
    
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
            player->SetActive(true);
            background->SetActive(true);
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


