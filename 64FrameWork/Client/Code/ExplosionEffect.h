#ifndef ExplosionEffect_h__
#define ExplosionEffect_h__

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

class CExplosionEffect : public CGameEffect
{
private:
	explicit CExplosionEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos, _bool bIsTracking, _float fMaxScale , _bool bIsLoop);
	virtual ~CExplosionEffect(void);

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

	static CExplosionEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale , _vec3 vPos, _bool bIsTracking ,_float fMaxScale=1.f, _bool bIsLoop = false);

private:
	_float		m_fSplashScale = 0.f;
	_vec3		m_vOldScale = { INIT_VEC3 };
	_float		m_fMaxScale = 0.f;
private:
	virtual void Free(void) override;
};

#endif // BoostEffect_h__s
