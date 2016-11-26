//=========       Copyright � Reperio Studios 2015 @ Bernt Andreas Eide!       ============//
//
// Purpose: Shared Data Handler Class : Keeps information about the player and npc sound sets.
//
//========================================================================================//

#ifndef GAME_DEFINITIONS_SHARED_H
#define GAME_DEFINITIONS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "achievement_shareddefs.h"

#ifdef CLIENT_DLL
#include "ComboList.h"
#endif

enum SharedDamageScaleTypes
{
	DAMAGE_SCALE_TO_PLAYER = 0,
	DAMAGE_SCALE_TO_PLAYER_ZOMBIE,
	DAMAGE_SCALE_TO_NPC_ZOMBIES,
	DAMAGE_SCALE_TO_NPC_HUMANS,
	DAMAGE_SCALE_TO_NPC_ZOMBIE_BOSSES,
	DAMAGE_SCALE_TO_NPC_HUMAN_BOSSES,
	NUM_DAMAGE_SCALES,
};

enum ChatCommandType
{
	CHAT_CMD_VOICE = 1,
	CHAT_CMD_TEAM,
	CHAT_CMD_ALL,
};

enum VoiceCommandTypes
{
	VOICE_COMMAND_AGREE = 0,
	VOICE_COMMAND_DISAGREE,
	VOICE_COMMAND_FOLLOW,
	VOICE_COMMAND_TAKEPOINT,
	VOICE_COMMAND_NOWEP,
	VOICE_COMMAND_OUTOFAMMO,
	VOICE_COMMAND_READY,
	VOICE_COMMAND_LOOK,
	VOICE_COMMAND_EXIT,
};

struct DataGamemodeItem_Shared_t
{
	int iXPRoundWinArena;
	int iXPRoundWinElimination;

	int iXPGameWinObjective;
	int iXPGameWinArena;
	int iXPGameWinElimination;
	int iXPGameWinDeathmatch;

	int iKillsRequiredToPerk;
	int iZombieCreditsRequiredToRage;

	int iDefaultZombieCredits;
	float flAmmoResupplyTime;
};

struct DataPlayerItem_Shared_t
{
	int iMaxLevel;
	int iXPIncreasePerLevel;
	int iTeamBonusDamageIncrease;
	int iTeamBonusXPIncrease;
	int iLevel;
	int iInfectionStartPercent;
	float flInfectionDuration;
	float flPerkTime;
};

struct DataPlayerItem_Player_Shared_t
{
	int iHealth;
	int iArmor;
	int iArmorType;
	float flSpeed;
	float flJumpHeight;
	float flLeapLength;
	float flHealthRegenerationRate;
	int iTeam;
	int iGameMode;
};

struct DataPlayerItem_Player_PowerupItem_t
{
	char pchName[32];
	char pchModelPath[MAX_WEAPON_STRING];
	char pchActivationSoundScript[64];

	int iFlag;
	float flHealth;
	float flSpeed;
	float flJumpHeight;
	float flLeapLength;
	float flHealthRegenerationRate;
	float flPerkDuration;
	float flExtraFactor;
};

struct DataPlayerItem_MiscSkillInfo_t
{
	float flBleedDuration;
	float flBleedFrequency;

	float flBurnDuration;
	float flBurnDamage;
	float flBurnFrequency;

	float flStunDuration;

	float flSlowDownDuration;
	float flSlowDownPercent;

	float flKickDamage;
	float flKickRange;
	float flKickKnockbackForce;
	float flKickCooldown;

	float flSlideLength;
	float flSlideSpeed;
	float flSlideCooldown;
};

struct DataPlayerItem_Humans_Skills_t
{
	float flAgilitySpeed;
	float flAgilityJump;
	float flAgilityLeap;
	float flAgilitySlide;
	float flAgilityEnhancedReflexes;
	float flAgilityMeleeSpeed;
	float flAgilityLightweight;
	float flAgilityWeightless;
	float flAgilityHealthRegen;
	float flAgilityRealityPhase;

	float flStrengthHealth;
	float flStrengthImpenetrable;
	float flStrengthPainkiller;
	float flStrengthLifeLeech;
	float flStrengthPowerKick;
	float flStrengthBleed;
	float flStrengthCripplingBlow;
	float flStrengthArmorMaster;
	float flStrengthBloodRage;

	float flFirearmResourceful;
	float flFirearmBlazingAmmo;
	float flFirearmColdsnap;
	float flFirearmEmpoweredBullets;
	float flFirearmMagazineRefill;
	float flFirearmGunslinger;
};

struct DataPlayerItem_Zombies_Skills_t
{
	float flHealth;
	float flDamage;
	float flDamageReduction;
	float flSpeed;
	float flJump;
	float flLeap;
	float flDeath;
	float flLifeLeech;
	float flHealthRegen;
	float flMassInvasion;
};

struct DataPlayerItem_ZombieRageMode_t
{
	float flHealth;
	float flHealthRegen;
	float flSpeed;
	float flJump;
	float flLeap;
};

struct DataPlayerItem_Shared_WeaponInfo_t
{
	char szWeaponClass[32];
	float flDamageScale[NUM_DAMAGE_SCALES];
	int iTeam;
	int iGameMode;
};

struct DataPlayerItem_Shared_LimbInfo_t
{
	char szLimb[32];
	float flScale;
	float flHealth;
	int iTeam;
	int iGameMode;
};

struct DataPlayerItem_Survivor_Shared_t
{
	char szSurvivorName[MAX_MAP_NAME];

	// Models
	char szHumanHandsPath[MAX_WEAPON_STRING];
	char szHumanModelPath[MAX_WEAPON_STRING];
	char szHumanBodyPath[MAX_WEAPON_STRING];

	char szZombieHandsPath[MAX_WEAPON_STRING];
	char szZombieModelPath[MAX_WEAPON_STRING];
	char szZombieBodyPath[MAX_WEAPON_STRING];

	// Gibs
	char szHumanGibHead[MAX_WEAPON_STRING];
	char szHumanGibArmLeft[MAX_WEAPON_STRING];
	char szHumanGibArmRight[MAX_WEAPON_STRING];
	char szHumanGibLegLeft[MAX_WEAPON_STRING];
	char szHumanGibLegRight[MAX_WEAPON_STRING];

	char szDeceasedGibHead[MAX_WEAPON_STRING];
	char szDeceasedGibArmLeft[MAX_WEAPON_STRING];
	char szDeceasedGibArmRight[MAX_WEAPON_STRING];
	char szDeceasedGibLegLeft[MAX_WEAPON_STRING];
	char szDeceasedGibLegRight[MAX_WEAPON_STRING];

#ifdef CLIENT_DLL
	char szFriendlySurvivorName[MAX_MAP_NAME];
	char szFriendlyDescription[128];
	char szSequence[MAX_MAP_NAME];

	// Customization
	int iSkins;
	int iSpecialHeadItems;
	int iSpecialBodyItems;
	int iSpecialRightLegItems;
	int iSpecialLeftLegItems;

	// Camera Data
	QAngle angAngles;
	Vector vecPosition;

	const model_t *m_pClientModelPtrHuman;
	const model_t *m_pClientModelPtrZombie;
#endif
};

struct DataInventoryItem_Base_t
{
	uint iItemID;
	bool bIsMapItem;
	int iType;
	int iSubType;
	int iRarity;
	int iLevelReq;
	int iWeight;
	char szModelPath[MAX_WEAPON_STRING];
	char szSoundScriptSuccess[32];
	char szSoundScriptFailure[32];

	Color clGlowColor;
	bool bGlobalGlow;
	bool bAutoConsume;
	bool bEnableObjectiveIcon;
	char szObjectiveIconTexture[MAX_WEAPON_STRING];

#ifdef CLIENT_DLL
	int iHUDTextureID;
#endif
};

struct DataInventoryItem_Misc_t
{
	uint iItemID;
	int iValue;
};

struct DataInventoryItem_Armor_t
{
	uint iItemID;
	int iReductionPercent;
	int iWeight;
};

struct DataSoundPrefixItem_t
{
	int iID;
	int iType;
	char szScriptName[MAX_MAP_NAME];
	char szFriendlyName[MAX_MAP_NAME];
	char szSurvivorLink[MAX_MAP_NAME]; // Only used for zombie & human sound sets.
};

struct DataBloodParticleItem_t
{
	char szDefault[MAX_MAP_NAME];
	char szExtreme[MAX_MAP_NAME];
};

struct DataGibBloodParticleItem_t
{
	char szDefault[MAX_MAP_NAME];
	char szExtreme[MAX_MAP_NAME];
	char szLimb[MAX_MAP_NAME];
};

struct DataParticleItem_t
{
	char szDefault[MAX_MAP_NAME];
};

struct DataExplosiveItem_t
{
	int iType;
	char szParticle[MAX_MAP_NAME];
	float flRadius;
	float flPlayerDamage;
	float flNPCDamage;
};

struct DataPenetrationItem_t
{
	unsigned short material;
	float depth;
};

class CGameDefinitionsShared
{
public:
	CGameDefinitionsShared();
	~CGameDefinitionsShared();

	void Cleanup(void);
	bool LoadData(void);
	bool Precache(void);

	// Gamemode Data:
	DataGamemodeItem_Shared_t GetGamemodeData(void) { return pszGamemodeData; }

	// Player Data:
	DataPlayerItem_Shared_t GetPlayerSharedData(void) const;
	DataPlayerItem_Player_Shared_t GetPlayerGameModeData(int iTeam) const;
	DataPlayerItem_MiscSkillInfo_t GetPlayerMiscSkillData(void) const;
	DataPlayerItem_Humans_Skills_t GetPlayerHumanSkillData(void) const;
	DataPlayerItem_Zombies_Skills_t GetPlayerZombieSkillData(void) const;
	DataPlayerItem_ZombieRageMode_t GetPlayerZombieRageData(void) const;
	DataPlayerItem_Survivor_Shared_t GetSurvivorDataForIndex(int index) const;
	DataPlayerItem_Player_PowerupItem_t GetPlayerPowerupData(const char *powerupName) const;
	DataPlayerItem_Player_PowerupItem_t GetPlayerPowerupData(int powerupFlag) const;
	CUtlVector<DataPlayerItem_Survivor_Shared_t> &GetSurvivorDataList(void) { return pszPlayerSurvivorData; }
	float GetPlayerSharedValue(const char *name, int iTeam);
	float GetPlayerSkillValue(int iSkillType, int iTeam, int iSubType);

	int GetIndexForSurvivor(const char *name);
	int GetIndexForPowerup(const char *name) const;
	const char *GetPlayerSurvivorModel(const char *name, int iTeam);
	const char *GetPlayerHandModel(const char *model);
	const char *GetPlayerBodyModel(const char *model);
	const char *GetPlayerGibForModel(const char *gib, const char *model);
	bool DoesPlayerHaveGibForLimb(const char *model, int gibID);
	float GetPlayerFirearmDamageScale(const char *weapon, int entityType, int team);
	float GetPlayerLimbData(const char *limb, int team, bool bHealth = false);

	// Inventory Data
	void ParseInventoryData(KeyValues *pkvData = NULL, bool bIsMapItem = false);
	void RemoveMapInventoryItems(void);
	int GetInventorySharedDataValue(const char *name, uint itemID, bool bIsMapItem);
	int GetInventoryMiscDataValue(uint itemID);
	int GetInventoryArmorDataValue(const char *name, uint itemID);
	bool DoesInventoryItemExist(uint itemID, bool bIsMapItem);
	int GetInventoryItemIndex(uint itemID, bool bIsMapItem);
	CUtlVector<DataInventoryItem_Base_t> &GetInventoryItemList(void) { return pszItemSharedData; }
	const char *GetInventoryItemModel(uint itemID, bool bIsMapItem);

	// Sound Data
#ifdef CLIENT_DLL
	const char *GetSoundPrefix(int iType, int index, const char *survivor = "survivor1");
	int GetConVarValueForEntitySoundType(int iType);
	const char *GetEntityNameFromEntitySoundType(int iType);
	int GetEntitySoundTypeFromEntityName(const char *name);
	void AddSoundScriptItems(vgui::ComboList *pList, int iType);
	void AddSoundScriptItems(vgui::ComboList *pList, int iType, const char *survivorLink);
	const char *GetSoundPrefixForChoosenItem(int iType, const char *survivorLink, const char *friendlyName);
	int GetSelectedSoundsetItemID(vgui::ComboList *pList, int iType, const char *survivorLink, const char *script);
	const char *GetPlayerSoundsetPrefix(int iType, const char *survivorLink, const char *script);
#endif

	// Particle Data
	const char *GetBloodParticle(bool bExtremeGore = false);
	const char *GetHeadshotParticle(bool bExtremeGore = false);
	const char *GetBleedoutParticle(bool bExtremeGore = false);
	const char *GetBloodExplosionMist(bool bExtremeGore = false);
	const char *GetGibParticleForLimb(const char *limb, bool bExtremeGore = false);

	// Explosion Data
	int GetExplosiveDataIndex(int type);
	const char *GetExplosionParticle(int type);
	CUtlVector<DataExplosiveItem_t> &GetExplosiveData(void) { return pszExplosionData; }

private:

	// Player Data
	DataGamemodeItem_Shared_t pszGamemodeData;
	DataPlayerItem_Shared_t pszPlayerSharedData;
	DataPlayerItem_MiscSkillInfo_t pszPlayerMiscSkillData;
	DataPlayerItem_Humans_Skills_t pszHumanSkillData;
	DataPlayerItem_Zombies_Skills_t pszZombieSkillData;
	DataPlayerItem_ZombieRageMode_t pszZombieRageModeData;
	CUtlVector<DataPlayerItem_Player_Shared_t> pszPlayerData;
	CUtlVector<DataPlayerItem_Survivor_Shared_t> pszPlayerSurvivorData;
	CUtlVector<DataPlayerItem_Shared_WeaponInfo_t> pszPlayerWeaponData;
	CUtlVector<DataPlayerItem_Shared_LimbInfo_t> pszPlayerLimbData;
	CUtlVector<DataPlayerItem_Player_PowerupItem_t> pszPlayerPowerupData;

	// Inventory Data
	CUtlVector<DataInventoryItem_Base_t> pszItemSharedData;
	CUtlVector<DataInventoryItem_Misc_t> pszItemMiscData;
	CUtlVector<DataInventoryItem_Armor_t> pszItemArmorData;

	// Sound Data
#ifdef CLIENT_DLL
	CUtlVector<DataSoundPrefixItem_t> pszSoundPrefixesData;
	int GetNextIndexForSoundSet(int iType, const char *survivorLink);
#endif

	// Particle Data
	CUtlVector<DataBloodParticleItem_t> pszBloodParticleData;
	CUtlVector<DataBloodParticleItem_t> pszHeadshotParticleData;
	CUtlVector<DataBloodParticleItem_t> pszBleedoutParticleData;
	CUtlVector<DataBloodParticleItem_t> pszBloodExplosionParticleData;
	CUtlVector<DataGibBloodParticleItem_t> pszGibParticleData;

	// Explosion Data
	CUtlVector<DataExplosiveItem_t> pszExplosionData;
};

extern const char *GetVoiceCommandString(int command);
extern const char *GetVoiceCommandChatMessage(int command);
extern const char *GetTeamPerkName(int perk);

extern const char *GetGamemodeName(int gamemode);
extern const char *GetGamemodeNameForPrefix(const char *map);
extern int GetGamemodeForMap(const char *map);

#define CURRENT_ACHIEVEMENT_NUMBER 72

extern achievementStatItem_t GAME_STAT_AND_ACHIEVEMENT_DATA[CURRENT_ACHIEVEMENT_NUMBER];
extern DataPenetrationItem_t *GetPenetrationDataForMaterial(unsigned short material);

#endif // GAME_DEFINITIONS_SHARED_H