VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

in vec3 blockColor;

void main() {
	FragColor = vec4(blockColor, 1.0);
}
)
