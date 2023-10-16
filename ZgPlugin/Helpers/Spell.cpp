#include "../stdafx.hpp"

Spell::Spell( SpellSlot slot )
{
    this->slot = slot;
    this->time = 0;
    this->from = GetPlayer(  )->Position(  );
}

Spell::Spell( SpellSlot slot, float range )
{
    this->slot = slot;
    this->range = range;
    this->time = 0;
    this->from = GetPlayer(  )->Position(  );
}

void Spell::SetSkillShot( float delay, float speed, float radius, bool hitbox, PredictionCollisionFlags flags )
{
    this->delay = delay;
    this->speed = speed;
    this->radius = radius;
    this->hitbox = hitbox;
    this->flags = flags;
}

float Spell::Range( ) const
{
    return this->range;
}

auto Spell::Level( ) const -> int
{
    return GetPlayer(  )->Spellbook(  )->GetSpell( this->slot )->Level(  );
}

SpellSlot Spell::Slot( ) const
{
    return this->slot;
}

float Spell::LastCastTime( ) const
{
    return this->time;
}

uint32_t Spell::Hash( ) const
{
    return GetPlayer(  )->Spellbook(  )->GetSpell( this->slot )->SpellData(  )->Hash(  );
}

bool Spell::IsReady( const float t ) const
{
    const auto spellbook = GetPlayer( )->Spellbook( );
    if ( !spellbook ) return false;

    const auto spell = spellbook->GetSpell( this->slot );
    if ( !spell || spell->Level( ) <= 0 ) return false;

    if ( spell->GetRemainingCooldown( ) > 0 && spell->GetRemainingCooldown( ) <= t ) return true;
    return spellbook->CanUseSpell( this->slot );
}

void Spell::Cast( )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time <= .25f )
        return;
    
    const auto player = GetPlayer( );
    if ( const auto spellbook = player->Spellbook( ) )
    {
        auto position = player->Position( );
        if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr ) )
        {
            this->time = g_pExportedGlobalClocks->GameTime( );
        }
    }
}

void Spell::Cast( GameObject* target )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time <= .25f )
        return;
    
    const auto player = GetPlayer( );
    if ( const auto spellbook = player->Spellbook( ) )
    {
        auto position = target->Position( );
        if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, target ) )
        {
            this->time = g_pExportedGlobalClocks->GameTime( );
        }
    }
}

void Spell::Cast( Vector3 position )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time <= .25f )
        return;
    
    const auto player = GetPlayer( );
    if ( const auto spellbook = player->Spellbook( ) )
    {
        if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr ) )
        {
            this->time = g_pExportedGlobalClocks->GameTime( );
        }
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

void Spell::RunPrediction( GameObject* pObject, PredictionOutput & output ) const
{
    if ( pObject )
    {
        const PredictionInput input
        {
            .m_pSource = GetPlayer( ),
            .m_vecSourcePosition = this->from,
            .m_bHitBox = this->hitbox,
            .m_flRange = this->range,
            .m_flDelay = this->delay,
            .m_flRadius = this->radius,
            .m_flSpeed = this->speed,
            .m_iFlags = this->flags,
        };

        g_pExportedPrediction->GetPrediction( pObject, input, output );
    }
}
