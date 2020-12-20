
#include <iberbar/Base/Lua/LuaStateRef.h>
#include <iberbar/Base/Lua/LuaTypes.h>

#include <list>



iberbar::PLuaAPIError iberbar::gLuaAPIError = iberbar::LuaAPIErrorDefault;
void iberbar::LuaAPIErrorDefault( const char* sfile, int sline, const char* err )
{
}



iberbar::CLuaCppClass::CLuaCppClass( void )
	: CUnknown()
	, m_pLuaState( NULL )
	, m_strClassName( "" )
	, m_bHasDeclareStarted( 0 )
{
}


void iberbar::CLuaCppClass::declareStart( lua_State* pLuaState, const char* strClassName )
{
	assert( pLuaState );
	assert( strClassName );
	m_pLuaState = pLuaState;
	m_strClassName = strClassName;

	assert( m_bHasDeclareStarted == 0 );
	m_bHasDeclareStarted = 1;

	lua_State* L = m_pLuaState;

	// ����method table������
	lua_newtable( L );
	m_methods = lua_gettop( L );

	// ����metaԪ��
	// ˵����luaL_newmetatable��������һ���±�(�˱�������metatable), ���±��
	// ��ջ�����������registry��T::className����ϵ��
	luaL_newmetatable( L, m_strClassName.c_str() );
	m_metatable = lua_gettop( L );

	// store method table in globals so that
	// scripts can add functions written in Lua.
	// ��method table����ѹ��ջ����Ȼ�����õ�ȫ�������У�����ΪT::className
	lua_pushvalue( L, m_methods );

	// set�����Ĺ��ܣ����set(A,B,C),
	// ��ô������ǰջ����Ԫ����Ϊֵ,C��Ϊkey����ӵ�����ΪB�ı���.
	// ��BΪ����ʱ��-1��-2����ֵ��keyռ�ã�-3��ѹ��ֵ֮ǰ��Ԫ��......
	// �������д����������LUA_GLOBALSINDEX�������T::classNameΪkey��ֵ��
	//                   ֵ�����Ǹ�ջ��Ԫ�ء�
	// =========����set��ջ�ص�ѹ��ֵ֮ǰ��״̬����Ϊlua_settable����ֵ��key��======
	//set( L, LUA_GLOBALSINDEX, m_strClassName.c_str() );
	lua_setglobal( L, m_strClassName.c_str() );

	// hide metatable from Lua getmetatable()
	// ����meta table��__metatable��
	lua_pushvalue( L, m_methods );
	set( L, m_metatable, "__metatable" );

	// ����meta table��__index��
	// ˵����__index�������һ��������Ҳ������һ����
	// ������һ��������ʱ��Lua��table��ȱ�ٵ�����Ϊ
	// �����������������������һ�����ʱ��Lua������
	// �����п��Ƿ���ȱ�ٵ���
	lua_pushvalue( L, m_methods );
	set( L, m_metatable, "__index" );

	lua_pushstring( L, m_strClassName.c_str() );
	set( L, m_methods, "classname" );         // add new_T to method table
}


void iberbar::CLuaCppClass::declareEnd()
{
	assert( m_bHasDeclareStarted == 1 );
	m_bHasDeclareStarted = 0;

	lua_State* L = m_pLuaState;

	lua_pop( L, 2 );  // drop metatable and method table
}


void iberbar::CLuaCppClass::setFunctionConstruct( lua_CFunction func )
{
	assert( m_bHasDeclareStarted == 1 );

	lua_State* L = m_pLuaState;

	lua_newtable( L );                // mt for method table
	lua_pushcfunction( L, func );
	lua_pushvalue( L, -1 );           // dup new_T function
	set( L, m_methods, "new" );         // add new_T to method table
	set( L, -3, "__call" );           // mt.__call = new_T
	lua_setmetatable( L, m_methods );
}


void iberbar::CLuaCppClass::setFunctionImportant( const char* name, lua_CFunction func )
{
	assert( m_bHasDeclareStarted == 1 );

	lua_State* L = m_pLuaState;

	lua_pushcfunction( L, func );
	set( L, m_metatable, name );
}


void iberbar::CLuaCppClass::setFunction( const luaL_Reg* regs )
{
	assert( m_bHasDeclareStarted == 1 );

	lua_State* L = m_pLuaState;

	// fill method table with methods from class T
	for ( const luaL_Reg *l = regs; l->name; l++ )
	{
		lua_pushstring( L, l->name );

		// Pushes a light userdata onto the stack. 
		// Userdata represent C values in Lua. 
		// A light userdata represents a pointer. 
		// It is a value (like a number): 
		// you do not create it, it has no 
		// individual metatable, and it is 
		// not collected (as it was never created). 
		// A light userdata is equal to "any" 
		// light userdata with the same C address. 
		lua_pushlightuserdata( L, (void*)l );
		//lua_pushcfunction( L, l->func );

		// ����c����thunk������1��upvalue������ջ�ϵ������upvalue
		// �˴���upvalue��lua_pushlightuserdata(L, (void*)l)ѹ���l
		lua_pushcclosure( L, l->func, 1 );
		//LuaSet( L, metatable, l->name );

		lua_settable( L, m_methods );
	}
}


void iberbar::CLuaCppClass::set( lua_State *L, int table_index, const char *key )
{
	lua_pushstring( L, key );
	lua_insert( L, -2 );  // swap value and key
	lua_settable( L, table_index );
}


int iberbar::CLuaCppClass::PushInstance( lua_State* L, const char* strClassName, void *obj, bool gc, bool* pbHasUserdata )
{

	if ( !obj )
	{
		lua_pushnil( L ); return 0;
	}
	luaL_getmetatable( L, strClassName );  // lookup metatable in Lua registry
	if ( lua_isnil( L, -1 ) )
	{
		luaL_error( L, "%s missing metatable", strClassName );
	}
	int mt = lua_gettop( L );

	// subtable������������ϲ���userdata��ֵ
	// �������userdata�ֶ��򷵻ظ��ֶε�ֵ����һ����
	// �����û��userdata�ֶ����½�һ������������½���
	// ��meta���__mode="v"��
	subtable( L, mt, "userdata", "v" );


	// pushuserdata��������ջ������userdata
	// ���userdata�����Ǹ���key���ҵ��ģ�Ҳ
	// �������´����ġ�
	LuaCppClassUsertype* lc_userdata =
		(LuaCppClassUsertype*)( push_userdata( L, obj, sizeof( LuaCppClassUsertype ) ) );
	if ( lc_userdata )
	{
		lc_userdata->data = obj;  // store pointer to object in userdata
		lua_pushvalue( L, mt );
		lua_setmetatable( L, -2 );
		if ( gc == false )
		{
			lua_checkstack( L, 3 );
			subtable( L, mt, "do not trash", "k" );
			lua_pushvalue( L, -2 );
			lua_pushboolean( L, 1 );
			lua_settable( L, -3 );
			lua_pop( L, 1 );
		}
	}
	if ( pbHasUserdata )
	{
		if ( lc_userdata )
			*pbHasUserdata = true;
		else
			*pbHasUserdata = false;
	}
	lua_replace( L, mt );
	lua_settop( L, mt );
	return mt;  // index of userdata containing pointer to T object
}


void* iberbar::CLuaCppClass::push_userdata( lua_State *L, void *key, size_t sz )
{
	void *ud = 0;
	lua_pushlightuserdata( L, key );
	lua_gettable( L, -2 );     // lookup[key]
	if ( lua_isnil( L, -1 ) )
	{
		lua_pop( L, 1 );         // drop nil
		lua_checkstack( L, 3 );
		ud = lua_newuserdata( L, sz );  // create new userdata
		lua_pushlightuserdata( L, key );
		lua_pushvalue( L, -2 );  // dup userdata

		lua_settable( L, -4 );   // lookup[key] = userdata
	}
	return ud;

}


void iberbar::CLuaCppClass::subtable( lua_State *L, int tindex, const char *name, const char *mode )
{
	// ��ָ���ı��ϲ���name��ֵ
	// ���û�ж�Ӧֵ�����½�һ���������������Ϊ�����meta��
	// �������ջ��
	lua_pushstring( L, name );
	lua_gettable( L, tindex );
	if ( lua_isnil( L, -1 ) )
	{
		lua_pop( L, 1 );
		lua_checkstack( L, 3 );
		lua_newtable( L );
		lua_pushvalue( L, -1 );  // table is its own metatable
		lua_setmetatable( L, -2 );
		lua_pushliteral( L, "__mode" );
		lua_pushstring( L, mode );
		lua_settable( L, -3 );   // metatable.__mode = mode
		lua_pushstring( L, name );
		lua_pushvalue( L, -2 );
		lua_settable( L, tindex );
	}
}





iberbar::CLuaUnknownEventDelegateBinder::CLuaUnknownEventDelegateBinder( PTR_CLuaStateRef ptrLua, PTR_CLuaCppClass ptrCppClass, const char* pzLuaFunction )
	: CUnknownObject()
	, m_ptrLuaRef( ptrLua )
	, m_ptrCppClass( ptrCppClass )
	, m_pzLuaFunction( pzLuaFunction )
{
}


void iberbar::CLuaUnknownEventDelegateBinder::execute( PTR_CUnknownObject pEventSender, const UnknownEventParams& refEventParams )
{
	lua_State* lc_pLuaState = m_ptrLuaRef->get();

	int a = 0;
	lua_getglobal( lc_pLuaState, m_pzLuaFunction.c_str() );
	// ��ʱ������CUnknown*����
	//m_ptrCppClass->PushInstance( lc_pLuaState, m_ptrCppClass->getClassName(), pEventSender, true ); // 1.object
	lua_pushcppunknown( lc_pLuaState, "CUnknownObject", (CUnknownObject*)pEventSender, true );
	lua_pushinteger( lc_pLuaState, refEventParams.nEventId );       // 2. eventid ( integer )
	lua_pushinteger( lc_pLuaState, refEventParams.wParam );         // 3. arg1 ( integer )
	lua_pushinteger( lc_pLuaState, refEventParams.lParam );         // 4. arg2 ( integer )
	lua_pushstring( lc_pLuaState, refEventParams.cParam.c_str() );  // 5. arg3 ( string )
	lua_pcall( lc_pLuaState, 5, 0, 0 );
}






iberbar::CLuaUnknownEventDelegateBinderArray::~CLuaUnknownEventDelegateBinderArray()
{
	if ( m_ArrayBinders.empty() == false )
	{
		std::vector< CLuaUnknownEventDelegateBinder* >::iterator lc_iter = m_ArrayBinders.begin();
		std::vector< CLuaUnknownEventDelegateBinder* >::iterator lc_end  = m_ArrayBinders.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
			UNKNOWN_SAFE_RELEASE_NULL( *lc_iter );
	}
}




//----------------------------------------------------------------------------------------------------------------
iberbar::CLuaCFunctionHelper::CLuaCFunctionHelper( lua_State* pLuaState, const char* pzFunctionName, int nReturns )
	: m_pLuaState( nullptr )
	, m_pzFunctionName( pzFunctionName )
	, m_nReturns( nReturns )
	, m_nReturnsPushed( 0 )
{
	assert( m_pLuaState );
	assert( m_pzFunctionName );
	assert( m_nReturns >= 0 );
}


//----------------------------------------------------------------------------------------------------------------
iberbar::CLuaCFunctionHelper::~CLuaCFunctionHelper()
{
	//GetLogFile()->logva(
	//	PRIORITY_ERROR,
	//	"CLuaCFunctionHelper",
	//	"In cfunction <%s>, it expects to return %d params, but it has just returned %d. Are you forget to push some params to return to lua?",
	//	m_pzFunctionName,
	//	m_nReturns,
	//	m_nReturnsPushed );
	assert( m_nReturnsPushed == m_nReturns );
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgBoolean( int nArg )
{
	bool lc_ret = lua_isboolean( m_pLuaState, nArg );
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not boolean.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgNumber( int nArg )
{
	bool lc_ret = ( lua_isnumber( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not number.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgInteger( int nArg )
{
	bool lc_ret = ( lua_isnumber( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not integer.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgString( int nArg )
{
	bool lc_ret = ( lua_isstring( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not string.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}


//----------------------------------------------------------------------------------------------------------------
bool iberbar::CLuaCFunctionHelper::isArgUserdata( int nArg )
{
	bool lc_ret = ( lua_isuserdata( m_pLuaState, nArg ) == 0 ) ? false : true;
	if ( lc_ret == false )
	{
		//GetLogFile()->logva(
		//	PRIORITY_ERROR,
		//	"CLuaCFunctionHelper",
		//	"In cfunction <%s>, the type of number%d argument is not userdata.",
		//	m_pzFunctionName,
		//	nArg );
	}
	return lc_ret;
}








//----------------------------------------------------------------------------------------------------------------
void iberbar::LuaSetConstVariable_Integer( PTR_CLuaStateRef pLuaRef, const char* strName, lua_Integer value )
{
	lua_State* lc_pLuaState = pLuaRef->get();
	lua_Integer* lc_pointer = (lua_Integer*)lua_newuserdata( lc_pLuaState, sizeof( lua_Integer ) );
	*lc_pointer = value;
	//lua_pushvalue( lc_pLuaState, -1 );
	lua_setglobal( lc_pLuaState, strName );
}








