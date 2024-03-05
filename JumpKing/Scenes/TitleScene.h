#pragma once
#include "Scene.h"

class SpriteGo;

class TitleScene:public Scene
{
protected:
    SpriteGo* backgroundImage;
    //TextGo* pressStart;
    
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
