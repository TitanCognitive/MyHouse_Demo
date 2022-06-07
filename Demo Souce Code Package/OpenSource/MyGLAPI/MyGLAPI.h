/* 
   Copyright (c) 2020 Titan Virtual Corp. All rights reserved.

   http://www.titanvx.com
   Support: support@titanvx.com
   
   The source code and/or documentation ("Licensed Deliverables") are 
   subject to Titan Virtual Corp. intellectual property rights under U.S. and 
   international Copyright laws. 

   The Licensed Deliverables contained herein are PROPRIETARY and
   CONFIDENTIAL to Titan Virtual Corp. and are being provided under the terms and
   conditions of a form of Titan Virtual Corp. software license agreement by and
   between Titan Virtual Corp. and Licensee ("License Agreement") or electronically
   accepted by Licensee. Notwithstanding any terms or conditions to the contrary 
   in the License Agreement, reproduction or disclosure of the Licensed Deliverables 
   to any third party without the express written consent of Titan Virtual Corp. is prohibited.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
   USE OR OTHER DEALINGS IN THE SOFTWARE.   
   
   Any use of the Licensed Deliverables in individual and commercial software must include, 
   in the user documentation and internal comments to the code, the above Disclaimer 
   and U.S. Government End Users Notice.
*/

#ifndef MY_GL_API_H
#define MY_GL_API_H

   // GLM declarations
   #define GLM_SWIZZLE
   #include "Externals/glm/glm/glm.hpp"  
   #include "Externals/glm/glm/gtc/matrix_transform.hpp"
   #include "Externals/glm/glm/gtc/type_ptr.hpp"
   #include "Externals/glm/glm/gtc/quaternion.hpp"

   // Forward declarations
   struct MyGLAPI;

   // Define number of vertices in each polygon category
   #define  QUAD_VERTICES                             4
   #define  TRIANGLE_VERTICES                         3

   // Font format (large/thin)
   #define FONT_THIN                                  0
   #define FONT_LARGE                                 1
   
   // Font alignment
   #define FONT_ALIGN_LEFT                            0
   #define FONT_ALIGN_RIGHT                           1
   #define FONT_ALIGN_HIGH                            2
   #define FONT_ALIGN_BOTTOM                          3

   // Targa file manipulation
   #define TGA_PROGRESSIVE_BLEND                      -1
   #define TGA_ABSOLUTE_BLEND                         -2
   #define TGA_RESIZE                                 -3
   #define TGA_GRAYSCALE                              -4

   // Mouse cursor state changes
   #define MY_MOUSE_CURSOR_ON                         0x1
   #define MY_MOUSE_CURSOR_OFF                        0x2
   #define MY_MOUSE_BUTTON_1                          0x1
   #define MY_MOUSE_BUTTON_1_CHANGED                  0x2
   #define MY_MOUSE_BUTTON_2                          0x4
   #define MY_MOUSE_BUTTON_2_CHANGED                  0x8
   #define MY_MOUSE_BUTTON_3                          0x10
   #define MY_MOUSE_BUTTON_3_CHANGED                  0x20

   // Key state enumerators
   #define KEY_STATE_UNKNOWN                          0x0
   #define KEY_STATE_OFF                              0x1                                    // Key state is off
   #define KEY_STATE_TOGGLE_OFF                       0x2                                    // Key state is toggled off
   #define KEY_STATE_ON                               0x4                                    // Key state is on
   #define KEY_STATE_TOGGLE_ON                        0x8                                    // Key state is toggled on
   #define KEY_STATE_ACTIVE                           0x10                                   // Key state is active (caps lock, num lock, scroll lock)

   // Maximum precache lines available
   #define MY_GL_PRECACHE_MAX_LINES                   512                                    // Allow up to 512 lines to be precached
   #define MY_GL_PRECACHE_FLAG_MODEL                  0x01                                   // True when the precache line is loading a model

   // Titan window haptics structure
   struct my_window_haptic_t
      {
      TITAN_INT                                       x_min;                                 // Window left pixel position
      TITAN_INT                                       x_max;                                 // Window right pixel position
      TITAN_INT                                       y_min;                                 // Window top pixel position
      TITAN_INT                                       y_max;                                 // Window bottom pixel position
      TITAN_INT                                       window_width;                          // Computed width of the window
      TITAN_INT                                       window_height;                         // Computed height of the window
      TITAN_INT                                       client_center_x;                       // X position of the center of the client
      TITAN_INT                                       client_center_y;                       // Y position of the center of the client
      TITAN_INT                                       screen_center_x;                       // X position of the center of the screen
      TITAN_INT                                       screen_center_y;                       // Y position of the center of the screen
      TITAN_BITMAP64                                  mouse_flags;                           // Mouse flags (visible cursor, etc)
      TITAN_BITMAP64                                  mouse_buttons;                         // Mouse button bits
      TITAN_INT                                       mouse_x;                               // Mouse x position
      TITAN_INT                                       mouse_y;                               // Mouse y position
      TITAN_INT                                       last_mouse_toggle_x;                   // Mouse x previous position before toggle ON
      TITAN_INT                                       last_mouse_toggle_y;                   // Mouse y previous position before toggle ON
      TITAN_FLOAT                                     aspect_ratio;                          // Computed window aspect ratio
      TITAN_BITMAP32                                  reserved;                              // Reserved
      TITAN_DOUBLE                                    mouse_time;                            // Time in seconds of last mouse movement
      TITAN_DOUBLE                                    mouse_delta_time;                      // Time in seconds since last mouse movement
      TITAN_DOUBLE                                    key_delta_times[256];                  // Keyboard press delta times - time since last toggle
      TITAN_DOUBLE                                    key_times[256];                        // Keyboard press times
      TITAN_BITMAP64                                  key_states[256];                       // Keyboard states
      TITAN_DOUBLE                                    last_key_sample_time;                  // Last time the keyboard was sampled
      };

   // Graphics library callback method prototypes
   typedef void(*OnMouseEvent)(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
   typedef void(*OnWindowEvent)(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
   typedef void(*OnKeyboardEvent)(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
   typedef void(*MyGraphicsCallback)(MyGLAPI *gl_class, TITAN_POINTER user_data);

   // My GL model API structure definition
   struct MyGLModelAPI
      {
      virtual TITAN_INT CreateModel(const TITAN_STRING model_path) = 0;
      virtual TITAN_INT CreateModelInstance(TITAN_INT model_index, TITAN_FLOAT *transform, titan_vec4_t *color, TITAN_BOOL opaque) = 0;
      virtual void DestroyModelInstance(TITAN_INT model_index, TITAN_UINT model_instance, TITAN_FLOAT *transform, TITAN_FLOAT *color) = 0;
      virtual void DestroyModel(TITAN_INT model_index) = 0;
      virtual void DestroyModels() = 0;
      virtual TITAN_UINT GetNumModelNames(TITAN_INT model_index) = 0;
      virtual const TITAN_STRING GetModelName(TITAN_INT model_index, TITAN_UINT model_name_index) = 0;
      virtual const TITAN_STRING GetModelPath(TITAN_INT model_index) = 0;
      };

   // My GL font API structure definition
   struct MyGLFontAPI
      {
      virtual void FontPrint(titan_vec3_t *t_origin, TITAN_UINT zoom, const TITAN_STRING text) = 0;
      virtual void FontPrintv(titan_vec3_t *origin, TITAN_UINT zoom, const TITAN_STRING format, ...) = 0;
      };

   // My GL light API structure definition
   struct MyGLLightAPI
      {
      virtual TITAN_UINT AddLight() = 0;
      virtual void DestroyLight(TITAN_UINT light_index) = 0;
      virtual void EnableLight(TITAN_UINT light_index) = 0;
      virtual void DisableLight(TITAN_UINT light_index) = 0;
      virtual void SetLightColor(TITAN_UINT light_index, TITAN_FLOAT *light_color) = 0;
      virtual void SetLightType(TITAN_UINT light_index, TITAN_BOOL directional) = 0;
      virtual void SetLightTransform(TITAN_UINT light_index, TITAN_FLOAT *position) = 0;
      virtual void SetLightSpotAngle(TITAN_UINT light_index, TITAN_FLOAT *spotlight_angle) = 0;
      virtual void SetLightIntensity(TITAN_UINT light_index, TITAN_FLOAT light_intensity) = 0;
      virtual void SetLightFocus(TITAN_UINT light_index, TITAN_FLOAT light_focus) = 0;
      virtual void SetLightCutoffAngle(TITAN_UINT light_index, TITAN_FLOAT light_cutoff_angle) = 0;
      };

   // My GL API structure definition
   struct MyGLAPI
      {
      virtual titan_result_t PrecacheResource(TITAN_BITMAP64 resource_id, const TITAN_STRING model_path, TITAN_POINTER method_ptr, TITAN_POINTER class_ptr, TITAN_POINTER instance_ptr) = 0;
      virtual titan_result_t AddFrameIntercept(MyGraphicsCallback callback_method, TITAN_POINTER user_data) = 0;
      virtual titan_result_t SetPreFrameCallback(TITAN_UINT rendering_order, MyGraphicsCallback callback_method, TITAN_POINTER user_data) = 0;
      virtual titan_result_t SetPostFrameCallback(TITAN_UINT rendering_order, MyGraphicsCallback callback_method, TITAN_POINTER user_data) = 0;
      virtual titan_result_t SetMouseIntercept(OnMouseEvent MouseEvent, TITAN_POINTER user_data) = 0;
      virtual titan_result_t SetWindowIntercept(OnWindowEvent WindowEvent, TITAN_POINTER user_data) = 0;
      virtual titan_result_t SetKeyboardIntercept(OnKeyboardEvent KeyboardEvent, TITAN_POINTER user_data) = 0;
      virtual titan_result_t CopyToClipboard(TITAN_POINTER window_handle, const TITAN_STRING data) = 0;
      virtual titan_result_t CopyFromClipboard(TITAN_POINTER window_handle, const TITAN_STRING data, TITAN_SIZE64 max_data_size = 0) = 0;
      virtual void SetModelTexture(TITAN_INT model_index, TITAN_INT texture_index) = 0;
      virtual void SetModelMesh(TITAN_INT model_index, TITAN_INT mesh_index) = 0;
      virtual void SetCameraTransform(TITAN_FLOAT *camera_transform) = 0;
      virtual void SetMousePosition(TITAN_UINT x, TITAN_UINT y, TITAN_BITMAP32 mouse_flags = 0) = 0;
      virtual void ToggleFullScreen(TITAN_DOUBLE system_time = 0.0) = 0;
      virtual void ShowGrid() = 0;
      virtual void SetCameraPerspective() = 0;
      virtual void Resize(TITAN_DOUBLE system_time) = 0;
      virtual TITAN_ULONG GetFrame() = 0;
      virtual void GetKeyboardState(TITAN_DOUBLE **key_times, TITAN_BITMAP64 **key_states) = 0;
      virtual void FadeToColor(TITAN_FLOAT *end_color, TITAN_FLOAT fade_time) = 0;
      virtual void Enable2D(bool enable) = 0;
      virtual void GetWindowSize(TITAN_INT &width, TITAN_INT &height) = 0;

      // Sub-APIs
      MyGLModelAPI                                    *Model;
      MyGLFontAPI                                     *Font;
      MyGLLightAPI                                    *Light;
      };

#endif