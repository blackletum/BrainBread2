
#include "cbase.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
    #include "GameBase_Client.h"
#else
	#include "hl2mp_player.h"
#endif

#include "weapon_hl2mpbase_machinegun.h"
#include "in_buttons.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( HL2MPMachineGun, DT_HL2MPMachineGun )

BEGIN_NETWORK_TABLE( CHL2MPMachineGun, DT_HL2MPMachineGun )
#ifdef CLIENT_DLL
RecvPropBool(RECVINFO(m_bIsFiringBurst)),
RecvPropInt(RECVINFO(m_iBurstBullet)),
RecvPropTime(RECVINFO(m_flSoonestSecondaryAttack)),
#else
SendPropBool(SENDINFO(m_bIsFiringBurst)),
SendPropInt(SENDINFO(m_iBurstBullet), 4, SPROP_UNSIGNED),
SendPropTime(SENDINFO(m_flSoonestSecondaryAttack)),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CHL2MPMachineGun )
DEFINE_PRED_FIELD(m_bIsFiringBurst, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE),
DEFINE_PRED_FIELD(m_iBurstBullet, FIELD_INTEGER, FTYPEDESC_INSENDTABLE),
DEFINE_PRED_FIELD(m_flSoonestSecondaryAttack, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
END_PREDICTION_DATA()
#endif

BEGIN_DATADESC( CHL2MPMachineGun )
DEFINE_FIELD( m_flNextSoundTime, FIELD_TIME ),
END_DATADESC()

CHL2MPMachineGun::CHL2MPMachineGun( void )
{
	m_iBurstBullet = 0;
	m_flSoonestSecondaryAttack = 0.0f;
	m_bIsFiringBurst = false;
}

void CHL2MPMachineGun::PrimaryAttack( void )
{
	// Only the player fires this way so we can cast
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if (!pPlayer)
		return;

	CHL2MP_Player *pHL2MPPlayer = ToHL2MPPlayer(pPlayer);
	if (!pHL2MPPlayer)
		return;
	
	// Abort here to handle burst and auto fire modes
	if ( (UsesClipsForAmmo1() && m_iClip1 == 0) || ( !UsesClipsForAmmo1() && !pPlayer->GetAmmoCount(m_iPrimaryAmmoType) ) )
		return;

	pPlayer->DoMuzzleFlash();

	// To make the firing framerate independent, we may have to fire more than one bullet here on low-framerate systems, 
	// especially if the weapon we're firing has a really fast rate of fire.
	int iBulletsToFire = 0;
	float fireRate = GetFireRate();

	while ( m_flNextPrimaryAttack <= gpGlobals->curtime )
	{
		// MUST call sound before removing a round from the clip of a CHLMachineGun
		WeaponSound(SINGLE, m_flNextPrimaryAttack);
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + fireRate;
		iBulletsToFire++;
	}

	// Make sure we don't fire more than the amount in the clip, if this weapon uses clips
	if ( UsesClipsForAmmo1() )
	{
		if ( iBulletsToFire > m_iClip1 )
			iBulletsToFire = m_iClip1;
		m_iClip1 -= iBulletsToFire;
	}

	// Fire the bullets
	FireBulletsInfo_t info;
	info.m_iShots = iBulletsToFire;
	info.m_vecSrc = pHL2MPPlayer->Weapon_ShootPosition( );
	info.m_vecDirShooting = pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	info.m_vecSpread = pHL2MPPlayer->GetAttackSpread( this );
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_iTracerFreq = 2;
	info.m_vecFirstStartPos = pHL2MPPlayer->GetAbsOrigin();
	info.m_flDropOffDist = GetWpnData().m_flDropOffDistance;
	FireBullets( info );

	// Do the viewkick
	pPlayer->ViewPunch(GetViewKickAngle());

	int shootAct = GetPrimaryAttackActivity();
	SendWeaponAnim(shootAct);
	pHL2MPPlayer->DoAnimationEvent(PLAYERANIMEVENT_ATTACK_PRIMARY, shootAct);
}

void CHL2MPMachineGun::SecondaryAttack(void)
{
	if (m_bIsFiringBurst || (m_flSoonestSecondaryAttack > gpGlobals->curtime) || (m_flNextPrimaryAttack > gpGlobals->curtime))
		return;

	m_bIsFiringBurst = true;
	m_iBurstBullet = 0;
}

void CHL2MPMachineGun::FireBullets( const FireBulletsInfo_t &info )
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());
	if (pPlayer)
		pPlayer->FireBullets(info);
}

int CHL2MPMachineGun::WeaponSoundRealtime( WeaponSound_t shoot_type )
{
	int numBullets = 0;

	// ran out of time, clamp to current
	if (m_flNextSoundTime < gpGlobals->curtime)
	{
		m_flNextSoundTime = gpGlobals->curtime;
	}

	// make enough sound events to fill up the next estimated think interval
	float dt = clamp( m_flAnimTime - m_flPrevAnimTime, 0, 0.2 );
	if (m_flNextSoundTime < gpGlobals->curtime + dt)
	{
		WeaponSound( SINGLE_NPC, m_flNextSoundTime );
		m_flNextSoundTime += GetFireRate();
		numBullets++;
	}
	if (m_flNextSoundTime < gpGlobals->curtime + dt)
	{
		WeaponSound( SINGLE_NPC, m_flNextSoundTime );
		m_flNextSoundTime += GetFireRate();
		numBullets++;
	}

	return numBullets;
}

void CHL2MPMachineGun::ItemPostFrame(void)
{
	if (m_bIsFiringBurst)
	{
		BaseClass::DoWeaponFX();
		UpdateAutoFire();
	}
	else
	{
		BaseClass::ItemPostFrame();
	}

	if (m_flNextBashAttack > gpGlobals->curtime)
		return;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());
	if (!pOwner || !AllowBurst())
		return;

	if (pOwner->m_afButtonPressed & IN_ATTACK2)
		SecondaryAttack();

	if (m_bIsFiringBurst)
	{
		if (m_iBurstBullet >= GetMaxBurst())
		{
			m_bIsFiringBurst = false;
			return;
		}

		if (m_flSoonestSecondaryAttack < gpGlobals->curtime)
		{
			m_flSoonestSecondaryAttack = gpGlobals->curtime + GetBurstFireRate();
			m_flNextPrimaryAttack = gpGlobals->curtime;
			m_flNextSecondaryAttack = gpGlobals->curtime;
			PrimaryAttack();
			m_iBurstBullet++;

			if (AutoFiresFullClip())
				m_bFiringWholeClip = true;

#ifdef CLIENT_DLL
			pOwner->SetFiredWeapon(true);
#endif
		}
	}
}

bool CHL2MPMachineGun::Reload(void)
{
	bool fRet;

	CHL2MP_Player *pClient = ToHL2MPPlayer(GetOwner());
	if (pClient)
	{
		int reloadAct = GetReloadActivity();
		fRet = DefaultReload(GetMaxClip1(), GetMaxClip2(), reloadAct);
		if (fRet)
			pClient->DoAnimationEvent(PLAYERANIMEVENT_RELOAD, reloadAct);
	}
	else
		fRet = DefaultReload(GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD);

	if (fRet)
	{
		m_bIsFiringBurst = false;
		m_flSoonestSecondaryAttack = 0.0f;
		WeaponSound(RELOAD);
	}

	return fRet;
}

bool CHL2MPMachineGun::Holster(CBaseCombatWeapon *pSwitchingTo)
{
	m_bIsFiringBurst = false;
	m_flSoonestSecondaryAttack = 0.0f;

	return BaseClass::Holster(pSwitchingTo);
}

void CHL2MPMachineGun::Drop(const Vector &vecVelocity)
{
	m_bIsFiringBurst = false;
	m_flSoonestSecondaryAttack = 0.0f;

	BaseClass::Drop(vecVelocity);
}

bool CHL2MPMachineGun::CanHolster(void)
{
	bool bCanHolster = BaseClass::CanHolster();
	if (m_bIsFiringBurst)
		return false;

	return bCanHolster;
}