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
  // Your solution should go here
  // Only the ambient colour calculations have been provided as an example.
  gl_FragColor = vec4(ambientColor, 1.0);


  /* calculate diffuse */
  // calculate normal vector
  vec3 normalVec = normalize(normalInterp);
  // calculate light vector from light pos to vertex pos
  vec3 lightVec = normalize(lightPos - vertPos);
  float diffuseIntensity = max(dot(normalVec, lightVec), 0.0);
  vec3 diffColor = diffuseColor;
  // using dot product of normal and light to set different layers, lower value has darker color
  if (diffuseIntensity > 0.6)
    diffColor = diffColor;
  else if (diffuseIntensity > 0.30)
    diffColor = diffColor * 0.6;
  else if (diffuseIntensity > 0.10)
    diffColor = diffColor * 0.4;
  else
    diffColor = diffColor * 0.15;
  gl_FragColor = Ka * gl_FragColor + vec4(diffColor * Kd, 1.0);

  /* calculate specular lighting */
  // calculate reflect vector
  vec3 reflectVec = reflect(-lightVec, normalVec);
  float specularIntensity = max(dot(viewVec, reflectVec), 0.0);
  vec3 specColor = specularColor;
  // using dot product of view and reflection to set different layers
  if (specularIntensity > 0.97)
    specColor = specColor;  
  else
    specColor = specColor * pow(specularIntensity, shininessVal);
  gl_FragColor = Ka * gl_FragColor + vec4(specColor * Ks, 1.0);
}