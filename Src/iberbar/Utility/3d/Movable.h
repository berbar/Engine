//*=============================================================================
//*
//*  Name: b3dassist header
//*  Dest: The main writing some auxiliary class and the auxiliary structure,
//*        and contains an abstract base class
//*
//*  Edited at 2014.9 -- it's old version
//*  Need to improve it.
//*=============================================================================

#pragma once

#include <d3dx9math.h>



#define null NULL
#define BFLT_MAX FLT_MAX
#define BFLT_MIN 0.001f
#define STATIC_CONST  static const

#define BSQRT_2 1.4142f
#define BSQRT_3 1.7320f



class CMovable3d;








//*=======================================================================
//|
//|                   3ά�����任�������
//|
//*=======================================================================

#define B3D_MBOBJ_TRANS_MOVE   0
#define B3D_MBOBJ_TRANS_RM     1
#define B3D_MBOBJ_TRANS_RV     2
#define B3D_MBOBJ_TRANS_RB     3
#define B3D_MBOBJ_TRANS_TUM    4
#define B3D_MBOBJ_TRANS_TUV    5
#define B3D_MBOBJ_TRANS_TUB    6
#define B3D_MBOBJ_TRANS_TRM    7
#define B3D_MBOBJ_TRANS_TRV    8
#define B3D_MBOBJ_TRANS_TRB    9

class CMovable3d
{

public:
	CMovable3d( void );


public:
	void SetDirectionBoth( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp );
	void SetDirectionMove( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp );
	void SetDirectionView( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp );
	void SetDirectionMoveSameAsView();
	void SetDirectionViewSameAsMove();
	void SetPos( const D3DXVECTOR3& pos );

	void GetPos( D3DXVECTOR3* pOutPos )    const   { *pOutPos = m_ptPosition; }
	void GetDirectionMove( D3DXVECTOR3* pFace, D3DXVECTOR3* pLeft, D3DXVECTOR3* pUp ) const;
	void GetDirectionView( D3DXVECTOR3* pFace, D3DXVECTOR3* pLeft, D3DXVECTOR3* pUp ) const;

	// ֻ�����ƶ��������任λ��
	void TranslateOnXYZ( float fx, float fy, float fz );
	void MoveLeft( float fDistance );
	void MoveFront( float fDistance );
	void MoveUp( float fDistance );

	// ����ǰ������ת
	void RollBoth( float fAngle );
	void RollMove( float fAngle );
	void RollView( float fAngle );

	// ������������ת
	void TurnUpBoth( float fAngle );
	void TurnUpMove( float fAngle );
	void TurnUpView( float fAngle );

	// ������������ת
	void TurnRightBoth( float fAngle );
	void TurnRightMove( float fAngle );
	void TurnRightView( float fAngle );
	void TurnRightViewKeepElevation( float fAngle ); //�ƶ����������任�����Ӿ����������ƶ�����Up��ת

	void RotationXYZOnItsPos( float fAngleX, float fAngleY, float fAngleZ );
	void RotationAxis( const D3DXVECTOR3& pt1, const D3DXVECTOR3& pt2 );


protected:
	D3DXVECTOR3  m_ptPosition;   // ����λ��
	D3DXVECTOR3  m_vecMoveFace;  // �ƶ�ǰ����
	D3DXVECTOR3  m_vecMoveUp;    // �ƶ�������
	D3DXVECTOR3  m_vecViewFace;  // �Ӿ�ǰ����
	D3DXVECTOR3  m_vecViewUp;    // �Ӿ�������
};






