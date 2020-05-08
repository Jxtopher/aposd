###############################################################################
# Get info git and genereted version.h from version.h.in./ 
###############################################################################
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/version.h.in")
    file(WRITE "${CMAKE_SOURCE_DIR}/version.h.in" "#ifndef VERSION_H
#define VERSION_H

#define GIT_BRANCH \"@GIT_BRANCH@\"
#define GIT_COMMIT_HASH \"@GIT_COMMIT_HASH@\"

#endif
")
endif(NOT EXISTS "${CMAKE_SOURCE_DIR}/version.h.in")

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND git log -1 --format=%h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
else(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(GIT_BRANCH "")
  set(GIT_COMMIT_HASH "")
endif(EXISTS "${CMAKE_SOURCE_DIR}/.git")

message(STATUS "Git current branch: ${GIT_BRANCH}")
message(STATUS "Git commit hash: ${GIT_COMMIT_HASH}")
configure_file(
  ${CMAKE_SOURCE_DIR}/version.h.in
  ${CMAKE_BINARY_DIR}/generated/version.h
)

include_directories(${CMAKE_BINARY_DIR}/generated)