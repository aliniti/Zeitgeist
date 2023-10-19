#include "../stdafx.hpp"

Spell::Spell( SpellSlot slot )
{
    this->slot = slot;
    this->time = 0;
    this->range = 999999.f;
}

Spell::Spell( SpellSlot slot, float range )
{
    this->slot = slot;
    this->range = range;
    this->time = 0;
}

auto Spell::SetSkillShot( float delay, float speed, float radius, bool hitbox, PredictionCollisionFlags flags ) -> void
{
    this->delay = delay;
    this->speed = speed;
    this->radius = radius;
    this->hitbox = hitbox;
    this->flags = flags;
}

auto Spell::Range( ) const -> float
{
    return this->range;
}

auto Spell::Radius( ) const -> float
{
    return this->radius;
}

auto Spell::Level( ) const -> int
{
    return GetPlayer(  )->Spellbook(  )->GetSpell( this->slot )->Level(  );
}

auto Spell::Slot( ) const -> SpellSlot
{
    return this->slot;
}

auto Spell::LastCastTime( ) const -> float
{
    return this->time;
}

auto Spell::Hash( ) const -> uint32_t
{
    return GetPlayer(  )->Spellbook(  )->GetSpell( this->slot )->SpellData(  )->Hash(  );
}

auto Spell::IsReady( const float t ) const -> bool
{
    const auto spellbook = GetPlayer( )->Spellbook( );
    if ( !spellbook ) return false;

    const auto spell = spellbook->GetSpell( this->slot );
    if ( !spell || spell->Level( ) <= 0 ) return false;

    if ( spell->GetRemainingCooldown( ) > 0 && spell->GetRemainingCooldown( ) <= t ) return true;
    return spellbook->CanUseSpell( this->slot );
}

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

void Spell::SetFrom( Vector3 pos )
{
    this->from = pos;
}

auto Spell::SetRange( float range ) -> void
{
    this->range = range;
}

auto Spell::SetDelay( float delay ) -> void
{
    this->delay = delay;
}
auto Spell::SetSpeed( float speed ) -> void
{
    this->speed = speed;
}

auto Spell::SetRadius( float radius ) -> void
{
    this->radius = radius;
}

auto Spell::SetHitBox( bool hitbox ) -> void
{
    this->hitbox = hitbox;
}

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

auto Spell::HitBox( ) -> bool
{
    return this->hitbox;
}
