﻿/* We want the core profile, so we include GL/glcorearb.h. When including that, then
   GL/gl.h should not be included.

   If using compatibility profile, the GL/gl.h and GL/glext.h need to be included.

   GL/wglext.h gives WGL extensions.

   Note that Windows.h needs to be included before them. */

#include <cstdio>
#include <Windows.h>
#include <GL/glcorearb.h>
#include <GL/wglext.h>

LRESULT CALLBACK window_procedure(HWND, UINT, WPARAM, LPARAM);
void* get_proc(const char*);

/* gl_module is for opening the DLL, and the quit flag is here to prevent
   quitting when recreating the window (see the window_procedure function) */

HMODULE gl_module;
bool quit = false;

/* OpenGL function declarations. In practice, we would put these in a
   separate header file and add "extern" in front, so that we can use them
   anywhere after loading them only once. */

PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNGLGETSTRINGPROC glGetString;

int WINAPI WinMain(HINSTANCE instance_handle, HINSTANCE prev_instance_handle, PSTR cmd_line, int cmd_show) {
    /* REGISTER WINDOW */
    WNDCLASS window_class;

    // Clear all structure fields to zero first
    ZeroMemory(&window_class, sizeof(window_class));

    // Define fields we need (others will be zero)
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = instance_handle;
    window_class.lpszClassName = TEXT("OPENGL_WINDOW");

    // Give our class to Windows
    RegisterClass(&window_class);
    /* *************** */

    /* CREATE WINDOW */
    HWND window_handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
      TEXT("OPENGL_WINDOW"),
      TEXT("OpenGL window"),
      WS_OVERLAPPEDWINDOW,
      0, 0,
      800, 600,
      NULL,
      NULL,
      instance_handle,
      NULL);

    HDC dc = GetDC(window_handle);

    ShowWindow(window_handle, SW_SHOW);
    /* ************* */

    /* PIXEL FORMAT */
    PIXELFORMATDESCRIPTOR descriptor;

    // Clear all structure fields to zero first
    ZeroMemory(&descriptor, sizeof(descriptor));

    // Describe our pixel format
    descriptor.nSize = sizeof(descriptor);
    descriptor.nVersion = 1;
    descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER | PFD_SWAP_LAYER_BUFFERS;
    descriptor.iPixelType = PFD_TYPE_RGBA;
    descriptor.cColorBits = 32;
    descriptor.cRedBits = 8;
    descriptor.cGreenBits = 8;
    descriptor.cBlueBits = 8;
    descriptor.cAlphaBits = 8;
    descriptor.cDepthBits = 32;
    descriptor.cStencilBits = 8;

    // Ask for a similar supported format and set it
    int pixel_format = ChoosePixelFormat(dc, &descriptor);
    SetPixelFormat(dc, pixel_format, &descriptor);
    /* *********************** */

    /* RENDERING CONTEXT */
    HGLRC rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);
    /* ***************** */

    /* LOAD FUNCTIONS (should probably be put in a separate procedure) */
    gl_module = LoadLibrary(TEXT("opengl32.dll"));

    wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)get_proc("wglGetExtensionsStringARB");
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)get_proc("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)get_proc("wglCreateContextAttribsARB");
    glGetString = (PFNGLGETSTRINGPROC)get_proc("glGetString");

    FreeLibrary(gl_module);
    /* ************** */

    /* PRINT VERSION */
    const GLubyte* version = glGetString(GL_VERSION);
    printf("%s\n", version);
    fflush(stdout);
    /* ******* */

    /* NEW PIXEL FORMAT*/
    int pixel_format_arb;
    UINT pixel_formats_found;

    int pixel_attributes[] = {
        WGL_SUPPORT_OPENGL_ARB, 1,
        WGL_DRAW_TO_WINDOW_ARB, 1,
        WGL_DRAW_TO_BITMAP_ARB, 1,
        WGL_DOUBLE_BUFFER_ARB, 1,
        WGL_SWAP_LAYER_BUFFERS_ARB, 1,
        WGL_COLOR_BITS_ARB, 32,
        WGL_RED_BITS_ARB, 8,
        WGL_GREEN_BITS_ARB, 8,
        WGL_BLUE_BITS_ARB, 8,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 32,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        0
    };

    BOOL result = wglChoosePixelFormatARB(dc, pixel_attributes, NULL, 1, &pixel_format_arb, &pixel_formats_found);

    if (!result) {
        printf("Could not find pixel format\n");
        fflush(stdout);
        return 0;
    }
    /* **************** */

    /* RECREATE WINDOW */
    wglMakeCurrent(dc, NULL);
    wglDeleteContext(rc);
    ReleaseDC(window_handle, dc);
    DestroyWindow(window_handle);

    window_handle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
      TEXT("OPENGL_WINDOW"),
      TEXT("OpenGL window"),
      WS_OVERLAPPEDWINDOW,
      0, 0,
      800, 600,
      NULL,
      NULL,
      instance_handle,
      NULL);

    dc = GetDC(window_handle);

    ShowWindow(window_handle, SW_SHOW);
    /* *************** */

    /* NEW CONTEXT */
    GLint context_attributes[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    rc = wglCreateContextAttribsARB(dc, 0, context_attributes);
    wglMakeCurrent(dc, rc);
    /* *********** */

    /* EVENT PUMP */
    MSG msg;

    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // draw(); <- there goes your drawing

        SwapBuffers(dc);
    }
    /* ********** */

    return 0;
}

// Procedure that processes window events
LRESULT CALLBACK window_procedure(HWND window_handle, UINT message, WPARAM param_w, LPARAM param_l) {
    /* When destroying the dummy window, WM_DESTROY message is going to be sent,
       but we don't want to quit the application then, and that is controlled by
       the quit flag. */

    switch (message) {
    case WM_DESTROY:
        if (!quit)
            quit = true;
        else
            PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(window_handle, message, param_w, param_l);
}

/* A procedure for getting OpenGL functions and OpenGL or WGL extensions.
   When looking for OpenGL 1.2 and above, or extensions, it uses wglGetProcAddress,
   otherwise it falls back to GetProcAddress. */
void* get_proc(const char* proc_name) {
    void* proc = (void*)wglGetProcAddress(proc_name);
    if (!proc)
        proc = (void*)GetProcAddress(gl_module, proc_name);

    return proc;
}