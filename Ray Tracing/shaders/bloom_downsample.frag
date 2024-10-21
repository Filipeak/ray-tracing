#version 430 core


in vec2 TexCoords;

out vec4 FragColor;


uniform sampler2D u_ScreenTexture;
uniform vec2 u_Resolution;


void main()
{
    vec2 srcTexelSize = 1.0f / u_Resolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(u_ScreenTexture, vec2(TexCoords.x - 2 * x, TexCoords.y + 2 * y)).rgb;
    vec3 b = texture(u_ScreenTexture, vec2(TexCoords.x, TexCoords.y + 2 * y)).rgb;
    vec3 c = texture(u_ScreenTexture, vec2(TexCoords.x + 2 * x, TexCoords.y + 2 * y)).rgb;
    vec3 d = texture(u_ScreenTexture, vec2(TexCoords.x - 2 * x, TexCoords.y)).rgb;
    vec3 e = texture(u_ScreenTexture, vec2(TexCoords.x, TexCoords.y)).rgb;
    vec3 f = texture(u_ScreenTexture, vec2(TexCoords.x + 2 * x, TexCoords.y)).rgb;
    vec3 g = texture(u_ScreenTexture, vec2(TexCoords.x - 2 * x, TexCoords.y - 2 * y)).rgb;
    vec3 h = texture(u_ScreenTexture, vec2(TexCoords.x, TexCoords.y - 2 * y)).rgb;
    vec3 i = texture(u_ScreenTexture, vec2(TexCoords.x + 2 * x, TexCoords.y - 2 * y)).rgb;
    vec3 j = texture(u_ScreenTexture, vec2(TexCoords.x - x, TexCoords.y + y)).rgb;
    vec3 k = texture(u_ScreenTexture, vec2(TexCoords.x + x, TexCoords.y + y)).rgb;
    vec3 l = texture(u_ScreenTexture, vec2(TexCoords.x - x, TexCoords.y - y)).rgb;
    vec3 m = texture(u_ScreenTexture, vec2(TexCoords.x + x, TexCoords.y - y)).rgb;

    vec3 downsampled = e * 0.125;;
    downsampled += (a + c + g + i) * 0.03125;
    downsampled += (b + d + f + h) * 0.0625;
    downsampled += (j + k + l + m) * 0.125;
    downsampled = max(downsampled, 0.0001f);

    FragColor = vec4(downsampled, 1.0f);
}