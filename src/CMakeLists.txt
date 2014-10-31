PROJECT(libscript)

SET(LIB_NAME libscript)

#########################################################################################
## FILES
#########################################################################################

SET(SRC_LIST
    libscript.h
    libscript_sys.h
    libscript_sys.cpp
    libscript_bind.h
    libscript_bind.cpp
    libscript_cd.h
    libscript_cd.cpp
    libscript_function.h
    libscript_function.cpp
    libscript_fwd.h
    libscript_args.h
    libscript_args.cpp
    libscript_luaapi.h
    libscript_stack.h
    libscript_stack.cpp
    libscript_table.h
    libscript_table.cpp
    libscript_table_dump.h
    libscript_table_dump.cpp
    libscript_thread.h
    libscript_thread.cpp
    libscript_value.h
    libscript_value.cpp
    libscript_pusher.h
    libscript_pusher.cpp
)

SOURCE_GROUP(${LIB_NAME} FILES ${SRC_LIST})

ADD_LIBRARY(${LIB_NAME} ${SRC_LIST})

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