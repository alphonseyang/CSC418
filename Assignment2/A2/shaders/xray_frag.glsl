precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

void main() {
  // Your solution should go here.
  // calculate opacity based on the normal orientation
  float EdgeFalloff = 1.0;
  float opacity = dot(normalize(normalInterp), viewVec);
  opacity = abs(opacity);
  opacity = 1.0 - pow(opacity, EdgeFalloff);
  
  // calculate the color for xray shading
  gl_FragColor = vec4(opacity * diffuseColor, 1.0);
}
