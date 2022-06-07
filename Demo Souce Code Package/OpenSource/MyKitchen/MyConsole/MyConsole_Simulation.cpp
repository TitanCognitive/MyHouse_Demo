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
   This is a placeholder method for intercepting (x, y, z, t) triggers invoked by the simulation.
*/
void MyConsole::T_Think(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags)
{
   MyConsole                                          *self = _RP(MyConsole, coordinate->trigger.code_handle);
   console_data_t                                     *console_data = _RP(console_data_t, coordinate->trigger.data_handle);

   // Validate input; return if unsuccessful
   if (self == NULL || console_data == NULL) return;
   
   // Get local coordinate
   coordinate->trigger.Trigger = NULL;
   
   // Update coordinate
   self->Location->UpdateCoordinate(console_data->sim_object, TITAN_COORDINATE_TRS);
}

/*
   This is a placeholder method for intercepting (x, y, z, t) triggers invoked by the simulation.
*/
void MyConsole::T_EnterHelp(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags)
{
   MyConsole                                          *self = _RP(MyConsole, coordinate->trigger.code_handle);
   console_data_t                                     *console_data = _RP(console_data_t, coordinate->trigger.data_handle);

   // Validate input; return if unsuccessful
   if (self == NULL || console_data == NULL) return;

   // Get local coordinate
   VectorSet4(coordinate->scale.f.value, 1.2f, 1.2f, 1.2f, 0.2f);
   coordinate->trigger.Trigger = NULL;
   
   // Update coordinate
   self->Location->UpdateCoordinate(console_data->sim_object, TITAN_COORDINATE_TRANSLATE);
}

/*
   This is a placeholder method for intercepting (x, y, z, t) triggers invoked by the simulation.
*/
void MyConsole::T_LeaveHelp(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags)
{
   MyConsole                                          *self = _RP(MyConsole, coordinate->trigger.code_handle);
   console_data_t                                     *console_data = _RP(console_data_t, coordinate->trigger.data_handle);

   // Validate input; return if unsuccessful
   if (self == NULL || console_data == NULL) return;

   // Stop rendering
   console_data->show_help = false;

   // Get local coordinate
   coordinate->position.xyzt_f.y = 30.0f;
   coordinate->trigger.Trigger = NULL;
   
   // Update coordinate
   self->Location->UpdateCoordinate(console_data->sim_object, TITAN_COORDINATE_TRS);
}

/*
   End of file
*/
