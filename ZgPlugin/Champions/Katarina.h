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
    void OnPreUpdate( );
    void OnUpdate( );
    
    bool CanPostExecute( GameObject* unit );
    bool CanPreExecute( GameObject* unit );
    
    void UseQ( GameObject* unit );
    void UseW( GameObject* unit );
    void UseE( GameObject* unit, float range );
    void UseR( GameObject* unit );
    void GapE( );
    
    void UseIgnite( GameObject* unit );
    void UseProtoBelt( float range );

    void Auto( );
    void Combo( );
    void Harass( );
    void LastHit( );
    void Flee( );

    void OnCreateParticle( GameObject* pObject, std::uint32_t iHash );
    void RenderDaggers( );
    void RemoveDaggers( );

    Vector<GameObject*> ShunpoJumpList( );
    Vector3 ShunpoPosition( GameObject* pObject, bool hero = true );
    float DynamicRange( );
    bool Spinning( );
    
    float UltDmg( GameObject* pObject, float time );
    float GroundDaggerDmg( GameObject* pObject, bool draw = false );
    float ShunpoDmg( GameObject* pObject );
    float DaggerDmg( GameObject* pObject );
    float IgniteDmg( );

    // - setup menu
    void SetupMenu( );
    
    [[clang::no_destroy]] inline Vector<Dagger> Daggers;
    
    inline Spell* KatQ = nullptr;
    inline Spell* KatW = nullptr;
    inline Spell* KatE = nullptr;
    inline Spell* KatR = nullptr;
    inline Spell* Ignite = nullptr;
    inline Vector3* Cursor = { };

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
        inline MenuElement* GapE = nullptr;
        inline MenuElement* UseR = nullptr;
        inline MenuElement* AutoR = nullptr;
        inline MenuElement* UseItems = nullptr;
        inline MenuElement* UseIgnite = nullptr;
        inline MenuElement* Channel = nullptr;
        inline MenuElement* Cancel = nullptr;
        inline MenuElement* DaggerCalc = nullptr;
        inline MenuElement* ShunpoMode = nullptr;
        inline MenuElement* Style = nullptr;
        inline MenuElement* Toggle = nullptr;
        inline MenuElement* RKillable;
    }
};
