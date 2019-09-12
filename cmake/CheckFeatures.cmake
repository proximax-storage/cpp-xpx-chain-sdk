include(CheckCXXSourceCompiles)

function(check_string_literal_operator_template result)
	set(OLD_CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS})

	if (MSVC)
		set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} /std:c++17")
	else()
		set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -std=c++17")
	endif()

	check_cxx_source_compiles("
		template<char... chars> struct StringLiteral { };
		template<typename T, T... chars> constexpr StringLiteral<chars...> operator\"\"_sl() { return {}; }
		int main(int argc, char* argv[]) { return 0; }"
		${result})

	set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQUIRED_FLAGS})
	
	if (NOT ${result})
		set(${result} 0 PARENT_SCOPE)
	endif()
endfunction(check_string_literal_operator_template)
