#include "../stdafx.hpp"

namespace Vex
{
    void OnBoot( )
    {
        SetupMenu( );

        VexQ = new Spell( Q, 550.f );
        VexQ->SetSkillShot( 0.25f, 600.f, 180.f, true, PredictionCollisionFlags::Windwall );
        VexQ->SetHitBox( true );

        VexX = new Spell( Q, 1200.f );
        VexX->SetSkillShot( 0.80f, 3200.f, 100.f, true, PredictionCollisionFlags::Windwall );
        VexX->SetHitBox( true );

        VexW = new Spell( W, 475.f );

        VexE = new Spell( E, 1000.f);
        VexE->SetSkillShot( 1.1f, 0.f, 200.f, false, PredictionCollisionFlags::None );

        VexR = new Spell( R );
        VexR->SetSkillShot( 0.25f, 1600.f, 100.f, true, PredictionCollisionFlags::Heroes | PredictionCollisionFlags::Windwall );
        VexR->SetHitBox( true );

        if ( GetPlayer( )->Spellbook( )->GetSpell( Summoner1 )->SpellData( )->Hash( ) == FNV1A32CI( "SummonerDot" ) )
            Ignite = new Spell( Summoner1, 600.f );

        if ( GetPlayer( )->Spellbook( )->GetSpell( Summoner2 )->SpellData( )->Hash( ) == FNV1A32CI( "SummonerDot" ) )
            Ignite = new Spell( Summoner2, 600.f );

        Globals::Write( "Vex Loaded!\n" );
    }

    void OnTerminate( )
    {
        Menu::Root->Release( );
    }

    void OnDraw( )
    {
        if ( !GetPlayer( )->IsDead( ) )
        {
            const auto q_opa = GetPlayer( )->Spellbook( )->CanUseSpell( Q ) ? 255 : 25;
            if ( Menu::DrawQ->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( Q )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexQ->Range( ), 6, RGBA( 0, 0, 0, q_opa ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexQ->Range( ), 3, RGBA( 255, 20, 255, q_opa ) );
            }
            
            if ( Menu::DrawX->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( Q )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexX->Range( ), 6, RGBA( 0, 0, 0, q_opa ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexX->Range( ), 3, RGBA( 255, 20, 255, q_opa ) );
            }

            const auto w_opa = GetPlayer( )->Spellbook( )->CanUseSpell( W ) ? 255 : 25;
            if ( Menu::DrawW->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( W )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexW->Range( ), 6, RGBA( 0, 0, 0, w_opa ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexW->Range( ), 3, RGBA( 255, 20, 255, w_opa ) );
            }

            const auto e_opa = GetPlayer( )->Spellbook( )->CanUseSpell( E ) ? 255 : 25;
            if ( Menu::DrawE->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( E )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexE->Range( ), 6, RGBA( 0, 0, 0, e_opa ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexE->Range( ), 3, RGBA( 255, 20, 255, e_opa ) );
            }

            const auto r_opa = GetPlayer( )->Spellbook( )->CanUseSpell( R ) ? 255 : 25;
            if ( Menu::DrawR->Enabled( ) && GetPlayer( )->Spellbook( )->GetSpell( R )->Level( ) > 0 )
            {
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexR->Range( ), 6, RGBA( 0, 0, 0, r_opa ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddCircle( GetPlayer( )->Position( ), VexR->Range( ), 3, RGBA( 255, 255, 255, r_opa ) );
            }
        }
    }

    void OnPresentDraw( )
    {
        for ( auto i : g_pExportedEntityManager->Heroes( ) )
        {
            if ( i->IsAttackable( true, false ) && i->IsVisibleOnScreen( ) )
            {
                Vector3 wts;
                g_pExportedRenderer->WorldToScreen( i->Position( ), &wts );

                const auto dmg = PDmg( i ) + QDmg( i ) + WDmg( i ) + EDmg( i ) + RDmg( i ) + IgniteDmg( );
                auto pct = dmg / i->Health( ) * 100;

                char buffer[64];
                Globals::Sprintf( buffer, "%2.f%s", pct, "%" );

                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, buffer, 15, wts.x + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, buffer, 15, wts.x, wts.y, RGBA( 255, 20, 255, 255 ) );
            }
        }
    }

    void OnUpdate( )
    {
        VexR->SetRange( VexR->Level( ) * 500 + 1500 );
        Menu::SemiR->m_bEnabled = Menu::SemiR->Enabled( ) && !Menu::SemiR->IsBeingHeldDown( ) ? false : true;

        Auto( );
        Combo( );
        Clear();
        Harass( );
    }

    void Auto( )
    {
        VexQ->SetDelay( Debug::QDelay->Value(  ) );
        VexQ->SetSpeed( Debug::QSpeed->Value(  ) );
        VexQ->SetRadius( Debug::QRadius->Value(  ) );
        VexQ->SetRange( Debug::QRange->Value(  ) );
        VexQ->SetHitBox( Debug::QHitBox->Enabled(  ) );

        VexX->SetDelay( Debug::Q2Delay->Value(  ) );
        VexX->SetSpeed( Debug::Q2Speed->Value(  ) );
        VexX->SetRadius( Debug::Q2Radius->Value(  ) );
        VexX->SetRange( Debug::Q2Range->Value(  ) );
        VexX->SetHitBox( Debug::Q2HitBox->Enabled(  ) );

        VexE->SetDelay( Debug::EDelay->Value(  ) );
        VexE->SetSpeed( Debug::ESpeed->Value(  ) );
        VexE->SetRadius( Debug::ERadius->Value(  ) );
        VexE->SetRange( Debug::ERange->Value(  ) );
        VexE->SetHitBox( Debug::EHitBox->Enabled(  ) );

        VexR->SetDelay( Debug::RDelay->Value(  ) );
        VexR->SetSpeed( Debug::RSpeed->Value(  ) );
        VexR->SetRadius( Debug::RRadius->Value(  ) );
        VexR->SetRange( Debug::RRange->Value(  ) );
        VexR->SetHitBox( Debug::RHitBox->Enabled(  ) );
        
        if ( GUtils->CountEnemiesInRange( VexW->Range( ) ) >= Menu::AutoW->Value( ) )
        {
            if ( Menu::UseW->Enabled( ) && VexW->IsReady( ) )
            {
                VexW->Cast( );
            }
        }

        if ( Menu::SemiR->IsBeingHeldDown( ) )
        {
            UseR( g_pExportedTargetSelector->GetTarget( VexR->Range( ), true ), Menu::HxSemiR->Value( ), true );
        }
    }

    void Combo( )
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) )
        {
            UseW( g_pExportedTargetSelector->GetTarget( VexW->Range( ), VexW->HitBox(  ) ) );
            UseR( g_pExportedTargetSelector->GetTarget( VexR->Range( ), VexR->HitBox(  ) ), Debug::RChance->Value(  ) );
            UseE( g_pExportedTargetSelector->GetTarget( VexE->Range( ), VexE->HitBox(  ) ), Debug::EChance->Value(  ) );
            UseQ( g_pExportedTargetSelector->GetTarget( VexQ->Range( ), VexQ->HitBox(  ) ), Debug::QChance->Value(  ) );
            UseX( g_pExportedTargetSelector->GetTarget( VexX->Range( ), VexX->HitBox(  ) ), Debug::Q2Chance->Value(  ) );
            UseIgnite( g_pExportedTargetSelector->GetTarget( VexX->Range( ) ) );
        }
    }

    void Harass( )
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Harass )->Enabled( ) )
        {
            if ( GetPlayer( )->PAR( ) / GetPlayer( )->MaxPAR( ) * 100 > 55 )
            {
                UseE( g_pExportedTargetSelector->GetTarget( VexE->Range( ), VexE->HitBox(  ) ), 0 );
                UseQ( g_pExportedTargetSelector->GetTarget( VexQ->Range( ), VexQ->HitBox(  ) ), 1.00 );
                UseX( g_pExportedTargetSelector->GetTarget( VexX->Range( ), VexX->HitBox(  ) ), 1.00 );
            }
        }
    }

    void Clear( )
    {
        if (g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled(  ) )
        {
            auto has_mana = GetPlayer(  )->PAR(  ) / GetPlayer(  )->MaxPAR(  ) * 100 > 75;
            auto should = has_mana || GUtils->CountEnemiesInRange( VexX->Range(  ) + 1000 ) < 1;
        
            if (VexX->IsReady(  ) && Menu::ClearQ->Enabled(  ) && should )
            {
                Vector<Vector3> minions; minions.clear(  );
                for (auto i : g_pExportedEntityManager->Minions(  ))
                    if (GUtils->IsValidTarget( i, VexX->Range(  ) ) && QDmg( i ) >= i->Health(  ) )
                        minions.push_back( i->Position(  ) );

                Vector3 center; float radius;
                g_pExportedMath->CalculateMinimumEnclosingCircle( minions,  center, radius);
                
                if ( center.IsValid(  ) && radius > 25 )
                {
                    VexX->Cast(center);
                }
            }
        }
    }

    void UseQ( GameObject* unit, float chance )
    {
        if ( HoldSkills( unit ) )
            return;
        
        if ( VexQ->IsReady( ) && Menu::UseQ->Enabled( ) && unit != nullptr )
        {
            if ( GUtils->IsValidTarget( unit ) )
            {
                if ( PredictionOutput output; VexQ->RunPrediction( unit, output ) )
                {
                    if ( output.m_flHitChance >= chance  )
                    {
                        VexQ->Cast( output.m_vecCastPosition );
                    }
                }
            }
        }
    }

    void UseX( GameObject* unit, float chance )
    {
        if ( HoldSkills( unit ) )
            return;
        
        if ( VexX->IsReady( ) && Menu::UseX->Enabled( ) && unit != nullptr )
        {
            if ( GUtils->IsValidTarget( unit ) )
            {
                if ( PredictionOutput output; VexX->RunPrediction( unit, output ) )
                {
                    if (GetPlayer(  )->DistanceXZ( output.m_vecCastPosition ) > VexQ->Range(  ))
                    {
                        if ( output.m_flHitChance >= chance )
                        {
                            if (GetPlayer(  )->DistanceXZ( output.m_vecCastPosition ) <= VexX->Range(  ))
                            {
                                VexX->Cast( output.m_vecCastPosition );
                            }
                        }
                    }
                }
            }
        }
    }

    void UseW( GameObject* unit )
    {
        if ( HoldSkills( unit ) )
            return;
        
        if ( unit != nullptr && VexW->IsReady( ) && Menu::UseW->Enabled( ) )
        {
            if ( GUtils->IsValidTarget( unit, VexW->Range( ) ) )
            {
                VexW->Cast( );
            }
        }
    }

    void UseE( GameObject* unit, float chance )
    {
        if ( HoldSkills( unit ) )
            return;
        
        if ( VexE->IsReady( ) && Menu::UseE->Enabled( ) && unit != nullptr )
        {
            if ( GUtils->IsValidTarget( unit ) )
            {
                if ( PredictionOutput output; VexE->RunPrediction( unit, output ) )
                {
                    if ( output.m_flHitChance >= chance )
                    {
                        if (GetPlayer(  )->DistanceXZ( output.m_vecCastPosition ) <= VexE->Range(  ))
                        {
                            VexE->Cast( output.m_vecCastPosition );
                        }
                    }
                }
            }
        }
    }

    void UseR( GameObject* unit, float chance, bool semi )
    {
        if ( HoldSkills( unit ) )
            return;
        
        if ( unit != nullptr && VexR->IsReady( ) && ( Menu::UseR->Enabled( ) || semi ) )
        {
            if ( GUtils->IsValidTarget( unit, VexR->Range( ) ) )
            {
                if ( VexR->Hash( ) == FNV1A32CI( "VexR" ) )
                {
                    if ( CanPreExecute( unit ) || semi )
                    {
                        if ( PredictionOutput output; VexR->RunPrediction( unit, output ) )
                        {
                            if ( output.m_flHitChance >= chance )
                            {
                                VexR->Cast( output.m_vecCastPosition );
                            }
                        }
                    }
                }
                else
                {
                    if ( CanPreExecute( unit ) || g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) || semi )
                    {
                        VexR->Cast( );
                    }
                }
            }
        }
    }

    void UseIgnite( GameObject* unit )
    {
        if ( HoldSkills( unit ) )
            return;
        
        if ( Ignite != nullptr && Ignite->IsReady( ) && Menu::Ignite->Enabled( ) )
        {
            if ( unit != nullptr && GUtils->IsValidTarget( unit, 600 ) )
            {
                if ( CanPreExecute( unit ) )
                {
                    Ignite->Cast( unit );
                }
            }
        }
    }

    bool HoldSkills( GameObject* unit )
    {
        if (!unit) return false;
        
        auto aa = g_pExportedDamage->GetAutoAttackDamage( GetPlayer(  ), unit, true ).m_flTotal;
        auto dist = GetPlayer(  )->DistanceXZ( unit );
        auto lethal = aa * 3 >= unit->Health(  );
        
        return  dist <= GetPlayer(  )->CharacterIntermediate(  )->AttackRange(  ) + 35 && lethal;
    }

    bool CanPreExecute( GameObject* unit )
    {
        if ( unit == nullptr )
            return false;

        const auto dmg = PDmg( unit ) + QDmg( unit ) + WDmg( unit ) + EDmg( unit ) + RDmg( unit ) + IgniteDmg( );
        return dmg >= unit->Health( );
    }

    float PDmg( GameObject* unit )
    {
        // base damage values at different levels
        const double base = 30 + 10 * ( GetPlayer( )->Experience( )->Level( ) - 1 ); // Damage scales with level

        const auto dmg = base + GetPlayer( )->TotalAbilityDamage( ) * 0.2;
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float QDmg( GameObject* unit )
    {
        if ( unit == nullptr || !VexQ->IsReady( ) )
            return 0;

        // base damage values for each rank
        const double base_damage[] = { 70, 115, 160, 205, 250 };

        const auto dmg = base_damage[VexQ->Level( ) - 1] + GetPlayer( )->TotalAbilityDamage( ) * 0.7;
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float WDmg( GameObject* unit )
    {
        if ( unit == nullptr || !VexW->IsReady( ) )
            return 0;

        // base damage values for each rank
        const double base_damage[] = { 80, 120, 160, 200, 240 };

        const auto dmg = base_damage[VexW->Level( ) - 1] + GetPlayer( )->TotalAbilityDamage( ) * 0.3;
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float EDmg( GameObject* unit )
    {
        if ( unit == nullptr || !VexE->IsReady( ) )
            return 0;

        // base damage values for each rank
        const double base_damage[] = { 50, 70, 90, 110, 130 };
        constexpr double ap_scaling[] = { 0.4, 0.45, 0.5, 0.55, 0.6 };

        const auto dmg = base_damage[VexE->Level( ) - 1] + GetPlayer( )->TotalAbilityDamage( ) * ap_scaling[VexE->Level( ) - 1];
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float RDmg( GameObject* unit )
    {
        if ( unit == nullptr || !VexR->IsReady( ) )
            return 0;

        float dmg = 0;
        constexpr int first_base_damage[] = { 75, 125, 175 };
        constexpr int second_base_damage[] = { 150, 250, 350 };

        dmg += first_base_damage[VexR->Level( ) - 1] + ( GetPlayer( )->TotalAbilityDamage( ) * 0.2 );
        dmg += second_base_damage[VexR->Level( ) - 1] + ( GetPlayer( )->TotalAbilityDamage( ) * 0.5 );

        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float IgniteDmg( )
    {
        if ( Ignite != nullptr && Ignite->IsReady( ) && Menu::Ignite->Enabled( ) )
        {
            // constants
            const auto lvl = GetPlayer( )->Experience( )->Level( );
            constexpr double base_value_min = 70.0;
            constexpr double base_value_max = 410.0;

            // calculate the value based on the level
            return base_value_min + ( base_value_max - base_value_min ) / 17.0 * ( lvl - 1 );
        }

        return 0;
    }

    void SetupMenu( )
    {
        Menu::Root = g_pExportedMenu->AddMenu( "EzVex", MenuConfig( "EzSeries" ) );

        // ------------- TEST
        auto test = Menu::Root->AddMenu( MenuString( "TEST" ), MenuConfig("TEST") );

        Debug::QChance = test->AddSlider( MenuString( "(Q) Hitchance" ), MenuConfig( "test.q.chance" ), 0.00, 1.00, 0.00, 2, .01 );
        Debug::QHitBox = test->AddCheckbox( MenuString( "(Q) Include HitBox" ), MenuConfig( "test.q.hitbox" ), true );
        Debug::QDelay = test->AddSlider( MenuString( "(Q) Delay" ), MenuConfig( "test.q.delay" ), 0.00, 2.00, 0.25, 2, .01 );
        Debug::QSpeed = test->AddSlider( MenuString( "(Q) Speed" ), MenuConfig( "test.q.speed" ), 0.00, 3200.00, 600.00, 2, .01 );
        Debug::QRadius = test->AddSlider( MenuString( "(Q) Radius" ), MenuConfig( "test.q.radius" ), 0.00, 500.00, 180.00, 2, .01 );
        Debug::QRange = test->AddSlider( MenuString( "(Q) Range" ), MenuConfig( "test.q.range" ), 0.00, 3000.00, 550.00, 2, .01 );
        test->AddSeparator( MenuString( "-----" ) );
        Debug::Q2Chance = test->AddSlider( MenuString( "(Q2) Hitchance" ), MenuConfig( "test.q2.chance" ), 0.00, 1.00, 0.00, 2, .01 );
        Debug::Q2HitBox = test->AddCheckbox( MenuString( "(Q2) Include HitBox" ), MenuConfig( "test.q2.hitbox" ), true );
        Debug::Q2Delay = test->AddSlider( MenuString( "(Q2) Delay" ), MenuConfig( "test.q2.delay" ), 0.00, 2.00, 0.45, 2, .01 );
        Debug::Q2Speed = test->AddSlider( MenuString( "(Q2) Speed" ), MenuConfig( "test.q2.speed" ), 0.00, 3200.00, 3200.00, 2, .01 );
        Debug::Q2Radius = test->AddSlider( MenuString( "(Q2) Radius" ), MenuConfig( "test.q2.radius" ), 0.00, 500.00, 180.00, 2, .01 );
        Debug::Q2Range = test->AddSlider( MenuString( "(Q2) Range" ), MenuConfig( "test.q2.range" ), 0.00, 3000.00, 1200.00, 2, .01 );
        test->AddSeparator( MenuString( "-----" ) );
        Debug::EChance = test->AddSlider( MenuString( "(E) Hitchance" ), MenuConfig( "test.e.chance" ), 0.00, 1.00, 0.00, 2, .01 );
        Debug::EHitBox = test->AddCheckbox( MenuString( "(E) Include HitBox" ), MenuConfig( "test.e.hitbox" ), true );
        Debug::EDelay = test->AddSlider( MenuString( "(E) Delay" ), MenuConfig( "test.e.delay" ), 0.00, 2.00, 0.25, 2, .01 );
        Debug::ESpeed = test->AddSlider( MenuString( "(E) Speed" ), MenuConfig( "test.e.speed" ), 0.00, 3200.00, 1300.00, 2, .01 );
        Debug::ERadius = test->AddSlider( MenuString( "(E) Radius" ), MenuConfig( "test.e.radius" ), 0.00, 500.00, 200.00, 2, .01 );
        Debug::ERange = test->AddSlider( MenuString( "(E) Range" ), MenuConfig( "test.e.range" ), 0.00, 3000.00, 800.00, 2, .01 );

        test->AddSeparator( MenuString( "-----" ) );
        Debug::RChance = test->AddSlider( MenuString( "(R) Hitchance" ), MenuConfig( "test.r.chance" ), 0.00, 1.00, 0.00, 2, .01 );
        Debug::RHitBox = test->AddCheckbox( MenuString( "(R) Include HitBox" ), MenuConfig( "test.r.hitbox" ), true );
        Debug::RDelay = test->AddSlider( MenuString( "(R) Delay" ), MenuConfig( "test.r.delay" ), 0.00, 2.00, 0.25, 2, .01 );
        Debug::RSpeed = test->AddSlider( MenuString( "(R) Speed" ), MenuConfig( "test.r.speed" ), 0.00, 3200.00, 1600.00, 2, .01 );
        Debug::RRadius = test->AddSlider( MenuString( "(R) Radius" ), MenuConfig( "test.r.radius" ), 0.00, 500.00, 130.00, 2, .01 );
        Debug::RRange = test->AddSlider( MenuString( "(R) Range" ), MenuConfig( "test.r.range" ), 0.00, 3000.00, 2000.00, 2, .01 );
        // -----------

        
        Menu::Root->AddSeparator( MenuString( "Spells" ) );

        const auto q_menu = Menu::Root->AddMenu( MenuString( "(Q) Mistral Bolt " ), MenuConfig( "MistralBolt" ) );
        Menu::UseQ = q_menu->AddCheckbox( MenuString( "Use (Q)" ), MenuConfig( "vex.use.q" ), true );
        Menu::ClearQ = q_menu->AddCheckbox( MenuString( "- Smart Clear" ), MenuConfig( "vex.use.q.clear" ), true );
        Menu::DrawQ = q_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.q" ), true );
        Menu::UseX = q_menu->AddCheckbox( MenuString( "Use Extended (Q)" ), MenuConfig( "vex.use.q.extended" ), true );
        Menu::DrawX = q_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.x" ), false );
        
        const auto w_menu = Menu::Root->AddMenu( MenuString( "(W) Personal Space " ), MenuConfig( "PersonalSpace" ) );
        Menu::UseW = w_menu->AddCheckbox( MenuString( "Use (W)" ), MenuConfig( "vex.use.w" ), true );
        Menu::AutoW = w_menu->AddSlider( MenuString( "- Auto use if # in range" ), MenuConfig( "vex.auto.w" ), 1, 5, 2, 0, 1 );
        Menu::DrawW = w_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.w" ), true );

        const auto e_menu = Menu::Root->AddMenu( MenuString( "(E) Looming Darkness" ), MenuConfig( "LoomingDarkness" ) );
        Menu::UseE = e_menu->AddCheckbox( MenuString( "Use (E)" ), MenuConfig( "vex.use.e" ), true );
        // Menu::HxComboE = e_menu->AddSlider( MenuString( "- Combo Hitchance" ), MenuConfig( "vex.e.combo.chance.x" ), 0.0, 1.00, 0.15, 2, .01 );
        // Menu::HxHarassE = e_menu->AddSlider( MenuString( "- Harass Hitchance" ), MenuConfig( "vex.e.harass.chance.x" ), 0.0, 1.00, 0.25, 2, .01 );
        Menu::DrawE = e_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.e" ), true );

        const auto r_menu = Menu::Root->AddMenu( MenuString( "(R) Shadow Surge " ), MenuConfig( "ShadowSurge" ) );
        Menu::UseR = r_menu->AddCheckbox( MenuString( "Use (R)" ), MenuConfig( "vex.use.r" ), true );
        //Menu::HxR = r_menu->AddSlider( MenuString( "- Hitchance" ), MenuConfig( "vex.r.chance" ), 0.0, 1.00, 0.25, 2, .01 );
        Menu::DrawR = r_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.r" ), true );
        Menu::SemiR = r_menu->AddKeybind( MenuString( "Semi Cast" ), MenuConfig( "vex.semi.r.key" ), 'T', true );
        Menu::HxSemiR = r_menu->AddSlider( MenuString( "- Hitchance" ), MenuConfig( "vex.semi.r.chance" ), 0.0, 1.00, 0.10, 2, .01 );

        Menu::Root->AddSeparator( MenuString( "Misc" ) );
        //Menu::HarassPct = Menu::Root->AddSlider( MenuString( "- Harass Min Mana (%)" ), MenuConfig( "vex.harass.mana" ), 0, 100, 65, 0, 1 );
        Menu::Ignite = Menu::Root->AddCheckbox( MenuString( "Use Ignite" ), MenuConfig( "vex.ignite" ), true );
        
        Menu::Root->AddSeparator( MenuString( "EzSeries v0.60" ) );
    }
}
