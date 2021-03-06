//#include "pch.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include "CubeMesh.h"
#include "DirectXHelper.h"

using namespace DirectX;

CubeMesh::CubeMesh(void)
{
}


CubeMesh::~CubeMesh(void)
{
}

void CubeMesh::initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice )
{
	BaseClass::initialize( d3dDevice );

		VertexPositionColorTexCoord cubeVertices[] = 
		{
#ifdef COMMAND_LINE_BUILD
			{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f )},
#else
			{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 0.0f, 1.0f )},
			{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2( 1.0f, 0.0f )},
			{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 1.0f, 1.0f )},
			{XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2( 0.0f, 0.0f )},
			{XMFLOAT3( 0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 0.0f, 1.0f )},
			{XMFLOAT3( 0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2( 1.0f, 0.0f )},
			{XMFLOAT3( 0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 1.0f, 1.0f )},
#endif
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = cubeVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			d3dDevice->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
				)
			);

		unsigned short cubeIndices[] = 
		{
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
		};

		m_indexCount = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		indexBufferData.pSysMem = cubeIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			d3dDevice->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
				)
			);

}
