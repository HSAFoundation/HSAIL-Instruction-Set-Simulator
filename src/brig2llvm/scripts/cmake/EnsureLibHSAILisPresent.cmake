macro(ensure_libHSAIL_is_present dest_dir name)

if(EXISTS "${dest_dir}/${name}")
  MESSAGE("libHSAIL is present")
  execute_process( COMMAND ${GIT_EXECUTABLE} pull https://github.com/HSAFoundation/HSAIL-Tools.git
                   WORKING_DIRECTORY ${dest_dir}/${name})

else(EXISTS "${dest_dir}/${name}")
Find_Package(Git)

if(${GIT_FOUND})
  execute_process(COMMAND mkdir ${dest_dir}/${name})
  execute_process( COMMAND ${GIT_EXECUTABLE} init ${dest_dir}/${name})
  execute_process( COMMAND ${GIT_EXECUTABLE} pull https://github.com/HSAFoundation/HSAIL-Tools.git
                   WORKING_DIRECTORY ${dest_dir}/${name})
else(${GIT_FOUND})
  MESSAGE(FATAL_ERROR "LibHSAIL is not present at ${dest_dir}/${name} and GIT could not be found")
endif()

endif()
endmacro()

