#version 330 core

// Input data
in vec3 fragVertexColor;
//in vec3 v_world_normal;
//in vec3 v_to_camera;
//in vec3 v_to_light;
//in vec2 UV;

// Ouput data
out vec3 outputColor;

//uniform sampler2D model_texture;

void main()
{
	/*vec3 surface_reflection = normalize(reflect(-v_to_light, v_world_normal));
	
	float cosA = clamp(dot(surface_reflection, v_to_camera), 0.0, 1.0);
	float specular_factor = pow(cosA, 15);
	float shade_factor = max(dot(v_world_normal, v_to_light),0.1);

	vec3 object_color = shade_factor * texture( model_texture, UV ).rgb * (1.0 - specular_factor);
	vec3 specular_color = vec3(1.0, 1.0, 1.0) * specular_factor;

	color =  object_color + specular_color;*/

	outputColor = fragVertexColor;
}