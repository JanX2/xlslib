project(test)
set(testCPP_SOURCE_FILES mainCPP.cpp md5cpp.cpp)
add_executable(testCPP ${testCPP_SOURCE_FILES})
target_link_libraries (testCPP LINK_PUBLIC xlslib)