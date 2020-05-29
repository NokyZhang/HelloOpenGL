
#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void compileShader(GLenum shaderType, const GLchar *const* shaderSource, GLuint * shader);
void linkShader(GLuint vertexShader, GLuint fragShader, GLuint *shaderProgram);
void render();

float vertices1[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

float vertices2[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = { 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

float vertices3[] = {
	-0.75f, -0.75f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.25f, -0.75f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.25f,  0.25f, 0.0f, 0.0f, 1.0f, 0.0f
};

const char *vertexShaderSource  = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
	"out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"   vertexColor = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = vec4(aColor.r, aColor.g, aColor.b, 1.0);\n"
"}\0";

const char *fragShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec4 vertexColor;"
	"void main()\n"
	"{\n"
	"	FragColor = vertexColor;\n"
	"}\0";
const char *fragShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 timeColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = timeColor;\n"
	"}\0";

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__


	GLFWwindow *window = glfwCreateWindow(800, 600, "HelloOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void render() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//shader
	GLuint vertexShader;
	compileShader(GL_VERTEX_SHADER, &vertexShaderSource, &vertexShader);

	GLuint vertexShader2;
	compileShader(GL_VERTEX_SHADER, &vertexShaderSource2, &vertexShader2);

	GLuint fragShader;
	compileShader(GL_FRAGMENT_SHADER, &fragShaderSource, &fragShader);

	GLuint fragShader2;
	compileShader(GL_FRAGMENT_SHADER, &fragShaderSource2, &fragShader2);

	GLuint shaderProgram;
	linkShader(vertexShader, fragShader, &shaderProgram);

	GLuint shaderProgram2;
	linkShader(vertexShader, fragShader2, &shaderProgram2);

	GLuint shaderProgram3;
	linkShader(vertexShader2, fragShader, &shaderProgram3);

	//Set uniform
	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram2, "timeColor");

	//prepare vertices and indices
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VAO2;
	glGenVertexArrays(1, &VAO2);
	GLuint VAO3;
	glGenVertexArrays(1, &VAO3);

	glBindVertexArray(VAO);
	GLuint VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(VAO2);
	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glBindVertexArray(VAO3);
	GLuint VBO3;
	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//draw
	glBindVertexArray(VAO2);
	glUseProgram(shaderProgram);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glUseProgram(shaderProgram2);
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glBindVertexArray(VAO3);
	glUseProgram(shaderProgram3);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	
	//clear
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void compileShader(GLenum shaderType, const GLchar *const* shaderSource, GLuint * shader) {
	int success;
	char infoLog[512];
	*shader = glCreateShader(shaderType);
	glShaderSource(*shader, 1, shaderSource, NULL);
	glCompileShader(*shader);
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(*shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void linkShader(GLuint vertexShader, GLuint fragShader, GLuint *shaderProgram) {
	int success;
	char infoLog[512];
	*shaderProgram = glCreateProgram();
	glAttachShader(*shaderProgram, vertexShader);
	glAttachShader(*shaderProgram, fragShader);
	glLinkProgram(*shaderProgram);
	glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}