# Ray Traacing

## Overview
Simple Ray Tracing system made in OpenGL. All logic is written in fragment shader

## Preview

## Features
 - Ray Tracing Spheres

## Setup
Project is made in Visual Studio 2022, so just open ```.sln``` file

## TODO
 - [ ] Framebuffers, Uniform Buffers, GLSL Hot Reload
 - [ ] Camera / Scene abstraction
 - [ ] ImGUI
 - [ ] RenderDoc / NVIDIA Nsight debugging
	- https://www.youtube.com/watch?v=TEKYu00ca0E
 - [ ] Ray Tracer design (shaders functions, "t" in ray payload) & Backface culling (Multiple "t" values)
    - https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/frontfacesversusbackfaces
	 
 - [ ] Ray Tracing
  	- https://www.youtube.com/watch?v=A61S_2swwAc
	- https://www.youtube.com/watch?app=desktop&si=luM0kHic_EdapEET&v=kHNewYRvgSk
  	- https://raytracing.github.io/books/RayTracingInOneWeekend.html
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html
	- https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html
 - [ ] Illumination & Reflections (Bling-Phong Model, Diffuse, Specular) & Light Scattering & Refraction (Snell's Law)
	- https://www.youtube.com/watch?v=YnBhJbQZLuE&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=23
	- https://www.youtube.com/watch?v=e-lnyzN2wrM&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=24
	- https://www.youtube.com/watch?v=dJo1Ao9XydM&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=25
	- https://www.youtube.com/watch?v=ToCSRyXva5w&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=26
	- https://www.youtube.com/watch?v=MAJqiDll0a8&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=27
	- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
	- https://learnopengl.com/Lighting/Basic-Lighting
	- https://en.wikipedia.org/wiki/Ray_tracing_(graphics)#/media/File:Ray_Tracing_Illustration_First_Bounce.png
	- https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/introduction-global-illumination-path-tracing.html
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/phong-shader-BRDF/phong-illumination-models-brdf.html
 - [ ] Path Tracing
	- https://www.youtube.com/watch?v=NIpC53vesHo&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=12
	- https://en.wikipedia.org/wiki/Path_tracing
 - [ ] PBR (Conservation of Energy, BRDF, ...)
    - https://www.youtube.com/watch?v=7_iy4Rov2Ck&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=10
	- https://www.youtube.com/watch?v=AbVfW4X01a0&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=15
	- https://learnopengl.com/PBR/Theory
	- https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom
	- https://www.youtube.com/watch?v=XK_p2MxGBQs
 - [ ] Progressive Rendering / Accumulation (Monte Carlo)
    - https://www.youtube.com/watch?v=46ddlUImiQA&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=11
	- https://www.youtube.com/watch?v=A61S_2swwAc
 - [ ] Antialiasing, Soft shadows
	- https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
	- https://raytracing.github.io/books/RayTracingInOneWeekend.html#antialiasing
	- https://www.youtube.com/watch?v=A61S_2swwAc
 - [ ] Gamma Correction
	- https://learnopengl.com/Advanced-Lighting/Gamma-Correction
	- https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/usinggammacorrectionforaccuratecolorintensity
	- https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model#Fragment_shader
 - [ ] Bloom
 - [ ] BVH Algorithm / Surface Subdivision / KD Tree
	- https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-acceleration-structure/bounding-volume.html
	- https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies
	- https://www.khronos.org/blog/ray-tracing-in-vulkan#Acceleration_Structures
 - [ ] CUDA / OptiX - Path Tracing, Mesh Ray Tracing
 - [ ] DirectX 11, 12 / Vulkan

## Resources
1. https://raytracing.github.io/
2. https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl
3. https://www.youtube.com/playlist?list=PL5B692fm6--sgm8Uiava0IIvUojjFOCSR