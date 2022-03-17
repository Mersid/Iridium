cmake_minimum_required (VERSION 3.8)

project (Shimmerlight)
set(CMAKE_CXX_STANDARD 17) # Using C++20 breaks Eigen technically. Still compiles, just gets some IDE false errors.

add_executable(Shimmerlight src/Main.cpp src/Shimmerlight.cpp src/Shimmerlight.h src/texture/StbImplementation.cpp src/texture/StbImplementation.h src/texture/Texture.cpp src/texture/Texture.h src/texture/TextureSerializer.cpp src/texture/TextureSerializer.h src/geometry/Parallelogram.cpp src/geometry/Parallelogram.h src/geometry/Ray.cpp src/geometry/Ray.h src/scene/Camera.cpp src/scene/Camera.h src/scene/Scene.cpp src/scene/Scene.h src/scene/CameraMode.h src/scene/Light.cpp src/scene/Light.h src/geometry/Primitive.h src/geometry/Sphere.cpp src/geometry/Sphere.h src/noise/PerlinNoiseSampler.cpp src/noise/PerlinNoiseSampler.h src/geometry/Material.cpp src/geometry/Material.h src/noise/PerlinFilterMode.h src/texture/PerlinTextureGenerator.cpp src/texture/PerlinTextureGenerator.h src/texture/CheckerboardTextureGenerator.cpp src/texture/CheckerboardTextureGenerator.h src/geometry/Triangle.cpp src/geometry/Triangle.h src/geometry/OffSerializer.cpp src/geometry/OffSerializer.h src/utils/StringUtils.cpp src/utils/StringUtils.h src/geometry/Mesh.cpp src/geometry/Mesh.h src/geometry/Model.cpp src/geometry/Model.h src/geometry/Box.cpp src/geometry/Box.h src/geometry/BoundingVolumeHierarchy.cpp src/geometry/BoundingVolumeHierarchy.h src/geometry/RayTraceInfo.cpp src/geometry/RayTraceInfo.h)
target_include_directories(Shimmerlight PUBLIC lib/eigen)
target_include_directories(Shimmerlight PUBLIC lib/stb)

# https://stackoverflow.com/questions/13429656/how-to-copy-contents-of-a-directory-into-build-directory-after-make-with-cmake
add_custom_command(TARGET Shimmerlight
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:Shimmerlight>/data
)
