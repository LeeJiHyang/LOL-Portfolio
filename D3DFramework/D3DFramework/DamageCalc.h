﻿#pragma once
#include "Stat.h"

enum class CalcKind
{
	BASIC, MAXHP_PER, CURRENTHP_PER, LOSTHP_PER, END
};

namespace Engine
{
	class DamageCalc
	{
	protected:
		DamageCalc();

	public:
		~DamageCalc();
		virtual void Calc(float* _damage, Stat* _myStat, Stat* _targetStat) = 0;

	protected:
		float Calc_Defense(float _damage, Stat* _myStat, Stat* _targetStat);
	protected:
		float value = 0.f;
		bool trueDamage = false;
	};
}

