#pragma once
#include "Scene.h"
#include "TextGo.h"

class Map;

class GameScene:public Scene
{
protected:
    std::wstring str= L"\t\t\t\t전설에 따르면\n정상에 정말 끝내주는 아가씨가 있다고 하는데...";
    TextGo* introText;
    float fadeoutElapsedTime;
    float fadeoutDuration;
    bool isFadingOut=false;
    Map* map;
    

public:
    GameScene(SceneIds id);
    ~GameScene() override;

    GameScene(const GameScene&) = delete;
    GameScene(GameScene&&) = delete;
    GameScene& operator=(const GameScene&) = delete;
    GameScene& operator=(GameScene&&) = delete;
    
    void Init() override;
    void Release() override;
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};
