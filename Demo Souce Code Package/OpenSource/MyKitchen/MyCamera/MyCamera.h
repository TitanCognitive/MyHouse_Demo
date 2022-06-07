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

// Dependencies
#ifndef MY_CONCEPT_CAMERA_H
#define MY_CONCEPT_CAMERA_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"

   // Dependencies
   #include "../MyGLAPI/MyGLAPI.h"


   // Forward declarations
   struct console_data_t;
   struct scene_data_t;
   class MyCamera;

   // Camera instance data structure definition
   struct camera_data_t
      {
      TITAN_HANDLE64                                  reference;                             // Reference pointing to this camera instance
      TITAN_HANDLE64                                  sim_object;                            // Simulation object handle (See Atlas Attributes)
      TITAN_HANDLE64                                  target_reference;                      // Reference of the target this camera instance is tracking
      titan_vec3_t                                    internal_angles;                       // Internal camera angles [pitch, yaw, roll]
      glm::mat4                                       video_transform;                       // Camera transform in video space [4x4 right-handed] matrix
      glm::vec3                                       position;                              // Camera position in video space [x, y, z]
      glm::vec3                                       angles;                                // Camera angles in video space [pitch, yaw, roll]
      glm::vec3                                       forward;                               // Camera forward vector in video space
      glm::vec3                                       right;                                 // Camera right vector in video space
      glm::vec3                                       up;                                    // Camera up vector in video space
      console_data_t                                  *console;                              // Current console hosting this camera (See MyConsole)
      TITAN_INT                                       model_handle;                          // Handle of the camera model (or -1 if no model is selected)
      TITAN_INT                                       model_instance;                        // Handle of the camera model instance (or -1 if no instance is selected)
      TITAN_FLOAT                                     speed;                                 // Rate of motion multiplier of the camera
      TITAN_BOOL                                      rendering_info;                        // True when rendering camera information (See DiplayData)
      TITAN_BOOL                                      follow_target;                         // True when the camera is following its target
      };

   // Camera intercept structure definition
   // Instances of this structure are sent to the static (callback) methods 
   struct my_camera_intercept_t
      {
      MyCamera                                        *self;                                 // Camera class pointer
      camera_data_t                                   *camera;                               // Camera instance data
      };

   // Camera axiom class 
   class MyCamera : public TitanAxiom
      {
      public:
         // Framework methods:
         MyCamera(TITAN_HANDLE64 handshake);
         ~MyCamera();
         
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
         TITAN_NLP_QUERY BE_Active(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Fast(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Slow(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Current(ATLAS_SIMULATE_PARAMS);

      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY DO_Select(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Track(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Follow(ATLAS_SIMULATE_PARAMS);

      private:
         // Simulation callback method:
         static void Think(atlas_method_t *method);
         // Video callback methods:
         static void LoadModel(TITAN_INT model_index, MyCamera *self, camera_data_t *camera);
         static void UpdateCamera(MyGLAPI *GL, TITAN_POINTER user_data);
         static void DisplayData(MyGLAPI *GL, TITAN_POINTER user_data);
         // Haptic callback methods:
         static void MyMouseEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);
         static void MyKeyboardEvent(TITAN_DOUBLE system_time, TITAN_POINTER user_data, my_window_haptic_t *haptics);


      private:
         // Internal methods:
         TITAN_HANDLE64 GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, 
                                   camera_data_t **camera_data, TITAN_BITMAP64 query_flags = ATLAS_GET_INSTANCE);
         void AlignAngle(camera_data_t *camera, TITAN_UINT angle_index, TITAN_FLOAT snap_angle);

      private:
         // Private variables
         TITAN_HANDLE64                               m_concept;                             // Handle of the concept hosting this camera axiom
         TITAN_HANDLE64                               m_camera_interface_label;              // Camera interface label "Camera Interface"
         camera_data_t                                *m_active_camera;                      // Active camera being used rendered
         camera_data_t                                m_default;                             // Default instance 
         my_camera_intercept_t                        m_camera_intercept;                    // Camera intercept structure used for callbacks

      private:
         // Private dependency variables
         scene_data_t                                 *m_scene;                              // Scene concept instance
         MyGLAPI                                      *m_GL;                                 // GL interface

      private:
         // Framework interfaces:
         AtlasAttributeLocationAPI                    Location;                              // Atlas attribute location interface
         AtlasAttributeSizeAPI                        Size;                                  // Atlas attribute Size interface
         AtlasAttributeColorAPI                       Color;                                 // Atlas attribute Color interface
      };

#endif

/*
   End of file
*/
