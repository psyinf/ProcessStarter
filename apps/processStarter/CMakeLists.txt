project(ProcessStarter)

set(CMAKE_CXX_STANDARD 23)
file(GLOB_RECURSE HEADER_FILES 	CONFIGURE_DEPENDS "*.h*")
file(GLOB_RECURSE CPP_FILES 	CONFIGURE_DEPENDS "*.cpp")

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${CPP_FILES} )


CPMADDPACKAGE(
    NAME CLI11
    GITHUB_REPOSITORY "CLIUtils/CLI11"
    GIT_TAG v1.9.1
)

CPMAddPackage(
	NAME nlohmann_json
	GITHUB_REPOSITORY nlohmann/json
	GIT_TAG v3.11.3
)

CPMADDPACKAGE(
	NAME yamlcpp
	GITHUB_REPOSITORY jbeder/yaml-cpp
	GIT_TAG 0.8.0
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
	processLib::processLib
	nlohmann_json::nlohmann_json
	CLI11::CLI11
	yaml-cpp::yaml-cpp
)
target_include_directories(${PROJECT_NAME}
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

enable_coverage(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME})