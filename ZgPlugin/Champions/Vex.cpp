#include "../stdafx.hpp"

namespace Vex
{
    void OnBoot( )
    {
        SetupMenu( );

        VexQ = new Spell( Q, 550 );
        VexQ->SetSkillShot( 0.15f, 600.0f, 180.0f, true, PredictionCollisionFlags::Windwall );

        VexX = new Spell( Q, 1200 );
        VexX->SetSkillShot( 0.40f, 3200.0f, 180.0f, true, PredictionCollisionFlags::Windwall );

        VexW = new Spell( W, 475 );
        
        VexE = new Spell( E, 800 );
        VexE->SetSkillShot( 0.25f, 1300.0f, 200.0f, true, PredictionCollisionFlags::None);
        
        VexR = new Spell( R );
        VexR->SetSkillShot( 0.25f, 1600.0f, 100.0f, true, PredictionCollisionFlags::Heroes | PredictionCollisionFlags::Windwall);

        if (GetPlayer(  )->Spellbook(  )->GetSpell( Summoner1 )->SpellData(  )->Hash(  ) == FNV1A32CI("SummonerDot"))
            Ignite = new Spell ( Summoner1, 600);
        
        if (GetPlayer(  )->Spellbook(  )->GetSpell( Summoner2 )->SpellData(  )->Hash(  ) == FNV1A32CI("SummonerDot"))
            Ignite = new Spell ( Summoner2, 600);

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
        for (auto i : g_pExportedEntityManager->Heroes(  ))
        {
            if (i->IsAttackable( true, false ) && i->IsVisibleOnScreen(  ))
            {
                Vector3 wts;
                g_pExportedRenderer->WorldToScreen( i->Position(  ), &wts );
                
                const auto dmg  = PDmg( i ) + QDmg( i ) + WDmg( i ) + EDmg( i ) + RDmg( i, true ) + IgniteDmg( );
                auto pct = dmg/ i->MaxHealth(  ) * 100;
                
                char buffer[64];
                Globals::Sprintf( buffer, "%2.f%s",  pct, "%");
        
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, buffer, 15, wts.x + 1, wts.y + 1, RGBA( 0, 0, 0, 255 ) );
                g_pExportedRenderer->GetCurrentDrawList( )->AddTextA( nullptr, buffer, 15, wts.x, wts.y, RGBA( 255, 20, 255, 255 ) );
            }
        }
    }

    void OnUpdate( )
    {
        VexR->SetRange( VexR->Level(  ) * 500 + 1500 );
        Menu::SemiR->m_bEnabled = Menu::SemiR->Enabled(  ) && !Menu::SemiR->IsBeingHeldDown(  ) ? false : true;
        
        Auto();
        Combo( );
        Harass( );
    }

    void Auto( )
    {
        if ( Utils->CountEnemiesInRange( GetPlayer(  ), VexW->Range(  )) >= Menu::AutoW->Value(  ) )  
        {
            if ( Menu::UseW->Enabled(  ) &&  VexW->IsReady(  ) )
            {
                VexW->Cast();
            }
        }

        if ( Menu::SemiR->IsBeingHeldDown(  ) )
        {
            UseR( g_pExportedTargetSelector->GetTarget( VexR->Range(  ), true ), Menu::SemixR->Value(  ), true);
        }
    }

    void Combo( )
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled( ) )
        {
            UseR( g_pExportedTargetSelector->GetTarget( VexR->Range(  ), true ), Menu::HxR->Value(  ));
            
            if (VexR->Hash(  ) != FNV1A32CI( "VexR2" ) && g_pExportedGlobalClocks->GameTime(  ) - VexR->LastCastTime(  ) > 1.1f)
            {
                UseE( g_pExportedTargetSelector->GetTarget( VexE->Range( ), true ), Menu::HxComboE->Value(  ) );
                UseQ( g_pExportedTargetSelector->GetTarget( VexX->Range( ), true ), Menu::HxComboQ->Value(  ) );
                UseW( g_pExportedTargetSelector->GetTarget( VexW->Range( ), true ));
                UseIgnite(  g_pExportedTargetSelector->GetTarget( VexX->Range( ) ) );
            }
        }
    }

    void Harass()
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::Harass )->Enabled( ) )
        {
            UseE( g_pExportedTargetSelector->GetTarget( VexE->Range( ), true ), Menu::HxHarassE->Value(  ) );
            UseQ( g_pExportedTargetSelector->GetTarget( VexX->Range( ), true ), Menu::HxHarassQ->Value(  ) );
        }
    }

    void Clear()
    {
        if ( g_pExportedOrbwalker->GetMode( OrbwalkerMode::LaneClear )->Enabled( ) )
        {
            
        }
    }

    void UseQ( GameObject* unit, float chance )
    {
        if ( VexQ->IsReady( ) && Menu::UseQ->Enabled( ) && unit != nullptr )
        {
            if ( Utils->IsValidTarget( unit, VexQ->Range(  ) ) )
            {
                PredictionOutput output;
                VexQ->RunPrediction( unit, output );
                
                if ( output.m_flHitChance >= chance )
                {
                    VexQ->Cast( output.m_vecCastPosition );
                }
            }
            
            if ( Utils->IsValidTarget( unit, VexX->Range(  ) ) )
            {
                PredictionOutput output;
                VexX->RunPrediction( unit, output );
                
                if ( output.m_flHitChance >= chance )
                {
                    VexQ->Cast( output.m_vecCastPosition );
                }
            }
        }
    }

    void UseW( GameObject* unit )
    {
        if ( unit != nullptr &&  VexW->IsReady( ) && Menu::UseW->Enabled( ) )
        {
            if (Utils->IsValidTarget( unit, VexW->Range(  ) ))
            {
                VexW->Cast();
            }
        }
    }

    void UseE( GameObject* unit, float chance )
    {
        if ( VexE->IsReady( ) && Menu::UseE->Enabled( ) && unit != nullptr )
        {
            if ( Utils->IsValidTarget( unit, VexE->Range(  ) ) )
            {
                PredictionOutput output;
                VexE->RunPrediction( unit, output );
                
                if ( output.m_flHitChance >= chance )
                {
                    VexE->Cast( output.m_vecCastPosition );
                }
            }
        }
    }

    void UseR( GameObject* unit, float chance, bool semi )
    {
        if ( unit != nullptr && VexR->IsReady( ) && Menu::UseR->Enabled( ) ) 
        {
            if ( Utils->IsValidTarget( unit, VexR->Range(  ) ) )
            {
                if ( VexR->Hash( ) == FNV1A32CI( "VexR" ) )
                {
                    if (CanPreExecute( unit ) || semi )
                    {
                        PredictionOutput output;
                        VexR->RunPrediction( unit, output );
                    
                        if ( output.m_flHitChance >= chance )
                        {
                            VexR->Cast( output.m_vecCastPosition );
                        }
                    }
                }
                else
                {
                    if (CanPreExecute( unit ) || g_pExportedOrbwalker->GetMode( OrbwalkerMode::Combo )->Enabled(  ) || semi )
                    {
                        VexR->Cast();
                    }
                }
            }
        }
    }

    void UseIgnite( GameObject* unit )
    {
        if ( Ignite != nullptr && Ignite->IsReady(  ) && Menu::Ignite->Enabled(  ) )
        {
            if ( unit != nullptr && Utils->IsValidTarget( unit, 600 ) )
            {
                if ( CanPreExecute( unit ) )
                {
                    Ignite->Cast( unit );
                }
            }
        }
    }

    bool CanPreExecute( GameObject* unit )
    {
        if (unit == nullptr)
            return false;
        
        const auto dmg  = PDmg( unit ) + QDmg( unit ) + WDmg( unit ) + EDmg( unit ) + RDmg( unit, true ) + IgniteDmg();
        return dmg >= unit->Health(  );
    }

    float PDmg( GameObject * unit )
    {
        // base damage values at different levels
        const double base = 30 + 10 * (GetPlayer(  )->Experience(  )->Level(  ) - 1); // Damage scales with level

        const auto dmg = base + GetPlayer(  )->TotalAbilityDamage(  ) * 0.2;
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float QDmg( GameObject* unit )
    {
        if (unit == nullptr || !VexQ->IsReady(  ))
            return 0;
        
        // base damage values for each rank
        const double base_damage[] = { 70, 115, 160, 205, 250 };
        
        const auto dmg = base_damage[VexQ->Level(  ) - 1] + GetPlayer(  )->TotalAbilityDamage(  ) * 0.7;
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float WDmg( GameObject* unit )
    {
        if (unit == nullptr || !VexW->IsReady(  ))
                return 0;
        
        // base damage values for each rank
        const double base_damage[] = { 80, 120, 160, 200, 240 };
        
        const auto dmg = base_damage[VexW->Level(  ) - 1] + GetPlayer(  )->TotalAbilityDamage(  ) * 0.3;
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float EDmg( GameObject* unit )
    {
        if (unit == nullptr || !VexE->IsReady(  ))
            return 0;
        
        // base damage values for each rank
        const double base_damage[] = { 50, 70, 90, 110, 130 };
        constexpr double ap_scaling[] = { 0.4, 0.45, 0.5, 0.55, 0.6 };
        
        const auto dmg = base_damage[VexE->Level(  ) - 1] + GetPlayer(  )->TotalAbilityDamage(  ) * ap_scaling[VexE->Level(  ) - 1];
        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float RDmg( GameObject * unit, bool recast )
    {
        if (unit == nullptr || !VexR->IsReady(  ))
            return 0;
        
        float dmg = 0;
        const int first_base_damage[] = { 75, 125, 175 };
        const int second_base_damage[] = { 150, 250, 350 };

        dmg += first_base_damage[VexR->Level(  ) - 1] + GetPlayer(  )->TotalAbilityDamage(  ) * 0.2;
        
        if (recast)
            dmg += second_base_damage[VexR->Level(  ) - 1] + GetPlayer(  )->TotalAbilityDamage(  ) * 0.5;

        return g_pExportedDamage->CalculateMagicDamage( GetPlayer( ), unit, dmg ).m_flTotal;
    }

    float IgniteDmg()
    {
        if ( Ignite != nullptr && Ignite->IsReady(  ) && Menu::Ignite->Enabled(  ) )
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
        Menu::Root = g_pExportedMenu->AddMenu( "EzSeries - Vex", MenuConfig( "EzSeries" ) );
        Menu::Root->AddSeparator( MenuString( "Spells" ) );
        
        const auto q_menu = Menu::Root->AddMenu( MenuString( "(Q) Mistral Bolt " ), MenuConfig( "MistralBolt" ) );
        Menu::UseQ = q_menu->AddCheckbox( MenuString( "Use (Q)" ), MenuConfig( "vex.use.q" ), true );
        Menu::HxComboQ = q_menu->AddSlider( MenuString( "- Combo Hitchance" ), MenuConfig( "vex.q.combo.chance" ), 0.0, 1.00, 0.12, 2, .01 );
        Menu::HxHarassQ = q_menu->AddSlider( MenuString( "- Harass Hitchance" ), MenuConfig( "vex.q.harass.chance" ), 0.0, 1.00, 0.20, 2, .01 );
        Menu::DrawQ = q_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.q" ), true );
        
        const auto w_menu = Menu::Root->AddMenu( MenuString( "(W) Personal Space " ), MenuConfig( "PersonalSpace" ) );
        Menu::UseW = w_menu->AddCheckbox( MenuString( "Use (W)" ), MenuConfig( "vex.use.w" ), true );
        Menu::AutoW = w_menu->AddSlider( MenuString( "- Auto use if # in Range" ), MenuConfig( "vex.auto.w" ), 1, 5, 2, 0, 1 );
        Menu::DrawW = w_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.w" ), true );

        const auto e_menu = Menu::Root->AddMenu( MenuString( "(E) Looming Darkness" ), MenuConfig( "LoomingDarkness" ) );
        Menu::UseE = e_menu->AddCheckbox( MenuString( "Use (E)" ), MenuConfig( "vex.use.e" ), true );
        Menu::HxComboE = e_menu->AddSlider( MenuString( "- Combo Hitchance" ), MenuConfig( "vex.e.combo.chance" ), 0.0, 1.00, 0.12, 2, .01 );
        Menu::HxHarassE = e_menu->AddSlider( MenuString( "- Harass Hitchance" ), MenuConfig( "vex.e.harass.chance" ), 0.0, 1.00, 0.20, 2, .01 );
        Menu::DrawE = e_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.e" ), true );

        const auto r_menu = Menu::Root->AddMenu( MenuString( "(R) Shadow Surge " ), MenuConfig( "ShadowSurge" ) );
        Menu::UseR = r_menu->AddCheckbox( MenuString( "Use (R)" ), MenuConfig( "vex.use.r" ), true );
        Menu::HxR = r_menu->AddSlider( MenuString( "- Combo Hitchance" ), MenuConfig( "vex.r.chance" ), 0.0, 1.00, 0.20, 2, .01 );
        Menu::DrawR = r_menu->AddCheckbox( MenuString( "- Draw" ), MenuConfig( "vex.draw.r" ), true );
        Menu::SemiR = r_menu->AddKeybind( MenuString( "Semi Cast" ), MenuConfig( "vex.semi.r" ), 'X', true  );
        Menu::SemixR = r_menu->AddSlider( MenuString( "- Semi Hitchance" ), MenuConfig( "vex.semi.r.chance" ), 0.0, 1.00, 0.00, 2, .01 );

        Menu::Root->AddSeparator( MenuString( "Misc" ) );
        Menu::Ignite = Menu::Root->AddCheckbox( MenuString( "Use Ignite" ), MenuConfig( "vex.ignite"), true );
        
        Menu::Root->AddSeparator( MenuString( "EzSeries v0.54" ) );
    }
}
