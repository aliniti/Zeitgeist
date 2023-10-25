#include "../stdafx.hpp"

// Constructor
Spell::Spell( SpellSlot slot )
{
    this->slot = slot;
    this->time = 0;
    this->range = 999999.f;
}

// Constructor
Spell::Spell( SpellSlot slot, float range )
{
    this->slot = slot;
    this->range = range;
    this->time = 0;
}

// Get the starting position
auto Spell::HitBox( ) const -> bool
{
    return this->hitbox;
}

// Get the spell range
auto Spell::Range( ) const -> float
{
    return this->range;
}

// Get the spell radius
auto Spell::Radius( ) const -> float
{
    return this->radius;
}

// Get the spell level
auto Spell::Level( ) const -> int
{
    return GetPlayer(  )->Spellbook(  )->GetSpell( this->slot )->Level(  );
}

// Get the spell slot
auto Spell::Slot( ) const -> SpellSlot
{
    return this->slot;
}

// Get the last cast time
auto Spell::LastCastTime( ) const -> float
{
    return this->time;
}

// Get the spell hash
auto Spell::Hash( ) const -> uint32_t
{
    return GetPlayer(  )->Spellbook(  )->GetSpell( this->slot )->SpellData(  )->Hash(  );
}

// Set the starting position
void Spell::SetFrom( Vector3 pos )
{
    this->from = pos;
}

// Set the range
auto Spell::SetRange( float range ) -> void
{
    this->range = range;
}

// Set the delay
auto Spell::SetDelay( float delay ) -> void
{
    this->delay = delay;
}

// Set the speed
auto Spell::SetSpeed( float speed ) -> void
{
    this->speed = speed;
}

// Set the radius
auto Spell::SetRadius( float radius ) -> void
{
    this->radius = radius;
}

// Set the hitbox
auto Spell::SetHitBox( bool hitbox ) -> void
{
    this->hitbox = hitbox;
}

// Set the skill values
auto Spell::SetSkillShot( float delay, float speed, float radius, bool hitbox, PredictionCollisionFlags flags ) -> void
{
    this->delay = delay;
    this->speed = speed;
    this->radius = radius;
    this->hitbox = hitbox;
    this->flags = flags;
}

// Run prediction from the player
auto Spell::RunPrediction( GameObject* unit, PredictionOutput& output ) const -> bool
{
    if ( !unit ) return false;
    
    const PredictionInput input
    {
        .m_pSource = GetPlayer( ),
        .m_vecSourcePosition = GetPlayer(  )->Position(  ),
        .m_bHitBox = this->hitbox,
        .m_flRange = this->range,
        .m_flDelay = this->delay,
        .m_flRadius = this->radius,
        .m_flSpeed = this->speed,
        .m_iFlags = this->flags,
    };
    
    return g_pExportedPrediction->GetPrediction( unit, input, output );
}

// Run prediction from a given position
auto Spell::RunPrediction( GameObject* from, GameObject* unit, PredictionOutput& output ) const -> bool
{
    if ( !unit ) return false;
    if ( !from ) return false;
    
    const PredictionInput input
    {
        .m_pSource = from,
        .m_vecSourcePosition = from->Position(  ),
        .m_bHitBox = this->hitbox,
        .m_flRange = this->range,
        .m_flDelay = this->delay,
        .m_flRadius = this->radius,
        .m_flSpeed = this->speed,
        .m_iFlags = this->flags,
    };
    
    return g_pExportedPrediction->GetPrediction( unit, input, output );
}

// Cast the spell
auto Spell::Cast( ) -> void
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time <= .25f )
        return;
    
    const auto player = GetPlayer( );
    if ( const auto spellbook = player->Spellbook( ) )
    {
        auto position = player->Position( );
        this->time = g_pExportedGlobalClocks->GameTime( );
        spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr );
    }
}

// Cast the spell on the target
auto Spell::Cast( GameObject* target ) -> void
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time <= .25f )
        return;
    
    const auto player = GetPlayer( );
    if ( const auto spellbook = player->Spellbook( ) )
    {
        auto position = target->Position( );
        this->time = g_pExportedGlobalClocks->GameTime( );
        spellbook->SendSpellCastPacket( this->slot, &position, &position, target );
    }
}

// Cast the spell at the given position
auto Spell::Cast( Vector3 position ) -> void
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time <= .25f )
        return;
    
    const auto player = GetPlayer( );
    if ( const auto spellbook = player->Spellbook( ) )
    {
        this->time = g_pExportedGlobalClocks->GameTime( );
        spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr );
    }
}

// Check if the spell is ready to be casted
auto Spell::IsReady( const float t ) const -> bool
{
    const auto spellbook = GetPlayer( )->Spellbook( );
    if ( !spellbook ) return false;

    const auto spell = spellbook->GetSpell( this->slot );
    if ( !spell || spell->Level( ) <= 0 ) return false;

    if ( spell->GetRemainingCooldown( ) > 0 && spell->GetRemainingCooldown( ) <= t ) return true;
    return spellbook->CanUseSpell( this->slot );
}

