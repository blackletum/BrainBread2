//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Activities that are available to all NPCs.
//
//=============================================================================//

#include "cbase.h"
#include "ai_activity.h"
#include "ai_basenpc.h"
#include "stringregistry.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
// Init static variables
//=============================================================================
CStringRegistry* CAI_BaseNPC::m_pActivitySR	= NULL;
int				 CAI_BaseNPC::m_iNumActivities = 0;

//-----------------------------------------------------------------------------
// Purpose: Add an activity to the activity string registry and increment
//			the acitivty counter
//-----------------------------------------------------------------------------
void CAI_BaseNPC::AddActivityToSR(const char *actName, int actID) 
{
	Assert( m_pActivitySR );
	if ( !m_pActivitySR )
		return;

	// technically order isn't dependent, but it's too damn easy to forget to add new ACT_'s to all three lists.

	// NOTE: This assertion generally means that the activity enums are out of order or that new enums were not added to all
	//		 relevant tables.  Make sure that you have included all new enums in:
	//			game_shared/ai_activity.h
	//			game_shared/activitylist.cpp
	//			dlls/ai_activity.cpp
	MEM_ALLOC_CREDIT();

	static int lastActID = -2;
	Assert( actID >= LAST_SHARED_ACTIVITY || actID == lastActID + 1 || actID == ACT_INVALID );
	lastActID = actID;

	m_pActivitySR->AddString(actName, actID);
	m_iNumActivities++;
}

//-----------------------------------------------------------------------------
// Purpose: Given and activity ID, return the activity name
//-----------------------------------------------------------------------------
const char *CAI_BaseNPC::GetActivityName(int actID) 
{
	if ( actID == -1 )
		return "ACT_INVALID";

	// m_pActivitySR only contains public activities, ActivityList_NameForIndex() has them all
	const char *name = ActivityList_NameForIndex(actID);	

	if( !name )
	{
		AssertOnce( !"CAI_BaseNPC::GetActivityName() returning NULL!" );
	}

	return name;
}

//-----------------------------------------------------------------------------
// Purpose: Given and activity name, return the activity ID
//-----------------------------------------------------------------------------
int CAI_BaseNPC::GetActivityID(const char* actName) 
{
	Assert( m_pActivitySR );
	if ( !m_pActivitySR )
		return ACT_INVALID;

	return m_pActivitySR->GetStringID(actName);
}

#define ADD_ACTIVITY_TO_SR(activityname) AddActivityToSR(#activityname,activityname)

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CAI_BaseNPC::InitDefaultActivitySR(void) 
{
	ADD_ACTIVITY_TO_SR(ACT_INVALID);
	ADD_ACTIVITY_TO_SR(ACT_RESET);				// Set m_Activity to this invalid value to force a reset to m_IdealActivity
	ADD_ACTIVITY_TO_SR(ACT_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_TRANSITION);
	ADD_ACTIVITY_TO_SR(ACT_COVER);					// FIXME: obsolete? redundant with ACT_COVER_LOW?
	ADD_ACTIVITY_TO_SR(ACT_COVER_LOW);				// FIXME: rename ACT_IDLE_CROUCH?
	ADD_ACTIVITY_TO_SR(ACT_WALK);
	ADD_ACTIVITY_TO_SR(ACT_WALK_AIM);
	ADD_ACTIVITY_TO_SR(ACT_WALK_CROUCH);
	ADD_ACTIVITY_TO_SR(ACT_WALK_CROUCH_AIM);
	ADD_ACTIVITY_TO_SR(ACT_RUN);
	ADD_ACTIVITY_TO_SR(ACT_RUN_AIM);
	ADD_ACTIVITY_TO_SR(ACT_RUN_CROUCH);
	ADD_ACTIVITY_TO_SR(ACT_RUN_CROUCH_AIM);
	ADD_ACTIVITY_TO_SR(ACT_RUNTOIDLE);
	ADD_ACTIVITY_TO_SR(ACT_SCRIPT_CUSTOM_MOVE);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK1);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK1_LOW);		// FIXME: not used yet, crouched versions of the range attack
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK2_LOW);		// FIXME: not used yet, crouched versions of the range attack
	ADD_ACTIVITY_TO_SR(ACT_DIESIMPLE);
	ADD_ACTIVITY_TO_SR(ACT_DIERAGDOLL);
	ADD_ACTIVITY_TO_SR(ACT_FLY);				// Fly (and flap if appropriate)
	ADD_ACTIVITY_TO_SR(ACT_HOVER);
	ADD_ACTIVITY_TO_SR(ACT_GLIDE);
	ADD_ACTIVITY_TO_SR(ACT_SWIM);
	ADD_ACTIVITY_TO_SR(ACT_JUMP);
	ADD_ACTIVITY_TO_SR(ACT_HOP);				// vertical jump
	ADD_ACTIVITY_TO_SR(ACT_LEAP);				// long forward jump
	ADD_ACTIVITY_TO_SR(ACT_LAND);
	ADD_ACTIVITY_TO_SR(ACT_CLIMB_UP);
	ADD_ACTIVITY_TO_SR(ACT_CLIMB_DOWN);
	ADD_ACTIVITY_TO_SR(ACT_CLIMB_DISMOUNT);
	ADD_ACTIVITY_TO_SR(ACT_SHIPLADDER_UP);
	ADD_ACTIVITY_TO_SR(ACT_SHIPLADDER_DOWN);
	ADD_ACTIVITY_TO_SR(ACT_STRAFE_LEFT);
	ADD_ACTIVITY_TO_SR(ACT_STRAFE_RIGHT);
	ADD_ACTIVITY_TO_SR(ACT_ROLL_LEFT);			// tuck and roll, left
	ADD_ACTIVITY_TO_SR(ACT_ROLL_RIGHT);			// tuck and roll, right
	ADD_ACTIVITY_TO_SR(ACT_TURN_LEFT);			// turn quickly left (stationary)
	ADD_ACTIVITY_TO_SR(ACT_TURN_RIGHT);			// turn quickly right (stationary)
	ADD_ACTIVITY_TO_SR(ACT_TURN);
	ADD_ACTIVITY_TO_SR(ACT_CROUCH);				// FIXME: obsolete? only used be soldier (the act of crouching down from a standing position)
	ADD_ACTIVITY_TO_SR(ACT_CROUCHIDLE);			// FIXME: obsolete? only used be soldier (holding body in crouched position (loops))
	ADD_ACTIVITY_TO_SR(ACT_STAND);				// FIXME: obsolete? should be transition (the act of standing from a crouched position)
	ADD_ACTIVITY_TO_SR(ACT_USE);

	ADD_ACTIVITY_TO_SR(ACT_LOOKBACK_RIGHT);		// look back over shoulder without turning around.
	ADD_ACTIVITY_TO_SR(ACT_LOOKBACK_LEFT);
	ADD_ACTIVITY_TO_SR(ACT_COWER);				// FIXME: unused, should be more extreme version of crouching
	ADD_ACTIVITY_TO_SR(ACT_SMALL_FLINCH);		// FIXME: needed? shouldn't flinching be down with overlays?
	ADD_ACTIVITY_TO_SR(ACT_BIG_FLINCH);
	ADD_ACTIVITY_TO_SR(ACT_MELEE_ATTACK1);
	ADD_ACTIVITY_TO_SR(ACT_MELEE_ATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_START);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_FINISH);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_LOW);
	ADD_ACTIVITY_TO_SR(ACT_ARM);				// pull out gun, for instance
	ADD_ACTIVITY_TO_SR(ACT_DISARM);				// reholster gun
	ADD_ACTIVITY_TO_SR(ACT_DROP_WEAPON);
	ADD_ACTIVITY_TO_SR(ACT_PICKUP_GROUND);		// pick up something in front of you on the ground
	ADD_ACTIVITY_TO_SR(ACT_PICKUP_RACK);		// pick up something from a rack or shelf in front of you.
	ADD_ACTIVITY_TO_SR(ACT_IDLE_ANGRY);			// FIXME: being used as an combat ready idle?  alternate idle animation in which the monster is clearly agitated. (loop)

	ADD_ACTIVITY_TO_SR(ACT_SPECIAL_ATTACK1);	// very monster specific special attacks.
	ADD_ACTIVITY_TO_SR(ACT_SPECIAL_ATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_COMBAT_IDLE);		// FIXME: unused?  agitated idle.
	ADD_ACTIVITY_TO_SR(ACT_WALK_SCARED);
	ADD_ACTIVITY_TO_SR(ACT_RUN_SCARED);
	ADD_ACTIVITY_TO_SR(ACT_VICTORY_DANCE);		// killed a player); do a victory dance.
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_HEAD);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_CHEST);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_STOMACH);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_LEFTARM);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_RIGHTARM);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_LEFTLEG);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_RIGHTLEG);
	ADD_ACTIVITY_TO_SR(ACT_FLINCH_PHYSICS);

	ADD_ACTIVITY_TO_SR(ACT_IDLE_ON_FIRE);		// ON FIRE animations
	ADD_ACTIVITY_TO_SR(ACT_WALK_ON_FIRE);
	ADD_ACTIVITY_TO_SR(ACT_RUN_ON_FIRE);

	ADD_ACTIVITY_TO_SR(ACT_RAPPEL_LOOP);		// Rappel down a rope!

	ADD_ACTIVITY_TO_SR(ACT_180_LEFT);			// 180 degree left turn
	ADD_ACTIVITY_TO_SR(ACT_180_RIGHT);

	ADD_ACTIVITY_TO_SR(ACT_90_LEFT);			// 90 degree turns
	ADD_ACTIVITY_TO_SR(ACT_90_RIGHT);

	ADD_ACTIVITY_TO_SR(ACT_STEP_LEFT);			// Single steps
	ADD_ACTIVITY_TO_SR(ACT_STEP_RIGHT);
	ADD_ACTIVITY_TO_SR(ACT_STEP_BACK);
	ADD_ACTIVITY_TO_SR(ACT_STEP_FORE);

	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK1);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_MELEE_ATTACK1);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_MELEE_ATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK1_LOW);	// FIXME: not used yet, crouched versions of the range attack
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK2_LOW);	// FIXME: not used yet, crouched versions of the range attack

	ADD_ACTIVITY_TO_SR(ACT_MELEE_ATTACK_SWING_GESTURE);

	ADD_ACTIVITY_TO_SR(ACT_GESTURE_SMALL_FLINCH);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_BIG_FLINCH);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_BLAST);			// Startled by an explosion
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_BLAST_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_BLAST_DAMAGED);	// Damaged by an explosion
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_BLAST_DAMAGED_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_HEAD);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_CHEST);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_STOMACH);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_LEFTARM);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_RIGHTARM);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_LEFTLEG);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_FLINCH_RIGHTLEG);

	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_LEFT);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_RIGHT);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_LEFT45);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_RIGHT45);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_LEFT90);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_RIGHT90);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_LEFT45_FLAT);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_RIGHT45_FLAT);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_LEFT90_FLAT);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_TURN_RIGHT90_FLAT);

	// Sometimes); you just want to set an NPC's sequence to a sequence that doesn't actually
	// have an activity. The AI will reset the NPC's sequence to whatever its IDEAL activity
	// is); though. So if you set ideal activity to DO_NOT_DISTURB); the AI will not interfere
	// with the NPC's current sequence. (SJB)
	ADD_ACTIVITY_TO_SR(ACT_DO_NOT_DISTURB);

	// Custom BB2 World Activities:
	ADD_ACTIVITY_TO_SR(ACT_RISE);
	ADD_ACTIVITY_TO_SR(ACT_RISE_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_TANK_PRIMARY_FIRE);
	ADD_ACTIVITY_TO_SR(ACT_TANK_SECONDARY_FIRE);

	// viewmodel (weapon) activities
	// FIXME: move these to the specific viewmodels); no need to make global
	ADD_ACTIVITY_TO_SR(ACT_VM_DRAW);
	ADD_ACTIVITY_TO_SR(ACT_VM_DRAW_RARE);
	ADD_ACTIVITY_TO_SR(ACT_VM_HOLSTER);
	ADD_ACTIVITY_TO_SR(ACT_VM_HOLSTER_EMPTY);
	ADD_ACTIVITY_TO_SR(ACT_VM_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_VM_FIDGET);
	ADD_ACTIVITY_TO_SR(ACT_VM_PULLBACK);
	ADD_ACTIVITY_TO_SR(ACT_VM_PULLBACK_HIGH);
	ADD_ACTIVITY_TO_SR(ACT_VM_PULLBACK_LOW);
	ADD_ACTIVITY_TO_SR(ACT_VM_THROW);
	ADD_ACTIVITY_TO_SR(ACT_VM_THROW_RARE);
	ADD_ACTIVITY_TO_SR(ACT_VM_PULLPIN);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK);		// fire
	ADD_ACTIVITY_TO_SR(ACT_VM_SECONDARYATTACK);		// alt. fire
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_START);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_FINISH);
	ADD_ACTIVITY_TO_SR(ACT_VM_DRYFIRE);				// fire with no ammo loaded.
	ADD_ACTIVITY_TO_SR(ACT_VM_HITLEFT);				// bludgeon, swing to left - hit (primary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_HITLEFT2);			// bludgeon, swing to left - hit (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_HITRIGHT);			// bludgeon, swing to right - hit (primary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_HITRIGHT2);			// bludgeon, swing to right - hit (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_HITCENTER);			// bludgeon, swing center - hit (primary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_HITCENTER2);			// bludgeon, swing center - hit (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_MISSLEFT);			// bludgeon, swing to left - miss (primary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_MISSLEFT2);			// bludgeon, swing to left - miss (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_MISSRIGHT);			// bludgeon, swing to right - miss (primary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_MISSRIGHT2);			// bludgeon, swing to right - miss (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_MISSCENTER);			// bludgeon, swing center - miss (primary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_MISSCENTER2);			// bludgeon, swing center - miss (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_HAULBACK);			// bludgeon, haul the weapon back for a hard strike (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_SWINGHARD);			// bludgeon, release the hard strike (secondary attk)
	ADD_ACTIVITY_TO_SR(ACT_VM_SWINGMISS);
	ADD_ACTIVITY_TO_SR(ACT_VM_SWINGHIT);
	ADD_ACTIVITY_TO_SR(ACT_VM_IDLE_TO_LOWERED);
	ADD_ACTIVITY_TO_SR(ACT_VM_IDLE_LOWERED);
	ADD_ACTIVITY_TO_SR(ACT_VM_LOWERED_TO_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_VM_RECOIL1);
	ADD_ACTIVITY_TO_SR(ACT_VM_RECOIL2);
	ADD_ACTIVITY_TO_SR(ACT_VM_RECOIL3);
	ADD_ACTIVITY_TO_SR(ACT_VM_PICKUP);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELEASE);
	ADD_ACTIVITY_TO_SR(ACT_VM_IDLE_EMPTY);
	ADD_ACTIVITY_TO_SR(ACT_VM_DRAW_EMPTY);

	ADD_ACTIVITY_TO_SR(ACT_VM_ATTACH_SILENCER);
	ADD_ACTIVITY_TO_SR(ACT_VM_DETACH_SILENCER);

	//===========================
	// BB2 Specific Activities
	//===========================
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD0);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD1);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD2);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD3);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD4);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD5);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD6);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD7);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD8);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD9);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD10);

	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY0);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY1);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY2);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY3);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY4);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY5);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY6);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY7);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY8);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY9);
	ADD_ACTIVITY_TO_SR(ACT_VM_RELOAD_EMPTY10);

	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP0);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP1);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP2);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP3);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP4);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP5);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP6);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP7);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP8);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP9);
	ADD_ACTIVITY_TO_SR(ACT_VM_PUMP10);

	ADD_ACTIVITY_TO_SR(ACT_VM_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_VM_MELEE_EMPTY);

	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK0);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK1);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK3);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK4);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK5);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK6);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK7);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK8);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK9);
	ADD_ACTIVITY_TO_SR(ACT_VM_PRIMARYATTACK10);

	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK0);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK1);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK2);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK3);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK4);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK5);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK6);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK7);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK8);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK9);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPECIALATTACK10);

	ADD_ACTIVITY_TO_SR(ACT_VM_IDLE_TO_SPIN);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPIN_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_VM_SPIN_TO_IDLE);

	ADD_ACTIVITY_TO_SR(ACT_VM_LASTBULLET);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_LEFT_LAST);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_RIGHT_LAST);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_LEFT);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_RIGHT);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_BOTH);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_LEFT_DRYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_RIGHT_DRYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_VM_SHOOT_BOTH_DRYFIRE);

	ADD_ACTIVITY_TO_SR(ACT_VM_CHARGE_START);
	ADD_ACTIVITY_TO_SR(ACT_VM_CHARGE_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_VM_CHARGE_ATTACK);

	//===========================
	// HL2 Specific Activities
	//===========================
	// Shotgun Specialty Activities
	ADD_ACTIVITY_TO_SR(ACT_SHOTGUN_RELOAD_START);
	ADD_ACTIVITY_TO_SR(ACT_SHOTGUN_RELOAD_START_EMPTY);
	ADD_ACTIVITY_TO_SR(ACT_SHOTGUN_RELOAD_FINISH);
	ADD_ACTIVITY_TO_SR(ACT_SHOTGUN_RELOAD_FINISH_EMPTY);
	ADD_ACTIVITY_TO_SR(ACT_SHOTGUN_PUMP);

	// weapon override activities
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_AR1);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_AR2);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_AR2_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_AR2_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_HMG1);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_ML);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_SMG1_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_SHOTGUN_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_PISTOL_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_SLAM);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_TRIPWIRE);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_THROW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_SNIPER_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_MELEE_ATTACK_SWING);

	ADD_ACTIVITY_TO_SR(ACT_RANGE_AIM_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_AIM_SMG1_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_AIM_PISTOL_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_AIM_AR2_LOW);

	ADD_ACTIVITY_TO_SR(ACT_COVER_PISTOL_LOW);
	ADD_ACTIVITY_TO_SR(ACT_COVER_SMG1_LOW);

	// weapon override activities
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_AR1);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_AR2);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_AR2_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_HMG1);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_ML);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_SMG1_LOW);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_PISTOL_LOW);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_SLAM);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_TRIPWIRE);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_THROW);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_SNIPER_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_MELEE_ATTACK_SWING);

	ADD_ACTIVITY_TO_SR(ACT_IDLE_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_IDLE_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_IDLE_ANGRY_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_IDLE_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_IDLE_ANGRY_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_IDLE_ANGRY_SHOTGUN);

	// Manned guns
	ADD_ACTIVITY_TO_SR(ACT_IDLE_MANNEDGUN);

	// Melee weapon
	ADD_ACTIVITY_TO_SR(ACT_IDLE_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_IDLE_ANGRY_MELEE);

	ADD_ACTIVITY_TO_SR(ACT_WALK_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_WALK_AIM_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_WALK_CROUCH_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_WALK_CROUCH_AIM_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_RUN_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_RUN_AIM_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_RUN_CROUCH_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_RUN_CROUCH_AIM_RIFLE);

	ADD_ACTIVITY_TO_SR(ACT_WALK_AIM_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_RUN_AIM_SHOTGUN);

	ADD_ACTIVITY_TO_SR(ACT_WALK_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_RUN_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_WALK_AIM_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_RUN_AIM_PISTOL);

	// Reloads
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_PISTOL_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_SMG1_LOW);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_RELOAD_SHOTGUN_LOW);

	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RELOAD);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RELOAD_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RELOAD_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RELOAD_SHOTGUN);

	// Dodge animations
	ADD_ACTIVITY_TO_SR(ACT_DUCK_DODGE);

	ADD_ACTIVITY_TO_SR(ACT_PHYSCANNON_DETACH);	// An activity to be played if we're picking this up with the physcannon
	ADD_ACTIVITY_TO_SR(ACT_PHYSCANNON_ANIMATE); // An activity to be played by an object being picked up with the physcannon); but has different behavior to DETACH
	ADD_ACTIVITY_TO_SR(ACT_PHYSCANNON_ANIMATE_PRE);	// An activity to be played by an object being picked up with the physcannon); before playing the ACT_PHYSCANNON_ANIMATE
	ADD_ACTIVITY_TO_SR(ACT_PHYSCANNON_ANIMATE_POST);// An activity to be played by an object being picked up with the physcannon); after playing the ACT_PHYSCANNON_ANIMATE

	ADD_ACTIVITY_TO_SR(ACT_OPEN_DOOR);

	// HL2MP
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_KICK);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_PISTOL);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_BASH_PISTOL);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_SMG1);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_SMG1);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_AR2);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_BASH_AR2);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_BASH_SHOTGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_SHOTGUN_INSERT);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_GRENADE);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROSSBOW);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_CROSSBOW);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_CROSSBOW);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_CROSSBOW);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_CROSSBOW);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_CROSSBOW);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_CROSSBOW);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_MELEE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_MELEE);

	// BrainBread 2:
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_RIFLE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_RIFLE);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_MINIGUN);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_MELEE_1HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_MELEE_1HANDED);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_MELEE_2HANDED);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_MELEE_2HANDED);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_PROPANE);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_PROPANE);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_INFECTED);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_SAWEDOFF);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_FAMAS);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_FAMAS);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_REVOLVER);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_BASH_REVOLVER);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_RUN_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_IDLE_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_IDLE_CROUCH_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_WALK_CROUCH_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RANGE_ATTACK_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_RELOAD_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_JUMP_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_SLIDE_AKIMBO);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_GESTURE_BASH_AKIMBO);

	ADD_ACTIVITY_TO_SR(ACT_HL2MP_MENU01);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_MENU02);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_MENU03);
	ADD_ACTIVITY_TO_SR(ACT_HL2MP_MENU04);

	// BB2 Custom NPC acts.
	ADD_ACTIVITY_TO_SR(ACT_IDLE_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_WALK_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_CROUCH_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_COVER_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_WALK_CROUCH_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_GESTURE_RANGE_ATTACK_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_RANGE_ATTACK_MINIGUN);
	ADD_ACTIVITY_TO_SR(ACT_MONK_GUN_IDLE);

	// Multiplayer
	ADD_ACTIVITY_TO_SR(ACT_MP_STAND_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_MP_CROUCH_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_MP_CROUCH_DEPLOYED_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_MP_CROUCH_DEPLOYED);
	ADD_ACTIVITY_TO_SR(ACT_MP_CROUCHWALK_DEPLOYED);
	ADD_ACTIVITY_TO_SR(ACT_MP_DEPLOYED_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_MP_RUN);
	ADD_ACTIVITY_TO_SR(ACT_MP_WALK);
	ADD_ACTIVITY_TO_SR(ACT_MP_SLIDE_IDLE);
	ADD_ACTIVITY_TO_SR(ACT_MP_AIRWALK);
	ADD_ACTIVITY_TO_SR(ACT_MP_CROUCHWALK);
	ADD_ACTIVITY_TO_SR(ACT_MP_SPRINT);
	ADD_ACTIVITY_TO_SR(ACT_MP_JUMP);
	ADD_ACTIVITY_TO_SR(ACT_MP_JUMP_START);
	ADD_ACTIVITY_TO_SR(ACT_MP_JUMP_FLOAT);
	ADD_ACTIVITY_TO_SR(ACT_MP_JUMP_LAND);
	ADD_ACTIVITY_TO_SR(ACT_MP_DOUBLEJUMP);
	ADD_ACTIVITY_TO_SR(ACT_MP_SWIM);
	ADD_ACTIVITY_TO_SR(ACT_MP_DEPLOYED);
	ADD_ACTIVITY_TO_SR(ACT_MP_SWIM_DEPLOYED);
	ADD_ACTIVITY_TO_SR(ACT_MP_BASH);
	ADD_ACTIVITY_TO_SR(ACT_MP_SLIDE);
	ADD_ACTIVITY_TO_SR(ACT_MP_INFECTED);
	ADD_ACTIVITY_TO_SR(ACT_MP_KICK);

	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_STAND_PRIMARYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_STAND_PRIMARYFIRE_DEPLOYED);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_STAND_SECONDARYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_STAND_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_CROUCH_PRIMARYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_CROUCH_PRIMARYFIRE_DEPLOYED);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_CROUCH_SECONDARYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_CROUCH_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_SWIM_PRIMARYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_SWIM_SECONDARYFIRE);
	ADD_ACTIVITY_TO_SR(ACT_MP_ATTACK_SWIM_GRENADE);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_STAND);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_STAND_LOOP);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_STAND_END);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_CROUCH);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_CROUCH_LOOP);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_CROUCH_END);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_SWIM);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_SWIM_LOOP);
	ADD_ACTIVITY_TO_SR(ACT_MP_RELOAD_SWIM_END);

	// Flinches
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_HEAD);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_CHEST);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_STOMACH);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_LEFTARM);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_RIGHTARM);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_LEFTLEG);
	ADD_ACTIVITY_TO_SR(ACT_MP_GESTURE_FLINCH_RIGHTLEG);
}
