#include <iostream>

#define GLEW_STATIC 1
#include <GL/glew.h>

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#define __gl_h_
#include <GLUT/glut.h>
#define MACOSX_COMPATIBILITY GLUT_3_2_CORE_PROFILE
#else
#include <GL/glut.h>
#define MACOSX_COMPATIBILITY 0
#endif
#include "glhelper.h"
#include "camera.h"
#include "mesh.h"

GLuint vao;
GLuint n_elements;

Camera cam;
GLuint program_id;
GLuint program_tbn_id;

GLuint color_texture;
GLuint ao_texture;
GLuint displacement_texture;
GLuint normal_texture;

// value for the rotation of the model
bool turn = true;
float angle = 0.0f;

// value to display the TBN basis
bool tbn = true;


void init()
{
  program_tbn_id = glhelper::create_program_from_file("./shaders/texture.vert","./shaders/normal.geom", "./shaders/color.frag");
  program_id = glhelper::create_program_from_file("./shaders/texture.vert","./shaders/texture.frag");

  color_texture = glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Color.png");
  ao_texture = glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_AmbientOcclusion.png");
  displacement_texture = glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Displacement.png");
  normal_texture = glhelper::load_texture("./data/Rocks002_2K/Rocks002_2K_Normal.png");

  // Activer et associer la texture de couleur
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, color_texture);
  

  // Activer et associer la carte d'occlusion
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, ao_texture);
  
  // Activer et associer la carte de déplacement
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, displacement_texture);

  // Activer et associer la carte de normal map
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, normal_texture);

  
  glUseProgram(program_id);
  glUniform1i(glGetUniformLocation(program_id, "myTextureSampler"), 0);
  glUniform1i(glGetUniformLocation(program_id, "aoTextureSampler"), 1);
  glUniform1i(glGetUniformLocation(program_id, "sampler_displacement"), 2);
  glUniform1i(glGetUniformLocation(program_id, "normalTextureSampler"), 3);

  std::vector<float> light_poses = {10.0, 20.0, 0.0, -10.0, -20.0, 0.0};
  glUniform3fv(glGetUniformLocation(program_id, "List_light"), light_poses.size()/3, light_poses.data());

  Mesh m = Mesh::create_sphere(200, 200);
  n_elements = m.size_element();
  vao = m.load_to_gpu();

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

static void display_callback()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cam.draw_frame();

  glUseProgram(program_id);
  glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  glhelper::set_uniform_mat4(program_id, "Model", model);
  glhelper::set_uniform_mat4(program_id, "View", cam.view());
  glhelper::set_uniform_mat4(program_id, "Perspective", cam.projection());

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);

  if(tbn)
  {
    glUseProgram(program_tbn_id);
    glhelper::set_uniform_mat4(program_tbn_id, "Model", model);
    glhelper::set_uniform_mat4(program_tbn_id, "View", cam.view());
    glhelper::set_uniform_mat4(program_tbn_id, "Perspective", cam.projection());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
  }

  CHECK_GL_ERROR();

  glBindVertexArray(0);
  glutSwapBuffers();
}

static void keyboard_callback(unsigned char key, int, int)
{
  int viewport[4];
  int polygonMode;

  switch (key)
  {
    case 'o':
      turn = !turn;
   break;
    case 'f':
      tbn = !tbn;
    break;
    case 'w':
      glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
      glPolygonMode(GL_FRONT_AND_BACK, polygonMode == GL_LINE ? GL_FILL : GL_LINE);
   break;
    case 'p':
      glGetIntegerv(GL_VIEWPORT, viewport);
      glhelper::print_screen(viewport[2], viewport[3]);
      break;
    case 'q':
    case 'Q':
    case 27:
      exit(0);
  }
}

static void reshape_callback(int width, int height)
{
  cam.common_reshape(width,height);
  glViewport(0,0, width, height);
}
static void mouse_callback (int button, int action, int x, int y)
{
  cam.common_mouse(button, action, x, y);
}
static void motion_callback(int x, int y)
{
  cam.common_motion(x, y);
}
static void timer_callback(int)
{
  if(turn) angle += 0.01;
  glutPostRedisplay();
  glutTimerFunc(25, timer_callback, 0);
}
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | MACOSX_COMPATIBILITY);
  glutInitWindowSize(cam.width(), cam.height());
  glutCreateWindow("opengl");
  glutDisplayFunc(display_callback);
  glutMotionFunc(motion_callback);
  glutMouseFunc(mouse_callback);
  glutKeyboardFunc(keyboard_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(25, timer_callback, 0);
  glewExperimental=true;
  glewInit();

  int major, minor;
  glGetIntegerv(GL_MAJOR_VERSION, &major);
  glGetIntegerv(GL_MINOR_VERSION, &minor);
  std::cout << "OpenGL Version: " << major <<"."<< minor << std::endl;

  init();
  glutMainLoop();

  return 0;
}