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
#include "MyObject/MyObject.h"

/*
   This is a placeholder method for intercepting (x, y, z, t) triggers invoked by the simulation.
*/
void MyObject::T_Think(TITAN_FLOAT sim_time, atlas_coordinate_t *coordinate, TITAN_BITMAP32 trigger_flags)
{
   MyObject                                           *self = _RP(MyObject, coordinate->trigger.code_handle);
   object_data_t                                      *object_data = _RP(object_data_t, coordinate->trigger.data_handle);

   // REMOVE: Do nothing
   return;

   // Return if class instance data is not available
   if (self == NULL) return;
   // Return if concept instance data is not available
   if (object_data == NULL) return;
   
   // Randomize yaw
   // If this is a couch model
   coordinate->angle.rot.yaw = random();
   coordinate->angle.xyzt_f.t = 1.0f + random() * 5.0f;
   VectorSet3(object_data->coordinates.position.f.value, -1.0f + random() * 2.0f, 0.3f, -1.0f + random() * 2.0f);
   coordinate->trigger.Trigger = NULL;

   // Update coordinate for this reference
   self->Location->UpdateCoordinate(object_data->sim_object, TITAN_COORDINATE_TRS, 0, true);
}

/*
   End of file
*/
