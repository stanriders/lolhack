#pragma once

#include "interfaces.h"
#include <eiface.h>
#include <string>

bool CanSeePlayer(IClientEntity* player);

RecvProp *GetRecvProp(const std::string& Name, RecvTable* Table);

DWORD_PTR GetEntry(const std::string& Data, const std::string& Name, RecvTable* Table);

DWORD_PTR GetNetVar(const std::string& Data, const std::string& Name);

IClientEntity *GetLocalPlayer();

void NormalizeAngles(QAngle& angle);

float GetFov(const QAngle& viewAngle, const QAngle& aimAngle);

void VectorAngles(const Vector& forward, QAngle &angles);

QAngle CalcAngle(Vector src, Vector dst);

static bool screen_transform(const Vector& in, Vector& out);
bool WorldToScreen(const Vector& in, Vector& out);