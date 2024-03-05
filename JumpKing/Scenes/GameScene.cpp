#include "pch.h"
#include "GameScene.h"

GameScene::GameScene(SceneIds id):Scene(id)
{
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
    
    Scene::Init();
}

void GameScene::Release()
{
    Scene::Release();
}

void GameScene::Enter()
{
    Scene::Enter();
}

void GameScene::Exit()
{
    Scene::Exit();
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}

void GameScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
}

void GameScene::FixedUpdate(float dt)
{
    Scene::FixedUpdate(dt);
}

void GameScene::Draw(sf::RenderWindow& window)
{
    Scene::Draw(window);
}
