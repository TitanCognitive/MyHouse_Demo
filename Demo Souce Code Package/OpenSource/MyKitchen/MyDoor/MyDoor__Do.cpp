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
#include "MyDoor/MyDoor.h"

/*
   Do: Open
   Open directive object door 
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyDoor::DO_Open(ATLAS_SIMULATE_PARAMS) 
{ 
   MyDoor                                             *self = _RP(MyDoor, user_data);
   door_data_t                                        *door;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Get the directive first object (transitive) and validate that it is in fact a door; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &door)) == 0) return NO_RESPONSE();

   // Open the door
   return self->OpenDoor(door, directive_header.query);
}

/*
   Do: Close
   Close directive object door
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyDoor::DO_Close(ATLAS_SIMULATE_PARAMS) 
{ 
   MyDoor                                             *self = _RP(MyDoor, user_data);
   door_data_t                                        *door;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Get the directive first object (transitive) and validate that it is in fact a door; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &door)) == 0) return NO_RESPONSE();

   // Close the door
   return self->CloseDoor(door, directive_header.query);
}


/*
   Do: Destroy
   Destroy directive object reference and its associated instance
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyDoor::DO_SYS_Destroy(ATLAS_SIMULATE_PARAMS) 
{ 
   MyDoor                                             *self = _RP(MyDoor, user_data);
   door_data_t                                        *door;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a door; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &door)) == 0) return NO_RESPONSE();

   // Destroy the retrieved door reference and its instance data
   if (door->reference) self->Reference->Destroy(door->reference);

   // Return true
   return NO_RESPONSE();
}


/*
   End of file
*/
