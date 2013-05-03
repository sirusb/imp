# Autogenerated file, run tools/build/setup_cmake.py to regenerate

include_directories(%(includepath)s)
link_directories(%(libpath)s)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${%(NAME)s_CXX_FLAGS}")


File(GLOB runtimepytests "${PROJECT_BINARY_DIR}/test/%(name)s/test_*.py")

math(EXPR short_timeout "${IMP_TIMEOUT_FACTOR} * 5")
math(EXPR medium_timeout "${IMP_TIMEOUT_FACTOR} * 15")
math(EXPR expensive_timeout "${IMP_TIMEOUT_FACTOR} * 120")

# should make into function
foreach (test ${runtimepytests} %(pytests)s)
  GET_FILENAME_COMPONENT(name ${test} NAME)
  add_test("%(name)s.${name}" ${IMP_TEST_SETUP} python ${test})
  set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test;length_short")
  set_tests_properties("%(name)s.${name}" PROPERTIES TIMEOUT ${short_timeout})
  set_tests_properties("%(name)s.${name}" PROPERTIES COST 1)
endforeach(test)

foreach (test %(mdpytests)s)
  GET_FILENAME_COMPONENT(name ${test} NAME)
  add_test("%(name)s.${name}" ${IMP_TEST_SETUP} python ${test})
  set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test;length_medium")
  set_tests_properties("%(name)s.${name}" PROPERTIES TIMEOUT ${medium_timeout})
  set_tests_properties("%(name)s.${name}" PROPERTIES COST 2)
endforeach(test)

foreach (test %(expytests)s)
  GET_FILENAME_COMPONENT(name ${test} NAME)
  add_test("%(name)s.${name}" ${IMP_TEST_SETUP} python ${test})
  set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test;length_expensive")
  set_tests_properties("%(name)s.${name}" PROPERTIES TIMEOUT ${expensive_timeout})
  set_tests_properties("%(name)s.${name}" PROPERTIES COST 3)
endforeach(test)

set(cpp_tests %(cpptests)s)

foreach (test ${cpp_tests})
   GET_FILENAME_COMPONENT(name ${test} NAME)
   GET_FILENAME_COMPONENT(name_we ${test} NAME_WE)
   add_executable("%(name)s.${name}" ${test})
   target_link_libraries("%(name)s.${name}"     imp_%(name)s
    %(modules)s
    %(dependencies)s)
   set_target_properties("%(name)s.${name}" PROPERTIES
                         RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test/%(name)s/"
                         OUTPUT_NAME ${name_we})
   add_test("%(name)s.${name}" ${IMP_TEST_SETUP}
            "${PROJECT_BINARY_DIR}/test/%(name)s/${name_we}${CMAKE_EXECUTABLE_SUFFIX}")
   set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test;length_short")
   set_tests_properties("%(name)s.${name}" PROPERTIES TIMEOUT ${short_timeout})
   set_tests_properties("%(name)s.${name}" PROPERTIES COST 1)
   set(executables ${executables} "%(name)s.${name}")
endforeach(test)


set(mdcpp_tests %(mdcpptests)s)

foreach (test ${mdcpp_tests})
   GET_FILENAME_COMPONENT(name ${test} NAME)
   GET_FILENAME_COMPONENT(name_we ${test} NAME_WE)
   add_executable("%(name)s.${name}" ${test})
   target_link_libraries("%(name)s.${name}"     imp_%(name)s
    %(modules)s
    %(dependencies)s)
   set_target_properties("%(name)s.${name}" PROPERTIES
                         RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test/%(name)s/"
                         OUTPUT_NAME ${name_we})
   add_test("%(name)s.${name}" ${IMP_TEST_SETUP}
            "${PROJECT_BINARY_DIR}/test/%(name)s/${name_we}${CMAKE_EXECUTABLE_SUFFIX}")
   set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test;length_medium")
   set_tests_properties("%(name)s.${name}" PROPERTIES TIMEOUT ${medium_timeout})
   set_tests_properties("%(name)s.${name}" PROPERTIES COST 1)
   set(executables ${executables} "%(name)s.${name}")
endforeach(test)

set(excpp_tests %(excpptests)s)

foreach (test ${excpp_tests})
   GET_FILENAME_COMPONENT(name ${test} NAME)
   GET_FILENAME_COMPONENT(name_we ${test} NAME_WE)
   add_executable("%(name)s.${name}" ${test})
   target_link_libraries("%(name)s.${name}"     imp_%(name)s
    %(modules)s
    %(dependencies)s)
   set_target_properties("%(name)s.${name}" PROPERTIES
                         RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/test/%(name)s/"
                         OUTPUT_NAME ${name_we})
   add_test("%(name)s.${name}" ${IMP_TEST_SETUP}
            "${PROJECT_BINARY_DIR}/test/%(name)s/${name_we}${CMAKE_EXECUTABLE_SUFFIX}")
   set_tests_properties("%(name)s.${name}" PROPERTIES LABELS "IMP.%(name)s;test;length_expensive")
   set_tests_properties("%(name)s.${name}" PROPERTIES TIMEOUT ${expensive_timeout})
   set_tests_properties("%(name)s.${name}" PROPERTIES COST 1)
   set(executables ${executables} "%(name)s.${name}")
endforeach(test)

add_custom_target("imp_%(name)s_tests" ALL DEPENDS ${executables})
