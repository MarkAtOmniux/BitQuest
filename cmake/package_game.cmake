# Stage a runnable BitQuest folder and wrap it as a DMG (macOS) or ZIP (Windows).
# Invoked by the package_game CMake target with -D variables.

if(NOT DEFINED BITQUEST_EXE OR NOT DEFINED OUTPUT_DIR OR NOT DEFINED RES_DIR)
  message(FATAL_ERROR "package_game.cmake requires BITQUEST_EXE, OUTPUT_DIR, and RES_DIR")
endif()

foreach(_bq_var BITQUEST_EXE BITQUEST_FILE_DIR BITQUEST_BUNDLE_DIR RES_DIR SFML_LIB_DIR SFML_SOURCE_DIR OPENAL_DLL OUTPUT_DIR)
  if(DEFINED ${_bq_var})
    string(REPLACE "\"" "" ${_bq_var} "${${_bq_var}}")
  endif()
endforeach()

file(MAKE_DIRECTORY "${OUTPUT_DIR}")

if(APPLE)
  if(NOT BITQUEST_BUNDLE_DIR)
    message(FATAL_ERROR "macOS packaging requires BITQUEST_BUNDLE_DIR (MACOSX_BUNDLE)")
  endif()

  set(APP "${BITQUEST_BUNDLE_DIR}")
  get_filename_component(APP_NAME "${APP}" NAME)
  set(STAGE "${OUTPUT_DIR}/dmg-root")
  file(REMOVE_RECURSE "${STAGE}")
  file(MAKE_DIRECTORY "${STAGE}")

  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${APP}" "${STAGE}/${APP_NAME}"
    RESULT_VARIABLE COPY_APP_RESULT)
  if(NOT COPY_APP_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to copy ${APP}")
  endif()

  set(STAGED_APP "${STAGE}/${APP_NAME}")
  set(MACOS_DIR "${STAGED_APP}/Contents/MacOS")
  file(MAKE_DIRECTORY "${MACOS_DIR}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${RES_DIR}" "${MACOS_DIR}/res")

  set(BU_CHMOD_BUNDLE_ITEMS ON)
  include(BundleUtilities)
  fixup_bundle(
    "${STAGED_APP}"
    ""
    "${SFML_LIB_DIR};${SFML_SOURCE_DIR}/extlibs/libs-osx/Frameworks")

  set(ARCH_LABEL "macOS")
  if(DEFINED BITQUEST_ARCHS AND NOT BITQUEST_ARCHS STREQUAL "")
    set(ARCH_LABEL "macOS-${BITQUEST_ARCHS}")
  endif()
  set(DMG "${OUTPUT_DIR}/BitQuest-${ARCH_LABEL}.dmg")
  set(ZIP "${OUTPUT_DIR}/BitQuest-${ARCH_LABEL}.zip")

  execute_process(COMMAND rm -f "${DMG}" "${ZIP}")
  execute_process(
    COMMAND hdiutil create -volname BitQuest -srcfolder "${STAGE}" -ov -format UDZO "${DMG}"
    RESULT_VARIABLE DMG_RESULT)
  if(NOT DMG_RESULT EQUAL 0)
    message(FATAL_ERROR "hdiutil failed to create ${DMG}")
  endif()

  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E tar cf "${ZIP}" --format=zip "${APP_NAME}"
    WORKING_DIRECTORY "${STAGE}"
    RESULT_VARIABLE ZIP_RESULT)
  if(NOT ZIP_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to create ${ZIP}")
  endif()

  message(STATUS "Created ${DMG}")
  message(STATUS "Created ${ZIP}")

elseif(WIN32)
  set(STAGE "${OUTPUT_DIR}/BitQuest")
  file(REMOVE_RECURSE "${STAGE}")
  file(MAKE_DIRECTORY "${STAGE}")

  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy "${BITQUEST_EXE}" "${STAGE}/")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${RES_DIR}" "${STAGE}/res")

  if(DEFINED BITQUEST_RUNTIME_FILES AND NOT BITQUEST_RUNTIME_FILES STREQUAL "")
    string(REPLACE "|" ";" BITQUEST_RUNTIME_FILES "${BITQUEST_RUNTIME_FILES}")
    foreach(RUNTIME_FILE IN LISTS BITQUEST_RUNTIME_FILES)
      if(EXISTS "${RUNTIME_FILE}")
        execute_process(
          COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${RUNTIME_FILE}" "${STAGE}/")
      endif()
    endforeach()
  endif()

  if(DEFINED OPENAL_DLL AND EXISTS "${OPENAL_DLL}")
    execute_process(
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${OPENAL_DLL}" "${STAGE}/")
  endif()

  set(ZIP "${OUTPUT_DIR}/BitQuest-windows.zip")
  execute_process(COMMAND "${CMAKE_COMMAND}" -E rm -f "${ZIP}")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" -E tar cf "${ZIP}" --format=zip "BitQuest"
    WORKING_DIRECTORY "${OUTPUT_DIR}"
    RESULT_VARIABLE ZIP_RESULT)
  if(NOT ZIP_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to create ${ZIP}")
  endif()

  message(STATUS "Created ${ZIP}")

else()
  message(FATAL_ERROR "package_game supports macOS and Windows only")
endif()
