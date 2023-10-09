#pragma once

namespace Camille
{
    struct Avoid
    {
        GameObject* Obj;
        float CreateTime;
        float Radius;
    };
    
    // - main 
    void OnBoot( );
    void OnTerminate( );
    void OnUpdate( );
    void OnPresentDraw( );
    void OnDraw( );
    void OnCreateParticle( GameObject* pObject, std::uint32_t iHash );
    void OnExecuteCastFrame( GameObject* pCaster, SpellCasterClient* pCasterClient );
    void OnIssueOrder( Vector3* pPosition, GameObject* pTarget, bool bWasPlayerIssued, bool* pCancel );

    // - aura mechanics
    bool HasQ( );
    bool HasQ2( );
    bool ChargingW( );
    bool OnWall( );
    bool IsDashing( );
    bool KnockedBack( GameObject* unit );
    bool CanW( GameObject* unit );

    // -> ability methods
    void UseQ( GameObject* unit );
    void UseW( GameObject* unit );
    void UseE( Vector3 pos, bool combo = true);
    void UseR( GameObject* unit, bool force = false );

    // -> damage
    float AutoAttackDmg( GameObject* unit );
    float ComboDamage( GameObject* unit );
    float IgniteDmg( );
    float QDmg( GameObject* unit, bool includeq2 = true );
    float WDmg( GameObject* unit, bool bonus = false );
    float EDmg( GameObject* unit, bool aa = true );
    float RDmg( double dmg, GameObject* unit );

    inline Spell* CamQ;
    inline Spell* CamW;
    inline Spell* CamE;
    inline Spell* CamR;
    inline Spell* Ignite;
    inline float Limiter;
    inline float MyRange;
    inline Vector3* Cursor = { };
    
    [[clang::no_destroy]] inline Vector<Avoid> AvoidList;

    namespace Menu
    {
        inline MenuElement* Root;
        inline MenuElement* UseQ;
        inline MenuElement* DrawQ;
        inline MenuElement* UseW;
        inline MenuElement* DrawW;
        inline MenuElement* UseE;
        inline MenuElement* DrawE;
        inline MenuElement* UseR;
        inline MenuElement* DrawR;
        inline MenuElement* FleeW;
        inline MenuElement* Difficulty;
        inline MenuElement* AutoR;
        inline MenuElement* UseIgnite;
        inline MenuElement* DrawDamageBar;
        inline MenuElement* HookSpot;
        inline MenuElement* HarassQPct;
        inline MenuElement* HarassWPct;
        inline MenuElement* Selected;
        inline MenuElement* DiveE;
        inline MenuElement* LastHitQ;
    }
    
}
