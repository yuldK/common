set(target test)
include_directories(${ROOT})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

make_executable(${target} "CONSOLE")

# add_pch(${target} pch.cpp pch.h) 

add_sources(${target}
	"bit operator"
	"bit_op.test.cpp"
	"${ROOT}/code/bit_op.h"
)

add_sources(${target}
	"group"
	"group.test.cpp"
	"${ROOT}/code/group.h"
)

add_sources(${target}
	"test main"
)

#add_and_link_dependency(${target} )

target_link_libraries(${target}
	PUBLIC
		GTest::gtest
		GTest::gtest_main
		${WINLIBS}
)
