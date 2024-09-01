file(GLOB SRC_FILES "*.cpp" "*.c")

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS ""
)
