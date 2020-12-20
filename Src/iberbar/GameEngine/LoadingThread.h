#pragma once

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Ref.h>
#include <functional>
#include <thread>
#include <mutex>
#include <xmemory>
#include <memory_resource>



#ifndef iberbar_OperatorNewAndDelete
#define iberbar_OperatorNewAndDelete \
	void* operator new( size_t n ); \
	void operator delete(void*); \
	void* operator new[]( size_t n ); \
	void operator delete[](void*);
#endif


namespace iberbar
{
	namespace Game
	{

		class CLoadingTask;
		class CLoadingStepResult;
		class CLoadingThread;


		enum class ULoadingTaskState
		{
			Abort = -2, // ��ֹ�������������
			Complete = 0, // ������񣬿����гɹ��ĺ�ʧ�ܵģ�������ֹ���������
			Continue = 1 // ������δ��ɣ���������Executeִ������ֱ����ɻ���ֹ
		};


		// �����������
		class __iberbarGameEngineApi__ CLoadingTask abstract
			: public CRef
		{
		public:
			CLoadingTask()
				: m_strTaskName()
			{
			}
			virtual ~CLoadingTask() {}

		public:
			void SetStepName( const std::string& strTaskName ) { m_strTaskName = strTaskName; }
			// ������ڣ�����ִ�иò��裬ֱ������0
			// ��Loading���߳�ִ��
			virtual int Execute( CLoadingThread* pThread ) = 0;

		protected:
			std::string m_strTaskName;
		};




		// ����������ִ�н������
		class __iberbarGameEngineApi__ CLoadingStepResult abstract
		{
		public:
			virtual ~CLoadingStepResult() {}
			// Execute�����������߳�ִ��
			virtual void Execute() = 0;
			void* operator new[]( size_t n ) = delete;
			void operator delete[]( void* ) = delete;
		};




		// �����̵߳��������
		class CLoadingTaskQueue
		{
		public:
			CLoadingTaskQueue();
			~CLoadingTaskQueue();

		public:
			bool TryLock() { return m_Mutex.try_lock(); }
			void Lock() { m_Mutex.lock(); }
			void Unlock() { m_Mutex.unlock(); }
			void AddStep( CLoadingTask* pStep ) { m_Steps.push_back( pStep ); }
			CLoadingTask* PopStep()
			{
				if ( m_Steps.empty() )
					return nullptr;
				CLoadingTask* pStep = *(m_Steps.begin());
				m_Steps.pop_front();
				return pStep;
			}

		private:
			std::mutex m_Mutex;
			std::list<CLoadingTask*> m_Steps;
		};




		// �����̵߳����������
		class __iberbarGameEngineApi__ CLoadingThread
		{
		private:
			enum class UEventIdForThread
			{
				Shutdown = 0,
				Wakeup,
				__Count
			};

		public:
			struct UMsg
			{
			public:
				int nEvent;
				int nValue;
				std::string strText;
			};

		public:
			CLoadingThread();
			~CLoadingThread();

		public:
			void Initial();
			void Destroy();
			void Lock() { m_Mutex.lock(); }
			bool TryLock() { return m_Mutex.try_lock(); }
			void Unlock() { return m_Mutex.unlock(); }
			void AddTask( CLoadingTask* Step );
			void Wakeup();
			bool IsWakeup() { return !m_bIsWaiting; }
			
			// �����̵߳�task�ڲ�����
			void AddStepResult( CLoadingStepResult* pStepResult );
			// ���̵߳���
			void LoopStepResults();

		private:
			std::thread m_Thread;
			std::mutex m_Mutex;
			bool m_bRunThread;
			bool m_bIsWaiting;
			bool m_bStop;
#ifdef _WINDOWS
			//HANDLE m_hEventWhenYield;
			HANDLE m_Handles[ (int)UEventIdForThread::__Count ];
#endif
			CLoadingTaskQueue m_TaskQueue;
			std::pmr::vector<CLoadingStepResult*> m_StepResultQueue;

		private:
			void OnRunThread();
			void OnYield();
		};




		// �ؼ����񣬱������κμ��ش���ֻ����Ϊ�����б��е�һ���ؼ��ڵ�
		class __iberbarGameEngineApi__ CLoadingTask_KeyTask final
			: public CLoadingTask
		{
		private:
			typedef void _Callback();

			class _Result final
				: public CLoadingStepResult
			{
			public:
				_Result( std::function<_Callback> callback ) : m_callback( callback ) {}
			public:
				virtual void Execute() override;
				void* operator new(size_t n);
				void operator delete(void*);

			private:
				std::function<_Callback> m_callback;
			};

		public:
			CLoadingTask_KeyTask();
			virtual int Execute( CLoadingThread* pThread ) override;
			void SetCallback( std::function<_Callback> callback ) { m_callback = callback; }

		private:
			std::function<_Callback> m_callback;
		};





		// ����������������
		class __iberbarGameEngineApi__ CLoadingTask_Textures
			: public CLoadingTask
		{
		public:
			typedef void _CallbackOnStep( const std::string&, int, int, bool, const std::string& );
			typedef void _CallbackOnFinal();

		private:
			typedef std::pmr::string _ByteString;

			class _StepResult final
				: public CLoadingStepResult
			{
			public:
				_StepResult( const std::string& strName, int nTotal, int nIndex, std::function<_CallbackOnStep> Callback );

			public:
				virtual void Execute() override;
				void* operator new( size_t n );
				void operator delete( void* );

			public:
				void SetFailure( const std::string& strError ) { m_nRet = false; m_strError = strError; }
				void SetSuccess() { m_nRet = true; }
				void SetSuccess( const _ByteString& Bytes ) { m_nRet = true; m_Bytes = Bytes; }

			private:
				int m_nProgressTotal;
				int m_nProgressIndex;
				std::string m_strName;
				bool m_nRet;
				std::string m_strError;
				_ByteString m_Bytes;
				std::function<_CallbackOnStep> m_Callback;
			};

			class _FinalResult final
				: public CLoadingStepResult
			{
			public:
				_FinalResult( std::function<_CallbackOnFinal> Callback );
			public:
				virtual void Execute() override;
				void* operator new(size_t n);
				void operator delete(void*);

			private:
				std::function<_CallbackOnFinal> m_Callback;
			};

		public:
			CLoadingTask_Textures();

			virtual int Execute( CLoadingThread* pThread ) override;

			void SetSleepFor( uint32 nMilliseconds ) { m_nMilliseconds = nMilliseconds; }
			void SetCallbackOnStep( std::function<_CallbackOnStep> Callback ) { m_CallbackOnStep = Callback; }
			void SetCallbackOnFinal( std::function<_CallbackOnFinal> Callback ) { m_CallbackOnFinal = Callback; }
			void AddTexture( const char* strName );

		private:
			uint32 m_nMilliseconds;
			std::vector<std::string> m_TextureList;
			uint32 m_nLoadedCount;
			std::function<_CallbackOnStep> m_CallbackOnStep;
			std::function<_CallbackOnFinal> m_CallbackOnFinal;
		};




		__iberbarGameEngineApi__ std::pmr::memory_resource* LoadingThread_GetMemoryPool();

		template < typename TElem >
		inline std::pmr::polymorphic_allocator<TElem> LoadingThread_GetMemoryAllocator()
		{
			return std::pmr::polymorphic_allocator<TElem>( LoadingThread_GetMemoryPool() );
		}
	}
}

