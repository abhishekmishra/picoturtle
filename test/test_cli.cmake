function(expect_exit label expected)
    execute_process(
        COMMAND ${ARGN}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE stdout
        ERROR_VARIABLE stderr
    )
    if(NOT result EQUAL expected)
        message(
            FATAL_ERROR
            "${label}: expected exit ${expected}, got ${result}\n"
            "stdout:\n${stdout}\nstderr:\n${stderr}"
        )
    endif()
endfunction()

expect_exit("unknown option" 1 "${PICOTURTLE}" --not-an-option)
expect_exit("missing program" 1 "${PICOTURTLE}")
expect_exit(
    "multiple programs"
    1
    "${PICOTURTLE}"
    first.lua
    second.lua
)
expect_exit(
    "missing Lua file"
    1
    "${PICOTURTLE}"
    --no-wait
    "${SOURCE_DIR}/test/does-not-exist.lua"
)

set(export_path "${OUTPUT_DIR}/picoturtle-cli-export.png")
file(REMOVE "${export_path}")
expect_exit(
    "PNG export"
    0
    "${PICOTURTLE}"
    --no-wait
    --output
    "${export_path}"
    "${SOURCE_DIR}/lua/samples/polygons.lua"
)
if(NOT EXISTS "${export_path}")
    message(FATAL_ERROR "PNG export did not create ${export_path}")
endif()
file(SIZE "${export_path}" export_size)
if(export_size LESS 100)
    message(FATAL_ERROR "PNG export is unexpectedly small: ${export_size} bytes")
endif()

expect_exit(
    "PNG export failure"
    1
    "${PICOTURTLE}"
    --no-wait
    --output
    "${OUTPUT_DIR}"
    "${SOURCE_DIR}/lua/samples/empty.lua"
)
