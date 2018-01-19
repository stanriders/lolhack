
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
	g_traces->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

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

	for (auto pList = g_client->GetAllClasses(); pList; pList = pList->m_pNext)
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
	return g_entityList->GetClientEntity(g_engineClient->GetLocalPlayer());
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

//--------------------------------------------------------------------------------
static bool screen_transform(const Vector& in, Vector& out)
{
	static auto& w2sMatrix = g_engineClient->WorldToScreenMatrix();

	out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
	out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
	out.z = 0.0f;

	float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

	if (w < 0.001f) {
		out.x *= 100000;
		out.y *= 100000;
		return false;
	}

	out.x /= w;
	out.y /= w;

	return true;
}
//--------------------------------------------------------------------------------
bool WorldToScreen(const Vector& in, Vector& out)
{
	if (screen_transform(in, out)) {
		int w, h;
		g_engineClient->GetScreenSize(w, h);

		out.x = (w / 2.0f) + (out.x * w) / 2.0f;
		out.y = (h / 2.0f) - (out.y * h) / 2.0f;

		return true;
	}
	return false;
}