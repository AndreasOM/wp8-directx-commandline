#pragma once

struct VertexPositionColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

struct VertexPositionColorTexCoord
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 texCoord;
};


class Mesh
{
public:
	Mesh(void);
	virtual ~Mesh(void);

	virtual void	initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice );
	void	render( Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext );
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;
	uint32									m_indexCount;

};

