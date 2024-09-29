#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 tile_color;
out vec3 color;

uniform mat4 projection;
uniform mat4 view;

void main(){
    color = tile_color;
    gl_Position = projection * view * vec4(aPos,1.0f);
}
