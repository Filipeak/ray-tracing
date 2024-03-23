# Ray Traacing

## Overview
Simple Ray Tracing system made in OpenGL. All logic is written in fragment shader: ```shaders/rt.frag```

## Features
- Ray Tracing Spheres (Path Tracing)
- Blinn-Phong Illumination
- Antialiasing (SSAA)

## Preview
Currently there is no preview :(

## Setup
Project is made in Visual Studio 2022, so just open ```.sln``` file

## TODO / Ideas (with Resources)
- **Ray Tracing**
  - [ ] Refraction (Snell's Law & Fresnel equations)
	- https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#/media/File:Ray_Tracing_Illustration_First_Bounce.png
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/adding-reflection-and-refraction.html
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel.html
	- https://github.com/engilas/raytracing-opengl/blob/master/assets/shaders/rt.frag
	- https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#Recursive_ray_tracing_algorithm
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-overview/light-transport-ray-tracing-whitted.html
  - [ ] PBR (Conservation of Energy, BRDF, Camera properties, Light Intensity - Lux, Candela, Lumen ...)
	- https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html
    - https://www.youtube.com/watch?v=7_iy4Rov2Ck&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
	- https://www.youtube.com/watch?v=AbVfW4X01a0&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=15
	- https://learnopengl.com/PBR/Theory
	- https://www.youtube.com/watch?v=XK_p2MxGBQs
	- https://en.wikipedia.org/wiki/Bidirectional_reflectance_distribution_function
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation.html
  - [ ] Metallic materials (Specular config)
	- https://www.youtube.com/watch?v=9RHGLZLUuwc
	- https://www.youtube.com/watch?v=yMRp9DVZYnI time=1:00
- **Lighting**
  - [ ] Shadow Rays (Directional light) - Backfaces (lighting inside)
	- https://github.com/carl-vbn/opengl-raytracing/blob/main/shaders/fragment.glsl#L236C6-L236C31
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows.html
  - [ ] Soft shadows
	- https://www.youtube.com/watch?v=A61S_2swwAc
	- https://medium.com/@alexander.wester/ray-tracing-soft-shadows-in-real-time-a53b836d123b
- **Post Processing**
   - [ ] Progressive Rendering / Accumulation (Monte Carlo), resize callback
    - https://www.youtube.com/watch?v=46ddlUImiQA&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
  	- https://www.youtube.com/watch?v=A61S_2swwAc
	- https://www.youtube.com/watch?v=QQ3jr-9Rc1o
  	- https://learnopengl.com/Advanced-OpenGL/Framebuffers
  	- https://github.com/carl-vbn/opengl-raytracing/blob/main/src/main.cpp
	- https://github.com/Eduard0110/Path-tracer-using-OpenGL/blob/main/source/Main.cpp
	- https://github.com/ishaanshah/raytracer/blob/main/src/main.cpp#L181
  - [ ] Screenshots - GUI Button
	- https://github.com/carl-vbn/opengl-raytracing/blob/main/src/main.cpp#L285
  - [ ] Gamma Correction
    - https://learnopengl.com/Advanced-Lighting/Gamma-Correction
  	- https://en.wikipedia.org/wiki/Gamma_correction
  	- https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/usinggammacorrectionforaccuratecolorintensity
	- https://www.youtube.com/watch?v=wFx0d9c8WMs
  - [ ] Bloom & HDR
    - https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model#Fragment_shader
    - https://learnopengl.com/Advanced-Lighting/Bloom
    - https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
	- https://www.youtube.com/watch?v=tI70-HIc5ro
	- https://learnopengl.com/Advanced-Lighting/HDR
	- https://www.youtube.com/watch?v=1U-jym5WADU
	- https://www.youtube.com/watch?v=iikdcAA7cww
- **Utility**
  - [ ] Premake / CMake
  - [ ] Acceleration structures - BVH Algorithm / Surface Subdivision / KD Tree
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies
	- https://www.khronos.org/blog/ray-tracing-in-vulkan#Acceleration_Structures
	- https://github.com/ishaanshah/raytracer/tree/main
  - [ ] Signed Distance Function (SDF) / Ray Marching
	- https://en.wikipedia.org/wiki/Signed_distance_function
	- https://www.youtube.com/watch?v=khblXafu7iA
  - [ ] Boxes, Quads
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html#quadrilaterals
  - [ ] Cornell Box
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html#volumes
  - [ ] FBX Import - Blender 3d Monkey
  - [ ] Change to Left Handed Coordinate System
  - [ ] Further OpenGL Abstraction
- **Technical**
  - [ ] DLSS
  - [ ] CUDA / OptiX
  - [ ] Compute Shaders
  - [ ] DirectX 11, 12
  - [ ] Vulkan Ray Tracing acceleration