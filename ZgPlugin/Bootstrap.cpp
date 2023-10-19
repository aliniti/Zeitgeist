#include "stdafx.hpp"

// set to SDBM32CI("ChampionName") to load on a specific champion only
extern "C" inline __declspec(dllexport) std::uint32_t g_iUniqueAssemblyName = 0;

void OnGainBuff( GameObject* unit, BuffInstance* instance)
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        // case SDBM32CI( "Vex" ):
        //     return Vex::OnGainBuff( unit, instance );
        default: ;
    } 
}

void OnCreateObject( GameObject* unit )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnCreateObj( unit );
        default: ;
    } 
}

void OnCreateParticle( GameObject* unit, std::uint32_t iHash )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnCreateParticle( unit, iHash );
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnCreateParticle( unit, iHash );
        default: ;
    }
}

void OnDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnDraw( );
        case SDBM32CI( "Vex" ):
            return Vex::OnDraw( );
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnDraw( );
        default: ;
    }
}

void OnPresentDraw( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnPresentDraw( );
        case SDBM32CI( "Vex" ):
            return Vex::OnPresentDraw( );
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnPresentDraw( );
        default: ;
    }
}

void OnUpdate( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnUpdate( );
        case SDBM32CI( "Vex" ):
            return Vex::OnUpdate( );
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnUpdate( );    
        default: ;
    }
}

void OnBootPlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnBoot( );
        case SDBM32CI( "Vex" ):
            return Vex::OnBoot( );
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnBoot( );       
        default: ;
    }
}

void OnTerminatePlugin( )
{
    switch ( GetPlayer( )->CharacterData( )->SkinHash( ) )
    {
        case SDBM32CI( "Katarina" ):
            return Katarina::OnTerminate( );
        case SDBM32CI( "Vex" ):
            return Vex::OnTerminate( );
        // case SDBM32CI( "Zed" ):
        //     return Zedd::OnTerminate( );               
        default: ;
    }
}

#pragma section(".tempc", execute, read)
extern "C" bool __declspec(dllexport, noinline, code_seg(".tempc")) __stdcall ZgLoad( SdkContext* pSdkContext )
{
    pSdkContext->Initialize( );
    Globals::Allocate( );
    
    OnBootPlugin( );
    g_pExportedEventHandler->Add( EventType::OnGainBuff, OnGainBuff );
    g_pExportedEventHandler->Add( EventType::OnCreateObject, OnCreateObject );
    g_pExportedEventHandler->Add( EventType::OnCreateParticle, OnCreateParticle );
    g_pExportedEventHandler->Add( EventType::OnWorldDraw, OnDraw );
    g_pExportedEventHandler->Add( EventType::OnPresentDraw, OnPresentDraw );
    g_pExportedEventHandler->Add( EventType::OnGameUpdate, OnUpdate );
    return true;
}

extern "C" void __declspec(dllexport, noinline, code_seg(".text")) __stdcall ZgUnload( )
{
    OnTerminatePlugin(  );
    g_pExportedEventHandler->Remove( EventType::OnGainBuff, OnGainBuff );
    g_pExportedEventHandler->Remove( EventType::OnCreateObject, OnCreateObject );
    g_pExportedEventHandler->Remove( EventType::OnCreateParticle, OnCreateParticle );
    g_pExportedEventHandler->Remove( EventType::OnWorldDraw, OnDraw );
    g_pExportedEventHandler->Remove( EventType::OnPresentDraw, OnPresentDraw );
    g_pExportedEventHandler->Remove( EventType::OnGameUpdate, OnUpdate );
}
