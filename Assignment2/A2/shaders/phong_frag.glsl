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
  // Only the ambient colour calculations have been provided as an example.
  gl_FragColor = vec4(ambientColor, 1.0);

  /* calculate Diffuse */
  // calculate the light vector
  vec3 lightVec = normalize(lightPos - vertPos);  
  float diffuseIntensity = max(dot(normalize(normalInterp), lightVec), 0.0);
  gl_FragColor = gl_FragColor * Ka + Kd * vec4(diffuseColor * diffuseIntensity, 1.0);
   
  /* calculate Specular Term */
  // calculate the refelct vector 
  vec3 reflectVec = normalize(-reflect(lightVec,normalInterp)); 
  float specularIntensity = pow(max(dot(reflectVec,viewVec),0.0),0.3*shininessVal);
  gl_FragColor =  gl_FragColor + Ks * vec4(specularColor * specularIntensity, 1.0); 
}
