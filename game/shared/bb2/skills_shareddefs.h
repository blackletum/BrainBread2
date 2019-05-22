//=========       Copyright � Reperio Studios 2015 @ Bernt Andreas Eide!       ============//
//
// Purpose: Shared info about skills.
//
//========================================================================================//

#ifndef SKILLS_SHAREDDEFS_H
#define SKILLS_SHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif

#define MAX_SKILL_ARRAY 40
#define MAX_PLAYER_LEVEL 500
#define MAX_PLAYER_TALENTS 125

enum playerSkills_t
{
	// AGILITY
	PLAYER_SKILL_HUMAN_SPEED = 0,
	PLAYER_SKILL_HUMAN_ACROBATICS,
	PLAYER_SKILL_HUMAN_SLIDE,
	PLAYER_SKILL_HUMAN_SNIPER_MASTER,
	PLAYER_SKILL_HUMAN_ENHANCED_REFLEXES,
	PLAYER_SKILL_HUMAN_MELEE_SPEED,
	PLAYER_SKILL_HUMAN_LIGHTWEIGHT,
	PLAYER_SKILL_HUMAN_WEIGHTLESS,
	PLAYER_SKILL_HUMAN_HEALTHREGEN,
	PLAYER_SKILL_HUMAN_REALITY_PHASE,

	// STRENGTH
	PLAYER_SKILL_HUMAN_HEALTH,
	PLAYER_SKILL_HUMAN_IMPENETRABLE,
	PLAYER_SKILL_HUMAN_PAINKILLER,
	PLAYER_SKILL_HUMAN_LIFE_LEECH,
	PLAYER_SKILL_HUMAN_POWER_KICK,
	PLAYER_SKILL_HUMAN_BLEED,
	PLAYER_SKILL_HUMAN_CRIPPLING_BLOW,
	PLAYER_SKILL_HUMAN_ARMOR_MASTER,
	PLAYER_SKILL_HUMAN_MELEE_MASTER,
	PLAYER_SKILL_HUMAN_BLOOD_RAGE,

	// FIREARM / PROFICIENCY
	PLAYER_SKILL_HUMAN_RIFLE_MASTER,
	PLAYER_SKILL_HUMAN_SHOTGUN_MASTER,
	PLAYER_SKILL_HUMAN_PISTOL_MASTER,
	PLAYER_SKILL_HUMAN_RESOURCEFUL,
	PLAYER_SKILL_HUMAN_BLAZING_AMMO,
	PLAYER_SKILL_HUMAN_COLDSNAP,
	PLAYER_SKILL_HUMAN_SHOUT_AND_SPRAY,
	PLAYER_SKILL_HUMAN_EMPOWERED_BULLETS,
	PLAYER_SKILL_HUMAN_MAGAZINE_REFILL,
	PLAYER_SKILL_HUMAN_GUNSLINGER,

	// ZOMBIES - BASE
	PLAYER_SKILL_ZOMBIE_HEALTH,
	PLAYER_SKILL_ZOMBIE_DAMAGE,
	PLAYER_SKILL_ZOMBIE_DAMAGE_REDUCTION,
	PLAYER_SKILL_ZOMBIE_SPEED,
	PLAYER_SKILL_ZOMBIE_JUMP,
	PLAYER_SKILL_ZOMBIE_LEAP,
	PLAYER_SKILL_ZOMBIE_DEATH,
	PLAYER_SKILL_ZOMBIE_LIFE_LEECH,
	PLAYER_SKILL_ZOMBIE_HEALTH_REGEN,
	PLAYER_SKILL_ZOMBIE_MASS_INVASION,

	PLAYER_SKILL_END,
};

enum playerSkillSubTypes_t
{
	PLAYER_SKILL_HUMAN_LEAP = 0, // Linked to Acrobatics
	PLAYER_SKILL_HUMAN_JUMP, // Linked to Acrobatics
};

enum playerSkillWeaponTypes_t
{
	WEAPON_TYPE_PISTOL = 0,
	WEAPON_TYPE_SHOTGUN,
	WEAPON_TYPE_RIFLE,
	WEAPON_TYPE_SNIPER,
	WEAPON_TYPE_SPECIAL,
	WEAPON_TYPE_REVOLVER,
	WEAPON_TYPE_SMG,
};

enum playerPerkFlags
{
	PERK_HUMAN_GUNSLINGER = 0x001,	
	PERK_HUMAN_BLOODRAGE = 0x002,	
	PERK_HUMAN_REALITYPHASE = 0x004,	
	PERK_POWERUP_CRITICAL = 0x008,
	PERK_POWERUP_CHEETAH = 0x010,
	PERK_POWERUP_PREDATOR = 0x020,
	PERK_POWERUP_PAINKILLER = 0x040,
	PERK_POWERUP_NANITES = 0x080,
	PERK_ZOMBIE_RAGE = 0x100,
	PERK_MAX_BITS = 9
};

enum playerSkillFlags
{
	SKILL_FLAG_BLEED = 0x001,
	SKILL_FLAG_BLAZINGAMMO = 0x002,
	SKILL_FLAG_COLDSNAP = 0x004,
	SKILL_FLAG_EMPOWERED_BULLETS = 0x008,
	SKILL_FLAG_CRIPPLING_BLOW = 0x010,
	SKILL_FLAG_MAX_BITS = 5
};

enum playerEffects_t
{
	PLAYER_EFFECT_ZOMBIE_FLASHLIGHT = 0,
	PLAYER_EFFECT_FULLCHECK,
	PLAYER_EFFECT_DEATH,
	PLAYER_EFFECT_ENTERED_GAME,
	PLAYER_EFFECT_BECOME_ZOMBIE,
};

// Team Perk Definitions:
enum teamHumanEliminationPerks_t
{
	TEAM_HUMAN_PERK_UNLIMITED_AMMO = 1,
	END_HUMAN_TEAM_PERKS,
};

enum teamDeceasedEliminationPerks_t
{
	TEAM_DECEASED_PERK_INCREASED_DAMAGE = END_HUMAN_TEAM_PERKS,
};

struct playerSkillAffectionItem_t
{
	float duration; // When is this over?
	float damage; // How much damage to take when we're affected?
	float nextTimeToTakeDamage; // Time between damage inflictions, if any.
	float timeToTakeDamage; // When do we want to take damage.
	int flag; // Which flag are we affected by?
	int overlayFlag; // Which overlay is drawn on the client?
	int misc; // Misc data, such as weapon ID, etc...
	EHANDLE m_pAttacker; // Who attacked us?
};

#endif // SKILLS_SHAREDDEFS_H