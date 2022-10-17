#    add_custom_command( TARGET vertices POST_BUILD
#                         COMMAND ${CMAKE_COMMAND}
#                         ARGS "-DSOURCE=${CMAKE_BINARY_DIR}/external" "-DOUTPUT_DIR=${CMAKE_BINARY_DIR}/good" -P "${install_script}"
#                     )

file( INSTALL ${SOURCE} DESTINATION ${OUTPUT_DIR} )