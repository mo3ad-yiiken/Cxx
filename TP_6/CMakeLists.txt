## Indique qu'elle version de cmake minimale est utilisée.
cmake_minimum_required (VERSION 3.16.3)

## Specifie le nom du projet et le langage de programmation du projet
project(Lab5 CXX)

## Force le standard 
set(CMAKE_CXX_STANDARD 17)



# structure de test 

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

# Apple Clang places /usr/local/include before any -isystem directories
# in its default search path.
#
# googletest marks its headers as SYSTEM includes, which means that if a
# newer Homebrew-installed gtest exists in /usr/local/include, it can be
# picked up before the fetched version from this project.
#
# That causes mixed API/header usage and breaks the build.
#
# On macOS, we clear INTERFACE_SYSTEM_INCLUDE_DIRECTORIES for the gtest
# targets so the fetched sources are used consistently.

enable_testing()


if(APPLE)
    foreach(_gtest_target IN ITEMS
        gtest
        gtest_main
        gmock
        gmock_main
    )
        if(TARGET ${_gtest_target})
            set_property(
                TARGET ${_gtest_target}
                PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ""
            )
        endif()
    endforeach()
endif()


## Indique les options de compilations pour le projet
## -pg : donne la possibilité de faire du profiling
## -Wall : affiche les warnings
set(CMAKE_CXX_FLAGS "-Wall")

## Définir la localisation des entêtes.
## include_directories sera propager à l'ensemble du projet
include_directories(${CMAKE_SOURCE_DIR}/include)

## Parcours les sous répertoires contenant les définitions (.cxx)
## On commence par créer une bibliothèque
add_subdirectory(src)

## On créée les tests pour la bibliothèque.
## Ces tests sont unitaires ou fonctionnels
add_subdirectory(test)

## On implémente une démo, un programme de vérification.