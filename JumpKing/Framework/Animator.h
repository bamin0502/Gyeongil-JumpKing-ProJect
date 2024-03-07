#pragma once

enum class AnimationLoopTypes
{
    Single,
    Loop,
    PingPong
};

struct AnimationFrame
{
    std::string textureId;
    sf::IntRect texCoord;

    AnimationFrame(const std::string& id, const sf::IntRect& Coord):textureId(id),texCoord(Coord)
    {
        
    }
    
    const sf::Texture& GetTexture() const
    {
        return RES_MGR_TEXTURE.Get(textureId);
    }
};

struct AnimationClip
{
    std::string id;
    std::vector<AnimationFrame> frames;
    AnimationLoopTypes LoopType=AnimationLoopTypes::Single;
    int fps=5;

    int GetTotalFrame() const
    {
        return frames.size();
    }

    bool loadFromFile(const std::string& filePath);
    
};

struct AnimationEvent
{
    std::string clipId;
    int frame=0;
    std::function<void()> action;

};

class Animator
{
protected:
    std::unordered_map<std::string, AnimationClip> clips;
    std::queue<std::string> queue;
    std::list<AnimationEvent> eventList;

    float speed =1.f;

    bool isPlaying = false;
    AnimationClip* currentClip=nullptr;
    int totalFrame=0;
    int currentFrame=-1;
    int addFrame=1;
    float clipDuration=0.f;
    float accumTime=0.f;
    
    sf::Sprite* target=nullptr;
public:
    Animator();
    virtual ~Animator();

    void AddEvent(const std::string& clipId, int frame, std::function<void()> action);
    void ClearEvent();

    bool IsPlaying() const {return isPlaying;}
    const std::string& GetCurrentClipId()
    {
        return RES_MGR_ANIMATION_CLIP.Get(currentClip->id).id;
    }
    
    sf::Sprite* GetTarget() const{return target;}
    void SetTarget(sf::Sprite* t){target=t;}

    float GetSpeed() const{return speed;}
    void SetSpeed(float s){speed=s;}

    

    void Update(float dt);

    void PlayIndex(const std::string& clipId, int index);
    void Play(const std::string& clipId,bool clearQueue=true);
    void PlayQueue(const std::string& clipId);
    void Stop();
    void SetFrame(const AnimationFrame& frame);
    
};
