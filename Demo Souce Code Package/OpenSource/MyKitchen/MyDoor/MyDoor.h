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

#ifndef MY_CONCEPT_DOOR_H
#define MY_CONCEPT_DOOR_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"
   #include "../MyGLAPI/MyGLAPI.h"

   // Forward declarations
   struct scene_data_t;
   class MyDoor;

   // Door sub-object structure definition
   struct door_object_t
      {
      TITAN_HANDLE64                                  sim_object;                            // Door simulation object
      TITAN_HANDLE64                                  instance;                              // Door instance handle
      atlas_coordinate_t                              *coordinate;                           // Door coordinate
      titan_mat4_t                                    *transform;                            // Door transform
      titan_vec4_t                                    *color;                                // Door color
      };

   // Door instance structure definition
   struct door_data_t
      {
      TITAN_HANDLE64                                  reference;                             // Reference control handle
      TITAN_BOOL                                      inverted;                              // True when the door is inverted
      door_object_t                                   frame;                                 // Door frame data
      door_object_t                                   panel;                                 // Door panel data
      };

   // Door axiom class 
   class MyDoor : public TitanAxiom
      {
      public:
         // Framework methods:
         MyDoor(TITAN_HANDLE64 handshake);
         ~MyDoor();
         
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

      protected:
         // Interpretation methods: BE
         TITAN_NLP_QUERY BE_Opened(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Closed(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BE_Inverted(ATLAS_SIMULATE_PARAMS);

      protected:
         // Interpretation methods: DO
         TITAN_NLP_QUERY DO_Open(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_Close(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DO_SYS_Destroy(ATLAS_SIMULATE_PARAMS);

      private:
         // Expert methods:
         // Internal methods:
         atlas_probe_result_t OpenDoor(door_data_t *door, TITAN_UBYTE query_type);
         atlas_probe_result_t CloseDoor(door_data_t *door, TITAN_UBYTE query_type);
         // Simulation methods
         static void Think(TITAN_FLOAT sim_time, TITAN_HANDLE64 code_data, TITAN_HANDLE64 user_data);
         // Video methods:
         static void DisplayData(MyGLAPI *GL, TITAN_POINTER user_data);

      private:
         // Internal methods:
         TITAN_HANDLE64 GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, door_data_t **door_data,
                                   TITAN_BITMAP64 query_flags = 0);
         titan_result_t CreateSubObjectInstance(TITAN_HANDLE64 reference, TITAN_HANDLE64 sub_concept, door_object_t *sub_object, TITAN_ULONG num_elements = 1, TITAN_HANDLE64 params_handle = NULL, TITAN_HANDLE64 handshake = 0);
         titan_result_t GetSubObjectInstance(TITAN_HANDLE64 sub_concept, door_object_t *object, TITAN_HANDLE64 handshake = 0);
         void Destroy(door_data_t *door);

      protected:
         // Protected variables:
         static TITAN_HANDLE64                        m_coordinate_interface_label;          // "Coordinate interface" label
         static TITAN_HANDLE64                        m_coordinate_label;                    // "Coordinate" label
         static TITAN_HANDLE64                        m_transform_label;                     // "Transform" label
         static TITAN_HANDLE64                        m_color_label;                         // "Color" label
         AtlasAttributeLocationAPI                    m_SubPanelCoordinate;                  // Door panel coordinate interface
         AtlasAttributeLocationAPI                    m_SubFrameCoordinate;                  // Door frame coordinate interface

      private:
         // Private variables
         TITAN_HANDLE64                               m_concept;                             // Concept handle
         TITAN_HANDLE64                               m_sub_frame;                           // Sub-concept handle: Door Frame
         TITAN_HANDLE64                               m_sub_panel;                           // Sub-concept handle: Door Panel
         TITAN_HANDLE64                               m_reference;                           // Reference handle
         scene_data_t                                 *m_scene;                              // Scene concept instance
      };

#endif

/*
   End of file
*/
