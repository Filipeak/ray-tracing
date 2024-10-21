#version 430 core


in vec2 TexCoords;

out vec4 FragColor;


uniform sampler2D u_ScreenTexture;
uniform sampler2D u_BloomTexture;
uniform int u_BloomEnabled;
uniform float u_BloomStrength;
uniform float u_HdrExposure;
uniform float u_Gamma;


void main()
{
    vec3 hdrColor = texture(u_ScreenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(u_BloomTexture, TexCoords).rgb;
    vec3 pixelColor = u_BloomEnabled == 1 ? mix(hdrColor, bloomColor, u_BloomStrength) : hdrColor;


    /**
     * REF: https://learnopengl.com/Advanced-Lighting/HDR
     * REF: https://www.youtube.com/watch?v=1U-jym5WADU
     */
    pixelColor = vec3(1.0f) - exp(-pixelColor * u_HdrExposure);


    /**
     * REF: https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/usinggammacorrectionforaccuratecolorintensity
     * REF: https://learnopengl.com/Advanced-Lighting/Gamma-Correction
     * REF: https://www.youtube.com/watch?v=iikdcAA7cww
     */
    pixelColor = pow(pixelColor, vec3(1.0f / u_Gamma));


    FragColor = vec4(pixelColor, 1.0f);
}