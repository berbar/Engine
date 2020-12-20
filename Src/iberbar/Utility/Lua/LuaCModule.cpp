

#include <iberbar/Base/Lua/LuaCModule.h>


namespace iberbar
{
	bool LuaCModuleParseNamespace( LuaCModuleNamespaceList& namespace_list, const std::string& name );
}

bool iberbar::LuaCModuleParseNamespace( LuaCModuleNamespaceList& namespace_list, const std::string& name )
{
	size_t nstart = 0;
	size_t nend = 0;
	while ( nend != std::string::npos )
	{
		nend = name.find( '.', nstart );
		if ( nend == std::string::npos )
		{
			if ( name.c_str()[ nstart ] == 0 )
				return true;

			namespace_list.push_back( name.substr( nstart ) );
		}
		else
		{
			if ( nend <= nstart )
				return false;

			namespace_list.push_back( name.substr( nstart, nend - nstart ) );
		}
			
		nstart = nend + 1;
	}
	return true;
}



iberbar::Result iberbar::LuaCModuleRegister( lua_State* pLuaState, const char* name, CLuaCModuleAbstract* mod )
{
	assert( pLuaState );
	assert( mod );

	Result result;

	// ע�᲻�������ռ��ȫ�ֺ���
	if ( name == nullptr )
	{
		mod->onRegister( pLuaState, -1 );
		return result;
	}

	LuaCModuleNamespaceList name_list;
	if ( name[ 0 ] == 0 || LuaCModuleParseNamespace( name_list, name ) == false )
	{
		result.code = ResultCode::Bad;
		result.format( "invaild name of \"%s\".", name );
		return result;
	}

	int tb = 0;
	int tb_prev = 0;
	int tb_count = 0;
	int top_count = 0;
	int top_old = lua_gettop( pLuaState );
	LuaCModuleNamespaceList::iterator iter = name_list.begin();
	LuaCModuleNamespaceList::iterator end = name_list.end();
	const char* iter_cstr = nullptr;

	iter_cstr = iter->c_str();

	// ��ȡȫ��table
	lua_getglobal( pLuaState, iter_cstr );
	tb = lua_gettop( pLuaState );
	top_count ++;
	if ( lua_isnil( pLuaState, tb ) )
	{
		// ȫ��table�����ڣ�����table
		lua_createtable( pLuaState, 0, 0 );
		tb = lua_gettop( pLuaState );
		top_count ++;
		lua_pushvalue( pLuaState, tb );
		lua_setglobal( pLuaState, iter_cstr );
		tb_count ++;

	}
	else if ( lua_istable( pLuaState, tb ) )
	{
		// ����ȫ��table
		tb_count ++;
	}
	else
	{
		// ����table����
		result.format( "Confict" );
	}
	++ iter;

	// ��һ��namespace��ȡ��Чʱ��ֱ�ӷ��ش���
	if ( tb_count == 0 )
	{
		lua_pop( pLuaState, top_count );
		return result;
	}

	// ѭ��Ϊ��һ��namespace���Ӵμ�namespace
	for ( ; iter != end; ++ iter )
	{
		iter_cstr = iter->c_str();

		lua_pushstring( pLuaState, iter_cstr );
		lua_gettable( pLuaState, -2 );
		tb_prev = tb;
		tb = lua_gettop( pLuaState );
		top_count ++;
		if ( lua_isnil( pLuaState, tb ) )
		{
			lua_createtable( pLuaState, 0, 0 );
			tb = lua_gettop( pLuaState );
			top_count ++;
			lua_pushvalue( pLuaState, tb );
			lua_pushstring( pLuaState, iter_cstr );
			lua_insert( pLuaState, -2 );  // swap value and key
			lua_settable( pLuaState, tb_prev );
			tb_count ++;
		}
		else if ( lua_istable( pLuaState, tb ) )
		{
			tb_count ++;
		}
		else
		{
			break;
		}
	}

	// ���е������ռ䶼��Чʱ��ע��
	if ( tb_count == (int)name_list.size() )
	{
		mod->onRegister( pLuaState, tb );
	}
	else
	{
		result.code = ResultCode::Bad;
		result.data = "Can't push namespaces into lua.";
	}
	int top_new = lua_gettop( pLuaState );
	lua_pop( pLuaState, top_count );

	return result;
}


