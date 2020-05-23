

function(packageManager name_pakage)
    if (UNIX)
        if(EXISTS "/usr/bin/apt-get") 
            message(STATUS "sudo apt-get install ${name_pakage}")
            execute_process(COMMAND sudo apt-get install -y ${name_pakage})
        else()
            message(STATUS "apt-get does not exist")
        endif()
    endif (UNIX)
endfunction()
