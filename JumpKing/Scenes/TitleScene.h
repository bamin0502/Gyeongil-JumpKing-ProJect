#pragma once

class TitleScene:public Scene
{
protected:
    
public:
    TitleScene();
    ~TitleScene();

    void Init() override;
    void Release() override;
    void Enter() override;
    void Exit() override;
    void Update(float dt) override;
    void LateUpdate(float dt) override;
    void FixedUpdate(float dt) override;
    void Draw(sf::RenderWindow& window) override;
};
