#pragma once

#include "iMesh.h"

namespace Jellyfish
{
	class GLMesh : public iMesh
	{
	public:
		GLMesh()
		{}
		~GLMesh()
		{}

		void Draw() const;

	private:
	};
}