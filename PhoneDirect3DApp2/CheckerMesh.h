#pragma once
#include "mesh.h"
class CheckerMesh : public Mesh
{
	typedef Mesh BaseClass;
public:
	CheckerMesh(void);
	~CheckerMesh(void);

	virtual void	initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice );
};

