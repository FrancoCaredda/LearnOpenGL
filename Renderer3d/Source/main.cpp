#include "GL/glew.h"
#include "glfw3.h"

#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Shaders/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image/stb_image.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>

int main(void)
{
	if (!glfwInit())
	{
		std::cerr << "[ERROR] " << __LINE__ << ": GLFW isn\'t initialized!";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1200, 900, "Lighting chapter", nullptr, nullptr);
	
	if (!window)
	{
		std::cerr << "[INFO] " << __LINE__ << ": Window isn\'t created!";
		return -2;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "[ERROR] " << __LINE__ << ": GLEW isn\'t initialized!";
		return -3;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	const char* glslVersion = "#version 330";

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glslVersion);
	
	std::vector<float> vertexData = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
	};
	std::vector<float> cubeVertexData = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	glm::vec3 cameraPosition = { 0.0f, 0.0f, -10.0f };
	glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPosition);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1200.0f / 900.0f, 0.1f, 100.0f);

	glm::vec3 lightColor = { 1.0f, 1.0f, 1.0f }; // 0.5f, 0.23f, 1.0f
	glm::vec3 objectColor = { 1.0f, 0.0f, 0.56f };

	////////////////////////LIGHT///////////////////////////////////////////////////////////////////


	VertexBuffer lightSourceVertexBuffer(GL_STATIC_DRAW);
	lightSourceVertexBuffer.Bind();
	lightSourceVertexBuffer.Allocate(vertexData);

	VertexArray lightSource;
	lightSource.Bind();

	lightSource.SetVertexAttributes({ {0, {3, 3, 0}} });
	lightSource.EnableVertexAttibutes();

	Shader lightVertexShader(GL_VERTEX_SHADER);
	Shader lightFragmentShader(GL_FRAGMENT_SHADER);

	lightVertexShader.Load("Shaders/VertexShader/lightSource.glsl");
	lightFragmentShader.Load("Shaders/FragmentShader/lightSource.glsl");

	lightVertexShader.Compile();
	lightFragmentShader.Compile();

	glm::vec4 lightPosition = { -1.0f, -1.0f, 2.0f, 1.0 };
	glm::mat4 modelLight = glm::translate(glm::mat4(1.0f), glm::vec3(lightPosition));
	modelLight = glm::scale(modelLight, { 0.5f, 0.5f, 0.5f });
//modelLight = glm::rotate(modelLight, glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	ShaderProgram lightProgram;
	lightProgram.Attach(lightVertexShader);
	lightProgram.Attach(lightFragmentShader);
	lightProgram.Link();
	lightProgram.Bind();

	lightProgram.SetMat4f("u_Model", modelLight);
	lightProgram.SetMat4f("u_View", view);
	lightProgram.SetMat4f("u_Projection", projection);

	lightProgram.SetVector3f("u_Color", lightColor);

	////////////////////////CUBE///////////////////////////////////////////////////////////////////

	
	VertexBuffer cubeVBO(GL_STATIC_DRAW);
	cubeVBO.Bind();
	cubeVBO.Allocate(cubeVertexData);

	VertexArray cubeVAO;
	cubeVAO.Bind();

	cubeVAO.SetVertexAttributes({ 
		{0, {3, 8, 0}},
		{1, {3, 8, 3}},
		{2, {2, 8, 6}}
	});
	cubeVAO.EnableVertexAttibutes();

	

	Shader cubeVertexShader(GL_VERTEX_SHADER);
	Shader cubeFragmentShader(GL_FRAGMENT_SHADER);

	cubeVertexShader.Load("Shaders/VertexShader/main.glsl");
	cubeFragmentShader.Load("Shaders/FragmentShader/main.glsl");

	cubeVertexShader.Compile();
	cubeFragmentShader.Compile();

	glm::vec3 cubePosition = { -2.0f, 1.0f, 1.0f };
	glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePosition);
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	ShaderProgram program;
	program.Attach(cubeVertexShader);
	program.Attach(cubeFragmentShader);
	program.Link();
	program.Bind();

	program.SetMat4f("u_Model", model);
	program.SetMat4f("u_View", view);
	program.SetMat4f("u_Projection", projection);

	int x, y, channels;

	stbi_set_flip_vertically_on_load(1);
	uint8_t* data = stbi_load("Texture/wood.png", &x, &y, &channels, 0);

	uint32_t textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	uint8_t* specularTextureData = stbi_load("Texture/wood_specular.png", &x, &y, &channels, 0);

	uint32_t specularTextureId;
	glGenTextures(1, &specularTextureId);
	glBindTexture(GL_TEXTURE_2D, specularTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, specularTextureData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(specularTextureData);

	program.SetVector3f("u_CameraPosition", cameraPosition);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	program.SetInt("u_Material.Diffuse", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTextureId);
	program.SetInt("u_Material.Specular", 1);


	float previousTime = 0.0f;
	float deltaTime = 0.0f;

	float dir = 1;

	float shininess = 4;

	int lightType = 1;
	float constant = 1.0, linear = 0.09f, quadratic = 0.032f;

	glm::vec3 spotlightDir(1.0f);
	float cutoffAngle = 0.0f;
	float outerCutoff = 0.0f;
	float degree = glm::radians(15.0f * deltaTime);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		deltaTime = glfwGetTime() - previousTime;
		previousTime += deltaTime;

		ImGui::Begin("Properties");
		ImGui::BeginChild("Lighting properties", {500, 250});
		ImGui::Text("Lighting properties");
		ImGui::DragFloat3("Light position", glm::value_ptr(lightPosition), 0.05, -55.0f, 55.0f);
		ImGui::SliderFloat3("Light color", glm::value_ptr(lightColor), 0, 1);
		ImGui::RadioButton("Directional Light", &lightType, 0);
		ImGui::RadioButton("Standart Light", &lightType, 1);
		ImGui::RadioButton("Pointlight", &lightType, 2);
		ImGui::RadioButton("Spotlight", &lightType, 3);
		ImGui::RadioButton("Depth", &lightType, 4);

		if (lightType == 2)
		{
			ImGui::SliderFloat("Linear Coeficient", &linear, 0.0014f, 0.7f);
			ImGui::SliderFloat("Quadratic Coeficient", &quadratic, 0.000007f, 1.8f);
		}
		else if (lightType == 3)
		{
			ImGui::SliderFloat3("Spotlight direction", glm::value_ptr(spotlightDir), -100.0f, 100.0f);
			ImGui::SliderFloat("Inner cutoff angle", &cutoffAngle, 0.0f, 90);
			ImGui::SliderFloat("Outer cutoff angle", &outerCutoff, 0.0f, 90);
		}

		ImGui::EndChild();
		ImGui::BeginChild("Object properties", { 500, 100 });
		ImGui::Text("Object properties");
		ImGui::SliderFloat("Object shininess", &shininess, 4, 256);
		ImGui::DragFloat3("Object position", glm::value_ptr(cubePosition), 0.05, -55.0f, 55.0f);
		ImGui::EndChild();
		ImGui::End();

		if (lightType != 0)
		{
			lightSource.Bind();
			lightSourceVertexBuffer.Bind();
			lightProgram.Bind();
			modelLight = glm::translate(glm::mat4(1.0), glm::vec3(lightPosition));
			modelLight = glm::scale(modelLight, { 0.5f, 0.5f, 0.5f });

			lightProgram.SetMat4f("u_Model", modelLight);
			lightProgram.SetVector3f("u_Color", lightColor);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		cubeVAO.Bind();
		cubeVBO.Bind();
		program.Bind();

		program.SetInt("u_LightType", lightType);

		model = glm::translate(glm::mat4(1.0), cubePosition);
		model = glm::rotate(model, degree, glm::vec3(0.0f, 1.0f, 0.0f));
		program.SetMat4f("u_Model", model);

		if (lightType == 0)
		{
			program.SetVector3f("u_DirectionLight.Direction", lightPosition);
			program.SetVector3f("u_DirectionLight.Properties.Ambient", lightColor);
			program.SetVector3f("u_DirectionLight.Properties.Diffuse", lightColor);
			program.SetVector3f("u_DirectionLight.Properties.Specular", lightColor);
		}
		else if (lightType == 3)
		{
			program.SetVector3f("u_Spotlight.Position", lightPosition);
			program.SetVector3f("u_Spotlight.Direction", lightPosition);
			program.SetVector3f("u_Spotlight.Properties.Ambient", lightColor);
			program.SetVector3f("u_Spotlight.Properties.Diffuse", lightColor);
			program.SetVector3f("u_Spotlight.Properties.Specular", lightColor);
			program.SetFloat("u_Spotlight.InnerRadius", glm::cos(glm::radians(cutoffAngle)));
			program.SetFloat("u_Spotlight.OuterRadius", glm::cos(glm::radians(outerCutoff)));
		}
		else
		{
			program.SetVector3f("u_Pointlight.Position", lightPosition);
			program.SetVector3f("u_Pointlight.Properties.Ambient", lightColor);
			program.SetVector3f("u_Pointlight.Properties.Diffuse", lightColor);
			program.SetVector3f("u_Pointlight.Properties.Specular", lightColor);
		}


		if (lightType == 2)
		{
			program.SetFloat("u_Pointlight.Constant", 1.0f);
			program.SetFloat("u_Pointlight.Linear", linear);
			program.SetFloat("u_Pointlight.Quadratic", quadratic);
		}

		program.SetFloat("u_Material.Shininess", shininess);

		degree += glm::radians(15.0f * deltaTime);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}