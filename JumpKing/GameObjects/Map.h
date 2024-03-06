#pragma once
#include "GameObject.h"
#include "SpriteGo.h"

class Map:public GameObject
{
protected:
    SpriteGo* map1;
    SpriteGo* map2;
    SpriteGo* map3;

    
public:
    Map(const std::string& name = "");

    void Init() override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void Release() override;
    void Reset() override;
    void SetPosition(const sf::Vector2f& pos) override;
    
};
