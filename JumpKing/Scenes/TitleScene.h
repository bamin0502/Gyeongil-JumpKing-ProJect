#pragma once
#include "Scene.h"

class SpriteGo;
class TextGo;

class TitleScene:public Scene
{
protected:
    SpriteGo* TitleImage;
    TextGo* pressStart;
    std::wstring str= L"Space 버튼을 누르세요";
    float fadeInElapsedTime;
    float fadeInDuration;
    sf::Music bgm;
    bool isFadingIn=false;
public:
    TitleScene(SceneIds id);
    ~TitleScene() override=default;

    TitleScene(const TitleScene&) = delete;
    TitleScene(TitleScene&&) = delete;
    TitleScene& operator=(const TitleScene&) = delete;
    TitleScene& operator=(TitleScene&&) = delete;

    void Init() override;
    void Release() override;
    
    void Enter() override;
    void Exit() override;
    
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    
    void Draw(sf::RenderWindow& window) override;
};
