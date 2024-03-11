#include "pch.h"
#include "TitleScene.h"
#include "SpriteGo.h"
#include "TextGo.h"


TitleScene::TitleScene(SceneIds id):Scene(id)
{
}

void TitleScene::Init()
{
    Scene::Init();
    
    TitleImage = new SpriteGo("TitleImage");
    TitleImage->SetTexture("graphics/title.png");
    TitleImage->SetOrigin(Origins::MC);
    TitleImage->SetScale({0.75f, 0.75f});
    TitleImage->SetPosition({0.f, -250.f});
    AddGo(TitleImage);
    
    pressStart = new TextGo("PressStart");
    pressStart->SetFont("fonts/Galmuri11-Bold.ttf");
    pressStart->SetScale({1.5f, 1.5f});
    pressStart->SetString(str);
    pressStart->SetOrigin(Origins::MC);
    pressStart->SetPosition({0.f, 150.f});
    AddGo(pressStart);
    
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

    TitleImage->SetAlpha(0);

    fadeInElapsedTime = 0.f;
    fadeInDuration = 1.5f;

    isFadingIn = true;
}

void TitleScene::Exit()
{
    Scene::Exit();
}

void TitleScene::Update(float dt)
{
    Scene::Update(dt);
    if(isFadingIn)
    {
        fadeInElapsedTime += dt;
        float alpha = (fadeInElapsedTime / fadeInDuration) * 255;
        TitleImage->SetAlpha(alpha);

        if (fadeInElapsedTime >= fadeInDuration) {
            isFadingIn = false;
            fadeInElapsedTime = 0.f; // 페이드인 종료 후에 elapsedTime 초기화
        }
    }

    static float elapsedTime = 0.f;
    static bool visible = true;
    elapsedTime += dt;

    // 일정 간격으로 텍스트가 깜박이도록 함
    if (!isFadingIn && elapsedTime >= 0.5f) { // 페이드인이 완료되었을 때에만 깜빡이도록 함
        visible = !visible;
        // 텍스트의 색상을 변경하여 가시성을 조절함
        if (visible) {
            pressStart->SetFontColor(sf::Color::White);
        } else {
            pressStart->SetFontColor(sf::Color::Transparent);
        }
        elapsedTime = 0.f;
    }

    //스페이스바 누르면 넘어갈 수 있게 
    if(InputMgr::GetKeyDown(sf::Keyboard::Space))
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
