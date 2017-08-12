#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>

#define GL_LOADER_GL_VERSION_3_0
#define GL_LOADER_IMPLEMENTATION
#include "gl_loader.h"

typedef int s32;
typedef float float32;
typedef unsigned int u32;

static bool running = true;
LRESULT CALLBACK main_window_callback(HWND window,
				      UINT message,
				      WPARAM w_param,
				      LPARAM l_param) {
  LRESULT result = 0;
  switch (message) {
  case WM_CLOSE:
    running = false;
    break;
  case WM_DESTROY:
    running = false;
    break;
  default:
    result = DefWindowProc(window, message, w_param, l_param);
    break;
  }

  return(result);
}

void gl_loader_error_callback(char *api_name, char *function_name, s32 major_version_number, s32 minor_version_number) {
  if (api_name[0] == 'g' && api_name[1] == 'l') {
    printf("The OpenGL %i.%i function '%s' failed to load.\n", major_version_number, minor_version_number, function_name);
  }
}

s32 WINAPI WinMain(HINSTANCE instance,
		   HINSTANCE prev_instance,
		   LPSTR command_line,
		   int command_show) {
  WNDCLASS window_class = {};
  window_class.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  window_class.lpfnWndProc = main_window_callback;
  window_class.hInstance = instance;
  window_class.hCursor = LoadCursor(0, IDC_ARROW);
  window_class.lpszClassName = "Windows GL Loader Test";
  if (!RegisterClass(&window_class)) {
    printf("Failed to register main window class.");
    return -1;
  }

  HWND window = CreateWindowEx(0, window_class.lpszClassName,
				"Windows GL Loader Test",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				0, 0, instance, 0);
  if(!window) {
    printf("Failed to create window.");
    return -1;
  }

  PIXELFORMATDESCRIPTOR pixel_format = {};
  pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pixel_format.nVersion = 1;
  pixel_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pixel_format.iPixelType = PFD_TYPE_RGBA;
  pixel_format.cColorBits = 32;
  pixel_format.cDepthBits = 24;
  pixel_format.cStencilBits = 8;
  pixel_format.iLayerType = PFD_MAIN_PLANE;

  HDC device_context = GetDC(window);
  int format_index = ChoosePixelFormat(device_context, &pixel_format);
  if (!format_index) {
    printf("Failed to get a pixel format off the device context.\n");
    return -1;
  }

  if (!SetPixelFormat(device_context, format_index, &pixel_format)) {
    printf("Couldn't set the pixel format.\n");
    return -1;
  }
  HGLRC render_context = wglCreateContext(device_context);
  if (!render_context) {
    printf("Couldn't create OpenGL render context\n");
    return -1;
  }
  if (!wglMakeCurrent(device_context, render_context)) {
    MessageBox(0, "Cannot activate the OpenGL rendering context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    printf("Cannot activate the OpenGL rendering context.\n");
    return -1;
  }
  s32 gl_functions_loaded = gl_loader_load_opengl_functions_with_error_callback(gl_loader_error_callback);
  if (!gl_functions_loaded) {
    MessageBox(0, "Some OpenGL functions failed to load.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    printf("Some OpenGL functions failed to load.\n");
    return -1;
  }

  struct Vertex {float32 x, y, z;};
  Vertex vertices[] = {{-1.0f, -1.0f, 0}, {1.0f, -1.0f, 0}, {0, 1.0f, 0}};
  s32 num_vertices = sizeof(vertices)/sizeof(Vertex);
  
  u32 vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, num_vertices*sizeof(Vertex), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glClearColor(0.12f, 0.34f, 0.76f, 1.0f);
  
  while(running) {
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) running = false;
      TranslateMessage(&message);
      DispatchMessageA(&message);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor4f(0.84f, 0.09f, 0.12f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    SwapBuffers(device_context);
  }

  return 0;
}
