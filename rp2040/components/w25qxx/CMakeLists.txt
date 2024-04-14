add_library(w25qxx
    w25qxx.c
)

target_include_directories(w25qxx PUBLIC ${CMAKE_CURRENT_LIST_DIR})