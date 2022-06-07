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

#ifndef MY_CONCEPT_LIGHT_H
#define MY_CONCEPT_LIGHT_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"
   #include "../MyGLAPI/MyGLAPI.h"

   // Forward declarations
   struct scene_data_t;
   struct camera_data_t;
   class MyLight;

   // Light instance structure definition
   struct light_data_t
      {
      TITAN_BOOL                                      on;                                    // True when light is turned on
      TITAN_BOOL                                      directional;                           // True when light is turned directional
      TITAN_FLOAT                                     attenuation;                           // Light attenuation
      TITAN_FLOAT                                     focus;                                 // Light focus
      TITAN_FLOAT                                     aperture_angle;                        // Light cut-off angle
      TITAN_UINT                                      light_index;                           // Hosted light index in the video renderer
      TITAN_HANDLE64                                  reference;                             // Reference control handle
      TITAN_HANDLE64                                  body_handle;                           // Lamp body handle
      TITAN_HANDLE64                                  body_vro;                              // Lamp body VRO
      titan_mat4_t                                    *transform;                            // Light transform
      titan_vec4_t                                    *color;                                // Light color
      };

   // Light axiom class 
   class MyLight : public TitanAxiom
      {
      public:
         // Framework methods:
         MyLight(TITAN_HANDLE64 handshake);
         ~MyLight();
         
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
         // I6: Get subordinate instance of supplied parent instance
         virtual TITAN_HANDLE64 AtlasSim_GetSub(TITAN_HANDLE64 sub_concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0);
         // I7: Get size, in bytes, of supplied instance
         virtual TITAN_SIZE64 AtlasSim_GetSize(TITAN_ULONG num_elements, TITAN_HANDLE64 params, TITAN_HANDLE64 handshake = 0) { return sizeof(light_data_t); }

      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY BE_Active(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Directional(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Overhead(ATLAS_SIMULATE_PARAMS);
      
      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY DO_Activate(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Deactivate(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_SYS_Destroy(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Brighten(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Dim(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Focus(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Diffuse(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Open(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Close(ATLAS_SIMULATE_PARAMS);

      private:
         // Expert methods:
         // Simulation methods
         static void Think(atlas_method_t *method);

      private:
         // Internal methods:
         TITAN_HANDLE64 GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, light_data_t **light_data,
                                   TITAN_BITMAP64 query_flags = 0);

      protected:
         // Protected variables:
         static TITAN_HANDLE64                        m_coordinate_interface_label;          // "Coordinate interface" label
         static TITAN_HANDLE64                        m_coordinate_label;                    // "Coordinate" label
         static TITAN_HANDLE64                        m_transform_label;                     // "Transform" label
         static TITAN_HANDLE64                        m_color_label;                         // "Color" label

      private:
         // Private variables
         TITAN_HANDLE64                               m_concept;                             // Concept handle
         TITAN_HANDLE64                               m_object_concept;                      // Sub-concept handle: Object
         TITAN_HANDLE64                               m_reference;                           // Reference handle
         camera_data_t                                **m_camera;                            // Camera interface
         MyGLAPI                                      *m_GL;                                 // GL interface
         scene_data_t                                 *m_scene;                              // Scene concept instance
      };

#endif

/*
   End of file
*/
