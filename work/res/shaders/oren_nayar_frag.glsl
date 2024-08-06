#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;

uniform float ka;
uniform float rho; //mixing ratio
uniform float albedo; 

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;

const float PI = 3.14159265359;



void main() {
	vec3 lightPos = (uModelViewMatrix * vec4(10.0f, 10.0f, 10.0f, 1.0f)).xyz;
	vec3 lightDir = normalize(f_in.position - lightPos);
    vec3 L = -lightDir;
	vec3 N = normalize(f_in.normal);
    vec3 V = normalize(-f_in.position);
    vec3 H = normalize(V + L);
	vec3 radiance = vec3(1.0f, 1.0f, 1.0f); //Light Color
    

    float LdotV = max(dot(L, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float NdotV = max(dot(N, V), 0.0f);

    float s = LdotV - NdotL * NdotV;
    float t;
    if( s<= 0) {
        t = 1.0f;
    } else {
        t = max(NdotL, NdotV);
    }
    float A = 1.0f/(PI + (PI/2 - 2.0/3.0) * rho);
    float B = rho/(PI + (PI/2 - 2.0/3.0) * rho);

    float ON_diffuse = albedo * (A + B * s/t);

    vec3 result = (ON_diffuse) * NdotL * radiance * uColor;


	// Apply ambient light
    vec3 ambient = uColor * ka;
    result += ambient;

	fb_color = vec4(result, 1.0);
}