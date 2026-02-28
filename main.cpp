#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"

"out vec4 vertexColor;\n" // Specify color output to the fragment shader
"out vec3 ourColor2;\n"
"out vec2 TexCoord;\n"
 //out vec3 ourPos;\n

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //ourPos = aPos;\n
"   vertexColor = vec4(0.5, 0.0, 0.0, 0.1);\n" // Output variable is a dark red color
"   ourColor2 = vec3(aColor.x, aColor.y, aColor.z);\n"
"   TexCoord = aTexCoord;"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor2;\n"
 //in vec3 ourPos;\n

"in vec3 ourColor2;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
    //FragColor2 = vec4(ourPos, 1.0f);\n
//"   FragColor2 = vec4(ourColor2, 1.0f);\n"
"   FragColor2 = texture(ourTexture, TexCoord);\n"
"}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for MacOS


    // Window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

	// Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    unsigned int indices[] = { // Specifiy order to draw triangles
        0, 1, 3,     // first triangle
        1, 2, 3      // second triangle
    };
 
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    /** 
    Defined vertex data, would like to send it as input to the GPU
    Create memory in GPU to store this data and configure how OpenGL should interpret it
    We use Vertex Buffer Objects (VBO) to do that
    **/

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy previosuly defined vertex data into the buffer
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Basic vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR SHADER VERTEX COMPILE FAILED" << infoLog << std::endl;
    }

    // Basic fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR SHADER FRAGMENT 1 COMPILE FAILED" << infoLog << std::endl;
    }

    // Shader progra
    unsigned int shaderProgram, shaderProgram_yellow;
    shaderProgram = glCreateProgram();
    shaderProgram_yellow = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, vertexShader_2);
    glAttachShader(shaderProgram, fragmentShader);


    //glAttachShader(shaderProgram_yellow, vertexShader);
    //glAttachShader(shaderProgram_yellow, fragmentShader_yellow);

    glLinkProgram(shaderProgram);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR SHADER COMPILE FAILED" << infoLog << std::endl;
    }

    // Delet shader objects after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //glDeleteShader(fragmentShader_yellow);

    // Now we link vertex attribute to each vertex
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /**
    // Copy vertices array in a buffer  for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Set vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Use our shader program when we want to render an object
    glUseProgram(shaderProgram);
    // Draw the object
    **/

    // The above process is repeated every time we want to draw an object
    // Doesn't scale well, need to store state configs to an object and simply bind it to restore its state
    // We do this with a Vertex Array Object (VAO)
    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); 

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
   

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw as lines

    // Load textures
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load and generate texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("Images/container.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); // Free the image data


	// Keep window open until closed by user
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw the object
        glUseProgram(shaderProgram);         
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

