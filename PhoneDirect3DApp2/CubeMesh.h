#pragma once
#include "mesh.h"
class CubeMesh : public Mesh
{
	typedef Mesh BaseClass;
public:
	CubeMesh(void);
	~CubeMesh(void);

	virtual void	initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice );
};

