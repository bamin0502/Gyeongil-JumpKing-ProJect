#pragma once

#include "Scene.h"
#include "SpriteGo.h"
#include "TextGo.h"

class MapTile;
class Player;


class GameScene:public Scene
{
protected:
    std::wstring str= L"\t\t\t\t전설에 따르면\n정상에 정말 끝내주는 아가씨가 있다고 하는데...";
    TextGo* introText = nullptr;
    
    float fadeoutElapsedTime;
    float fadeoutDuration;
    bool isFadingOut=false;
    
    bool showPixelCoords = false;
    sf::Color pixelColor=sf::Color::Red;
public:
    Player* player=nullptr;

    GameScene(SceneIds id);
    ~GameScene() override;
    SpriteGo* background;
    SpriteGo* background2;
    SpriteGo* background3;
    sf::Image map1Texture;
    sf::Texture map1PixelTexture;
    sf::Sprite map1Sprite;

    sf::RectangleShape map1Rect;
    sf::RectangleShape playerRect;
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
    
    
    sf::Vector2f PlayerBoundsWorldToView(sf::Vector2f playerPosition);
    
    bool IsCollision(const sf::Vector2f& position);


};
