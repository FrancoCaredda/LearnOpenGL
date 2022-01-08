#include "GL/glew.h"
#include "glfw3.h"

#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Shaders/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

	glm::vec3 lightPosition = { -1.0f, -1.0f, 2.0f };
	glm::mat4 modelLight = glm::translate(glm::mat4(1.0f), lightPosition);
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
		{0, {3, 6, 0}},
		{1, {3, 6, 3}}
	});
	cubeVAO.EnableVertexAttibutes();

	Shader cubeVertexShader(GL_VERTEX_SHADER);
	Shader cubeFragmentShader(GL_FRAGMENT_SHADER);

	cubeVertexShader.Load("Shaders/VertexShader/rectangle.glsl");
	cubeFragmentShader.Load("Shaders/FragmentShader/rectangle.glsl");

	cubeVertexShader.Compile();
	cubeFragmentShader.Compile();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), {-2.0f, 1.0f, 1.0f});
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	ShaderProgram program;
	program.Attach(cubeVertexShader);
	program.Attach(cubeFragmentShader);
	program.Link();
	program.Bind();

	program.SetMat4f("u_Model", model);
	program.SetMat4f("u_View", view);
	program.SetMat4f("u_Projection", projection);

	program.SetVector3f("u_Material.ambient", { 1.0f, 0.5f, 0.31f });
	program.SetVector3f("u_Material.diffuse", { 1.0f, 0.5f, 0.31f });
	program.SetVector3f("u_Material.specular", { 0.5f, 0.5f, 0.5f });
	program.SetFloat("u_Material.shininess", 256);
	program.SetVector3f("u_Light.position", lightPosition);
	program.SetVector3f("u_Light.ambient", { 1.0f, 1.0f, 1.0f });
	program.SetVector3f("u_Light.diffuse", { 1.0f, 1.0f, 1.0f });
	program.SetVector3f("u_Light.specular", { 1.0f, 1.0f, 1.0f });
	program.SetVector3f("u_CameraPosition", cameraPosition);

	float previousTime = 0.0f;
	float deltaTime = 0.0f;

	float dir = 1;

	float shininess = 4;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		deltaTime = glfwGetTime() - previousTime;
		previousTime += deltaTime;

		ImGui::Begin("Properties");
		ImGui::BeginChild("Lighting properties", {500, 50});
		ImGui::DragFloat3("Light position", glm::value_ptr(lightPosition), 0.05, -6, 6);
		ImGui::DragFloat3("Light color", glm::value_ptr(lightColor), 0.005, 0, 1);
		ImGui::EndChild();
		ImGui::BeginChild("Object properties", { 500, 50 });
		ImGui::DragFloat3("Object color", glm::value_ptr(objectColor), 0.005, 0, 1);
		ImGui::DragFloat("Object shininess", &shininess, 1, 4, 256);
		ImGui::EndChild();
		ImGui::End();

		lightSource.Bind();
		lightSourceVertexBuffer.Bind();
		lightProgram.Bind();
		modelLight = glm::translate(glm::mat4(1.0), lightPosition);
		modelLight = glm::scale(modelLight, { 0.5f, 0.5f, 0.5f });

		lightProgram.SetMat4f("u_Model", modelLight);
		lightProgram.SetVector3f("u_Color", lightColor);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		cubeVAO.Bind();
		cubeVBO.Bind();
		program.Bind();
		program.SetVector3f("u_Light.position", lightPosition);
		program.SetVector3f("u_Light.ambient", lightColor);
		program.SetVector3f("u_Light.diffuse", lightColor);
		program.SetVector3f("u_Light.specular", lightColor);
		program.SetVector3f("u_Material.ambient", objectColor);
		program.SetVector3f("u_Material.diffuse", objectColor);
		program.SetVector3f("u_Material.specular", objectColor);
		program.SetFloat("u_Material.shininess", shininess);


		model = glm::rotate(model, glm::radians(15.0f * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		program.SetMat4f("u_Model", model);

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