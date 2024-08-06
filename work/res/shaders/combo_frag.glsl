#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;

uniform float roughness;
uniform float F0;  // 0 < F0 < 1
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

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Normal distribution function (GGX)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a2 = roughness * roughness;
    
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}



void main() {
	vec3 radiance = vec3(1.0f, 1.0f, 1.0f); //Light color
	vec3 lightPos = (uModelViewMatrix * vec4(10.0f, 10.0f, 10.0f, 1.0f)).xyz;
	vec3 lightDir = normalize(f_in.position - lightPos);

    vec3 L = -lightDir;
	vec3 N = normalize(f_in.normal);
    vec3 V = normalize(-f_in.position);
    vec3 H = normalize(V + L);
	float HdotV = max(dot(H, V), 0.0);
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float LdotH = max(dot(L, H), 0.0);
	float LdotV = max(dot(L, V), 0.0);



	// Calculate Fresnel factor
    vec3 F = fresnelSchlick(HdotV, vec3(F0));

	// Calculate normal distribution factor
    float D = DistributionGGX(N, H, roughness);

	float G = 1/(LdotH * LdotH + 0.001); // Kelemen approximation replaces geometry function and denominator cos terms with 1/(l.h)^2. added 0.001 to avoid divide by zero error. 

	// Cook-Torrance BRDF
    vec3 numerator = D * G * F;
    vec3 specular = numerator / 4.0;


    // Oren Nayar diffuse 
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

	// Calculate final specular color
    vec3 result = (specular + ON_diffuse) * radiance * NdotL * uColor;


	// Apply ambient light
    vec3 ambient = uColor * ka;
    result += ambient;

	fb_color = vec4(result, 1.0);
}