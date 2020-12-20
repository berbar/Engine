

//////////////////////////////////////////////////////////////////////////
//
// iberbar\Base\UnknownSingleton.h
//
// ��ͷ�ļ��е�����ֻ���ں� _DEBUG ����ʱ����Ч����Debugģʽ�£�
//////////////////////////////////////////////////////////////////////////
#ifndef __iberbar__Base__UnknownSingleton_h__
#define __iberbar__Base__UnknownSingleton_h__

#include "iberbar\Base\Unknown.h"

#ifdef _DEBUG



namespace iberbar
{
#ifndef IBERBAR_DEBUG_METHOD
#define IBERBAR_DEBUG_METHOD( func )       Debug_##func
#endif

	// ��Unknown��������������ӵ���������
	// �� IBERBAR_DECLARE_SINGLETON_CLASS �Ѿ��Զ���ӵ���������
	void Debug_AddUnknownSingleton( CUnknown* singleton );

	// ��Unknown�������ͷ�ʱ�ӹ��������Ƴ���
	// �� IBERBAR_DECLARE_SINGLETON_CLASS �Ѿ��Զ��ӹ��������Ƴ�
	void Debug_RemoveUnknownSingleton( CUnknown* singleton );

	// �ڳ������ǰ���øú������Unknown�����Ƿ��ͷŵ�
	void Debug_CheckUnknownSingleton();






}

#else

#ifndef IBERBAR_DEBUG_METHOD
#define IBERBAR_DEBUG_METHOD( func )
#endif

#endif


#define IBERBAR_DECLARE_SINGLETON_INSTANCE( type )         static PTR_##type sm_pShared
#define IBERBAR_DECLARE_SINGLETON_INSTANCE_GET( type )     static PTR_##type GetShared()        { assert( sm_pShared ); return sm_pShared; }
#define IBERBAR_DECLARE_SINGLETON_INSTANCE_CREATE( type )  static PTR_##type CreateShared()     { assert( sm_pShared == NULL ); sm_pShared = PTR_##type::_sNew(); IBERBAR_DEBUG_METHOD( AddUnknownSingleton )( sm_pShared ); return sm_pShared; }
#define IBERBAR_DECLARE_SINGLETON_INSTANCE_DELETE( type )  static void       DeleteShared()     { IBERBAR_DEBUG_METHOD( RemoveUnknownSingleton )( sm_pShared ); sm_pShared = NULL; }
#define IBERBAR_DECLARE_SINGLETON_CLASS( type )  \
	private:\
		IBERBAR_DECLARE_SINGLETON_INSTANCE( type ); \
	public: \
		IBERBAR_DECLARE_SINGLETON_INSTANCE_CREATE( type ); \
		IBERBAR_DECLARE_SINGLETON_INSTANCE_GET( type ) \
		IBERBAR_DECLARE_SINGLETON_INSTANCE_DELETE( type )

#endif