#version 330 core

struct Light 
{
    vec4 position;
	vec3 spotDirection;

	float spotCutOff;
	float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
	float linear;
	float quadratic;
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
in vec3 FragmentPosition; // We use our own fragment position because all lighting calculation must be done in world space if we use gl_FragCoord we will do all lighting
						  // calculation in screen space. In screen space lighting doesn't depend on position of lighting source and on position of object. We'll see static image that doesn't change with changing
						  // source position. It's because the position of each fragment is in range of [0, screenResolution]. gl_FragCoord doesn't apply any changes of transformations on vertex.
						  // If light source position is in world space and for calculating a direction vector we use gl_FragCoord, than we'll get a very strange vector that will point somewhere far in world, because
						  // gl_FragCoord has very big coord values
in vec2 TextureCoordinates;

float near = 0.1;
float far = 100.0;

out vec4 FragColor;

void main()
{
	if (u_Light.position.w != 4)
	{
		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * texture(u_Material.diffuse, TextureCoordinates).rgb * u_Light.ambient;
		float specularStrength = 1;
		vec3 normal = normalize(Normal);
		vec3 direction = vec3(0);
		vec3 reflectDirection = vec3(0);

		vec3 spotDir = normalize(u_Light.spotDirection);

		if (u_Light.position.w != 0) // Other than directional
		{
			direction = normalize(vec3(u_Light.position) - FragmentPosition); // equation of phong lighting model defines dir vector from fragment to source
			reflectDirection = reflect(vec3(direction), normal); // reflect works fine without inversing a direction vector; reflect -> v - 2 * dot(v, n) * n
		}
		else // Directional light
		{
			direction = normalize(vec3(-u_Light.position));
			reflectDirection = reflect(vec3(-direction), normal); // reflect works fine without inversing a direction vector; reflect -> v - 2 * dot(v, n) * n
		}

		vec3 viewDirection = normalize(u_CameraPosition - FragmentPosition); // equation of phong lighting model defines dir vector from fragment to source
		vec3 specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess) * texture(u_Material.specular, TextureCoordinates).rgb * u_Light.specular;
		vec3 diffuse = max(dot(direction, normal), 0.0) * texture(u_Material.diffuse, TextureCoordinates).rgb * u_Light.diffuse;

		if (u_Light.position.w == 2) // Pointlight
		{
			float distanceFrom = length(vec3(u_Light.position) - FragmentPosition);
			float F = 1.0 / (u_Light.constant + u_Light.linear * distanceFrom + u_Light.quadratic * distanceFrom * distanceFrom);
			specular *= F;
			ambient * F;
			diffuse *= F;
		}
	
		if (u_Light.position.w == 3) // Spotlight
		{
			float tetha = dot(spotDir, direction);
			float intensity = clamp((tetha - u_Light.outerCutOff) / (u_Light.spotCutOff - u_Light.outerCutOff), 0.0, 1.0);

			if (u_Light.spotCutOff < tetha)
				FragColor = vec4((ambient + diffuse + specular), 1.0);
			else if (tetha < u_Light.spotCutOff && tetha > u_Light.outerCutOff)
				FragColor = vec4((ambient + diffuse * intensity + specular * intensity), 1.0);
			else
				FragColor = vec4(ambient, 1.0);
		}
		else
		{
			FragColor = vec4((ambient + diffuse + specular), 1.0);
		}
	}
	else
	{
		float ndc = 2.0 * gl_FragCoord.z - 1.0;
		float linear = (2.0 * near * far) / (far + near - ndc * (far - near));
		FragColor = vec4(vec3(linear / far), 1.0);
	}
}
