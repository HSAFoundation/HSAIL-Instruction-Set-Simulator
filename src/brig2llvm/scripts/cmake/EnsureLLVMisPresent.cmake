macro(ensure_llvm_is_present dest_dir name)

if(EXISTS "${dest_dir}/${name}")
MESSAGE("llvm is present")
  execute_process( COMMAND ${Subversion_SVN_EXECUTABLE} switch http://llvm.org/svn/llvm-project/llvm/branches/release_35/ ${dest_dir}/${name} )
  execute_process( COMMAND ${Subversion_SVN_EXECUTABLE} up ${dest_dir}/${name} )
else(EXISTS "${dest_dir}/${name}")
Find_Package(Subversion)

if(${Subversion_FOUND})
  execute_process( COMMAND ${Subversion_SVN_EXECUTABLE} co http://llvm.org/svn/llvm-project/llvm/branches/release_35/ ${dest_dir}/${name} )
  execute_process( COMMAND ${Subversion_SVN_EXECUTABLE} propset svn:ignore ${name} ${dest_dir}  )
else(${Subversion_FOUND})
  MESSAGE(FATAL_ERROR "Upstream llvm is not present at ${dest_dir}/${name} and svn could not be found")
endif()

endif()
endmacro()
