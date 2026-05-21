#pragma once
#include "ScriptBase.h"

namespace Nova
{
	class TestScript : public ScriptBase
	{
	public:
		SCRIPT_NAME(TestScript)

		void Awake() override;
		void Start() override;
		void Update() override;
	};
}
