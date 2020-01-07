VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

in vec3 sheepColor;

void main() {
	FragColor = vec4(sheepColor, 1.0);
}
)
