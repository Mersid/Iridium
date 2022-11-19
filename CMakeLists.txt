cmake_minimum_required (VERSION 3.8)

project (Iridium)
set(CMAKE_CXX_STANDARD 20)

add_executable(Iridium src/Main.cpp src/Iridium.cpp src/Iridium.h src/texture/StbImplementation.cpp src/texture/StbImplementation.h src/texture/Texture.cpp src/texture/Texture.h src/texture/TextureSerializer.cpp src/texture/TextureSerializer.h src/geometry/Parallelogram.cpp src/geometry/Parallelogram.h src/geometry/Ray.cpp src/geometry/Ray.h src/scene/Camera.cpp src/scene/Camera.h src/scene/Scene.cpp src/scene/Scene.h src/scene/CameraMode.h src/scene/Light.cpp src/scene/Light.h src/geometry/Primitive.h src/geometry/Sphere.cpp src/geometry/Sphere.h src/geometry/Material.cpp src/geometry/Material.h src/texture/CheckerboardTextureGenerator.cpp src/texture/CheckerboardTextureGenerator.h src/geometry/Triangle.cpp src/geometry/Triangle.h src/geometry/OffSerializer.cpp src/geometry/OffSerializer.h src/utils/StringUtils.cpp src/utils/StringUtils.h src/geometry/Mesh.cpp src/geometry/Mesh.h src/geometry/Model.cpp src/geometry/Model.h src/geometry/Box.cpp src/geometry/Box.h src/geometry/BoundingVolumeHierarchy.cpp src/geometry/BoundingVolumeHierarchy.h src/geometry/RayTraceInfo.cpp src/geometry/RayTraceInfo.h src/misc/Vector3dConvert.h src/misc/Transform.cpp src/misc/Transform.h src/misc/Options.cpp src/misc/Options.h)

target_include_directories(Iridium PRIVATE lib/eigen-3.4.0)
target_include_directories(Iridium PRIVATE lib/stb)

# https://www.reddit.com/r/cpp_questions/comments/bdxcp0/im_a_beginner_to_c_help_me_install_and_use/
# https://github.com/jbeder/yaml-cpp/issues/566
add_subdirectory(lib/yaml-cpp-0.7.0)
target_link_libraries(Iridium PRIVATE yaml-cpp)

# https://stackoverflow.com/questions/42476765/copying-assets-directory-from-source-to-build-directory-in-cmake
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
        )
add_dependencies(Iridium copy_assets)