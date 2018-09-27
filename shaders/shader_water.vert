

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform float time;

out vec3 interpNormal;
out vec3 pos;
void main()
{
    pos = vertexPosition;
    vec3 a;a = pos;a.y += sin(time * a.x) + cos(time + a.z);
	gl_Position = modelViewProjectionMatrix * vec4(a.xyz, 1.0);
	interpNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;

}
