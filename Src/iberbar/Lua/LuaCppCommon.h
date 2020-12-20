#pragma once

#include <iberbar/Lua/LuaBase.h>

namespace iberbar
{
	namespace LuaCpp
	{
		// Class �������� CFoo
		extern const char __iberbarLuaApi__ uClassReflection_ClassName[];

		// Class ������ģ�������� iberbar.Test
		extern const char __iberbarLuaApi__ uClassReflection_ModuleName[];

		// Class ȫ������ iberbar.Test.CFoo
		extern const char __iberbarLuaApi__ uClassReflection_FullName[];

		// Class ������Դ��1=C++��2=Lua
		extern const char __iberbarLuaApi__ uClassReflection_CType[];

		extern const char __iberbarLuaApi__ uWeakTable_ForUserData[];

		enum class UClassReflectionCType
		{
			Cpp = 1,
			Lua = 2
		};
	}
}


// Class �������� CFoo
#define iberbar_LuaCpp_ClassReflection_ClassName "__cpp_classname"

// Class ������ģ�������� iberbar.Test
#define iberbar_LuaCpp_ClassReflection_ModuleName "__cpp_modulename"

// Class ȫ������ iberbar.Test.CFoo
#define iberbar_LuaCpp_ClassReflection_FullName "__cpp_fullname"

// Class ������Դ��1=C++��2=Lua
#define iberbar_LuaCpp_ClassReflection_CType "__ctype"

