#pragma once
#include "SpriteGo.h"
#include "TextGo.h"

class TitleScene:public Scene
{
protected:
    SpriteGo* title;
    //TextGo* pressStart;
    
public:
    TitleScene(SceneIds id);
    ~TitleScene() override;

    void Init() override;
    void Release() override;
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};
