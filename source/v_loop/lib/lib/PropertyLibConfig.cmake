set(LIB PropertyLib)

# Set the include directory
set(${LIB}_INSTALL_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Create imported target for my_library
add_library(${LIB} STATIC IMPORTED)
set_target_properties(${LIB} PROPERTIES
    IMPORTED_LOCATION "${${LIB}_INSTALL_DIR}/lib${LIB}.a"
    INTERFACE_INCLUDE_DIRECTORIES "${${LIB}_INSTALL_DIR}/../include"
)
