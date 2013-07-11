macro(assembleHSAIL input_name output_name)
add_custom_command(OUTPUT ${output_name}
  COMMAND ${CMAKE_BINARY_DIR}/HSAIL-Tools/hsailasm ${input_name} -o ${output_name} -g
  DEPENDS ${input_name} build_libhsail
)
endmacro()

set(sample_OUTPUTS)
file(GLOB sample_sources RELATIVE ${PROJECT_SOURCE_DIR}/test *.hsail)
foreach(_file ${sample_sources})
  string(REPLACE ".hsail" ".o" _file_o ${_file})
  assembleHSAIL(${PROJECT_SOURCE_DIR}/test/${_file} ${_file_o})
  list(APPEND sample_OUTPUTS ${_file_o})
endforeach()

if(IS_X86)
  add_custom_target(HSAILAssemblerSamples ALL SOURCES ${sample_OUTPUTS})
endif(IS_X86)
