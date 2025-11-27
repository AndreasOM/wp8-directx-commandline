//#include "pch.h"

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>

#include "CubeRenderer.h"
#include "CubeMesh.h"
#include "CheckerMesh.h"
#include <winuser.h>
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

CubeRenderer::CubeRenderer() :
	m_loadingComplete(false),
	m_indexCount(0)
	, m_fov( 70.0f )
	, m_pMesh( nullptr )
	, m_pCheckerMesh( nullptr )
{
}

void CubeRenderer::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();

	auto loadVSTask = DX::ReadDataAsync("SimpleVertexShader.hlslo");
	auto createVSTask = loadVSTask.then([this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			m_d3dDevice->CreateVertexShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_vertexShader
				)
			);

		const D3D11_INPUT_ELEMENT_DESC vertexDesc[] = 
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_d3dDevice->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				fileData->Data,
				fileData->Length,
				&m_inputLayout
				)
			);
	});

	auto loadPSTask = DX::ReadDataAsync("SimplePixelShader.hlslo");
	auto createPSTask = loadPSTask.then([this](Platform::Array<byte>^ fileData) {
		DX::ThrowIfFailed(
			m_d3dDevice->CreatePixelShader(
				fileData->Data,
				fileData->Length,
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_d3dDevice->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});


	auto createCubeTask = (createPSTask && createVSTask).then([this] () {
		m_pMesh = new CubeMesh();
		m_pMesh->initialize( m_d3dDevice );
	});

	auto createCheckerTask = (createPSTask && createVSTask).then([this] () {
		m_pCheckerMesh = new CheckerMesh();
		m_pCheckerMesh->initialize( m_d3dDevice );
	});


	auto loadTextureTask = (createPSTask && createVSTask).then([this] () {
		D3D11_TEXTURE2D_DESC depthBufferDesc;
			ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = 128;
	depthBufferDesc.Height = 128;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	float pData[ 128*128*4 ];

	const int rw = 8*8;
	const int rh = 8*8;
	float pRandom[ rw*rh ];
	for( int y=0; y<rh; ++y )
	{
		for( int x=0; x<rw; ++x )
		{
			pRandom[ y*rw + x ] = ( float )rand()/RAND_MAX;
		}
	}

	for( int y=0; y<128; ++y )
	{
		for( int x=0; x<128; ++x )
		{
			float rx = ( x*( rw-1 ) )/128.0f;
			float ry = ( y*( rh-1 ) )/128.0f;

			int rix = ( int )rx;
			float rfx = rx-rix;

			int riy = (int)ry;
			float rfy = ry-riy;

			float corners[ 4 ];
			corners[ 0 ] = pRandom[ ( int )riy*rw + rix ];
			corners[ 1 ] = pRandom[ ( int )riy*rw + rix+1 ];
			corners[ 2 ] = pRandom[ ( int )( riy+1 )*rw + rix ];
			corners[ 3 ] = pRandom[ ( int )( riy+1 )*rw + rix+1 ];

//			float rl = pRandom[ ( int )ry*rw + rix ];
//			float rr = pRandom[ ( int )ry*rw + rix+1 ];
//			float r = ( 1-rfx )*rl+( rfx )*rr;

			float upper = ( 1-rfx )*corners[ 0 ]+( rfx )*corners[ 1 ];
			float lower = ( 1-rfx )*corners[ 2 ]+( rfx )*corners[ 3 ];
			float r = ( 1-rfy )*upper+( rfy )*lower;

			float u = ( x-64 )/128.0f;
			float v = ( y-64 )/128.0f;
			int i = 128*y+x;
//			pData[ i*4+0 ] = 1*( sinf( 2*3.14f*u )+sinf( 2*3.14f*v ) );
			float c = ( 0.25f*( u*u+v*v )+0.75f )*( 0.5f*r+0.5f );
			pData[ i*4+0 ] = c;
			pData[ i*4+1 ] = c;
			pData[ i*4+2 ] = c;
//			pData[ i*4+0 ] = 1*( (x+y)%256 )/256.0f;
//			pData[ i*4+1 ] = 1*( (x+y)%256 )/256.0f;
//			pData[ i*4+2 ] = 1*( (x+y)%256 )/256.0f;
			pData[ i*4+3 ] = 1.0;
		}
	}
	/*
	for( int i=0; i<128*128; ++i )
	{
		pData[ i*4+0 ] = ( i%128 )/128.0f;
		pData[ i*4+1 ] = ( i%128 )/128.0f;
		pData[ i*4+2 ] = ( i%128 )/128.0f;
		pData[ i*4+3 ] = 1.0;
	}
	*/
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = pData;
	data.SysMemPitch = 128*4*4;
	data.SysMemSlicePitch = 0;

	ID3D11Texture2D* tex = 0;
	// Create the texture for the depth buffer using the filled out description.
			DX::ThrowIfFailed(
				m_d3dDevice->CreateTexture2D(&depthBufferDesc, &data, &tex)
			);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
memset(&SRVDesc, 0, sizeof(SRVDesc));
SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
SRVDesc.Texture2D.MipLevels = 1;

	DX::ThrowIfFailed(
		m_d3dDevice->CreateShaderResourceView( tex, &SRVDesc, &m_pTextures[0])
	);
//		m_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//		m_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	});


	( createCubeTask && createCheckerTask && loadTextureTask ).then([this] () {
		m_loadingComplete = true;
	});

	// :TODO: m_vertexBuffer
}

void CubeRenderer::updateProjection()
{
	float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	float fovAngleY = m_fov* XM_PI / 180.0f;
	if (aspectRatio < 1.0f)
	{
		fovAngleY /= aspectRatio;
	}

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(
			XMMatrixPerspectiveFovRH(
				fovAngleY,
				aspectRatio,
				0.01f,
				100.0f
				)
			)
		);
}

void CubeRenderer::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();
	updateProjection();
}

void CubeRenderer::Update(float timeTotal, float timeDelta)
{
	(void) timeDelta; // Unused parameter.

	m_fov = 70.0f + sinf( timeTotal*0.5f )*30.0f;
	updateProjection();

	float eyeZ = 2.5f - 1.0f*sinf( timeTotal*0.5f );

	static float eyeY = 0.7f;
	eyeY = 0.7f - 0.7f*sinf( timeTotal*3.0f );
	if( eyeY > 0.5f )
	{
		eyeY = 0.5f;
	}
	eyeY += 1.0f;

	XMVECTOR eye = XMVectorSet(0.0f, eyeY, eyeZ, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, eyeY-0.8f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

	m_rotX = timeTotal * XM_PIDIV4;
	m_rotX = sinf( m_rotX*1.5f )*2.5f;

	m_rotZ += fabsf( 0.001f*m_rotX );


}

void CubeRenderer::Render()
{
#ifdef COMMAND_LINE_BUILD
	const float clearColor[] = { 0.298f, 0.098f, 0.039f, 1.000f };
#else
	const float clearColor[] = { 0.098f, 0.098f, 0.439f, 1.000f };
#endif
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		clearColor
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);

	// Only draw the cube once it is loaded (loading is asynchronous).
	if (!m_loadingComplete)
	{
		return;
	}

	m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	XMStoreFloat4x4(
		&m_constantBufferData.model,
		XMMatrixTranspose(
			XMMatrixMultiply(
				XMMatrixMultiply(
					XMMatrixRotationY(m_rotX),
					XMMatrixRotationZ(m_rotZ )
				),
				XMMatrixTranslation( 0.0f, 0.0f, 0.0f )
			)
		)
	);
	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);


	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	m_d3dContext->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	m_d3dContext->PSSetShaderResources( 0, 1, m_pTextures );
	//deviceContext->PSSetSamplers(0, 1, &m_sampleState);


	m_pMesh->render( m_d3dContext );


	float x = sinf( m_rotX )*3.0f;
	float y = -1.0f;
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixMultiply( XMMatrixRotationY(0.0f*m_rotX), XMMatrixTranslation( x, y, 0.0f ) ) ) );
	m_d3dContext->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);


	m_d3dContext->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);
	m_pCheckerMesh->render( m_d3dContext );
}
