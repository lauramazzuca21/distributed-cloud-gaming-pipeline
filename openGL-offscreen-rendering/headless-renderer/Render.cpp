#include "Render.h"
#include "FileHandler.h"
// #include "ShaderMaker.h"

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
    img_name << "./frames/frame" << i << ".jpg";
    file_handler::save_jpeg_from_buffer(img_name.str().c_str(), width, height, buffer);
    i++;
    delete[] buffer;
}
void Render::initShader()
{
		// GLenum ErrorCheckValue = glGetError();

		// char* vertexShader = (char*)"vertexShader_C.glsl";
		// char* fragmentShader = (char*)"fragmentShader_C.glsl";

		// programId = shader_maker::createProgram(vertexShader, fragmentShader);
		// glUseProgram(programId);

        // // Ottieni l'identificativo della variabile uniform mat4 Projection (in vertex shader).
        // //Questo identificativo sarà poi utilizzato per il trasferimento della matrice Projection al Vertex Shader
        // MatProj = glGetUniformLocation(programId, "Projection");
        // // Ottieni l'identificativo della variabile uniform mat4 Model (in vertex shader)
        // //Questo identificativo sarà poi utilizzato per il trasferimento della matrice Model al Vertex Shader
        // MatModel = glGetUniformLocation(programId, "Model");
        // //Ottieni l'identificativo della variabile uniform mat4 View (in vertex shader)
        // //Questo identificativo sarà poi utilizzato per il trasferimento della matrice View al Vertex Shader
        // MatView = glGetUniformLocation(programId, "View");

}

void Render::init() {
    glewInit();
    // initShader();
    
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
    
//5. setup background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPixelStorei sets pixel storage modes that affect the operation of subsequent glReadPixels
    // as well as the unpacking of texture patterns (see glTexImage2D and glTexSubImage2D). 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(0.0f, width, height, 0.0f, zNear, zFar);
	// Projection = glm::perspective(glm::radians((float)fov), (float)(width) / float(height), zNear, zFar);
    // glLoadMatrixf((const float*)glm::value_ptr(Projection));
    glMatrixMode(GL_MODELVIEW);

    Cube();

    print_stats();
    errors::CHECK_FRAMEBUFFER_STATUS();

    // encoder->setCodecID(AV_CODEC_ID_MPEG1VIDEO);
    // encoder->start("tmp.mpg", 25, width, height);
}

void Render::display() {
    static GLfloat angle = 0.0f; 

    // //Passo al Vertex Shader il puntatore alla matrice Projection, che sarà associata alla variabile Uniform mat4 Projection
	// //all'interno del Vertex shader. Uso l'identificatio MatProj
	// Projection = glm::perspective(glm::radians((float)fov), (float)(width) / float(height), zNear, zFar);
	// glUniformMatrix4fv(MatProj, 1, GL_FALSE, glm::value_ptr(Projection));
	// //Costruisco la matrice di Vista che applicata ai vertici in coordinate mondo WCS
	// //li trasforma nel sistema di riferimento della camera VCS.
	// // usage: lookAt(eye,at,up);
	// View = camera->GetViewMatrix();
	// glUniformMatrix4fv(MatView, 1, GL_FALSE, glm::value_ptr(View));

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glRotatef(50.0f+angle, 15.0f, 15.0f, 15.0f);
    glBegin(GL_TRIANGLES);

    int i = 0;
    while (i<Index)
    {
        glColor4f(vColors[i][0], vColors[i][1], vColors[i][2], vColors[i][3]);//blue channel
        glVertex3f(vPositions[i][0], vPositions[i][1], vPositions[i][2]);
        i++;
    }

    glEnd();
    glFlush();

    print_frame();
    // encoder->glread_rgb(width, height);
    // encoder->encode_frame();

    angle+=10.0f;
}

void Render::polygon_multicolor(int a, int b, int c, int d)
{
    vColors[Index] = colors[a]; vPositions[Index] = positions[a]; Index++;
    vColors[Index] = colors[b]; vPositions[Index] = positions[b]; Index++;
    vColors[Index] = colors[c]; vPositions[Index] = positions[c]; Index++;
    
    vColors[Index] = colors[a]; vPositions[Index] = positions[a]; Index++;
    vColors[Index] = colors[c]; vPositions[Index] = positions[c]; Index++;
    vColors[Index] = colors[d]; vPositions[Index] = positions[d]; Index++;
}

//create two triangles for each face and assigns colors to the vertices
void Render::polygon_monocolor(int a, int b, int c, int d)
{
    vColors[Index] = colors[a]; vPositions[Index] = positions[a]; Index++;
    vColors[Index] = colors[a]; vPositions[Index] = positions[b]; Index++;
    vColors[Index] = colors[a]; vPositions[Index] = positions[c]; Index++;
    
    vColors[Index] = colors[a]; vPositions[Index] = positions[a]; Index++;
    vColors[Index] = colors[a]; vPositions[Index] = positions[c]; Index++;
    vColors[Index] = colors[a]; vPositions[Index] = positions[d]; Index++;
}

void Render::Cube()
{
    polygon_monocolor(1, 0, 3, 2);
    polygon_monocolor(2, 3, 7, 6);
    polygon_monocolor(3, 0, 4, 7);
    polygon_monocolor(6, 5, 1, 2);
    polygon_monocolor(4, 5, 6, 7);
    polygon_monocolor(5, 4, 0, 1);
}