//#include "pch.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include "CheckerMesh.h"
#include "DirectXHelper.h"

using namespace DirectX;

CheckerMesh::CheckerMesh(void)
{
}


CheckerMesh::~CheckerMesh(void)
{
}

void CheckerMesh::initialize( Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice )
{
	BaseClass::initialize( d3dDevice );

	const float tileSize = 0.5f;
	const int rows = 16;
	const int cols = 128;
	const int vertexCount = rows*cols*2;
	VertexPositionColorTexCoord* pVertices = new VertexPositionColorTexCoord[ vertexCount ];

	float left = -tileSize*0.5f*cols;
	float top = -tileSize*0.5f*rows;
	for( int l=0; l<2; ++l )
	{
		for( int r=0; r<rows; ++r )
		{
			for( int c=0; c<cols; ++c )
			{
				int o = r*cols + c + l*(rows*cols);
				VertexPositionColorTexCoord& v = pVertices[ o ];
				v.pos.x = left+c*tileSize;
				v.pos.y = 0.0f;
				v.pos.z = top+r*tileSize;
				v.color.x = l&1?0.4f:0.7f;
				v.color.y = l&1?0.1f:0.7f;
				v.color.z = l&1?0.1f:0.1f;
				v.texCoord.x = c&1?1.0f:0.0f;
				v.texCoord.y = r&1?1.0f:0.0f;
			}
		}
	}
		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = pVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(VertexPositionColorTexCoord)*vertexCount, D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			d3dDevice->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
				)
			);

	m_indexCount = ( rows-1 )*( cols-1 )*6;
	unsigned short* pIndices= new unsigned short[ m_indexCount ];

	int i = 0;
	for( int r=0; r<rows-1; ++r )
	{
		for( int c=0; c<cols-1; ++c )
		{
			int l = (r+c)&1;
			int layerStart = l*( rows*cols );
			int row0 = layerStart + r*cols;
			int row1 = row0+cols;
			int col00 = row0 + c;
			int col10 = row1 + c;

			pIndices[ i++ ] = col00;
			pIndices[ i++ ] = col00+1;
			pIndices[ i++ ] = col10;

			pIndices[ i++ ] = col10;
			pIndices[ i++ ] = col00+1;
			pIndices[ i++ ] = col10+1;
			/*
			pIndices[ i++ ] = l*( rows*cols )+r*cols+c;
			pIndices[ i++ ] = l*( rows*cols )+r*cols+c+1;
			pIndices[ i++ ] = l*( rows*cols )+r*cols+rows;
			pIndices[ i++ ] = l*( rows*cols )+r*cols+rows;
			pIndices[ i++ ] = l*( rows*cols )+r*cols+c+1;
			pIndices[ i++ ] = l*( rows*cols )+r*cols+rows+c+1;
			*/
		}
	}
	
		D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		indexBufferData.pSysMem = pIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short)*m_indexCount, D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			d3dDevice->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
				)
			);
}
