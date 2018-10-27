#version 330
in highp vec3 color;
in highp vec2 fpos;
out highp vec4 fragColor;
uniform sampler2D sampler;
void main()
{
   vec3 colorBack = texelFetch(sampler, ivec2(fpos.xy), 0).rgb;
   fragColor = vec4(color*colorBack, 1.0);
}
