#ifndef HitEffect_h__
#define HitEffect_h__

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

class CHitEffect : public CGameEffect
{
private:
	explicit CHitEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec2 vScale, _vec3 vPos, _float fMaxScale, _float fSpeed, _uint uiRenderOpt);
	virtual ~CHitEffect(void);

public:
	virtual HRESULT		Ready_GameObject(void) override;
	virtual HRESULT		LateReady_GameObject(void);

	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		Render_GameObject(void) override;

private:
	virtual HRESULT		Add_Component(void)override;
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect)override;
public:
	void				Splash_Scale(_float fTimeDelta, _float fMaxScale);
	void				Set_Distortion();
	void				VerticalMove(_float fTimeDelta);
	virtual void		Set_Enable(bool bEnable)override;
	void				Set_Enable(bool bEnable,_vec3 vAddPos);


private:
	_bool		m_bIsDistortion = false;
public:

	static CHitEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec2 vScale, _vec3 vPos, _float fMaxScale=1.f, _float fSpeed=1.f, _uint uiRenderOpt=0);

private:
	_float		m_fSplashScale = 0.f;
	_vec3		m_vOldScale = { INIT_VEC3 };
	_float		m_fMaxScale = 0.f;
	_float		m_fSinTime = 0.f;
	_float		m_fSCTimeSpeed = 1.f;
	_uint		m_uiRenderOpt;
private:
	virtual void Free(void) override;
};

#endif // HitEffect_h__
