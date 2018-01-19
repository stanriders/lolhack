
#include "utils.h"

class CTraceFilterPlayersOnly : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}
	bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask)
	{
		return true;
	}
};

bool CanSeePlayer(IClientEntity* player)
{
	CGameTrace tr;
	Ray_t ray;
	CTraceFilterPlayersOnly filter;

	Vector start = GetLocalPlayer()->GetAbsOrigin();
	Vector end = player->GetAbsOrigin();

	ray.Init(start, end);
	traces->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.DidHit();
}

RecvProp *GetRecvProp( const std::string& Name, RecvTable* Table)
{
	RecvProp* dwRet = NULL;
	RecvProp* Prop = NULL;
	RecvTable* Child = NULL;

	for (int i = 0; i < Table->GetNumProps(); i++)
	{
		Prop = Table->GetPropA(i);

		if (!Prop)
			continue;

		Child = Prop->GetDataTable();

		if (Child && Child->GetNumProps())
			dwRet = GetRecvProp( Name, Child);

		if ( !Name.compare(Prop->GetName()))
		{
			dwRet = Prop;
			break;
		}
	}
	return dwRet;
}

DWORD_PTR GetEntry(const std::string& Data, const std::string& Name, RecvTable* Table)
{
	DWORD_PTR dwRet = NULL;
	RecvProp* Prop = NULL;
	RecvTable* Child = NULL;

	for (int i = 0; i < Table->GetNumProps(); i++)
	{
		Prop = Table->GetPropA(i);

		if (!Prop)
			continue;

		Child = Prop->GetDataTable();

		if (Child && Child->GetNumProps())
			dwRet += GetEntry(Data, Name, Child);

		if (!Data.compare(Table->GetName()) && !Name.compare(Prop->GetName()))
		{
			dwRet += Prop->GetOffset();
			break;
		}
	}
	return dwRet;
}

DWORD_PTR GetNetVar(const std::string& Data, const std::string& Name)
{
	DWORD_PTR dwRet = NULL;

	for (auto pList = client->GetAllClasses(); pList; pList = pList->m_pNext)
	{
		RecvTable* Table = pList->m_pRecvTable;

		if (!Table)
			continue;

		if (!Table->GetNumProps())
			continue;

		dwRet = GetEntry(Data, Name, Table);

		if (dwRet)
			break;
	}
	return dwRet;
}

IClientEntity *GetLocalPlayer()
{
	return entityList->GetClientEntity(engineClient->GetLocalPlayer());
}

void NormalizeAngles(QAngle& angle)
{
	while (angle.x > 89.0f)
		angle.x -= 180.f;

	while (angle.x < -89.0f)
		angle.x += 180.f;

	while (angle.y > 180.f)
		angle.y -= 360.f;

	while (angle.y < -180.f)
		angle.y += 360.f;
}

float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	NormalizeAngles(delta);

	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}

void VectorAngles(const Vector& forward, QAngle &angles)
{
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles[1] = 0.0f;  //yaw left/right
	}
	else
	{
		angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
		angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

		if (angles[1] > 90)
			angles[1] -= 180;
		else if (angles[1] < 90)
			angles[1] += 180;
		else if (angles[1] == 90)
			angles[1] = 0;
	}

	angles[2] = 0.0f;
}
QAngle CalcAngle(Vector src, Vector dst)
{
	QAngle angles;
	Vector delta = src - dst;

	VectorAngles(delta, angles);

	return angles;
}