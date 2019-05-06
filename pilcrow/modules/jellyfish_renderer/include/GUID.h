#pragma once

#include <cstring>  //std>>memcp
#include <cstdint>  //uint<size>_t


//!!! THIS ISN"T BEING USED YET BUT IT SHOULD BE! ///



//Global Unique Identifier, each one is randomly generated, guaranteed unique, 
//and identified in a comment where the GUID is assigned to the object
namespace Jellyfish
{
	// Describes unique identifier
	struct GUID
	{
		uint32_t Data1;
		uint16_t Data2;
		uint16_t Data3;
		uint8_t  Data4[8];

		bool operator == (const GUID& rhs)const
		{
			return Data1 == rhs.Data1 &&
				Data2 == rhs.Data2 &&
				Data3 == rhs.Data3 &&
				std::memcmp(Data4, rhs.Data4, sizeof(Data4)) == 0;
		}
	};

	// Unknown interface, i.e. not found
	static constexpr GUID GUID_Unknown = { 0, 0, 0,{ 0, 0, 0, 0, 0, 0, 0, 0 } };
}