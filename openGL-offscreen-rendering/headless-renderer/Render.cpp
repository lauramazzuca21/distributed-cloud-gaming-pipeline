#include "Render.h"
#include "ShaderMaker.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

void Render::print_stats() {
    GLint red_bits, green_bits, blue_bits, alpha_bits;

    glGetIntegerv(GL_RED_BITS,   &red_bits);
    glGetIntegerv(GL_GREEN_BITS, &green_bits);
    glGetIntegerv(GL_BLUE_BITS,  &blue_bits);
    glGetIntegerv(GL_ALPHA_BITS, &alpha_bits);

    fprintf(stderr, "FBO format R%dG%dB%dA%d\n",
        (int)red_bits,
        (int)green_bits,
        (int)blue_bits,
        (int)alpha_bits );  

    GLint imp_fmt, imp_type;

    glGetIntegerv (GL_IMPLEMENTATION_COLOR_READ_FORMAT, &imp_fmt);
    glGetIntegerv (GL_IMPLEMENTATION_COLOR_READ_TYPE,   &imp_type);

    printf ("Supported Color Format/Type: %x/%x\n", imp_fmt, imp_type);
}

void Render::print_frame() {
    static int i=0;
    unsigned char* buffer = new unsigned char[ width * height * 4 ];

    printf("Reading pixels...");
    glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
    errors::assertOpenGLError("glReadPixels");
    printf("Done.\nPrinting frame %d...", i);
    
    std::ostringstream img_name;
    img_name << "./frames/frame" << i << ".png";
    // stbi_write_jpg(img_name.str().c_str(), width, height, 4, buffer, 300);
    stbi_write_png(img_name.str().c_str(), width, height, 4, buffer, 0);
    printf("Done.\n");

    i++;
    delete[] buffer;
}

void Render::initShader()
{
    GLenum ErrorCheckValue = glGetError();

    std::string vertexShader = "shaders/v_passthrough.glsl";
    std::string fragmentShader = "shaders/f_passthrough.glsl";

    programId = shader_maker::createProgram(vertexShader.c_str(), fragmentShader.c_str());
    glUseProgram(programId);

    // Ottieni l'identificativo della variabile uniform mat4 Projection (in vertex shader).
    //Questo identificativo sarà poi utilizzato per il trasferimento della matrice Projection al Vertex Shader
    MatProj = glGetUniformLocation(programId, "P");
    // Ottieni l'identificativo della variabile uniform mat4 Model (in vertex shader)
    //Questo identificativo sarà poi utilizzato per il trasferimento della matrice Model al Vertex Shader
    MatModel = glGetUniformLocation(programId, "M");
    //Ottieni l'identificativo della variabile uniform mat4 View (in vertex shader)
    //Questo identificativo sarà poi utilizzato per il trasferimento della matrice View al Vertex Shader
    MatView = glGetUniformLocation(programId, "V");
    glUniform4fv(glGetUniformLocation(programId, "Color"), 1, value_ptr(glm::vec4(1.0, 0.0, 0.0, 1.0)));

}

void Render::initBuffers() {
    //correct setup thanks to https://github.com/cirosantilli/cpp-cheat/blob/70b22ac36f92e93c94f951edb8b5af7947546525/opengl/offscreen.c
//1. Generate framebuffer to hold rendering destination
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
//2. Generate color render buffer
    glGenRenderbuffers(1, &color);
    glBindRenderbuffer(GL_RENDERBUFFER, color);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
    errors::assertOpenGLError("glRenderbufferStorage");
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color);
    errors::assertOpenGLError("glFramebufferRenderbuffer");
//3. Generate depth render buffer with 32 bit component to handle alpha as well
    glGenRenderbuffers(1, &depth);
    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
//4.
    glReadBuffer(GL_COLOR_ATTACHMENT0);
}

void Render::init() {
    glewInit();

    initShader();
    initBuffers();

    model = new Model("meshes/bunny.obj", "bunny");

//5. setup background
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width, height);

    //glPixelStorei sets pixel storage modes that affect the operation of subsequent glReadPixels
    // as well as the unpacking of texture patterns (see glTexImage2D and glTexSubImage2D). 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    print_stats();
    errors::CHECK_FRAMEBUFFER_STATUS();
}

void Render::display() {

	//Passo al Vertex Shader il puntatore alla matrice Projection, che sar� associata alla variabile Uniform mat4 Projection
	//all'interno del Vertex shader. Uso l'identificatio MatProj
	Projection = glm::perspective(glm::radians((float)fov), (float)(width) / float(height), zNear, zFar);
	glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
	//Costruisco la matrice di Vista che applicata ai vertici in coordinate mondo WCS
	//li trasforma nel sistema di riferimento della camera VCS.
	// usage: lookAt(eye,at,up);
	View = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(MatView, 1, GL_FALSE, value_ptr(View));

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    model->Draw(&MatModel);

    glFinish();
    errors::assertOpenGLError("glFinish");


    print_frame();
}