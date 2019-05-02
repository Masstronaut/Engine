#pragma once

namespace Jellyfish
{
	// Describes common device object attributes
	struct DeviceObjectAttribs
	{
		// Object name
		const char* Name;

		// Base constructor, to init will null name 
		DeviceObjectAttribs() :
			Name(nullptr)
		{}
	};

} //end namespace Jellyfish