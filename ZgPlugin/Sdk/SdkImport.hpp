#pragma once

#include <Windows.h>
#include <stdint.h>
#include <type_traits>
#include <string>
#include <vector>
#include <utility>
#include <emmintrin.h>
#include <memory>
#include <type_traits>
#include <algorithm>
#include <smmintrin.h>

#ifndef __clang__
#error Unsupported Compiler
#endif

#ifndef _WIN64 
#error Unsupported Architecture
#endif

extern "C" inline __declspec(dllexport) std::uint32_t g_iSdkVersion = 4;

namespace Zeitgeist::SdkImport {
	namespace Globals {
		inline void** g_ppPlayer;

		inline decltype(&malloc) Malloc;
		inline decltype(&memmove) Memmove;
		inline decltype(&free) Free;
		inline decltype(&sprintf) Sprintf;

		inline void(__cdecl* Write)(const char* szFormat, ...);
		inline void(__cdecl* Allocate)();

		inline float g_flDisplayWidth = 0.f;
		inline float g_flDisplayHeight = 0.f;
		inline std::uint32_t g_iMapId = 11;
		inline bool g_bIsReplay = false;
	}

#pragma region Hashing

	constexpr std::uint32_t FNV1A32HashCI(char const* const szString, const std::uint32_t iHash = 0x811C9DC5) {
		return '\0' == szString[0] ? iHash : FNV1A32HashCI(&szString[1], (((szString[0] >= 'A' && szString[0] <= 'Z') ? szString[0] + ('a' - 'A') : szString[0]) ^ iHash) * 0x1000193);
	}

	constexpr std::uint32_t FNV132HashCI(char const* const szString, const std::uint32_t iHash = 0x811C9DC5) {
		return '\0' == szString[0] ? iHash : FNV132HashCI(&szString[1], (iHash * 0x1000193) ^ ((szString[0] >= 'A' && szString[0] <= 'Z') ? szString[0] + ('a' - 'A') : szString[0]));
	}

	constexpr std::uint32_t FNV1A32Salt(const std::uint32_t iStartHash, const std::uint32_t iSalt) {
		std::uint32_t iHash = iStartHash;

		for ( size_t iIndex = 0; iIndex < 4; iIndex++ ) {
			iHash ^= reinterpret_cast<const uint8_t*>(&iSalt)[iIndex];
			iHash *= 0x1000193;
		}

		return iHash;
	}

	constexpr std::uint32_t SDBM32HashCI(char const* const szString, const std::uint32_t iHash = 0) {
		return '\0' == szString[0] ? iHash : SDBM32HashCI(&szString[1], ((szString[0] >= 'A' && szString[0] <= 'Z') ? szString[0] + ('a' - 'A') : szString[0]) + (iHash << 6) + (iHash << 16) - iHash);
	}

	constexpr std::uint32_t ELF32HashCI(char const* const szString, const  std::uint32_t iHash = 0x0) {
		std::uint32_t iValue = (16 * iHash + ((szString[0] >= 'A' && szString[0] <= 'Z') ? (szString[0] - 'A' + 'a') : szString[0]));
		return '\0' == szString[0] ? iHash : ELF32HashCI(&szString[1], (iValue & 0xF0000000) ? (iValue & 0xF0000000 ^ ((iValue & 0xF0000000) >> 24) ^ iValue) : iValue);
	}

#define FNV1A32CI(s) std::integral_constant<std::uint32_t, FNV1A32HashCI(s)>::value
#define FNV132CI(s) std::integral_constant<std::uint32_t, FNV132HashCI(s)>::value
#define SDBM32CI(s) std::integral_constant<std::uint32_t, SDBM32HashCI(s)>::value
#define ELF32CI(s) std::integral_constant<uint32_t, ELF32HashCI(s)>::value

#pragma endregion

#pragma region Enumerations

	enum struct GameObjectType : std::uint32_t {
		NeutralMinionCamp = FNV1A32CI("NeutralMinionCamp"),
		AIHeroClient = FNV1A32CI("AIHeroClient"),
		AIMarker = FNV1A32CI("AIMarker"),
		AIMinionClient = FNV1A32CI("AIMinionClient"),
		ObjectAttacher = FNV1A32CI("ObjectAttacher"),
		LevelPropAIClient = FNV1A32CI("LevelPropAIClient"),
		AITurretCommon = FNV1A32CI("AITurretCommon"),
		AITurretClient = FNV1A32CI("AITurretClient"),
		obj_GeneralParticleEmitter = FNV1A32CI("obj_GeneralParticleEmitter"),
		GameObject = FNV1A32CI("GameObject"),
		Pawn = FNV1A32CI("Pawn"),
		MissileClient = FNV1A32CI("MissileClient"),
		DrawFX = FNV1A32CI("DrawFX"),
		UnrevealedTarget = FNV1A32CI("UnrevealedTarget"),
		TFTFinisherPlayerProxyClient = FNV1A32CI("TFTFinisherPlayerProxyClient"),
		TFTSlotObject = FNV1A32CI("TFTSlotObject"),
		Barracks = FNV1A32CI("Barracks"),
		BarracksDampener = FNV1A32CI("BarracksDampener"),
		obj_Levelsizer = FNV1A32CI("obj_Levelsizer"),
		obj_NavPoint = FNV1A32CI("obj_NavPoint"),
		obj_SpawnPoint = FNV1A32CI("obj_SpawnPoint"),
		AnimatedBuilding = FNV1A32CI("AnimatedBuilding"),
		BuildingClient = FNV1A32CI("BuildingClient"),
		GrassObject = FNV1A32CI("GrassObject"),
		HQ = FNV1A32CI("HQ"),
		obj_InfoPoint = FNV1A32CI("obj_InfoPoint"),
		MapProp = FNV1A32CI("MapProp"),
		LevelPropGameObject = FNV1A32CI("LevelPropGameObject"),
		LevelPropSpawnerPoint = FNV1A32CI("LevelPropSpawnerPoint"),
		Shop = FNV1A32CI("Shop"),
		Turret = FNV1A32CI("Turret"),
	};

	enum struct CombatType : std::uint32_t {
		Melee = 1,
		Ranged = 2
	};

	enum struct DampenerState : std::uint32_t {
		Dead = 0,
		Alive = 1
	};

	enum struct ItemID : std::uint32_t {
		ArcaneSweeper = 3187,
		ArcaneSweeperAlt = 3348,
		Ohmwrecker = 3056,
		EdgeofNight = 3814,
		TwinShadows = 3905,
		ZzRotPortal = 3512,
		RanduinsOmen = 3143,
		RighteousGlory = 3800,
		BannerofCommand = 3060,
		FrostQueensClaim = 3092,
		YoumuusGhostblade = 3142,
		YoumuusGhostbladeOrnn = 3388,
		GargoyleStoneplate = 3193,
		SwordOfTheDivine = 3131,
		Spellbinder = 3907,
		DeathfireGrasp = 3128,
		SpectralCutlass = 4004,
		TheBlackSpear = 3599,
		YourCut = 3400,
		MinionDematerializer = 2403,

		EyeoftheWatchers = 2301,
		KnightsVow = 3109,
		Redemption = 3107,
		RedemptionOrnn = 3382,
		ZekesHarbinger = 3050,
		NomadsMedallion = 3096,
		ShurelyasReverie = 2065,
		RemnantOfTheAspect = 3401,
		TalismanOfAscension = 3069,
		TimewornFrostQueensClaim = 3312,
		TimewornNomadsMedallion = 3305,
		TimewornRelicShield = 3307,
		TimewornSpellthiefsEdge = 3310,
		TimewornTalismanofAscension = 3306,
		TimewornTargonsBrace = 3308,
		LocketOfTheIronSolari = 3190,
		LocketOfTheIronSolariOrnn = 3383,
		HeartOfTargon = 4302,
		PhilosophersMedallion = 4301,
		SpaceKnightsVow = 3118,

		BootsOfSpeed = 1001,
		SlightlyMagicalBoots = 2422,
		IonianBootsofLucidity = 3158,
		BerserkersGreaves = 3006,
		BootsofMobility = 3117,
		SorcerersShoes = 3020,
		NinjaTabi = 3047,
		MercurysTreads = 3111,
		BootsofSwiftness = 3009,

		ControlWard = 2055,
		ExplorersWard = 2050,
		OracleAlteration = 3364,
		FarsightAlteration = 3363,
		GreaterVisionTotemTrinket = 3362,
		GreaterStealthTotemTrinket = 3361,
		PeeringFarsightWard = 2057,
		PilferedStealthWard = 2056,
		SweepingLensTrinket = 3341,
		WardingTotemTrinket = 3340,

		ManaPotion = 2004,
		HealthPotion = 2003,
		ElixirOfIron = 2138,
		ElixirOfSkill = 2011,
		HuntersPotion = 2032,
		ElixirOfWrath = 2140,
		OraclesExtract = 2047,
		ElixirOfSorcery = 2139,
		RefillablePotion = 2031,
		CorruptingPotion = 2033,
		CrystallineFlask = 2041,
		ElixirOfFortitude = 2037,
		TotalBiscuitofRejuvenation = 2009,
		TotalBiscuitofEverlastingWill = 2010,

		SlySackOfGold = 2319,
		PilferedHealthPotion = 2061,
		LootedBiscuitofRejuvenation = 2012,
		LootedOraclesExtract = 2013,
		PilferedPotionOfRouge = 2062,
		TravelSizeElixirOfIron = 2058,
		TravelSizeElixirOfSorcery = 2059,

		Muramana = 3042,
		Manamune = 3004,
		SeraphsEmbrace = 3040,
		ArchangelsStaff = 3003,
		TearoftheGoddess = 3070,
		ManamuneQuickCharge = 3008,
		ArchangelsStaffQuickCharge = 3007,
		TearoftheGoddessQuickCharge = 3073,

		DervishBlade = 3137,
		MikaelsCrucible = 3222,
		QuicksilverSash = 3140,
		MercurialScimitar = 3139,
		SpaceMercurialScimitar = 3231,

		Stopwatch = 2420,
		StopwatchAlt = 2423,
		WoogletsWitchcap = 3090,
		WoogletsWitchcapOrnn = 3385,
		ZhonyasHourglass = 3157,
		ZhonyasHourglassOrnn = 3386,

		BilgewaterCutlass = 3144,
		BladeoftheRuinedKing = 3153,
		BladeoftheRuinedKingOrnn = 3389,
		SpaceBilgewaterCutlass = 3162,
		SpaceBladeoftheRuinedKing = 3149,

		Hexdrinker = 3155,
		HextechGLP800 = 3030,
		HextechGunblade = 3146,
		HextechRevolver = 3145,
		HextechProtobelt01 = 3152,
		SpaceHextechGunblade = 3148,

		Tiamat = 3077,
		TitanicHydra = 3748,
		RavenousHydra = 3074,
		SpaceRavenousHydra = 3230,

		EnchantmentBloodrazor = 1416,
		EnchantmentCinderhulk = 1401,
		EnchantmentRunicEchoes = 1402,
		EnchantmentWarrior = 1400,


		EnchantmentWarriorStalkersBlade = 1400,
		EnchantmentCinderhulkStalkersBlade = 1401,
		EnchantmentRunicEchoesStalkersBlade = 1402,
		EnchantmentBloodrazorStalkersBlade = 1416,

		EnchantmentWarriorSkirmishersSabre = 1412,
		EnchantmentCinderHulkSkirmishersSabre = 1413,
		EnchantmentRunicEchoesSkirmishersSabre = 1414,
		EnchantmentBloodrazorSkirmishersSabre = 1419,

		EnchantmentWarriorPridestalkers = 3671,
		EnchantmentCinderhulkPridestalkers = 3672,
		EnchantmentRunicEchoesPridestalkers = 3673,
		EnchantmentBloodrazorPridestalkers = 3675,

		EnchantmentBladeNexusPridestalkers = 4101,
		EnchantmentWarriorNexusPridestalkers = 4102,
		EnchantmentCinderhulkNexusPridestalkers = 4103,
		EnchantmentRunicEchoesNexusPridestalkers = 4104,
		EnchantmentBloodrazorNexusPridestalkers = 4105,

		AbyssalScepter = 3001,
		AbyssalScepterOrnn = 3379,
		AegisOfTheLegion = 3105,
		WingedMoonplate = 3066,
		AetherWisp = 3113,
		AmplifyingTome = 1052,
		AncientCoin = 3301,
		ArdentCenser = 3504,
		AthenesUnholyGrail = 3174,
		AtmasReckoning = 3005,
		BamisCinder = 3751,
		BansheesVeil = 3102,
		BFSword = 1038,
		BlastingWand = 1026,
		BrawlersGloves = 1051,
		BrokenStopwatch = 2421,
		BrokenStopwatchAlt = 2424,
		CatalystofAeons = 3010,
		CaulfieldsWarhammer = 3133,
		ChainVest = 1031,
		ChaliceofHarmony = 3028,
		CloakofAgility = 1018,
		CloakofStars = 3594,
		ClothArmor = 1029,
		CommencingStopwatch = 2419,
		CosmicShackle = 3690,
		CrystallineBracer = 3801,
		Cull = 1083,
		Dagger = 1042,
		DarkMatterScythe = 3692,
		DarkStarSigil = 3695,
		DeadMansPlate = 3742,
		DeathsDance = 3812,
		DeathsDaughter = 3902,
		DietPoroSnax = 2054,
		DoransBlade = 1055,
		DoransLostBlade = 4202,
		DoransLostIdol = 4204,
		DoransLostRing = 4203,
		DoransLostShield = 4201,
		DoransRing = 1056,
		DoransShield = 1054,
		DuskbladeofDraktharr = 3147,
		EntropyField = 3643,
		EspressoSnax = 3682,
		EssenceReaver = 3508,
		ExecutionersCalling = 3123,
		EyeoftheEquinox = 2303,
		EyeoftheHerald = 3513,
		EyeoftheHeraldAlt = 3514,
		EyeoftheOasis = 2302,
		FaerieCharm = 1004,
		FiendishCodex = 3108,
		FireatWill = 3901,
		FlashZone = 3640,
		ForbiddenIdol = 3114,
		ForceOfNature = 4401,
		FrostedSnax = 3680,
		Frostfang = 3098,
		FrozenHeart = 3110,
		FrozenMallet = 3022,
		GhostPoro = 3520,
		Ghostwalkers = 4001,
		GiantsBelt = 1011,
		GiantSlayer = 3034,
		GlacialShroud = 3024,
		GoldenTranscendence = 3460,
		GoldenTranscendenceDisabled = 3461,
		GravityBoots = 3693,
		GuardianAngel = 3026,
		GuardiansHammer = 3184,
		GuardiansHorn = 2051,
		GuardiansOrb = 3112,
		GuinsoosRageblade = 3124,
		HauntingGuise = 3136,
		HeadofKhaZix = 3175,
		HeadofKhaZixAlt1 = 3422,
		HeadofKhaZixAlt2 = 3416,
		HeadofKhaZixAlt3 = 3455,
		HeadofKhaZixAlt4 = 3410,
		HuntersMachete = 1041,
		HuntersTalisman = 1039,
		IcebornGauntlet = 3025,
		IcebornGauntletOrnn = 3387,
		InfinityEdge = 3031,
		InfinityEdgeOrnn = 3371,
		InnervatingLocket = 4402,
		JaurimsFist = 3052,
		Kindlegem = 3067,
		KircheisShard = 2015,
		LastWhisper = 3035,
		LiandrysTorment = 3151,
		LichBane = 3100,
		Lifeline = 4003,
		LongSword = 1036,
		LordDominiksRegards = 3036,
		LordVanDammsPillager = 3104,
		LostChapter = 3802,
		LudensEcho = 3285,
		LudensEchoOrnn = 3390,
		MawofMalmortius = 3156,
		MejaisSoulstealer = 3041,
		MoonflairSpellblade = 3170,
		Morellonomicon = 3165,
		MortalReminder = 3033,
		NashorsTooth = 3115,
		NeedlesslyLargeRod = 1058,
		NegatronCloak = 1057,
		NexusSiegeSiegeWeaponSlot = 3637,
		NullMagicMantle = 1033,
		OblivionOrb = 3916,
		OverlordsBloodmail = 3084,
		PerfectHexCore = 3198,
		Phage = 3044,
		PhantomDancer = 3046,
		Pickaxe = 1037,
		PoachersDirk = 3252,
		PoroSnax = 2052,
		PortPad = 3635,
		PrototypeHexCore = 3200,
		RabadonsDeathcap = 3089,
		RabadonsDeathcapOrnn = 3374,
		RainbowSnaxPartyPack = 3683,
		RaiseMorale = 3903,
		RapidFirecannon = 3094,
		RaptorCloak = 2053,
		RecurveBow = 1043,
		RejuvenationBead = 1006,
		RelicShield = 3302,
		RodofAges = 3027,
		RodofAgesQuickCharge = 3029,
		RubyCrystal = 1028,
		RubySightstone = 2045,
		RunaansHurricane = 3085,
		RylaisCrystalScepter = 3116,
		SanguineBlade = 3181,
		SapphireCrystal = 1027,
		SeekersArmguard = 3191,
		SeerStoneTrinketAlt = 3462,
		SeerStoneTrinket = 3645,
		SerratedDirk = 3134,
		Sheen = 3057,
		ShieldTotem = 3647,
		SiegeBallista = 3631,
		SiegeRefund = 3642,
		SiegeSightWarder = 3649,
		SiegeTeleport = 3630,
		SiegeTeleportInactive = 3648,
		Sightstone = 2049,
		SingularityLantern = 3691,
		SkirmishersSabre = 3715,
		SoulAnchorTrinket = 3345,
		SpaceBootsofLucidity = 3173,
		SpaceBloodthirster = 3080,
		SpaceMawofMalmortius = 3163,
		SpaceVampiricScepter = 1059,
		SpearofShojin = 3161,
		SpectresCowl = 3211,
		SpellthiefsEdge = 3303,
		SpiritVisage = 3065,
		StalkersBlade = 3706,
		StatikkShiv = 3087,
		StatStickOfStoicism = 4403,
		SteraksGage = 3053,
		Stinger = 3101,
		Stormrazor = 3095,
		SunfireCape = 3068,
		SunfireCapeOrnn = 3373,
		SuperSpicySnax = 3681,
		TargonsBrace = 3097,
		TheBlackCleaver = 3071,
		TheBlackCleaverOrnn = 3380,
		TheBloodthirster = 3072,
		TheDarkSeal = 1082,
		TheHexCoremk1 = 3196,
		TheHexCoremk2 = 3197,
		TheLightbringer = 3185,
		Thornmail = 3075,
		TimewornAncientCoin = 3304,
		TimewornFaceoftheMountain = 3309,
		TimewornFrostfang = 3311,
		TowerBeamofRuination = 3634,
		TowerStormBulwark = 3636,
		TrackersKnife = 3711,
		TrinityForce = 3078,
		TrinityForceOrnn = 3384,
		Unknown1 = 3632,
		VampiricScepter = 1053,
		VanguardBanner = 3641,
		VoidStaff = 3135,
		WardensMail = 3082,
		WarmogsArmor = 3083,
		WickedHatchet = 3122,
		WitsEnd = 3091,
		Zeal = 3086,
		Zephyr = 3172,

		SilvermereDawn = 6035,

		DivineSunderer = 6632,

		Noonquiver = 6670,
		Galeforce = 6671,
		KrakenSlayer = 6672,
		Rageknife = 6677,

		IronspikeWhip = 6029,
		Goredrinker = 6630,
		Stridebreaker = 6631,

		Recall = 2001,

		FrostFang = 3851,
		ShardOfTrueIce = 3853,
		TargonBucker = 3859,
		BulwarkOfTheMountain = 3860,
		HarrowingCrescent = 3863,
		BlackMistSScythe = 3864,
		RunesteelSpaulders = 3855,
		PauldronsofWhiterock = 3857,

		Arena_GuardiansHorn = 222051,
		Arena_ShurelyasBattlesong = 222065,
		Arena_Evenshroud = 223001,
		Arena_ArchangelsStaff = 223003,
		Arena_Manamune = 223004,
		Arena_BerserkersGreaves = 223006,
		Arena_BootsofSwiftness = 223009,
		Arena_ChemtechPutrifier = 223011,
		Arena_SorcerersShoes = 223020,
		Arena_GuardianAngel = 223026,
		Arena_InfinityEdge = 223031,
		Arena_MortalReminder = 223033,
		Arena_LordDominiksRegards = 223036,
		Arena_SeraphsEmbrace = 223040,
		Arena_Muramana = 223042,
		Arena_PhantomDancer = 223046,
		Arena_PlatedSteelcaps = 223047,
		Arena_ZekesConvergence = 223050,
		Arena_SteraksGage = 223053,
		Arena_SpiritVisage = 223065,
		Arena_SunfireAegis = 223068,
		Arena_BlackCleaver = 223071,
		Arena_Bloodthirster = 223072,
		Arena_RavenousHydra = 223074,
		Arena_Thornmail = 223075,
		Arena_TrinityForce = 223078,
		Arena_Heartsteel = 223084,
		Arena_RunaansHurricane = 223085,
		Arena_StatikkShiv = 223087,
		Arena_RabadonsDeathcap = 223089,
		Arena_WitsEnd = 223091,
		Arena_RapidFirecannon = 223094,
		Arena_Stormrazor = 223095,
		Arena_LichBane = 223100,
		Arena_BansheesVeil = 223102,
		Arena_Redemption = 223107,
		Arena_KnightsVow = 223109,
		Arena_FrozenHeart = 223110,
		Arena_MercurysTreads = 223111,
		Arena_GuardiansOrb = 223112,
		Arena_NashorsTooth = 223115,
		Arena_RylaisCrystalScepter = 223116,
		Arena_WintersApproach = 223119,
		Arena_Fimbulwinter = 223121,
		Arena_GuinsoosRageblade = 223124,
		Arena_VoidStaff = 223135,
		Arena_MercurialScimitar = 223139,
		Arena_YoumuusGhostblade = 223142,
		Arena_RanduinsOmen = 223143,
		Arena_HextechRocketbelt = 223152,
		Arena_BladeofTheRuinedKing = 223153,
		Arena_MawofMalmortius = 223156,
		Arena_ZhonyasHourglass = 223157,
		Arena_IonianBootsofLucidity = 223158,
		Arena_SpearOfShojin = 223161,
		Arena_Morellonomicon = 223165,
		Arena_GuardiansBlade = 223177,
		Arena_Hullbreaker = 223181,
		Arena_GuardiansHammer = 223184,
		Arena_GuardiansDirk = 223185,
		Arena_LocketoftheIronSolari = 223190,
		Arena_GargoyleStoneplate = 223193,
		Arena_MikaelsBlessing = 223222,
		Arena_ArdentCenser = 223504,
		Arena_EssenceReaver = 223508,
		Arena_DeadMansPlate = 223742,
		Arena_TitanicHydra = 223748,
		Arena_EdgeofNight = 223814,
		Arena_ImperialMandate = 224005,
		Arena_ForceofNature = 224401,
		Arena_HorizonFocus = 224628,
		Arena_CosmicDrive = 224629,
		Arena_Riftmaker = 224633,
		Arena_NightHarvester = 224636,
		Arena_DemonicEmbrace = 224637,
		Arena_CrownoftheShatteredQueen = 224644,
		Arena_Shadowflame = 224645,
		Arena_SilvermereDawn = 226035,
		Arena_DeathsDance = 226333,
		Arena_ChempunkChainsword = 226609,
		Arena_StaffofFlowingWater = 226616,
		Arena_MoonstoneRenewer = 226617,
		Arena_EchoesofHelia = 226620,
		Arena_Goredrinker = 226630,
		Arena_Stridebreaker = 226631,
		Arena_DivineSunderer = 226632,
		Arena_LiandrysAnguish = 226653,
		Arena_LudensTempest = 226655,
		Arena_Everfrost = 226656,
		Arena_RodofAges = 226657,
		Arena_IcebornGauntlet = 226662,
		Arena_TurboChemtank = 226664,
		Arena_JakShoTheProtean = 226665,
		Arena_RadiantVirtue = 226667,
		Arena_Galeforce = 226671,
		Arena_KrakenSlayer = 226672,
		Arena_ImmortalShieldbow = 226673,
		Arena_NavoriQuickblades = 226675,
		Arena_TheCollector = 226676,
		Arena_DuskbladeofDraktharr = 226691,
		Arena_Eclipse = 226692,
		Arena_ProwlersClaw = 226693,
		Arena_SeryldasGrudge = 226694,
		Arena_SerpentsFang = 226695,
		Arena_AxiomArc = 226696,
		Arena_Syzygy = 227001,
		Arena_DraktharrsShadowcarver = 227002,
		Arena_FrozenFist = 227005,
		Arena_Typhoon = 227006,
		Arena_IcathiasCurse = 227009,
		Arena_Vespertide = 227010,
		Arena_UpgradedAeropack = 227011,
		Arena_LiandrysLament = 227012,
		Arena_EyeofLuden = 227013,
		Arena_EternalWinter = 227014,
		Arena_CeaselessHunger = 227015,
		Arena_Dreamshatter = 227016,
		Arena_Deicide = 227017,
		Arena_InfinityForce = 227018,
		Arena_ReliquaryoftheGoldenDawn = 227019,
		Arena_ShurelyasRequiem = 227020,
		Arena_Starcaster = 227021,
		Arena_Equinox = 227023,
		Arena_Caesura = 227024,
		Arena_Leviathan = 227025,
		Arena_TheUnspokenParasite = 227026,
		Arena_PrimordialDawn = 227027,
		Arena_InfiniteConvergence = 227028,
		Arena_YoumuusWake = 227029,
		Arena_SeethingSorrow = 227030,
		Arena_EdgeofFinality = 227031,
		Arena_Flicker = 227032,
		Arena_CryoftheShriekingCity = 227033,
		Arena_AnathemasChains = 228001,
		Arena_AbyssalMask = 228020
	};

	enum SpellSlot : std::int32_t {
		Q = 0,
		W,
		E,
		R,
		Summoner1,
		Summoner2,
		Item1,
		Item2,
		Item3,
		Item4,
		Item5,
		Item6,
		Trinket,
		Recall,
		EmpoweredAutoAttack = 45,
		EmpoweredAutoAttack1 = 46,
		EmpoweredAutoAttack2 = 47,
		EmpoweredAutoAttack3 = 48,
		EmpoweredAutoAttack4 = 49,
		BasicAttack = 65,
		AlternateBasicAttack = 66
	};

	enum struct BuffType : std::uint8_t {
		Internal = 0,
		Aura,
		CombatEnchancer,
		CombatDehancer,
		SpellShield,
		Stun,
		Invisibility,
		Silence,
		Taunt,
		Berserk,
		Polymorph,
		Slow,
		Snare,
		Damage,
		Heal,
		Haste,
		SpellImmunity,
		PhysicalImmunity,
		Invulnerability,
		AttackSpeedSlow,
		NearSight,
		Currency,
		Fear,
		Charm,
		Poison,
		Suppression,
		Blind,
		Counter,
		Shred,
		Flee,
		Knockup,
		Knockback,
		Disarm,
		Grounded,
		Drowsy,
		Asleep,
		ObscuredBuff,
		ClickproofToEnemies,
		UnKillable
	};

	enum struct HeroOrder : std::uint32_t {
		HoldPosition = 1,
		MoveTo,
		AttackUnit,
		AutoAttackPet,
		AutoAttack,
		MovePet,
		AttackTo,
		Stop = 10
	};

	enum struct InputLocks : std::uint32_t {
		None = 0,
		IssuingOrders = 1 << 1,
		CastingSpells = 1 << 2,
		CastingSummoners = 1 << 3,
		CameraMovements = 1 << 7,
		All = 0xFFFFFFFF,
	};

	DEFINE_ENUM_FLAG_OPERATORS(InputLocks)

	enum struct StatusFlag : std::uint32_t {
		Invulnerable = 1 << 0,
		WillRevive = 1 << 1,
		Unstoppable = 1 << 2,
		WontDieLowHealth = 1 << 4,
		MagicImmune = 1 << 6,
		PhysicalImmune = 1 << 7,
		SpellImmune = 1 << 8
	};

	DEFINE_ENUM_FLAG_OPERATORS(StatusFlag)

	enum struct ActionState : std::uint32_t {
		CanAttack = 1 << 0,
		CanCast = 1 << 1,
		CanMove = 1 << 2,
		Immovable = 1 << 3,
		Unknown = 1 << 4,
		IsStealth = 1 << 5,
		Taunted = 1 << 6,
		Feared = 1 << 7,
		Fleeing = 1 << 8,
		Surpressed = 1 << 9,
		Asleep = 1 << 10,
		NearSight = 1 << 11,
		Ghosted = 1 << 12,
		HasGhost = 1 << 13,
		Charmed = 1 << 14,
		NoRender = 1 << 15,
		ForceRenderParticles = 1 << 16,
		DodgePiercing = 1 << 17,
		DisableAmbientGold = 1 << 18,
		DisableAmbientXP = 1 << 19,
		IsCombatEnchanced = 1 << 20,
		IsSelectable = 1 << 25
	};

	DEFINE_ENUM_FLAG_OPERATORS(ActionState)

	enum struct PerkID : std::uint32_t {
		SummonAery = 8214,
		ArcaneComet = 8229,
		PhaseRush = 8230,
		NullifyingOrb = 8224,
		ManaflowBand = 8226,
		NimbusCloak = 8275,
		Transcendence = 8210,
		Celerity = 8234,
		AbsoluteFocus = 8233,
		Scorch = 8237,
		Waterwalking = 8232,
		GatheringStorm = 8236,

		GlacialAugment = 8351,
		Kleptomancy = 8359,
		UnsealedSpellbook = 8360,
		HextechFlashtraption = 8306,
		MagicalFootwear = 8304,
		PerfectTiming = 8313,
		FuturesMarket = 8321,
		MinionDematerializer = 8316,
		BiscuitDelivery = 8345,
		CosmicInsight = 8347,
		ApproachVelocity = 8410,
		TimeWarpTonic = 8352,

		PressTheAttack = 8005,
		LethalTempo = 8008,
		FleetFootwork = 8021,
		Conqueror = 8010,
		Overheal = 9101,
		Triumph = 9111,
		PresenceOfMind = 8009,
		LegendAlacrity = 9104,
		LegendTenacity = 9105,
		LegendBloodline = 9103,
		CoupDeGrace = 8014,
		CutDown = 8017,
		LastStand = 8299,

		Electrocute = 8112,
		Predator = 8124,
		DarkHarvest = 8128,
		HailOfBlades = 9923,
		CheapShot = 8126,
		TasteOfBlood = 8139,
		SuddenImpact = 8143,
		ZombieWard = 8136,
		GhostPoro = 8120,
		EyeballCollection = 8138,
		RavenousHunter = 8135,
		IngeniousHunter = 8134,
		RelentlessHunter = 8105,
		UltimateHunter = 8106,

		GraspOfTheUndying = 8437,
		Aftershock = 8439,
		Guardian = 8465,
		Demolish = 8446,
		FontOfLife = 8463,
		BonePlating = 8473,
		Conditioning = 8429,
		SecondWind = 8444,
		Chrysalis = 8472,
		Overgrowth = 8451,
		Revitalize = 8453,
		Unflinching = 8242,
		ShieldBash = 8401
	};

	enum struct PingType : std::uint8_t {
		Default = 0,
		Target = 1,
		Caution = 5,
		AreaIsWarded = 7,
		Danger = 2,
		Missing = 3,
		OnMyWay = 4,
		AssistMe = 6
	};

	enum EmoteType : std::int32_t {
		Dance = 0,
		Taunt = 1,
		Laugh = 2,
		Joke = 3,
		Toggle = 4
	};

	enum GameMap : std::uint32_t {
		SummonersRift = 11,
		HowlingAbyss = 12,
		TFT = 22,
		CherryArena = 30
	};

	enum UnitTagFlags : std::uint64_t {
		Special = 0,
		Champion = 1ull << 0,
		Champion_Clone = 1ull << 1,
		Minion = 1ull << 2,
		Minion_Lane = 1ull << 3,
		Minion_Lane_Melee = 1ull << 4,
		Minion_Lane_Ranged = 1ull << 5,
		Minion_Lane_Siege = 1ull << 6,
		Minion_Lane_Super = 1ull << 7,
		Minion_Summon = 1ull << 8,
		Minion_Summon_Large = 1ull << 9,
		Monster = 1ull << 10,
		Monster_Epic = 1ull << 11,
		Monster_Large = 1ull << 12,
		Monster_Medium = 1ull << 13,
		Monster_Buff = 1ull << 14,
		Special_TurretIgnores = 1ull << 15,
		Special_LaneMinionIgnores = 1ull << 16,
		Special_AzirW = 1ull << 17,
		Special_AzirR = 1ull << 18,
		Special_CorkiBomb = 1ull << 19,
		Special_YorickW = 1ull << 20,
		Special_MonsterIgnores = 1ull << 21,
		Special_EpicMonsterIgnores = 1ull << 22,
		Special_Peaceful = 1ull << 23,
		Special_AscXerath = 1ull << 24,
		Special_PerksWormhole = 1ull << 25,
		Special_UntargetableBySpells = 1ull << 26,
		Special_SyndraSphere = 1ull << 27,
		Special_TeleportTarget = 1ull << 28,
		Special_TeleportInvalid = 1ull << 29,
		Special_TeleportPad = 1ull << 30,
		Special_Trap = 1ull << 31,
		Special_Tunnel = 1ull << 32,
		Special_KPMinion = 1ull << 33,
		Special_Void = 1ull << 34,
		Structure = 1ull << 35,
		Structure_Inhibitor = 1ull << 36,
		Structure_Nexus = 1ull << 37,
		Structure_Turret = 1ull << 38,
		Structure_Turret_Outer = 1ull << 39,
		Structure_Turret_Inner = 1ull << 40,
		Structure_Turret_Inhib = 1ull << 41,
		Structure_Turret_Nexus = 1ull << 42,
		Structure_Turret_Shrine = 1ull << 43,
		Ward = 1ull << 44,
		Monster_Dragon = 1ull << 45,
		Monster_Blue = 1ull << 46,
		Monster_Red = 1ull << 47,
		Monster_Crab = 1ull << 48,
		Monster_Gromp = 1ull << 49,
		Monster_Krug = 1ull << 50,
		Monster_Raptor = 1ull << 51,
		Monster_Wolf = 1ull << 52,
		Monster_Baron = 1ull << 53,
		Monster_RiftHerald = 1ull << 54,
		Monster_Horde = 1ull << 55,
		Monster_Camp = 1ull << 56,
		Plant = 1ull << 57,
		IsolationNonImpacting = 1ull << 58,
		KingPoro = 1ull << 59,
		Adventure_ChampionEnemy = 1ull << 60,
		SLIME_WalkingNexus = 1ull << 61,
		NEXUSBLITZ_EventCharacter = 1ull << 62,
		Minion_AttackMove_Inactive = 1ull << 63
	};

	enum struct CellCollisionFlags : std::uint16_t {
		None = 0,
		Grass = 1 << 0,
		Wall = 1 << 1,
		Building = 1 << 6,
		Property = 1 << 7,
		GlobalVision = 1 << 8
	};

	DEFINE_ENUM_FLAG_OPERATORS(CellCollisionFlags)

	enum struct EventType : std::uint32_t {
		OnGameStart = 0,                   // void OnGameStart()
		OnGameExit,                        // void OnGameExit()

		OnWndProc,                         // void OnWndProc(HWND iHwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)

		OnPreGameUpdate,                   // void OnPreGameUpdate()
		OnGameUpdate,                      // void OnGameUpdate()

		OnWorldDraw,                       // void OnWorldDraw()
		OnHudManagerDraw,                  // void OnHudManagerDraw()
		OnPresentDraw,                     // void OnPresentDraw()

		OnProcessSpellCast,                // void OnProcessSpellCast(GameObject* pCaster, SpellCastInfo* pCastInfo)
		OnProcessAutoAttack,               // void OnProcessAutoAttack(GameObject* pCaster, SpellCastInfo* pCastInfo)

		OnPlayAnimation,                   // void OnPlayAnimation(GameObject* pCaster, char* pString, std::uint32_t iHash)
		OnStopCast,                        // void OnStopCast(GameObject* pCaster)

		OnDie,                             // void OnDie(GameObject* pObject, GameObject* pKiller)
		OnLeaveVisibilityClient,           // void OnLeaveVisibilityClient(GameObject* pObject)

		OnCreateObject,                    // void OnCreateObject(GameObject* pObject)
		OnDeleteObject,                    // void OnDeleteObject(void* pObject)
		OnDeleteObjectAccess,              // void OnDeleteObjectAccess(void* pObject) 
										   // Everything can be accessed, however only for debugging purposes. Do not use use in a release build as it is unreliable.

		OnCreateParticle,                  // void OnCreateParticle(GameObject* pObject, std::uint32_t iEffectKeyHash)

		OnNewPath,                         // void OnNewPath(GameObject* pObject, bool bDashing, float flDashSpeed)

		OnIssueOrder,                      // void OnIssueOrder(Vector3* pPosition, GameObject* pTarget, bool bWasPlayerIssued, bool* pCancel)

		OnTargetless,                      // void OnTargetless(GameObject** ppTarget)
		OnPreAttack,                       // void OnPreAttack(GameObject** ppTarget, bool* pCancel)
		OnPreMove,                         // void OnPreMove(Vector3* pPosition, bool* pCancel)

		OnExecuteCastFrame,                // void OnExecuteCastFrame(GameObject* pCaster, SpellCasterClient* pCasterClient)

		OnLocalSpellCast,                  // void OnLocalSpellCast(std::int32_t iSlot, GameObject** ppTarget, Vector3* pStartPosition, Vector3* pClickPosition, Vector3* pTargetPosition, bool* pCancel)

		OnApplyDamage,                     // void OnApplyDamage(GameObject* pReceiver, GameObject* pDealer, float flDamage)

		OnGainBuff,                        // void OnGainBuff(GameObject* pObject, BuffInstance* pInstance);
		OnLoseBuff,                        // void OnLoseBuff(GameObject* pObject, BuffInstance* pInstance);

		OnCreateClientParticle,            // void OnCreateClientParticle(GameObject* pObject, std::uint32_t iEffectKeyHash)  
		OnDeleteClientParticle             // void OnDeleteClientParticle(GameObject* pObject)  
	};

	enum struct OrbwalkerMode : std::uint32_t {
		Combo = 0,
		Harass,
		LaneClear,
		LastHit,
		Flee
	};

	enum struct TargetSelectorMode : std::int32_t {
		Menu = -1,
		Automatic = 0,
		LeastAttacks,
		LeastHealth,
		LowestDistancePlayer,
		LowestDistanceMouse
	};

	enum struct PredictionCollisionFlags : std::uint32_t {
		None = 0,
		Minions = 1 << 0,
		Heroes = 1 << 1,
		Turrets = 1 << 2,
		Inhibitors = 1 << 3,
	};

	DEFINE_ENUM_FLAG_OPERATORS(PredictionCollisionFlags)

	enum Camp : std::uint32_t {
		BlueOrder = 1,
		WolvesOrder,
		RaptorsOrder,
		RedOrder,
		KrugsOrder,
		Dragon,
		BlueChaos,
		WolvesChaos,
		RaptorsChaos,
		RedChaos,
		KrugsChaos,
		Baron,
		GrompOrder,
		GrompChaos,
		CrabBottom,
		CrabTop,
		Herald
	};


#pragma endregion

#pragma region Forward Declarations

	class GameObject;
	class MissileMovement;
	class CharacterIntermediate;
	class Spellbook;
	class CharacterData;
	class AbilityResourceSlotInfo;
	class CharacterRecord;
	class CharacterDataStack;
	class PathControllerCommon;
	class UnitInfoComponent;
	class Experience;
	class AvatarClient;
	class HeroInventoryCommon;
	class BuffInstanceClient;
	class BuffManager;
	class BuffInstance;
	class SpellCastInfo;
	class InventorySlot;
	class ScriptBaseItem;
	class ItemData;
	class SpellCasterClient;
	class SpellDataInstance;
	class SpellData;
	class SpellDataResource;
	class FloatingInfobar;
	class HudHealthbar;
	class NavigationPath;

#pragma endregion

#pragma region Subclasses

#define Pi                   (3.141592654f)
#define ToRadian( flDegree ) (( flDegree ) * (Pi / 180.f))
#define ToDegree( flRadian ) (( flRadian ) * (180.f / Pi))

	struct Vector3 {
	public:

		float x, y, z;

	public:

		Vector3() = default;

		constexpr Vector3(float flX, float flY, float flZ) : x(flX), y(flY), z(flZ) {};

		constexpr Vector3(float flX, float flZ) : x(flX), y(0.0f), z(flZ) {};

		Vector3& operator += (const Vector3& stOther);

		Vector3& operator -= (const Vector3& stOther);

		Vector3& operator *= (float flValue);

		Vector3& operator /= (float flValue);

		Vector3 operator + () const;

		Vector3 operator - () const;

		Vector3 operator + (const Vector3& stOther) const;

		Vector3 operator - (const Vector3& stOther) const;

		Vector3 operator * (float flValue) const;

		Vector3 operator / (float flValue) const;

		friend Vector3 operator * (float flValue, const Vector3& stOther) {
			return Vector3(flValue * stOther.x, flValue * stOther.y, flValue * stOther.z);
		}

		bool operator == (const Vector3& stOther) const;

		bool operator != (const Vector3& stOther) const;

		bool IsValid() const;

		float LengthXZSqr() const;

		float LengthXZ() const;

		float DistanceXZSqr(const Vector3& stOther) const;

		float DistanceXZ(const Vector3& stOther) const;

		Vector3 PerpendicularClockwise() const;

		Vector3 PerpendicularCounterClockwise() const;

		Vector3 NormalizeXZ();

		Vector3 Rotated(float flAngle) const;

		float AngleBetweenXZ(const Vector3& stOther) const;

		float DotProductXZ(const Vector3& stOther) const;

		float CrossProductXZ(const Vector3& stOther) const;

		Vector3 Extend(const Vector3& stTo, float flDistance) const;

		bool IsOnScreen(float flThreshold = 0.f) const;
	};

	struct Vector2 {
	public:

		float x, y;

	public:

		Vector2() = default;
		constexpr Vector2(float flX, float flY) : x(flX), y(flY) {};
		constexpr Vector2(const Vector3& stVector3) : x(stVector3.x), y(stVector3.y) {};
	};

	struct StackString {
		char m_szString[16];
		std::uint32_t m_iSize;
		std::uint32_t m_iCapacity;
	};

	struct AllocatedString {
		char* m_szString;
		std::uint32_t m_iSize;
		std::uint32_t m_iCapacity;
	};

	template<typename T>
	struct AllocatedArray {
		T* m_pList;
		std::uint32_t m_iSize;
		std::uint32_t m_iCapacity;

		const T& front() {
			return this->m_pList[0];
		}

		const T& back() {
			return this->m_pList[this->m_iSize - 1];
		}

		std::uint32_t size() {
			return this->m_iSize;
		}

		T* begin() {
			return this->m_pList;
		}

		T* end() {
			return this->m_pList + this->m_iSize;
		}

		const T& at(std::size_t iIndex) {
			return this->m_pList[iIndex];
		}

		const T& operator[](std::size_t iIndex) {
			return this->m_pList[iIndex];
		}
	};

	template<typename T>
	struct Vector {
	public:

		std::uint32_t m_iSize;
		std::uint32_t m_iCapacity;
		T* m_pData;

		typedef T value_type;
		typedef value_type* iterator;
		typedef const value_type* const_iterator;

		using reference = T&;
		using const_reference = const T&;
		using value_type = T;
		using size_type = std::uint32_t;

		constexpr Vector() : m_iSize(0), m_iCapacity(0), m_pData(NULL) {}

		Vector(const Vector<T>& stSource) {
			this->m_iSize = this->m_iCapacity = 0; this->m_pData = NULL;
			this->operator=(stSource);
		}

		Vector<T>& operator=(const Vector<T>& stSource) {
			this->clear();
			this->resize(stSource.m_iSize);
			memcpy(this->m_pData, stSource.m_pData, this->m_iSize * sizeof(T));
			return *this;
		}

		~Vector() {
			if ( NULL != this->m_pData ) /*[[likely_old]]*/ {
				Globals::Free(this->m_pData);
			}
		}

		bool empty() const {
			return 0 == this->m_iSize;
		}

		size_type size() {
			return this->m_iSize;
		}

		size_type capacity() {
			return this->m_iCapacity;
		}

		T& operator[](std::uint32_t iIndex) {
			return this->m_pData[iIndex];
		}

		const T& operator[](std::uint32_t iIndex) const {
			return this->m_pData[iIndex];
		}

		void clear() {
			if ( NULL != this->m_pData ) /*[[likely_old]]*/ {
				this->m_iSize = this->m_iCapacity = 0;
				Globals::Free(this->m_pData);
				this->m_pData = NULL;
			}
		}

		T* begin() {
			return this->m_pData;
		}

		const T* begin() const {
			return this->m_pData;
		}

		T* end() {
			return this->m_pData + this->m_iSize;
		}

		const T* end() const {
			return this->m_pData + this->m_iSize;
		}

		T* rbegin() {
			return end();
		}

		const T* rbegin() const {
			return end();
		}

		T* rend() {
			return begin();
		}

		const T* rend() const {
			return begin();
		}

		T& front() {
			return this->m_pData[0];
		}

		const T& front() const {
			return this->m_pData[0];
		}

		T& back() {
			return this->m_pData[this->m_iSize - 1];
		}

		const T& back() const {
			return this->m_pData[this->m_iSize - 1];
		}

		void swap(Vector<T>& stRight) {
			size_type iRightSize = stRight.m_iSize;
			stRight.m_iSize = this->m_iSize;
			this->m_iSize = iRightSize;

			size_type iRightCapacity = stRight.m_iCapacity;
			stRight.m_iCapacity = this->m_iCapacity;
			this->m_iCapacity = iRightCapacity;

			T* pRightData = stRight.m_pData;
			stRight.m_pData = this->m_pData;
			this->m_pData = pRightData;
		}

		size_type grow_capacity(size_type iNewSize) const {
			int iNewCapacity = this->m_iCapacity ? (this->m_iCapacity + this->m_iCapacity / 2) : 8;
			return iNewCapacity > iNewSize ? iNewCapacity : iNewSize;
		}

		void  resize(size_type iNewSize) {
			if ( iNewSize > this->m_iCapacity ) /*[[likely_old]]*/ {
				this->reserve(grow_capacity(iNewSize));
			}

			this->m_iSize = iNewSize;
		}

		void shrink(size_type iNewSize) {
			this->m_iSize = iNewSize;
		}

		void reserve(size_type iNewCapacity) {
			if ( iNewCapacity <= this->m_iCapacity ) {
				return;
			}

			T* pNewData = reinterpret_cast<T*>(Globals::Malloc(iNewCapacity * sizeof(T)));
			if ( this->m_pData ) /*[[likely_old]]*/ {
				memcpy(pNewData, this->m_pData, this->m_iSize * sizeof(T));
				Globals::Free(this->m_pData);
			}

			this->m_pData = pNewData;
			this->m_iCapacity = iNewCapacity;
		}

		__attribute((noinline)) void push_back(const T& stT) {
			if ( this->m_iSize == this->m_iCapacity ) [[unlikely_old]] {
				this->reserve(this->grow_capacity(this->m_iSize + 1));
			}

			new (&this->m_pData[this->m_iSize]) T(stT);
			this->m_iSize++;
		}

		void pop_back() {
			this->m_iSize--;
		}

		void push_front(const T& stT) {
			if ( 0 == this->m_iSize ) {
				this->push_back(stT);
				return;
			}

			this->insert(this->m_pData, stT);
		}

		void pop_front() {
			if ( 0 == this->m_iSize ) {
				return;
			}

			this->erase(this->begin());
		}

		T* erase(const T* pIterator) {
			const std::ptrdiff_t pDelta = pIterator - this->m_pData;
			Globals::Memmove(this->m_pData + pDelta, this->m_pData + pDelta + 1, (this->m_iSize - static_cast<size_type>(pDelta) - 1) * sizeof(T));
			this->m_iSize--;
			return this->m_pData + pDelta;
		}

		T* insert(const T* pIterator, const T& stT) {
			const std::ptrdiff_t pDelta = pIterator - this->m_pData;

			if ( this->m_iSize == this->m_iCapacity ) [[unlikely_old]] {
				this->reserve(this->grow_capacity(this->m_iSize + 1));
			}

			if ( pDelta < this->m_iSize ) {
				Globals::Memmove(this->m_pData + pDelta + 1, this->m_pData + pDelta, (this->m_iSize - static_cast<size_type>(pDelta)) * sizeof(T));
			}

			new (&this->m_pData[pDelta]) T(stT);
			this->m_iSize++;
			return this->m_pData + pDelta;
		}

		template<typename Comparator>
		void do_quicksort(int iLeft, int iRight, Comparator pfnCompare) {
			if ( iLeft < iRight ) {
				int iPivotIndex = (iLeft + iRight) / 2;
				const T& stPivot = this->operator[](iPivotIndex);

				int iIndex = iLeft;
				int jIndex = iRight;
				while ( iIndex <= jIndex ) {
					while ( pfnCompare(this->operator[](iIndex), stPivot) ) {
						iIndex++;
					}
					while ( pfnCompare(stPivot, this->operator[](jIndex)) ) {
						jIndex--;
					}
					if ( iIndex <= jIndex ) {
						std::swap(this->operator[](iIndex), this->operator[](jIndex));
						iIndex++;
						jIndex--;
					}
				}

				do_quicksort(iLeft, jIndex, pfnCompare);
				do_quicksort(iIndex, iRight, pfnCompare);
			}
		}

		template<typename Comparator>
		void sort(Comparator pfnCompare) {
			do_quicksort(0, this->m_iSize - 1, pfnCompare);
		}
	};

	struct PredictionInput {
		GameObject* m_pSource = NULL;
		Vector3 m_vecSourcePosition;

		bool m_bHitBox = false;
		float m_flRange = 0.f;
		float m_flDelay = 0.f;
		float m_flRadius = 0.f;
		float m_flSpeed = 0.f;

		PredictionCollisionFlags m_iFlags;
	};

	struct PredictionOutput {
		PredictionInput m_stInput;

		Vector3 m_vecTargetPosition;
		Vector3 m_vecCastPosition;

		float m_flHitChance = 0.f;
		float m_flTimeToHit = 0.f;

		bool m_bInRange = false;
		bool m_bSeeThrough = false;
	};

	namespace CompileTimeRandom {
		constexpr const char* g_szTime = __TIME__;

		constexpr unsigned int g_iSeed = static_cast<unsigned int>(g_szTime[7]) +
			static_cast<unsigned int>(g_szTime[6]) * 10 +
			static_cast<unsigned int>(g_szTime[4]) * 60 +
			static_cast<unsigned int>(g_szTime[3]) * 600 +
			static_cast<unsigned int>(g_szTime[1]) * 3600 +
			static_cast<unsigned int>(g_szTime[0]) * 36000;

		template <int N>
		struct Generator
		{
		private:
			static constexpr unsigned a = 16807;  // 7^5
			static constexpr unsigned m = 2147483647;  // 2^31 - 1
			static constexpr unsigned s = Generator<N - 1>::value;
			static constexpr unsigned lo = a * (s & 0xFFFF);  // multiply lower 16 bits by 16807
			static constexpr unsigned hi = a * (s >> 16);  // multiply higher 16 bits by 16807
			static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16);  // combine lower 15 bits of hi with lo's upper bits
			static constexpr unsigned hi2 = hi >> 15;  // discard lower 15 bits of hi
			static constexpr unsigned lo3 = lo2 + hi;

		public:
			static constexpr unsigned max = m;
			static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
		};

		template <>
		struct Generator<0> {
			static constexpr unsigned value = g_iSeed;
		};

		template <int N, int M>
		struct MinMax {
			static constexpr auto value = Generator<N + 1>::value % M;
		};

		template <int N>
		struct Int {
			static const int value = static_cast<int>(1 + MinMax<N % 256, (0xFFFFFFFF / 0x2) - 1>::value);
		};
	};

	template<std::size_t iSize>
	constexpr static std::size_t GetBufferSize() {
		return ((iSize / 16) + (iSize % 16 != 0)) * 2;
	}

	template<typename T>
	__forceinline static T LoadFromRegister(T iValue) noexcept {
		asm("" : "=r"(iValue) : "0"(iValue) : );
		return iValue;
	}

	template<typename T, std::size_t iMaxSize>
	struct CompileTimeString {
		alignas(8) std::uint64_t m_iKey;
		alignas(8) std::uint64_t m_rgData[GetBufferSize<iMaxSize>()];
		alignas(8) std::size_t m_iLength;

		__forceinline std::uint64_t constexpr LoadValue64(std::size_t iIndexOffset2, std::size_t iSize, const T* szString) noexcept {
			constexpr auto iValueSize = sizeof(T);
			constexpr auto iIndexOffset = 8 / iValueSize;
			using CastType = typename std::make_unsigned<T>::type;

			std::uint64_t iValue = 0;
			for ( std::size_t iIndex = 0; iIndex < iIndexOffset && iIndex + iIndexOffset2 * iIndexOffset < iSize; ++iIndex ) {
				iValue ^=
					(std::uint64_t{ static_cast<CastType>(szString[iIndex + iIndexOffset2 * iIndexOffset]) }
				<< ((iIndex % iIndexOffset) * 8 * iValueSize));
			}

			if ( 0 == iValue ) {
				return iValue;
			}

			return iValue ^ this->m_iKey;
		}

		__forceinline constexpr CompileTimeString(T* stOther) : m_iKey{ (static_cast<std::uint64_t>(CompileTimeRandom::Int<__COUNTER__ + iMaxSize>::value) << 32) | CompileTimeRandom::Int<__COUNTER__ + iMaxSize>::value }, m_iLength(0) {
			this->operator=(stOther);
		}

		__forceinline constexpr CompileTimeString(const T* stOther) : m_iKey{ (static_cast<std::uint64_t>(CompileTimeRandom::Int<__COUNTER__ + iMaxSize>::value) << 32) | CompileTimeRandom::Int<__COUNTER__ + iMaxSize>::value }, m_iLength(0) {
			this->operator=(stOther);
		}

		__forceinline constexpr CompileTimeString() : m_iKey{ ((static_cast<std::uint64_t>(CompileTimeRandom::Int<__COUNTER__>::value) << 32) | static_cast<std::uint64_t>(CompileTimeRandom::Int<__COUNTER__>::value)) }, m_iLength(0) {}

		template <class L, size_t... iIndeces>
		__forceinline constexpr CompileTimeString(std::size_t iSize, L l, std::index_sequence<iIndeces...>) : m_iKey{ ((static_cast<std::uint64_t>(CompileTimeRandom::Int<__COUNTER__>::value) << 32) | static_cast<std::uint64_t>(CompileTimeRandom::Int<__COUNTER__>::value)) }, m_rgData{
			LoadFromRegister(LoadValue64(iIndeces, iSize, l()))... }, m_iLength(iSize - 1) {
		}

		__forceinline constexpr T EncryptCharacter(const T iValue, std::size_t iIndex) {
			volatile std::uint64_t* pData = reinterpret_cast<std::uint64_t*>((reinterpret_cast<std::uintptr_t>(this->m_rgData)));
			__m128 iKey128 = _mm_castsi128_ps(_mm_set1_epi64x(static_cast<std::uint64_t>((this->m_iKey))));
			__m128 iValue128 = _mm_castsi128_ps(_mm_set1_epi64x(pData[iIndex / 8]));
			__m128 iEncryptedValue128 = _mm_xor_ps(iValue128, iKey128);
			reinterpret_cast<uint8_t*>(&iEncryptedValue128)[iIndex % 8] = iValue;
			iEncryptedValue128 = _mm_xor_ps(iEncryptedValue128, iKey128);
			pData[iIndex / 8] = *reinterpret_cast<volatile uint64_t*>(&iEncryptedValue128);
			return iValue;
		}

		__forceinline constexpr CompileTimeString<T, iMaxSize >& operator=(const T* stOther) noexcept {
			std::uint32_t iLength = 0;
			while ( *stOther ) {
				EncryptCharacter(*stOther++, iLength++);

				if ( iLength >= iMaxSize ) {
					break;
				}
			}

			this->m_iLength = iLength;
			return *this;
		}
	};

#pragma endregion

#define INITIALIZE_FACTORY(Class) g_pExported##Class = this->m_stInstances.m_p##Class##Factory;
#define IMPORT_METHOD(ReturnType, ImportName, ImportArguments, ExportArguments, FunctionCall ) \
        public: virtual ReturnType __DONOTCALL##ImportName ExportArguments = 0; \
        public: ReturnType ImportName ImportArguments{ return CONTEXT_FACTORY->__DONOTCALL##ImportName FunctionCall; }

#define CONTEXT_FACTORY g_pExportedGameObject
	inline class GameObject* g_pExportedGameObject;
	class GameObject {
		IMPORT_METHOD(GameObjectType, ClassId, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint16_t, Handle, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t, Handle32, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsZombie, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t, Flags, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::int32_t, Team, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::string*, Name, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t, NetworkId, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsActive, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsVisibleOnScreen, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsDead, (), (GameObject* pThis), (this))
			IMPORT_METHOD(Vector3, Position, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsVisible, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsVisibleToObserver, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, PAR, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, MaxPAR, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, PAREnabled, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, SAR, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, MaxSAR, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, SAREnabled, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::StatusFlag, StatusFlags, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, Lifetime, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, MaxLifetime, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, LifetimeTicks, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, Health, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, MaxHealth, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, AllShield, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, PhysicalShield, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, MagicalShield, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, ChampSpecificHealth, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, IncomingHealingAllied, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, IncomingHealingEnemy, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, StopShieldFade, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, PathfindingCollisionRadius, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t, IsTargetable, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t, IsTargetableToTeamFlags, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::DampenerState, State, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t*, ActionState, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::uint32_t*, ActionState2, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::CharacterIntermediate*, CharacterIntermediate, (), (GameObject* pThis), (this))
			IMPORT_METHOD(Vector3, FacingDirection, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::CombatType, CombatType, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::Spellbook*, Spellbook, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::CharacterData*, CharacterData, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::CharacterDataStack*, CharacterDataStack, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::PathControllerCommon*, PathControllerCommon, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::UnitInfoComponent*, UnitInfoComponent, (), (GameObject* pThis), (this))
			IMPORT_METHOD(SdkImport::BuffManager*, BuffManager, (), (GameObject* pThis), (this))

			// AIHeroClient
			IMPORT_METHOD(SdkImport::Experience*, Experience, (), (GameObject* pThis), (this))
			
			IMPORT_METHOD(SdkImport::AvatarClient*, AvatarClient, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, HasPerk, (PerkID iPerkId, float* pStacks = NULL), (GameObject* pThis, PerkID iPerkId, float* pStacks = NULL), (this, iPerkId, pStacks))
			
			IMPORT_METHOD(SdkImport::HeroInventoryCommon*, HeroInventoryCommon, (), (GameObject* pThis), (this))
			// ..

			// AIMinionClient
			IMPORT_METHOD(bool, IsPet, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsWard, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsLaneMinion, (), (GameObject* pThis), (this))
			// ..

			// AIHeroClient
			IMPORT_METHOD(SdkImport::InputLocks*, InputLocks, (), (GameObject* pThis), (this))
			// ..

			IMPORT_METHOD(float, TotalMagicalShieldHealth, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, TotalPhysicalShieldHealth, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, TotalShieldHealth, (), (GameObject* pThis), (this))

			IMPORT_METHOD(float, AttackDelay, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, AttackCastDelay, (), (GameObject* pThis), (this))

			IMPORT_METHOD(float, BoundingRadius, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, GetDeathDuration, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, GetRespawnTimeRemaining, (), (GameObject* pThis), (this))
			IMPORT_METHOD(GameObject*, GetGoldRedirectTarget, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsPlayingAnimation, (std::uint32_t iHash), (GameObject* pThis, std::uint32_t iHash), (this, iHash))

			// MissileClient
			IMPORT_METHOD(class SdkImport::SpellCastInfo*, SpellCastInfo, (), (GameObject* pThis), (this))
			IMPORT_METHOD(class SdkImport::MissileMovement*, MissileMovement, (), (GameObject* pThis), (this))
			// ..

			// obj_GeneralParticleEmitter
			IMPORT_METHOD(std::uint32_t, GetEffectKeyHash, (), (GameObject* pThis), (this))
			IMPORT_METHOD(GameObject*, GetEmitter, (), (GameObject* pThis), (this))
			IMPORT_METHOD(GameObject*, GetAttachment, (), (GameObject* pThis), (this))
			IMPORT_METHOD(GameObject*, GetTargetAttachment, (), (GameObject* pThis), (this))
			IMPORT_METHOD(Vector3, Orientation, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, ParticleScale, (), (GameObject* pThis), (this))
			// ..

			IMPORT_METHOD(bool, IsRecalling, (), (GameObject* pThis), (this))

			IMPORT_METHOD(bool, IsAlly, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsEnemy, (), (GameObject* pThis), (this))

			IMPORT_METHOD(bool, IsAI, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsMinion, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsHero, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsTurret, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsMissile, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsAnimatedBuilding, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsAttackableUnit, (), (GameObject* pThis), (this))

			IMPORT_METHOD(bool, IsPlant, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsEpicMonster, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsBaron, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsDragon, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsElderDragon, (), (GameObject* pThis), (this))

			IMPORT_METHOD(bool, IsMelee, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, IsRanged, (), (GameObject* pThis), (this))

			IMPORT_METHOD(bool, CanCast, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, CanMove, (), (GameObject* pThis), (this))
			IMPORT_METHOD(bool, CanAttack, (), (GameObject* pThis), (this))

			IMPORT_METHOD(void*, GetCircleIcon, (), (GameObject* pThis), (this))
			IMPORT_METHOD(void*, GetSquareIcon, (), (GameObject* pThis), (this))

			// AIHeroClient
			IMPORT_METHOD(bool, HasItem, (ItemID iItemId), (GameObject* pThis, ItemID iItemId), (this, iItemId))
			IMPORT_METHOD(SdkImport::InventorySlot*, GetItem, (ItemID iItemId), (GameObject* pThis, ItemID iItemId), (this, iItemId))
			IMPORT_METHOD(SpellSlot, GetItemSlot, (ItemID iItemId), (GameObject* pThis, ItemID iItemId), (this, iItemId))
			typedef bool(__cdecl* pfnIterateItem)(InventorySlot* pItem, std::int32_t iItemId, std::int32_t iSpellSlot);
		    IMPORT_METHOD(bool, IterateItems, (pfnIterateItem pfnIterator), (GameObject* pThis, pfnIterateItem pfnIterator), (this, pfnIterator))
			// ..

			IMPORT_METHOD(bool, HasBuff, (std::uint32_t iHash), (GameObject* pThis, std::uint32_t iHash), (this, iHash))
			IMPORT_METHOD(bool, HasBuffOfType, (BuffType iType), (GameObject* pThis, BuffType iType), (this, iType))
			IMPORT_METHOD(SdkImport::BuffInstance*, GetBuff, (std::uint32_t iHash), (GameObject* pThis, std::uint32_t iHash), (this, iHash))
			typedef bool(__cdecl* pfnIterateBuff)(BuffInstance* pBuff);
		    IMPORT_METHOD(bool, IterateBuffs, (pfnIterateBuff pfnIterator), (GameObject* pThis, pfnIterateBuff pfnIterator), (this, pfnIterator))

			IMPORT_METHOD(float, BonusPhysicalDamage, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, TotalAttackDamage, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, PhysicalDamageScale, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, PhysicalDamage, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, TotalAbilityDamage, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, BonusAbilityDamage, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, AbilityDamageScale, (), (GameObject* pThis), (this))
			IMPORT_METHOD(float, AbilityDamage, (), (GameObject* pThis), (this))

			IMPORT_METHOD(bool, IsAttackable, (bool bIgnoreAlly, bool bAllowPlants), (GameObject* pThis, bool bIgnoreAlly, bool bAllowPlants), (this, bIgnoreAlly, bAllowPlants))

			IMPORT_METHOD(bool, IsUnderTurret, (bool bAllowAlly = false), (GameObject* pThis, bool bAllowAlly = false), (this, bAllowAlly))

			IMPORT_METHOD(float, DistanceXZ, (const Vector3& vecPosition), (GameObject* pThis, const Vector3& vecPosition), (this, vecPosition))
			IMPORT_METHOD(float, DistanceXZ, (GameObject* pObject), (GameObject* pThis, GameObject* pObject), (this, pObject))
			IMPORT_METHOD(float, DistanceXZSqr, (const Vector3& vecPosition), (GameObject* pThis, const Vector3& vecPosition), (this, vecPosition))
			IMPORT_METHOD(float, DistanceXZSqr, (GameObject* pObject), (GameObject* pThis, GameObject* pObject), (this, pObject))

			// AIHeroClient: Local Player
			IMPORT_METHOD(bool, IssueMoveOrder, (Vector3* pPosition, bool bCallEvent, bool bReset, bool bLimit, bool bShowClick, float* pTimer), (GameObject* pThis, Vector3* pPosition, bool bCallEvent, bool bReset, bool bLimit, bool bShowClick, float* pTimer), (this, pPosition, bCallEvent, bReset, bLimit, bShowClick, pTimer))
			IMPORT_METHOD(bool, IssueAttackOrder, (GameObject* pTarget, bool bCallEvent, bool bReset, bool bLimit, bool bShowClick, float* pTimer), (GameObject* pThis, GameObject* pTarget, bool bCallEvent, bool bReset, bool bLimit, bool bShowClick, float* pTimer), (this, pTarget, bCallEvent, bReset, bLimit, bShowClick, pTimer))
			IMPORT_METHOD(bool, IssueStopOrder, (float* pTimer), (GameObject* pThis, float* pTimer), (this, pTimer))
			IMPORT_METHOD(void, DoEmote, (std::int8_t iEmoteId), (GameObject* pThis, std::int8_t iEmoteId), (this, iEmoteId))
			// ..

			IMPORT_METHOD(void, DrawDamageIndicator, (float flDamage, bool bPhysical, bool bMagical), (GameObject* pThis, float flDamage, bool bPhysical, bool bMagical), (this, flDamage, bPhysical, bMagical))

			IMPORT_METHOD(bool, GetMissileSpeed, (float* pMissileSpeed), (GameObject* pThis, float* pMissileSpeed), (this, pMissileSpeed))

			// AIBaseClient
			IMPORT_METHOD(bool, HasUnitTag, (std::uint64_t iFlag), (GameObject* pThis, std::uint64_t iFlag), (this, iFlag))
			IMPORT_METHOD(AllocatedString*, UnitTagsString, (), (GameObject* pThis), (this))
			IMPORT_METHOD(std::int32_t, EvolvePoints, (), (GameObject* pThis), (this))
			// ..

		    // AIHeroClient
			IMPORT_METHOD(bool, IsBot, (), (GameObject* pThis), (this))
			// ..

			template<typename T>
			bool IterateBuffs2(T pfnIterator) {
				SdkImport::BuffManager* pBuffManager = this->BuffManager();
				if ( NULL == pBuffManager ) {
					return false;
				}

				std::vector<BuffInstanceClient>* pInstances = pBuffManager->Instances();
				if ( 0 == pInstances->size() ) {
					return false;
				}

				for ( auto pIterator = pInstances->begin(); pIterator != pInstances->end(); pIterator++ ) {
					if ( NULL == pIterator->m_pInstance || NULL == pIterator->m_pInstance->IsActive() ) {
						continue;
					}

					if ( pfnIterator(pIterator->m_pInstance) ) {
						return true;
					}
				}

				return false;
			}

			template<typename T>
			bool IterateItems2(T pfnIterator) {
				SdkImport::HeroInventoryCommon* pHeroInventoryCommon = this->HeroInventoryCommon();
				if ( NULL == pHeroInventoryCommon ) {
					return false;
				}

				for ( int iIndex = 0, iSpellSlot = 6; iIndex < 7; iIndex++, iSpellSlot++ ) {
					InventorySlot* pInventorySlot = pHeroInventoryCommon->InventorySlots()[iIndex];
					if ( NULL == pInventorySlot ) {
						continue;
					}

					ScriptBaseItem* pScriptBaseItem = pInventorySlot->ScriptBaseItem();
					if ( NULL == pScriptBaseItem ) {
						continue;
					}

					ItemData* pItemData = pScriptBaseItem->ItemData();
					if ( NULL == pItemData ) {
						continue;
					}

					if ( pfnIterator(pInventorySlot, pItemData->ItemId(), iSpellSlot) ) {
						return true;
					}
				}

				return false;
			}
	};

#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedCharacterIntermediate
	inline class CharacterIntermediate* g_pExportedCharacterIntermediate;
	class CharacterIntermediate {
		IMPORT_METHOD(float, PercentCooldownMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, AbilityHasteMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, BaseAttackDamage, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, BaseAbilityDamage, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, Dodge, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, Crit, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, Armor, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, BonusArmor, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, SpellBlock, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, BonusSpellBlock, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, HPRegenRate, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PARRegenRate, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, AttackRange, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatPhysicalDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentPhysicalDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentBonusPhysicalDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentBasePhysicalDamageAsFlatBonusMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentAttackSpeedMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentMultiplicativeAttackSpeedMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, CritDamageMultiplier, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, BaseAttackDamageSansPercentScale, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentBaseAttackDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatBaseAttackDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentEXPBonus, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentCCReduction, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentCooldownCapMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, SecondaryARBaseRegenRateRep, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, SecondaryARRegenRateRep, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, BaseHPRegenRate, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, MagicLethality, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PhysicalLethality, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentBonusMagicPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentCritTotalArmorPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentCritBonusArmorPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentBonusArmorPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatMagicDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentMagicDamageMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatMagicReduction, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentMagicReduction, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, AttackSpeedMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatCastRangeMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PassiveCooldownEndTime, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PassiveCooldownTotalTime, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatArmorPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentArmorPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatMagicPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentMagicPenetration, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentLifeStealMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentSpellVampMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentDamageToBarracksMinionMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatDamageReductionFromBarracksMinionMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, IncreasedMoveSpeedMinionMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatBubbleRadiusMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PercentBubbleRadiusMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, MoveSpeed, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatBaseArmorMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, FlatBaseSpellBlockMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, MoveSpeedBaseIncrease, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, ScaleSkinCoef, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PathfindingRadiusMod, (), (CharacterIntermediate* pThis), (this))
			IMPORT_METHOD(float, PrimaryARBaseRegenRateRep, (), (CharacterIntermediate* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedInventorySlot
	inline class InventorySlot* g_pExportedInventorySlot;
	class InventorySlot {
		IMPORT_METHOD(SdkImport::ScriptBaseItem*, ScriptBaseItem, (), (InventorySlot* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedScriptBaseItem
	inline class ScriptBaseItem* g_pExportedScriptBaseItem;
	class ScriptBaseItem {
		IMPORT_METHOD(SdkImport::ItemData*, ItemData, (), (ScriptBaseItem* pThis), (this))
			IMPORT_METHOD(float, OnHitDamage, (), (ScriptBaseItem* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedItemData
	inline class ItemData* g_pExportedItemData;
	class ItemData {
		IMPORT_METHOD(ItemID, ItemId, (), (ItemData* pThis), (this))

			IMPORT_METHOD(float, FlatMovementSpeedMod, (), (ItemData* pThis), (this))
			IMPORT_METHOD(float, PercentMovementSpeedMod, (), (ItemData* pThis), (this))
			IMPORT_METHOD(float, PercentMultiplicativeMovementSpeedMod, (), (ItemData* pThis), (this))
			IMPORT_METHOD(bool, Clickable, (), (ItemData* pThis), (this))
			IMPORT_METHOD(bool, MajorActiveItem, (), (ItemData* pThis), (this))
	};
#undef CONTEXT_FACTORY

	class BuffInstanceClient {
	public:
		class BuffInstance* m_pInstance;
		void* m_pPadding;
	};

#define CONTEXT_FACTORY g_pExportedBuffManager
	inline class BuffManager* g_pExportedBuffManager;
	class BuffManager {
		IMPORT_METHOD(std::vector< BuffInstanceClient>*, Instances, (), (BuffManager* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedBuffInstance
	inline class BuffInstance* g_pExportedBuffInstance;
	class BuffInstance {
		IMPORT_METHOD(BuffType, Type, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(float, StartTime, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(float, ExpireTime, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(float, Duration, (), (BuffInstance* pThis), (this))

			IMPORT_METHOD(bool, IsActive, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(GameObject*, Caster, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(std::uint32_t, Count, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(std::uint32_t, Stacks, (), (BuffInstance* pThis), (this))

			IMPORT_METHOD(char**, Name, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(std::uint32_t, Hash, (), (BuffInstance* pThis), (this))

			IMPORT_METHOD(bool, IsHardCrowdControl, (), (BuffInstance* pThis), (this))
			IMPORT_METHOD(bool, IsCrowdControl, (), (BuffInstance* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedSpellbook
	inline class Spellbook* g_pExportedSpellbook;
	class Spellbook {
		IMPORT_METHOD(SdkImport::SpellCasterClient*, ActiveSpell, (), (Spellbook* pThis), (this))
			IMPORT_METHOD(SdkImport::SpellDataInstance*, GetSpell, (std::int32_t iSlot), (Spellbook* pThis, std::int32_t iSlot), (this, iSlot))
			IMPORT_METHOD(bool, CanUseSpell, (std::int32_t iSlot), (Spellbook* pThis, std::int32_t iSlot), (this, iSlot))
			IMPORT_METHOD(bool, HasRecentlyCastedSpell, (), (Spellbook* pThis), (this))

			IMPORT_METHOD(bool, SendSpellCastPacket, (SpellSlot iSlot, Vector3* vecStartPosition, Vector3* vecEndPosition, GameObject* pTarget, bool bIgnoreSpellState = false), (Spellbook* pThis, SpellSlot iSlot, Vector3* vecStartPosition, Vector3* vecEndPosition, GameObject* pTarget, bool bIgnoreSpellState = false), (this, iSlot, vecStartPosition, vecEndPosition, pTarget, bIgnoreSpellState))
			IMPORT_METHOD(bool, UpdateChargeableSpell, (SpellSlot iSlot, Vector3* vecEndPosition, bool bIgnoreSpellState = false), (Spellbook* pThis, SpellSlot iSlot, Vector3* vecEndPosition, bool bIgnoreSpellState = false), (this, iSlot, vecEndPosition, bIgnoreSpellState))
			IMPORT_METHOD(float, GetManaCost, (std::int32_t iSlot), (Spellbook* pThis, std::int32_t iSlot), (this, iSlot))
			IMPORT_METHOD(void, EvolveSpell, (std::int32_t iSlot), (Spellbook* pThis, std::int32_t iSlot), (this, iSlot))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedSpellDataInstance
	inline class SpellDataInstance* g_pExportedSpellDataInstance;
	class SpellDataInstance {
		IMPORT_METHOD(std::int32_t, Level, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(float, ExpireTime, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(int, Ammo, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(float, AmmoRechargeStart, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(float, AmmoRechargeTime, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(std::int32_t, ToggleState, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(float*, EffectAmount, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(std::uint8_t, IconIndex, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(SdkImport::SpellData*, SpellData, (), (SpellDataInstance* pThis), (this))

			IMPORT_METHOD(void*, GetIcon, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(float, GetRemainingCooldown, (), (SpellDataInstance* pThis), (this))
			IMPORT_METHOD(float, SetTimeForCooldown, (float flCooldown), (SpellDataInstance* pThis, float flCooldown), (this, flCooldown))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedSpellCasterClient
	inline class SpellCasterClient* g_pExportedSpellCasterClient;
	class SpellCasterClient {
		IMPORT_METHOD(SdkImport::SpellCastInfo*, SpellCastInfo, (), (SpellCasterClient* pThis), (this))

			IMPORT_METHOD(float*, StartTime, (), (SpellCasterClient* pThis), (this))
			IMPORT_METHOD(float*, EndTime, (), (SpellCasterClient* pThis), (this))
			IMPORT_METHOD(float*, CastEndTime, (), (SpellCasterClient* pThis), (this))

			IMPORT_METHOD(bool, IsCharging, (), (SpellCasterClient* pThis), (this))
			IMPORT_METHOD(bool, IsInstantCast, (), (SpellCasterClient* pThis), (this))
			IMPORT_METHOD(bool, SpellWasCast, (), (SpellCasterClient* pThis), (this))
			IMPORT_METHOD(bool, IsStopped, (), (SpellCasterClient* pThis), (this))
			IMPORT_METHOD(bool, IsChanneling, (), (SpellCasterClient* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedSpellCastInfo
	inline class SpellCastInfo* g_pExportedSpellCastInfo;
	class SpellCastInfo {
		IMPORT_METHOD(SdkImport::SpellData*, SpellData, (), (SpellCastInfo* pThis), (this))

			IMPORT_METHOD(bool, IsAutoAttack, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(std::uint16_t, CasterHandle, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(std::uint16_t, TargetHandle, (), (SpellCastInfo* pThis), (this))

			IMPORT_METHOD(Vector3, StartPosition, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(Vector3, EndPosition, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(Vector3, CastPosition, (), (SpellCastInfo* pThis), (this))

			IMPORT_METHOD(float, StartTime, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(float, CastDelay, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(float, Delay, (), (SpellCastInfo* pThis), (this))

			IMPORT_METHOD(bool, IsBasicAttack, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(bool, IsSpecialAttack, (), (SpellCastInfo* pThis), (this))

			IMPORT_METHOD(std::int32_t, Level, (), (SpellCastInfo* pThis), (this))
			IMPORT_METHOD(SpellSlot, Slot, (), (SpellCastInfo* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedMissileMovement
	inline class MissileMovement* g_pExportedMissileMovement;
	class MissileMovement {
		IMPORT_METHOD(float, GetCurrentSpeed, (), (MissileMovement* pThis), (this))
			IMPORT_METHOD(float, GetMissileLifetime, (), (MissileMovement* pThis), (this))
			IMPORT_METHOD(bool, IsMovementComplete, (), (MissileMovement* pThis), (this))
			IMPORT_METHOD(Vector3*, GetTargetPosition, (), (MissileMovement* pThis), (this))
			IMPORT_METHOD(Vector3*, GetEndPosition, (), (MissileMovement* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedSpellData
	inline class SpellData* g_pExportedSpellData;
	class SpellData {
		IMPORT_METHOD(std::string*, Name, (), (SpellData* pThis), (this))
			IMPORT_METHOD(std::uint32_t, Hash, (), (SpellData* pThis), (this))
			IMPORT_METHOD(SdkImport::SpellDataResource*, SpellDataResource, (), (SpellData* pThis), (this))
			IMPORT_METHOD(void*, GetTextureByIndex, (std::int32_t iIndex), (SpellData* pThis, std::int32_t iIndex), (this, iIndex))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedSpellDataResource
	inline class SpellDataResource* g_pExportedSpellDataResource;
	class SpellDataResource {
		IMPORT_METHOD(std::uint32_t, Flags, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(std::uint32_t, AffectsTypeFlags, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(std::uint32_t, AffectsStatusFlags, (), (SpellDataResource* pThis), (this))

			IMPORT_METHOD(AllocatedArray<std::uint32_t>*, SpellTagHashes, (), (SpellDataResource* pThis), (this))

			IMPORT_METHOD(float, Coefficient, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float, Coefficient2, (), (SpellDataResource* pThis), (this))

			IMPORT_METHOD(bool, AmmoNotAffectedByCDR, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CooldownNotAffectedByCDR, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, AmmoCountHiddenInUI, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CostAlwaysShownInUI, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CannotBeSuppressed, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CanTriggerChargeSpellWhileDisabled, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CanCastOrQueueWhileCasting, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CanOnlyCastWhileDisabled, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CantCancelWhileWindingUp, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CantCancelWhileChanneling, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CantCastWhileRooted, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ChannelIsInterruptedByDisables, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ChannelIsInterruptedByAttacking, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ApplyAttackDamage, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ApplyAttackEffect, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ApplyMaterialOnHitSound, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, DoesntBreakChannels, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, BelongsToAvatar, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, IsDisabledWhileDead, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CanOnlyCastWhileDead, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CursorChangesInGrass, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CursorChangesInTerrain, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ProjectTargetToCastRange, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, SpellRevealsChampion, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, UseMinimapTargeting, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CastRangeUseBoundingBoxes, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, MinimapIconRotation, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, UseChargeChanneling, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, CanMoveWhileChanneling, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, DisableCastBar, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ShowChannelBar, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, AlwaysSnapFacing, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, HaveHitEffect, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, IsToggleSpell, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, DoNotNeedToFaceTarget, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, TurnSpeedScalar, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, NoWinddownIfCancelled, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, IgnoreRangeCheck, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, OrientRadiusTextureFromPlayer, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, IgnoreAnimContinueUntilCastFrame, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, HideRangeIndicatorWhenCasting, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, UpdateRotationWhenCasting, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, PingableWhileDisabled, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, ConsideredAsAutoAttack, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, DoesNotConsumeMana, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(bool, DoesNotConsumeCooldown, (), (SpellDataResource* pThis), (this))

			IMPORT_METHOD(float*, CastRange, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float*, CastRangeDisplayOverride, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float*, CastRadius, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float*, CastRadiusSecondary, (), (SpellDataResource* pThis), (this))

			IMPORT_METHOD(float, CastTargetAdditionalUnitsRadius, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float, LuaOnMissileUpdateDistanceInterval, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float, MissileSpeed, (), (SpellDataResource* pThis), (this))
			IMPORT_METHOD(float, LineWidth, (), (SpellDataResource* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedCharacterData
	inline class CharacterData* g_pExportedCharacterData;
	class CharacterData {
		IMPORT_METHOD(std::string*, SkinName, (), (CharacterData* pThis), (this))
			IMPORT_METHOD(std::uint32_t, SkinHash, (), (CharacterData* pThis), (this))
			IMPORT_METHOD(SdkImport::CharacterRecord*, CharacterRecord, (), (CharacterData* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedCharacterRecord
	inline class CharacterRecord* g_pExportedCharacterRecord;
	class CharacterRecord {
		IMPORT_METHOD(float, BaseHp, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, HpPerLevel, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, BaseStaticHpRegen, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, BaseFactorHpRegen, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, BaseMoveSpeed, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, AttackRange, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, AttackSpeed, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, AttackSpeedRatio, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(float, AttackSpeedPerLevel, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(AbilityResourceSlotInfo*, PrimaryAbilityResource, (), (CharacterRecord* pThis), (this))
			IMPORT_METHOD(AbilityResourceSlotInfo*, SecondaryAbilityResource, (), (CharacterRecord* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedAbilityResourceSlotInfo
	inline class AbilityResourceSlotInfo* g_pExportedAbilityResourceSlotInfo;
	class AbilityResourceSlotInfo {
		IMPORT_METHOD(std::uint8_t, ArType, (), (AbilityResourceSlotInfo* pThis), (this))
			IMPORT_METHOD(float, ArBase, (), (AbilityResourceSlotInfo* pThis), (this))
			IMPORT_METHOD(float, ArPerLevel, (), (AbilityResourceSlotInfo* pThis), (this))
			IMPORT_METHOD(float, ArBaseStaticRegen, (), (AbilityResourceSlotInfo* pThis), (this))
			IMPORT_METHOD(float, ArBaseFactorRegen, (), (AbilityResourceSlotInfo* pThis), (this))
			IMPORT_METHOD(float, ArRegenPerLevel, (), (AbilityResourceSlotInfo* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedCharacterDataStack
	inline class CharacterDataStack* g_pExportedCharacterDataStack;
	class CharacterDataStack {
		IMPORT_METHOD(AllocatedString*, SkinName, (), (CharacterDataStack* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedPathControllerCommon
	inline class PathControllerCommon* g_pExportedPathControllerCommon;
	class PathControllerCommon {
		IMPORT_METHOD(bool, IsMoving, (), (PathControllerCommon* pThis), (this))
			IMPORT_METHOD(SdkImport::NavigationPath*, NavigationPath, (), (PathControllerCommon* pThis), (this))
			IMPORT_METHOD(Vector3, ServerPosition, (), (PathControllerCommon* pThis), (this))

			IMPORT_METHOD(bool, GridOfLineSightTest2, (Vector3 vecFrom, Vector3 vecTo, Vector3* pCollision), (PathControllerCommon* pThis, Vector3 vecFrom, Vector3 vecTo, Vector3* pCollision), (this, vecFrom, vecTo, pCollision))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedNavigationPath
	inline class NavigationPath* g_pExportedNavigationPath;
	class NavigationPath {
		IMPORT_METHOD(std::uint32_t, NextWaypointIndex, (), (NavigationPath* pThis), (this))
			IMPORT_METHOD(AllocatedArray<Vector3>*, Nodes, (), (NavigationPath* pThis), (this))

			IMPORT_METHOD(float, DashSpeed, (), (NavigationPath* pThis), (this))
			IMPORT_METHOD(bool, IsDashing, (), (NavigationPath* pThis), (this))

			IMPORT_METHOD(Vector3, EndPosition, (), (NavigationPath* pThis), (this))
			IMPORT_METHOD(Vector3, StartPosition, (), (NavigationPath* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedUnitInfoComponent
	inline class UnitInfoComponent* g_pExportedUnitInfoComponent;
	class UnitInfoComponent {
		IMPORT_METHOD(SdkImport::FloatingInfobar*, FloatingInfobar, (), (UnitInfoComponent* pThis), (this))
			IMPORT_METHOD(bool, GetHealthBarScreenPosition, (Vector3* pPosition), (UnitInfoComponent* pThis, Vector3* pPosition), (this, pPosition))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedFloatingInfobar
	inline class FloatingInfobar* g_pExportedFloatingInfobar;
	class FloatingInfobar {
		IMPORT_METHOD(SdkImport::HudHealthbar*, HudHealthbar, (), (FloatingInfobar* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedHudHealthbar
	inline class HudHealthbar* g_pExportedHudHealthbar;
	class HudHealthbar {
		IMPORT_METHOD(float, Width, (), (HudHealthbar* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedExperience
	inline class Experience* g_pExportedExperience;
	class Experience {
		IMPORT_METHOD(float, CurrentExperience, (), (Experience* pThis), (this))
			IMPORT_METHOD(std::int32_t, Level, (), (Experience* pThis), (this))

			IMPORT_METHOD(float, ExpToGivenLevel, (std::int32_t iLevel), (Experience* pThis, std::int32_t iLevel), (this, iLevel))
			IMPORT_METHOD(float, AmountToNextLevel, (), (Experience* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedAvatarClient
	inline class AvatarClient* g_pExportedAvatarClient;
	class AvatarClient {
		IMPORT_METHOD(void*, GetKeystonePerkTexture, (), (AvatarClient* pThis), (this))
	};
#undef CONTEXT_FACTORY

#define CONTEXT_FACTORY g_pExportedHeroInventoryCommon
	inline class HeroInventoryCommon* g_pExportedHeroInventoryCommon;
	class HeroInventoryCommon {
		IMPORT_METHOD(InventorySlot**, InventorySlots, (), (HeroInventoryCommon* pThis), (this))
	};
#undef CONTEXT_FACTORY

	inline class ObjectManager* g_pExportedObjectManager;
	class ObjectManager {
	public:

		virtual std::vector<GameObject*>* Objects() = 0;
		virtual GameObject* GetObjectByHandle(std::uint16_t iHandle) = 0;
	};

	inline class HudScoreboard* g_pExportedHudScoreboard;
	class HudScoreboard {
	public:

		virtual std::uint32_t* OrderedBlueNetworkIds() = 0;
		virtual std::uint32_t* OrderedRedNetworkIds() = 0;
	};

	inline class GlobalClocks* g_pExportedGlobalClocks;
	class GlobalClocks {
	public:

		virtual float GameTime() = 0;
		virtual float BaseTime() = 0;
		virtual float LimitTime() = 0;
	};

	inline class NetClient* g_pExportedNetClient;
	class NetClient {
	public:

		virtual float FloorToServerTime(float flTime) = 0;
		virtual float CeilToServerTime(float flTime) = 0;
		virtual float RoundToServerTime(float flTime) = 0;
		virtual float RoundTripLatency() = 0;
	};

	inline class ControlStation* g_pExportedControlStation;
	class ControlStation {
	public:

		virtual int* ScreenMousePosition() = 0;
	};

	inline class HudManager* g_pExportedHudManager;
	class HudManager {
	public:

		virtual Vector3* CursorPositionClipped() = 0;
		virtual Vector3* CursorPositionUnclipped() = 0;
		virtual Vector3* LastWaypoint() = 0;
		virtual std::uint16_t HoveredObjectHandle() = 0;
		virtual std::uint16_t SelectedObjectHandle() = 0;
		virtual bool SelectQuestObject(GameObject* pObject) = 0;
	};

	inline class NavigationMesh* g_pExportedNavigationMesh;
	class NavigationMesh {
	public:

		virtual void CellToWorld(float flX, float flZ, Vector3& vecWorld) = 0;
		virtual void WorldToCell(const Vector3& vecWorld, float& flGridX, float& flGridY) = 0;
		virtual CellCollisionFlags GetCellState(const Vector3& vecPosition) = 0;
		virtual CellCollisionFlags GetCellState(float flX, float flZ) = 0;
		virtual float GetHeightForPosition(const Vector3& vecPosition) = 0;
		virtual float GetHeightForPosition(float flX, float flZ) = 0;
		virtual Vector3 GetNearestPassable(SdkImport::PathControllerCommon* pPathControllerCommon, const Vector3& vecPosition) = 0;
		virtual bool CastRay(const Vector3& vecStart, const Vector3& vecEnd, float flDistance, Vector3& vecRay) = 0;
		virtual bool IsInFOW(const Vector3& vecPosition) = 0;

		virtual void SetCellData(const Vector<Vector3>& rgData, std::uint16_t iFlags) = 0;
		virtual void RestoreCellData(const Vector<Vector3>& rgData, std::uint16_t iFlags) = 0;
		virtual bool BuildPath(Vector3 vecStart, Vector3 vecEnd, bool bSmooth, Vector<Vector3>* pPath) = 0;

		virtual std::uint32_t GridHeight() = 0;
		virtual std::uint32_t GridWidth() = 0;
	};

	inline class MinimapViewController* g_pExportedMinimapViewController;
	class MinimapViewController {
	public:

		virtual bool ShowPingMiniMap(GameObject* pObject, const Vector3& vecPosition, PingType iPingType) = 0;
		virtual bool SendPingToServer(const Vector3& vecPosition, std::uint32_t iNetworkId, PingType iPingType) = 0;
	};

	inline class MenuGUI* g_pExportedMenuGUI;
	class MenuGUI {
	public:

		virtual std::uint32_t InputMode() = 0;

		bool IsGameInput() {
			return 0 == this->InputMode();
		}
	};

	inline class EntityManager* g_pExportedEntityManager;
	class EntityManager {
	public:

		virtual AllocatedArray<GameObject*>& Heroes() = 0;
		virtual AllocatedArray<GameObject*>& Minions() = 0;
		virtual AllocatedArray<GameObject*>& Turrets() = 0;
		virtual AllocatedArray<GameObject*>& Inhibitors() = 0;
		virtual AllocatedArray<GameObject*>& AttackableUnits() = 0;
	};

	inline class EventHandler* g_pExportedEventHandler;
	class EventHandler {
	public:

		virtual void Add(EventType iEventType, void* pCallback) = 0;
		virtual void Remove(EventType iEventType, void* pCallback) = 0;
	};

	inline class Math* g_pExportedMath;
	class Math {
	public:

		virtual bool IsNan(float flValue) = 0;
		virtual float SinF(const float flValue) = 0;
		virtual float CosF(const float flValue) = 0;
		virtual float AcosF(const float flValue) = 0;
		virtual float AsinF(const float flValue) = 0;
		virtual float AtanF(const float flValue) = 0;
		virtual float Atan2F(const float flValue1, const float flValue2) = 0;
		virtual float PowF(const float flValue, float flExponent) = 0;
		virtual float RoundFStepUp(const float flValue, const float flStep) = 0;
		virtual float RoundFStepDown(const float flValue, const float flStep) = 0;
		virtual bool IsOnSegment(const Vector3& vecPosition, const Vector3& vecSegmentStart, const Vector3& vecSegmentEnd, Vector3& vecSegmentPosition) = 0;
		virtual std::uint32_t CircleLineIntersection(Vector3 vecPosition, float flRadius, Vector3 vecPoint1, Vector3 vecPoint2, Vector3& vecIntersect1, Vector3& vecIntersect2) = 0;
		virtual bool LineIntersection(const Vector3& vecLine1Start, const Vector3& vecLine1End, const Vector3& vecLine2Start, const Vector3& vecLine2End, Vector3& vecIntersect) = 0;
		virtual bool CalculateMinimumEnclosingCircle(Vector<Vector3> rgPoints, Vector3& vecCenter, float& flRadius) = 0;

		static float SqrtF(float flValue) {
			return _mm_cvtss_f32(_mm_sqrt_ss(_mm_load_ss(&flValue)));
		}

		static float FloorF(const float flValue) {
			return _mm_cvtss_f32(_mm_floor_ss(_mm_load_ss(&flValue), _mm_load_ss(&flValue)));
		}

		static float CeilF(const float flValue) {
			return _mm_cvtss_f32(_mm_ceil_ss(_mm_setzero_ps(), _mm_load1_ps(&flValue)));
		}

		static float RoundF(const float flValue) {
			return _mm_cvtss_f32(_mm_round_ps(_mm_load1_ps(&flValue), _MM_FROUND_NINT));
		}

		static float MinF(const float flValue1, const float flValue2) {
			return _mm_cvtss_f32(_mm_min_ps(_mm_load1_ps(&flValue1), _mm_load1_ps(&flValue2)));
		}

		static float MaxF(const float flValue1, const float flValue2) {
			return _mm_cvtss_f32(_mm_max_ps(_mm_load1_ps(&flValue1), _mm_load1_ps(&flValue2)));
		}

		static float FMod(const float flValue1, const float flValue2) {
			return _mm_cvtss_f32((_mm_load1_ps(&flValue1), _mm_load1_ps(&flValue2)));
		}

		static float AbsF(const float flValue) {
			const int __declspec(align(16)) szAbsMask[] = { 0x7fffffff, 0x7fffffff, 0x7fffffff, 0x7fffffff };
			return _mm_cvtss_f32(_mm_and_ps(_mm_load1_ps(&flValue), *(const __m128*)szAbsMask));
		}
	};

	class DrawList {
	public:

		virtual void AddRectTextured(void* pTexture, float flX, float flY, float flWidth, float flHeight, std::uint32_t iColor = 0xFFFFFFFF, float flTextureLeft = 0.f, float flTextureTop = 0.f, float flTextureRight = 1.f, float flTextureBottom = 1.f) = 0;
		virtual void AddRect(float flX, float flY, float flWidth, float flHeight, float flThickness, std::uint32_t iColor) = 0;
		virtual void AddRectFilled(float flX, float flY, float flWidth, float flHeight, std::uint32_t iColor) = 0;
		virtual void AddRectOutline(float flX, float flY, float flWidth, float flHeight, float flThickness, std::uint32_t iColor, std::uint32_t iBorderColor) = 0;

		virtual void AddPolyline(const Vector2* pPoints, const size_t iPointCount, float flThickness, std::uint32_t iColor, bool bClosed) = 0;
		virtual void AddLine(float flX1, float flY1, float flX2, float flY2, float flThickness, std::uint32_t iColor) = 0;

		virtual void AddImage(void* pTexture, float flX, float flY, float flWidth, float flHeight, std::uint32_t iColor = 0xFFFFFFFF, float flTextureLeft = 0.f, float flTextureTop = 0.f, float flTextureRight = 1.f, float flTextureBottom = 1.f) = 0;
		virtual void AddImageRounded(void* pTexture, float flX, float flY, float flRadius, std::uint32_t iColor = 0xFFFFFFFF, float flTextureLeft = 0.f, float flTextureTop = 0.f, float flTextureRight = 1.f, float flTextureBottom = 1.f) = 0;

		virtual void AddCircleTextured(void* pTexture, float flX, float flY, float flRadius, std::uint32_t iColor = 0xFFFFFFFF, float flTextureLeft = 0.f, float flTextureTop = 0.f, float flTextureRight = 1.f, float flTextureBottom = 1.f) = 0;
		virtual void AddCircle(float flX, float flY, float flZ, float flRadius, float flThickness, std::uint32_t iColor, float flSegments = 0.f) = 0;
		virtual void AddCircle(const Vector3& vecPosition, float flRadius, float flThickness, std::uint32_t iColor, float flSegments = 0.f) = 0;

		virtual void AddCircle2D(float flX, float flY, float flRadius, std::uint32_t iColor) = 0;
		virtual void AddCircle2D(const Vector2& vecPosition, float flRadius, std::uint32_t iColor) = 0;
		virtual void AddCircleOutline2D(float flX, float flY, float flRadius, float flThickness, std::uint32_t iColor) = 0;
		virtual void AddCircleOutlineMinimap2D(float flX, float flY, float flRadius, float flThickness, std::uint32_t iColor) = 0;

		virtual void AddTextA(void* pFont, const char* szString, float flSize, float flX, float flY, std::uint32_t iColor) = 0;
		virtual void AddTextA(void* pFont, const CompileTimeString<char, 64>& stString, float flSize, float flX, float flY, std::uint32_t iColor) = 0;
		virtual void AddTextExA(void* pFont, const char* szString, float flSize, float flLeft, float flTop, float flRight, float flBottom, std::uint32_t iFlags, std::uint32_t iColor) = 0;
		virtual void AddTextExA(void* pFont, const CompileTimeString<char, 64>& stString, float flSize, float flLeft, float flTop, float flRight, float flBottom, std::uint32_t iFlags, std::uint32_t iColor) = 0;

		virtual void AddArc(float flX, float flY, float flZ, float flRadius, float flThickness, float flPercentage, std::uint32_t iColor) = 0;
		virtual void AddArc(const Vector3& vecPosition, float flRadius, float flThickness, float flPercentage, std::uint32_t iColor) = 0;

		virtual void AddArc2D(float flX, float flY, float flRadius, float flThickness, float flPercentage, std::uint32_t iColor) = 0;
		virtual void AddArc2D(const Vector2& vecPosition, float flRadius, float flThickness, float flPercentage, std::uint32_t iColor) = 0;
		
		virtual void AddRectGradient(float flX, float flY, float flWidth, float flHeight, std::uint32_t iColor1, std::uint32_t iColor2) = 0;
	};

#define RGBA(iRed, iGreen, iBlue, iAlpha)    (((std::uint32_t)(iAlpha)<<24) | ((std::uint32_t)(iBlue)<<16) | ((std::uint32_t)(iGreen)<<8) | ((std::uint32_t)(iRed)<<0))
#define ARGB(iAlpha, iRed, iGreen, iBlue)    RGBA(iRed, iGreen, iBlue, iAlpha)

	inline class Renderer* g_pExportedRenderer;
	class Renderer {
	private:
		DrawList* m_rgDrawLists[3];
		std::uint32_t m_iDrawListIndex;

	public:
		virtual bool WorldToScreen(const Vector3& vecPosition, Vector3* pPosition) = 0;
		virtual bool ScreenToWorld(const Vector3& vecPosition, Vector3* pPosition) = 0;
		virtual bool WorldToMinimap(const Vector3& vecPosition, Vector3* pPosition) = 0;

	public:
		DrawList* GetCurrentDrawList() { return this->m_rgDrawLists[this->m_iDrawListIndex]; }
	};

	class Mode {
	public:

		virtual bool Enabled() = 0;
		virtual GameObject* GetTarget() = 0;
		virtual Vector3 GetMovePosition(GameObject* pTarget) = 0;
	};

	inline class Evade* g_pExportedEvade;
	class Evade {
	public:

		virtual bool IsEvading() = 0;
		virtual bool IsPathing() = 0;
		virtual void SetIsEvading(bool bState) = 0;
		virtual void SetIsPathing(bool bState) = 0;
	};

	inline class Orbwalker* g_pExportedOrbwalker;
	class Orbwalker {
	public:
		Vector<GameObject*>* m_pMinions;
		Vector<GameObject*>* m_pHeroes;

	public:
		virtual bool IsProcessingAttack() = 0;
		virtual bool IsWindingUp(float flBuffer = 0.f) = 0;
		virtual float GetAttackCooldown() = 0;
		virtual bool CanWeaveSpell(GameObject* pTarget, float flTime) = 0;
		virtual bool IsOrbwalkable(GameObject* pTarget, bool bIgnoreAlly = true) = 0;
		virtual bool CanAttack(GameObject* pTarget = NULL) = 0;
		virtual bool CanMove(GameObject* pTarget = NULL) = 0;
		virtual Mode* GetMode(OrbwalkerMode iMode) = 0;

		// only use if blocking attacks & casting at the same time
		virtual void SetIsProcessingAttack() = 0;
	};

	inline class HealthPrediction* g_pExportedHealthPrediction;
	class HealthPrediction {
	public:

		virtual float GetHealthPrediction(GameObject* pTarget, float flTimeToHit) = 0;
		virtual float GetConservativeHealthPrediction(GameObject* pTarget, float flTimeToHit) = 0;
	};

	inline class Prediction* g_pExportedPrediction;
	class Prediction {
	public:

		virtual Vector3 CalculateIntercept(GameObject* pTarget, const PredictionInput& stInput) = 0;
		virtual bool GetPrediction(GameObject* pTarget, const PredictionInput& stInput, PredictionOutput& stOutput) = 0;
		virtual bool GetCollision(const Vector3& vecPosition, const PredictionInput& stInput, GameObject* pIgnoreObject, Vector<GameObject*>* pCollision = NULL, bool bBreakAfterCollision = true) = 0;
	};

	inline class TargetSelector* g_pExportedTargetSelector;
	class TargetSelector {
	public:

		typedef bool(__cdecl* pfnFilterTarget)(GameObject* pObject);
		virtual GameObject* GetTarget(float flRange = 0.f, bool bHitBox = false, const Vector3& vecOverride = { 0.f, 0.f, 0.f }, pfnFilterTarget pfnFilter = NULL, TargetSelectorMode iTargetSelectorMode = TargetSelectorMode::Menu) = 0;
		virtual Vector<GameObject*>* const GetTargets(float flRange = 0.f, pfnFilterTarget pfnFilter = NULL, TargetSelectorMode iTargetSelectorMode = TargetSelectorMode::Menu) = 0;

		virtual GameObject* GetForcedTarget() = 0;
		virtual bool IsTargetPrioritized() = 0;
		virtual bool IsTargetForced() = 0;
		virtual float GetPriority(GameObject* pTarget) = 0;
	};

	struct DamageResult {
		float m_flPhysical;
		float m_flMagical;
		float m_flTrue;
		float m_flTotal;
	};

	inline class Damage* g_pExportedDamage;
	class Damage {
	public:

		virtual DamageResult GetAutoAttackDamage(GameObject* pSource, GameObject* pTarget, bool bIncludeOnHit = true, bool bIncludeItems = true, bool bIncludeCrit = true, bool bForceCrit = false);
		virtual DamageResult CalculateMagicDamage(GameObject* pSource, GameObject* pTarget, float flDamage, bool bIncludeOnHit = false, bool bIncludeItems = false);
		virtual DamageResult CalculatePhysicalDamage(GameObject* pSource, GameObject* pTarget, float flDamage, bool bIncludeOnHit = false, bool bIncludeItems = false);
	};

	struct CampData {
		std::uint32_t m_iCampId;
		float m_flInitialSpawnTime = 0.f;
		float m_flKillTime = 0.f;
		float m_flRespawnTime = 0.f;
	};

	inline class CampManager* g_pExportedCampManager;
	class CampManager {
	public:
		virtual CampData* GetData(std::uint32_t iCampId) = 0;

		// class id, flags, position, team should work fine. anything else is undefined behaviour as the object is different from AIBaseClient
		virtual GameObject* GetCampObject(std::uint32_t iCampId) = 0;
	};

	inline class SpellDatabaseClient* g_pExportedSpellDatabaseClient;
	class SpellDatabaseClient {
	public:
		virtual SpellData* GetData(std::uint32_t iHash) = 0;
	};

	enum MenuElementFlags : std::uint32_t {
		MenuElementFlags_Drawable = 1 << 0,
		MenuElementFlags_Inputable = 1 << 1
	};

	class MenuElement {
	public:
		alignas(4) bool m_bEnabled;
		alignas(4) bool m_bIsBeingHeldDown;

		union {
			alignas(4) std::uint32_t m_iSelectedItem;
			alignas(4) std::uint32_t m_iSelectedColor;
		};

		alignas(4) float m_flValue;
		alignas(8) bool* m_pPermaShow;
		alignas(4) bool m_bToggle;
		alignas(4) std::uint32_t m_iFlags;

	public:
		virtual void Release() = 0;
		virtual MenuElement* AddCheckbox(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iConfigHash, bool bValue, bool bPermaShow = false) = 0;
		virtual MenuElement* AddSeparator(const CompileTimeString<char, 64>& stDisplayName, bool bPermaShow = false) = 0;
		virtual MenuElement* AddKeybind(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iConfigHash, std::uint32_t iKeyCode, bool bChat = true, bool bPermaShow = false, bool bSaveState = false) = 0;
		virtual MenuElement* AddSlider(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iConfigHash, float flMin, float flMax, float flValue, int iDecimals, float flStep = 1.f, bool bPermaShow = false) = 0;
		virtual MenuElement* AddDropdown(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iConfigHash, Vector<CompileTimeString<char, 64>> rgItems, std::uint32_t iDefaultItem) = 0;
		virtual MenuElement* AddColorPicker(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iHash, std::uint32_t iRGBA) = 0;
		virtual MenuElement* AddMenu(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iConfigHash) = 0;
		virtual MenuElement* SetPermaShowName(const CompileTimeString<char, 64>& stPermaShowName) = 0;
		virtual void Save() = 0;
		virtual MenuElement* SetTooltipName(const CompileTimeString<char, 256>& stTooltipName) = 0;

	public:
		void SetPermaShow(bool* pPointer) {
			this->m_pPermaShow = pPointer;
		}

		void SetPermaShow(MenuElement* pCheckbox) {
			this->SetPermaShow(&pCheckbox->m_bEnabled);
		}

		void SetToggle(bool bToggle) {
			this->m_bToggle = bToggle;
		}
		
		void SetShouldDraw(bool bDraw) {
			if ( bDraw ) {
				this->m_iFlags |= MenuElementFlags_Drawable;
				return;
			}

			this->m_iFlags &= ~MenuElementFlags_Drawable;
		}

		void SetShouldInput(bool bInput) {
			if ( bInput ) {
				this->m_iFlags |= MenuElementFlags_Inputable;
				return;
			}

			this->m_iFlags &= ~MenuElementFlags_Inputable;
		}

	public:
		bool Enabled() { return this->m_bEnabled; }
		bool IsBeingHeldDown() { return this->m_bIsBeingHeldDown; }
		std::uint32_t SelectedItem() { return this->m_iSelectedItem; }
		std::uint32_t SelectedColor() { return this->m_iSelectedColor; }
		float Value() { return this->m_flValue; }
	};

	inline class Menu* g_pExportedMenu;
	class Menu {
	public:

		virtual MenuElement* AddMenu(const CompileTimeString<char, 64>& stDisplayName, std::uint32_t iConfigHash) = 0;
	};

	enum PluginSdk : std::int32_t {
		PluginSdk_WafendEvade = 0, // -> WafendEvade::Sdk*
	};

	inline class PluginSdkExportClient* g_pExportedPluginSdkExportClient;
	class PluginSdkExportClient {
		void* m_pReserved[10] = { 0 };

	public:

		void RegisterSdk(std::int32_t iIndex, void* pPlugin) {
			this->m_pReserved[iIndex] = pPlugin;
		}

		void UnregisterSdk(std::int32_t iIndex) {
			this->m_pReserved[iIndex] = NULL;
		}

		template<typename T>
		T* GetSdkExport(std::int32_t iIndex) {
			return reinterpret_cast<T*>(this->m_pReserved[iIndex]);
		}
	};

	inline class MouseOverEffect* g_pExportedMouseOverEffect;
	class MouseOverEffect {
	public:

		virtual void RenderEffect(GameObject* pObject, std::uint32_t iColor, std::int32_t iThickness, std::int32_t iBlurness) = 0;
	};

#ifndef EVADE

	namespace WafendEvade {
		enum BehaviourFlag : std::uint32_t {
			BehaviourFlag_None = 0,

			// the spell is a stun
			BehaviourFlag_Stun = 1 << 0,

			// the spell is a root
			BehaviourFlag_Root = 1 << 1,

			// the spell is a charm
			BehaviourFlag_Charm = 1 << 2,

			// the spell is a knockup
			BehaviourFlag_Knockup = 1 << 3,

			// the spell is a knockback
			BehaviourFlag_Knockback = 1 << 4,

			// the spell is a fear/flee
			BehaviourFlag_Flee = 1 << 5,

			// the spell is a hook
			BehaviourFlag_Hook = 1 << 6,

			// the spell is a dash
			BehaviourFlag_Dash = 1 << 7,

			// the spell is a silence
			BehaviourFlag_Silence = 1 << 8,

			// the spell is a slow
			BehaviourFlag_Slow = 1 << 9,

			// the spell is dangerous
			BehaviourFlag_Dangerous = 1 << 10,

			BehaviourFlags_CrowdControl = (BehaviourFlag_Stun | BehaviourFlag_Root | BehaviourFlag_Charm | BehaviourFlag_Knockup | BehaviourFlag_Knockback | BehaviourFlag_Flee | BehaviourFlag_Hook)
		};

		enum SpellType : std::int32_t {
			SpellType_Line = 0,
			SpellType_Circle,
			SpellType_ConeArc,
			SpellType_ConeTriangle,
			SpellType_HermiteSpline,
			SpellType_Quadrilateral,
			SpellType_Cage,
			SpellType_Custom
		};

		struct SpellMetaData {
			std::int8_t m_iId = 0;
			std::int32_t m_iSpellType = 0;

			GameObject* m_pCaster = NULL;
			GameObject* m_pMissile = NULL;

			CompileTimeString<char, 64> m_stDisplayName;

			std::uint32_t m_iContextSpell = 0;
			std::uint32_t m_iBehaviourFlags = 0;

			MenuElement* m_pEnabled = NULL;
			MenuElement* m_pDraw = NULL;
			MenuElement* m_pDangerous = NULL;
			MenuElement* m_pFogOfWar = NULL;
			MenuElement* m_pDangerLevel = NULL;
			MenuElement* m_pHealthPercent = NULL;

			Vector<Vector3>* m_pPolygon = NULL;

			std::int32_t m_iSlot = 0;
		};

		struct PositionSafetyData {
			Vector3 m_vecPosition; // reserved, default value 0, 0, 0
			std::uint16_t m_iTargetHandle = 0; // reserved, default value 0

			struct {
				std::uint64_t m_iTotal = NULL;

				std::uint64_t m_iSafe = NULL;
				std::uint64_t m_iUnsafe = NULL;
				std::uint64_t m_iDangerous = NULL;
			} m_stSafety;

			float m_flTimeToTravel = 0.f;
			float m_flTimeToHitMax = 0.f;
			float m_flTimeToHitMin = FLT_MAX;
			float m_flDangerLevelMax = 0.f;

			float m_flAngle = 0.f;   // reserved, default value 0
			bool m_bDispose = false; // reserved
		};

		class SpellCastInstance {
		public:

			virtual bool IsEnabled(void* pMethod, bool bIgnoreDisabled = false) = 0;
			virtual float CalculateDisplacement(const Vector3& vecPosition, bool bInverted = false) = 0;
			virtual float SolveTimeForHit(float flDisplacement, bool bIncludeLifetime = false) = 0;
			virtual float SolveTimeForHitRelative(float flDisplacement, bool bIncludeLifetime = false) = 0;
			virtual bool IsPathSafe(Vector<Vector3>* pNodes, float flSpeed, float flDelay, float flBuffer, std::uint32_t iReserved = NULL, PositionSafetyData* pPositionSafetyData = NULL) = 0;
			virtual bool IsPositionSafe(const Vector3& vecPosition, bool bPathfindPolygon = false, bool bClippingPolygon = false) = 0;
			virtual SpellMetaData* GetSpellMetaData() = 0;
			virtual Vector3 GetStartPosition();
			virtual Vector3 GetEndPosition();
			virtual Vector3 GetPosition(float flDelta = 0.f);
		};

		class Sdk {
		public:

			virtual Vector<SpellCastInstance*>* ActiveSpellInstances() = 0;
			virtual void* GetEvadeMethod(std::uint32_t iSpellHash) = 0;
			virtual std::uint64_t ActiveEvadingBitmap();
		};
	};

#endif

	inline GameObject* GetPlayer() {
		return *reinterpret_cast<GameObject**>(Globals::g_ppPlayer);
	}

#define MenuString(szString) CompileTimeString<char, 64>(sizeof(szString) / sizeof(szString[0]), []{ return szString; }, std::make_index_sequence<GetBufferSize<64>()>{})
#define TooltipString(szString) CompileTimeString<char, 256>(sizeof(szString) / sizeof(szString[0]), []{ return szString; }, std::make_index_sequence<GetBufferSize<256>()>{})
#define MenuConfig(szString) FNV1A32CI(szString)

	struct __declspec(align(0x1000)) SdkInstances {
		GameObject* m_pGameObjectFactory = NULL;
		CharacterIntermediate* m_pCharacterIntermediateFactory = NULL;
		InventorySlot* m_pInventorySlotFactory = NULL;
		ScriptBaseItem* m_pScriptBaseItemFactory = NULL;
		ItemData* m_pItemDataFactory = NULL;
		BuffInstance* m_pBuffInstanceFactory = NULL;
		Spellbook* m_pSpellbookFactory = NULL;
		SpellDataInstance* m_pSpellDataInstanceFactory = NULL;
		SpellCasterClient* m_pSpellCasterClientFactory = NULL;
		SpellCastInfo* m_pSpellCastInfoFactory = NULL;
		MissileMovement* m_pMissileMovementFactory = NULL;
		SpellData* m_pSpellDataFactory = NULL;
		SpellDataResource* m_pSpellDataResourceFactory = NULL;
		CharacterData* m_pCharacterDataFactory = NULL;
		CharacterDataStack* m_pCharacterDataStackFactory = NULL;
		PathControllerCommon* m_pPathControllerCommonFactory = NULL;
		NavigationPath* m_pNavigationPathFactory = NULL;
		UnitInfoComponent* m_pUnitInfoComponentFactory = NULL;
		FloatingInfobar* m_pFloatingInfobarFactory = NULL;
		HudHealthbar* m_pHudHealthbarFactory = NULL;
		Experience* m_pExperienceFactory = NULL;
		AvatarClient* m_pAvatarClientFactory = NULL;
		ObjectManager* m_pObjectManagerFactory = NULL;
		HudScoreboard* m_pHudScoreboardFactory = NULL;
		GlobalClocks* m_pGlobalClocksFactory = NULL;
		NetClient* m_pNetClientFactory = NULL;
		ControlStation* m_pControlStationFactory = NULL;
		HudManager* m_pHudManagerFactory = NULL;
		NavigationMesh* m_pNavigationMeshFactory = NULL;
		MinimapViewController* m_pMinimapViewControllerFactory = NULL;
		MenuGUI* m_pMenuGUIFactory = NULL;
		EntityManager* m_pEntityManagerFactory = NULL;
		EventHandler* m_pEventHandlerFactory = NULL;
		Math* m_pMathFactory = NULL;
		Renderer* m_pRendererFactory = NULL;
		Evade* m_pEvadeFactory = NULL;
		Orbwalker* m_pOrbwalkerFactory = NULL;
		HealthPrediction* m_pHealthPredictionFactory = NULL;
		Prediction* m_pPredictionFactory = NULL;
		TargetSelector* m_pTargetSelectorFactory = NULL;
		Damage* m_pDamageFactory = NULL;
		Menu* m_pMenuFactory = NULL;
		CharacterRecord* m_pCharacterRecordFactory = NULL;
		AbilityResourceSlotInfo* m_pAbilityResourceSlotInfoFactory = NULL;
		CampManager* m_pCampManagerFactory = NULL;
		BuffManager* m_pBuffManagerFactory = NULL;
		HeroInventoryCommon* m_pHeroInventoryCommonFactory = NULL;
		SpellDatabaseClient* m_pSpellDatabaseClientFactory = NULL;
		PluginSdkExportClient* m_pPluginSdkExportClientFactory = NULL;
		MouseOverEffect* m_pMouseOverEffectFactory = NULL;
	};

	struct __declspec(align(0x1000)) SdkContext {
		SdkInstances m_stInstances;

		GameObject** m_ppPlayer;
		decltype(&malloc) Malloc;
		decltype(&memmove) Memmove;
		decltype(&free) Free;
		decltype(&sprintf) Sprintf;
		void(__cdecl* Write)(const char* szFormat, ...);
		void(__cdecl* Allocate)();

		float m_flDisplayWidth = 0.f;
		float m_flDisplayHeight = 0.f;
		std::uint32_t m_iMapId;
		bool m_bIsReplay;
		bool m_bIsMatchmade;

		__forceinline void Initialize() {
			INITIALIZE_FACTORY(GameObject);
			INITIALIZE_FACTORY(CharacterIntermediate);
			INITIALIZE_FACTORY(InventorySlot);
			INITIALIZE_FACTORY(ScriptBaseItem);
			INITIALIZE_FACTORY(ItemData);
			INITIALIZE_FACTORY(BuffInstance);
			INITIALIZE_FACTORY(Spellbook);
			INITIALIZE_FACTORY(SpellDataInstance);
			INITIALIZE_FACTORY(SpellCasterClient);
			INITIALIZE_FACTORY(SpellCastInfo);
			INITIALIZE_FACTORY(MissileMovement);
			INITIALIZE_FACTORY(SpellData);
			INITIALIZE_FACTORY(SpellDataResource);
			INITIALIZE_FACTORY(CharacterData);
			INITIALIZE_FACTORY(CharacterDataStack);
			INITIALIZE_FACTORY(PathControllerCommon);
			INITIALIZE_FACTORY(NavigationPath);
			INITIALIZE_FACTORY(UnitInfoComponent);
			INITIALIZE_FACTORY(FloatingInfobar);
			INITIALIZE_FACTORY(HudHealthbar);
			INITIALIZE_FACTORY(Experience);
			INITIALIZE_FACTORY(AvatarClient);
			INITIALIZE_FACTORY(ObjectManager);
			INITIALIZE_FACTORY(HudScoreboard);
			INITIALIZE_FACTORY(GlobalClocks);
			INITIALIZE_FACTORY(NetClient);
			INITIALIZE_FACTORY(ControlStation);
			INITIALIZE_FACTORY(HudManager);
			INITIALIZE_FACTORY(NavigationMesh);
			INITIALIZE_FACTORY(MinimapViewController);
			INITIALIZE_FACTORY(MenuGUI);
			INITIALIZE_FACTORY(EntityManager);
			INITIALIZE_FACTORY(EventHandler);
			INITIALIZE_FACTORY(Math);
			INITIALIZE_FACTORY(Renderer);
			INITIALIZE_FACTORY(Evade);
			INITIALIZE_FACTORY(Orbwalker);
			INITIALIZE_FACTORY(HealthPrediction);
			INITIALIZE_FACTORY(Prediction);
			INITIALIZE_FACTORY(TargetSelector);
			INITIALIZE_FACTORY(Damage);
			INITIALIZE_FACTORY(Menu);
			INITIALIZE_FACTORY(CharacterRecord);
			INITIALIZE_FACTORY(AbilityResourceSlotInfo);
			INITIALIZE_FACTORY(CampManager);
			INITIALIZE_FACTORY(BuffManager);
			INITIALIZE_FACTORY(HeroInventoryCommon);
			INITIALIZE_FACTORY(SpellDatabaseClient);
			INITIALIZE_FACTORY(PluginSdkExportClient);			
			INITIALIZE_FACTORY(MouseOverEffect);

			Globals::g_ppPlayer = reinterpret_cast<void**>(this->m_ppPlayer);
			Globals::Malloc = this->Malloc;
			Globals::Memmove = this->Memmove;
			Globals::Free = this->Free;
			Globals::Sprintf = this->Sprintf;
			Globals::Allocate = this->Allocate;
			Globals::Write = this->Write;
			Globals::g_flDisplayWidth = this->m_flDisplayWidth;
			Globals::g_flDisplayHeight = this->m_flDisplayHeight;
			Globals::g_iMapId = this->m_iMapId;
			Globals::g_bIsReplay = this->m_bIsReplay;
		}
	};

#pragma region Definitions

	inline Vector3& Vector3::operator += (const Vector3& stOther) {
		this->x += stOther.x;
		this->y += stOther.y;
		this->z += stOther.z;
		return *this;
	}

	inline Vector3& Vector3::operator -= (const Vector3& stOther) {
		this->x -= stOther.x;
		this->y -= stOther.y;
		this->z -= stOther.z;
		return *this;
	}

	inline Vector3& Vector3::operator *= (float flValue) {
		this->x *= flValue;
		this->y *= flValue;
		this->z *= flValue;
		return *this;
	}

	inline Vector3& Vector3::operator /= (float flValue) {
		const float flInverse = 1.f / flValue;
		this->x *= flInverse;
		this->y *= flInverse;
		this->z *= flInverse;
		return *this;
	}

	inline Vector3 Vector3::operator + () const {
		return *this;
	}

	inline Vector3 Vector3::operator - () const {
		return Vector3(-this->x, -this->y, -this->z);
	}

	inline Vector3 Vector3::operator + (const Vector3& stOther) const {
		return Vector3(this->x + stOther.x, this->y + stOther.y, this->z + stOther.z);
	}

	inline Vector3 Vector3::operator - (const Vector3& stOther) const {
		return Vector3(this->x - stOther.x, this->y - stOther.y, this->z - stOther.z);
	}

	inline Vector3 Vector3::operator * (float flValue) const {
		return Vector3(this->x * flValue, this->y * flValue, this->z * flValue);
	}

	inline Vector3 Vector3::operator / (float flValue) const {
		const float flInverse = 1.f / flValue;
		return Vector3(this->x * flInverse, this->y * flInverse, this->z * flInverse);
	}

	inline bool Vector3::operator == (const Vector3& stOther) const {
		return this->x == stOther.x && this->y == stOther.y && this->z == stOther.z;
	}

	inline bool Vector3::operator != (const Vector3& stOther) const {
		return this->x != stOther.x || this->y != stOther.y || this->z != stOther.z;
	}

	inline bool Vector3::IsValid() const {
		return this->x != 0.f && this->z != 0.f;
	}

	inline float Vector3::LengthXZSqr() const {
		return this->x * this->x + this->z * this->z;
	}

	inline float Vector3::LengthXZ() const {
		return g_pExportedMath->SqrtF(this->x * this->x + this->z * this->z);
	}

	inline float Vector3::DistanceXZSqr(const Vector3& stOther) const {
		const float flDeltaX = this->x - stOther.x;
		const float flDeltaZ = this->z - stOther.z;
		return flDeltaX * flDeltaX + flDeltaZ * flDeltaZ;
	}

	inline float Vector3::DistanceXZ(const Vector3& stOther) const {
		return g_pExportedMath->SqrtF(this->DistanceXZSqr(stOther));
	}

	inline Vector3 Vector3::PerpendicularClockwise() const {
		return Vector3(-this->z, this->y, this->x);
	}

	inline Vector3 Vector3::PerpendicularCounterClockwise() const {
		return Vector3(this->z, this->y, -this->x);
	}

	inline Vector3 Vector3::NormalizeXZ() {
		const float flLength = this->LengthXZ();

		if ( 0.f != flLength ) {
			const float flInverse = 1.0f / flLength;
			this->x *= flInverse;
			this->y = 0.f;
			this->z *= flInverse;
		}
		return *this;
	}

	inline Vector3 Vector3::Rotated(float flAngle) const {
		const float flCos = g_pExportedMath->CosF(flAngle);
		const float flSin = g_pExportedMath->SinF(flAngle);

		Vector3 vecResult = *this;
		vecResult.x = this->x * flCos - this->z * flSin;
		vecResult.z = this->z * flCos + this->x * flSin;
		return vecResult;
	}

	inline float Vector3::AngleBetweenXZ(const Vector3& stOther) const {
		return g_pExportedMath->AcosF(this->DotProductXZ(stOther));
	}

	inline float Vector3::DotProductXZ(const Vector3& stOther) const {
		return this->x * stOther.x + this->z * stOther.z;
	}

	inline float Vector3::CrossProductXZ(const Vector3& stOther) const {
		return this->x * stOther.z - this->z * stOther.x;
	}

	inline Vector3 Vector3::Extend(const Vector3& stTo, float flDistance) const {
		return *this + (stTo - *this).NormalizeXZ() * flDistance;
	}

	inline bool Vector3::IsOnScreen(float flThreshold) const {
		return this->x > -flThreshold && this->y > -flThreshold && this->x < Globals::g_flDisplayWidth + flThreshold && this->y < Globals::g_flDisplayHeight + flThreshold;
	}

#pragma endregion
}
