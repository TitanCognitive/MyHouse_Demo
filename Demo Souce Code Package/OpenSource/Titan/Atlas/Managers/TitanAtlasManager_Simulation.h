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

#ifndef TITAN_ATLAS_MANAGER_SIMULATION_H
#define TITAN_ATLAS_MANAGER_SIMULATION_H

   // Atlas simulation request types
   enum atlas_simulation_request_t
      {
      ATLAS_SIMULATION_DESTROY,                       // Request simulation destruction.
      ATLAS_SIMULATION_ENTER,                         // Request for a context agent to join the simulation.
      ATLAS_SIMULATION_LEAVE,                         // Request for a context agent to leave the simulation.
      ATLAS_SIMULATION_CREATE_AGENT,                  // Request an Atlas clone to be created.
      ATLAS_SIMULATION_DESTROY_AGENT,                 // Request an Atlas clone to be destroyed.
      ATLAS_SIMULATION_JOIN_AGENT,                    // Request from a context agent to join an Atlas clone.
      ATLAS_SIMULATION_LEAVE_AGENT,                   // Request from a context agent to leave an Atlas clone.
      ATLAS_SIMULATION_CREATE_OBJECT,                 // Request creation of a simulation object.
      ATLAS_SIMULATION_REGISTER_ACTIVITY,             // Request from a context agent to register an activity with its hosting Atlas clone.
      };

   // Atlas simulation manager "create agent" request structure
   struct atlas_simulation_request_create_agent_t
      {
      TITAN_HANDLE64                                  simulation_label;                      // Simulation label.
      TITAN_HANDLE64                                  clone_label;                           // Simulation clone label.
      TITAN_FLOAT                                     frame_time_ms;                         // Activity frame time in milliseconds.
      };

   // Atlas reference manager "set activity" request structure
   struct atlas_simulation_request_activity_t
      {
      TITAN_HANDLE64                                  clone_id;                              // Simulation clone responsible for the activity.
      TITAN_HANDLE64                                  reference;                             // Handle of the reference being queried.
      TITAN_HANDLE64                                  directive;                             // Activity requested.
      };

   // SX: Simulation manager method prototype
   typedef titan_result_t (*AtlasSimulationManager)(TITAN_HANDLE64 caller_id, atlas_simulation_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
