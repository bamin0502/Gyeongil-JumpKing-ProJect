#pragma once
#include "GameScene.h"
#include "SpriteGo.h"

class Map:public GameObject
{
protected:
    //충돌할 이미지가 그려져 있는 이미지
    //픽셀값을 받아오는 함수
    std::vector<sf::Texture> background=
    {
        background1=RES_MGR_TEXTURE.Get("graphics/map1_backGround.png"),
        //background2=RES_MGR_TEXTURE.Get("graphics/map2_backGround.png"),
        //background3=RES_MGR_TEXTURE.Get("graphics/map3_backGround.png")
    };
    //그 위에 충돌할 이미지를 가릴 이미지
    // std::vector<SpriteGo> foreground=
    // {
    //     *foreground1=SpriteGo("graphics/map1.png"),
    //     *foreground2=SpriteGo("graphics/map2.png"),
    //     *foreground3=SpriteGo("graphics/map3.png")
    // };

    sf::Texture background1;
    SpriteGo* foreground1;
    
    sf::Texture background2;
    SpriteGo* foreground2;
    
    sf::Texture background3;
    SpriteGo* foreground3;

    sf::Image image;
    int cutSizeY=500;

    GameScene* gameScene;
public:
    Map(const std::string& filename="");
    
    void Init() override;
    void Release() override;
    void Reset() override;
    void Update(float dt) override;
    void FixedUpdate(float dt) override;
    
    void SetPosition(float x,float y);
    void SetPosition(const sf::Vector2f& pos) override;
    void Translate(float x,float y);
    void Translate(const sf::Vector2f& delta) override;
    void SetRotation(float r) override;
    
    void Draw(sf::RenderWindow& window) override;
    
};
