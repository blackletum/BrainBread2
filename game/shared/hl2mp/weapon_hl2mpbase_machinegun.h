
#include "weapon_hl2mpbase.h"

#ifndef BASEHLCOMBATWEAPON_H
#define BASEHLCOMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

#if defined( CLIENT_DLL )
	#define CHL2MPMachineGun C_HL2MPMachineGun
#endif

//=========================================================
// Machine gun base class
//=========================================================
class CHL2MPMachineGun : public CWeaponHL2MPBase
{
public:
	DECLARE_CLASS( CHL2MPMachineGun, CWeaponHL2MPBase );
	DECLARE_DATADESC();

	CHL2MPMachineGun();
	
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	virtual void PrimaryAttack(void);
	virtual void SecondaryAttack(void);
	virtual bool AllowBurst(void) { return false; }
	virtual float GetBurstFireRate(void) { return GetFireRate(); }

	// Default calls through to m_hOwner, but plasma weapons can override and shoot projectiles here.
	virtual void FireBullets( const FireBulletsInfo_t &info );
	virtual int WeaponSoundRealtime( WeaponSound_t shoot_type );
	virtual void ItemPostFrame(void);
	virtual bool Reload(void);
	virtual bool CanHolster(void);
	virtual bool Holster(CBaseCombatWeapon *pSwitchingTo = NULL);
	virtual void Drop(const Vector &vecVelocity);

private:	
	CHL2MPMachineGun( const CHL2MPMachineGun & );

protected:
	CNetworkVar(int, m_iBurstBullet);
	CNetworkVar(bool, m_bIsFiringBurst);
	CNetworkVar(float, m_flSoonestSecondaryAttack);

	float	m_flNextSoundTime;	// real-time clock of when to make next sound
};

#endif // BASEHLCOMBATWEAPON_H