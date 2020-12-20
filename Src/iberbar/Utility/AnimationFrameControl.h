

#ifndef __iberbar__Base__AnimationFrameControl_h__
#define __iberbar__Base__AnimationFrameControl_h__

#include <iberbar\Base\Unknown.h>


namespace iberbar
{
	
	class IBERBAR_DLL CAnimationFrameController
		: public CUnknown
	{
	public:
		enum _FrameSet
		{
			FrameSet_Zero,       // ��0��ʼƫ�ƣ�����ʵ��ֵ
			FrameSet_Begin,      // �ӵ�һ֡��ʼƫ��
			FrameSet_Current,    // �ӵ�ǰ֡��ʼƫ��
			FrameSet_End,        // �����һ֡��ʼƫ�ƣ���������ǰƫ��
		};

		IBERBAR_UNKNOWN_CLONE_DISABLED( CAnimationFrameController );

	public:
		CAnimationFrameController( void );


	public:
		void   setFrameLength( int nBegin, int nEnd );
		void   setFrameCur( int nOffset, int nType = FrameSet_Zero ); // ���õ�ǰ֡��λ��
		void   setRepeatTimesMax( int nTimesMax ); // ��ϴ�����1���൱����0�������䵽1������2�ε����ȴ�0��1�����ٴ�1���䵽0���Դ����ƣ�������С�ڵ���0ʱΪ����ѭ��
		void   setRepeatTimesMaxInfinite()                { setRepeatTimesMax( 0 ); }
		void   setTimePerFrame( float fTime );
		void   start();
		void   stop();
		void   resume();
		void   pause();
		void   update( float nElapsedTime ); //ͨ��ʱ�������µ�ǰ֡


	public:
		int    getFrameBegin() const          { return m_nFrameBegin; }
		int    getFrameEnd() const            { return m_nFrameEnd; }
		int    getFrameCur() const            { return m_nFrameCur; }
		bool   getIsRepeat() const            { return ( m_nRepeatTimesMax <= 0 ) ? true : false; }
		int    getRepeatTimesMax() const      { return m_nRepeatTimesMax; }
		int    getRepeatTimesAlready() const  { return m_nRepeatTimesAlready; }
		float  getTimePerFrame() const        { return m_fTimePerFrame; }
		bool   getHasStarted() const          { return m_bHasStarted; }
		bool   getIsPaused() const            { return m_bPaused; }


	private:
		int    m_nFrameCur;
		int    m_nFrameBegin;
		int    m_nFrameEnd;    
		int    m_nRepeatTimesMax; // �Ƿ�ѭ������
		int    m_nRepeatTimesAlready;
		float  m_fTimePerFrame; //һ֡����ʱ��
		float  m_fElapsedTime;
		bool   m_bHasStarted;
		bool   m_bPaused;
	};

	IBERBAR_UNKNOWN_PTR_DECLARE( CAnimationFrameController );
}

#endif