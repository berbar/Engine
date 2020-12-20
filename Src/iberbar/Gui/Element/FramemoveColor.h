


#ifndef __iberbar__Gui__Element__FramemoveColor_h__
#define __iberbar__Gui__Element__FramemoveColor_h__

#include "iberbar\Base\Color.h"

namespace iberbar
{
	class IBERBAR_DLL FramemoveColor
	{
	public:
		FramemoveColor( void );


	public:
		void startBlend();    // ���¿�ʼ��Ͻ���
		void overBlend();     // ������Ͻ���
		void update( float nElapsedTime );


		void setBlendTimesMax( int nTimesMax ); // ��ϴ�����1���൱����0�������䵽1������2�ε����ȴ�0��1�����ٴ�1���䵽0���Դ����ƣ�������С�ڵ���0ʱΪ����ѭ��
		void setBlendTimesMaxInfinite()                { setBlendTimesMax( 0 ); }
		void setBlendTimeNeed( float nSecondes ); // һ�λ�Ͻ�������Ҫ��ʱ��
		void setBaseColor( const Color4B& color0, const Color4B& color1 );
		
		Color4B getBaseColor0() const                  { return m_ColorBase[ 0 ]; }
		Color4B getBaseColor1() const                  { return m_ColorBase[ 1 ]; }
		Color4F getCurrentColor() const                { return m_ColorCurrent; }
		bool isOverBlend() const                       { return m_bBlendOver; }


	protected:
		int     m_iBlendTimesMax;  // ��ϴ�����1���൱����0�������䵽1������2�ε����ȴ�0��1�����ٴ�1���䵽0���Դ����ƣ�������Ϊ-1ʱΪ����ѭ��
		int     m_iBlendTimesAlready;
		bool    m_bBlendOver;
		float   m_fBlendTimeNeed; // һ�λ�Ͻ�������Ҫ��ʱ��
		float   m_fBlendTimeElapsed;
		Color4F m_ColorCurrent;
		Color4F m_ColorBase[ 2 ];
	};
}

#endif