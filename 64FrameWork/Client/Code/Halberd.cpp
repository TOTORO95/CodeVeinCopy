#include "stdafx.h"
#include "Halberd.h"

#include "Export_Function.h"

CHalberd::CHalberd(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiInstIdx)
	: Engine::CGameObject(pGraphicDev)
{

	m_uiIdx = uiInstIdx;
	m_wstrInstName = L"Halberd_"+to_wstring(m_uiIdx);
	m_wstrEquipName = L"RedDevil_"+to_wstring(m_uiIdx);

}


CHalberd::~CHalberd(void)
{

}

HRESULT CHalberd::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Set_Scale(1.75f, 1.75f, 1.75f);


	return S_OK;
}

_int CHalberd::Update_GameObject(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		_uint uiInstIdx = 0;
		Engine::CDynamicMesh*	pMonsterMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", m_wstrEquipName.c_str(), L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pMonsterMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pBone = pMonsterMeshCom->Get_FrameByName("RightHandAttach");
		NULL_CHECK_RETURN(pBone, 0);

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

		Engine::CTransform*	pObjectTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", m_wstrEquipName.c_str(), L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pObjectTransform, 0);

		m_pParentWorldMatrix = pObjectTransform->Get_WorldMatrixPointer();
		m_bIsEquip = true;
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	if (m_bIsEquip)
	{	
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
		m_pTransformCom->Get_WorldMatrix(&m_MatOldWorld);
		memcpy(m_vOldPos, &m_MatOldWorld._41, sizeof(_vec3));
		
	}
	else
	{
		D3DXVec3TransformNormal(&m_vThrow, &_vec3(0.f, -0.25f, 1.f), &m_MatOldWorld);
		m_fAccThrow += fTimeDelta;
		m_vThrow *= m_fAccThrow;
		m_vOldPos+= m_vThrow;
		m_pTransformCom->Set_Pos(&m_vOldPos);

		_matrix matRotY, matTemp;
		D3DXMatrixRotationY(&matRotY,D3DXToRadian(60.f));
		matTemp= matRotY*m_MatOldWorld ;
				
		m_pTransformCom->Set_ParentMatrix(&matTemp);
	}

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CHalberd::Render_GameObject(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	//
	//m_pMeshCom->Render_Meshes();

	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(false), &matWorld);


	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();
	_uint	iPassMax = 0;
	SetUp_ConstantTable(pEffect);
	pEffect->Begin(&iPassMax, 0);
	pEffect->BeginPass(0);
	m_pMeshCom->Render_Meshes(pEffect);
	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);


}

HRESULT CHalberd::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);

	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	return E_NOTIMPL;
}

HRESULT CHalberd::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"SK_NormalHalberdB"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}


void CHalberd::Set_EquipObject(wstring wstrObjName)
{
	m_wstrEquipName = wstrObjName;
}

void CHalberd::Set_Equip(_bool bIsEquip)
{
	  m_bIsEquip = bIsEquip; 
	  
}

void CHalberd::Set_Throw()
{

}



CHalberd* CHalberd::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiInstIdx)
{
	CHalberd*	pInstance = new CHalberd(pGraphicDev, uiInstIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CHalberd::Free(void)
{
	Engine::CGameObject::Free();
}


