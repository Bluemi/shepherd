VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

uniform vec3 player_position;
uniform vec3 color;

in vec3 norm;
in vec3 vertex_position;

void main() {
	float transparency = clamp(distance(player_position, vertex_position)-0.2f, 0.0, 1.0);

	if (norm.x / (abs(norm.y) + 0.1) > 1.78) {
		if (norm.x / (abs(norm.z) + 0.1) > 2.78) {
			FragColor = vec4(color*0.2, transparency);
		} else {
			FragColor = vec4(0.3, 0.3, 0.3, transparency);
		}
	} else {
		FragColor = vec4(color, transparency);
	}
}
)
