#version 430 core


in vec2 TexCoords;

out vec4 FragColor;


uniform sampler2D u_ScreenTexture;
uniform float u_FilterRadius;


void main()
{
    float x = u_FilterRadius;
    float y = u_FilterRadius;

    // Take 9 samples around current texel:
    // a - b - c
    // d - e - f
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(u_ScreenTexture, vec2(TexCoords.x - x, TexCoords.y + y)).rgb;
    vec3 b = texture(u_ScreenTexture, vec2(TexCoords.x + 0, TexCoords.y + y)).rgb;
    vec3 c = texture(u_ScreenTexture, vec2(TexCoords.x + x, TexCoords.y + y)).rgb;
    vec3 d = texture(u_ScreenTexture, vec2(TexCoords.x - x, TexCoords.y + 0)).rgb;
    vec3 e = texture(u_ScreenTexture, vec2(TexCoords.x + 0, TexCoords.y + 0)).rgb;
    vec3 f = texture(u_ScreenTexture, vec2(TexCoords.x + x, TexCoords.y + 0)).rgb;
    vec3 g = texture(u_ScreenTexture, vec2(TexCoords.x - x, TexCoords.y - y)).rgb;
    vec3 h = texture(u_ScreenTexture, vec2(TexCoords.x + 0, TexCoords.y - y)).rgb;
    vec3 i = texture(u_ScreenTexture, vec2(TexCoords.x + x, TexCoords.y - y)).rgb;

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    vec3 upsampled = e * 4.0;
    upsampled += (b + d + f + h) * 2.0;
    upsampled += (a + c + g + i);
    upsampled *= 1.0 / 16.0;

    FragColor = vec4(upsampled, 1.0f);
}