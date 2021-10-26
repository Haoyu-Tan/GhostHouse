#version 150 core
//uniform sampler2D tex0;
in vec2 texcoord;
in vec3 Color;
out vec3 outColor;
void main(){
//outColor = vec4(texture(tex0,texcoord).rgb,1.0f);
outColor = Color.rgb
}