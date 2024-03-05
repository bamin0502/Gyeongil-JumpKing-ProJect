#pragma once

class GameScene:public Scene
{
protected:
    
public:
    GameScene(SceneIds id);
    ~GameScene() override;

    void Init() override;
    void Release() override;
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};
