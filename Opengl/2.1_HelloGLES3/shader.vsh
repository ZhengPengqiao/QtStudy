#version 330
layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 col;
out vec3 color;
out vec2 fpos;
uniform mat4 projMatrix;
void main()
{
   ivec2 pos = ivec2(gl_InstanceID % 32, gl_InstanceID / 32);
   fpos = vec2(pos.xy);
   color = col.rgb;
   gl_Position = projMatrix * vec4(vertex.x + pos.x-16, vertex.y+pos.y-16, vertex.z, vertex.w);
}

