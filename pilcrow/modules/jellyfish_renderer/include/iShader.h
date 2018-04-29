#pragma once

#include <string>
#include <utils/include/Resource.hpp>

namespace Jellyfish
{
	class iShader : public Resource {
	public:
		virtual ~iShader() {}

		unsigned Type() const
		{
			return m_ShaderType;
		}

		unsigned ID() const
		{
			return m_ShaderID;
		}
	
	protected:
		virtual bool Load() =0;
		virtual void Unload() =0;
		virtual bool Check() const =0;

	private:
		unsigned m_ShaderID{ 0 };
		unsigned m_ShaderType{ 0 };
	};
}