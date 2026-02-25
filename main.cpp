#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec4 vertexColor;\n" // Specify color output to the fragment shader
"out vec3 ourColor2;\n"
 //out vec3 ourPos;\n
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //ourPos = aPos;\n
"   vertexColor = vec4(0.5, 0.0, 0.0, 0.1);\n" // Output variable is a dark red color
"   ourColor2 = vec3(aColor.x, aColor.y, aColor.z);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\0";

const char* fragmentShaderSource_yellow =
"#version 330 core\n"
"out vec4 FragColor2;\n"
 //in vec3 ourPos;\n
"in vec3 ourColor2;\n"
"void main()\n"
"{\n"
    //FragColor2 = vec4(ourPos, 1.0f);\n
"   FragColor2 = ourColor2;\n"
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
    

    // Hello Triangle Chapter
    float vertices[] = {
     -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };
    
    // Intro to Element Buffer Objects (EBO)
    float rect_vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = { // Specifiy order to draw triangles
        0, 1, 3,     // first triangle
        1, 2, 3      // second triangle
    };
 
    /*float vertices_2_triangles[] = {
     -0.2f, -0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f,

     -0.2f, -0.4f, 0.0f,
     0.2f, -0.4f, 0.0f,
     0.0f,  -0.6f, 0.0f,
    };*/
    
    float vertices_1[] = {
     -0.2f, -0.2f, 0.0f,
     0.2f, -0.2f, 0.0f,
     0.0f,  0.2f, 0.0f, 
    };

     float vertices_2[] = {
     -0.2f, -0.4f, 0.0f,    1.0f, 0.0f, 0.0f, // Position,   Color
     0.2f, -0.4f, 0.0f,     0.0f, 1.0f, 0.0f,
     0.0f,  -0.6f, 0.0f,    0.0f, 0.0f, 1.0f,
    };   
    
    unsigned int indices_2_tri[] = { // Specifiy order to draw triangles
        0, 1, 2,     // first triangle
        3, 4, 5      // second triangle
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
        std::cout << "ERROR SHADER FRAGMENT COMPILE FAILED" << infoLog << std::endl;
    }

    unsigned int fragmentShader_yellow;
    fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);
    glCompileShader(fragmentShader_yellow);

    glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
        std::cout << "ERROR SHADER FRAGMENT COMPILE FAILED" << infoLog << std::endl;
    }

    // Shader progra
    unsigned int shaderProgram, shaderProgram_yellow;
    shaderProgram = glCreateProgram();
    shaderProgram_yellow = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, vertexShader_2);
    glAttachShader(shaderProgram, fragmentShader);


    glAttachShader(shaderProgram_yellow, vertexShader);
    glAttachShader(shaderProgram_yellow, fragmentShader_yellow);

    glLinkProgram(shaderProgram);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR SHADER COMPILE FAILED" << infoLog << std::endl;
    }

    glLinkProgram(shaderProgram_yellow);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
        std::cout << "ERROR SHADER COMPILE FAILED" << infoLog << std::endl;
    }


    // Delet shader objects after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader_yellow);

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
    
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    unsigned int VBO[2];
    glGenBuffers(2, VBO);

   /* unsigned int VAO, VAO_2;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &VAO_2);

    unsigned int VBO, VBO_2;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO_2);*/
       
    // Bind vertex array object
    glBindVertexArray(VAO[0]);
    // Copy vertex array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_2_tri), indices_2_tri, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
   

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Draw as lines

	// Keep window open until closed by user
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw the object
        glUseProgram(shaderProgram);
        // Let's change colors
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue / 2.0f) + 0.5f);
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        if (vertexColorLocation == -1) { return 0; } // Could not find ourColor

        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        //glBindVertexArray(VAO);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUseProgram(shaderProgram_yellow);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

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

