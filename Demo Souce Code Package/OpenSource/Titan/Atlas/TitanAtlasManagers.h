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

#ifndef TITAN_ATLAS_MANAGERS_H
#define TITAN_ATLAS_MANAGERS_H

   // Forward declarations
   struct atlas_method_t;
   struct atlas_device_method_t;
   struct atlas_coordinate_t;
   
   // Manager dependencies
   #include "Managers/TitanAtlasManager_ContextAgent.h"
   #include "Managers/TitanAtlasManager_Device.h"
   #include "Managers/TitanAtlasManager_DeviceInterface.h"
   #include "Managers/TitanAtlasManager_DeviceProxy.h"
   #include "Managers/TitanAtlasManager_Directive.h"
   #include "Managers/TitanAtlasManager_Domain.h"
   #include "Managers/TitanAtlasManager_Group.h"
   #include "Managers/TitanAtlasManager_Ledger.h"
   #include "Managers/TitanAtlasManager_Reference.h"
   #include "Managers/TitanAtlasManager_Simulation.h"
   #include "Managers/TitanAtlasManager_Terminal.h"
   #include "Managers/TitanAtlasManager_Zone.h"

   // GENERAL METHODS PROTOTYPES:
   // Generic Atlas method prototype
   typedef titan_result_t (*AtlasMethod)(atlas_method_t *method);
   // Context agent activity method prototype
   typedef void (*ActivityMethod)(atlas_method_t *method);

   // COGNITIVE METHOD PROTOTYPES:
   // Keyword method definition
   typedef titan_result_t(*AtlasKeywordMethod)(TITAN_HANDLE64 caller_id, atlas_method_t *method, const TITAN_STRING keyword_text, TITAN_STRING *tokens, TITAN_UINT num_tokens);
   // Expression manager method prototype
   typedef titan_result_t(*AtlasExpressionManager)(TITAN_HANDLE64 caller_id, atlas_method_t *method, TITAN_HANDLE64 directive, atlas_ping_type_t ping_type, TITAN_HANDLE64 data, TITAN_HANDLE64 handshake);
   // OX: Observation method prototype
   #define ATLAS_OBSERVE_PARAMS                       TITAN_FLOAT sim_time, TITAN_HANDLE64 code_handle, TITAN_HANDLE64 data_handle, TITAN_HANDLE64 directive, TITAN_HANDLE64 handshake
   typedef void (*AtlasObservationMethod)(ATLAS_OBSERVE_PARAMS);

   // SIMULATION METHOD PROTOTYPES:
   // Simulation trigger method prototype
   typedef void(*AtlasSimulationTrigger)(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags);
   // Definition of simulation parameters
   #define ATLAS_SIMULATE_PARAMS                      TITAN_HANDLE64 user_data, TITAN_HANDLE64 directive, atlas_directive_header_t directive_header
   // YX: Simulation method prototype
   typedef atlas_probe_result_t (*AtlasSimulationMethod)(ATLAS_SIMULATE_PARAMS); 

   // Atlas method structure
   struct atlas_method_t
      {
      // Manager methods
      union
         {
         AtlasDomainManager                           DomainManager;                         // DX: Domain manager method         
         AtlasZoneManager                             ZoneManager;                           // ZX: Context zone manager method         
         AtlasSimulationManager                       SimulationManager;                     // SX: Simulation manager method         
         AtlasAgentManager                            AgentManager;                          // AX: Context agent manager method         
         AtlasReferenceManager                        ReferenceManager;                      // RX: Reference manager method         
         AtlasGroupManager                            GroupManager;                          // GX: Group manager method       
         AtlasLedgerManager                           LedgerManager;                         // LX: Ledger manager method
         AtlasObservationMethod                       ObservationMethod;                     // OX: Observation method
         AtlasSimulationMethod                        SimulationMethod;                      // YX: Simulation method
         AtlasDeviceSampleMethod                      DeviceSampleMethod;                    // Device sampling method
         AtlasSimulationTrigger                       Trigger;                               // Simulation trigger
         AtlasTerminalManager                         TerminalManager;                       // O1: Terminal manager method
         AtlasDeviceInterfaceManager                  DeviceInterfaceManager;                // O2: Device interface manager method
         AtlasDeviceManager                           DeviceManager;                         // O3: Device manager method
         AtlasDeviceProxyManager                      DeviceProxyManager;                    // O4: Device proxy manager method
         AtlasDeviceProxyChannelManager               DeviceProxyChannelManager;             // O5: Device proxy channel manager method
         AtlasKeywordMethod                           KeywordMethod;                         // KX: Atlas keyword method
         AtlasDirectiveManager                        DirectiveManager;                      // XX: Directive manager method
         ActivityMethod                               ActivityMethod;                        // Context agent activity method
         AtlasExpressionManager                       ExpressionManager;                     // Expression validation manager
         };
      TITAN_HANDLE64                                  code_handle;                           // Code handle sent to the manager method
      TITAN_HANDLE64                                  data_handle;                           // Data handle sent to the manager method
      };

#endif

/*
   End of file
*/
