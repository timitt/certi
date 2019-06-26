enable_testing()

include_directories(${CERTI_SOURCE_DIR})
include_directories(${CERTI_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include/ieee1516-2010)
include_directories(${CMAKE_BINARY_DIR}/include/ieee1516-2010)

find_package(Threads REQUIRED)

add_executable(TestRTI-IEEE1516-2010
                simpletypeencoding_test.cpp
                hlatypes_test.cpp
                ../../main.cpp
                )

target_link_libraries(TestRTI-IEEE1516-2010
                      RTI RTI1516e
                      ${GTEST_BOTH_LIBRARIES}
                      ${CMAKE_THREAD_LIBS_INIT}
                      )
                      
target_compile_definitions(TestRTI-IEEE1516-2010 PRIVATE CERTI_TEST)
                

if (COMPILE_WITH_COVERAGE)
    SETUP_TARGET_FOR_COVERAGE(
        NAME TestRTI-IEEE1516-2010_coverage
        EXECUTABLE TestRTI-IEEE1516-2010 --gtest_output=xml:../output/results-TestRTI-1_3.xml
        DEPENDENCIES TestRTI-IEEE1516-2010
    )

    SETUP_TARGET_FOR_COVERAGE_COBERTURA(
        NAME TestRTI-IEEE1516-2010_cobertura
        EXECUTABLE TestRTI-IEEE1516-2010 --gtest_output=xml:../output/results-TestRTI-1_3.xml
        DEPENDENCIES TestRTI-IEEE1516-2010
    )
endif()

add_test(AllTests TestRTI-IEEE1516-2010)
