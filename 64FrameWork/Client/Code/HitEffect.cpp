#include "stdafx.h"
#include "HitEffect.h"
#include "Export_Function.h"



CHitEffect::CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec2 vScale,_vec3 vPos,_float fMaxScale,_float fSpeed, _uint uiRenderOpt)
	: CGameEffect(pGraphicDev,wstrTexName,vScale,vPos,false), m_fMaxScale(fMaxScale),m_fSCTimeSpeed(fSpeed),m_uiRenderOpt(uiRenderOpt)
{
	m_vPos = vPos;

}

CHitEffect::~CHitEffect(void)
{

}

HRESULT CHitEffect::Ready_GameObject(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_vScale.x = m_pTextureCom->Get_ImageInfo().Width*0.01f*m_vMultiScale.x;
	m_vScale.y = m_pTextureCom->Get_ImageInfo().Height*0.01f*m_vMultiScale.y;
	m_vScale.z = m_pTextureCom->Get_ImageInfo().Width*0.01f*m_vMultiScale.x;
	m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	m_fFrameMax=m_pTextureCom->Get_ImageCnt();


	return S_OK;
}

HRESULT CHitEffect::LateReady_GameObject(void)
{
	//m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"RussianHat_0", L"Com_Transform", Engine::ID_DYNAMIC));
	//_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);

	//m_pTransformCom->Get_WorldMatrix(&mat);




	return S_OK;
}

_int CHitEffect::Update_GameObject(const _float& fTimeDelta)
{


	Splash_Scale(fTimeDelta, m_fMaxScale);
	
	m_vOldScale = { m_vScale.x * m_vMultiScale.x,
					m_vScale.y*m_vMultiScale.y,
					m_vScale.z*m_vMultiScale.x };

	m_pTransformCom->Set_Scale(m_vOldScale.x, m_vOldScale.y, m_vOldScale.z);


	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y , m_vPos.z );




	m_fFrameCnt += m_fFrameMax * fTimeDelta;

	if (m_fFrameCnt >= m_fFrameMax)
	{
			m_fFrameCnt = m_fFrameMax -1;
			//m_fFrameCnt = 0;

	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	Engine::CGameObject::Compute_ViewZ(&m_pTransformCom->m_vInfo[Engine::INFO_POS]);
	

	switch (m_uiRenderOpt)
	{
	case 0:
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
		break;
	case 1:
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_DISTORTION, this);
		break;
	case 2:
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_DISTORTION, this);
		break;

	default:
		break;
	}
 	return 0;
}

void CHitEffect::Render_GameObject(void)
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

HRESULT CHitEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
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
	
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}



HRESULT CHitEffect::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//pEffect->SetFloat("g_fAlphaRatio", 1.f - (m_fSplashScale / m_fMaxScale)*0.5f);
	pEffect->SetFloat("g_fAlphaRatio", 1.f );

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(m_fFrameCnt));
	
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");



	return S_OK;
}

void CHitEffect::Splash_Scale(_float fTimeDelta, _float fMaxScale)
{
	//if (fMaxScale >= m_fSplashScale)
	//{
	//	m_fSplashScale += fTimeDelta*fMaxScale;
	//	m_vMultiScale.x = m_fSplashScale;
	//	m_vMultiScale.y = m_fSplashScale;
	//} 
	//if(fMaxScale < m_fSplashScale)
	//{
	//	m_vMultiScale.x = fMaxScale;
	//	m_vMultiScale.y = fMaxScale;
	//	//Set_Enable(false);
	//}
	_float fMaxDgree=0.f;
	if (m_uiRenderOpt == 2)
	{
		fMaxDgree = 180.f;
		if (fMaxDgree > m_fSinTime)
		{
			m_fSinTime += fTimeDelta*fMaxDgree*m_fSCTimeSpeed;
			m_fSplashScale = sinf(D3DXToRadian(m_fSinTime))*m_fMaxScale;
			m_vMultiScale.x = m_fSplashScale;
			m_vMultiScale.y = m_fSplashScale;
		}

	}
	if (m_uiRenderOpt == 1)
	{
		fMaxDgree = 90.f;
		if (fMaxDgree > m_fSinTime)
		{
			m_fSinTime += fTimeDelta*fMaxDgree*(m_fSCTimeSpeed*0.5f);
			m_fSplashScale = sinf(D3DXToRadian(m_fSinTime))*m_fMaxScale;
			m_vMultiScale.x = m_fSplashScale;
			m_vMultiScale.y = m_fSplashScale;
		}

	}

	if(fMaxDgree < m_fSinTime)
	{
		m_fSinTime = 0.f;
		m_bEnable = false;
	}

}

void CHitEffect::Set_Distortion()
{
	m_bIsDistortion = true;
}

void CHitEffect::VerticalMove(_float fTimeDelta)
{


	if (180.f >= m_fVerticalTime)
	{
		m_fVerticalTime += fTimeDelta*180.f;

		m_fVerticalPos = sinf(D3DXToRadian(m_fVerticalTime));
	}
	else
	{
		m_fVerticalTime = 0.f;

	}
}

void CHitEffect::Set_Enable(bool bEnable)
{
	m_bEnable = bEnable;
	if (bEnable)
	{
		
		if (m_bIsParent)
		{
			m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
			m_pTransformCom->Get_WorldMatrix(&m_OldMatrix);
			memcpy(m_vPos, &m_OldMatrix._41, sizeof(_vec3));
		}

	}
	else
	{
	
	}

}

void CHitEffect::Set_Enable(bool bEnable, _vec3 vAddPos)
{
	m_bEnable = bEnable;
	if (bEnable)
	{
		m_fVerticalTime = 0.f;
		m_fSplashScale = 0.f;
		if (m_bIsParent)
		{
	
			
			m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
			m_pTransformCom->Get_WorldMatrix(&m_OldMatrix);
			memcpy(m_vPos, &m_OldMatrix._41, sizeof(_vec3));
			m_vAddPos = vAddPos;
			m_vAddPos*=50.f;
			m_vAddPos.y = 0.f;
		}

	}
	else
	{

	}

}

CHitEffect * CHitEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec2 vScale, _vec3 vPos,  _float fMaxScale, _float fSpeed, _uint uiRenderOpt)
{
	CHitEffect*	pInstance = new CHitEffect(pGraphicDev, wstrTexName,  vScale, vPos,fMaxScale, fSpeed, uiRenderOpt);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}



void CHitEffect::Free(void)
{


	Engine::CGameObject::Free();
}

