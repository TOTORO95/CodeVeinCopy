#include "stdafx.h"
#include "BossUI.h"
#include "Export_Function.h"

CBossUI::CBossUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos,_float fAlphaSpeed, _bool bIsFadeOut)
	:CUI(pGraphicDev,wstrTexName,vPos,PIVOT::PIVOT_LT, _vec3(0.f,0.f,0.f))
{
	m_fAlphaSpd = fAlphaSpeed;
}

CBossUI::~CBossUI(void)
{

}

HRESULT CBossUI::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fConvertSizeX = m_fSizeX = m_pTextureCom->Get_ImageInfo().Width;
	m_fConvertSizeY = m_fSizeY = m_pTextureCom->Get_ImageInfo().Height;

	m_pTransformCom->Set_Scale((WINCX / m_fConvertSizeX), (WINCY / m_fConvertSizeY), 1.0f);
	ConvertPivotPos();
	m_vUBA.z = 0.f;
	return S_OK;
}

HRESULT CBossUI::LateReady_GameObject(void)
{

	return S_OK;
}

_int CBossUI::Update_GameObject(const _float& fTimeDelta)
{
	m_vUBA = { 0.f,0.f,1.f };
	m_fTime += fTimeDelta;
	LerpGague(fTimeDelta);
	m_fConvertSizeX;
	if (m_fTime >= 4.f)
	{
		if (m_wstrTexName.find(L"TitleWallpaper_UI") != wstring::npos)
			m_vUBA.z = 1.f;
	}


	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CBossUI::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);
	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

}

HRESULT CBossUI::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_InRcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrTexName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Title"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CBossUI::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_fConvertSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;


	matWorld._41 = m_vConvertPos.x - WINCX * 0.5f;
	matWorld._42 = -m_vConvertPos.y + WINCY * 0.5f;
	matWorld._43 = m_vConvertPos.z;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(m_fFrameCnt));
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	pEffect->SetFloat("g_TextureU", m_vUBA.x);
	pEffect->SetFloat("g_TextureV", m_vUBA.y);
	pEffect->SetFloat("g_TextureA", m_vUBA.z);
	pEffect->SetFloat("g_TextureUScale", m_fUScale);
	pEffect->SetFloat("g_TextureFilterVal", m_fFilter);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive	= D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Power		= 100.f;

	return S_OK;
}

void CBossUI::Set_MaxAlpha(_float fMaxAlpha)
{
	m_fAlphaMax = fMaxAlpha;
}

void CBossUI::Set_GaugeRatio(_float fRatio)
{
	m_fOldUScale = fRatio;
}

void CBossUI::Blink_Image(_float fTimeDelta,_float fSpeed, _float fStartTime)
{
	if (m_fAlphaSpd>0.f)
	{
		m_fSin += fTimeDelta*m_fAlphaSpd;
		if (m_fSin > 180.f)
			m_fSin = 0.f;
		m_vUBA.z = m_fAlphaMax - (sinf(D3DXToRadian(m_fSin))*0.5f)*m_fAlphaMax;
	}
	else
	{
		m_fSin = 0.f;
		m_vUBA.z = 1.0f;
	}

}

void CBossUI::LerpGague(_float fTimeDelata)
{
	if (m_fLerpTime < 1.0f)
	{
		m_fLerpTime += fTimeDelata*0.1f;
		_float fLerp = (1 - m_fLerpTime)*m_fUScale+ m_fLerpTime*m_fOldUScale;
		m_fUScale = fLerp;
		m_fConvertSizeX = m_fSizeX *m_fUScale;
		ConvertPivotPos();

	}
	if (m_fLerpTime > 1.0f)
		m_fLerpTime = 0.f;

}



CBossUI* CBossUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos,_float fAlphaSpeed, _bool bIsFadeOut)
{
	CBossUI*	pInstance = new CBossUI(pGraphicDev, wstrTexName, vPos, fAlphaSpeed, bIsFadeOut);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBossUI::Free(void)
{
	CUI::Free();
}

