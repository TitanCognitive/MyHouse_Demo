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

#ifndef TITAN_ATLAS_API_H
#define TITAN_ATLAS_API_H
    
   // Atlas instance interface 
   typedef struct _AtlasInstantiatorAPI
      {
      // I1: Create an instance for supplied reference
      virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0) = 0;
      // I2: Destroy a supplied instance
      virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) = 0;
      // I3: Load an instance from supplied file/name into supplied reference
      virtual titan_result_t AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake = 0) = 0;
      // I4: Save an instance to supplied file/name 
      virtual titan_result_t AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) = 0;
      // I5: Return the simulation object of supplied instance
      virtual TITAN_HANDLE64 AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) = 0;
      // I6: Get subordinate instance of supplied parent instance
      virtual TITAN_HANDLE64 AtlasSim_GetSub(TITAN_HANDLE64 sub_concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) = 0;
      // I7: Get size, in bytes, of supplied instance
      virtual TITAN_SIZE64 AtlasSim_GetSize(TITAN_ULONG num_elements, TITAN_HANDLE64 params, TITAN_HANDLE64 handshake = 0) = 0;
      // I8: Set supplied instance value
      virtual titan_result_t AtlasSim_SetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) = 0;
      // I9: Get supplied instance value 
      virtual titan_result_t AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) = 0;
      // IA: Set number of elements in supplied instance
      virtual titan_result_t AtlasSim_SetElementCount(TITAN_HANDLE64 instance, TITAN_ULONG num_elements, TITAN_HANDLE64 handshake = 0) = 0;
      // IB: Get number of elements in supplied instance
      virtual TITAN_ULONG AtlasSim_GetElementCount(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) = 0;
      // IC: Get an element, specified by index, from supplied instance
      virtual TITAN_HANDLE64 AtlasSim_GetElement(TITAN_HANDLE64 instance, TITAN_ULONG element_index, TITAN_HANDLE64 handshake = 0) = 0;
      // ID: Get the first element in supplied instance
      virtual TITAN_HANDLE64 AtlasSim_GetFirstElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) = 0;
      // IE: Get next element of supplied instance element
      virtual TITAN_HANDLE64 AtlasSim_GetNextElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) = 0;
      // IF: Add an element to supplied instance
      virtual TITAN_HANDLE64 AtlasSim_AddElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0) = 0;
      // IG: Remove supplied element in supplied instance
      virtual titan_result_t AtlasSim_RemoveElement(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake = 0) = 0;
      } *AtlasInstantiatorAPI;

   // Atlas cognitive interface
   typedef struct _AtlasCognitiveAPI
      {
      // C1: Get an interface specified by name and ID in the domain
      virtual TITAN_POINTER GetInterface(TITAN_HANDLE64 interface_id, TITAN_HANDLE64 interface_label = 0, TITAN_HANDLE64 handshake = 0) = 0;
      // C2: Set the concept's instance manager (instantiator)
      virtual titan_result_t SetInstantiator(AtlasInstantiatorAPI instance_interface) = 0;
      // C3: Register Do method with the knowledge base
      virtual titan_result_t Do(TITAN_UINT perspective_index, const TITAN_STRING method_name, TITAN_USHORT simulation_index, TITAN_HANDLE64 code_handle, AtlasSimulationMethod method, TITAN_BITMAP64 method_flags = 0) = 0;
      // C4: Register Be method with the knowledge base
      virtual titan_result_t Be(TITAN_UINT perspective_index, const TITAN_STRING method_name, TITAN_USHORT simulation_index, TITAN_HANDLE64 code_handle, AtlasSimulationMethod method, TITAN_BITMAP64 method_flags = 0) = 0;
      // C5: Register Have method with the knowledge base
      virtual titan_result_t Have(TITAN_UINT perspective_index, const TITAN_STRING method_name, TITAN_USHORT simulation_index, TITAN_HANDLE64 code_handle, AtlasSimulationMethod method, TITAN_BITMAP64 method_flags = 0) = 0;
      // C6: Get a concept specified by name in the domain
      virtual TITAN_HANDLE64 GetConcept(TITAN_HANDLE64 caller_id , const TITAN_STRING concept_name, TITAN_HANDLE64 handshake = 0) = 0;
      // C7: Get subordinate concept of calling concept by name
      virtual TITAN_HANDLE64 GetSubconcept(const TITAN_STRING sub_concept_name) = 0;
      // C8: Return TRUE if the specified reference is compatible with the calling concept
      virtual TITAN_BOOL IsCompatible(TITAN_HANDLE64 reference) = 0;
      // C9: Set a sub-concept's instance manager (instantiator)
      virtual titan_result_t SetSubinstantiator(const TITAN_STRING concept_name, TITAN_HANDLE64 sub_concept_data) = 0;
      // CA: Add an attribute, specified by ID, to the concept
      virtual TITAN_POINTER AddAttribute(atlas_attribute_scope_t feature_scope, TITAN_BITMAP64 feature_id, TITAN_HANDLE64 sim_handshake = 0) = 0;
      // CB: Pause Atlas
      virtual titan_result_t Pause(TITAN_HANDLE64 sim_handshake = 0) = 0;
      // CC: Resume Atlas
      virtual titan_result_t Resume(TITAN_HANDLE64 sim_handshake = 0) = 0;
      // CD: Shutdown Atlas
      virtual titan_result_t ShutDown(TITAN_HANDLE64 sim_handshake = 0) = 0;
      // Return: virtual TITAN_BOOL GetType(TITAN_HANDLE64 public_reference) = 0;
      } *AtlasCognitiveAPI;

   // Atlas domain interface
   typedef struct _AtlasDomainAPI
      {
      // C1: Create a domain by name
      virtual TITAN_HANDLE64 Create(TITAN_HANDLE64 caller_id, const TITAN_STRING domain_name, const TITAN_STRING zone_name, TITAN_USHORT simulation_level, atlas_method_t *domain_manager = NULL, atlas_method_t *zone_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // C2: Create a domain by label
      virtual TITAN_HANDLE64 _Create(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 domain_label, TITAN_HANDLE64 zone_label, TITAN_USHORT simulation_level, atlas_method_t *domain_manager = NULL, atlas_method_t *zone_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // C3: Destroy a domain by handle
      virtual titan_result_t Destroy(TITAN_HANDLE64 domain, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // C4: Create a context zone, by name, in the domain
      virtual TITAN_HANDLE64 CreateZone(TITAN_HANDLE64 caller_id, const TITAN_STRING zone_name, atlas_method_t *zone_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // C5: Create a context zone, by label, in the domain
      virtual TITAN_HANDLE64 _CreateZone(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 zone_label, atlas_method_t *zone_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // C6: Destroy a context zone by handle
      virtual titan_result_t DestroyZone(TITAN_HANDLE64 zone, TITAN_HANDLE64 zone_handshake = 0) = 0;
      // C7: Start a simulation, by name, in the domain
      virtual titan_result_t StartSimulation(TITAN_HANDLE64 caller_id, TITAN_FLOAT frequency, TITAN_SIZE64 attention_level, TITAN_INT history_size, atlas_method_t *simulation_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // C8: Stop a simulation 
      virtual titan_result_t StopSimulation(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      // C9: Load a supplied manifest in the domain hosting the caller
      virtual titan_result_t LoadManifest(TITAN_HANDLE64 caller_id, TITAN_STRING manifest, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      } *AtlasDomainAPI;

   // Atlas simulation interface
   typedef struct _AtlasSimulationAPI
      {
      // S1: Get simulation time since creation in seconds
      virtual TITAN_FLOAT GetTime(TITAN_HANDLE64 caller_id) = 0;
      // S2: Create an Atlas clone within the domain hosting the simulation
      virtual TITAN_HANDLE64 CreateAtlasClone(TITAN_HANDLE64 caller_id, const TITAN_STRING clone_name, TITAN_FLOAT frequency, TITAN_ULONG attention_level, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      // S3: Destroy an existing Atlas clone within the domain hosting the simulation
      virtual titan_result_t DestroyAtlasClone(TITAN_HANDLE64 clone, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      // S4: Create a simulation object and return its handle
      virtual TITAN_HANDLE64 CreateSimulationObject(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      // S6: Set the parent object of the simulation object
      virtual titan_result_t SetSimulationObjectParent(TITAN_HANDLE64 simulation_object, TITAN_HANDLE64 parent_object, TITAN_HANDLE64 joint = 0) = 0;
      // S7: Load a simulation object from an existing file
      virtual titan_result_t LoadSimulationObject(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 concept, TITAN_HANDLE64 &out_simulation_object) = 0;
      // S8: Save an existing simulation object to file
      virtual titan_result_t SaveSimulationObject(TITAN_HANDLE64 file, TITAN_HANDLE64 simulation_object) = 0;
      // S9: Activate processing of a simulation object
      virtual titan_result_t ActivateSimulationObject(TITAN_HANDLE64 simulation_object) = 0;
      // SA: Deactivate processing of simulation object
      virtual titan_result_t DeactivateSimulationObject(TITAN_HANDLE64 simulation_object) = 0;
      // SB: Destroy an existing simulation object
      virtual void DestroySimulationObject(TITAN_HANDLE64 simulation_object) = 0;
      } *AtlasSimulationAPI;

   // Atlas agent interface
   typedef struct _AtlasAgentAPI
      {
      // A1: Create a context agent by name
      virtual TITAN_HANDLE64 Create(const TITAN_STRING domain_name, const TITAN_STRING zone_name, TITAN_SIZE64 memory_units,
                                    TITAN_HANDLE64 &out_reference, const TITAN_STRING agent_name, TITAN_HANDLE64 concept, 
                                    TITAN_HANDLE64 instance_params = 0, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 zone_handshake = 0, 
                                    TITAN_HANDLE64 instance_handshake = 0) = 0;
      // A2: Create a context agent by label
      virtual TITAN_HANDLE64 _Create(TITAN_HANDLE64 domain_label, TITAN_HANDLE64 zone_label, TITAN_SIZE64 memory_units,
                                     TITAN_HANDLE64 &out_reference, TITAN_HANDLE64 agent_label, TITAN_HANDLE64 concept, 
                                     TITAN_HANDLE64 instance_params = 0, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 zone_handshake = 0, 
                                     TITAN_HANDLE64 instance_handshake = 0) = 0;
      // A3: Destroy an existing context agent specified by handle
      virtual titan_result_t Destroy(TITAN_HANDLE64 agent_id, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // A4: Register a context agent manager
      virtual titan_result_t RegisterManager(TITAN_HANDLE64 agent_id, const atlas_method_t *agent_manager, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // A5: Join/subscribe a context agent to an Atlas clone
      virtual titan_result_t JoinAtlasClone(TITAN_HANDLE64 agent_id, const TITAN_STRING clone_name, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      // A6: Leave/unsubscribe a context agent from an Atlas clone
      virtual titan_result_t LeaveAtlasClone(TITAN_HANDLE64 agent_id) = 0;
      // A7: Enter a context zone by name
      virtual titan_result_t EnterZone(TITAN_HANDLE64 caller_id, const TITAN_STRING domain_name, const TITAN_STRING zone_name, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 zone_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // A8: Enter a context zone by label
      virtual titan_result_t _EnterZone(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 domain_label, TITAN_HANDLE64 zone_label, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 zone_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // A9: Exit a context zone by label
      virtual titan_result_t ExitZone(TITAN_HANDLE64 caller_id, const TITAN_STRING zone_name, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // AA: Exit a context zone by name
      virtual titan_result_t _ExitZone(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 zonelabel , TITAN_HANDLE64 agent_handshake = 0) = 0;
      // AB: Tell an expression to an expert
      virtual titan_result_t Tell(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 expert, const TITAN_STRING expression, atlas_method_t *method = NULL, TITAN_HANDLE64 handshake = 0) = 0;
      // AC: Ping a directive to a  context agent 
      virtual titan_result_t Ping(TITAN_HANDLE64 source_reference, TITAN_HANDLE64 destination_reference, atlas_ping_type_t ping_type, TITAN_HANDLE64 directive, TITAN_HANDLE64 user_data = 0, TITAN_HANDLE64 handshake = 0) = 0;
      // AD: Send a datagram to a  context agent 
      virtual titan_result_t Send(TITAN_HANDLE64 source_reference, TITAN_HANDLE64 destination_reference, TITAN_HANDLE32 message_type, TITAN_SIZE32 message_size, TITAN_HANDLE64 user_data = 0, TITAN_HANDLE64 handshake = 0) = 0;
      // AE: Execute a script specified by name
      virtual titan_result_t ExecuteScript(TITAN_HANDLE64 caller_id, const TITAN_STRING script_name, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // AF: Get content of an attachment specified by name
      virtual TITAN_HANDLE64 GetAttachment(TITAN_HANDLE64 caller_id, const TITAN_STRING attachment_name) = 0;
      // AG: Get content of an attachment specified by label
      virtual TITAN_HANDLE64 _GetAttachment(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 attachment_label) = 0;
      // AH: Register a context agent as an expert
      virtual titan_result_t RegisterExpert(TITAN_HANDLE64 caller_id, atlas_method_t *expert_manager, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // AI: Unregister a context agent as an expert
      virtual titan_result_t UnregisterExpert(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // AJ: Observe another context agent in the domain
      virtual titan_result_t Observe(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 observed_reference, TITAN_BOOL observe = true,
                                     TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 observed_handshake = 0, TITAN_HANDLE64 observer_handshake = 0) = 0;
      // AK: Start a new activity of a context agent by its hosting clone.
      virtual TITAN_HANDLE64 StartActivity(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 directive, atlas_method_t *method, 
                                           TITAN_HANDLE64 simulation_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // AL: Pause an existing activity of a context agent.
      virtual titan_result_t PauseActivity(TITAN_HANDLE64 activity) = 0;
      // AM: Resume an existing activity of a context agent.
      virtual titan_result_t ResumeActivity(TITAN_HANDLE64 activity) = 0;
      // AN: Stop an existing activity of a context agent.
      virtual titan_result_t StopActivity(TITAN_HANDLE64 caller_id) = 0;
      } *AtlasAgentAPI;

   // Atlas reference interface
   typedef struct _AtlasReferenceAPI
      {
      // R1: Add a reference, specified by name, to an existing instance/element 
      virtual TITAN_HANDLE64 Add(TITAN_HANDLE64 caller_id, const TITAN_STRING name, TITAN_HANDLE64 concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element = 0, TITAN_BITMAP64 agency = ATLAS_AGENCY_NONE, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // R2: Add a reference, specified by label, to an existing instance/element 
      virtual TITAN_HANDLE64 _Add(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 label, TITAN_HANDLE64 concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element = 0, TITAN_BITMAP64 agency = ATLAS_AGENCY_NONE, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // R3: Create an instance with a reference specified by name 
      virtual TITAN_HANDLE64 Create(TITAN_HANDLE64 caller_id, const TITAN_STRING name, TITAN_HANDLE64 concept, TITAN_ULONG num_elements = 1, TITAN_HANDLE64 instance_params = 0, TITAN_BITMAP64 agency = ATLAS_AGENCY_NONE, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // R4: Create an instance with a reference specified by label
      virtual TITAN_HANDLE64 _Create(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 label, TITAN_HANDLE64 concept, TITAN_ULONG num_elements = 1, TITAN_HANDLE64 instance_params = 0, TITAN_BITMAP64 agency = ATLAS_AGENCY_NONE, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // R5: Destroy an existing reference specified by handle
      virtual titan_result_t Destroy(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 reference_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // R6: Register a reference manager
      virtual titan_result_t RegisterManager(TITAN_HANDLE64 caller_id, atlas_method_t *reference_manager, TITAN_HANDLE64 reference_handshake = 0) = 0;
      // R7: Request a public handle for the reference to be used by other concepts
      virtual TITAN_HANDLE64 GetPublicHandle(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 reference_handshake = 0) = 0;
      // R8: Load a reference from an existing file
      virtual titan_result_t Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 concept, const TITAN_STRING name, TITAN_HANDLE64 &out_id, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 reference_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // R9: Save a reference to a file
      virtual titan_result_t Save(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 reference, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 reference_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // RA: Observe another reference in the domain
      virtual titan_result_t Observe(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 observed_reference, TITAN_BOOL observe = true, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 observed_handshake = 0, TITAN_HANDLE64 observer_handshake = 0) = 0;
      // RB: Get information of a reference specified 
      virtual titan_result_t GetInfo(TITAN_HANDLE64 reference, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 reference_handshake = 0) = 0;
      // RC: Get spatial-temporal transform of a reference 
      virtual titan_result_t GetTransform(TITAN_HANDLE64 reference, titan_mat4_t &transform, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 reference_handshake = 0) = 0;
      } *AtlasReferenceAPI;

   // Atlas instance interface
   typedef struct _AtlasInstanceAPI
      {
      // M1: Create an instance based on a supplied concept and a number of elements
      virtual titan_result_t Create(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, TITAN_HANDLE64 &out_instance, TITAN_ULONG num_elements = 1, TITAN_HANDLE64 instance_params = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // M2: Destroy an existing instance specified by name
      virtual titan_result_t Destroy(TITAN_HANDLE64 concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // M3: Load an instance from an existing file and a concept specified by handle
      virtual titan_result_t Load(TITAN_HANDLE64 load_id, TITAN_HANDLE64 file, TITAN_HANDLE64 concept, TITAN_HANDLE64 &out_instance, TITAN_HANDLE64 &out_element, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // M4: Save an existing instance of a concept, specified by handle, into supplied file
      virtual titan_result_t Save(TITAN_HANDLE64 file, TITAN_HANDLE64 concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element = 0, TITAN_HANDLE64 agent_handshake = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // M5: Get the value, specified by label, of a supplied instance
      virtual titan_result_t GetValue(TITAN_HANDLE64 concept, atlas_value_t &value, TITAN_HANDLE64 instance, TITAN_HANDLE64 element = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // M6: Set the value, specified by label, of a supplied instance
      virtual titan_result_t SetValue(TITAN_HANDLE64 concept, atlas_value_t &value, TITAN_HANDLE64 instance, TITAN_HANDLE64 element = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      // M7: Set the simulation object of a supplied instance
      virtual titan_result_t GetSimulationObject(TITAN_HANDLE64 concept, TITAN_HANDLE64 &out_object, TITAN_HANDLE64 instance, TITAN_HANDLE64 element = 0, TITAN_HANDLE64 instance_handshake = 0) = 0;
      } *AtlasInstanceAPI;

   // Atlas group interface
   typedef struct _AtlasGroupAPI
      {
      // G1: Create a group
      virtual TITAN_HANDLE64 Create(atlas_method_t *group_manager = NULL) = 0;
      // G2: Destroy an existing group
      virtual titan_result_t Destroy(TITAN_HANDLE64 group, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G3: Lock a group and prevent future updates until cleared/destroyed
      virtual titan_result_t Lock(TITAN_HANDLE64 group, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G4: Add an existing reference as a new member of a supplied group
      virtual titan_result_t AddMember(TITAN_HANDLE64 group, TITAN_HANDLE64 reference, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G5: Remove an existing member from a group
      virtual titan_result_t RemoveMember(TITAN_HANDLE64 group, TITAN_HANDLE64 group_member, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G6: Clear all contents of the group and unlock it
      virtual titan_result_t Clear(TITAN_HANDLE64 group, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G7: Get number of members in the group
      virtual TITAN_ULONG GetMemberCount(TITAN_HANDLE64 group, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G8: Get the first member of a supplied group
      virtual TITAN_HANDLE64 GetFirst(TITAN_HANDLE64 group, atlas_reference_info_t &member_data, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT, TITAN_HANDLE64 group_handshake = 0) = 0;
      // G9: Get the next member of supplied member in a group
      virtual TITAN_HANDLE64 GetNext(TITAN_HANDLE64 group_member, atlas_reference_info_t &member_data, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT, TITAN_HANDLE64 group_handshake = 0) = 0;
      } *AtlasGroupAPI;

   // Atlas label interface
   typedef struct _AtlasLabelAPI
      {
      #define A1 GetHandle
      // L1: Get or create a label based on supplied text
      virtual TITAN_HANDLE64 GetHandle(const TITAN_STRING text, TITAN_BOOL create = true) = 0;
      // L2: Copy an existing label's text string into supplied output string
      virtual TITAN_UINT GetString(TITAN_HANDLE64 label, TITAN_STRING output, TITAN_UINT output_max_size = 0) = 0;
      // L3: Get the value, specified by name, of an existing reference 
      virtual titan_result_t GetValue(TITAN_HANDLE64 reference, const TITAN_STRING format, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) = 0;
      // L4: Get the value, specified by label embedded in <value>, of an existing reference 
      virtual titan_result_t _GetValue(TITAN_HANDLE64 reference, atlas_value_t &value, TITAN_HANDLE64 handshake = 0) = 0;
      // L5: Add a new name to an existing reference
      virtual titan_result_t AddLabel(TITAN_HANDLE64 caller_id, const TITAN_STRING label_name, TITAN_HANDLE64 reference, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // L6: Add a new name, specified by label, to an existing reference
      virtual titan_result_t _AddLabel(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 label, TITAN_HANDLE64 reference, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // L7: Remove an name from a supplied reference
      virtual titan_result_t RemoveLabel(TITAN_HANDLE64 caller_id, const TITAN_STRING label_name, TITAN_HANDLE64 reference, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // L8: Remove an existing name, specified by label, from a supplied reference
      virtual titan_result_t _RemoveLabel(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 label, TITAN_HANDLE64 reference, TITAN_HANDLE64 domain_handshake = 0, TITAN_HANDLE64 agent_handshake = 0) = 0;
      // L9: Get the first reference matching supplied name
      virtual TITAN_HANDLE64 GetFirst(TITAN_HANDLE64 caller_id, const TITAN_STRING name, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT) = 0;
      // LA: Get the first reference, specified by label, matching supplied name
      virtual TITAN_HANDLE64 _GetFirst(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 label, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT) = 0;
      // LB: Get the next matching reference matching specified label element (returned by LA, LB)
      virtual TITAN_HANDLE64 GetNext(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 label_element, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT) = 0;
      } *AtlasLabelAPI;

   // Atlas directive interface
   typedef struct _AtlasDirectiveAPI
      {
      // D1: Get a component, specified by ID, from a supplied directive
      virtual TITAN_HANDLE64 Get(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D2:  Get the next available component based on supplied directive component
      virtual TITAN_HANDLE64 GetNext(TITAN_HANDLE64 directive, TITAN_HANDLE64 component, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D3: Get a component, based on relation label, from a supplied directive
      virtual titan_result_t Get(TITAN_HANDLE64 directive, TITAN_HANDLE64 relation, atlas_reference_info_t &reference_info, TITAN_BITMAP64 query_flags = ATLAS_GET_DEFAULT, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D4: Rephrase specified component to supplied reference
      virtual titan_result_t Rephrase(TITAN_HANDLE64 directive, TITAN_HANDLE64 component, TITAN_HANDLE64 reference = 0, TITAN_HANDLE64 reference_handshake = 0, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D5: Get total number of components in a directive
      virtual TITAN_ULONG GetNumComponents(TITAN_HANDLE64 directive, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D6: Get the action data structure of a supplied directive
      virtual titan_result_t GetAction(TITAN_HANDLE64 directive, atlas_action_info_t &action_info, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D7: Get the first state state of a directive, or the next state of a directive if a previous state is supplied
      virtual TITAN_HANDLE64 GetState(TITAN_HANDLE64 directive, TITAN_HANDLE64 previous_state, atlas_value_t &state_info, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D8: Set the handshake of a supplied component in a directive
      virtual titan_result_t SetHandshake(TITAN_HANDLE64 directive, TITAN_HANDLE64 component, TITAN_HANDLE64 component_handshake, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // D9: Clear all directive handshakes
      virtual titan_result_t ClearHandshakes(TITAN_HANDLE64 directive, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // DA: Remember a directive for future invocation
      virtual TITAN_HANDLE64 Remember(TITAN_HANDLE64 directive, atlas_method_t *directive_manager, TITAN_HANDLE64 directive_handshake = 0) = 0;
      // DB: Forget a previously remembered directive
      virtual titan_result_t Forget(TITAN_HANDLE64 directive, TITAN_HANDLE64 directive_handshake = 0) = 0;
      } *AtlasDirectiveAPI;

   // Atlas ledger interface
   typedef struct _AtlasLedgerAPI
      {
      // V1: Create a new ledger by name
      virtual TITAN_HANDLE64 Create(TITAN_HANDLE64 caller_id, const TITAN_STRING ledger_name, TITAN_SIZE64 ledger_buffer_size, atlas_method_t *ledger_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      // V2: Destroy an existing ledger
      virtual void Destroy(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 ledger, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // V3:  Get an existing ledger specified by name
      virtual TITAN_HANDLE64 Get(TITAN_HANDLE64 caller_id, const TITAN_STRING ledger_name, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // V4: Clear content of a supplied ledger
      virtual titan_result_t Clear(TITAN_HANDLE64 ledger, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // V5: Get the supplied ledger message
      virtual TITAN_HANDLE64 GetMessageLine(TITAN_HANDLE64 message, TITAN_UINT &line_index, TITAN_SIZE32 &line_size, TITAN_STRING output_text = NULL, TITAN_SIZE64 output_text_size = 0, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // V6: Get the data of supplied ledger message
      virtual TITAN_POINTER GetMessageData(TITAN_HANDLE64 message, TITAN_USHORT &line_size, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // V7: Insert a new message into a supplied ledger
      virtual TITAN_HANDLE64 InsertMessage(TITAN_HANDLE64 ledger, TITAN_USHORT requested_size, TITAN_HANDLE64 ledger_handshake, const TITAN_STRING format, ...) = 0;
      // V8: Get the earliest message in a supplied ledger
      virtual TITAN_HANDLE64 GetEarliestMessage(TITAN_HANDLE64 ledger, TITAN_UINT &line_index, TITAN_SIZE32 &line_size, TITAN_STRING output_text = NULL, TITAN_SIZE64 output_text_size = 0, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // V9: Get the latest message in a supplied ledger
      virtual TITAN_HANDLE64 GetLatestMessage(TITAN_HANDLE64 ledger, TITAN_UINT &line_index, TITAN_SIZE32 &line_size, TITAN_STRING output_text = NULL, TITAN_SIZE64 output_text_size = 0, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // VA: Get the previous message in a supplied ledger
      virtual TITAN_HANDLE64 GetPreviousMessage(TITAN_HANDLE64 message, TITAN_UINT &line_index, TITAN_SIZE32 &line_size, TITAN_STRING output_text = NULL, TITAN_SIZE64 output_text_size = 0, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // VB: Get the next message in a supplied ledger
      virtual TITAN_HANDLE64 GetNextMessage(TITAN_HANDLE64 message, TITAN_UINT &line_index, TITAN_SIZE32 &line_size, TITAN_STRING output_text = NULL, TITAN_SIZE64 output_text_size = 0, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // VC: Register a ledger keyword
      virtual TITAN_HANDLE64 RegisterKeyword(TITAN_HANDLE64 ledger, const TITAN_STRING keyword, atlas_method_t *keyword_manager, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // VD: Unregister a ledger keyword
      virtual void UnregisterKeyword(TITAN_HANDLE64 ledger, TITAN_HANDLE64 keyword_label, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      // VE: Process a ledger keyword
      virtual titan_result_t ProcessKeyword(TITAN_HANDLE64 ledger, TITAN_HANDLE64 caller_id, const TITAN_STRING input_text, TITAN_HANDLE64 ledger_handshake = 0) = 0;
      } *AtlasLedgerAPI;

   // Atlas location attribute interface
   typedef struct _AtlasAttributeLocationInterface
      {
      virtual TITAN_FLOAT GetVersion() = 0;
      virtual titan_result_t SetCoordinate(TITAN_HANDLE64 object, atlas_coordinate_t *coordinate, titan_mat4_t *transform = NULL, TITAN_BOOL update_trail = false, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t UpdateCoordinate(TITAN_HANDLE64 object, TITAN_HANDLE64 on_flags = TITAN_COORDINATE_TRS, TITAN_HANDLE64 off_flags = 0, TITAN_BOOL update_trail = false, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t GetEarliestCoordinate(TITAN_HANDLE64 object, TITAN_ULONG &current_index, atlas_coordinate_t *out_coordinate, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t GetLatestCoordinate(TITAN_HANDLE64 object, TITAN_ULONG &current_index, atlas_coordinate_t *out_coordinates, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t GetPreviousCoordinate(TITAN_HANDLE64 object, TITAN_ULONG &current_index, atlas_coordinate_t *out_coordinates, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t GetNextCoordinate(TITAN_HANDLE64 object, TITAN_ULONG &current_index, atlas_coordinate_t *out_coordinates, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t GetCoordinates(TITAN_HANDLE64 object, TITAN_ULONG &current_index, atlas_coordinate_t *out_coordinates, TITAN_UINT &num_out_coordinates, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t Pause(TITAN_HANDLE64 object, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      virtual titan_result_t Resume(TITAN_HANDLE64 object, TITAN_HANDLE64 simulation_handshake = 0) = 0;
      } *AtlasAttributeLocationAPI;

   // Atlas size attribute interface
   typedef struct _AtlasAttributeSizeInterface
      {
      virtual TITAN_FLOAT GetVersion() = 0;
      } *AtlasAttributeSizeAPI;

   // Atlas shape attribute interface
   typedef struct _AtlasAttributeShapeInterface
      {
      virtual TITAN_FLOAT GetVersion() = 0;
      } *AtlasAttributeShapeAPI;

   // Atlas texture attribute interface
   typedef struct _AtlasAttributeTextureInterface
      {
      virtual TITAN_FLOAT GetVersion() = 0;
      } *AtlasAttributeTextureAPI;

   // Atlas color attribute interface
   typedef struct _AtlasAttributeColorInterface
      {
      virtual TITAN_FLOAT GetVersion() = 0;
      virtual titan_result_t SetColorData(TITAN_HANDLE64 object, TITAN_FLOAT *color) = 0;
      } *AtlasAttributeColorAPI;

   // Atlas window interface
   typedef struct _AtlasWindowInterface
      {
      virtual TITAN_HANDLE64 Create(const TITAN_STRING window_name, atlas_method_t *app_manager) = 0;
      virtual TITAN_HANDLE64 Get(const TITAN_STRING window_name, TITAN_HANDLE64 app_handshake = 0) = 0;
      virtual titan_result_t Resize(TITAN_HANDLE64 window, TITAN_USHORT left, TITAN_USHORT right, TITAN_USHORT top, TITAN_USHORT bottom) = 0;
      virtual titan_result_t Destroy(TITAN_HANDLE64 window) = 0;
      virtual titan_result_t Show(TITAN_HANDLE64 window) = 0;
      virtual titan_result_t Hide(TITAN_HANDLE64 window) = 0;
      } *AtlasWindowAPI;

   // Atlas device thread interface
   typedef struct _AtlasDeviceThreadInterface
      {
      virtual TITAN_HANDLE64 Create(const TITAN_STRING thread_name, TITAN_DOUBLE time_out, atlas_method_t *thread_manager = NULL) = 0;
      virtual titan_result_t Destroy(TITAN_HANDLE64 device_thread, TITAN_HANDLE64 thread_handshake = 0) = 0;
      virtual titan_result_t Activate(TITAN_HANDLE64 device_thread) = 0;
      virtual titan_result_t Deactivate(TITAN_HANDLE64 device_thread) = 0;
      } *AtlasDeviceThreadAPI;

   // Atlas device interface
   typedef struct _AtlasDeviceInterface
      {
      virtual TITAN_HANDLE64 Create(TITAN_HANDLE64 caller_id, const TITAN_STRING device_name, atlas_method_t *device_manager = NULL, TITAN_HANDLE64 domain_handshake = 0) = 0;
      virtual titan_result_t Destroy(TITAN_HANDLE64 device_handle, TITAN_HANDLE64 device_handshake = 0) = 0;
      virtual titan_result_t Join(TITAN_HANDLE64 device_handle, TITAN_HANDLE64 interface_handle, TITAN_HANDLE64 interface_handshake = 0, TITAN_HANDLE64 device_handshake = 0) = 0;
      virtual titan_result_t Leave(TITAN_HANDLE64 device_handle, TITAN_HANDLE64 interface_handle, TITAN_HANDLE64 device_handshake = 0) = 0;
      virtual titan_result_t Activate(TITAN_HANDLE64 device_handle, TITAN_HANDLE64 device_handshake = 0) = 0;
      virtual titan_result_t Deactivate(TITAN_HANDLE64 device_handle, TITAN_HANDLE64 device_handshake = 0) = 0;
      virtual titan_result_t Reset(TITAN_HANDLE64 device_handle, TITAN_HANDLE64 device_handshake = 0) = 0;
      } *AtlasDeviceAPI;

#endif

/*
   End of file
*/
