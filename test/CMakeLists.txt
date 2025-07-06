cmake_minimum_required(VERSION 3.5)
project(org_chart_test CXX)

add_executable(${PROJECT_NAME}_test 
    test_main.cc
    test_controllers.cc
    # Consolidated and refined test files
    test_AuthController.cc
    test_DepartmentController.cc
    test_Components.cc
)

target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)

ParseAndAddDrogonTests(${PROJECT_NAME}_test)
