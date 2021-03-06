# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Copyright (c) 2017-2021, Battelle Memorial Institute; Lawrence Livermore
# National Security, LLC; Alliance for Sustainable Energy, LLC.
# See the top-level NOTICE for additional details.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set(helicsShared_headers
    helicsCore.h
    api-data.h
    ValueFederate.h
    MessageFederate.h
    MessageFilters.h
    helicsCallbacks.h
)

set(helicsShared_sources
    helicsExport.cpp
    FederateExport.cpp
    ValueFederateExport.cpp
    MessageFederateExport.cpp
    MessageFiltersExport.cpp
    helicsCallbacks.cpp
    internal/api_objects.h
)

include(GenerateExportHeader)

if(PYTHON_EXECUTABLE)
    add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics.h
               ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics_api.h
        COMMAND ${PYTHON_EXECUTABLE} ARGS ${PROJECT_SOURCE_DIR}/scripts/generateHelicsH.py
                ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/ ${PROJECT_SOURCE_DIR}
        DEPENDS helicsCore.h
                helicsCallbacks.h
                MessageFederate.h
                ValueFederate.h
                MessageFilters.h
                api-data.h
                ../helics_enums.h
    )
else()
    configure_file(
        ${PROJECT_SOURCE_DIR}/src/helics/shared_api_library/backup/helics/helics.h
        ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics.h COPYONLY
    )
    configure_file(
        ${PROJECT_SOURCE_DIR}/src/helics/shared_api_library/backup/helics/helics_api.h
        ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics_api.h COPYONLY
    )
endif()

add_library(
    helics SHARED ${helicsShared_sources} ${helicsShared_headers}
                  ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics.h
)

add_library(HELICS::helics ALIAS helics)
generate_export_header(helics BASE_NAME helics)

target_include_directories(
    helics PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    INTERFACE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/helics_generated_includes>
              $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(helics PROPERTIES VERSION ${HELICS_VERSION} SOVERSION ${HELICS_VERSION_MAJOR})

set_target_properties(helics PROPERTIES CXX_VISIBILITY_PRESET hidden C_VISIBILITY_PRESET hidden)
target_link_libraries(helics PRIVATE helics_application_api)
target_link_libraries(helics PRIVATE compile_flags_target)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.0)
    target_compile_options(helics PUBLIC -Wno-documentation)
endif()

if(UNIX OR MINGW)
    if(NOT APPLE)
        target_link_libraries(
            helics PRIVATE -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/export.txt
        )
    endif(NOT APPLE)
endif()

set_target_properties(helics PROPERTIES FOLDER interfaces)

install(
    TARGETS helics ${HELICS_EXPORT_COMMAND}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(WIN32)
    install(
        FILES $<TARGET_LINKER_FILE:helics>
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT libs
        EXCLUDE_FROM_ALL
    )
    install(
        FILES $<TARGET_FILE:helics>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Runtime
        EXCLUDE_FROM_ALL
    )
else()
    install(
        TARGETS helics
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT RUNTIME
        EXCLUDE_FROM_ALL
    )
endif()

if(MSVC AND NOT HELICS_EMBEDDED_DEBUG_INFO)
    install(
        FILES $<TARGET_PDB_FILE:helics>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        OPTIONAL
        COMPONENT libs
    )
endif()

if(NOT HELICS_BINARY_ONLY_INSTALL)
    install(FILES ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics.h
                  ${PROJECT_BINARY_DIR}/helics_generated_includes/helics/helics_api.h
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/helics COMPONENT headers
    )

    install(FILES ../helics98.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/helics COMPONENT headers)
endif()
