cmake_minimum_required(VERSION 2.8)

FOREACH(MY_SAMPLES
  helloworld
  dumptable
  thread
  bindclass
  function
  upvalue
)

ADD_SUBDIRECTORY(${MY_SAMPLES})
SET_PROJECT_OUTPUT_PROPERTIES(${MY_SAMPLES})
SET_TARGET_PROPERTIES(${MY_SAMPLES} PROPERTIES FOLDER "sample")

ENDFOREACH()