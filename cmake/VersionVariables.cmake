# Populate the CMake variables used to configure version.cpp.in

set(SHPXX_VERSION_MAJOR ${shpxx_VERSION_MAJOR})
set(SHPXX_VERSION_MINOR ${shpxx_VERSION_MINOR})
set(SHPXX_VERSION_PATCH ${shpxx_VERSION_PATCH})

set(SHPXX_VERSION_STR "${SHPXX_VERSION_MAJOR}.${SHPXX_VERSION_MINOR}.${SHPXX_VERSION_PATCH}")

string(TIMESTAMP SHPXX_BUILD_TIMESTAMP "%Y-%m-%dT%H:%M:%S" UTC)

include(GetGitRevisionDescription)
get_git_head_revision(SHPXX_GIT_REF SHPXX_GIT_HASH)

git_local_changes(SHPXX_GIT_STATE)
if(SHPXX_GIT_STATE STREQUAL "DIRTY")
    set(SHPXX_GIT_HASH "${SHPXX_GIT_HASH}-dirty")
endif()
