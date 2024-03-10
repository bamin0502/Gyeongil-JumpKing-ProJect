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
    //background->SetOrigin(Origins::MC);
    AddGo(background);
    
    map1Texture.loadFromFile("graphics/backTexture.png");
    map1PixelTexture.loadFromImage(map1Texture);
    map1Sprite.setTexture(map1PixelTexture);
    map1Sprite.setPosition({-335.f,-2250.f});

    
    player = new Player("player");
    player->SetOrigin(Origins::BC);
    player->SetPosition({0.f, 210.f});
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
    background->SetPosition({-335.f,-2250.f});
    
    
    //player->SetPosition({0.f, 210.f});
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
    // 뷰 업데이트를 결정하기 위해 플레이어의 Y축 위치 확인
    sf::Vector2f playerPosition = player->GetPosition();
    float viewStep = 500.0f; // 뷰를 업데이트할 Y축 단위
    float currentViewBottom = worldView.getCenter().y + viewStep / 2; // 현재 뷰의 하단 위치 계산
    
    // 플레이어가 현재 뷰의 상단을 벗어난 경우
    if(playerPosition.y < worldView.getCenter().y - viewStep / 2) {
        // 뷰를 위로 업데이트
        float newCenterY = worldView.getCenter().y - viewStep;
        worldView.setCenter(worldView.getCenter().x, newCenterY);
    }
    // 플레이어가 현재 뷰의 하단을 벗어난 경우
    else if(playerPosition.y > currentViewBottom) {
        // 뷰를 아래로 업데이트
        float newCenterY = worldView.getCenter().y + viewStep;
        worldView.setCenter(worldView.getCenter().x, newCenterY);
    }
}

void GameScene::FixedUpdate(float dt)
{
    Scene::FixedUpdate(dt);
    
}

void GameScene::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
    
}

sf::Vector2f GameScene::PlayerBoundsWorldToView(sf::Vector2f playerPosition)
{
    sf::Vector2f viewPosition = playerPosition;
    viewPosition.x = playerPosition.x - worldView.getCenter().x + worldView.getSize().x / 2;
    viewPosition.y = playerPosition.y - worldView.getCenter().y + worldView.getSize().y / 2;
    return viewPosition;
}

bool GameScene::IsPlayerInView(sf::Vector2f playerPosition)
{
    sf::Vector2f viewPosition = PlayerBoundsWorldToView(playerPosition);
    return viewPosition.x >= 0 && viewPosition.x <= worldView.getSize().x && viewPosition.y >= 0 && viewPosition.y <= worldView.getSize().y;
}


