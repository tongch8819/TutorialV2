#version 330 core
in vec3 vWorldPos;
in vec3 vWorldN;
in vec2 vUV;

uniform vec3 uLightPos;

uniform bool uUseTexture;
uniform sampler2D uTex;

out vec4 FragColor;

void main(){
    vec3 N = normalize(vWorldN);
    vec3 L = normalize(uLightPos - vWorldPos);

    float diff = max(dot(N, L), 0.0);

    vec3 base = vec3(0.8, 0.8, 0.8);
    if (uUseTexture) {
        base = texture(uTex, vUV).rgb;
    }

    vec3 color = base * (0.2 + 0.8 * diff);
    FragColor = vec4(color, 1.0);
}
