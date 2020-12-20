#pragma once

#include <iberbar/Lua/LuaCppInstantiate.h>


// ��C++����ѹ��luaջ
#define lua_pushcppobject( L, n, o ) iberbar::LuaCpp::Class_Default_PushObjectWithName( L, n, o );

// ��C++ CRef����ѹ��luaջ
#define lua_pushcppref( L, n, o ) iberbar::LuaCpp::Class_Ref_PushObjectWithName( L, n, o );

// ��luaջ��ȡC++����
#define lua_tocppobject( L, arg, type )  iberbar::LuaCpp::ToClassObject<type>( (L), (arg) )

#define lua_tocppobject_safe( L, arg, type, clsname )  iberbar::LuaCpp::ToClassObject<type>( (L), (arg), (clsname) )

// ��luaջ��selfת��ΪC++����
#define lua_self_tocppobject( L, type ) iberbar::LuaCpp::ToClassObject<type>( L, 1 );