#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 position;
in vec3 in_Normal;
//in vec2 in_Texture;

out vec3 fragVertexColor;
out vec3 v_WorldNormal;
out vec3 v_ToCamera;
out vec3 v_ToLight;
//out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 m_Projection;
uniform mat4 m_View;
uniform mat4 m_Model;
uniform vec3 lightPosition;
uniform vec3 vertexColor;

void main(){	
  vec4 v_WorldPosition = m_Model * vec4(position, 1.0f);

  gl_Position = m_Projection * m_View * v_WorldPosition;

  v_WorldNormal = normalize((m_Model * vec4(in_Normal,0.0)).xyz); 
  v_ToLight = normalize(lightPosition - v_WorldPosition.xyz);
  v_ToCamera =  normalize((inverse(m_View) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - v_WorldPosition.xyz);
  
  //UV = in_texture;

  fragVertexColor = vertexColor;
}

