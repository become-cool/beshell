
function(embed_file input_file output_file)

    file(TIMESTAMP input_file_time ${input_file})
    file(TIMESTAMP output_file_time ${output_file})

    if(file1_time STRGREATER file2_time)
        message("input was modified more recently than output")
    endif()
    

    # get_filename_component(varname ${input_file} NAME)
    # string(REPLACE "." "_" varname ${varname})
    # string(REPLACE "/" "_" varname ${varname})
    # string(REPLACE "\\" "_" varname ${varname})
    # string(REPLACE ":" "_" varname ${varname})

    # add_custom_command(
    #     OUTPUT ${output_file}
    #     COMMAND ${CMAKE_COMMAND} -E echo "embed ${input_file} as ${output_file}"
    #     # COMMAND echo \#pragma once > ${output_file}
    #     # COMMAND echo  >> ${output_file}
    #     # COMMAND echo -n const char embeded_file_${varname} = R(\\\" >> ${output_file}
    #     # COMMAND cat ${input_file} >> ${output_file}
    #     # COMMAND echo -n  >> ${output_file}
    #     # COMMAND echo \\\")\; >> ${output_file}
    #     DEPENDS ${input_file}
    # )
    # add_custom_target(target_embed_${varname} ALL DEPENDS ${output_file})
endfunction()

embed_file(${CMAKE_CURRENT_LIST_DIR}/src/module/wifi.js ${CMAKE_CURRENT_LIST_DIR}/src/module/wifi.js.h)
