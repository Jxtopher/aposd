

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

# function(dlSources url_git)
#     execute_process(COMMAND sudo apt-get install -y ${name_pakage})
# endfunction()

# git clone https://github.com/catchorg/Catch2.git
# mkdir -p Catch2/build
# cd Catch2/build
# cmake .. && make
# sudo make install

# -------------------

# apt-get install  libargtable2-dev libcurl4-gnutls-dev libmicrohttpd libhiredis-dev

# git clone https://github.com/cinemast/libjson-rpc-cpp.git
# mkdir -p libjson-rpc-cpp/build
# cd libjson-rpc-cpp/build
# cmake .. && make
# sudo make install