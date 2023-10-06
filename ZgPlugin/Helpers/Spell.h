#pragma once

class Spell
{
public:
    Spell( SpellSlot slot, float range );
    bool IsReady( float time = 0 ) const;

    void Cast( );
    void Cast( GameObject* target );
    void Cast( Vector3 position );
    
    float Range( ) const;
    SpellSlot Slot() const;
    float LastCastTime( ) const;

    void* operator new( size_t size ) { return Globals::Malloc( size ); }
    void operator delete( void* p ) noexcept { Globals::Free( p ); }

protected:
    SpellSlot slot;
    float range;
    float time;
};
