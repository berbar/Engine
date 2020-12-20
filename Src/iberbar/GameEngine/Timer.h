

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Unknown.h>
#include <functional>


namespace iberbar
{

	namespace Game
	{
		class CTimer;
		class CTimerEasySystem;

		typedef void UFunction_TimerExecute( CTimer* pTimer );

		class __iberbarGameEngineApi__ CTimer
			: public CRef
		{
		public:
			CTimer();
			~CTimer();

		public:
			// ��ʱֻ֧�ֵ����ص��ӿ�
			bool Start( float fTimeout, bool bLoop );

			// ֹͣtimer����timer����Ϊdisable��ͬʱ����onDestroy
			// ���������timer��Ҫ��stop������start������disable��timer���ᱻ��ϵͳ�����
			void Stop();

			// ֻ����ͣtimer������ʹ��resume���ָ�
			void Pause();

			// �ظ�timer������
			void Resume();

			// ����ʱ��ά�ȣ���ά�Ȳ���ֻ�����������ʱ���ߣ����Ǿ��Ե�ά������
			void Scale( float nScaleParams );

			// ����ִ�лص�����
			void SetCallbackExecute( std::function<UFunction_TimerExecute> Callback ) { m_CallbackExecute = Callback; }

			// ִ��
			void Run( float nDelta );

			void SetId( const std::string& strId ) { m_strId = strId; }
			const std::string& GetId() const { return m_strId; }


		public:
			float GetTimeout() { return m_nTimeout; }
			bool  IsEnable() { return m_bEnabled; }
			bool  IsLoop() { return m_bLoop; }
			bool  IsPause() { return m_bPause; }
			float GetScaleParam() { return m_nScale; }



		private:
			bool m_bEnabled;
			bool m_bLoop;
			bool m_bPause;
			bool m_bRemoveSelf;
			float m_nTimeout;
			float m_nCountdown;
			float m_nScale;

			std::string m_strId;
			std::function<UFunction_TimerExecute> m_CallbackExecute;
		};


		class __iberbarGameEngineApi__ CTimerEasySystem
		{
		private:
			struct _TimerNode
			{
				bool bRemove;
				CTimer* pTimer;
			};

		public:
			CTimerEasySystem();
			~CTimerEasySystem();

		public:
			void Run( float nDelta );
			void AddTimer( CTimer* pTimer );
			bool FindTimer( const std::string& strId, CTimer** ppOutTimer );
			void RemoveTimer( CTimer* pTimer );

		private:
			std::list<_TimerNode> m_Timers;
		};
	}
}

