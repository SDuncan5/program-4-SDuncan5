#version 330 core 

out vec4 color;

uniform vec3 MatAmb;
uniform vec3 MatDif;
uniform vec3 MatSpec;
uniform float MatShine;
uniform int flip;

//interpolated normal and light vector in camera space
in vec3 fragNor;
in vec3 lightDir;
//position of the vertex in camera space
in vec3 EPos;

void main()
{
	//you will need to work with these for lighting
	vec3 normal = normalize(fragNor);
	if (flip == 1) {
		normal = normal * -1.0f;
	}
	vec3 light = normalize(lightDir);

	float dC = clamp(dot(normal, light), 0, 1);
	//float dC = dot(normal, light);

	vec3 halfV = normalize(-1*EPos + lightDir);
	//vec3 halfV = normalize( lightDir - EPos);
	float sC = pow(clamp(dot(halfV, normal), 0, 1), MatShine);

	color = vec4(MatAmb + dC*MatDif +sC*MatSpec, 1.0);
	// color = vec4(MatAmb*10.0, 1.0);
	// should last val be a 1 or 0?
}
