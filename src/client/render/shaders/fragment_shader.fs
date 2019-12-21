VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

in vec3 norm;

uniform vec3 color;

void main()
{
	FragColor = vec4(color + (norm.x + norm.y + norm.z)*0.1, 1.0);
}
)
