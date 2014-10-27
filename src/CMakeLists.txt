PROJECT(libscript)

SET(LIB_NAME libscript)

#########################################################################################
## FILES
#########################################################################################

SET(HEADER_LIST
    libscript.h
    libscript_sys.h
    libscript_bind.h
    libscript_function.h
    libscript_fwd.h
    libscript_args.h
    libscript_luaapi.h
    libscript_stack.h
    libscript_stack.h
    libscript_table.h
    libscript_table_dump.h
    libscript_thread.h
    libscript_value.h
    libscript_pusher.h
    libscript_cd.h
)

SET(SOURCE_LIST
    libscript_sys.cpp
    libscript_bind.cpp
    libscript_function.cpp
    libscript_args.cpp
    libscript_stack.cpp
    libscript_table.cpp
    libscript_table_dump.cpp
    libscript_thread.cpp
    libscript_value.cpp
    libscript_pusher.cpp
    libscript_cd.cpp
)

SOURCE_GROUP(${LIB_NAME} FILES ${HEADER_LIST})
SOURCE_GROUP(${LIB_NAME} FILES ${SOURCE_LIST})

ADD_LIBRARY(${LIB_NAME} ${HEADER_LIST} ${SOURCE_LIST})

#########################################################################################
## INCLUDE & LINK
#########################################################################################

FOREACH(DEPENDENCIES_LIB
  lua
)
    ADD_DEPENDENCIES_LIB(${DEPENDENCIES_LIB})
ENDFOREACH()

#########################################################################################
## Others
#########################################################################################

SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIBRARY_OUTPUT_PATH}
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${LIBRARY_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIBRARY_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${LIBRARY_OUTPUT_PATH}
    PROJECT_LABEL ${LIB_NAME}
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)