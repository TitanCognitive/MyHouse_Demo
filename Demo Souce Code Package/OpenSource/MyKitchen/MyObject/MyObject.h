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

#ifndef MY_CONCEPT_OBJECT_H
#define MY_CONCEPT_OBJECT_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"
   #include "../MyGLAPI/MyGLAPI.h"

   // Forward declarations
   struct scene_data_t;
   struct camera_data_t;
   class MyObject;

   // Object instance structure definition
   struct object_data_t
      {
      TITAN_HANDLE64                                  reference;                             // Reference control handle
      TITAN_HANDLE64                                  sim_object;                            // Simulated object handle
      atlas_coordinate_t                              coordinates;                           // Instance coordinates
      titan_mat4_t                                    transform;                             // Transform
      titan_vec4_t                                    color;                                 // Color (RGBA)
      TITAN_INT                                       model_handle;                          // Model handle
      TITAN_INT                                       model_instance;                        // Model instance
      TITAN_BOOL                                      rendering_info;                        // True when rendering information
      };

   // Object intercept structure definition
   struct my_object_intercept_t
      {
      MyObject                                        *self;                                 // Object axiom
      object_data_t                                   *object;                               // Object data
      };

   // Object axiom class 
   class MyObject : public TitanAxiom
      {
      public:
         // Framework methods:
         MyObject(TITAN_HANDLE64 handshake);
         ~MyObject();
         
      protected:
         // Domain methods:
         // T1: Attach concept to the knowledge base
         virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params);
         // T3: Attach concept to the simulation
         virtual titan_result_t AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept);
         // T5: Link concept to other concepts in the simulation
         virtual titan_result_t AtlasSim_Link(TITAN_HANDLE64 concept);
         
         // Instantiator methods:
         // I1: Create an instance for supplied reference
         virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0);
         // I2: Destroy a supplied instance
         virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0);
         // I3: Load an instance from supplied file/name into supplied reference
         virtual titan_result_t AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake = 0);
         // I4: Save an instance to supplied file/name 
         virtual titan_result_t AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0);
         // I5: Return the simulation object of supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0);
         // I9: Get supplied instance value 
         virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0);

      protected:
         // Interpretation methods: BE
         TITAN_NLP_QUERY BE_Last(ATLAS_SIMULATE_PARAMS);
      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY DO_Select(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Clear(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Reset(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Undo(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Redo(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Remove(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Move(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Rotate(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Stop(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Stand(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_SYS_Destroy(ATLAS_SIMULATE_PARAMS);

      private:
         // Simulation methods
         static void T_Think(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags);
         // Video methods:
         static void LoadModel(TITAN_INT model_index, MyObject *self, object_data_t *object);
         static void DisplayData(MyGLAPI *GL, TITAN_POINTER user_data);
         // Haptic methods:
         static void MyMouseEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
         static void MyWindowEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
         static void MyKeyboardEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);

      private:
         // Internal methods:
         TITAN_HANDLE64 GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, object_data_t **object_data,
                                   TITAN_BITMAP64 query_flags = 0);

      protected:
         // Protected variables:
         static TITAN_HANDLE64                        m_coordinate_interface_label;          // "Coordinate interface" label
         static TITAN_HANDLE64                        m_coordinate_label;                    // "Coordinate" label
         static TITAN_HANDLE64                        m_transform_label;                     // "Transform" label
         static TITAN_HANDLE64                        m_color_label;                         // "Color" label
         TITAN_HANDLE64                               m_armature_handle;                     // Handle of the concept simulation armature
         TITAN_HANDLE64                               manifold_handle;                       // Handle of the concept simulation manifold(s)
         TITAN_HANDLE64                               m_object_handle;                       // Handle of the concept simulation object
         TITAN_HANDLE64                               m_pose_handle;                         // Handle of the concept simulation pose
         titan_mat4_t                                 *m_pose_transforms;                    // Pose transforms
         atlas_animation_t                            *m_animation;                          // Loaded animation tracks
         atlas_coordinate_t                           *m_animation_coordinates;              // Loaded animation coordinates of a track

      private:
         // Private variables
         TITAN_HANDLE64                               m_concept;                             // Concept handle
         TITAN_HANDLE64                               m_reference;                           // Reference handle
         TITAN_UINT                                   m_max_trail_coordinates;               // Maximum number of trail coordinates
         TITAN_UINT                                   m_num_trail_coordinates;               // Used number of trail coordinates
         TITAN_ULONG                                  m_current_coordinate_index;            // Current trial coordinate index in the trail coordinates array
         atlas_coordinate_t                           *m_trail_coordinates;                  // Trail coordinates array
         object_data_t                                m_instance;                            // Main instance
         object_data_t                                *m_selected_object;                    // Selected object
         object_data_t                                *m_last_object;                        // Selected object
         my_object_intercept_t                        m_object_intercept;                    // Object intercept structure
         TITAN_STRING                                 m_load_model_name;                     // Name of the pending model to be loaded (when non NULL)
         TITAN_BOOL                                   m_show_help;                           // Show help menu when TRUE
         camera_data_t                                **m_camera;                            // Camera interface
         MyGLAPI                                      *m_GL;                                 // GL interface
         scene_data_t                                 *m_scene;                              // Scene concept instance
         AtlasAttributeLocationAPI                    Location;                              // Location attribute interface
         AtlasAttributeSizeAPI                        Size;                                  // Size attribute interface
         AtlasAttributeColorAPI                       Color;                                 // Color attribute interface
         AtlasAttributeShapeAPI                       Shape;                                 // Shape attribute interface
         AtlasAttributeTextureAPI                     Texture;                               // Texture attribute interface
      };

#endif

/*
   End of file
*/
