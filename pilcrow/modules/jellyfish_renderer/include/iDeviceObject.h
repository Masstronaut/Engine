#pragma once

#include "iObject.h"
#include "ObjectAttribs.h"

namespace Jellyfish
{
	//GUID for a Device Object : {5B4CCA0B-5075-4230-9759-F48769EE5502}
	static constexpr Jellyfish::GUID IID_DeviceObject =
	{ 0x5b4cca0b, 0x5075, 0x4230,{ 0x97, 0x59, 0xf4, 0x87, 0x69, 0xee, 0x55, 0x2 } };

	// Base interface for all objects created by the render device is Jellyfish::iRenderDevice
	class IDeviceObject : public iObject
	{
	public:
		// Queries the specific interface, see iObject::QueryInterface() for details
		virtual void QueryInterface(const Jellyfish::GUID& guid, iObject **ppInterface) = 0;

		// Returns the buffer object description, (just a name rn)
		virtual const DeviceObjectAttribs& GetDesc() const = 0;
	};

} //end namespace Jellyfish