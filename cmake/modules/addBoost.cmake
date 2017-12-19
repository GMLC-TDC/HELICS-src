IF (MSVC)

set (boost_versions
boost_1_66_0
boost_1_65_1
boost_1_65_0
boost_1_64_0
boost_1_63_0
boost_1_62_0
boost_1_61_0
)

set(poss_prefixes
C:
C:/boost
C:/local
C:/boost
C:/Libraries
D:
D:/boost
D:/local
D:/boost
)

# create an empty list
list(APPEND boost_paths "")

foreach( dir ${poss_prefixes})
	foreach( boostver ${boost_versions})
		IF (IS_DIRECTORY ${dir}/${boostver})
			list(APPEND boost_paths ${dir}/${boostver})
		ENDIF()
	endforeach()
endforeach()


find_path(BOOST_TEST_PATH
			NAMES 			boost/version.hpp
			PATHS		${boost_paths}
		)

		if (BOOST_TEST_PATH)
		set(BOOST_ROOT ${BOOST_TEST_PATH})
		endif(BOOST_TEST_PATH)
ENDIF(MSVC)

SHOW_VARIABLE(BOOST_ROOT PATH "Boost root directory" "${BOOST_ROOT}")

# Minimum version of Boost required for building HELICS
set(BOOST_MINIMUM_VERSION 1.58)
set(Boost_USE_STATIC_LIBS   ${USE_BOOST_STATIC_LIBS})
if (${MPI_C_FOUND})
  #find_package(Boost ${BOOST_MINIMUM_VERSION} COMPONENTS program_options unit_test_framework filesystem mpi system date_time REQUIRED)
  find_package(Boost ${BOOST_MINIMUM_VERSION} COMPONENTS program_options mpi unit_test_framework filesystem system date_time REQUIRED)
ELSE(${MPI_C_FOUND})
  find_package(Boost ${BOOST_MINIMUM_VERSION} COMPONENTS program_options unit_test_framework filesystem system date_time REQUIRED)
ENDIF(${MPI_C_FOUND})

# Minimum version of Boost required for building test suite
if (Boost_VERSION LESS 106100)
  set(BUILD_HELICS_TESTS OFF)
  message(WARNING "Boost version >=1.61 required for building HELICS tests (Found Boost version ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION})")
endif()

mark_as_advanced(CLEAR BOOST_ROOT)

message(STATUS "Using Boost include files : ${Boost_INCLUDE_DIR}")
#message(STATUS "Using Boost libraries in : ${Boost_LIBRARY_DIRS}")
#message(STATUS "Using Boost libraries : ${Boost_LIBRARIES}")
set(modifier,"")
foreach(loop_var ${Boost_LIBRARIES})
	if (${loop_var} MATCHES "debug")
		list(INSERT modifier 0 ${loop_var})
	elseif(${loop_var} MATCHES "optimized")
		list(INSERT modifier 0 ${loop_var})
	else()
		#message("Boost_LIBRARIES ${loop_var}")
		if(${loop_var} MATCHES "unit_test")
			list(APPEND Boost_LIBRARIES_test ${modifier} ${loop_var})
		else()
			list(APPEND Boost_LIBRARIES_core ${modifier} ${loop_var})
		endif()
		list(LENGTH modifier modifier_size)
		if (modifier_size GREATER 0)
		list(REMOVE_AT modifier -1)
		endif()
	endif()
endforeach(loop_var)

#message(STATUS "Using Boost core libraries : ${Boost_LIBRARIES_core}")
#message(STATUS "Using Boost test libraries : ${Boost_LIBRARIES_test}")

