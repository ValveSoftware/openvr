@ECHO OFF
for /r %%i in ("*.hlsl") Do (
	%VULKAN_SDK%\bin\glslangvalidator.exe -S vert -e VSMain -o %%~ni_vs.spv -V --hlsl-iomap --auto-map-bindings --shift-cbuffer-binding 0 --shift-texture-binding 1 --shift-sampler-binding 2 -D %%i
	%VULKAN_SDK%\bin\glslangvalidator.exe -S frag -e PSMain -o %%~ni_ps.spv -V --hlsl-iomap --auto-map-bindings --shift-cbuffer-binding 0 --shift-texture-binding 1 --shift-sampler-binding 2 -D %%i
)
