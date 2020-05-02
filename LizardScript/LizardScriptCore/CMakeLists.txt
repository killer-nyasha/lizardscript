project(LizardScriptCore CXX)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "new_meta.py"
    "opcodes_from_switch.py"
)
source_group("" FILES ${no_group_source_files})

set(default
    "LizardScriptDefault.cpp"
    "LizardScriptDefault.h"
)
source_group("default" FILES ${default})

set(external
    "LizardScript.h"
)
source_group("external" FILES ${external})

set(lexer
    "i_lexer.cpp"
    "i_lexer.h"
    "Lexer.cpp"
    "Lexer.h"
    "SyntaxCore.cpp"
    "SyntaxCore.h"
    "Tokens.h"
)
source_group("lexer" FILES ${lexer})

set(metagen
    "metagen_def.h"
    "metagen_init.h"
    "metagen_system.h"
    "TypeInfo.h"
)
source_group("metagen" FILES ${metagen})

set(parser
    "i_parser.cpp"
    "i_parser.h"
    "Parser.cpp"
    "Parser.h"
)
source_group("parser" FILES ${parser})

set(pch
    "pch.cpp"
    "pch.h"
)
source_group("pch" FILES ${pch})

set(runtime
    "LsTypedefs.h"
    "Opcodes.hxx"
    "OpcodesText.cxx"
    "Runtime.cpp"
    "Runtime.h"
    "RuntimeCases.h"
    "RuntimeCases.i"
    "RuntimeMacros.h"
)
source_group("runtime" FILES ${runtime})

set(system__containers
    "FastRemoving.h"
    "NonTypedStack.h"
    "Pools.h"
    "VectorsTuple.h"
)
source_group("system\\containers" FILES ${system__containers})

set(system__other
    "algorithms.h"
    "Delegate.h"
    "Exception.h"
    "interpreter_util.h"
)
source_group("system\\other" FILES ${system__other})

set(system__string_and_i/o
    "ConsoleColor.h"
    "crossplatform_tchar.h"
    "Logger.cpp"
    "Logger.h"
    "Print.cpp"
    "Print.h"
    "StringBuilder.h"
    "stringptr.h"
)
source_group("system\\string and i/o" FILES ${system__string_and_i/o})

set(ALL_FILES
    ${no_group_source_files}
    ${default}
    ${external}
    ${lexer}
    ${metagen}
    ${parser}
    ${pch}
    ${runtime}
    ${system__containers}
    ${system__other}
    ${system__string_and_i/o}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

add_precompiled_header(${PROJECT_NAME} "pch.h" "pch.cpp")

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE LizardScriptCore)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_LIB;"
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_LIB;"
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:WINDOWS
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:WINDOWS
        )
    endif()
endif()

################################################################################
# Pre build events
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMANDS
        COMMAND   $<CONFIG:Debug> cd $<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/> && cl /P RuntimeCases.h && python.exe opcodes_from_switch.py
        COMMAND $<CONFIG:Release> cd $<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/> && cl /P RuntimeCases.h && python.exe opcodes_from_switch.py
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    add_custom_command_if(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMANDS
        COMMAND   $<CONFIG:Debug> cd $<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/> && cl /P RuntimeCases.h && python.exe opcodes_from_switch.py
        COMMAND $<CONFIG:Release> cd $<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/> && cl /P RuntimeCases.h && python.exe opcodes_from_switch.py
    )
endif()

