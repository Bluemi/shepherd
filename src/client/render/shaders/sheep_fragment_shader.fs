VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

uniform vec3 player_position;

in vec3 sheepColor;
in vec3 vertex_position;

void main() {
	float transparency = clamp(distance(player_position, vertex_position)-0.2f, 0.0, 1.0);
	FragColor = vec4(sheepColor, transparency);
}
)
