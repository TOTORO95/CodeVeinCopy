#include "stdafx.h"
#include "SwordTrail.h"

#include "Texture.h"
#include "TrailBuffer.h"
#include "Shader.h"

#include "Export_Function.h"

CSwordTrail::CSwordTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSwordTrail::~CSwordTrail()
{
}

HRESULT CSwordTrail::Ready_GameObject(const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	FAILED_CHECK_RETURN(Add_Component(wContainerIdx, wstrTextureTag), E_FAIL);

	return S_OK;
}

_int CSwordTrail::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	if (iExit & 0x80000000)
		return -1;

	if (m_bOn)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_DISTORTION, this);

	return 0;
}

_int CSwordTrail::LateUpdate_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CSwordTrail::Render_GameObject()
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	FAILED_CHECK_VOIDRETURN(SetUp_ConstantTable(pEffect));

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer(&m_TrailList);

	pEffect->EndPass();

	pEffect->End();

	::Safe_Release(pEffect);



	//m_pBufferCom->Render_Buffer();

}

void CSwordTrail::Add_Vertex(const _vec3 * pTopPos, const _vec3 * pBottomPos)
{
	m_bOn = true;

	cout << "X=" << pTopPos->x << "Y=" << pTopPos->y << "Z=" << pTopPos->z << endl;


	m_TrailList.push_back(make_pair(*pTopPos, *pBottomPos));

	if (m_TrailList.size() > 15)
		m_TrailList.pop_front();

	//m_pBufferCom->Add_Vertex_CatmullRom(&m_TrailList);
}

void CSwordTrail::Clear_Vertex()
{
	m_bOn = false;

	m_TrailList.clear();

	m_pBufferCom->Clear_Vertex();
}

HRESULT CSwordTrail::Add_Component(const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(wContainerIdx, wstrTextureTag.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pBufferCom = dynamic_cast<Engine::CTrailBuffer*>(Engine::Clone(RESOURCE_STAGE, L"Buffer_Trail"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	pComponent->AddRef();

	return S_OK;
}

HRESULT CSwordTrail::SetUp_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix			matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	return S_OK;
}

CSwordTrail * CSwordTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const wstring& wstrTextureTag)
{
	CSwordTrail* pInst = new CSwordTrail(pGraphicDev);

	if (FAILED(pInst->Ready_GameObject(wContainerIdx, wstrTextureTag)))
		::Safe_Release(pInst);

	return pInst;
}

void CSwordTrail::Free()
{
	CGameObject::Free();
}