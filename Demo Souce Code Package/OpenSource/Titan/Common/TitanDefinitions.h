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

#ifndef TITAN_BASE_DEFINITIONS_H
#define TITAN_BASE_DEFINITIONS_H
 
   // Titan dependencies file path
   #define TITAN_DEPENDENCIES                         "Common/TitanDependencies.h"

   // Define NULL
   #ifndef NULL
   #define NULL                                       0
   #endif

   // Titan OK, Titan ERROR definitions
   #define TITAN_ERROR_08                             0xFF                                   // Titan Error
   #define TITAN_ERROR_16                             0xFFFF                                 // Titan Error
   #define TITAN_ERROR_32                             0xFFFFFFFF                             // Titan Error
   #define TITAN_HANDLE_ERROR                         0xFFFFFFFF                             // Titan Handle Error

   // Extrema
   #define TITAN_UBYTE_MIN                            0                                      // Minimum value for 8-bit unsigned register 
   #define TITAN_UBYTE_MAX                            0xFF                                   // Maximum value for 8-bit unsigned register
   #define TITAN_BYTE_MIN                             -128                                   // Minimum value for 8-bit signed register 
   #define TITAN_BYTE_MAX                             127                                    // Maximum value for 8-bit signed register
   #define TITAN_USHORT_MIN                           0                                      // Minimum value for 16-bit unsigned register
   #define TITAN_USHORT_MAX                           0xFFFF                                 // Maximum value for 16-bit unsigned register
   #define TITAN_SHORT_MIN                            -32768                                 // Minimum value for 16-bit signed register
   #define TITAN_SHORT_MAX                            32767                                  // Maximum value for 16-bit signed register
   #define TITAN_UINT_MIN                             0                                      // Minimum value for 32-bit unsigned register 
   #define TITAN_UINT_MAX                             0xFFFFFFFF                             // Maximum value for 32-bit unsigned register
   #define TITAN_SIZE32_MIN                           0                                      // Minimum value for 32-bit size
   #define TITAN_SIZE32_MAX                           0xFFFFFFFF                             // Maximum value for 32-bit size
   #define TITAN_INT_MIN                              -2147483648                            // Minimum value for 32-bit signed register
   #define TITAN_INT_MAX                              2147483647                             // Maximum value for 32-bit signed register
   #define TITAN_ULONG_MIN                            0                                      // Minimum value for 64-bit unsigned register
   #define TITAN_ULONG_MAX                            0xFFFFFFFFFFFFFFFF                     // Maximum value for 64-bit unsigned register
   #define TITAN_SIZE_MIN                             0                                      // Minimum value for 64-bit size
   #define TITAN_SIZE_MAX                             0xFFFFFFFFFFFFFFFF                     // Maximum value for 64-bit size
   #define TITAN_LONG_MIN                             0                                      // Minimum value for 64-bit signed register
   #define TITAN_LONG_MAX                             0                                      // Maximum value for 64-bit signed register
   #define TITAN_FLOAT_MIN                            FLT_MIN                                // Minimum value for 32-bit IEEE floating-point register
   #define TITAN_FLOAT_MAX                            FLT_MAX                                // Maximum value for 32-bit IEEE floating-point register
   #define TITAN_FLOAT_MIN_ACCURATE                   -16777216                              // Minimum value for 32-bit IEEE floating-point register that can be represented accurately
   #define TITAN_FLOAT_MAX_ACCURATE                   16777216                               // Maximum value for 32-bit IEEE floating-point register that can be represented accurately
   #define TITAN_DOUBLE_MIN                           DBL_MIN                                // Minimum value for 64-bit double-precision register
   #define TITAN_DOUBLE_MAX                           DBL_MAX                                // Maximum value for 64-bit double-precision register
   #define TITAN_HANDLE_MIN                           0                                      // Minimum value for 32-bit unsigned register
   #define TITAN_HANDLE_MAX                           0xFFFFFFFF                             // Maximum value for 32-bit unsigned register
   #define TITAN_BITMAP16_MAX                         0xFFFF                                 // Maximum value for 16-bit bitmap
   #define TITAN_BITMAP32_MAX                         0xFFFFFFFF                             // Maximum value for 32-bit bitmap
   #define TITAN_BITMAP16_MASK                        0xFFFF                                 // Mask value for 16-bit bitmap
   #define TITAN_BITMAP32_MASK                        0xFFFFFFFF                             // Mask value for 32-bit bitmap
   #define TITAN_BITMAP64_MASK                        0xFFFFFFFFFFFFFFFF                     // Mask value for 64-bit bitmap

   // Error codes based on data sizes
   #define TITAN_USHORT_ERROR                         0xFFFF                                 // Error in unsigned 16-bit registers
   #define TITAN_USHORT_NO_VALUE                      TITAN_USHORT_ERROR                     // No Value in unsigned 16-bit registers
   #define TITAN_UINT_ERROR                           0xFFFFFFFF                             // Error in unsigned 32-bit registers
   #define TITAN_UINT_NO_VALUE                        TITAN_UINT_ERROR                       // No Value in unsigned 32-bit registers
   #define TITAN_SIZE32_ERROR                         0xFFFFFFFF                             // Error in unsigned 32-bit registers used for size
   #define TITAN_SIZE64_ERROR                         0xFFFFFFFFFFFFFFFF                     // Error in unsigned 64-bit registers used for size
   #define TITAN_HANDLE32_ERROR                       0xFFFFFFFF                             // Error in unsigned 32-bit registers used for size
   #define TITAN_HANDLE64_ERROR                       0xFFFFFFFFFFFFFFFF                     // Error in unsigned 64-bit registers used for size

   // Axioms
   #define TITAN_KILOBYTE_SIZE                        1024L                                  // Number of bytes in one kilobyte
   #define TITAN_MEGABYTE_SIZE                        1048576L                               // Number of bytes in one megabyte

   // Search results
   #define TITAN_SEARCH_LESS                          -1                                     // Search position is before the match position
   #define TITAN_SEARCH_MATCHED                       0                                      // Search position is match position
   #define TITAN_SEARCH_MORE                          1                                      // Search position is past the match position
   #define TITAN_SEARCH_CONTINUE                      -1                                     // Result has not been found yet, continue searching
   #define TITAN_SEARCH_MATCHED                       0                                      // Result is match
   #define TITAN_SEARCH_NO_MATCH                      1                                      // Result has no matches
   #define TITAN_SEARCH_ERROR                         -9999                                  // Search had an error

   // Size of the Titan ID
   typedef unsigned long long int                     TITAN_ID;                              // Definition of a Titan ID atom
   typedef unsigned long long int                     TITAN_UNIT_ID;                         // Definition of a Titan unit ID
   typedef unsigned long long int                     TITAN_NODE_ID;                         // Definition of a Titan node ID
   typedef bool                                       TITAN_BOOL;                            // Single bit (not in my lifetime)
   typedef unsigned char                              TITAN_HANDLE_TYPE;                     // Unsigned byte (8-bit) used as a handle type
   typedef unsigned char                              TITAN_RESOURCE_TYPE;                   // Unsigned byte (8-bit) used as a resource type
   typedef unsigned char                              TITAN_RESOURCE_CATEGORY;               // Unsigned byte (8-bit) used as a resource category
   typedef unsigned char                              TITAN_UBYTE;                           // Unsigned byte (8-bit)
   typedef char                                       TITAN_BYTE;                            // Signed byte (8-bit)
   typedef char                                       TITAN_CHAR;                            // Signed byte (8-bit)
   typedef unsigned short int                         TITAN_USHORT;                          // Unsigned short integer (16-bit)
   typedef short int                                  TITAN_SHORT;                           // Signed short integer (16-bit)
   typedef unsigned int                               TITAN_UINT;                            // Unsigned integer (32-bit)
   typedef int                                        TITAN_INT;                             // Signed integer (32-bit)
   typedef unsigned long long int                     TITAN_ULONG;                           // Unsigned long long integer (64-bit)
   typedef long long int                              TITAN_LONG;                            // Long long integer  (64-bit)
   typedef float                                      TITAN_FLOAT;                           // Single-precision floating point (32-bit)
   typedef unsigned short int                         TITAN_HALF;                            // Half-precision floating point (16-bit)
   typedef double                                     TITAN_DOUBLE;                          // Double-precision floating point (64-bit)
   typedef unsigned long long int                     TITAN_INTERFACE_HANDLE;                // Interface plug handle (64-bit unsigned)
   typedef unsigned int                               TITAN_INTERFACE_COMMAND;               // Interface command (32-bit unsigned)
   typedef unsigned int                               TITAN_HANDLE;                          // Handle (32-bit unsigned)
   typedef unsigned char                              TITAN_HANDLE08;                        // Handle (08-bit unsigned)
   typedef unsigned short int                         TITAN_HANDLE16;                        // Handle (16-bit unsigned)
   typedef unsigned int                               TITAN_HANDLE32;                        // Handle (32-bit unsigned)
   typedef unsigned long long int                     TITAN_HANDLE64;                        // Handle (64-bit unsigned)
   typedef unsigned int                               TITAN_TIME;                            // Titan time in milliseconds since application started (32-bit unsigned)
   typedef unsigned long long int                     TITAN_TIME64;                          // Titan time stamp in milliseconds
   typedef unsigned short int                         TITAN_KEYCODE;                         // 16-bit Keycode: See TitanDeviceKeyboard.h
   typedef unsigned int                               TITAN_KEYMAP;                          // 32-bit Key bitmap: See TitanDeviceKeyboard.h
   typedef unsigned int                               TITAN_EVENT;                           // 32-bit Titan event
   typedef unsigned char                              TITAN_BITMAP8;                         // 8-bit bitmap
   typedef unsigned short int                         TITAN_BITMAP16;                        // 16-bit bitmap
   typedef unsigned int                               TITAN_BITMAP32;                        // 32-bit bitmap 
   typedef unsigned long long int                     TITAN_BITMAP64;                        // 64-bit bitmap 
   typedef unsigned char                              TITAN_MULTI8;                          // 8-bit multivalue
   typedef unsigned short int                         TITAN_MULTI16;                         // 16-bit multivalue
   typedef unsigned int                               TITAN_MULTI32;                         // 32-bit multivalue
   typedef unsigned long long int                     TITAN_MULTI64;                         // 64-bit multivalue
   typedef float                                      TITAN_QUAD[4];                         // A quad of floats
   typedef unsigned char                              TITAN_RESOURCE_INDEX8;                 // Unsigned byte (8-bit) resource index
   typedef unsigned short int                         TITAN_RESOURCE_INDEX16;                // Unsigned byte (16-bit) resource index
   typedef unsigned int                               TITAN_RESOURCE_INDEX24;                // Unsigned byte (24-bit) resource index
   typedef unsigned int                               TITAN_RESOURCE_INDEX32;                // Unsigned byte (32-bit) resource index
   typedef unsigned long long int                     TITAN_RESOURCE_INDEX40;                // Unsigned byte (40-bit) resource index
   typedef unsigned long long int                     TITAN_RESOURCE_INDEX48;                // Unsigned byte (48-bit) resource index
   typedef char                                       TITAN_NAME;                            // Definition of a Titan Name
   typedef unsigned int                               TITAN_SIZE32;                          // Data size (32-bit) definition
   typedef size_t                                     TITAN_SIZE64;                          // Data size (size_t) definition
   typedef char                                       *TITAN_STRING;                         // Character string
   typedef char                                       *TITAN_SHADOW_STRING;                  // Character string shadow
   typedef char                                       *TITAN_STATIC_STRING;                  // Character string that is static
   typedef unsigned char                              *TITAN_BUFFER;                         // Buffer of byte data
   typedef unsigned char                              *TITAN_ARRAY;                          // Array of byte data
   typedef void                                       *TITAN_POINTER;                        // Pointer definition
   typedef void                                       *TITAN_REFERENCE;                      // Titan reference definition
   typedef float                                      TITAN_VALUE;                           // Titan value definition

   // Titan function process result
   enum titan_result_t                               {TITAN_RESULT_FOUND = 0,                // Result: Result is found
                                                      TITAN_RESULT_PROCESS,                  // Result: Process the information
                                                      TITAN_RESULT_STOP,                     // Result: Stop processing the information
                                                      TITAN_RESULT_PAUSE,                    // Result: Pause processing the information
                                                      TITAN_RESULT_RESUME,                   // Result: Resume processing the information
                                                      TITAN_RESULT_OK = 100,                 // Result: All is OK
                                                      TITAN_RESULT_INVALID_INPUT = 102,      // Result: Supplied input was invalid
                                                      TITAN_RESULT_WARNING,                  // Result: Warning
                                                      TITAN_RESULT_INIT_FAIL,                // Result: Initialization has failed
                                                      TITAN_RESULT_NO_MEMORY,                // Result: Not enough memory
                                                      TITAN_RESULT_NOT_FOUND,                // Result: Queried value is not found
                                                      TITAN_RESULT_FILE_NOT_FOUND,           // Result: File was not found
                                                      TITAN_RESULT_FILE_LOAD_ERROR,          // Result: File could not be loaded
                                                      TITAN_RESULT_FILE_SAVE_ERROR,          // Result: File could not be saved
                                                      TITAN_RESULT_FILE_BAD_FORMAT,          // Result: File was opened but had a bad format
                                                      TITAN_RESULT_ERROR,                    // Result: Error has occurred
                                                      TITAN_RESULT_NOT_INITIALIZED,          // Result: Data was not initialized
                                                      TITAN_RESULT_NOT_SUPPORTED,            // Result: Functionality not supported
                                                      TITAN_RESULT_DATA_CORRUPTED,           // Result: Data is corrupted
                                                      TITAN_RESULT_DATA_MISMATCH,            // Result: Data is mismatched
                                                      TITAN_RESULT_STALL,                    // Result: Process has stalled
                                                      TITAN_RESULT_LIMIT_REACHED,            // Result: Limit has been reached
                                                      TITAN_RESULT_PARSE_ERROR,              // Result: A parse error has occurred
                                                      TITAN_RESULT_RESOURCE_NOT_FOUND,       // Result: Resource not found
                                                      TITAN_RESULT_NODE_NOT_FOUND,           // Result: Node not found
                                                      TITAN_RESULT_AGENT_BUSY,               // Result: Agent is busy
                                                      TITAN_RESULT_DESCRIPTION_NOT_FOUND,    // Result: Description not found
                                                      TITAN_RESULT_INTERPRETATION_NOT_FOUND, // Result: Interpretation not found
                                                      TITAN_RESULT_DEFINITION_NOT_FOUND,     // Result: Interpretation not found
                                                      TITAN_RESULT_VALUE_NOT_FOUND,          // Result: Interpretation not found
                                                      TITAN_RESULT_OUT_OF_BOUNDS,            // Result: Supplied data is out of bounds
                                                      TITAN_RESULT_INCOMPATIBLE,             // Result: Process or data is incompatible
                                                      TITAN_RESULT_ATTACH_FAIL,              // Result: Failed to attach a concept
                                                      TITAN_RESULT_LINK_FAIL,                // Result: Failed to link a concept
                                                      TITAN_RESULT_DEFINE_FAIL,              // Result: Failed to define a concept
                                                      TITAN_RESULT_INSERTION_FAIL,           // Result: Insertion has failed
                                                      TITAN_RESULT_DELETION_FAIL,            // Result: Deletion has failed
                                                      TITAN_RESULT_TERMINATED,               // Result: Process is terminated                                                     
                                                      TITAN_RESULT_NO_DATA,                  // Result: There is no data to be processed
                                                      };

   // Titan drive editor tag types
   enum titan_data_type_t                            {TITAN_DATA_TYPE_UNKNOWN = 0,           // Tag is unknown
                                                      TITAN_DATA_TYPE_BYTE = 1,              // Tag is of byte (7.1-bit) type
                                                      TITAN_DATA_TYPE_UBYTE,                 // Tag is of unsigned byte (8-bit) type
                                                      TITAN_DATA_TYPE_SHORT,                 // Tag is of short integer (15.1-bit) type
                                                      TITAN_DATA_TYPE_USHORT,                // Tag is of unsigned short integer (16-bit) type
                                                      TITAN_DATA_TYPE_INT,                   // Tag is of integer (31.1-bit) type
                                                      TITAN_DATA_TYPE_UINT,                  // Tag is of unsigned integer (32-bit) type
                                                      TITAN_DATA_TYPE_LONG,                  // Tag is of long integer (63.1-bit) type
                                                      TITAN_DATA_TYPE_ULONG,                 // Tag is of unsigned long integer (64-bit) type
                                                      TITAN_DATA_TYPE_FLOAT,                 // Tag is of single-precision float (32-bit) type
                                                      TITAN_DATA_TYPE_DOUBLE,                // Tag is of double-precision float (64-bit) type
                                                      TITAN_DATA_TYPE_HALF,                  // Tag is of half-precision float (16-bit) type
                                                      TITAN_DATA_TYPE_RGB,                   // Tag is of unsigned byte RGB type
                                                      TITAN_DATA_TYPE_RGBA,                  // Tag is of unsigned byte RGBA type
                                                      TITAN_DATA_TYPE_RGB_FLOAT,             // Tag is of floating-point RGB type
                                                      TITAN_DATA_TYPE_RGBA_FLOAT,            // Tag is of floating-point RGBA type
                                                      TITAN_DATA_TYPE_VEC2,                  // Tag is of Vec2 type
                                                      TITAN_DATA_TYPE_VEC3,                  // Tag is of Vec3 type
                                                      TITAN_DATA_TYPE_VEC4,                  // Tag is of Vec4 type
                                                      TITAN_DATA_TYPE_MAT2,                  // Tag is of Mat2 type
                                                      TITAN_DATA_TYPE_MAT3,                  // Tag is of Mat3 type
                                                      TITAN_DATA_TYPE_MAT4,                  // Tag is of Mat4 type
                                                      TITAN_DATA_TYPE_MAT3x2,                // Tag is of Mat3x2 type
                                                      TITAN_DATA_TYPE_MAT4x2,                // Tag is of Mat4x2 type
                                                      TITAN_DATA_TYPE_MAT4x3,                // Tag is of Mat4x3 type
                                                      TITAN_DATA_TYPE_STRING,                // Tag is of string type
                                                      TITAN_DATA_TYPE_ARRAY,                 // Tag is of array type
                                                      TITAN_DATA_TYPE_REFERENCE,             // Tag if of reference type
                                                      };

   // Bit fields 
   #define TITAN_BIT0                                 0x1
   #define TITAN_BIT1                                 0x2
   #define TITAN_BIT2                                 0x4
   #define TITAN_BIT3                                 0x8
   #define TITAN_BIT4                                 0x10
   #define TITAN_BIT5                                 0x20
   #define TITAN_BIT6                                 0x40
   #define TITAN_BIT7                                 0x80
   #define TITAN_BIT8                                 0x100
   #define TITAN_BIT9                                 0x200
   #define TITAN_BIT10                                0x400
   #define TITAN_BIT11                                0x800
   #define TITAN_BIT12                                0x1000
   #define TITAN_BIT13                                0x2000
   #define TITAN_BIT14                                0x4000
   #define TITAN_BIT15                                0x8000
   #define TITAN_BIT16                                0x10000
   #define TITAN_BIT17                                0x20000
   #define TITAN_BIT18                                0x40000
   #define TITAN_BIT19                                0x80000
   #define TITAN_BIT20                                0x100000
   #define TITAN_BIT21                                0x200000
   #define TITAN_BIT22                                0x400000
   #define TITAN_BIT23                                0x800000
   #define TITAN_BIT24                                0x1000000
   #define TITAN_BIT25                                0x2000000
   #define TITAN_BIT26                                0x4000000
   #define TITAN_BIT27                                0x8000000
   #define TITAN_BIT28                                0x10000000
   #define TITAN_BIT29                                0x20000000
   #define TITAN_BIT30                                0x40000000
   #define TITAN_BIT31                                0x80000000
   #define TITAN_BIT32                                0x100000000
   #define TITAN_BIT33                                0x200000000
   #define TITAN_BIT34                                0x400000000
   #define TITAN_BIT35                                0x800000000
   #define TITAN_BIT36                                0x1000000000
   #define TITAN_BIT37                                0x2000000000
   #define TITAN_BIT38                                0x4000000000
   #define TITAN_BIT39                                0x8000000000
   #define TITAN_BIT40                                0x10000000000
   #define TITAN_BIT41                                0x20000000000
   #define TITAN_BIT42                                0x40000000000
   #define TITAN_BIT43                                0x80000000000
   #define TITAN_BIT44                                0x100000000000
   #define TITAN_BIT45                                0x200000000000
   #define TITAN_BIT46                                0x400000000000
   #define TITAN_BIT47                                0x800000000000
   #define TITAN_BIT48                                0x1000000000000
   #define TITAN_BIT49                                0x2000000000000
   #define TITAN_BIT50                                0x4000000000000
   #define TITAN_BIT51                                0x8000000000000
   #define TITAN_BIT52                                0x10000000000000
   #define TITAN_BIT53                                0x20000000000000
   #define TITAN_BIT54                                0x40000000000000
   #define TITAN_BIT55                                0x80000000000000
   #define TITAN_BIT56                                0x100000000000000
   #define TITAN_BIT57                                0x200000000000000
   #define TITAN_BIT58                                0x400000000000000
   #define TITAN_BIT59                                0x800000000000000
   #define TITAN_BIT60                                0x1000000000000000
   #define TITAN_BIT61                                0x2000000000000000
   #define TITAN_BIT62                                0x4000000000000000
   #define TITAN_BIT63                                0x8000000000000000
   // Byte --------------<little-endian> -------x10->   1111110000000000
   // Byte --------------<little-endian> ----------->   5432109876543210

   // Callbacks
   #define TITAN_FUNCTION(D, Z)                       &D::Z
   #define TITAN_METHOD_HANDLE64                      static TITAN_HANDLE64
   #define TITAN_METHOD_RESULT                        static titan_result_t
   #define TITAN_METHOD_ULONG                         static TITAN_ULONG
   #define TITAN_NLP_QUERY                            static atlas_probe_result_t
   #define TITAN_NLP                                  static void 
   #define TITAN_AF(X)                                static X
   #define TITAN_REQUEST                              static titan_result_t
   
   // Define base class
   #define SETBASE(X)                                 typedef X BaseClass; 

#endif

/*
   End of file
*/
