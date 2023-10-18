#pragma once

namespace Vex
{
    void OnBoot();
    void OnTerminate();
    void OnDraw();
    void OnPresentDraw();
    void OnUpdate( );
    
    void Auto( );
    void Combo( );
    void Clear( );
    void Harass( );
    
    void UseQ( GameObject* unit, float chance );
    void UseX( GameObject* unit, float chance );
    void UseW( GameObject* unit );
    void UseE( GameObject* unit, float chance );
    void UseR( GameObject* unit, float chance, bool semi = false );
    void UseIgnite( GameObject* unit ); 

    bool CanPreExecute( GameObject* unit );
    float PDmg( GameObject * unit);
    float QDmg( GameObject* unit );
    float WDmg( GameObject* unit );
    float EDmg( GameObject* unit );
    float RDmg( GameObject * unit );
    float IgniteDmg( );
    
    void SetupMenu( );

    inline Spell* VexQ;
    inline Spell* VexX;
    inline Spell* VexW;
    inline Spell* VexE;
    inline Spell* VexR;
    inline Spell* Ignite = nullptr;

    namespace Menu
    {
        inline MenuElement* Root;
        inline MenuElement* HxComboQ;
        inline MenuElement* HxHarassQ;
        inline MenuElement* UseQ;
        inline MenuElement* ClearQ;
        inline MenuElement* DrawQ;
        inline MenuElement* UseW;
        inline MenuElement* DrawW;
        inline MenuElement* UseE;
        inline MenuElement* HxComboE;
        inline MenuElement* HxHarassE;
        inline MenuElement* DrawE;
        inline MenuElement* HxR;
        inline MenuElement* UseR;
        inline MenuElement* DrawR;
        inline MenuElement* AutoW;
        inline MenuElement* SemiR;
        inline MenuElement* HxSemiR;
        inline MenuElement* Ignite;
        inline MenuElement* UseExtendedQ;
    }
};
