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
#include "Common/Titan.h"

// Titan framework dependencies
#define TITAN_SYSTEM_MEMORY
#define TITAN_SYSTEM_TOOLS
#include TITAN_DEPENDENCIES

// Dependencies
#include "MyConsole/MyConsole.h"

/*
   Attach concept to the knowledge base
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyConsole::AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params)
{  
   titan_result_t                                     result;

   // Validate framework; return failure if unsuccessful
   if (Memory == NULL || Tools == NULL) return TITAN_RESULT_NOT_INITIALIZED;

   // Remember concept
   m_concept = concept;

   // BE: Console states
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Online", 0, TO_HANDLE64(this), &MyConsole::BE_Online)) != TITAN_RESULT_OK) return result;
   // DO: Console actions
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Clear", 0, TO_HANDLE64(this), &MyConsole::DO_Clear)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Activate", 0, TO_HANDLE64(this), &MyConsole::DO_Activate)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Deactivate", 0, TO_HANDLE64(this), &MyConsole::DO_Deactivate)) != TITAN_RESULT_OK) return result;

   // Set console concept instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;

   // Register format labels; return failure if unsuccessful
   if ((m_console_interface_label = Label->GetHandle("Console Interface")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if ((m_console_instance_data_label = Label->GetHandle("Instance Data")) == 0) return TITAN_RESULT_INSERTION_FAIL;
   if ((m_v_camera = Label->GetHandle("Camera")) == 0) return TITAN_RESULT_INSERTION_FAIL;

   // Get simulation object attribute interfaces
   if ((Location = _S(AtlasAttributeLocationAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_LOCATION))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Size = _S(AtlasAttributeSizeAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_SIZE))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
   if ((Color = _S(AtlasAttributeColorAPI, Cognitive->AddAttribute(ATLAS_ATTRIB_SCOPE_ELEMENT, ATLAS_ATTRIBUTE_COLOR))) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;

   // Initialize the editor; return failure if unsuccessful
   if (m_Editor.Initialize(this, Ledger, Location) != TITAN_RESULT_OK) return TITAN_RESULT_INIT_FAIL;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Attach console axiom to the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyConsole::AtlasSim_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept)
{
   const TITAN_STRING                                 clone_name = "User Clone";

   // Create a presence; return failure if unsuccessful
   if ((m_agent = Agent->Create("House", "Kitchen", 4, m_agent_reference, "User Console", concept)) == 0)
      {
      // Report failure
      printf("Agent 'User Console' could not be created in domain 'House' in zone 'Kitchen'.\n");
      // Return failure
      return TITAN_RESULT_INSERTION_FAIL;
      }

   // Create an Atlas clone to service the client agent; return failure if unsuccessful
   printf("Creating an Atlas clone, named '%s'.\n", clone_name);
   // Create the Atlas clone with a -1 frequency - this makes the clone exclusive to the agent that subscribes to it
   if ((m_user_clone = Simulation->CreateAtlasClone(m_agent_reference, clone_name, -1.0f, 4, 0)) == 0)
      {
      // Report error
      printf("Failed to create an Atlas clone '%s'.\n", clone_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Subscribe the context agent to the Atlas clone
   printf("Subscribing context agent '%s' to Atlas clone '%s'.\n", "User Console", clone_name);
   // Join an existing process; return failure if unsuccessful
   if (Agent->JoinAtlasClone(m_agent, clone_name) != TITAN_RESULT_OK)
      {
      // Report issue
      printf("Context agent '%s' could not subscribe to Atlas clone '%s.\n", "User Clone", clone_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Link console axiom to other axioms in the simulation
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyConsole::AtlasSim_Link(TITAN_HANDLE64 concept_handle)
{
   TITAN_HANDLE64                                     label_handle;
   atlas_value_t                                      value;
   atlas_reference_info_t                             info = { 0 };
   TITAN_BITMAP64                                     *key_states;

   // Grab the construct interface
   if ((label_handle = Label->GetFirst(m_agent_reference, "ApplicationWindow", info, ATLAS_GET_REFERENCE)) != NULL)
      {
      // If the reference is valid, get the video interface from reference; otherwise, return a failure
      if (info.reference == 0 || (Label->GetValue(info.reference, "Video Interface", value) != TITAN_RESULT_OK)) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      // Get GL interface; return failure if unsuccessful
      if (value.amount.format_handle == TITAN_UINT_ERROR || (m_GL = _SP(MyGLAPI, value.amount.raw_value)) == NULL) return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // If a construct exists
   if (m_GL)
      {
      // Get keyboard key states structure
      m_GL->GetKeyboardState(NULL, &key_states);
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   End of file
*/
