cmake_minimum_required (VERSION 2.6)
project (sqlite_wrapper)

find_package(Sqlite REQUIRED)

add_library(sqlite_wrapper src/sqlite_wrapper.c)
TARGET_LINK_LIBRARIES(sqlite_wrapper ${SQLITE_LIBRARY})


find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})
add_library(sqliteWrapper SHARED jni/src/sqlite_wrapper.c)

