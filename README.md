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
  - [ ] Shadow Rays
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
  - [X] Antialiasing - SSAA
    - https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing
	- https://en.wikipedia.org/wiki/Supersampling
  - [ ] Antialiasing - MSAA
	- https://en.wikipedia.org/wiki/Multisample_anti-aliasing
  - [ ] Antialiasing - FXAA
    - https://en.wikipedia.org/wiki/Fast_approximate_anti-aliasing
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
  - [ ] Camera (Clipping, Viewport, Objects stretching, FOV)
  	- https://learnopengl.com/Getting-started/Camera
  	- https://www.glfw.org/docs/3.3/input_guide.htmlq
  	- https://www.youtube.com/watch?v=86_pQCKOIPk
  	- https://learnopengl.com/Getting-started/Coordinate-Systems
  	- https://learnopengl.com/Getting-started/Camera
  	- https://www.youtube.com/watch?v=MAJqiDll0a8&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=27
	- https://stackoverflow.com/questions/15654267/ray-tracer-objects-stretch-when-off-center
	- https://raytracing.github.io/books/RayTracingInOneWeekend.html#positionablecamera
  - [ ] Acceleration structures - BVH Algorithm / Surface Subdivision / KD Tree
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies
	- https://www.khronos.org/blog/ray-tracing-in-vulkan#Acceleration_Structures
  - [ ] Denoising
	- https://www.youtube.com/watch?v=6O2B9BZiZjQ
  - [ ] Premake / CMake
- **Technical**
  - [ ] DLSS
  - [ ] CUDA / OptiX - Path Tracing, Mesh Ray Tracing
  - [ ] DirectX 11, 12 / Vulkan