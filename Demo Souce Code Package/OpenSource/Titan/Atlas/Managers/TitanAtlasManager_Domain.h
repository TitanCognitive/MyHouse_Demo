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

#ifndef TITAN_ATLAS_MANAGER_DOMAIN_H
#define TITAN_ATLAS_MANAGER_DOMAIN_H

   // Atlas domain request types
   enum atlas_domain_request_t
      {
      ATLAS_DOMAIN_CREATE = 0,                       // Request domain creation (handled by creating domain, not the created domain).
      ATLAS_DOMAIN_DESTROY,                           // Request domain destruction.
      ATLAS_DOMAIN_ENTER,                             // Request for an agent to enter the domain.
      ATLAS_DOMAIN_EXIT,                              // Request for an agent to exit the domain.
      ATLAS_DOMAIN_CREATE_ZONE,                       // Request zone creation.
      ATLAS_DOMAIN_DESTROY_ZONE,                      // Request zone destruction.
      ATLAS_DOMAIN_START_SIMULATION,                  // Request simulation to be started.
      ATLAS_DOMAIN_STOP_SIMULATION,                   // Request simulation tp be stopped.
      ATLAS_DOMAIN_LEARN_CONCEPT,                     // Request concept to be learned.
      ATLAS_DOMAIN_FORGET_CONCEPT,                    // Request concept to be forgotten.
      ATLAS_DOMAIN_GET_CONCEPT,                       // Request concept id.
      ATLAS_DOMAIN_READ_MANIFEST,                     // Request Atlas to read a manifest. (See Manifest).
      ATLAS_DOMAIN_CREATE_TERMINAL,                   // Request Atlas to create a terminal.
      ATLAS_DOMAIN_CREATE_DEVICE_INTERFACE,           // Request Atlas to create a device interface.
      ATLAS_DOMAIN_CREATE_DEVICE,                     // Request Atlas to create a device.
      ATLAS_DOMAIN_CREATE_DEVICE_PROXY,               // Request Atlas to create a device proxy.
      ATLAS_DOMAIN_CREATE_AGENT,                      // Request Atlas to create a context agent.
      ATLAS_DOMAIN_DESTROY_AGENT,                     // Request Atlas to destroy a context agent.
      ATLAS_DOMAIN_REGISTER_EXPERT,                   // Request Atlas to register an expert.
      ATLAS_DOMAIN_UNREGISTER_EXPERT,                 // Request Atlas to unregister an expert.
      ATLAS_DOMAIN_ADD_LABEL,                         // Request for the domain to add a new label for one of its references.
      ATLAS_DOMAIN_REMOVE_LABEL,                      // Request for the domain to remove an existing label from one of its references.
      ATLAS_DOMAIN_LOAD_MANIFEST,                     // Request for the domain to load a manifest of concepts.
      };

   // Atlas domain manager "create agent" request structure
   struct atlas_domain_request_create_agent_t
      {
      TITAN_HANDLE64                                  agent_label;                           // Label of the agent being created.
      TITAN_HANDLE64                                  domain_label;                          // Label of the domain entered by the new context agent.
      TITAN_HANDLE64                                  zone_label;                            // Label of the zone entered by the new context agent.
      TITAN_HANDLE64                                  concept;                               // Concept managing this agent.
      TITAN_SIZE64                                    memory_units;                          // Memory units requested by the context agent.
      TITAN_USHORT                                    simulation_level;                      // Simulation level of the context agent.
      };

   // Atlas domain manager "create" request structure
   struct atlas_domain_request_create_t
      {
      TITAN_HANDLE64                                  domain_label;                          // Label of the domain being created.
      TITAN_HANDLE64                                  zone_label;                            // Label of the zone being created.
      TITAN_USHORT                                    simulation_level;                      // Domain maximum simulation level.
      };

   // Atlas domain manager "create zone" request structure
   struct atlas_domain_request_create_zone_t
      {
      TITAN_HANDLE64                                  domain_label;                          // Label of the domain being created.
      TITAN_HANDLE64                                  zone_label;                            // Label of the zone being created.
      };

   // Atlas domain manager "create simulation" request structure
   struct atlas_domain_request_start_simulation_t
      {
      TITAN_HANDLE64                                  domain_label;                          // Label of the domain hosting the simulation
      TITAN_FLOAT                                     frequency;                             // Simulation frequency
      TITAN_INT                                       history_size;                          // History size of the simulation
      TITAN_SIZE64                                    attention_level;                       // Attention level of the simulation
      TITAN_HANDLE64                                  simulation_label;                      // Label of the simulation being created.
      };

   // DX: Domain manager method prototype
   typedef titan_result_t (*AtlasDomainManager)(TITAN_HANDLE64 caller_id, atlas_domain_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
