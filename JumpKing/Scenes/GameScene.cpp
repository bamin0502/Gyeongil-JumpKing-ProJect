#include "pch.h"
#include "GameScene.h"
#include <setjmp.h>
#include "rapidcsv.h"
#include "Player.h"

GameScene::GameScene(const SceneIds id): Scene(id), fadeoutElapsedTime(0), fadeoutDuration(0),
                                         player(nullptr),
                                         background(nullptr),
                                         background2(nullptr),
                                         background3(nullptr),
                                         map1(nullptr)
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
    background2 = new SpriteGo("background2");
    background2->SetTexture("graphics/map2.png");
    AddGo(background2);
    background3 = new SpriteGo("background3");
    background3->SetTexture("graphics/map3.png");
    AddGo(background3);
    
    map1Texture.loadFromFile("graphics/backTexture.png");
    map1PixelTexture.loadFromImage(map1Texture);
    map1Sprite.setTexture(map1PixelTexture);
    map1Sprite.setScale({background->GetGlobalBounds().width,background->GetGlobalBounds().height});
    
    
    
    player = new Player("player");
    player->SetOrigin(Origins::BC);
    //player->SetPosition({0.f, 210.f});
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

    const auto windowSize = FRAMEWORK.GetWindowSize();
    constexpr auto height = 500.f;
    const auto width = windowSize.x * height / windowSize.y; //
    worldView.setSize(width, height);
    uiView.setSize(windowSize.x, windowSize.y);
   
    background->SetPosition({-335.f,-2250.f});
    map1Sprite.setPosition(background->GetPosition());
    background2->SetPosition({-335.f,-4750.f});
    background3->SetPosition({-335.f,-7250.f});
    
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
        const float alpha=255.f*(fadeoutElapsedTime/fadeoutDuration);
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
    const sf::Vector2f playerPosition = player->GetPosition();
    constexpr float viewStep = 500.0f;
    const float currentViewBottom = worldView.getCenter().y + viewStep / 2; // 현재 뷰의 하단 위치 계산
    
    if(playerPosition.y < worldView.getCenter().y - viewStep / 2) {
        const float newCenterY = worldView.getCenter().y - viewStep;
        worldView.setCenter(worldView.getCenter().x, newCenterY);
    }
    else if(playerPosition.y > currentViewBottom) {
        const float newCenterY = worldView.getCenter().y + viewStep;
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
    window.draw(map1Sprite);
}

sf::Vector2f GameScene::PlayerBoundsWorldToView(sf::Vector2f playerPosition)
{
    const sf::Vector2f map1Position = background->GetPosition();
    const sf::Vector2f playerPositionRelativeToMap1 = playerPosition - map1Position;
    const sf::Vector2f viewPosition = playerPositionRelativeToMap1 + worldView.getCenter() - worldView.getSize() / 2.f;
    const sf::Vector2f viewPositionRelativeToMap1 = viewPosition - map1Position;

    return viewPositionRelativeToMap1;
}

bool GameScene::IsPlayerInView(sf::Vector2f playerPosition)
{
    const sf::Vector2f viewPosition = PlayerBoundsWorldToView(playerPosition);
    
    return viewPosition.x >= 0 && viewPosition.x <= worldView.getSize().x && viewPosition.y >= 0 && viewPosition.y <= worldView.getSize().y;
}


