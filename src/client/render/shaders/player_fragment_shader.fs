VISUALIZER_SHADER_STRINGIFY(
out vec4 FragColor;

in vec3 norm;

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
	if (norm.x / (abs(norm.y) + 0.1) > 1.78) {
		if (norm.x / (abs(norm.z) + 0.1) > 2.78) {
			FragColor = vec4(color*0.2, 1.0);
		} else {
			FragColor = vec4(0.3, 0.3, 0.3, 1.0);
		}
	} else {
		FragColor = vec4(color, 1.0);
	}
	// FragColor = vec4(color+norm.y*0.2, 1.0);
}
)
