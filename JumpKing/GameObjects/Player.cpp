#include "pch.h"
#include "Player.h"
#include "GameScene.h"

Player::Player(const std::string& name)
    : SpriteGo(name), jumpPhase(JumpPhase::Grounded),jumpHeight(0), jumpDirection(0.f),
      jumpGuage(0),
      currentJumpStage(0),
      currentHeight(0)
{
    
}

void Player::Init() {
    animator.SetTarget(&sprite);
    SetPosition({0.f, 210.f});
}

void Player::Release() {
    
}

void Player::Reset() {
    gameScene = dynamic_cast<GameScene*>(SCENE_MGR.GetCurrentScene());
    collisionMap = gameScene->map1Sprite.getTexture()->copyToImage();
    animator.Play("animations/player_Idle.csv");
    SetOrigin(Origins::BC);

    jumpPhase = JumpPhase::Grounded;
}

void Player::Update(float dt) {
    SpriteGo::Update(dt);
    animator.Update(dt);

    
    //std::cout<<"player position: "<<position.x<<", "<<position.y<<std::endl;
    if (isCollision(playerBounds.getPosition())) {
        
    }
    
    
    HandleInput();
    if (isJumpCharging) {
        const float chargingTime = timer.getElapsedTime().asSeconds() - jumpStartTime;
        if (chargingTime > 1.0f) { 
            PerformJump(); 
        }
    }
    UpdateMovement(dt);
    UpdateAnimation();
    SetPosition(position);
}

void Player::LateUpdate(float dt) {
    SpriteGo::LateUpdate(dt);
    
}

void Player::FixedUpdate(float dt) {
    SpriteGo::FixedUpdate(dt);
    
}

void Player::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    
    //플레이어 바운딩 박스 그리기
    sf::RectangleShape boundingBox;
    boundingBox.setPosition(sprite.getPosition());
    boundingBox.setOrigin(sprite.getOrigin());
    sf::FloatRect bounds = sprite.getGlobalBounds();
    boundingBox.setSize(sf::Vector2f(bounds.width, bounds.height));
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setOutlineColor(sf::Color::Black); // 빨간색으로 테두리를 그립니다.
    boundingBox.setOutlineThickness(1.f); // 테두리 두께 설정
    window.draw(boundingBox);
    // 맵 내에 픽셀 색깔을 기준으로 보여줄 바운딩 박스
    sf::FloatRect mapBounds = gameScene->map1Sprite.getGlobalBounds();
    sf::RectangleShape mapBox;
    mapBox.setPosition(mapBounds.left, mapBounds.top);
    mapBox.setSize(sf::Vector2f(mapBounds.width, mapBounds.height));
    mapBox.setFillColor(sf::Color::Transparent);
    mapBox.setOutlineColor(sf::Color::Blue); // 파란색으로 테두리를 그립니다.
    mapBox.setOutlineThickness(1.f); // 테두리 두께 설정
    window.draw(mapBox);
    
}

void Player::HandleInput() {
    if (!isJumping) {
        jumpDirection = 0.f;
        if (InputMgr::GetKey(sf::Keyboard::Left)) {
            jumpDirection = -1.f;
            sprite.setScale(-1.f, 1.f); 
        } else if (InputMgr::GetKey(sf::Keyboard::Right)) {
            jumpDirection = 1.f;
            sprite.setScale(1.f, 1.f); 
        }
    }
    
    if (InputMgr::GetKeyDown(sf::Keyboard::Space) && isGrounded && !isJumping) {
        StartJumpCharging();
    }
    
    if (InputMgr::GetKeyUp(sf::Keyboard::Space) && isJumpCharging && !isJumping) {
        PerformJump();
    }
}

void Player::UpdateMovement(const float dt) {
    sf::Vector2f potentialPosition = position;
    
    switch (jumpPhase)
    {
        case JumpPhase::Rising:
        case JumpPhase::Falling:
            potentialPosition +=velocity * dt;
            if(!isCollision(potentialPosition))
            {
                position=potentialPosition;
            }
            // velocity.y += gravity * dt;
            // position += velocity * dt;
            break;
    case JumpPhase::Grounded:
            potentialPosition +=sf::Vector2f(jumpDirection * moveSpeed*dt,0.f);
            if(!isCollision(potentialPosition))
            {
                position=potentialPosition;
            }
            // velocity.x = jumpDirection * moveSpeed;
            // position += velocity * dt;
            break;
        case JumpPhase::Charging:
            break;
        case JumpPhase::Landing:
            break;
        default:
            break;
    }
}



void Player::UpdateAnimation() {
    switch (jumpPhase) {
    case JumpPhase::Charging:
        if (animator.GetCurrentClipId() != "animations/player_Jump.csv") {
            animator.Play("animations/player_Jump.csv");
        }
        break;
    case JumpPhase::Rising:
        if (animator.GetCurrentClipId() != "animations/player_JumpUp.csv") {
            animator.Play("animations/player_JumpUp.csv");
        }
        break;
    case JumpPhase::Falling:
        if (animator.GetCurrentClipId() != "animations/player_JumpDown.csv") {
            animator.Play("animations/player_JumpDown.csv");
        }
        break;
    case JumpPhase::Grounded:
          if (std::abs(velocity.x) >= 0.1f) {
                if (animator.GetCurrentClipId() != "animations/player_Move.csv") {
                    animator.Play("animations/player_Move.csv");
                }
            } else {
                if (animator.GetCurrentClipId() != "animations/player_Idle.csv") {
                    animator.Play("animations/player_Idle.csv");
                }
            }
        break;
    default: ;
    }
}

void Player::StartJumpCharging() {
    isJumpCharging = true;
    jumpPhase = JumpPhase::Charging;
    jumpStartTime = timer.getElapsedTime().asSeconds();
}

float calculateJumpHeight(float chargeDuration, float maxJumpHeight)
{
    constexpr int levels = 35;
    constexpr float maxChargeTime = 1.5f;
    constexpr float levelDuration = maxChargeTime / levels;
    const int level = std::min(static_cast<int>(chargeDuration / levelDuration), levels);
    const float jumpHeight = maxJumpHeight * (level / static_cast<float>(levels));

    return jumpHeight;
}
void Player::PerformJump() {
    const float chargeDuration = timer.getElapsedTime().asSeconds() - jumpStartTime;
    const float jumpHeight = calculateJumpHeight(chargeDuration, maxJumpHeight);

    isJumpCharging = false;
    isJumping = true;
    jumpPhase = JumpPhase::Rising;

    //2였지만 일단 잘되는지 테스트를 위해서 수정
    velocity.y = -sqrt(2 * gravity * jumpHeight); 
    velocity.x = jumpDirection * moveSpeed * 0.5; 

    sprite.setScale(jumpDirection < 0 ? -1.f : 1.f, 1.f);
}

bool Player::isCollision(sf::Vector2f position)
{
    //플레이어의 충돌체크를 위한 코드
    //빨간색 픽셀은 충돌이 있는 부분이고 플레이어의 충돌체크는 빨간색 픽셀이 있는지 없는지만 확인하면 된다.
    //파란색 픽셀은 바닥을 감지하는 용도로 사용한다.
    //플레이어의 중심점을 기준으로 4방향의 충돌체크를 한다.
    // 플레이어의 전역 경계를 가져옴
    sf::FloatRect playerBounds = sprite.getGlobalBounds();

    // 상하좌우 중심점을 계산
    sf::Vector2f points[4] = {
        sf::Vector2f(playerBounds.left + playerBounds.width / 2, playerBounds.top), // 상
        sf::Vector2f(playerBounds.left + playerBounds.width / 2, playerBounds.top + playerBounds.height), // 하
        sf::Vector2f(playerBounds.left, playerBounds.top + playerBounds.height / 2), // 좌
        sf::Vector2f(playerBounds.left + playerBounds.width, playerBounds.top + playerBounds.height / 2) // 우
    };

    for (auto& point : points) {
        // 맵의 좌표계로 변환
        sf::Vector2f scenePos = gameScene->map1Sprite.getTransform().getInverse().transformPoint(point);

        // 변환된 좌표가 이미지 범위 내에 있는지 확인
        if (scenePos.x >= 0 && scenePos.y >= 0 && scenePos.x < collisionMap.getSize().x && scenePos.y < collisionMap.getSize().y) {
            sf::Color color = collisionMap.getPixel(static_cast<unsigned int>(scenePos.x), static_cast<unsigned int>(scenePos.y));
            if (color == sf::Color(255, 0, 0) || color == sf::Color(0, 71, 255)) { // 빨간색 또는 파란색인 경우 충돌 감지
                std::cout<<"color: "<<(int)color.r<<", "<<(int)color.g<<", "<<(int)color.b<<std::endl;
                return true;
            }
        }
    }
    std::cout<<"color: "<<(int)0<<", "<<(int)0<<", "<<(int)0<<std::endl;
    return false; // 충돌 없음
}

int Player::checkCollision(sf::Vector2f newPosition)
{
    //벽과 바닥 충돌을 검사하는 로직 구현
    //벽 충돌시에는 1, 바닥 충돌시에는 0, 충돌이 없는 상태는 -1을 반환한다.
    //플레이어의 중심점을 기준으로 4방향의 충돌체크를 한다.
    // 플레이어의 전역 경계를 가져옴
    enum class Direction
    {
        NOCONFLICT=-1,
        GROUND,
        WALL
    };
    
    
}

// sf::Vector2f mousePos=InputMgr::GetMousePos();
// sf::Vector2f scenePos=gameScene->ScreenToWorld((sf::Vector2i)mousePos);
// scenePos=gameScene->map1Sprite.getTransform().getInverse().transformPoint(scenePos);
//
// sf::Color color=collisionMap.getPixel(scenePos.x,scenePos.y);
// std::cout<<"color: "<<(int)color.r<<", "<<(int)color.g<<", "<<(int)color.b<<std::endl;
//















