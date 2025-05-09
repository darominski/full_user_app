function(MakeBsp EL)
    # BSP_HOME must absolutely be absolute, otherwise BUILD_BYPRODUCTS and IMPORTED_LOCATION are not interpreted the same way
    get_filename_component(BSP_HOME ${CMAKE_CURRENT_FUNCTION_LIST_DIR} ABSOLUTE)
    set(LIBRARY "${BSP_HOME}/psu_cortexa53_1/lib/libxil_el${EL}.a")

    include(ExternalProject)

    ExternalProject_Add(xil_bsp_el${EL}
            SOURCE_DIR ${BSP_HOME}
            CONFIGURE_COMMAND ""
            BUILD_COMMAND make EL=el${EL} EXTRA_COMPILER_FLAGS=-I${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../bmboot/slave/zynqmp/el${EL}
            INSTALL_COMMAND ""
            BUILD_IN_SOURCE TRUE
            BUILD_BYPRODUCTS "${LIBRARY}"
            )

    add_library(fgc4_bsp_el${EL} STATIC IMPORTED)

    set_target_properties(fgc4_bsp_el${EL} PROPERTIES
            IMPORTED_LOCATION "${LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${BSP_HOME}/psu_cortexa53_1/include;${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../bmboot/slave/zynqmp/el${EL}"
            )

    set(BMBOOT_BSP_EL${EL}_HOME ${BSP_HOME}/psu_cortexa53_1 PARENT_SCOPE)
    set(BMBOOT_BSP_EL${EL}_INCLUDE_DIR ${BSP_HOME}/psu_cortexa53_1/include ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../bmboot/slave/zynqmp/el${EL} PARENT_SCOPE)
    set(BMBOOT_BSP_EL${EL}_LIBRARIES "${BSP_HOME}/psu_cortexa53_1/lib/libxil_el${EL}.a" PARENT_SCOPE)
endfunction()
