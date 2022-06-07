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

#ifndef MY_CONCEPT_SCENE_H
#define MY_CONCEPT_SCENE_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"
   #include "../MyGLAPI/MyGLAPI.h"

   // Forward declarations
   struct object_data_t;
   struct camera_data_t;
   class MyScene;

   // Scene instance structure definition
   struct scene_data_t
      {
      TITAN_HANDLE64                                  reference;                             // Reference control handle
      MyScene                                         *Scene;                                // Scene class handling this structure
      TITAN_UINT                                      max_references;                        // Maximum number of references allowed in the scene
      TITAN_UINT                                      num_references;                        // Number of references in the scene
      TITAN_HANDLE64                                  *references;                           // Array of registered references
      TITAN_FLOAT                                     **transforms;                          // Reference transforms
      TITAN_STRING                                    *names;                                // Reference names
      };

   // Scene intercept structure definition
   struct my_scene_intercept_t
      {
      MyScene                                         *self;                                 // Scene axiom
      scene_data_t                                    *scene;                                // Scene data
      };

   // Scene axiom class 
   class MyScene : public TitanAxiom
      {
      #define SCENE_FILE_EXTENSION                    "dat"                                  // Extension of the scene file

      public:
         // Framework methods:
         MyScene(TITAN_HANDLE64 handshake);
         ~MyScene();
         
      public:
         // Interface methods:
         titan_result_t Register(scene_data_t *scene, TITAN_HANDLE64 reference_handle, TITAN_FLOAT *transform);
         titan_result_t Cancel(scene_data_t *scene, TITAN_HANDLE64 reference_handle);
         void ToggleRendering(scene_data_t *scene);

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
         // I9: Get supplied instance value 
         virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0);

      protected:
         // Interpretation methods: BE
         TITAN_NLP_QUERY BE_Active(ATLAS_SIMULATE_PARAMS);
      
      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY DO_Remember(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Recall(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Forget(ATLAS_SIMULATE_PARAMS);

      private:
         // Video methods:
         static void DisplayData(MyGLAPI *GL, TITAN_POINTER user_data);

      private:
         // Internal methods:
         TITAN_HANDLE64 GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, scene_data_t **scene_data,
                                   TITAN_BITMAP64 query_flags = 0);

      protected:
         // Protected variables:
         TITAN_HANDLE64                               m_scene_interface_label;               // Scene interface label
         MyGLAPI                                      *m_GL;                                 // GL interface

      private:
         // Private variables
         TITAN_HANDLE64                               m_concept;                             // Concept handle
         TITAN_HANDLE64                               m_reference;                           // Reference handle
         scene_data_t                                 m_instance;                            // Main instance
         my_scene_intercept_t                         m_intercept;                           // Scene intercept
      };

#endif

/*
   End of file
*/
