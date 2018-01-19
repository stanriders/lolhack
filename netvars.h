#pragma once

#include "utils.h"

class NetVars
{
public:
	void OnSetup();

	DWORD_PTR
		m_lifeState,
		m_iHealth,
		m_iTeamNum,
		m_vecOrigin,
		m_vecViewOffset,
		m_vecVelocity,
		m_fFlags,
		m_Local,
		m_aimPunchAngle,
		m_iCrossHairID,
		m_hActiveWeapon,
		m_angEyeAngles,
		m_ArmorValue,
		m_iShotsFired,
		m_nForceBone,
		m_BoneMatrix,
		m_pModel,
		m_flFlashDuration,
		m_nTickBase,
		m_iCompetitiveRanking,
		m_iCompetitiveWins,
		m_szArmsModel;

	DWORD_PTR
		m_iClip1,
		m_fAccuracyPenalty,
		m_iWeaponId,
		m_flNextPrimaryAttack;
};