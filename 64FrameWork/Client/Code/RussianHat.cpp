#include "stdafx.h"
#include "RussianHat.h"
#include "Export_Function.h"
#include "ColliderManager.h"
#include "Shield.h"
#include "Gauge.h"
#include "RussianHatHont.h"
#include "IceSword.h"
#include "BoostEffect.h"
#include "SplashEffect.h"
#include "SnowSpalshEffect.h"
#include "ChargeEffect.h"
#include "ThirdPersonCamera.h"
#include "ExplosionEffect.h"
CRussianHat::CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx, _uint uiStageIdx )
	: CDynamicObject(pGraphicDev,wstrName,uiIdx, uiStageIdx)
{
	m_bIsMonster = true;

}

CRussianHat::CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx)
	:CDynamicObject(pGraphicDev,wstrName,uiIdx,tInfo, uiStageIdx)

{
	m_bIsMonster = true;
}

CRussianHat::~CRussianHat()
{
}

HRESULT CRussianHat::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fCurHp = m_fMaxHp = 150.f;
	m_fAttackRange = 4.f;
	Set_TransformData();

	m_eCurState = RUSSIAN_START_IDLE;
	m_ePreState = RUSSIAN_APPERANCE_N;

	if (LOAD_MODE != 0)
		m_uiStageIdx = LOAD_MODE;
	switch ((LOADMODE)m_uiStageIdx)
	{
	case LOAD_NOMAL:
		m_pNaviCom->Set_Index(38);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_NOMAL2:
		m_pTransformCom->Set_Pos(13.6f, 6.4578f, -62.274f);
		m_pNaviCom->Set_Index(148);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/AttachMapNav.txt");


		break;
	case LOAD_NOMAL3:
		break;

	case LOAD_PLAYER:
		break;
	case LOAD_MONSTER:
		break;
	case LOAD_BATTLE:

		m_pTransformCom->Set_Pos(13.6f, 6.4578f, -62.274f);
		m_pNaviCom->Set_Index(148);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/AttachMapNav.txt");
		
		////눈맵 플레이어바로앞
		//m_pTransformCom->Set_Pos(-12.32f, 2.73f, -25.3241f);
		//m_pNaviCom->Set_Index(76);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");
		break;

		//보스 정위치
		//m_pTransformCom->Set_Pos(-20.2f, 1.9f, -49.f);
		//m_pNaviCom->Set_Index(111);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");

		//m_pTransformCom->Set_Pos(18.36f, 0.47f, -0.674f);
		//m_pNaviCom->Set_Index(58);// Base Init Idx 38  //43 Idle

		//m_pTransformCom->Set_Pos(5.3f, 0.19f, -0.95f);
		//m_pNaviCom->Set_Index(38);// Snow Init Idx 38  //43 Idle
		//Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");

	case LOAD_END:
		break;
	default:
		break;
	}



//Test	
	//m_eCurState = RUSSIAN_FIST_ATTACKJUMP_S;




	return S_OK;
}

HRESULT CRussianHat::LateReady_GameObject()
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	//m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Navi", Engine::ID_STATIC));
	//m_pNaviCom->Set_Index(38);// Base Init Idx 38 
	//m_pSword= dynamic_cast<IceSword*>(Engine::Get_GameObject(L"GameLogic",L"Ice")
	Engine::CLayer* pLayer = Engine::Get_Layer(L"GameLogic");
	Engine::CGameObject* pGameObject = m_pSword = CIceSword::Create(m_pGraphicDev, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianSword", pGameObject), E_FAIL);
	m_pSword->Set_Equip(false);

	m_pCam = dynamic_cast<CThirdPersonCamera*>(Engine::Get_GameObject(L"UI", L"ThirdPersonCamera"));


	_float fTestY = -4.f;
	pGameObject =m_pSplashEffect[0]=  CSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_01", L"RussianHat_0", "RightHand", _vec2(1.f, 1.f), _vec3(0.f, fTestY, 0.f), false, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Shield_Splash_01", pGameObject), E_FAIL);

	pGameObject = m_pSplashEffect[1] = CSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_02", L"RussianHat_0", "RightHand", _vec2(2.f, 2.f), _vec3(0.f, fTestY, 0.f), false, true);
	dynamic_cast<CSplashEffect*>(pGameObject)->Set_Distortion();
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Shield_Splash_02", pGameObject), E_FAIL);

	pGameObject = m_pSplashEffect[2] = CSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_03", L"RussianHat_0", "RightHand", _vec2(1.5f, 1.5f), _vec3(0.f, fTestY, 0.f), false, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Shield_Splash_03", pGameObject), E_FAIL);

	pGameObject = m_pSplashEffect[3] = CSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_05", L"RussianHat_0", "RightHand", _vec2(1.f, 1.f), _vec3(0.f, 1.f+ fTestY, 0.f), false, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Shield_Splash_05", pGameObject), E_FAIL);

	pGameObject = m_pSplashEffect[4] = CSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_06", L"RussianHat_0", "RightHand", _vec2(2.f, 2.f), _vec3(0.f, fTestY, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Shield_Splash_06", pGameObject), E_FAIL);



	pGameObject = m_pChargeEffect[0] = CChargeEffect::Create(m_pGraphicDev, L"Shine", L"RussianHat_0", "LeftHand", _vec2(1.f, 1.f), _vec3(INIT_VEC3), true,true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Charge1Effect", pGameObject), E_FAIL);
	m_pChargeEffect[0]->Set_ChargeSpeed(4.f);
	
	pGameObject = m_pChargeEffect[1] = CChargeEffect::Create(m_pGraphicDev, L"TC5RadialGradient09", L"RussianHat_0", "Hips_FrontAemor", _vec2(0.25f, 0.25f), _vec3(INIT_VEC3), true, false);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Charge2Effect", pGameObject), E_FAIL);
	m_pChargeEffect[1]->Set_ChargeSpeed(2.f);

	pGameObject = m_pChargeEffect[2] = CChargeEffect::Create(m_pGraphicDev, L"Shine", L"RussianHat_0", "Hips_FrontAemor", _vec2(0.5f, 0.5f), _vec3(INIT_VEC3), true, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Charge3Effect", pGameObject), E_FAIL);
	m_pChargeEffect[2]->Set_ChargeSpeed(2.f);
	//TC5RadialGradient09
	pGameObject = m_pExplosionEffect[0] = CExplosionEffect::Create(m_pGraphicDev, L"Explosion", L"RussianHat_0", "Hips_FrontAemor", _vec2(1.f, 1.f), _vec3(0.f, 0.f, 0.f), false,5.f, true);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_ExplosionEffect", pGameObject), E_FAIL);

	pGameObject = m_pExplosionEffect[1] = CExplosionEffect::Create(m_pGraphicDev, L"Ring", L"RussianHat_0", "Hips_FrontAemor", _vec2(1.f, 1.f), _vec3(0.f, 0.f, 0.f), false, 5.f, true);
	dynamic_cast<CExplosionEffect*>(pGameObject)->Set_Distortion();
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_RingEffect", pGameObject), E_FAIL);



	
	//pGameObject = CSnowSplashEffect::Create(m_pGraphicDev, L"Shine", L"RussianHat_0", "LeftHand", _vec2(1.f, 1.f),_vec3(INIT_VEC3),true);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_ChargeEffect", pGameObject), E_FAIL);

	//pGameObject = m_pChargeEffect[1] = CSplashEffect::Create(m_pGraphicDev, L"Shine", L"RussianHat_0", "LeftHand", _vec2(2.f, 2.f), _vec3(0.f, fTestY, 0.f));
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"RussianHat_Shield_Splash_06", pGameObject), E_FAIL);

	return S_OK;
}

_int CRussianHat::Update_GameObject(const _float & fTimeDelta)
{
	if (m_uiPass == 2)
	{
		if (m_fDSTime < 3.4f)
			m_fDSTime += fTimeDelta*0.06f;
		else
			m_fDSTime = 3.4f;
	}

	if (CKeyMgr::GetInstance()->KeyPressing(KEY_U))
	{
		m_pTransformCom->Set_Pos(13.6f, 6.4578f, -62.274f);
		m_pNaviCom->Set_Index(148);// Base Init Idx 38 


	}
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM3))
	//{
	//	//m_pChargeEffect[0]->Set_Enable(false);
	//	//_vec3 vAddPos = Get_Look();
	//	//for (int i = 0; i < 5; i++)
	//	//	m_pSplashEffect[i]->Set_Enable(true, vAddPos);
	//}

	////ChargeEffect(fTimeDelta, 1.f);
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM2))
	//{
	//	On_ExplosionEffect(fTimeDelta);
	//	//On_KetsugiEffect(fTimeDelta);
	//	//for (int i = 1; i < 3; i++)
	//	//	m_pChargeEffect[i]->Set_Enable(true);
	//}
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM1))
	//{
	//	//Off_KetsugiEffect(fTimeDelta);
	//	Off_ExplosionEffect(fTimeDelta);

	//	//for (int i = 1; i < 3; i++)
	//	//	m_pChargeEffect[i]->Set_Enable(false);
	//}
	//m_eCurState = RUSSIAN_DEFORMATION;
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM3))
	//{
	//	m_pTransformCom->Set_Pos(13.6f, 6.4578f, -62.274f);
	//	m_pNaviCom->Set_Index(148);// Base Init Idx 38 

	//}
	//if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM2))
	//{
	//	m_eCurState = RUSSIAN_ICEBLADE_N;
	//}

	//if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM1))
	//{
	//	m_uiAni++;
	//	cout << m_uiAni << endl;
	//}
		//StateMachine();
		//Pattern(fTimeDelta);
	//m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, true);
	//cout << "보스 체력= " << m_fCurHp << endl;
	//cout << m_eCurState << endl;



	srand((unsigned int)time(NULL));
	if (m_fCurHp > 0.f)
	{
		UpdateGague(fTimeDelta);
		if (!m_bIsStart)
		{
			Battle_Start(fTimeDelta);
			StateMachine();

		}
		else
		{
			if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM1))
				m_eCurState = RUSSIAN_DEFORMATION;

			StateMachine();
			m_fDistance = Get_TargetDist();
			Pattern(fTimeDelta);

		}
	}
	else
	{
		m_eCurState = RUSSIAN_DEATH_N;
		StateMachine();
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(m_fAnimSpeed*fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return S_OK;
}

void CRussianHat::Render_GameObject(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);
	pEffect->SetFloat("g_fTime", m_fDSTime);
	pEffect->SetTexture("g_DissolveTexture", m_pNoiseTextureCom->Get_Texture());
	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(m_uiPass);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderGroupCom->Render_Collider();

}

HRESULT CRussianHat::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"RussianHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// 변경
	
	pComponent = m_pNoiseTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"T_FX_ExternalRGBNoise01"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);// 변경


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

	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev, m_wstrInstName, m_pTransformCom, m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);
	for (int i = 0; i < Engine::COLOPT_END; i++)
		m_pColliderGroupCom->Set_ColliderEnable((Engine::COLLOPTION)i, true);



	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CNaviMesh::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	return S_OK;
}

void CRussianHat::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale = m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle = m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}
}

void CRussianHat::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		m_ePreState = m_eCurState;
		m_fAnimSpeed = 1.0f;

		switch (m_eCurState)
		{
		case RUSSIAN_START_IDLE:  
		{

			m_fAttackRange = 4.0f;
			m_fAnimSpeed = 2.0f;
			m_pMeshCom->Set_AnimationSet(43);
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);
			//m_pMeshCom->Set_AnimationSet(42);

		}
		break;

		case RUSSIAN_BATTLE_START:
		{
			Engine::CLayer*	pLayer = Engine::Get_Layer(L"UI");
			Engine::CGameObject *pGameObject = nullptr;

			pGameObject= m_pHPGauge = CGauge::Create(m_pGraphicDev, L"BossHPBar", _vec3(WINCX*0.5, 20.f, 0.1f), PIVOT_M);
			pLayer->Add_GameObject(L"Boss_HPBar", pGameObject);
		

			pGameObject = m_pHPGauge = CGauge::Create(m_pGraphicDev, L"BossHP", _vec3(WINCX*0.5, 20.f, 0.09f), PIVOT_M, _vec3(0.05f, 0.f, 1.0f));
			pLayer->Add_GameObject(L"Boss_HP", pGameObject);


			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

			m_fAnimSpeed = 2.5f;
			m_pMeshCom->Set_AnimationSet(44);
			//m_pMeshCom->Set_AnimationSet(43);
		}
			break;
		case RUSSIAN_BATTLE_IDLE:
		{
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 3.0f;
			m_fAttackRange = 7.f;
			m_pMeshCom->Set_AnimationSet(42);
			//m_pMeshCom->Set_AnimationSet(41);
		}
		break;
		case RUSSIAN_WALK://29~32
		{
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

			m_fSpeed = 1.f;
			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 3.0f;
			m_fAttackRange = 20.f;

			m_pMeshCom->Set_AnimationSet(35);
			//m_pMeshCom->Set_AnimationSet(34);
		}
		break;
		case RUSSIAN_RUN: //
		{
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

			m_fSpeed = 3.f;
			m_fRotSpeed = 6.0f;
			m_fAnimSpeed = 1.5f;
			m_fAttackRange = 4.f;
			m_pMeshCom->Set_AnimationSet(37);
			//m_pMeshCom->Set_AnimationSet(36);
		}
			break;
		case RUSSIAN_HURT://26~27
			break;
		case RUSSIAN_APPERANCE_N:
			m_pMeshCom->Set_AnimationSet(44);
			//m_pMeshCom->Set_AnimationSet(43);
			break;
		case RUSSIAN_DOWN:
			m_pMeshCom->Set_AnimationSet(29);
			//m_pMeshCom->Set_AnimationSet(28);
			break;
		case RUSSIAN_DODGE:
		{
			m_fAttackRange = 40.f;
			m_fAnimSpeed = 2.f;

			m_pMeshCom->Set_AnimationSet(32);
			//m_pMeshCom->Set_AnimationSet(31);
		}
			break;
		case RUSSIAN_ATTACK_READY:
		{
			m_fAnimSpeed = 1.5f;
			m_fRotSpeed = 5.0f;

			m_fAttackRange = 40.f;
			m_pMeshCom->Set_AnimationSet(28);
			//m_pMeshCom->Set_AnimationSet(27);
		}
			break;
		case RUSSIAN_ATTACK1:
		{
			m_fAnimSpeed = 2.0f;
			m_fRotSpeed = 5.0f;
			m_pMeshCom->Set_AnimationSet(27);
			//m_pMeshCom->Set_AnimationSet(26);
		}
			break;
		case RUSSIAN_ATTACK2:
			m_fAnimSpeed = 3.0f;
			m_pMeshCom->Set_AnimationSet(26);
			//m_pMeshCom->Set_AnimationSet(25);
			break;
		case RUSSIAN_ATTACK3:
			m_pMeshCom->Set_AnimationSet(25);
			//m_pMeshCom->Set_AnimationSet(24);
			break;

		case RUSSIAN_ATTACK_SLIDE_S:
			m_fRotSpeed = 17.0f;
			m_fAttackRange = 40.f;
			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(24);
			//m_pMeshCom->Set_AnimationSet(23);
			break;
		case RUSSIAN_ATTACK_SLIDE_L:
			m_fAttackRange = 40.f;
			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(23);
			//m_pMeshCom->Set_AnimationSet(22);
			break;
		case RUSSIAN_ATTACK_SLIDE_E:
			m_fAttackRange = 40.f;

			m_pMeshCom->Set_AnimationSet(22);
			//m_pMeshCom->Set_AnimationSet(21);
			break;
		case RUSSIAN_ATTACK_HORN1:
		{
			m_fAttackRange = 10.f;

			m_fRotSpeed = 15.0f;
			m_pMeshCom->Set_AnimationSet(20);
			//m_pMeshCom->Set_AnimationSet(19);
		}
			break;
		case RUSSIAN_ATTACK_HORN2:
		{
			m_pMeshCom->Set_AnimationSet(19);
			//m_pMeshCom->Set_AnimationSet(18);
		}
		break;
		case RUSSIAN_ATTACK_JUMP:
		{
			m_fRotSpeed = 17.0f;

			m_fAnimSpeed = 3.0f;
			m_fAttackRange = 40.f;
			m_pMeshCom->Set_AnimationSet(18);
			//m_pMeshCom->Set_AnimationSet(17);
		}
			break;
		case RUSSIAN_DEFORMATION:
		{
			m_pColliderGroupCom->Set_ColliderEnable(Engine::COLOPT_ATTACK, false);

			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(17);
			//m_pMeshCom->Set_AnimationSet(16);
		}
			break;
		case RUSSIAN_KETSUGI_AIMSHOOT:
		{
			m_pMeshCom->Set_AnimationSet(21);
			//m_pMeshCom->Set_AnimationSet(20);
		}
		break;
		
		case RUSSIAN_FIST_ATTACKBOOST1_S:
		{	
			m_fAnimSpeed = 2.0f;
			m_fRotSpeed = 7.0f;
			m_fAttackRange = 30.f;
			m_pMeshCom->Set_AnimationSet(16);
			//m_pMeshCom->Set_AnimationSet(15);
		}
		break;
		case RUSSIAN_FIST_ATTACKBOOST1_L:
		{	
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(15);
			//m_pMeshCom->Set_AnimationSet(14);
		}
		break;
		case RUSSIAN_FIST_ATTACKBOOST1_A_E:
			m_fAnimSpeed = 2.0f;
			m_pMeshCom->Set_AnimationSet(14);
			//m_pMeshCom->Set_AnimationSet(13);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_B_E:
			m_pMeshCom->Set_AnimationSet(13);
			//m_pMeshCom->Set_AnimationSet(12);
			break;
		case RUSSIAN_FIST_ATTACKNORMAL1:
			m_fAttackRange = 6.0f;
			m_pMeshCom->Set_AnimationSet(11);
			//m_pMeshCom->Set_AnimationSet(10);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_S:
			m_fRotSpeed = 4.0f;
			m_fAttackRange = 30.f;
			m_fAnimSpeed = 2.f;
			m_pMeshCom->Set_AnimationSet(7);
			//m_pMeshCom->Set_AnimationSet(6);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_L:
			m_fAnimSpeed = 2.f;
			m_pMeshCom->Set_AnimationSet(9);
			//m_pMeshCom->Set_AnimationSet(8);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_E:
			m_fAnimSpeed = 2.f;
			m_fRotSpeed = 1.0f;

			m_pMeshCom->Set_AnimationSet(8);
			//m_pMeshCom->Set_AnimationSet(7);
			break;
		case RUSSIAN_FIST_BOOSTUP_S:
		{
			m_pMeshCom->Set_AnimationSet(5);
			//m_pMeshCom->Set_AnimationSet(6);
		}	break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_S:
		{	
			m_pMeshCom->Set_AnimationSet(6);
			//m_pMeshCom->Set_AnimationSet(5);
		}
		break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_L:
		{	
			m_pMeshCom->Set_AnimationSet(5);
			//m_pMeshCom->Set_AnimationSet(4);
		}
		break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_E:
		{	
			m_pMeshCom->Set_AnimationSet(4);
			//m_pMeshCom->Set_AnimationSet(3);
		}
		break;
		case RUSSIAN_DEATH_N:
			m_pMeshCom->Set_AnimationSet(0);
			m_uiPass = 2;
			break;
		case RUSSIAN_ICEBLADE_N:
			m_pMeshCom->Set_AnimationSet(25);
			break;
		case RUSSIAN_END:
			break;
		default:
			break;
		}

	}

}

void CRussianHat::Battle_Start(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_BATTLE_START)
	{
		_float fRatio = Get_AniRatio();
		if(fRatio>=0.3f)
			PlayMonSound(L"Blunt.wav", m_bIsAtkSound);
		if (fRatio >= 0.43f)
			PlayMonSound(L"Hit_Scale_Blunt.wav", m_bIsHurtSound);

		cout << Get_AniRatio() << endl;
		if (Get_AniRatio()>=0.7f)
		{
			m_bIsStart = true;
			m_bIsAtkSound = false;
			m_bIsHurtSound = false;// 변수만들기싫어서씀
		}
	}
	if (m_eCurState == RUSSIAN_START_IDLE)
	{

		if (Get_TargetDist() <= 6.0f)
			m_eCurState = RUSSIAN_BATTLE_START;
	}

}

void CRussianHat::Pattern(_float fTimeDelta)
{
	if (m_bIsPhase2)
	{
		Phase2(fTimeDelta);
		return;
	}
	if (m_fCurHp / m_fMaxHp >= 0.6f&&!m_bIsPhase2)
		Phase1(fTimeDelta);
	else if(m_fCurHp / m_fMaxHp <= 0.6f)
	{
		m_eCurState = RUSSIAN_DEFORMATION;
		Deformation(fTimeDelta);
	}


}

void CRussianHat::UpdateGague(_float fTimeDelta)
{
	if (m_pHPGauge != nullptr)
	{
		m_pHPGauge->Set_GaugeRatio(m_fCurHp / m_fMaxHp);
	}

}


void CRussianHat::Chaing_Target(_float fTimeDelta)
{
	if (m_pNaviCom != nullptr)
	{
		if (m_eCurState == RUSSIAN_RUN||m_eCurState == RUSSIAN_WALK)
		{
			_vec3 vDir, vOutPos;
			_float	fDgree = Get_AngleOnTheTarget();
			vDir = Get_TargetPos() - Get_Pos();
			m_fDistance = D3DXVec3Length(&vDir);
			vDir.y = 0.f;
			D3DXVec3Normalize(&vDir, &vDir);
			RotateToTarget(fTimeDelta, 0.0f);

			m_pNaviCom->Move_OnNaviMesh(&Get_Pos(), &(vDir * m_fSpeed* fTimeDelta), &vOutPos);
			m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
		}
	}

}

void CRussianHat::Phase1(_float fTimeDelta)
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM2))
		m_eCurState = RUSSIAN_ATTACK_HORN1;
	if (m_fDistance >= m_fAttackRange)
	{
		if (m_fDistance >= 7.f)
		{
			if (rand() & 1)
				m_eCurState = RUSSIAN_ATTACK_JUMP;
			else
				m_eCurState = RUSSIAN_ATTACK_READY;

		}

		 if (m_fDistance < 7.f)
			m_eCurState = RUSSIAN_RUN;
		else if (m_fDistance < 4.f)
			m_fAttackRange =10.f;

		Chaing_Target(fTimeDelta);
	}
	else
	{
		if (m_fDistance <= m_fAttackRange)
		{
			switch (m_eCurState)
			{
			case RUSSIAN_BATTLE_IDLE:
				Idle(fTimeDelta);
				break;
			case RUSSIAN_DODGE:
				Dodge(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_READY:
				TshieldAttack_Ready(fTimeDelta);
				break;
			case RUSSIAN_ATTACK1:
				TshieldAttack(fTimeDelta);
				break;
			case RUSSIAN_ATTACK2:
				Tshield_Whirlwind(fTimeDelta);
				break;
			case RUSSIAN_ATTACK3:
				TshieldFist(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_SLIDE_S:
				TshieldSlide_S(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_SLIDE_L:
				TshieldSlide_L(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_SLIDE_E:
				TshieldSlide_E(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_JUMP:
				Tshield_JumpAttack(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_HORN1:
				HoneAttack1(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_HORN2:
				break;
			case RUSSIAN_DEFORMATION:
				Deformation(fTimeDelta);
				break;
			case RUSSIAN_KETSUGI_AIMSHOOT:
				break;
			case RUSSIAN_DEATH_N:
				m_fAnimSpeed = 2.0f;

				break;
			default:
				Idle(fTimeDelta);
				break;
			
			}

		}
	}

}

void CRussianHat::TshieldAttack_Ready(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_READY)
	{

		m_fRotSpeed = 3.0f;
		RotateToTarget(fTimeDelta, 0.0f, 0.3f);

		m_fRotSpeed = 10.0f;
		RotateToTarget(fTimeDelta, 0.43f);

		SetColliderEnable(0.4f, 0.5f);
		if (Get_AniRatio() >= 0.1f)
			PlayMonSound(L"delia_swing_strong_v1.wav", m_bIsAtkSound);
		if (Get_AniRatio() >= 0.4f)
			PlayMonSound(L"Hurk_Swing_GreatSword_KnockBack_01.wav", m_bIsThieldSound);

		if (Get_AniRatio() >= 0.58f)
		{
			m_bIsAtkSound = false;
			m_bIsThieldSound = false;
			if (Get_TargetDist() >= 6.0f || rand() & 1)
				m_eCurState = RUSSIAN_ATTACK1;
			else
			
				m_eCurState = RUSSIAN_ATTACK2;
		}

	}
}

void CRussianHat::TshieldAttack(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK1)
	{

		RotateToTarget(fTimeDelta, 0.0f, 0.1f);
		SetColliderEnable(0.0f, 0.89f);

		if (Get_AniRatio() >= 0.9f)
		{
			m_bIsBoostSound = false;
			m_eCurState = RUSSIAN_BATTLE_IDLE;
		}
		else
		{
			//PlayMonSound(L"Charge.wav", m_bIsBoostSound);
			if(Get_AniRatio()<0.5f)
				PlayStepSound(L"FireCrash.wav", fTimeDelta, 0.25f);
			if (Get_AniRatio() <= 0.3f)
			{
				//BoostEffect(fTimeDelta);
				SnowSplashEffect(fTimeDelta, 100.f);
			}

			MoveAni(fTimeDelta, 0.0f, 0.1f, 30.0f, Get_Look());
			MoveAni(fTimeDelta, 0.1f, 0.2f, 10.0f, Get_Look());
			MoveAni(fTimeDelta, 0.2f, 0.3f, 30.0f, Get_Look());
		}
	}
}

void CRussianHat::Tshield_Whirlwind(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK2)
	{
		PlayMonSound(L"SpinAttack.wav", m_bIsAtkSound);

		SetColliderEnable(0.2f, 0.6f);


		if (Get_AniRatio() >= 0.9f)
		{
			m_eCurState = RUSSIAN_BATTLE_IDLE;
			m_bIsAtkSound = false;
		}
		else
		{
			if(Get_AniRatio()<=0.7f)
				SnowSplashEffect(fTimeDelta, 100.f);
			RotateToTarget(fTimeDelta, 0.0f, 0.2f);
			MoveAni(fTimeDelta, 0.2f, 0.6f , 6.0f, Get_Look());
		}
	}
}

void CRussianHat::TshieldFist(_float fTimeDelta)
{

	if(m_eCurState == RUSSIAN_ATTACK3)
	{
		if(Get_AniRatio()>=0.25f)
			PlayMonSound(L"Fist.wav", m_bIsAtkSound);

		SetColliderEnable(0.29f, 0.47f);
		if (m_fAttackRange == 4.0f)
			m_fAttackRange = 8.0f;

		if (Get_AniRatio() >= 0.8f)
		{
			m_bIsAtkSound = false;
			if (!m_bIsPhase2)
				m_eCurState = RUSSIAN_DODGE;
			else
			{
				if (m_uiFist >= 1)
				{
					m_uiFist = 0;
					m_eCurState = RUSSIAN_BATTLE_IDLE;
				}
				else
				{
					m_eCurState = RUSSIAN_FIST_ATTACKNORMAL1;
					m_uiFist++;
				}


			}

		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.4f);
			MoveAni(fTimeDelta, 0.3f, 0.4f, 6.f, Get_Look());
			if (!m_bIsPhase2)
			{
				if (Get_AniRatio() >= 0.4f)
					m_fAnimSpeed = 3.5f;
			}
			else
			{
				if (Get_AniRatio() >= 0.4f)
					m_fAnimSpeed = 3.5f;
				else
					m_fAnimSpeed = 2.f;
			}

		}

	}
}

void CRussianHat::TshieldSlide_S(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_SLIDE_S)
	{


		SetColliderEnable(0.5f, 0.8f);
		if (Get_AniRatio() >= 0.25f)
			PlayMonSound(L"SliceOn.wav", m_bIsAtkSound);

		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_ATTACK_SLIDE_L;
			m_bIsAtkSound = false;
		}
		else
		{
			
			RotateToTarget(fTimeDelta, 0.f, 0.6f);
			MoveAni(fTimeDelta, 0.6f, 0.8f, 4.0f, Get_Look());

		}
	}
}

void CRussianHat::TshieldSlide_L(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_SLIDE_L)
	{
		PlayStepSound(L"FireCrash.wav", fTimeDelta, 0.1f);
		SetColliderEnable(0.0f, 0.8f);
		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_ATTACK_SLIDE_E;
		}
		else
		{
			SnowSplashEffect(fTimeDelta, 100.f, 0.05f);

			BoostEffect(fTimeDelta);
			MoveAni(fTimeDelta, 0.0f, 0.7f, 25.0f, Get_Look());
			MoveAni(fTimeDelta, 0.7f, 0.8f, 15.0f, Get_Look());
		}
	}

}

void CRussianHat::TshieldSlide_E(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_SLIDE_E)
	{
		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_BATTLE_IDLE;
		}
		else
		{
			MoveAni(fTimeDelta, 0.0f, 0.2f, 8.0f, Get_Look());

		}
	}
}

void CRussianHat::Dodge(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_DODGE)
	{

		if (Get_AniRatio() >= 0.08f)
			PlayMonSound(L"BackStep1.wav", m_bIsBoostSound);
		if (Get_AniRatio() >= 0.4f)
			PlayMonSound(L"BackStep2.wav", m_bIsAtkSound);

		if (Get_AniRatio() >= 0.65f)
		{
			m_bIsBoostSound = false;
			m_bIsAtkSound = false;
			if(!m_bIsPhase2)
				m_eCurState = RUSSIAN_ATTACK_SLIDE_S;
			else
				m_eCurState = RUSSIAN_FIST_ATTACKBOOST1_S;

		}
		else
		{
			MoveAni(fTimeDelta, 0.1f, 0.4f, 10.f, -Get_Look());
			MoveAni(fTimeDelta, 0.4f, 0.5f, 4.f, -Get_Look());
			MoveAni(fTimeDelta, 0.5f, 0.6f, 1.f, -Get_Look());



		}
	}
}

void CRussianHat::Tshield_JumpAttack(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_JUMP)
	{
		SetColliderEnable(0.5f, 0.6f);
		PlayMonSound(L"BoostJump.wav", m_bIsJumpSound);

		
		if (Get_AniRatio() >= 0.48f)
			PlayMonSound(L"HoneUp.wav", m_bIsAtkSound);

		if (Get_AniRatio() >= 0.65f)
		{
			m_bIsAtkSound = false;
			m_bIsJumpSound = false;
			m_eCurState = RUSSIAN_ATTACK2;
			m_bIsSplash = false;
		}
		else
		{
			if(Get_AniRatio()>=0.4f)
			SplashEffect();
		

			RotateToTarget(fTimeDelta, 0.f, 0.1f);
			if(Get_AniRatio()>=0.05f&&Get_AniRatio() <= 0.5f)
				BoostEffect(fTimeDelta);
			MoveAni(fTimeDelta, 0.1f, 0.125f, 12.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.125f, 0.3f, 8.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.3f, 0.35f, 10.f, _vec3(0.f, -1.f, 0.f), true);
			MoveAni(fTimeDelta, 0.35f, 0.5f, 50.f, _vec3(0.f, -1.f, 0.f), true);
			MoveAni(fTimeDelta, 0.1f, 0.2f, 20.f, Get_Look(), true);
			MoveAni(fTimeDelta, 0.2f, 0.5f, 10.f, Get_Look(), true);
		}
		
	
	
	}
}

void CRussianHat::HoneAttack1(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_HORN1)
	{//
		if (Get_AniRatio() >= 0.05f)
			PlayMonSound(L"Hurk_Swing_GreatSword_Strong_01.wav", m_bisSwing);
		if (Get_AniRatio() >= 0.18f)
			PlayMonSound(L"HoneDown.wav", m_bIsAtkSound);
		if (Get_AniRatio() >= 0.48f)
			PlayMonSound(L"HoneUp.wav", m_bIsHoneSound);

		if (Get_AniRatio() >= 0.8f)
		{
			m_bIsAtkSound = false;
			m_bIsHoneSound = false;
			m_bisSwing = false;
			m_fAnimSpeed = 1.0f;
			if (!m_bIsPhase2)
			{
				m_bIsSpawnHone = false;

				if (m_uiPattern & 1)
					m_eCurState = RUSSIAN_ATTACK3;
				else
					m_eCurState = RUSSIAN_ATTACK_READY;
			}
			else
			{
				m_eCurState = RUSSIAN_BATTLE_IDLE;
				m_bIsSpawnHone = false;
			}
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.1f);
			if (Get_AniRatio() <= 0.4f)
				m_fAnimSpeed = 2.5f;

			if (Get_AniRatio() > 0.5f)
			{
				if (!m_bIsSpawnHone)
				{
					_float fDgree = Get_Angle(Get_Look(), _vec3(0.f, 0.f, 1.f));
					fDgree += 180.f;
					_vec3 vHonePos = Get_Pos() + (Get_Look()*5.f);
					Engine::CGameObject* pGameObject = CRussainHatHone::Create(m_pGraphicDev, vHonePos, D3DXToRadian(fDgree));
					Engine::CLayer* pLayer = Engine::Get_Layer(L"GameLogic");
					wstring wstrInstName = L"Hone_" + to_wstring(m_uiHoneIdx);
					pLayer->Add_GameObject(wstrInstName.c_str(), pGameObject);
					m_uiHoneIdx++;
					m_bIsSpawnHone = true;
				}
			}
			if (Get_AniRatio() >= 0.4f)
			{
				m_fAnimSpeed = 4.f;
			}
		}



	}
}

void CRussianHat::Idle(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_WALK||m_eCurState== RUSSIAN_RUN)
	{
		m_eCurState = RUSSIAN_BATTLE_IDLE;
	
	}
	else if (m_eCurState == RUSSIAN_BATTLE_IDLE)
	{


		if (!m_bIsPhase2)
		{
			if (Get_AniRatio() >= 0.05f)
			{

				if (m_uiPattern == 0)
					m_eCurState = RUSSIAN_ATTACK_HORN1;
				else if (m_uiPattern == 1)
					m_eCurState = RUSSIAN_ATTACK_READY;
				else if (m_uiPattern == 2)
					m_eCurState = RUSSIAN_ATTACK3;
				else if (m_uiPattern == 3)
					m_eCurState = RUSSIAN_ATTACK_SLIDE_S;
				else if (m_uiPattern >= 4)
					m_uiPattern = -1;
				m_uiPattern++;

			}
		}
		else
		{
			if (m_uiPattern == 0)
				m_eCurState = RUSSIAN_FIST_ATTACKBOOST1_S;
			else if (m_uiPattern == 1)
				m_eCurState = RUSSIAN_ICEBLADE_N;
			else if (m_uiPattern == 2)
				m_eCurState = RUSSIAN_FIST_ATTACKJUMP_S;
			else if (m_uiPattern == 3)
				m_eCurState = RUSSIAN_FIST_ATTACKNORMAL1;
			else if (m_uiPattern == 4)
				m_eCurState = RUSSIAN_ATTACK_HORN1;
			else if (m_uiPattern >= 5)
				m_uiPattern = -1;

			m_uiPattern++;
		}

	}
	else if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = RUSSIAN_BATTLE_IDLE;
	}
}

void CRussianHat::Deformation(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_DEFORMATION)
	{
		if (Get_AniRatio() >= 0.25f)
		{
			CShield* pShield= dynamic_cast<CShield*>(Engine::Get_GameObject(L"GameLogic", L"Shield"));
			if (pShield != nullptr)
			{
				pShield->Set_Equip(false);
				pShield->Set_CellIdx(m_pNaviCom->Get_CurIndex());
				pShield->Set_Throw();

			}
		}
		if (Get_AniRatio() >= 0.85f)
		{
			m_bIsPhase2 = true;
			m_eCurState = RUSSIAN_BATTLE_IDLE;

		}


	}
}

void CRussianHat::Phase2(_float fTimeDelta)
{
	if (m_fDistance >= m_fAttackRange)
	{
		if (m_fDistance >= 7.f)
		{
			if (rand() & 1)
				m_eCurState = RUSSIAN_FIST_ATTACKJUMP_S;
			else
				m_eCurState = RUSSIAN_FIST_ATTACKBOOST1_S;

		}
		else if (m_fDistance < 7.f)
			m_eCurState = RUSSIAN_RUN;
		else if (m_fDistance < 4.f)
			m_fAttackRange = 10.f;

		Chaing_Target(fTimeDelta);

	}
	else
	{

		if (m_fDistance <= m_fAttackRange)
		{
			switch (m_eCurState)
			{
			case RUSSIAN_DODGE:
			{
				Dodge(fTimeDelta);
			}
			case RUSSIAN_ATTACK_HORN1:
			{				
				HoneAttack1(fTimeDelta);
			}
			break;

			case RUSSIAN_FIST_ATTACKBOOST1_S:
			{
				BoostAttack_S(fTimeDelta);
			}
			break;
			case RUSSIAN_FIST_ATTACKBOOST1_L:
			{
				BoostAttack_L(fTimeDelta);
			}
			break;
			case RUSSIAN_FIST_ATTACKBOOST1_A_E:
			{
				BoostAttack_E(fTimeDelta);
			}
			break;
			case RUSSIAN_FIST_ATTACKNORMAL1:
			{
				FistAttack_N(fTimeDelta);
			}
			break;
			case RUSSIAN_ATTACK3:
			{
				TshieldFist(fTimeDelta);
			}
			break;
			case RUSSIAN_FIST_ATTACKJUMP_S:
			{
				BoostJump_S(fTimeDelta);
			}
			break;
			case RUSSIAN_FIST_ATTACKJUMP_L:
			{
				BoostJump_L(fTimeDelta);
			}
			break;
			case RUSSIAN_FIST_ATTACKJUMP_E:
			{
				BoostJump_E(fTimeDelta);
			}
			break;
			case RUSSIAN_ICEBLADE_N:
			{
				IceBlade(fTimeDelta);
			}
			break;
			default:
			{
				Idle(fTimeDelta);
			}
			break;
			}


		}
	}
}

void CRussianHat::BoostAttack_S(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_FIST_ATTACKBOOST1_S)
	{
		if (Get_AniRatio() >= 0.05f)
			PlayMonSound(L"Hurk_Swing_GreatSword_Strong_02.wav", m_bIsAtkSound);

		if(Get_AniRatio()>=0.5f)
			PlayMonSound(L"Hurk_Swing_GreatSword_KnockBack_01.wav", m_bIsBoostSound);
		if (Get_AniRatio() >= 0.7f)
		{
			m_eCurState = RUSSIAN_FIST_ATTACKBOOST1_L;
			m_bIsBoostSound = false;
			m_bIsAtkSound = false;
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.05f, 0.45f);

		}
	}
}

void CRussianHat::BoostAttack_L(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_FIST_ATTACKBOOST1_L)
	{
		if (Get_AniRatio() >= 3.5f)
			m_eCurState = RUSSIAN_FIST_ATTACKBOOST1_A_E;
		else
		{
			PlayStepSound(L"FireCrash.wav", fTimeDelta, 0.15f);

			if (Get_AniRatio() <= 3.2f)
			{
				SnowSplashEffect(fTimeDelta, 200.f);
				BoostEffect(fTimeDelta);
			}
			MoveAni(fTimeDelta, 0.0f,3.4f, 28.f, Get_Look());
			MoveAni(fTimeDelta, 3.4f, 3.5f, 10.f, Get_Look());

		}
	}
}

void CRussianHat::BoostAttack_E(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_FIST_ATTACKBOOST1_A_E)
	{
		if (Get_AniRatio() >= 0.8f)
		{
			m_fAttackRange = 4.0f;
			m_eCurState = RUSSIAN_BATTLE_IDLE;

		}
		else
		{

			MoveAni(fTimeDelta, 0.f, 0.1f, 6.5f, Get_Look());
			MoveAni(fTimeDelta, 0.1f, 0.2f, 2.5f, Get_Look());

		}

	}
}

void CRussianHat::FistAttack_N(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_FIST_ATTACKNORMAL1)
	{
		if (Get_AniRatio() >= 0.25f)
			PlayMonSound(L"Fist.wav", m_bIsAtkSound);

		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_ATTACK3;
			m_bIsAtkSound = false;
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.4f);
			MoveAni(fTimeDelta, 0.3f, 0.4f, 6.f, Get_Look());
			if (Get_AniRatio() >= 0.4f)
				m_fAnimSpeed = 3.5f;
			else
				m_fAnimSpeed = 2.f;

		}
	}
}

void CRussianHat::IceBlade(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ICEBLADE_N)
	{
		m_fAttackRange = 20.0f;
		if (Get_AniRatio() >= 0.05f)
			PlayMonSound(L"Hit_common_ice.wav", m_bIsAtkSound);
		if (Get_AniRatio() >= 0.35f)
			PlayMonSound(L"IceBladeSwing.wav", m_bisSwing);


		if (Get_AniRatio() >= 0.58f)
		{
			m_pSword->Set_Equip(false);
		}
	
		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_BATTLE_IDLE;
			m_bIsAtkSound = false;
			m_bisSwing = false;
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.19f);
			SetColliderEnable(0.29f, 0.47f);

			if (Get_AniRatio() < 0.29f&&Get_AniRatio()>0.2f)
			{
				On_ChargeEffect(fTimeDelta);
				m_pSword->Set_Enable(true);
				m_pSword->Set_Equip(true);
				m_fAnimSpeed = 0.5f;
			}	
			else
			{
				Off_ChargeEffect(fTimeDelta);

			}

			if (Get_AniRatio() >= 0.29f)
			{
				m_fAnimSpeed = 1.25f;
				m_pSword->Set_Coll(true);
			}
			if (m_fAttackRange == 4.0f)
				m_fAttackRange = 8.0f;
		}


	}
}

void CRussianHat::BoostJump_S(_float fTimeDelta)
{

	if (m_eCurState == RUSSIAN_FIST_ATTACKJUMP_S)
	{
		if (Get_AniRatio() >= 0.1f)
			PlayMonSound(L"KetsugiCharge.wav", m_bIsAtkSound);
		if (Get_AniRatio() >= 0.48f)
			PlayStepSound(L"FireCrash.wav", fTimeDelta, 0.15f);

		if (Get_AniRatio() >= 0.89f)
		{
			m_bIsAtkSound = false;
			m_eCurState = RUSSIAN_FIST_ATTACKJUMP_L;
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.48f, 0.8f);
			if (Get_AniRatio() >= 0.48f)
			{
				BoostEffect(fTimeDelta);
			}
			else
			{
				On_KetsugiEffect(fTimeDelta);
			}

			MoveAni(fTimeDelta, 0.48f, 0.5f, 1.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.5f, 0.6f, 2.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.6f, 0.8f, 3.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.8f, 0.89f, 2.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.5f, 0.89f, 1.f, Get_Look(), true);
		}

	}

}

void CRussianHat::BoostJump_L(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_FIST_ATTACKJUMP_L)
	{
		PlayStepSound(L"FireCrash.wav", fTimeDelta, 0.15f);
		if (Get_AniRatio() >= 15.f)
		{
			m_eCurState = RUSSIAN_FIST_ATTACKJUMP_E;
		}
		else
		{
			BoostEffect(fTimeDelta);

			//RotateToTarget(fTimeDelta, 0.f, 0.3f);
			MoveAni(fTimeDelta, 0.0f, 4.f, 6.f, _vec3(0.f, -1.0f, 0.f), true);
			MoveAni(fTimeDelta, 4.f, 8.f, 12.f, _vec3(0.f, -1.0f, 0.f), true);
			MoveAni(fTimeDelta, 8.f, 11.f, 17.f, _vec3(0.f, -1.0f, 0.f), true);
			MoveAni(fTimeDelta, 11.f, 15.f, 25.f, _vec3(0.f, -1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.0f, 15.f, 30.f, Get_Look(), true);
		}



	}


}

void CRussianHat::BoostJump_E(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_FIST_ATTACKJUMP_E)
	{
		if (Get_AniRatio() >= 0.1f)
			PlayMonSound(L"Explosion.wav", m_bIsAtkSound);
		if (Get_AniRatio() >= 0.7f)
		{
		
			m_bIsAtkSound = false;
			m_eCurState = RUSSIAN_BATTLE_IDLE;
			m_fAttackRange = 4.f;
			Off_ExplosionEffect(fTimeDelta);
		}
		else
		{
			if (Get_AniRatio() <= 0.2f)
				m_fAnimSpeed = 5.0f;
			else
				m_fAnimSpeed = 2.0f;

			On_ExplosionEffect(fTimeDelta);



			MoveAni(fTimeDelta, 0.0f, 0.1f, 25.f, _vec3(0.f, -1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.0f, 0.1f, 4.f, Get_Look(), true);

			MoveAni(fTimeDelta, 0.2f, 0.3f, 2.f, Get_Look());
		}



	}
}

void CRussianHat::BoostEffect(_float fTimeDelta)
{

	if (m_fEffectRate >= 0.05f)
	{
		Engine::CLayer* pLayer = Engine::Get_Layer(L"GameLogic");
		
		wstring wstrInstName = L"FireR_" + to_wstring(m_uiEffectIdx);
		Engine::CGameObject* pGameObject = CBoostEffect::Create(m_pGraphicDev, L"Fire2", L"FireAlpha", true);
		pLayer->Add_GameObject(wstrInstName.c_str(), pGameObject);
		

		wstrInstName = L"FireL_" + to_wstring(m_uiEffectIdx);
		 pGameObject = CBoostEffect::Create(m_pGraphicDev, L"Fire2", L"FireAlpha", false);
		pLayer->Add_GameObject(wstrInstName.c_str(), pGameObject);

		m_uiEffectIdx++;
		m_fEffectRate = 0.f;
	}
	else
		m_fEffectRate += fTimeDelta;


}

void CRussianHat::SplashEffect()
{
	if (!m_bIsSplash)
	{
		_vec3 vAddPos = Get_Look()*3.f;
		for (int i = 0; i < 5; i++)
			m_pSplashEffect[i]->Set_Enable(true, vAddPos);
	
		m_bIsSplash = true;
	}
}

void CRussianHat::SnowSplashEffect(_float fTimeDelta,_float fLength,_float fRateTime)
{
	if (m_fEffectRate >= fRateTime)
	{
		Engine::CLayer* pLayer = Engine::Get_Layer(L"GameLogic");
		_vec3 vAddPos = Get_Look()*3.f*fLength;
		vAddPos.y = -2.f;
		wstring wstrInstName = L"SnowSplash_" + to_wstring(m_uiEffectIdx);
		Engine::CGameObject* pGameObject = CSnowSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_06",L"RussianHat_0","RightHand",_vec2(2.f,2.f), vAddPos);
		pLayer->Add_GameObject(wstrInstName.c_str(), pGameObject);


		wstrInstName = L"SnowSplashDistor_" + to_wstring(m_uiEffectIdx);
		pGameObject = CSnowSplashEffect::Create(m_pGraphicDev, L"RussianHat_Shield_Splash_06", L"RussianHat_0", "RightHand", _vec2(1.5f, 1.5f), _vec3(INIT_VEC3),true);
		pLayer->Add_GameObject(wstrInstName.c_str(), pGameObject);





		m_uiEffectIdx++;
		m_fEffectRate = 0.f;
	}
	else
		m_fEffectRate += fTimeDelta;
}

void CRussianHat::On_ChargeEffect(_float fTimeDelta)
{
	if (!m_bIsCharge)
	{
		m_bIsCharge = true;
		m_pChargeEffect[0]->Set_Enable(true);
	}
}

void CRussianHat::Off_ChargeEffect(_float fTimeDelta)
{
	
	if (m_bIsCharge)
	{
		m_bIsCharge = false;
		m_pChargeEffect[0]->Set_Enable(false);
	}
}

void CRussianHat::On_KetsugiEffect(_float fTimeDelta)
{
	if (!m_bIsCharge)
	{
		_vec3 vAddPos = Get_Look()*3.f;
		m_bIsCharge = true;
	
		for(int i=1; i<3;i++)
			m_pChargeEffect[i]->Set_Enable(true, vAddPos);



	}

}

void CRussianHat::Off_KetsugiEffect(_float fTimeDelta)
{
	if (m_bIsCharge)
	{
		_vec3 vAddPos = Get_Look();
		m_bIsCharge = false;
		for (int i = 1; i<3; i++)
			m_pChargeEffect[i]->Set_Enable(false);



	}
}

void CRussianHat::On_ExplosionEffect(_float fTimeDelta)
{
	if (!m_bIsExplosion)
	{
		_vec3 vAddPos = Get_Look()*3.f;
		m_bIsExplosion = true;

		for (int i = 0; i<2; i++)
			m_pExplosionEffect[i]->Set_Enable(true, vAddPos);
	}
}


void CRussianHat::Off_ExplosionEffect(_float fTimeDelta)
{
	if (m_bIsExplosion)
	{
		for (int i = 0; i < 2; i++)
			m_pExplosionEffect[i]->Set_Enable(false);

		Off_KetsugiEffect(fTimeDelta);
		m_bIsExplosion = false;
	}
}


CRussianHat * CRussianHat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx, _uint uiStageIdx)
{
	CRussianHat*	pInstance = new CRussianHat(pGraphicDev, wstrName, uiIdx, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CRussianHat * CRussianHat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx)
{
	CRussianHat*	pInstance = new CRussianHat(pGraphicDev, wstrName, uiIdx, tInfo, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}
void CRussianHat::Free(void)
{
	Engine::CGameObject::Free();
}
