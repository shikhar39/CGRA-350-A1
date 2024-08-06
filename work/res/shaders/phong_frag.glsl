#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;
uniform sampler2D ourTexture;

uniform float metallic;
uniform float roughness;
uniform float shininess;

// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;

void main() {
	// calculate lighting (hack)
	/*
	vec3 eye = normalize(-f_in.position);
	float light = abs(dot(normalize(f_in.normal), eye));
	vec3 color = mix(uColor / 4, uColor, light);

	// output to the frambuffer
	fb_color = vec4(color, 1);
	*/
	float ambientStrength = 0.3f;
	float diffuseStrength = 0.6f;
	float specularStrength = 0.2f;
	float shininess = 8.0f;

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDir = (uModelViewMatrix * vec4(normalize(vec3(-1.0f, -1.0f, -1.0f)), 0.0f)).xyz;

	//Ambient Light
	float ambient = ambientStrength;

	//Diffuse Light
	vec3 norm = normalize(f_in.normal);
    float diff = max(dot(norm, -lightDir), 0.0f);
    float diffuse = (diff * diffuseStrength);

	//Specular Light
	vec3 reflected = reflect(lightDir, norm);
	float specular = specularStrength * pow(max(dot(reflected, normalize(-f_in.position)), 0.0f), shininess);

	vec3 result = uColor * lightColor * (ambient + diffuse + specular);
	fb_color = vec4(result, 1.0f);
}