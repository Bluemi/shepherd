VISUALIZER_SHADER_STRINGIFY(
layout (location = 0) in vec3 aPos;

out vec3 norm;
out vec3 vertex_position;

uniform mat4 model;
uniform mat4 proj_view;
uniform vec3 player_position;

void main()
{
	gl_Position = proj_view * model * vec4(aPos, 1.0);
	norm = aPos;
	vertex_position = (model * vec4(aPos, 1.0)).xyz;
}
)
