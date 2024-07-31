#version 330 core

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform vec3 uColor;

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
	vec3 eye = normalize(-f_in.position);
	float light = abs(dot(normalize(f_in.normal), eye));
	vec3 color = mix(uColor / 4, uColor, light);

	// output to the frambuffer
	fb_color = vec4(color, 1);

	/*
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightDir = normalize(vec3(-1.0f, -1.0f, -1.0f));
	float ambientStrength = 0.2;
	float diffuseStrength = 0.6;

	vec3 ambient = lightColor * ambientStrength;

	vec3 norm = normalize(f_in.normal);
    //vec3 lightDir = normalize(lightPos - f_in.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * diffuseStrength);

	vec3 result = ambient + diffuse;
	fb_color = vec4(result * uColor, 1);
	*/
}