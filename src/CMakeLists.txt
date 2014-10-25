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
    libscript_helper.h
    libscript_luaapi.h
    libscript_stack.h
    libscript_stack.h
    libscript_table.h
    libscript_table_dump.h
    libscript_thread.h
    libscript_value.h
)

SET(SOURCE_LIST
    libscript_sys.cpp
    libscript_bind.cpp
    libscript_function.cpp
    libscript_helper.cpp
    libscript_stack.cpp
    libscript_table.cpp
    libscript_table_dump.cpp
    libscript_thread.cpp
    libscript_value.cpp
)

SOURCE_GROUP(${LIB_NAME} FILES ${HEADER_LIST})
SOURCE_GROUP(${LIB_NAME} FILES ${SOURCE_LIST})

ADD_LIBRARY(${LIB_NAME} ${HEADER_LIST} ${SOURCE_LIST})

#########################################################################################
## INCLUDE & LINK
#########################################################################################

TARGET_LINK_LIBRARIES(${LIB_NAME}
    lua
)
INCLUDE_DIRECTORIES(${DEPENDENCIES_PATH}/lua)