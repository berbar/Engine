#pragma once


#include <iberbar/Gui/Headers.h>
#include <iberbar/Utility/ViewportState.h>
#include <memory_resource>


namespace iberbar
{
	struct UMouseEventData;
	struct UKeyboardEventData;
	class CBaseCommand;
	class CCommandQueue;

	namespace Renderer
	{
		class CRenderer2d;
		class CRendererSprite;
		class CRenderCommand;
	}

	namespace Gui
	{
		class CWidget;
		class CDialog;

		class __iberbarGuiApi__ CEngine final
		{
		public:
			CEngine( Renderer::CRendererSprite* pSprite, CCommandQueue* pCommandQueue );
			CEngine( Renderer::CRendererSprite* pSprite, CCommandQueue* pCommandQueue, std::pmr::memory_resource* pMemoryRes );
			~CEngine();

		public:
			std::pmr::memory_resource* GetMemoryPool() { return m_pMemoryRes; }
			CViewportState* GetViewportState() { return &m_ViewportState; }
			Renderer::CRenderer2d* GetRenderer() { return m_pRenderer; }
			Renderer::CRendererSprite* GetRendererSprite() { return m_pSprite; }

		public:
			void AddDialog( CDialog* pDialog );
			CDialog* GetDialog( const char* strId );
			void RemoveDialog( CDialog* pDialog );

			void RequestFocus( CWidget* pWidget );
			void ClearFocus( bool bSendEvent );
			CWidget* GetFocus() { return m_pWidgetFocus; }
			void RequestTop( CDialog* pDialog );

			void AddCommand( CBaseCommand* pCommand );

			void Update( int64 nElapsedTimeMs, float nElapsedTimeSecond );
			void Render();
			void HandleMouse( const UMouseEventData* EventData );
			void HandleKeyboard( const UKeyboardEventData* pEventData );

		private:
			// ���´��ڵ�ZOrder����
			void UpdateZOrder();

		private:
			Renderer::CRenderer2d* m_pRenderer;
			Renderer::CRendererSprite* m_pSprite;
			CCommandQueue* m_pCommandQueue;
			bool m_bMemoryResAuto;
			std::pmr::memory_resource* m_pMemoryRes;
			CWidget* m_pWidgetFocus;
			std::list<CDialog*> m_Dialogs;
			CViewportState m_ViewportState;
			
			
		public:
			static CEngine* sGetInstance() { return sm_pInstance; }
		private:
			static CEngine* sm_pInstance;
		};

		template < typename TElem >
		std::pmr::polymorphic_allocator<TElem> GetMemoryAllocator()
		{
			return std::pmr::polymorphic_allocator<TElem>( CEngine::sGetInstance()->GetMemoryPool() );
		}

#define iberbarGuiOverride_OperatorNewAndDelete_1( type ) \
	public: \
	void* operator new(size_t n) { return GetMemoryAllocator<type>().allocate( 1 ); } \
	void operator delete(void* p) { GetMemoryAllocator<type>().deallocate( (type*)p, 1 ); } \
	void* operator new[]( size_t n ) = delete; \
	void operator delete[]( void* ) = delete;


	}
}



