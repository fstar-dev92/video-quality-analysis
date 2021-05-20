CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (FREEZE_DETECT_UT C CXX)
#SET (AMAGI_MIMAS  1)
if(AMAGI_MIMAS)
    #message(WARNING "amagi_mimas is being used")
    include("../../../../../common/helios.cmake")
    add_definitions(-DAMG_MIMAS)
    include_directories(${FREEZE_DETECT_UT_SOURCE_DIR}/../../inc/)
else()
    #message(WARNING "amagi_mimas is not being used")
    SET (INTEL_IPP /opt/intel/oneapi/ipp/2021.2.0/)
    SET (INTEL_IPP_INCLUDE_DIRS ${INTEL_IPP}/include)
    SET (INTEL_IPP_LIBRARY_DIRS ${INTEL_IPP}/lib/intel64)
    SET (INTEL_IPP_LIBRARIES ippi ipps ippcore ippvm ippcc ippcv)
    INCLUDE_DIRECTORIES (SYSTEM ${INTEL_IPP_INCLUDE_DIRS})
    LINK_DIRECTORIES (${INTEL_IPP_LIBRARY_DIRS})
endif(AMAGI_MIMAS)

include_directories(${FREEZE_DETECT_UT_SOURCE_DIR}/../inc/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
add_executable(amg_video_freeze_detect_ut_fsm
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_state_detect.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_freeze_detect.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_freeze_detect_fsm.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_freeze_detect_algo.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/amg_video_freeze_detect_ut_fsm.c
    )

if(AMAGI_MIMAS)
    target_link_libraries(amg_video_freeze_detect_ut_fsm ${INTEL_IPP_LIBRARIES} -std=c99 -lm amg-log4cxx-c-abi)
else()
    target_link_libraries(amg_video_freeze_detect_ut_fsm ${INTEL_IPP_LIBRARIES} -std=c99 -lm)
endif(AMAGI_MIMAS)
INSTALL (TARGETS amg_video_freeze_detect_ut_fsm
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)

add_executable(amg_video_black_detect_slice_algo_ut
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_black_detect_slice_algo.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/amg_video_black_detect_slice_algo_ut.c
    )

if(AMAGI_MIMAS)
    target_link_libraries(amg_video_black_detect_slice_algo_ut ${INTEL_IPP_LIBRARIES} -std=c99 -lm amg-log4cxx-c-abi)
else()
    target_link_libraries(amg_video_black_detect_slice_algo_ut ${INTEL_IPP_LIBRARIES} -std=c99 -lm)
endif(AMAGI_MIMAS)

INSTALL (TARGETS amg_video_black_detect_slice_algo_ut
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)

add_executable(amg_video_black_detect_algo_ut
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_black_detect_algo.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/amg_video_black_detect_algo_ut.c
    )

if(AMAGI_MIMAS)
    target_link_libraries(amg_video_black_detect_algo_ut ${INTEL_IPP_LIBRARIES} -std=c99 -lm amg-log4cxx-c-abi)
else()
    target_link_libraries(amg_video_black_detect_algo_ut ${INTEL_IPP_LIBRARIES} -std=c99 -lm)
endif(AMAGI_MIMAS)

INSTALL (TARGETS amg_video_black_detect_algo_ut
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)

add_executable(amg_video_state_detect_ut
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_state_detect.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/amg_video_state_detect_ut.c
    )

if(AMAGI_MIMAS)
    target_link_libraries(amg_video_state_detect_ut ${INTEL_IPP_LIBRARIES} -std=c99 -lm amg-log4cxx-c-abi)
else()
    target_link_libraries(amg_video_state_detect_ut ${INTEL_IPP_LIBRARIES} -std=c99 -lm)
endif(AMAGI_MIMAS)

INSTALL (TARGETS amg_video_state_detect_ut
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)

add_executable(amg_video_black_detect_ut_fsm
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_state_detect.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_black_detect_fsm.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_black_detect.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_black_detect_algo.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/../src/amg_video_black_detect_slice_algo.c
    ${FREEZE_DETECT_UT_SOURCE_DIR}/amg_video_black_detect_ut_fsm.c
    )

if(AMAGI_MIMAS)
    target_link_libraries(amg_video_black_detect_ut_fsm ${INTEL_IPP_LIBRARIES} -std=c99 -lm amg-log4cxx-c-abi)
else()
    target_link_libraries(amg_video_black_detect_ut_fsm ${INTEL_IPP_LIBRARIES} -std=c99 -lm)
endif(AMAGI_MIMAS)

INSTALL (TARGETS amg_video_black_detect_ut_fsm
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ WORLD_READ WORLD_EXECUTE)
