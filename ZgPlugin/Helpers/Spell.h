#pragma once

class Spell
{
public:
    Spell( SpellSlot slot );
    Spell( SpellSlot slot, float range );
    
    auto SetSkillShot( float delay, float speed, float radius, bool hitbox, PredictionCollisionFlags flags ) -> void;
    auto IsReady( float t = 0 ) const -> bool;

    auto Cast( ) -> void;
    auto Cast( GameObject* target ) -> void;
    auto Cast( Vector3 position ) -> void;
    auto SetFrom( Vector3 pos ) -> void;
    auto SetRange( float range ) -> void;

    auto RunPrediction( GameObject* unit, PredictionOutput& output ) const -> bool;

    auto Range( ) const -> float;
    auto Level( ) const -> int;
    auto Slot( ) const -> SpellSlot;
    auto LastCastTime( ) const -> float;
    auto Hash( ) const -> uint32_t;

    auto operator new( size_t size ) -> void* { return Globals::Malloc( size ); }
    auto operator delete( void* p ) noexcept -> void { Globals::Free( p ); }

protected:
    SpellSlot slot;
    float range;
    float time;
    bool hitbox;
    float delay;
    float radius;
    float speed;
    Vector3 from;
    PredictionCollisionFlags flags;
};
