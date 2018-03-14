attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;	// Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex
varying vec4 color;

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position in camera space


void main(){
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.

  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
  color = Ka * vec4(ambientColor, 1.0); 


  /* calculating the diffuse */
  vertPos = vec3(modelview * vec4(position, 1.0));
  // calculate the light vector 
  vec3 lightVec = normalize(vec3(lightPos - vertPos));
  // calculate the normal vector
  vec3 normalInterp = normalize(vec3(normalMat * vec4(normal, 0.0)));
  // calculate the diffuse intensity
  float diffuseIntensity = max(dot(lightVec, normalInterp), 0.0);
  // calculate the new color
  color = color + Kd * vec4(diffuseColor*diffuseIntensity, 1.0); 


  /* calculating the specular lighting */
  // calculate the view vector
  viewVec = normalize(-vertPos.xyz);
  // calculate reflect vector using reflect building in function
  vec3 reflectVec = reflect(-lightVec, normalInterp);
  float specularIntensity = pow(max(dot(reflectVec, viewVec), 0.0), shininessVal);
  // calculate the new color
  color = color + Ks * vec4(specularColor*specularIntensity, 1.0);
}