#pragma once


typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	Matrix			CombinedTransformationMatrix;

}D3DXFRAME_DERIVED;

typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTexture;

	LPD3DXMESH				pOriMesh;	// x������ �ε����� �� ������ ���¸� ������ �ִ� �� ��ü(�Һ�)

	ULONG					dwNumBones;	// ���� ������ �����ϴ� ����


	Matrix* pFrameOffsetMatrix;	// �ִϸ��̼��� ������ ������ �� ����
	Matrix** ppFrameCombinedMatrix;	// frame ���� ������ �ִ� CombinedTransformationMatrix�� �ּҰ����� �����ϱ� ���� ������
	Matrix* pRenderingMatrix;	//pFrameOffsetMatrix * (*ppFrameCombinedMatrix) ��������� ���������� �������� �ϱ� ���� ���


}D3DXMESHCONTAINER_DERIVED;

enum class ChampionType : char
{
	Garen,Darius,Diana,Leona,Leesin,Missfortune,Blitzcrank,Ahri,Amumu,Jinx
};

enum class SpellType : char
{
	Barrier,Boost,Exhaust,Flash,Haste,Heal,Ignite,Mana,Smite,Teleport
};