cmake_minimum_required (VERSION 3.8)

project (Shimmerlight)
set(CMAKE_CXX_STANDARD 20)

add_executable(Shimmerlight src/Main.cpp src/Shimmerlight.cpp src/Shimmerlight.h src/texture/StbImplementation.cpp src/texture/StbImplementation.h src/texture/Texture.cpp src/texture/Texture.h src/texture/TextureSerializer.cpp src/texture/TextureSerializer.h src/geometry/Parallelogram.cpp src/geometry/Parallelogram.h src/geometry/Ray.cpp src/geometry/Ray.h src/scene/Camera.cpp src/scene/Camera.h src/scene/Scene.cpp src/scene/Scene.h src/scene/CameraMode.h src/scene/Light.cpp src/scene/Light.h src/geometry/Primitive.h src/geometry/Sphere.cpp src/geometry/Sphere.h src/geometry/Material.cpp src/geometry/Material.h src/texture/CheckerboardTextureGenerator.cpp src/texture/CheckerboardTextureGenerator.h src/geometry/Triangle.cpp src/geometry/Triangle.h src/geometry/OffSerializer.cpp src/geometry/OffSerializer.h src/utils/StringUtils.cpp src/utils/StringUtils.h src/geometry/Mesh.cpp src/geometry/Mesh.h src/geometry/Model.cpp src/geometry/Model.h src/geometry/Box.cpp src/geometry/Box.h src/geometry/BoundingVolumeHierarchy.cpp src/geometry/BoundingVolumeHierarchy.h src/geometry/RayTraceInfo.cpp src/geometry/RayTraceInfo.h src/misc/Vector3dConvert.h src/defs/CameraDef.h src/defs/LightDef.h src/defs/ModelDef.h src/defs/TransformDef.h src/misc/Transform.cpp src/misc/Transform.h src/defs/MeshDef.h src/testing/Base.h src/testing/Derived.h)
target_include_directories(Shimmerlight PUBLIC lib/eigen-3.4.0)
target_include_directories(Shimmerlight PUBLIC lib/stb)

# https://www.reddit.com/r/cpp_questions/comments/bdxcp0/im_a_beginner_to_c_help_me_install_and_use/
# https://github.com/jbeder/yaml-cpp/issues/566
add_subdirectory(lib/yaml-cpp-0.7.0)
target_link_libraries(Shimmerlight PRIVATE yaml-cpp)

# https://stackoverflow.com/questions/13429656/how-to-copy-contents-of-a-directory-into-build-directory-after-make-with-cmake
add_custom_command(TARGET Shimmerlight
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:Shimmerlight>/data
)
