#pragma once
#include "Singleton.h"

class SoundMgr:public Singleton<SoundMgr>
{
    friend Singleton<SoundMgr>;
private:
    SoundMgr();
    ~SoundMgr() override;

    sf::Sound bgm[2];
    //위에 배열을 기준으로 현재 재생중인 bgm의 인덱스
    int frontBgmIndex=0;
    float fadespeed=0.5f;
    float fadeLimit=1.f;
    bool isFading=false;//크로스페이드 중인지 여부
    float crossFadeDuration=1.f;//크로스페이드 시간
    float crossFadeTimer=0.f;//크로스페이드 타이머

    std::list <sf::Sound*> playing;
    std::list <sf::Sound*> waiting;
    
    float sfxVolume=100.f;
    float bgmVolume=100.f;
public:
    void SetSfxVolume(float v) {sfxVolume=v;}
    void SetBgmVolume(float v) {bgmVolume=v;}
    
    void Init(int totalChannels=32);
    void Release();

    void Update(float dt);
    
    void PlaySfx(std::string id,bool loop=false);
    void PlaySfx(sf::SoundBuffer& buffer, bool loop = false);
    void PlayBgm(std::string id,bool crossFade=true);
    void StopBgm();

    void StopAllSfx();
    
};
#define SOUND_MGR (Singleton<SoundMgr>::Instance())