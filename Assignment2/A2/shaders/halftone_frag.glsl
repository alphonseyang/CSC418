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

// HINT: Use the built-in variable gl_FragCoord to get the screen-space coordinates

void main() {
  // Your solution should go here.
  // Only the background color calculations have been provided as an example.


  /* calculate the diffuse */
  // calculate the light vector
  vec3 lightVec = normalize(lightPos - vertPos);
  // calculate the diffuse color using the normal and light vectors
  float diffuseIntensity = max(dot(normalInterp, lightVec), 0.0);
  vec3 diffColor = diffuseIntensity * diffuseColor;
  vec3 color = Ka * ambientColor + Kd * diffColor;

  float scale = color.r;
  vec2 pixel = floor(vec2(gl_FragCoord));
  // set the thinkness
  float thinkness = 10.0;
  float b = thinkness/2.0;
  if (mod((pixel.y), thinkness*2.0) > thinkness)
  	pixel.x += b;
  pixel = mod(pixel, vec2(thinkness));
  float a = distance(pixel, vec2(b)) / (thinkness * 0.65);

  a = scale + a;
  float balance = clamp(pow(a, 5.0), 0.0, 1.0);
  
  color = Ka * ambientColor + Kd * diffColor;

  gl_FragColor = vec4(color*balance, 1.0);
}