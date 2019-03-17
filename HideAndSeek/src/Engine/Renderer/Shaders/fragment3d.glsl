#version 330 core

// Input data
in vec3 fragVertexColor;
in vec3 v_WorldNormal;
in vec3 v_ToCamera;
in vec3 v_ToLight;
//in vec2 UV;

// Ouput data
out vec3 outputColor;

//uniform sampler2D model_texture;

void main()
{
	vec3 surfaceReflection = normalize(reflect(-v_ToLight, v_WorldNormal));
	
	float cosA = clamp(dot(surfaceReflection, v_ToCamera), 0.0, 1.0);
	float specularFactor = pow(cosA, 15);
	float shadeFactor = max(dot(v_WorldNormal, v_ToLight),0.1);

	//When texturing
	//vec3 objectColor = shadeFactor * texture( model_texture, UV ).rgb * (1.0 - specularFactor);

	vec3 objectColor = shadeFactor * fragVertexColor * (1.0 - specularFactor);
	vec3 specularColor = vec3(1.0, 1.0, 1.0) * specularFactor;

	outputColor =  objectColor + specularColor;

}