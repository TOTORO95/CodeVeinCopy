#ifndef FireBreathEffect_h__
#define FireBreathEffect_h__

#include "Defines.h"
#include "GameObject.h"
#include "GameEffect.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END

class CFireBreathEffect : public CGameEffect
{
private:
	explicit CFireBreathEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos, _vec3 vDir,_bool bIsDistortion,_float fMaxScale);
	virtual ~CFireBreathEffect(void);

public:
	virtual HRESULT		Ready_GameObject(void) override;
	virtual HRESULT		LateReady_GameObject(void);

	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		Render_GameObject(void) override;

private:
	virtual HRESULT		Add_Component(void)override;
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect)override;
public:
	void				Set_ChargeSpeed(_float fSpeed);
	void				Charge_Scale(_float fTimeDelta, _float fMaxScale);
	void				Set_Distortion();
	void				VerticalMove(_float fTimeDelta);
	virtual void		Set_Enable(bool bEnable)override;
	void				Set_Enable(bool bEnable,_vec3 vAddPos);


private:
	_bool		m_bIsDistortion = false;
public:

	static CFireBreathEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
												wstring wstrTexName,
												wstring wstrParentInstName,
												string strBoneName,
												_vec2 vScale = { 1.f,1.f },
												_vec3 vPos = { INIT_VEC3}, 
												 _vec3 vDir={INIT_VEC3},
												_bool bIsDistortion = false,
												_float fMaxScale=2.5f);

private:
	_float		m_fSplashScale = 0.f;
	_vec3		m_vOldScale = { INIT_VEC3 };
	_float		m_fMaxScale = 0.f;
	_bool		m_bIsFinish = false;
	_float		m_fEndTime = 0.f;
	_float		m_fSCTimeSpeed = 1.f;
	_bool		m_bIsStart = false;
	_float		m_EndScale = 1.f;
	_vec3		m_vDir = { INIT_VEC3 };
private:
	virtual void Free(void) override;
};

#endif // FireEffect_h__
