#pragma once

//std
#include <string>

//ours
#include <utils/include/Resource.hpp>


namespace Jellyfish
{
	class iShader : virtual public Resource
	{
	public:
		//Not using this yet
		//// Add others as needed
		//enum class ShaderType
		//{
		//	vertex,
		//	fragment,
		//	tesselation,
		//	geometry,
		//	compute
		//}; //end enum ShaderType

		iShader()
		{
			//TODO: investigate need for this -- fix
			//this should never get called directly
		}

		virtual ~iShader() {}

		virtual unsigned Type() const = 0;
		virtual void Use() const = 0;

		//planned functionality
		//AddToEffect() const = 0;

		const unsigned int GetID() const
		{
			return m_ID;
		}
		const std::string GetName() const
		{
			return m_name;
		}

	protected:
		virtual bool Check() const =0;
		
		//Members
		unsigned int m_ID;
		std::string m_name;

	private:
		
	};
}