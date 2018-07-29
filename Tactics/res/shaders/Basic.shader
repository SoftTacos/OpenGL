#shader_vertex
#version 330 core//probably use 450 later

layout(location = 0) in vec4 position;//the 0 should match up with the 0 in vertexAttribPointer
void main(){
	gl_Position = position;
};

#shader_fragment
#version 330 core//probably use 450 later

layout(location = 0) out vec4 color;
void main(){
	color = vec4(1.0, 0.1, 0.1, 1.0);//RGBA
};
