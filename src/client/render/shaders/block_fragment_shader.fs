VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

in vec3 blockColor;

uniform vec3 color;

void main() {
	/*
	FragColor = vec4(
		color.x + (norm.y*0.05 + norm.z*0.015 + norm.x*0.012),
		color.y + (norm.y*0.05 + norm.z*0.013 + norm.x*0.017),
		color.z + (norm.y*0.05 + norm.z*0.011 + norm.x*0.019),
		1.0
	);
	*/
	FragColor = vec4(blockColor + color, 1.0);
}
)
