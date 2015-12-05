#version 150
uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

in vec4 position;
out vec4 vertexColor;

void main(){
	gl_Position = projectionTransform * viewTransform * modelTransform * position;
	vertexColor = vec4(1.0, 1.0, 1.0, 0.0);
}
