#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float metallic;
uniform float roughness;
uniform float shininess;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
    mat3 TBN;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	float ambientStrength = 0.3f;
	float diffuseStrength = 0.6f;
	float specularStrength = 0.2f;
	float shininess = 8.0f;

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDir = (uModelViewMatrix * vec4(normalize(vec3(1.0f, 1.0f, 1.0f)), 0.0f)).xyz;

	//Ambient Light
	float ambient = ambientStrength;

	//Diffuse Light
	vec3 norm = texture(texture1, f_in.textureCoord).xyz;
	vec3 texColor = texture(texture2, f_in.textureCoord).xyz;

    norm = norm * 2.0f - 1.0f;
    norm = normalize(f_in.TBN * norm); 

    // norm = f_in.normal;
    float diff = max(dot(norm, -lightDir), 0.0f);
    float diffuse = (diff * diffuseStrength);

	//Specular Light
	vec3 reflected = reflect(lightDir, norm);
	float specular = specularStrength * pow(max(dot(reflected, normalize(-f_in.position)), 0.0f), shininess);
	vec3 result =  texColor * lightColor * (ambient + diffuse + specular);
	fb_color = vec4(result, 1.0f);
}