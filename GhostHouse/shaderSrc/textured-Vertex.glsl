#version 150 core

in vec3 position;

const vec3 inLightDir = normalize(vec3(-1,1,-1));
in vec3 inNormal;
in vec2 inTexcoord;

in float Ns;
in vec3 Ka;
in vec3 Kd;
in vec3 Ks;
in vec3 Ke;
in float Ni;
in float d;
in float illum;

out vec3 Color;
out vec3 vertNormal;
out vec3 pos;
out vec2 texcoord;

out vec3 tp;
out vec3 tmdir;
out vec3 pp[15];

out vec3 aColor;
out vec3 dColor;
out vec3 sColor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 inColor;

uniform vec3 tpos;
uniform vec3 tdir;
uniform int pnum;
uniform vec3 ppos[15];


void main() {
   Color = inColor;
   aColor = Ka;
   dColor = Kd;
   sColor = Ks;
   gl_Position = proj * view * model * vec4(position,1.0);
   pos = (view * model * vec4(position,1.0)).xyz;
   tp =  (view * vec4(tpos,1.0)).xyz;
  
   for(int i = 0; i < pnum; i++){
           pp[i] = (view*vec4(ppos[i],1.0)).xyz;
    }
  
   //pp = (view *vec4(-12.000000, 0.300000, 14.000000,1.0)).xyz;
   tmdir = (view * vec4(tdir,0.0)).xyz;
   //lightDir = (view * vec4(inLightDir,0.0)).xyz; //It's a vector!
   vec4 norm4 = transpose(inverse(view*model)) * vec4(inNormal,0.0);
   vertNormal = normalize(norm4.xyz);
   texcoord = inTexcoord;
}