#include "pch.h"
#include "rapidcsv.h"

bool AnimationClip::loadFromFile(const std::string& filePath)
{
    rapidcsv::Document doc(filePath);
    //id=doc.GetCell<std::string>(0,0);
    id=filePath;
    fps=doc.GetCell<int>(1,0);
    LoopType=(AnimationLoopTypes)doc.GetCell<int>(2,0);
    //LoopType=static_cast<AnimationLoopTypes>(doc.GetCell<int>(2,0));

    for(int i = 3; i <doc.GetRowCount(); ++i)
    {
        auto row=doc.GetRow<std::string>(i);
        frames.push_back({row[0],{std::stoi(row[1]),std::stoi(row[2]),std::stoi(row[3]),std::stoi(row[4])}});
    }
    
    return true;
}

Animator::Animator()
{
    
}

Animator::~Animator()
{
    
}

void Animator::AddEvent(const std::string& clipId, int frame, std::function<void()> action)
{
    eventList.push_back({clipId,frame,action});
}

void Animator::ClearEvent()
{
    eventList.clear();
}


void Animator::Update(float dt)
{
    if(!isPlaying)
    {
        return;
    }
    accumTime += dt * speed;
    if(accumTime < clipDuration)
    {
        return;
    }
    accumTime=0.f;
    currentFrame += addFrame;
    
    if(currentFrame == totalFrame)
    {
        if(!queue.empty())
        {
            std::string id=queue.front();
            queue.pop();
            Play(id,false);
            return;
        }

        
        switch (currentClip->LoopType)
        {
        case AnimationLoopTypes::Single:
            currentFrame = totalFrame - 1;
            return;
        case AnimationLoopTypes::Loop:
            currentFrame = 0;
            break;
        case AnimationLoopTypes::PingPong:
            if(addFrame > 0)
            {
                currentFrame = totalFrame - 2;
                addFrame = -1;
                totalFrame = -1;
            }
            else
            {
                currentFrame = 1;
                addFrame = 1;
                totalFrame=currentClip->frames.size();
            }
            break;
        }
    }

    for(auto& event : eventList)
    {
        if(currentClip->id == event .clipId && currentFrame == event.frame)
        {
            if(event.action != nullptr)
            {
                event.action();
            }
        }
        
    }
    
    SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Play(const std::string& clipId,bool clearQueue)
{
    if(clearQueue)
    {
        while (!queue.empty())
        {
            queue.pop();
        }
    }

    addFrame = 1;
    isPlaying = true;
    accumTime = 0.f;
    currentClip = &RES_MGR_ANIMATION_CLIP.Get(clipId);
    currentFrame = 0;
    totalFrame = currentClip->GetTotalFrame();
    clipDuration = 1.f / currentClip->fps;
    SetFrame(currentClip->frames[currentFrame]);
    
}

void Animator::PlayQueue(const std::string& clipId)
{
    queue.push(clipId);
}

void Animator::Stop()
{
    isPlaying = false;
    
}

void Animator::SetFrame(const AnimationFrame& frame)
{
    target->setTextureRect(frame.texCoord);
    target->setTexture(frame.GetTexture());
    
}
