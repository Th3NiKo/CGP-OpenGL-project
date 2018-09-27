#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;

in vec3 interpNormal;
in vec3 pos;
in vec2 CoordMap;
uniform sampler2D samp;

void main()
{
	vec3 normal = normalize(interpNormal);
	vec2 mapa = CoordMap;
    mapa.y *= -1;
	float diffuse = max(dot(normal, abs(normalize(pos))), 0.0);
	vec4 texC = texture2D(samp, CoordMap);
	gl_FragColor = texC;
}
