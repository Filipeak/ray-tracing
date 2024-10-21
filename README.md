# Ray Tracing

## Overview
Simple Ray Tracing system made in OpenGL, made as an experiment.

Main Ray Tracing logic is located in ```shaders/raytracing.frag```.

## Preview
![](https://github.com/Filipeak/ray-tracing/blob/main/assets/Preview.png)

## Features
- Ray tracing spheres and boxes (Path tracing)
- Antialiasing (SSAA)
- Moveable camera
- Image accumulation
- Bloom
- Adjusting rendering parameters at runtime

## Setup
Project is made in Visual Studio 2022, so just open ```.sln``` file

## Controls
W - Forward
A - Left
S - Backward
D - Right
Q - Down
E - Up
Mouse Right Click + Move - Camera rotation

## Next steps
While doing the project I found many advanced things which can also be done:
- Acceleration structures (BVH)
- FBX Importing
- Refraction (Fresnel equations)
- Physically Based Rendering (Rendering equation, BRDF)
- Importance sampling
- Volumetric Lighting, Clouds
- Textures
- Compute Shaders
- Other APIs support (DirectX, Vulkan)

## Resources
- https://raytracing.github.io/
- https://pbr-book.org/3ed-2018/contents
- https://www.scratchapixel.com/index.html
- https://www.youtube.com/watch?v=Qz0KTGYJtUk
- https://www.youtube.com/watch?v=C1H4zIiCOaI
- https://www.youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl