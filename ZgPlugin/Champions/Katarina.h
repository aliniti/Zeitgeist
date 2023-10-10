#pragma once

namespace Katarina
{
    struct Dagger
    {
        GameObject* Obj;
        Vector3 Position;
        float CreateTime;
    };
    
    void OnBoot( );
    void OnTerminate( );
    void OnDraw( );
    void OnPresentDraw( );
    void OnCreateParticle( GameObject* pObject, std::uint32_t iHash );
    Vector3 ShunpoPosition( GameObject* pObject );
    void OnPreUpdate( );
    void OnUpdate( );
    void OnPostUpdate( );
    float DynamicRange( );
    
    bool CanPostExecute( GameObject* pObject );
    bool CanPreExecute( GameObject* pObject );
    bool Spinning( );

    void UseQ( );
    void UseW( );
    void UseE( float range );
    void UseR( );
    void UseIgnite( );
    void UseProtoBelt( float range );
    void Flee();
    
    float UltDmg( GameObject* pObject, float time );
    float GroundDaggerDmg( GameObject* pObject, bool draw = false );
    float ShunpoDmg( GameObject* pObject );
    float DaggerDmg( GameObject* pObject );
    float IgniteDmg( );
    
    [[clang::no_destroy]] inline Vector<Dagger> Daggers;
    [[clang::no_destroy]] inline Vector<GameObject*> ShunpoObjects;

    inline float Limiter = 0.f;
    inline Vector3* Cursor = { };
    inline Spell* KatQ = nullptr;
    inline Spell* KatW = nullptr;
    inline Spell* KatE = nullptr;
    inline Spell* KatR = nullptr;
    inline Spell* Ignite = nullptr;

    namespace Menu
    {
        inline MenuElement* Root = nullptr;
        
        inline MenuElement* DrawQ = nullptr;
        inline MenuElement* ColorQ = nullptr;
        inline MenuElement* DrawW = nullptr;
        inline MenuElement* ColorW = nullptr;
        inline MenuElement* DrawE = nullptr;
        inline MenuElement* ColorE = nullptr;
        inline MenuElement* DrawR = nullptr;
        inline MenuElement* ColorR = nullptr;
        inline MenuElement* DrawDagger = nullptr;

        inline MenuElement* DrawHPBar = nullptr;
        inline MenuElement* ColorHPBar = nullptr;

        inline MenuElement* UseQ = nullptr;
        inline MenuElement* LastHitQ = nullptr;
        inline MenuElement* UseW = nullptr;
        inline MenuElement* FleeW = nullptr;
        inline MenuElement* UseE = nullptr;
        inline MenuElement* FleeE = nullptr;
        inline MenuElement* DiveE = nullptr;
        inline MenuElement* UseR = nullptr;
        inline MenuElement* AutoR = nullptr;
        inline MenuElement* UseItems = nullptr;
        inline MenuElement* UseIgnite = nullptr;
        inline MenuElement* Channel = nullptr;
        inline MenuElement* Cancel = nullptr;
        inline MenuElement* DaggerCalc = nullptr;
        inline MenuElement* UltMode = nullptr;
        inline MenuElement* ShunpoMode = nullptr;
        inline MenuElement* Style = nullptr;
        inline MenuElement* Toggle = nullptr;
        inline MenuElement* Killsteal = nullptr;
        
    }
};
