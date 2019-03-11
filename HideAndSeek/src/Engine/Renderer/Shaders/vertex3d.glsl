#version 330 core

// Input vertex data, different for all executions of this shader.
in vec3 position;
//in vec3 in_normal;
//in vec2 in_texture;

out vec3 fragVertexColor;
//out vec3 v_world_normal;
//out vec3 v_to_camera;
//out vec3 v_to_light;
//out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 m_Projection;
uniform mat4 m_View;
uniform mat4 m_Model;
//uniform vec3 lightPosition;
uniform vec3 vertexColor;

void main(){	
  //vec4 v_world_position = m_Model * vec4(position, 1.0f);
  //gl_Position = m_Projection * m_View * v_world_position;

  gl_Position = m_Projection * m_View * m_Model * vec4(position, 1.0f);

  //v_world_normal = normalize((m_model * vec4(in_normal,0.0)).xyz); 
  //v_to_light = normalize(light_position - v_world_position.xyz);
  //v_to_camera =  normalize((inverse(m_view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - v_world_position.xyz);
  
  //UV = in_texture;

  fragVertexColor = vertexColor;
}

