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
   Create a console instance
   Returns: Handle to the console instance created upon successful operation
            0 upon failure to create console instance
*/

TITAN_HANDLE64 MyConsole::AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   console_data_t                                     *console;
   TITAN_HANDLE64                                     sim_object;

   // Create a simulation object; return failure if unsuccessful
   if ((sim_object = Simulation->CreateSimulationObject(caller_id, m_concept)) == NULL) return 0;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(console, console_data_t, num_elements) == NULL) return 0;

   // Set console to happy initially
   console->console_sim.reference = caller_id;
   console->target = caller_id;
   console->sim_object = sim_object;
   console->console_sim.coordinates.trigger.code_handle = TO_HANDLE64(this);
   console->console_sim.coordinates.trigger.data_handle = TO_HANDLE64(console);
   console->console_sim.coordinates.trigger.Trigger = &T_Think;

   // Set console simulation reference data (scale, angle, position, color)
   VectorSet4(console->console_sim.coordinates.scale.f.value, 1.0f, 1.0f, 1.0f, 0.0f);
   console->console_sim.coordinates.scale.xyzt_f.t = 
   VectorSet4(console->console_sim.coordinates.angle.f.value, 0.0f, 0.0f, 0.0f, 0.0f);
   VectorSet4(console->console_sim.coordinates.position.f.value, 0.0f, 0.0f, -20.0f, 0.0f);
   VectorSet4(console->console_sim.color.f.value, 0.8f + random() * 0.2f, 0.8f + random() * 0.2f, 0.8f + random() * 0.2f, 1.0f);
   // Set extra console data
   VectorSet4(console->input_line_color.f.value, 0.7f, 0.8f, 0.75f, 1.0f);

   // Set console color; return failure if unsuccessful
   if (Color->SetColorData(sim_object, console->input_line_color.f.value) != TITAN_RESULT_OK) return 0;

   // Allocate a new console of size 64-KB; return failure if unsuccessful
   if ((console->ledger = Ledger->Create(caller_id, "My Console", 1024 * 64)) == 0) return 0;
   Ledger->InsertMessage(console->ledger, 0, 0,  "Example Kitchen Demo");
   Ledger->InsertMessage(console->ledger, 0, 0, "Press ENTER to leave console.");
   Ledger->InsertMessage(console->ledger, 0, 0, " ");

   // Set current scroll line in the log (0 = no line selected and scroll all the way to the latest message)
   console->message.handle = 0; 

   // Initialize keyboard translation table
   SetConsoleKeyboardTranslationTable(console);
   // Set console simulation coordinate and output transform
   Location->SetCoordinate(console->sim_object, &console->console_sim.coordinates, &console->console_sim.transform, true);

   // Add a help reference; return failure if unsuccessful
   if ((console->help_sim.reference = Reference->Add(caller_id, "Console Help", m_concept, 0)) == 0) return 0;

   // Return console data structure
   return TO_HANDLE64(console);
}

/*
   Destroy console instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyConsole::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   console_data_t                                     *console;

   // Validate input; return failure if unscucessful
   if (instance == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get console instance data
   console = _RP(console_data_t, instance);

   // Free console data
   TITAN_FREE(console);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Set the console instance values 
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyConsole::AtlasSim_SetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake)
{
   console_data_t                                     *console;

   // Get data from element if it exists; otherwise, use instance data
   if (element) console = _RP(console_data_t, element);
   else if (instance) console = _RP(console_data_t, instance);
   else return TITAN_RESULT_ERROR;

   // If instance is invalid, return failure
   if (console == NULL) return TITAN_RESULT_ERROR;

   // If the target is being set
   if (value.amount.format_handle == m_v_camera)
      {
      // Return the window video interface
      if (value.amount.format_handle == TITAN_DATA_TYPE_REFERENCE) console->target;
      console->target = value.amount.handle_value;

      // Return success
      return TITAN_RESULT_OK;
      }
   
   // Return failure otherwise
   return TITAN_RESULT_ERROR;
}

/*
   Get the console instance value, specified by format <value.format_handle> from supplied <instance> and <element> pair, where:
      <value> is the value retrieved
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyConsole::AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake)
{
   console_data_t                                     *console;

   // Get data from element if it exists; otherwise, use instance data
   if (element) console = _RP(console_data_t, element);
   else if (instance) console = _RP(console_data_t, instance);
   else return TITAN_RESULT_ERROR;

   // If instance is invalid, return failure
   if (console == NULL) return TITAN_RESULT_ERROR;

   // If the Titan console interface is requested
   if (value.amount.format_handle == m_console_interface_label)
      {
      // Return the window video interface
      value.amount.format_handle = TITAN_DATA_TYPE_REFERENCE;
      value.amount.handle_value = console->ledger;
      // Return success
      return TITAN_RESULT_OK;
      }

   // If the Titan console instance data
   if (value.amount.format_handle == m_console_instance_data_label)
      {
      // Return the window video interface
      value.amount.format_handle = TITAN_DATA_TYPE_REFERENCE;
      value.amount.handle_value = TO_HANDLE64(console);
      // Return success
      return TITAN_RESULT_OK;
      }
 
   // Return failure otherwise
   return TITAN_RESULT_ERROR;
}

/*
   Get console simulation object based on supplied <instance> and <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle to the simulation object of the supplied console instance/element upon successful operation
            0 upon failure to retrieved the requested simulation object
*/
TITAN_HANDLE64 MyConsole::AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   console_data_t                                     *console;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) console = _RP(console_data_t, element);
   else if (instance) console = _RP(console_data_t, instance);
   else return 0;

   // If the console instance is valid, return its corresponding simulated object
   if (console) return console->sim_object;

   // Return failure otherwise
   return 0;
}

/*
   End of file
*/
