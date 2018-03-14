// Fragment shader template for the bonus question

precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
// NOTE: You may need to edit this section to add additional variables
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
// NOTE: You may need to edit this section to add additional variables
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position in camera space

uniform sampler2D uSampler;	// 2D sampler for the earth texture

void main() {
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.

  /* calculate opacity */
  // calculate normal vector from normal interpolation
  vec3 normalVec = normalize(normalInterp);
  // assume edge thinkness
  float EdgeFalloff = 5.0;
  float opacity = dot(normalVec, viewVec);
  opacity = abs(opacity);
  opacity = 1.0 - pow(opacity, EdgeFalloff);

  /* calculate diffuse */
  vec3 lightVec = normalize(lightPos - vertPos);
  float diffuse = max(dot(lightVec, normalVec), 0.0);

  // set final color by combining opacity with ambient and diffuse color
  gl_FragColor = vec4(opacity * (Ka * ambientColor + Kd * diffuse * diffuseColor), 1.0);
}
