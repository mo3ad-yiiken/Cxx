# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-src"
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-build"
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mo3ad/Documents/ENSIMAG/Second-year/C++/TP_CPP_touatia_iknem/Cxx/TP_5/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
