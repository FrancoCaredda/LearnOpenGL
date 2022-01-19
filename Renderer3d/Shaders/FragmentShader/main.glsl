#version 330 core

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 TextureCoordinates;

out vec4 FragColor;

#define DIRECTIONAL_LIGHT 0
#define STANDART_LIGHT 1
#define POINT_LIGHT 2
#define SPOT_LIGHT 3	
#define DEPTH_TEST 4

struct Light
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct Directionallight
{
	vec3 Direction;
	Light Properties;
};

struct Pointlight
{
	vec3 Position;

	Light Properties;

	float Constant;
	float Linear;
	float Quadratic;
};

struct Spotlight
{
	vec3 Position;
	vec3 Direction;

	Light Properties;

	float InnerRadius;
	float OuterRadius;
};

struct Material
{
	sampler2D Diffuse;
	sampler2D Specular;

	float Shininess;
};

uniform Directionallight u_DirectionLight;
uniform Pointlight u_Pointlight;
uniform Spotlight u_Spotlight;
uniform Material u_Material;

uniform int u_LightType;

uniform vec3 u_CameraPosition;

vec3 CalculateAmbient(float strength, Material material, Light light)
{
	return strength * texture(material.Diffuse, TextureCoordinates).rgb * light.Ambient;
}

vec3 CalculateDiffuse(vec3 direction, vec3 normal, Material material, Light light)
{
	return max(dot(direction, normal), 0)  * texture(material.Diffuse, TextureCoordinates).rgb * light.Diffuse;
}

vec3 CalculateSpecular(vec3 direction, vec3 normal, Material material, Light light)
{
	vec3 reflectedDirection = reflect(direction, normal);
	vec3 viewDirection = normalize(u_CameraPosition - FragmentPosition);

	return pow(max(dot(reflectedDirection, viewDirection), 0), material.Shininess) * texture(material.Specular, TextureCoordinates).rgb * light.Specular;
}

vec3 CalculatePointLight(vec3 ambient, vec3 diffuse, vec3 specular, Pointlight light)
{
	float dist = distance(light.Position, FragmentPosition);
	float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));
	
	return ambient * attenuation + diffuse * attenuation + specular * attenuation;
}

vec3 CalculateSpotlight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction, Spotlight light)
{
	float tetha = dot(normalize(light.Direction), direction);
	float intensity = clamp((tetha - light.OuterRadius) / (light.InnerRadius - light.OuterRadius), 0, 1);

	if (tetha > light.InnerRadius)
		return ambient + diffuse + specular;
	else if (tetha < light.InnerRadius && tetha > light.OuterRadius)
		return ambient + diffuse * intensity + specular * intensity;
	else
		return ambient;
}

void main()
{
	float near = 0.1;
	float far = 100.0;

	if (u_LightType != DEPTH_TEST)
	{
		vec3 direction;
		vec3 normal = normalize(Normal);
	
		if (u_LightType == SPOT_LIGHT)
			direction = normalize(u_Spotlight.Position - FragmentPosition);
		else
			direction = normalize(u_Pointlight.Position - FragmentPosition);

		vec3 ambient = CalculateAmbient(0.1, u_Material, u_Pointlight.Properties);
		vec3 diffuse;
		vec3 specular;

		if (u_LightType == DIRECTIONAL_LIGHT)
		{
			vec3 sunlightDirection = normalize(u_DirectionLight.Direction);

			diffuse = CalculateDiffuse(sunlightDirection, normal, u_Material, u_DirectionLight.Properties);
			specular = CalculateSpecular(sunlightDirection, normal, u_Material, u_DirectionLight.Properties);
		}
		else
		{
			diffuse = CalculateDiffuse(direction, normal, u_Material, u_Pointlight.Properties);
			specular = CalculateSpecular(direction, normal, u_Material, u_Pointlight.Properties);
		}

		if (u_LightType == POINT_LIGHT)
			FragColor = vec4(CalculatePointLight(ambient, diffuse, specular, u_Pointlight), 1.0);
		else if (u_LightType == SPOT_LIGHT)
			FragColor = vec4(CalculateSpotlight(ambient, diffuse, specular, direction, u_Spotlight), 1.0);
		else
			FragColor = vec4(ambient + diffuse + specular, 1.0);
	}
	else
	{
		float ndc = 2.0 * gl_FragCoord.z - 1.0;
		float linear = (2.0 * near * far) / (far + near - ndc * (far - near));
		FragColor = vec4(vec3(linear / far), 1.0);
	}
}