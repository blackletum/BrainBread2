//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef IPLAYERANIMSTATE_H
#define IPLAYERANIMSTATE_H
#ifdef _WIN32
#pragma once
#endif

typedef enum
{
	LEGANIM_9WAY,		// Legs use a 9-way blend, with "move_x" and "move_y" pose parameters.
	LEGANIM_8WAY,		// Legs use an 8-way blend with "move_yaw" pose param.
	LEGANIM_GOLDSRC	// Legs always point in the direction he's running and the torso rotates.
} LegAnimType_t;

#ifdef CLIENT_DLL
class C_BaseAnimatingOverlay;
#define CBaseAnimatingOverlay C_BaseAnimatingOverlay
#else
class CBaseAnimatingOverlay;
#endif

// If a guy is moving slower than this, then he's considered to not be moving
// (so he goes to his idle animation at full playback rate rather than his walk 
// animation at low playback rate).
#define MOVING_MINIMUM_SPEED 0.5f
#define MAIN_IDLE_SEQUENCE_LAYER 0	// For 8-way blended models, this layer blends an idle on top of the run/walk animation to simulate a 9-way blend. For 9-way blended models, we don't use this layer.
#define AIMSEQUENCE_LAYER		1	// Aim sequence uses layers 0 and 1 for the weapon idle animation (needs 2 layers so it can blend).
#define NUM_AIMSEQUENCE_LAYERS	4	// Then it uses layers 2 and 3 to blend in the weapon run/walk/crouchwalk animation.

abstract_class IPlayerAnimState
{
public:
	virtual void Release() = 0;

	// Update() and DoAnimationEvent() together maintain the entire player's animation state.
	//
	// Update() maintains the the lower body animation (the player's m_nSequence)
	// and the upper body overlay based on the player's velocity and look direction.
	//
	// It also modulates these based on events triggered by DoAnimationEvent.
	virtual void Update( float eyeYaw, float eyePitch ) = 0;

	// This is called by the client when a new player enters the PVS to clear any events
	// the dormant version of the entity may have been playing.
	virtual void ClearAnimationState() = 0;

	// The client uses this to figure out what angles to render the entity with (since as the guy turns,
	// it will change his body_yaw pose parameter before changing his rendered angle).
	virtual const QAngle& GetRenderAngles() = 0;
};

#endif // IPLAYERANIMSTATE_H
