﻿#pragma once
#include "DamageCalc.h"
class DamageCalc_MaxHpPercent :
    public DamageCalc
{
public:
    DamageCalc_MaxHpPercent();
    ~DamageCalc_MaxHpPercent();

    virtual void Calc(float* _damage, UnitStat* _myStat, UnitStat* _targetStat);

    static DamageCalc_MaxHpPercent* CreateCalc(float _value, bool _trueDamage = false);

};
