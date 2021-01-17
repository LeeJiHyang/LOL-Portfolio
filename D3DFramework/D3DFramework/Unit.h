#pragma once
#include "GameObject.h"
#include "Animation.h"

enum class Team
{
	NEUTRAL, BLUE, RED, END
};

enum class UnitState
{
	IDLE1,
	IDLE2,
	IDLE3,
	DEATH,
	RECALL,
	RECALL2,
	RECALL3,
	RUN,
	RUN2,
	RUN_HASTE,
	ATTACK1,
	ATTACK2,
	ATTACK3,
	ATTACK4,
	CRITICAL,
	Q,
	W,
	E,
	R,
	Q2,
	W2,
	E2,
	R2,
	SPAWN,
	END
};

class EnemyUnit : public GameObject
{
public:
    EnemyUnit();
    virtual ~EnemyUnit();

    virtual void Initialize() override;
    virtual void Release() override;
    virtual void Update() override;

	void UpdateAttack();

	void LookRotation(Vector3 _direction);
	void SetDestination(Vector3 _target);
	virtual void Attack(EnemyUnit* _target);
	virtual void Spell1();
	virtual void Spell2();
	virtual void Spell3();
	virtual void Spell4();



public:
	UnitState state = UnitState::IDLE1;
	Team team = Team::NEUTRAL;
	Animation* anim = nullptr;
	NavMeshAgent* agent = nullptr;

	// �⺻���� ����
	bool attackFlag = false;
	EnemyUnit* attackTarget = nullptr;
	float attackRange = 1.25f;
	float attackTick = 0.f;
	float attackPerSec = 0.625f;
	UnitState attackState = UnitState::ATTACK1;
	bool canAttack = false;
};

