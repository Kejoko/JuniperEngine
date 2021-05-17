#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
        GLSLC="bin/macOS/glslc"
else
        GLSLC="bin/linux/glslc"
fi

$GLSLC juniper/shaders/shader.vert -o juniper/shaders/vert.spv
$GLSLC juniper/shaders/shader.frag -o juniper/shaders/frag.spv