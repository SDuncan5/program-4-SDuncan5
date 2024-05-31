#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform sampler2D tex;
uniform sampler2D tex2;
uniform float day_night_ratio;

void main()
{
	vec4 tcol1 = texture(tex, vertex_tex);
	vec4 tcol2 = texture(tex2, vertex_tex);

	//color = tcol;
	color = tcol1 * day_night_ratio + tcol2 * (1 - day_night_ratio);
}
