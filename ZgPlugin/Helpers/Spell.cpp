#include "../stdafx.hpp"

Spell::Spell( SpellSlot slot, float range )
{
    this->slot = slot;
    this->range = range;
    this->time = 0;
    this->from = GetPlayer(  )->Position(  );
}

void Spell::SetSkillShot( float delay, float speed, float radius, bool hitbox, PredictionCollisionFlags collision )
{
    this->delay = delay;
    this->speed = speed;
    this->radius = radius;
    this->hitbox = hitbox;
    this->collision = collision;
}

float Spell::Range( ) const
{
    return this->range;
}

SpellSlot Spell::Slot( ) const
{
    return this->slot;
}

float Spell::LastCastTime( ) const
{
    return this->time;
}

bool Spell::IsReady( const float time ) const
{
    const auto spellbook = GetPlayer( )->Spellbook( );
    if ( !spellbook ) return false;

    const auto spell = spellbook->GetSpell( this->slot );
    if ( !spell || spell->Level( ) <= 0 ) return false;

    if ( spell->GetRemainingCooldown( ) > 0 && spell->GetRemainingCooldown( ) <= time ) return true;
    return spellbook->CanUseSpell( this->slot );
}

void Spell::Cast( )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time > .25f )
    {
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
}

void Spell::Cast( GameObject* target )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time > .25f )
    {
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
}

void Spell::Cast( Vector3 position )
{
    if ( g_pExportedGlobalClocks->GameTime( ) - this->time > .25f )
    {
        const auto player = GetPlayer( );
        if ( const auto spellbook = player->Spellbook( ) )
        {
            if ( spellbook->SendSpellCastPacket( this->slot, &position, &position, nullptr ) )
            {
                this->time = g_pExportedGlobalClocks->GameTime( );
            }
        }
    }
}

void Spell::SetFrom( Vector3 pos )
{
    this->from = pos;
}

bool Spell::RunPrediction( GameObject* pObject, PredictionOutput & output ) const
{
    if ( !pObject ) return false;

    const PredictionInput input
    {
        .m_pSource = nullptr,
        .m_vecSourcePosition = this->from,
        .m_bHitBox = this->hitbox,
        .m_flRange = this->range,
        .m_flDelay = this->delay,
        .m_flRadius = this->radius,
        .m_flSpeed = this->speed,
        .m_iFlags = this->collision,
    };
    
    return g_pExportedPrediction->GetPrediction(pObject, input, output);
}

PredictionOutput Spell::RunPrediction( GameObject* pObject ) const
{
    PredictionOutput output;
    if ( !pObject ) return output;
    
    PredictionInput input
    {
        .m_pSource = pObject,
        .m_vecSourcePosition = pObject->Position(  ),
        .m_bHitBox = this->hitbox,
        .m_flRange = this->range,
        .m_flDelay = this->delay,
        .m_flRadius = this->radius,
        .m_flSpeed = this->speed,
        .m_iFlags = this->collision,
    };

    if (g_pExportedPrediction->GetPrediction(pObject, input, output))
    {
        return output;
    }
    
    return {};
}
