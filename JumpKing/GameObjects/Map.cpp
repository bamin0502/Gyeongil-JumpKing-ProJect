#include "pch.h"
#include "Map.h"

Map::Map(const std::string& name)
{
}

void Map::Init()
{
    GameObject::Init();
}

void Map::Update(float dt)
{
    GameObject::Update(dt);
}

void Map::Draw(sf::RenderWindow& window)
{
    GameObject::Draw(window);
}

void Map::Release()
{
    GameObject::Release();
}

void Map::Reset()
{
    GameObject::Reset();
}

void Map::SetPosition(const sf::Vector2f& pos)
{
    GameObject::SetPosition(pos);
}
