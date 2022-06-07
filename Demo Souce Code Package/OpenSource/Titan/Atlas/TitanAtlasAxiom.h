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

#ifndef TITAN_ATLAS_AXIOM_H
#define TITAN_ATLAS_AXIOM_H

   // Dependencies
   #include "Common\Titan.h"
   #include "Atlas\TitanAtlasDefinitions.h"
   #include "Atlas\TitanAtlasManagers.h"
   #include "Atlas\TitanAtlasAttributes.h"
   #include "Atlas\TitanAtlasAPI.h"

   // Forward declarations
   struct _TitanToolsAPI;
   typedef _TitanToolsAPI *TitanToolsAPI;
   struct _TitanMemoryAPI;
   typedef _TitanMemoryAPI *TitanMemoryAPI;
   struct _TitanFileAPI;
   typedef _TitanFileAPI *TitanFileAPI;

   // Macros
   #define TO_INSTANTIATOR(X)                         _D(AtlasInstantiatorAPI, X)
   #define TO_AXIOM(X)                                _DP(AtlasAxiomInterface, X)

   // Titan axiom interface structure definition
   struct AtlasAxiomInterface
      {
      // Axiom interface methods:
      virtual TITAN_ULONG Atlas_Version() = 0;
      // T1: Attach concept to the knowledge base
      virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params) = 0;
      // T2: Detach concept from the knowledge base
      virtual titan_result_t AtlasKB_Detach(TITAN_HANDLE64 concept) = 0;
      // T3: Attach concept to the simulation
      virtual titan_result_t AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept) = 0;
      // T4: Detach concept from the simulation
      virtual titan_result_t AtlasSim_Detach(TITAN_HANDLE64 concept) = 0;
      // T5: Link concept to other concepts in the simulation
      virtual titan_result_t AtlasSim_Link(TITAN_HANDLE64 concept) = 0;
      // T6: Unlink concept from other concepts in the simulation
      virtual titan_result_t AtlasSim_Unlink(TITAN_HANDLE64 concept) = 0;
      // T7: Observe a pinged directive in the domain
      virtual titan_result_t AtlasSim_Observe(TITAN_HANDLE64 observed_reference, TITAN_HANDLE64 observer_reference, TITAN_HANDLE64 ping_type, TITAN_HANDLE64 directive, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) = 0;
      // T8: Hear an expression in the domain
      virtual titan_result_t AtlasSim_Hear(TITAN_HANDLE64 hearing_reference, TITAN_HANDLE64 telling_reference, TITAN_HANDLE64 tell_type, const TITAN_STRING expression, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) = 0;
      // T9: Receive a datagram in the domain
      virtual titan_result_t AtlasSim_Receive(TITAN_HANDLE64 receiver_reference, TITAN_HANDLE64 sender_reference, TITAN_SIZE32 message_size, TITAN_HANDLE32 message_type, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake = 0) = 0;
      };

   // Titan axiom class definition   
   class TitanAxiom: public _AtlasInstantiatorAPI, public AtlasAxiomInterface
      {
      // Axiom version
      #define TITAN_AXIOM_VERSION                     1
      
      public:
         // Framework functions:
         TitanAxiom(TITAN_HANDLE64 params);
         virtual ~TitanAxiom();
   
      public:
         // Axiom interface methods:
         virtual TITAN_ULONG Atlas_Version() { return TITAN_AXIOM_VERSION; }
         // T1: Attach concept to the knowledge base
         virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params) { return TITAN_RESULT_OK; }
         // T2: Detach concept from the knowledge base
         virtual titan_result_t AtlasKB_Detach(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T3: Attach concept to the simulation
         virtual titan_result_t AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T4: Detach concept from the simulation
         virtual titan_result_t AtlasSim_Detach(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T5: Link concept to other concepts in the simulation
         virtual titan_result_t AtlasSim_Link(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T6: Unlink concept from other concepts in the simulation
         virtual titan_result_t AtlasSim_Unlink(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T7: Observe a pinged directive in the domain
         virtual titan_result_t AtlasSim_Observe(TITAN_HANDLE64 observed_reference, TITAN_HANDLE64 observer_reference, TITAN_HANDLE64 ping_type, TITAN_HANDLE64 directive, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }
         // T8: Hear an expression in the domain
         virtual titan_result_t AtlasSim_Hear(TITAN_HANDLE64 hearing_reference, TITAN_HANDLE64 telling_reference, TITAN_HANDLE64 tell_type, const TITAN_STRING expression, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }
         // T9: Receive a datagram in the domain
         virtual titan_result_t AtlasSim_Receive(TITAN_HANDLE64 receiver_reference, TITAN_HANDLE64 sender_reference, TITAN_SIZE32 message_size, TITAN_HANDLE32 message_type, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }

      public:
         // Instantiator methods:
         // I1: Create an instance for supplied reference
         virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I2: Destroy a supplied instance
         virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I3: Load an instance from supplied file/name into supplied reference
         virtual titan_result_t AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I4: Save an instance to supplied file/name 
         virtual titan_result_t AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I5: Return the simulation object of supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I6: Get subordinate instance of supplied parent instance
         virtual TITAN_HANDLE64 AtlasSim_GetSub(TITAN_HANDLE64 sub_concept_data, TITAN_HANDLE64 parent_instance, TITAN_HANDLE64 parent_instance_element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I7: Get size, in bytes, of supplied instance
         virtual TITAN_SIZE64 AtlasSim_GetSize(TITAN_ULONG num_elements, TITAN_HANDLE64 params, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I8: Set supplied instance value
         virtual titan_result_t AtlasSim_SetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I9: Get supplied instance value 
         virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // IA: Set number of elements in supplied instance
         virtual titan_result_t AtlasSim_SetElementCount(TITAN_HANDLE64 instance, TITAN_ULONG num_elements, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // IB: Get number of elements in supplied instance
         virtual TITAN_ULONG AtlasSim_GetElementCount(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IC: Get an element, specified by index, from supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetElement(TITAN_HANDLE64 instance, TITAN_ULONG element_index, TITAN_HANDLE64 handshake = 0) { return 0; }
         // ID: Get the first element in supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetFirstElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IE: Get next element of supplied instance element
         virtual TITAN_HANDLE64 AtlasSim_GetNextElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IF: Add an element to supplied instance
         virtual TITAN_HANDLE64 AtlasSim_AddElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IG: Remove supplied element in supplied instance
         virtual titan_result_t AtlasSim_RemoveElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }

      protected:
         // Inherited variables:
         static _TitanToolsAPI                        *Tools;                                // Tools interface
         static _TitanMemoryAPI                       *Memory;                               // Memory interface
         static _TitanFileAPI                         *File;                                 // File interface
         // Cognitive APIs:
         _AtlasCognitiveAPI                           *Cognitive;                             // Atlas cognitive interface
         _AtlasDomainAPI                              *Domain;                                // Atlas domain interface
         _AtlasSimulationAPI                          *Simulation;                            // Atlas simulation interface
         _AtlasLabelAPI                               *Label;                                 // Atlas label interface 
         _AtlasAgentAPI                               *Agent;                                 // Atlas agent interface
         _AtlasReferenceAPI                           *Reference;                             // Atlas reference interface
         _AtlasGroupAPI                               *Group;                                 // Atlas group interface
         _AtlasDirectiveAPI                           *Directive;                             // Atlas directive interface
         _AtlasLedgerAPI                              *Ledger;                                // Atlas ledge interface
         _AtlasInstanceAPI                            *Instance;                              // Atlas instance interface
         // Simulation APIs:
         #ifdef ATLAS_VR
         _AtlasArmatureAPI                            *Armature;                             // Armature interface
         _AtlasManifoldAPI                            *Manifold;                             // Manifold interface
         _AtlasPoseAPI                                *Pose;                                 // Pose interface
         _AtlasPoseAPI                                *ObjectPose;                           // Object interface
         #endif
      };
   #if 0
   // Titan axiom class definition   
   class TitanAxiom: public _AtlasInstantiatorAPI, public AtlasAxiomInterface
      {
      public:
         // Framework functions:
         TitanAxiom(TITAN_HANDLE64 params);
         virtual ~TitanAxiom();
   
      public:
         // Axiom interface methods:
         virtual TITAN_ULONG Atlas_Version() { return TITAN_AXIOM_VERSION; }
         // T1: Attach concept to the knowledge base
         virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params) { return TITAN_RESULT_OK; }
         // T2: Detach concept from the knowledge base
         virtual titan_result_t AtlasKB_Detach(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T3: Attach concept to the simulation
         virtual titan_result_t AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T4: Detach concept from the simulation
         virtual titan_result_t AtlasSim_Detach(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T5: Link concept to other concepts in the simulation
         virtual titan_result_t AtlasSim_Link(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T6: Unlink concept from other concepts in the simulation
         virtual titan_result_t AtlasSim_Unlink(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T7: Observe a pinged directive in the domain
         virtual titan_result_t AtlasSim_Observe(TITAN_HANDLE64 observed_reference, TITAN_HANDLE64 observer_reference, TITAN_HANDLE64 ping_type, TITAN_HANDLE64 directive, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }
         // T8: Hear an expression in the domain
         virtual titan_result_t AtlasSim_Hear(TITAN_HANDLE64 hearing_reference, TITAN_HANDLE64 telling_reference, TITAN_HANDLE64 tell_type, const TITAN_STRING expression, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }
         // T9: Receive a datagram in the domain
         virtual titan_result_t AtlasSim_Receive(TITAN_HANDLE64 receiver_reference, TITAN_HANDLE64 sender_reference, TITAN_SIZE32 message_size, TITAN_HANDLE32 message_type, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }

      public:
         // Instantiator methods:
         // I1: Create an instance for supplied reference
         virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I2: Destroy a supplied instance
         virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I3: Load an instance from supplied file/name into supplied reference
         virtual titan_result_t AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I4: Save an instance to supplied file/name 
         virtual titan_result_t AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I5: Return the simulation object of supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I6: Get subordinate instance of supplied parent instance
         virtual TITAN_HANDLE64 AtlasSim_GetSub(TITAN_HANDLE64 sub_concept_data, TITAN_HANDLE64 parent_instance, TITAN_HANDLE64 parent_instance_element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I7: Get size, in bytes, of supplied instance
         virtual TITAN_SIZE64 AtlasSim_GetSize(TITAN_ULONG num_elements, TITAN_HANDLE64 params, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I8: Set supplied instance value
         virtual titan_result_t AtlasSim_SetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I9: Get supplied instance value 
         virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // IA: Set number of elements in supplied instance
         virtual titan_result_t AtlasSim_SetElementCount(TITAN_HANDLE64 instance, TITAN_ULONG num_elements, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // IB: Get number of elements in supplied instance
         virtual TITAN_ULONG AtlasSim_GetElementCount(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IC: Get an element, specified by index, from supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetElement(TITAN_HANDLE64 instance, TITAN_ULONG element_index, TITAN_HANDLE64 handshake = 0) { return 0; }
         // ID: Get the first element in supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetFirstElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IE: Get next element of supplied instance element
         virtual TITAN_HANDLE64 AtlasSim_GetNextElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IF: Add an element to supplied instance
         virtual TITAN_HANDLE64 AtlasSim_AddElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IG: Remove supplied element in supplied instance
         virtual titan_result_t AtlasSim_RemoveElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }

      protected:
         // Inherited variables:
         static _TitanToolsAPI                        *Tools;                                // Tools interface
         static _TitanMemoryAPI                       *Memory;                               // Memory interface
         static _TitanFileAPI                         *File;                                 // File interface
         // Cognitive APIs:
         _AtlasCognitiveAPI                           *Cognitive;                             // Atlas cognitive interface
         _AtlasDomainAPI                              *Domain;                                // Atlas domain interface
         _AtlasSimulationAPI                          *Simulation;                            // Atlas simulation interface
         _AtlasLabelAPI                               *Label;                                 // Atlas label interface 
         _AtlasAgentAPI                               *Agent;                                 // Atlas agent interface
         _AtlasReferenceAPI                           *Reference;                             // Atlas reference interface
         _AtlasGroupAPI                               *Group;                                 // Atlas group interface
         _AtlasDirectiveAPI                           *Directive;                             // Atlas directive interface
         _AtlasLedgerAPI                              *Ledger;                                // Atlas ledge interface
         _AtlasInstanceAPI                            *Instance;                              // Atlas instance interface
         // Simulation APIs:
         #ifdef ATLAS_VR
         _AtlasArmatureAPI                            *Armature;                             // Armature interface
         _AtlasManifoldAPI                            *Manifold;                             // Manifold interface
         _AtlasPoseAPI                                *Pose;                                 // Pose interface
         _AtlasPoseAPI                                *ObjectPose;                           // Object interface
         #endif
      };

   // Titan axiom class definition   
   class TitanAxiom : public _AtlasInstantiatorAPI, public AtlasAxiomInterface
      {
      // Axiom version
      #define TITAN_AXIOM_VERSION                     1

      public:
         // Framework functions:
         TitanAxiom(TITAN_HANDLE64 params);
         virtual ~TitanAxiom();

      public:
         // Axiom interface methods:
         virtual TITAN_ULONG Atlas_Version() { return TITAN_AXIOM_VERSION; }
         // T1: Attach concept to the knowledge base
         virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params) { return TITAN_RESULT_OK; }
         // T2: Detach concept from the knowledge base
         virtual titan_result_t AtlasKB_Detach(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T3: Attach concept to the simulation
         virtual titan_result_t AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T4: Detach concept from the simulation
         virtual titan_result_t AtlasSim_Detach(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T5: Link concept to other concepts in the simulation
         virtual titan_result_t AtlasSim_Link(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T6: Unlink concept from other concepts in the simulation
         virtual titan_result_t AtlasSim_Unlink(TITAN_HANDLE64 concept) { return TITAN_RESULT_OK; }
         // T7: Observe a pinged directive in the domain
         virtual titan_result_t AtlasSim_Observe(TITAN_HANDLE64 observed_reference, TITAN_HANDLE64 observer_reference, TITAN_HANDLE64 ping_type, TITAN_HANDLE64 directive, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }
         // T8: Hear an expression in the domain
         virtual titan_result_t AtlasSim_Hear(TITAN_HANDLE64 hearing_reference, TITAN_HANDLE64 telling_reference, TITAN_HANDLE64 tell_type, const TITAN_STRING expression, TITAN_BITMAP64 directive_params, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }
         // T9: Receive a datagram in the domain
         virtual titan_result_t AtlasSim_Receive(TITAN_HANDLE64 receiver_reference, TITAN_HANDLE64 sender_reference, TITAN_SIZE32 message_size, TITAN_HANDLE32 message_type, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_OK; }

      public:
         // Instantiator methods:
         // I1: Create an instance for supplied reference
         virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I2: Destroy a supplied instance
         virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I3: Load an instance from supplied file/name into supplied reference
         virtual titan_result_t AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I4: Save an instance to supplied file/name 
         virtual titan_result_t AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I5: Return the simulation object of supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I6: Get subordinate instance of supplied parent instance
         virtual TITAN_HANDLE64 AtlasSim_GetSub(TITAN_HANDLE64 sub_concept_data, TITAN_HANDLE64 parent_instance, TITAN_HANDLE64 parent_instance_element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I7: Get size, in bytes, of supplied instance
         virtual TITAN_SIZE64 AtlasSim_GetSize(TITAN_ULONG num_elements, TITAN_HANDLE64 params, TITAN_HANDLE64 handshake = 0) { return 0; }
         // I8: Set supplied instance value
         virtual titan_result_t AtlasSim_SetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // I9: Get supplied instance value 
         virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // IA: Set number of elements in supplied instance
         virtual titan_result_t AtlasSim_SetElementCount(TITAN_HANDLE64 instance, TITAN_ULONG num_elements, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }
         // IB: Get number of elements in supplied instance
         virtual TITAN_ULONG AtlasSim_GetElementCount(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IC: Get an element, specified by index, from supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetElement(TITAN_HANDLE64 instance, TITAN_ULONG element_index, TITAN_HANDLE64 handshake = 0) { return 0; }
         // ID: Get the first element in supplied instance
         virtual TITAN_HANDLE64 AtlasSim_GetFirstElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IE: Get next element of supplied instance element
         virtual TITAN_HANDLE64 AtlasSim_GetNextElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IF: Add an element to supplied instance
         virtual TITAN_HANDLE64 AtlasSim_AddElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) { return 0; }
         // IG: Remove supplied element in supplied instance
         virtual titan_result_t AtlasSim_RemoveElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) { return TITAN_RESULT_NOT_SUPPORTED; }

      protected:
         // Inherited variables:
         static _TitanToolsAPI                        *Tools;                                 // Tools interface
         static _TitanMemoryAPI                       *Memory;                                // Memory interface
         static _TitanFileAPI                         *File;                                  // File interface
         // Cognitive APIs:
         _AtlasCognitiveAPI                           *Cognitive;                             // Atlas cognitive interface
         _AtlasDomainAPI                              *Domain;                                // Atlas domain interface
         _AtlasSimulationAPI                          *Simulation;                            // Atlas simulation interface
         _AtlasLabelAPI                               *Label;                                 // Atlas label interface 
         _AtlasAgentAPI                               *Agent;                                 // Atlas agent interface
         _AtlasReferenceAPI                           *Reference;                             // Atlas reference interface
         _AtlasGroupAPI                               *Group;                                 // Atlas group interface
         _AtlasDirectiveAPI                           *Directive;                             // Atlas directive interface
         _AtlasLedgerAPI                              *Ledger;                                // Atlas ledge interface
         _AtlasInstanceAPI                            *Instance;                              // Atlas instance interface
         // Simulation APIs:
         #ifdef ATLAS_VR
         _AtlasArmatureAPI                            *Armature;                             // VR armature (VRA) interface
         _AtlasManifoldAPI                            *Manifold;                             // VR manifold (VRM) interface
         _AtlasObjectAPI                              *Object;                               // VR object (VRO) interface
         _AtlasPoseAPI                                *ObjectPose;                           // VR object pose (VROP) interface
         #endif
      };
   #endif

#endif

/*
   End of file
*/
