#version 150 core

in vec3 Color;
in vec3 vertNormal;
in vec3 pos;
//in vec3 lightDir;

/////
in vec3 tp; // torch position
in vec3 tmdir;
/////
in vec3 pp[15];


in vec3 aColor;
in vec3 dColor;
in vec3 sColor;

uniform int pn;
uniform vec3 fc;

////
in vec2 texcoord;



out vec4 outColor;




uniform sampler2D tex0;
uniform sampler2D tex1;

uniform int texID;
uniform int tStatus; // torch light status

const vec2 tangle = vec2(0.5/180.0*3.14,15.0/180.0*3.14);
const float tlight = 50.0f;
const float plight = 1.5f;
const float ambient = .1;
void main() {
  vec3 color;
  if (texID == -1)
    color = vec3(0.0,0.0,0.0);
  else if (texID == 0){
    color = texture(tex0, texcoord).rgb;
    //outColor = color;
    //return;
    }
  //else if (texID == 1)
   // color = texture(tex1, texcoord).rgb;  
   else if (texID == 1)
      color = vec3(0.0, 0.0, 0.0);
   else if (texID == 2){
      outColor = vec4(aColor.xyz,0.5f);
      return;
    }
   else if(texID == 3){
      vec3 oc = 0.05f * sColor + 0.5f*dColor + 0.2f*aColor;
      outColor = vec4(oc.xyz,1.0f);
      return;
}
  else{
    outColor = vec4(1,0,0,1);
    return; //This was an error, stop lighting!
  }
 
  vec3 tldir = (pos - tp);
  //vec3 tldir = pos;
  float tdist = pow(length(tldir),2);
  tldir = normalize(tldir);
  float ta = abs(acos(dot(normalize(tldir),normalize(tmdir))));
  
  float tr = 1.0f;
  if (ta > tangle.y){
	 tr = 0.0f;
}
  else if (ta > tangle.x){
      tr = 1.0f - (ta - tangle.x) / (tangle.y - tangle.x);
}
  


  vec3 normal = normalize(vertNormal);

  vec3 ambC = aColor*ambient;
 vec3 oColor = ambC;
vec3 viewDir = normalize(-pos); //We know the eye is at (0,0)! (Do you know why?)
  vec3 reflectDir = reflect(viewDir,normal);
 if(tStatus == 1){
  vec3 diffuseC = dColor*max(dot(-tldir,normal),0.0) * tr * tlight / tdist;
  
  
  float spec = max(dot(reflectDir,tldir),0.0);
  if (dot(-tldir,normal) <= 0.0) spec = 0; //No highlight if we are not facing the light
  vec3 specC = sColor*pow(spec,16) * tr * tlight / tdist;
 
  oColor = oColor + diffuseC+specC;
}  
for(int i = 0; i < pn; i++){

   vec3 pldir = (pos - pp[i]);
   float tdist1 = pow(length(pldir),2);
   pldir = normalize(pldir);
    vec3 diffuse = dColor*max(dot(-pldir,normal),0.0) * plight / tdist1;
    float spec1 = max(dot(reflectDir,pldir),0.0);
   if (dot(-pldir,normal) <= 0.0) spec1 = 0; 
    vec3 specular = sColor*pow(spec1,16) * plight / tdist1;
  oColor = oColor + diffuse+ specular;
}

  
  outColor = vec4(oColor.x*fc.x,oColor.y*fc.y,oColor.z*fc.z,1);
}