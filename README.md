# Ray Traacing

## Overview
Simple Ray Tracing system made in OpenGL. All logic is written in fragment shader

## Features
- Ray Tracing Spheres
- Blinn-Phong Illumination
- Antialiasing (SSAA)

## Preview
Currently there is no preview :(

## Setup
Project is made in Visual Studio 2022, so just open ```.sln``` file

## TODO / Ideas (with Resources)
- [ ] Random
- [ ] Framebuffers
- [ ] Shadows
- [ ] CHECK t distance (normalize, length, etc...)

- **Ray Tracing**
  - [X] Reflections
	- https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#/media/File:Ray_Tracing_Illustration_First_Bounce.png
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction.html
  - [ ] Refraction (Snell's Law)
	- https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#/media/File:Ray_Tracing_Illustration_First_Bounce.png
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction.html
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel.html
  - [ ] Path Tracing
	- https://en.wikipedia.org/wiki/Path_tracing
	- https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#Recursive_ray_tracing_algorithm
	- https://raytracing.github.io/books/RayTracingInOneWeekend.html
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html
	- https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html
	- https://www.youtube.com/watch?v=NIpC53vesHo&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=12
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation.html
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/introduction-global-illumination-path-tracing.html
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-overview/light-transport-ray-tracing-whitted.html
  - [ ] PBR (Conservation of Energy, BRDF, Camera properties, Light Intensity - Lux, Candela, Lumen ...)
    - https://www.youtube.com/watch?v=7_iy4Rov2Ck&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
	- https://www.youtube.com/watch?v=AbVfW4X01a0&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=15
	- https://learnopengl.com/PBR/Theory
	- https://www.youtube.com/watch?v=XK_p2MxGBQs
  - [ ] Emissive materials
  - [ ] Metallic materials (Specular config)
	- https://www.youtube.com/watch?v=9RHGLZLUuwc
- **Lighting**
  - [X] Illumination (Blinn-Phong Model - Ambient, Diffuse, Specular)
    - https://www.youtube.com/watch?v=YnBhJbQZLuE&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=23
  	- https://www.youtube.com/watch?v=e-lnyzN2wrM&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=24
  	- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
  	- https://learnopengl.com/Lighting/Basic-Lighting
  	- https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
  	- https://learnopengl.com/Lighting/Materials
  	- https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF/phong-illumination-models-brdf.html
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-overview/light-transport-ray-tracing-whitted.html
  - [X] Point Lights
  - [X] Directional Lights
  - [X] Multiple Lights
    - https://learnopengl.com/Lighting/Multiple-lights
	- https://docs.unity3d.com/Manual/Lighting.html
	- https://github.com/carl-vbn/opengl-raytracing/blob/main/shaders/fragment.glsl
	- https://www.youtube.com/watch?v=ToCSRyXva5w&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=25
  - [X] Light Attenuation & Power
	- https://learnopengl.com/Lighting/Light-casters
  - [ ] Shadow Rays (Directional light) - Backfaces (lighting inside)
	- https://github.com/carl-vbn/opengl-raytracing/blob/main/shaders/fragment.glsl#L236C6-L236C31
  - [ ] Soft shadows
	- https://www.youtube.com/watch?v=A61S_2swwAc
	- https://medium.com/@alexander.wester/ray-tracing-soft-shadows-in-real-time-a53b836d123b
- **Post Processing**
  - [ ] Gamma Correction
    - https://learnopengl.com/Advanced-Lighting/Gamma-Correction
  	- https://en.wikipedia.org/wiki/Gamma_correction
  	- https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/usinggammacorrectionforaccuratecolorintensity
  - [ ] Bloom
    - https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model#Fragment_shader
    - https://learnopengl.com/Advanced-Lighting/Bloom
    - https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
  - [ ] HDR
	- https://learnopengl.com/Advanced-Lighting/HDR
  - [X] Antialiasing - SSAA
    - https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing
	- https://en.wikipedia.org/wiki/Supersampling
- **Utility**
  - [ ] Progressive Rendering / Accumulation (Monte Carlo), Maybe add postprocessing shader?
    - https://www.youtube.com/watch?v=46ddlUImiQA&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
  	- https://www.youtube.com/watch?v=A61S_2swwAc
  	- https://learnopengl.com/Advanced-OpenGL/Framebuffers
  	- https://github.com/carl-vbn/opengl-raytracing/blob/main/src/main.cpp
  - [X] Backface culling (Multiple "t" values)
    - https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/frontfacesversusbackfaces
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/single-vs-double-sided-triangle-backface-culling.html
	- https://stackoverflow.com/questions/20184039/ray-tracing-with-sphere
  - [X] Camera (Clipping, Viewport, Objects stretching, FOV)
  	- https://learnopengl.com/Getting-started/Camera
	- https://www.youtube.com/watch?v=lXlXqUEEJ94&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=6
	- https://github.com/TheCherno/RayTracing/blob/master/RayTracing/src/Camera.cpp
	- https://stackoverflow.com/questions/2354821/raycasting-how-to-properly-apply-a-projection-matrix
	- https://gamedev.stackexchange.com/questions/112673/inverse-of-perspective-matrix-for-what
  	- https://www.youtube.com/watch?v=86_pQCKOIPk
  	- https://learnopengl.com/Getting-started/Coordinate-Systems
  	- https://www.youtube.com/watch?v=MAJqiDll0a8&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=27
	- https://raytracing.github.io/books/RayTracingInOneWeekend.html#positionablecamera
  - [ ] New Random System
	- https://www.reedbeta.com/blog/quick-and-easy-gpu-random-numbers-in-d3d11/
	- https://www.reedbeta.com/blog/hash-functions-for-gpu-rendering/
	- https://www.youtube.com/watch?v=5_RAHZQCPjE
  - [ ] Acceleration structures - BVH Algorithm / Surface Subdivision / KD Tree
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies
	- https://www.khronos.org/blog/ray-tracing-in-vulkan#Acceleration_Structures
  - [ ] Denoising
	- https://www.youtube.com/watch?v=6O2B9BZiZjQ
  - [ ] Premake / CMake
  - [ ] Boxes
  - [ ] Signed Distance Function (SDF)
	- https://en.wikipedia.org/wiki/Signed_distance_function
- **Technical**
  - [ ] DLSS
  - [ ] CUDA / OptiX
  - [ ] Compute Shaders
  - [ ] DirectX 11, 12 / Vulkan