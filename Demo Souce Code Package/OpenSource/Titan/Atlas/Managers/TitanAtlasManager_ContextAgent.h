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

#ifndef TITAN_ATLAS_MANAGER_CONTEXT_AGENT_H
#define TITAN_ATLAS_MANAGER_CONTEXT_AGENT_H

   // Atlas context agent request types
   enum atlas_agent_request_t
      {
      ATLAS_AGENT_DESTROY,                            // Request domain destruction.
      ATLAS_AGENT_REGISTER_MANAGER,                   // Request for the context agent to register a manager.
      ATLAS_AGENT_JOIN,                               // Request for the context agent to join a clone.
      ATLAS_AGENT_LEAVE,                              // Request for the context agent to leave a clone.
      ATLAS_AGENT_ENTER_DOMAIN,                       // Request for the context agent to enter a domain.
      ATLAS_AGENT_EXIT_DOMAIN,                        // Request for the context agent to exit a domain.
      ATLAS_AGENT_ENTER_ZONE,                         // Request for the context agent to enter a zone.
      ATLAS_AGENT_EXIT_ZONE,                          // Request for the context agent to exit a zone.
      ATLAS_AGENT_CREATE_REFERENCE,                   // Request for the context agent to create a reference.
      ATLAS_AGENT_DESTROY_REFERENCE,                  // Request for the context agent to destroy a reference.
      ATLAS_AGENT_LOAD_REFERENCE,                     // Request for the context agent to load a reference.
      ATLAS_AGENT_SAVE_REFERENCE,                     // Request for the context agent to save a reference.
      ATLAS_AGENT_ADD_LABEL,                          // Request for the context agent to add a new label for one of its references.
      ATLAS_AGENT_REMOVE_LABEL,                       // Request for the context agent to remove an existing label from one of its references.
      ATLAS_AGENT_REGISTER_ACTIVITY,                  // Request from a context agent to register an activity with its hosting Atlas clone.
      ATLAS_AGENT_LOAD_MANIFEST,                      // Request for the context agent to load a manifest of concepts into that manager's hosting domain.
      };

   // Atlas context agent manager zone access request structure
   struct atlas_context_agent_request_access_t
      {
      TITAN_HANDLE64                                  domain_label;                          // Label of the domain entered by the new context agent.
      TITAN_HANDLE64                                  zone_label;                            // Label of the zone entered by the new context agent.
      TITAN_HANDLE64                                  agent_id;                              // Handle of the agent entering the domain.
      TITAN_HANDLE64                                  reference;                             // Reference requesting agent context access.
      };

   // Atlas context agent reference request structure
   struct atlas_context_agent_request_reference_t
      {
      TITAN_HANDLE64                                  clone_label;                           // Label of the clone hosting current context agent.
      TITAN_HANDLE64                                  agent_id;                              // Handle of the agent hosting the new reference.
      TITAN_HANDLE64                                  reference;                             // Reference being updated (or created if 0).
      TITAN_HANDLE64                                  label_handle;                          // Label of the reference bing updated (or 0 if no label update).
      TITAN_BITMAP64                                  agency;                                // Agency bits (see ATLAS_AGENCY_* flags).
      };

   // Atlas context agent manager "set activity" request structure
   struct atlas_context_agent_request_activity_t
      {
      TITAN_HANDLE64                                  agent_id;                              // Handle of the context agent of the reference requesting the activity.
      TITAN_HANDLE64                                  clone_label;                           // Simulation clone that will be hosting the activity.
      TITAN_HANDLE64                                  reference;                             // Handle of the reference requesting the activity.
      TITAN_HANDLE64                                  directive;                             // Activity requested.
      };

   // AX: Context agent manager method prototype
   typedef titan_result_t (*AtlasAgentManager)(TITAN_HANDLE64 caller_id, atlas_agent_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
