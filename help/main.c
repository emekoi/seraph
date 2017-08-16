#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>

#define vertex_shader "vert.glsl"
#define fragment_shader "frag.glsl"
GLuint shaderProgram;

void *read_file(const char *filename, size_t *len) {
  size_t len_ = 0;
  if (!len) len = &len_;
  FILE *fp = fopen(filename, "rb");
  if (!fp) goto end;
  /* Get file size */
  fseek(fp, 0, SEEK_END);
  *len = ftell(fp);
  /* Load file */
  fseek(fp, 0, SEEK_SET);
  char *res = malloc(*len + 1);
  if (!res) return NULL;
  res[*len] = '\0';
  if (fread(res, 1, *len, fp) != *len) {
    free(res);
    fclose(fp);
    return NULL;
  } else {
    fclose(fp);
    return res;
  }
  end:
    return NULL;
}

static inline void DRAW_SETUP(void) {
  static float vertices[] = {
     0.0f,  0.5f, // Vertex 1 (X, Y)
     0.5f, -0.5f, // Vertex 2 (X, Y)
    -0.5f, -0.5f  // Vertex 3 (X, Y)
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;
  glGenBuffers(1, &vbo); // Generate 1 buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /* create shaders */
  const GLchar *vertexSource = read_file(vertex_shader, NULL);
  const GLchar *fragmentSource = read_file(fragment_shader, NULL);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  char buffer[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
  // fprintf(stderr, "vertex: %s\n", buffer);
  if (status != GL_TRUE) {
    exit(EXIT_FAILURE);
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  GLint status1;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status1);
  char buffer1[512];
  glGetShaderInfoLog(fragmentShader, 512, NULL, buffer1);
  // fprintf(stderr, "fragment: %s\n", buffer1);
  if (status1 != GL_TRUE) {
    exit(EXIT_FAILURE);
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);
}

void DRAW(const float start) {
  float time = (clock() - start) / 1000;
  printf("%f\n", time);
  GLint ucolor = glGetUniformLocation(shaderProgram, "color");
  float color[3] = {sin(time) * 2, cos(time) * 2, tan(time) * 2};
  glUniform3fv(ucolor, 1, color);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char *argv[]) {
    const float start = clock();
		/* Information about the current video settings. */
    const SDL_VideoInfo* info = NULL;
    /* Dimensions of our window. */
    int width = 0;
    int height = 0;
    /* Color depth in bits of our window. */
    int bpp = 0;
    /* Flags we will pass into SDL_SetVideoMode. */
    int flags = 0;

    /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError());
        exit( 1 );
    }

    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );

    if( !info ) {
        /* This should probably never happen. */
        fprintf( stderr, "Video query failed: %s\n", SDL_GetError());
        exit( 1 );
    }

    /*
     * Set our width/height to 640/480 (you would
     * of course let the user decide this in a normal
     * app). We get the bpp we will request from
     * the display. On X11, VidMode can't change
     * resolution, so this is probably being overly
     * safe. Under Win32, ChangeDisplaySettings
     * can change the bpp.
    */
    width = 640;
    height = 480;
    bpp = info->vfmt->BitsPerPixel;

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
    */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );


    /*
     * We want to request that SDL provide us
     * with an OpenGL window, in a fullscreen
     * video mode.
     *
     * EXERCISE:
     * Make starting windowed an option, and
     * handle the resize events properly with
     * glViewport.
    */
    flags = SDL_OPENGL;

    /*
     * Set the video mode
    */
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
        /*
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
        */
        fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError());
        exit( 1 );
    }
    /* Init GLEW */
    glewExperimental = GL_TRUE;
    glewInit();

    /* Check for success */
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);

    if (!vertexBuffer) exit(EXIT_FAILURE);

    DRAW_SETUP();

    SDL_Event windowEvent;
    while (1) {
			if (SDL_PollEvent(&windowEvent)) {
		  	if (windowEvent.type == SDL_QUIT)  break;
      }
      static double last = 0;
      DRAW(start);
      SDL_Surface *screen = SDL_GetVideoSurface();
        if (screen) {
           SDL_GL_SwapBuffers();
        }

        /* Wait for next frame */
        double step = (1. / 60);
        double now = SDL_GetTicks() / 1000.;
        double wait = step - (now - last);
        last += step;
        if (wait > 0) {
          SDL_Delay(wait * 1000.);
        } else {
          last = now;
        }
    }
    SDL_Quit();
    return 0;
}
