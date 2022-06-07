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

#ifndef TITAN_ST_DEFINITIONS_H
#define TITAN_ST_DEFINITIONS_H
 
   // Titan angles indices
   #define TITAN_PITCH                                0                                      // Pitch angle index in a vector (Rotate around X axis)
   #define TITAN_YAW                                  1                                      // Yaw angle index in a vector (Rotate around Y axis)
   #define TITAN_ROLL                                 2                                      // Roll angle index in a vector (Rotate around Z axis)

   // Titan simulation update parameters
   #define TITAN_COORDINATE_TRANSLATE                 0x01                                   // Coordinate is translated
   #define TITAN_COORDINATE_ROTATE                    0x02                                   // Coordinate is rotated
   #define TITAN_COORDINATE_SCALE                     0x04                                   // Coordinate is scaled
   #define TITAN_COORDINATE_TRS                       (TITAN_COORDINATE_TRANSLATE |\
                                                       TITAN_COORDINATE_ROTATE |\
                                                       TITAN_COORDINATE_SCALE)               // Coordinate is translated, rotated, and scaled

   // Enum for vertex types
   enum titan_vertex_type_t
      {
      TITAN_MESH_TYPE_P,                              // Vertex is represented by Position 
      TITAN_MESH_TYPE_PN,                             // Vertex is represented by Position, Normal
      TITAN_MESH_TYPE_PNB,                            // Vertex is represented by Position, Normal, Bone
      TITAN_MESH_TYPE_PNC,                            // Vertex is represented by Position, Normal, Color
      TITAN_MESH_TYPE_PNCB,                           // Vertex is represented by Position, Normal, Color, Bone
      TITAN_MESH_TYPE_PNT1,                           // Vertex is represented by Position, Normal, 1 Texture
      TITAN_MESH_TYPE_PNT2,                           // Vertex is represented by Position, Normal, 2 Textures
      TITAN_MESH_TYPE_PNT3,                           // Vertex is represented by Position, Normal, 3 Textures
      TITAN_MESH_TYPE_PNT4,                           // Vertex is represented by Position, Normal, 4 Textures
      TITAN_MESH_TYPE_PNT1B,                          // Vertex is represented by Position, Normal, 1 Textures, Bone
      TITAN_MESH_TYPE_PNT2B,                          // Vertex is represented by Position, Normal, 2 Textures, Bone
      TITAN_MESH_TYPE_PNT3B,                          // Vertex is represented by Position, Normal, 3 Textures, Bone
      TITAN_MESH_TYPE_PNT4B,                          // Vertex is represented by Position, Normal, 4 Textures, Bone
      TITAN_MESH_TYPE_PNCT1,                          // Vertex is represented by Position, Normal, Color, 1 Textures
      TITAN_MESH_TYPE_PNCT2,                          // Vertex is represented by Position, Normal, Color, 2 Textures
      TITAN_MESH_TYPE_PNCT3,                          // Vertex is represented by Position, Normal, Color, 3 Textures
      TITAN_MESH_TYPE_PNCT4,                          // Vertex is represented by Position, Normal, Color, 4 Textures
      TITAN_MESH_TYPE_PNCT1B,                         // Vertex is represented by Position, Normal, Color, 1 Textures, Bone
      TITAN_MESH_TYPE_PNCT2B,                         // Vertex is represented by Position, Normal, Color, 2 Textures, Bone
      TITAN_MESH_TYPE_PNCT3B,                         // Vertex is represented by Position, Normal, Color, 3 Textures, Bone
      TITAN_MESH_TYPE_PNCT4B                          // Vertex is represented by Position, Normal, Color, 4 Textures, Bone
      };

   // Titan 8-bit 2D vector structure definition
   union titan_vec2_8_t
      {
      // Integer X, Y 
      struct auto_xy_b
         {
         TITAN_BYTE                                   x;                                     // X coordinate - integer
         TITAN_BYTE                                   y;                                     // Y coordinate - integer
         } xy_b;

      // Unsigned Integer X, Y 
      struct auto_xy_ub
         {
         TITAN_UBYTE                                  x;                                     // X coordinate - unsigned integer 
         TITAN_UBYTE                                  y;                                     // Y coordinate - unsigned integer
         } xy_ub;

      // Integer value
      struct auto_2_b
         {
         TITAN_BYTE                                   value[2];                              // Value array - integer
         } b;

      // Unsigned integer value
      struct auto_2_ub
         {
         TITAN_UBYTE                                  value[2];                              // Value array - unsigned integer
         } ub;
      };

   // Titan 8-bit 3D vector structure definition
   union titan_vec3_8_t
      {
      // Integer X, Y, Z 
      struct auto_xyz_b
         {
         TITAN_BYTE                                   x;                                     // X coordinate - integer
         TITAN_BYTE                                   y;                                     // Y coordinate - integer
         TITAN_BYTE                                   z;                                     // Z coordinate - integer
         } xyz_b;

      // Unsigned Integer X, Y, Z 
      struct auto_xyz_ub
         {
         TITAN_UBYTE                                  x;                                     // X coordinate - unsigned integer 
         TITAN_UBYTE                                  y;                                     // Y coordinate - unsigned integer
         TITAN_UBYTE                                  z;                                     // Z coordinate - unsigned integer
         } xyz_ub;

      // Integer value
      struct auto_3_b
         {
         TITAN_BYTE                                   value[3];                              // Value array - integer
         } b;

      // Unsigned integer value
      struct auto_3_ub
         {
         TITAN_UBYTE                                  value[3];                              // Value array - unsigned integer
         } ub;

      // Integer RGB
      struct auto_rgb_b
         {
         TITAN_BYTE                                   red;                                   // Red channel - integer 
         TITAN_BYTE                                   green;                                 // Green channel - integer
         TITAN_BYTE                                   blue;                                  // Blue channel - integer
         } rgb_b;

      // Unsigned integer RGB
      struct auto_rgb_ub
         {
         TITAN_UBYTE                                  red;                                   // Red channel - unsigned integer 
         TITAN_UBYTE                                  green;                                 // Green channel - unsigned integer
         TITAN_UBYTE                                  blue;                                  // Blue channel - unsigned integer
         } rgb_ub;
      };

   // Titan 8-bit 4D vector structure definition
   union titan_vec4_8_t
      {
      // Integer X, Y, Z, T
      struct auto_xyzt_b
         {
         TITAN_BYTE                                   X;                                     // X coordinate - integer
         TITAN_BYTE                                   y;                                     // Y coordinate - integer
         TITAN_BYTE                                   z;                                     // Z coordinate - integer
         TITAN_BYTE                                   t;                                     // T coordinate - integer
         } xyzt_b;

      // Unsigned Integer X, Y, Z 
      struct auto_xyzt_ub
         {
         TITAN_UBYTE                                  x;                                     // X coordinate - unsigned integer 
         TITAN_UBYTE                                  y;                                     // Y coordinate - unsigned integer
         TITAN_UBYTE                                  z;                                     // Z coordinate - unsigned integer
         TITAN_UBYTE                                  w;                                     // T coordinate - unsigned integer
         } xyzt_ub;

      // Integer value
      struct auto_4_b
         {
         TITAN_BYTE                                   value[4];                              // Value array - integer
         } b;

      // Unsigned integer value
      struct auto_4_ub
         {
         TITAN_UBYTE                                  value[4];                              // Value array - unsigned integer
         } ub;

      // Integer RGBA
      struct auto_rgba_b
         {
         TITAN_BYTE                                   red;                                   // Red channel - integer 
         TITAN_BYTE                                   green;                                 // Green channel - integer
         TITAN_BYTE                                   blue;                                  // Blue channel - integer
         TITAN_BYTE                                   alpha;                                 // Alpha channel - integer
         } rgba_b;

      // Unsigned integer RGBA
      struct auto_rgba_ub
         {
         TITAN_UBYTE                                  red;                                   // Red channel - unsigned integer 
         TITAN_UBYTE                                  green;                                 // Green channel - unsigned integer
         TITAN_UBYTE                                  blue;                                  // Blue channel - unsigned integer
         TITAN_UBYTE                                  alpha;                                 // Alpha channel - unsigned integer
         } rgba_ub;
      };

   // Titan 16-bit 2D vector structure definition
   union titan_vec2_16_t
      {
      // Integer X, Y 
      struct auto_xy_s
         {
         TITAN_SHORT                                  x;                                     // X coordinate - integer
         TITAN_SHORT                                  y;                                     // Y coordinate - integer
         } xy_s;

      // Unsigned Integer X, Y 
      struct auto_xy_us
         {
         TITAN_USHORT                                 x;                                     // X coordinate - unsigned integer 
         TITAN_USHORT                                 y;                                     // Y coordinate - unsigned integer
         } xy_us;

      // Integer value
      struct auto_2_s
         {
         TITAN_SHORT                                  value[2];                              // Value array - integer
         } s;

      // Unsigned integer value
      struct auto_2_us
         {
         TITAN_USHORT                                 value[2];                              // Value array - unsigned integer
         } us;
      };

   // Titan 16-bit 3D vector structure definition
   union titan_vec3_16_t
      {
      // Integer X, Y, Z 
      struct auto_xyz_s
         {
         TITAN_SHORT                                  x;                                     // X coordinate - integer
         TITAN_SHORT                                  y;                                     // Y coordinate - integer
         TITAN_SHORT                                  z;                                     // Z coordinate - integer
         } xyz_s;

      // Unsigned Integer X, Y, Z 
      struct auto_xyz_us
         {
         TITAN_USHORT                                 x;                                     // X coordinate - unsigned integer 
         TITAN_USHORT                                 y;                                     // Y coordinate - unsigned integer
         TITAN_USHORT                                 z;                                     // Z coordinate - unsigned integer
         } xyz_us;

      // Integer value
      struct auto_3_s
         {
         TITAN_SHORT                                  value[3];                              // Value array - integer
         } s;

      // Unsigned integer value
      struct auto_3_us
         {
         TITAN_USHORT                                 value[3];                              // Value array - unsigned integer
         } us;

      // Integer RGB
      struct auto_rgb_s
         {
         TITAN_SHORT                                  red;                                   // Red channel - integer 
         TITAN_SHORT                                  green;                                 // Green channel - integer
         TITAN_SHORT                                  blue;                                  // Blue channel - integer
         } rgb_s;

      // Unsigned integer RGB
      struct auto_rgb_us
         {
         TITAN_USHORT                                 red;                                   // Red channel - unsigned integer 
         TITAN_USHORT                                 green;                                 // Green channel - unsigned integer
         TITAN_USHORT                                 blue;                                  // Blue channel - unsigned integer
         } rgb_us;
      };

   // Titan 16-bit 4D vector structure definition
   union titan_vec4_16_t
      {
      // Integer X, Y, Z 
      struct auto_xyzt_s
         {
         TITAN_SHORT                                  X;                                     // X coordinate - integer
         TITAN_SHORT                                  y;                                     // Y coordinate - integer
         TITAN_SHORT                                  z;                                     // Z coordinate - integer
         TITAN_SHORT                                  w;                                     // T coordinate - integer
         } xyzt_s;

      // Unsigned Integer X, Y, Z 
      struct auto_xyzt_us
         {
         TITAN_USHORT                                 x;                                     // X coordinate - unsigned integer 
         TITAN_USHORT                                 y;                                     // Y coordinate - unsigned integer
         TITAN_USHORT                                 z;                                     // Z coordinate - unsigned integer
         TITAN_USHORT                                 w;                                     // T coordinate - unsigned integer
         } xyzt_us;

      // Integer value
      struct auto_4_s
         {
         TITAN_SHORT                                  value[4];                              // Value array - integer
         } s;

      // Unsigned integer value
      struct auto_4_us
         {
         TITAN_USHORT                                 value[4];                              // Value array - unsigned integer
         } us;

      // Integer RGBA
      struct auto_rgba_s
         {
         TITAN_SHORT                                  red;                                   // Red channel - integer 
         TITAN_SHORT                                  green;                                 // Green channel - integer
         TITAN_SHORT                                  blue;                                  // Blue channel - integer
         TITAN_SHORT                                  alpha;                                 // Alpha channel - integer
         } rgba_s;

      // Unsigned integer RGBA
      struct auto_rgba_us
         {
         TITAN_USHORT                                 red;                                   // Red channel - unsigned integer 
         TITAN_USHORT                                 green;                                 // Green channel - unsigned integer
         TITAN_USHORT                                 blue;                                  // Blue channel - unsigned integer
         TITAN_USHORT                                 alpha;                                 // Alpha channel - unsigned integer
         } rgba_us;
      };

   // Titan 32-bit 2D vector structure definition
   union titan_vec2_32_t
      {
      // Integer X, Y 
      struct auto_xy_f
         {
         TITAN_FLOAT                                  x;                                     // X coordinate - single precision
         TITAN_FLOAT                                  y;                                     // Y coordinate - single precision
         } xy_f;

      // Integer X, Y 
      struct auto_xy_i
         {
         TITAN_INT                                    x;                                     // X coordinate - integer
         TITAN_INT                                    y;                                     // Y coordinate - integer
         } xy_i;

      // Unsigned Integer X, Y 
      struct auto_xy_ui
         {
         TITAN_UINT                                   x;                                     // X coordinate - unsigned integer 
         TITAN_UINT                                   y;                                     // Y coordinate - unsigned integer
         } xy_ui;

      // Integer value
      struct auto_2_f
         {
         TITAN_FLOAT                                  value[2];                              // Value array - single precision
         } f;

      // Integer value
      struct auto_2_i
         {
         TITAN_INT                                    value[2];                              // Value array - integer
         } i;

      // Unsigned integer value
      struct auto_2_ui
         {
         TITAN_UINT                                   value[2];                              // Value array - unsigned integer
         } ui;
      };

   // Titan 32-bit 3D vector structure definition
   union titan_vec3_32_t
      {
      // Single-precision pitch, yaw, roll
      struct auto_angle3_f
         {
         TITAN_FLOAT                                  pitch;                                 // X coordinate rotation - single precision
         TITAN_FLOAT                                  yaw;                                   // Y coordinate rotation - single precision 
         TITAN_FLOAT                                  roll;                                  // Z coordinate rotation - single precision
         } rot;

      // Single-precision X, Y, Z 
      struct auto_xyz_f
         {
         TITAN_FLOAT                                  x;                                     // X coordinate - single precision
         TITAN_FLOAT                                  y;                                     // Y coordinate - single precision 
         TITAN_FLOAT                                  z;                                     // Z coordinate - single precision
         } xyz_f;

      // Integer X, Y, Z 
      struct auto_xyz_i
         {
         TITAN_INT                                    x;                                     // X coordinate - integer
         TITAN_INT                                    y;                                     // Y coordinate - integer
         TITAN_INT                                    z;                                     // Z coordinate - integer
         } xyz_i;

      // Unsigned Integer X, Y, Z 
      struct auto_xyz_ui
         {
         TITAN_UINT                                   x;                                     // X coordinate - unsigned integer 
         TITAN_UINT                                   y;                                     // Y coordinate - unsigned integer
         TITAN_UINT                                   z;                                     // Z coordinate - unsigned integer
         } xyz_ui;

      // Single precision value
      struct auto_3_f
         {
         TITAN_FLOAT                                  value[3];                              // Value array - single precision
         } f;

      // Integer value
      struct auto_3_i
         {
         TITAN_INT                                    value[3];                              // Value array - integer
         } i;

      // Unsigned integer value
      struct auto_3_ui
         {
         TITAN_UINT                                   value[3];                              // Value array - unsigned integer
         } ui;

      // Single precision RGB
      struct auto_rgb_f
         {
         TITAN_FLOAT                                  red;                                   // Red channel - single precision
         TITAN_FLOAT                                  green;                                 // Green channel - single precision
         TITAN_FLOAT                                  blue;                                  // Blue channel - single precision
         } rgb_f;

      // Integer RGB
      struct auto_rgb_i
         {
         TITAN_INT                                    red;                                   // Red channel - integer 
         TITAN_INT                                    green;                                 // Green channel - integer
         TITAN_INT                                    blue;                                  // Blue channel - integer
         } rgb_i;

      // Unsigned integer RGB
      struct auto_rgb_ui
         {
         TITAN_UINT                                   red;                                   // Red channel - unsigned integer 
         TITAN_UINT                                   green;                                 // Green channel - unsigned integer
         TITAN_UINT                                   blue;                                  // Blue channel - unsigned integer
         } rgb_ui;
      };

   // Titan 32-bit 4D vector structure definition
   union titan_vec4_32_t
      {
      // Single-precision pitch, yaw, roll
      struct auto_quat_f
         {
         TITAN_FLOAT                                  w;                                     // W rotation - single precision
         TITAN_FLOAT                                  x;                                     // X coordinate - single precision 
         TITAN_FLOAT                                  y;                                     // Y coordinate - single precision
         TITAN_FLOAT                                  z;                                     // Z coordinate - single precision
         } quat;

      // Single-precision pitch, yaw, roll
      struct auto_angle4_f
         {
         TITAN_FLOAT                                  pitch;                                 // X coordinate rotation - single precision
         TITAN_FLOAT                                  yaw;                                   // Y coordinate rotation - single precision 
         TITAN_FLOAT                                  roll;                                  // Z coordinate rotation - single precision
         TITAN_FLOAT                                  t;                                     // T coordinate - single precision
         } rot;

      // Single-precision X, Y, Z 
      struct auto_xyzt_f
         {
         TITAN_FLOAT                                  x;                                     // X coordinate - single precision
         TITAN_FLOAT                                  y;                                     // Y coordinate - single precision 
         TITAN_FLOAT                                  z;                                     // Z coordinate - single precision
         TITAN_FLOAT                                  t;                                     // T coordinate - single precision
         } xyzt_f;

      // Integer X, Y, Z 
      struct auto_xyzt_i
         {
         TITAN_INT                                    x;                                     // X coordinate - integer
         TITAN_INT                                    y;                                     // Y coordinate - integer
         TITAN_INT                                    z;                                     // Z coordinate - integer
         TITAN_INT                                    t;                                     // T coordinate - integer
         } xyzt_i;

      // Unsigned Integer X, Y, Z 
      struct auto_xyzt_ui
         {
         TITAN_UINT                                   x;                                     // X coordinate - unsigned integer 
         TITAN_UINT                                   y;                                     // Y coordinate - unsigned integer
         TITAN_UINT                                   z;                                     // Z coordinate - unsigned integer
         TITAN_UINT                                   t;                                     // T coordinate - unsigned integer
         } xyzt_ui;

      // Single precision value
      struct auto_4_f
         {
         TITAN_FLOAT                                  value[4];                              // Value array - single precision
         } f;

      // Integer value
      struct auto_4_i
         {
         TITAN_INT                                    value[4];                              // Value array - integer
         } i;

      // Unsigned integer value
      struct auto_4_ui
         {
         TITAN_UINT                                   value[4];                              // Value array - unsigned integer
         } ui;

      // Single precision RGBA
      struct auto_rgba_f
         {
         TITAN_FLOAT                                  red;                                   // Red channel - single precision
         TITAN_FLOAT                                  green;                                 // Green channel - single precision
         TITAN_FLOAT                                  blue;                                  // Blue channel - single precision
         TITAN_FLOAT                                  alpha;                                 // Alpha channel - single precision
         } rgba_f;

      // Integer RGBA
      struct auto_rgba_i
         {
         TITAN_INT                                    red;                                   // Red channel - integer 
         TITAN_INT                                    green;                                 // Green channel - integer
         TITAN_INT                                    blue;                                  // Blue channel - integer
         TITAN_INT                                    alpha;                                 // Alpha channel - integer
         } rgba_i;

      // Unsigned integer RGBA
      struct auto_rgba_ui
         {
         TITAN_UINT                                   red;                                   // Red channel - unsigned integer 
         TITAN_UINT                                   green;                                 // Green channel - unsigned integer
         TITAN_UINT                                   blue;                                  // Blue channel - unsigned integer
         TITAN_UINT                                   alpha;                                 // Alpha channel - unsigned integer
         } rgba_ui;
      };

   // Titan 32-bit 3x3 matrix structure definition
   union titan_mat3_32_t
      {
      // Single precision value
      struct auto_3_f
         {
         TITAN_FLOAT                                  value[9];                              // Value array - single precision
         } f;

      // Integer value
      struct auto_3_i
         {
         TITAN_INT                                    value[9];                              // Value array - integer
         } i;

      // Unsigned integer value
      struct auto_3_ui
         {
         TITAN_UINT                                   value[9];                              // Value array - unsigned integer
         } ui;

      // Single precision value
      struct auto_3_mf
         {
         TITAN_FLOAT                                  value[3][3];                            // Value array - single precision
         } mf;

      // Integer value
      struct auto_3_mi
         {
         TITAN_INT                                    value[3][3];                           // Value array - integer
         } mi;

      // Unsigned integer value
      struct auto_3_mui
         {
         TITAN_UINT                                   value[3][3];                           // Value array - unsigned integer
         } mui;
      };

   // Titan 32-bit 4x4 matrix structure definition
   union titan_mat4_32_t
      {
      // Single precision value
      struct auto_4_f
         {
         TITAN_FLOAT                                  value[16];                             // Value array - single precision
         } f;

      // Integer value
      struct auto_4_i
         {
         TITAN_INT                                    value[16];                             // Value array - integer
         } i;

      // Unsigned integer value
      struct auto_4_ui
         {
         TITAN_UINT                                   value[16];                             // Value array - unsigned integer
         } ui;

      // Single precision value
      struct auto_4_mf
         {
         TITAN_FLOAT                                  value[4][4];                            // Value array - single precision
         } mf;

      // Integer value
      struct auto_4_mi
         {
         TITAN_INT                                    value[4][4];                           // Value array - integer
         } mi;

      // Unsigned integer value
      struct auto_4_mui
         {
         TITAN_UINT                                   value[4][4];                           // Value array - unsigned integer
         } mui;
      };

   // Define default vectors as 32-bit single-precision
   typedef titan_vec2_32_t                            titan_vec2_t;                          // Default 2D vector is 32-bits
   typedef titan_vec3_32_t                            titan_vec3_t;                          // Default 3D vector is 32-bits
   typedef titan_vec4_32_t                            titan_vec4_t;                          // Default 4D vector is 32-bits
   typedef titan_mat3_32_t                            titan_mat3_t;                          // Default 3x3 matrix is 32-bits
   typedef titan_mat4_32_t                            titan_mat4_t;                          // Default 4x4 matrix is 32-bits

   // Titan vertex union definition
   union titan_vertex_t
      {
      titan_vec2_t                                    *vec2;                                 // Vertex is a 2D coordinate
      titan_vec3_t                                    *vec3;                                 // Vertex is a 3D coordinate
      titan_vec4_t                                    *vec4;                                 // Vertex is a 4D coordinate
      };

#endif

/*
   End of file
*/
