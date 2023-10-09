#include "stdafx.hpp"

// set to SDBM32CI("ChampionName") to load on a specific champion only
extern "C" inline __declspec(dllexport) std::uint32_t g_iUniqueAssemblyName = 0;

MenuElement* Config;

void OnIssueOrder(Vector3* pPosition, GameObject* pTarget, bool bWasPlayerIssued, bool* pCancel)
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Camille" ):
            return Camille::OnIssueOrder(pPosition, pTarget, bWasPlayerIssued, pCancel);
        default: ;
    }
}

void OnExecuteCastFrame(GameObject* pCaster, SpellCasterClient* pCasterClient)
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Camille" ):
            return Camille::OnExecuteCastFrame( pCaster, pCasterClient );
        default: ;
    }
}

void OnCreateParticle( GameObject* pObject, std::uint32_t iHash )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnCreateParticle( pObject, iHash );

        case SDBM32CI( "Camille" ):
            return Camille::OnCreateParticle( pObject, iHash );
        default: ;
    }
}

void OnDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnDraw( );

        case SDBM32CI( "Camille" ):
            return Camille::OnDraw( );
        default: ;
    }
}

void OnPresentDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnPresentDraw( );
        case SDBM32CI( "Camille" ):
            return Camille::OnPresentDraw( );
            default: ;
    }
}

void OnUpdate( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnUpdate( );
        case SDBM32CI( "Camille" ):
            return Camille::OnUpdate( );
        default: ;
    }
}

void OnBootPlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            Katarina::OnBoot( );
            break;
        case SDBM32CI( "Camille" ):
            Camille::OnBoot( );
            break;
        default: ;
    }
}

void OnTerminatePlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            Katarina::OnTerminate( );
            break;

        case SDBM32CI( "Camille" ):
            Camille::OnTerminate( );
            break;
        default: ;
    }
}

#pragma section(".tempc", execute, read)
extern "C" bool __declspec(dllexport, noinline, code_seg(".tempc")) __stdcall ZgLoad( SdkContext* pSdkContext )
{
    pSdkContext->Initialize( );
    Globals::Allocate( );

    OnBootPlugin( );
    g_pExportedEventHandler->Add( EventType::OnIssueOrder, OnIssueOrder );
    g_pExportedEventHandler->Add( EventType::OnCreateParticle, OnCreateParticle );
    g_pExportedEventHandler->Add( EventType::OnWorldDraw, OnDraw );
    g_pExportedEventHandler->Add( EventType::OnPresentDraw, OnPresentDraw );
    g_pExportedEventHandler->Add( EventType::OnGameUpdate, OnUpdate );
    g_pExportedEventHandler->Add( EventType::OnExecuteCastFrame, OnExecuteCastFrame );
    return true;
}

extern "C" void __declspec(dllexport, noinline, code_seg(".text")) __stdcall ZgUnload( )
{
    OnTerminatePlugin(  );
    g_pExportedEventHandler->Remove( EventType::OnIssueOrder, OnIssueOrder );
    g_pExportedEventHandler->Remove( EventType::OnCreateParticle, OnCreateParticle );
    g_pExportedEventHandler->Remove( EventType::OnWorldDraw, OnDraw );
    g_pExportedEventHandler->Remove( EventType::OnPresentDraw, OnPresentDraw );
    g_pExportedEventHandler->Remove( EventType::OnGameUpdate, OnUpdate );
    g_pExportedEventHandler->Remove( EventType::OnExecuteCastFrame, OnExecuteCastFrame );
}
