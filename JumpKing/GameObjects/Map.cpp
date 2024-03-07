#include "pch.h"
#include "Map.h"
#include "GameObject.h"
#include "GameScene.h"
#include "Player.h"

Map::Map(const std::string& filename):GameObject("Map")
{
   //image.loadFromFile("graphics/map1_backGround.png");
   //image.loadFromFile("graphics/map2_backGround.png");
   //image.loadFromFile("graphics/map3_backGround.png");
   if(!image.loadFromFile(filename))
   {
       std::cerr<<"Failed to load image from "<<filename<<std::endl;
   }
}

void Map::Init()
{
    GameObject::Init();
    
    sf::Vector2u size = image.getSize();
    for(unsigned int x=0; x<size.x; ++x)
    {
        for(unsigned int y=0; y<size.y; ++y)
        {
            sf::Color color = image.getPixel(x, y);
            if(color==sf::Color::Red)
            {
                //player 충돌 처리
                
            }
        }
    }

    foreground1 = new SpriteGo("map1");
    foreground1->SetTexture("graphics/map1.png");
    foreground1->SetPosition({0.f, 0.f});
    foreground1->SetOrigin(Origins::MC);
    

    foreground2 = new SpriteGo("map2");
    foreground2->SetTexture("graphics/map2.png");
    foreground2->SetPosition({0.f, 2500.f});
    foreground2->SetOrigin(Origins::MC);
    

    foreground3 = new SpriteGo("map3");
    foreground3->SetTexture("graphics/map3.png");
    foreground3->SetPosition({0.f, 5000.f});
    foreground3->SetOrigin(Origins::MC);
   
    
    
}

void Map::Release()
{
    GameObject::Release();
}

void Map::Reset()
{
    GameObject::Reset();
}

void Map::Update(float dt)
{
    GameObject::Update(dt);
    
}

void Map::FixedUpdate(float dt)
{
    GameObject::FixedUpdate(dt);
}

void Map::SetPosition(float x, float y)
{
    
}

void Map::SetPosition(const sf::Vector2f& pos)
{
    GameObject::SetPosition(pos);
}

void Map::Translate(float x, float y)
{
    
}

void Map::Translate(const sf::Vector2f& delta)
{
    GameObject::Translate(delta);
}

void Map::SetRotation(float r)
{
    GameObject::SetRotation(r);
}

void Map::Draw(sf::RenderWindow& window)
{
    GameObject::Draw(window);
    
    
}
