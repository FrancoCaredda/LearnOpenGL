#version 330 core

struct Light 
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

uniform Light u_Light;
uniform Material u_Material;

uniform vec3 u_CameraPosition;

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TextureCoordinates;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * texture(u_Material.diffuse, TextureCoordinates).rgb * u_Light.ambient;
	float specularStrength = 1;
	vec3 normal = normalize(Normal);
	vec3 direction = normalize(u_Light.position - FragmentPosition); // equation of phong lighting model defines dir vector from fragment to source
	vec3 viewDirection = normalize(u_CameraPosition - FragmentPosition); // equation of phong lighting model defines dir vector from fragment to source
	vec3 reflectDirection = reflect(direction, normal); // reflect works fine without inversing a direction vector; reflect -> v - 2 * dot(v, n) * n
	vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess) * texture(u_Material.specular, TextureCoordinates).rgb * u_Light.specular;

	vec3 diffuse = max(dot(normal, direction), 0.0) * texture(u_Material.diffuse, TextureCoordinates).rgb * u_Light.diffuse;

	gl_FragColor = vec4((ambient + diffuse + specular), 1.0);
}
