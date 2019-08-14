include(ExternalProject)
set(BOOST_BUILD_COMMAND ./b2 toolset=clang install --prefix=${CMAKE_BINARY_DIR}/external --threading=single,multi --link=static --variant=release --address-model=64 -j4)
set(BOOST_ROOT "${CMAKE_BINARY_DIR}/external")
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON)
ExternalProject_Add(boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.70.0
  GIT_SHALLOW true
  BUILD_IN_SOURCE true
  CONFIGURE_COMMAND ./bootstrap.sh
  BUILD_COMMAND ${BOOST_BUILD_COMMAND}
  INSTALL_COMMAND ""
  PATCH_COMMAND ""
  UPDATE_COMMAND ""
  INSTALL_DIR ${CMAKE_BINARY_DIR}/external
)
