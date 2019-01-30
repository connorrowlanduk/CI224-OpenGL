#include "View.hpp"

View::View(int widtht, int heightt) {
    width = widtht;
    height = heightt;
    
}
View::~View() {
    
}

int View::initialise() {
    
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    
    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "My Game", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    // Set the mouse at the center of the screen
    
    glfwPollEvents();
    //we set the cursor at the centre so that it always start on our origin (0,0,0)
    glfwSetCursorPos(window, width / 2, height / 2);
    
    
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    programID = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    
    
    return 1;
}

void View::update() {
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    
    //    static const GLfloat g_vertex_buffer_data[] = {
    //        -1.0f, -1.0f, 0.0f,
    //        1.0f, -1.0f, 0.0f,
    //        0.0f,  1.0f, 0.0f
    //    };
    
    //Create sphere obj
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    loadAssImp("..//Assets//sphere.obj", indices, indexed_vertices, indexed_uvs, indexed_normals);
    //bool res =
    
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
    
    //Create grid obj
    std::vector<unsigned short> grid_indices;
    std::vector<glm::vec3> grid_indexed_vertices;
    std::vector<glm::vec2> grid_indexed_uvs;
    std::vector<glm::vec3> grid_indexed_normals;
    loadAssImp("..//Assets//grid.obj", grid_indices, grid_indexed_vertices, grid_indexed_uvs, grid_indexed_normals);
    //bool res =
    
    GLuint grid_vertexbuffer;
    glGenBuffers(1, &grid_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, grid_indexed_vertices.size() * sizeof(glm::vec3), &grid_indexed_vertices[0], GL_STATIC_DRAW);
    
    // One color for each vertex.
    static const GLfloat g_color_buffer_data[] = {
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f
    };
    
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    
    //    static const GLfloat g_uv_buffer_data[] = {
    //        1.0f,  0.0f,  0.0f,
    //        1.0f,  0.0f,  0.0f,
    //        1.0f,  0.0f,  0.0f
    //    };
    //
    //    GLuint uvbuffer;
    //    glGenBuffers(1, &uvbuffer);
    //    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    //
    //    GLuint Texture = loadBMP_custom("texture.bmp");
    //    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
    
    glm::vec3(4, 3, 3);
    GLuint MatrixID1 = glGetUniformLocation(programID, "MVP");
    
    do{
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Use our shader
        glUseProgram(programID);
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                              0,                  // attribute 0. No particular reason for 0, but must match the layout in the vertex shader.
                              3,                  // size
                              GL_FLOAT,     // type
                              GL_FALSE,    // normalized?
                              0,                   // stride
                              (void*)0           // array buffer offset
                              );
        
        // The following code draws a triangle using the function void glDrawArrays(     GLenum mode,      GLint first,      GLsizei count);
        glDrawArrays(GL_TRIANGLES, 0,indexed_vertices.size()); // first vertex: 0, count: 1 triangle is drawn. 1 triangle x 3 vertices = 3
        
        glDisableVertexAttribArray(0);
        
        glDisableVertexAttribArray(1);
        
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, grid_vertexbuffer);
        glVertexAttribPointer(
                              0,                  // attribute 0. No particular reason for 0, but must match the layout in the vertex shader.
                              3,                  // size
                              GL_FLOAT,     // type
                              GL_FALSE,    // normalized?
                              0,                   // stride
                              (void*)0           // array buffer offset
                              );
        
        //        // Bind our texture in Texture Unit 0
        //        glActiveTexture(GL_TEXTURE0);
        //        glBindTexture(GL_TEXTURE_2D, Texture);
        
        //        // Set our "myTextureSampler" sampler to use Texture Unit 0
        //        glUniform1i(TextureID, 0);
        //        // 2nd attribute buffer : textures
        //        glEnableVertexAttribArray(1);
        //        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        //        glVertexAttribPointer(
        //                              1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        //                              2,                                // size
        //                              GL_FLOAT,                         // type
        //                              GL_FALSE,                         // normalized?
        //                              0,                                // stride
        //                              (void*)0                          // array buffer offset
        //                              );
        
        // The following code draws a triangle using the function void glDrawArrays(     GLenum mode,      GLint first,      GLsizei count);
        glDrawArrays(GL_TRIANGLES, 0,grid_indexed_vertices.size()); // first vertex: 0, count: 1 triangle is drawn. 1 triangle x 3 vertices = 3
        
        glDisableVertexAttribArray(0);
        
        glDisableVertexAttribArray(1);
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        glClear( GL_COLOR_BUFFER_BIT );
        glm::mat4 mvp1 = getMVPMatrix();
        glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &mvp1[0][0]);
        
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
//  glDeleteBuffers(1, &uvbuffer);
    glDeleteProgram(programID);
//  glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
}

glm::mat4 getMVPMatrix() {
    
    glm::mat4 Projection =
    
    glm::mat4 View =
    
    glm::mat4 Model = glm::mat4(1.0f); // keep an identity matrix so the geometry stays where it was placed originally
    
    // Our ModelViewProjection : multiplication of our 3 matrices

    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    
    return mvp;
    
}
