//============== Copyright © 2011 IV:MP Team. All rights reserved. ==============
// File: CVehicleManagerInterface.h
//===============================================================================

#pragma once

#include "InterfaceCommon.h"
#include "CNetworkVehicleInterface.h"

class CVehicleManagerInterface
{
public:
	virtual EntityId Add(int iModelId, Vector3 vecSpawnPosition, Vector3 vecSpawnRotation, VehicleColors colors) = 0;
	virtual void Remove(EntityId vehicleId) = 0;
	virtual bool DoesExist(EntityId vehicleId) = 0;
	virtual int GetVehicleCount() = 0;

	virtual CNetworkVehicleInterface * GetAt(EntityId vehicleId) = 0;
};