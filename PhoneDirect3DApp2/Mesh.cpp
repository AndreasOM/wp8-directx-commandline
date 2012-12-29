//#include "pch.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>

#include "DirectXHelper.h"


#include "Mesh.h"

using namespace DirectX;


Mesh::Mesh(void)
	: m_indexCount( 0 )
{
}


Mesh::~Mesh(void)
{
}

void Mesh::initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice )
{
}

void Mesh::render( Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext )
{
	UINT stride = sizeof(VertexPositionColorTexCoord);
	UINT offset = 0;
	d3dContext->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	d3dContext->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT,
		0
		);

	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dContext->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}
