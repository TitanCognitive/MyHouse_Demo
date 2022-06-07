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
#define TITAN_SYSTEM_FILE
#include TITAN_DEPENDENCIES

// Dependencies
#include "MyDoor/MyDoor.h"

/*
   Create a door instance
   Returns: Handle to the door instance created upon successful operation
            0 upon failure to create door instance
*/
TITAN_HANDLE64 MyDoor::AtlasSim_Create(TITAN_HANDLE64 reference, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   door_data_t                                       *door;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(door, door_data_t, num_elements) == NULL) return 0;

   // Remember door reference control handle
   door->reference = reference;
   if (CreateSubObjectInstance(reference, m_sub_frame, &door->frame, num_elements, 0, handshake) != TITAN_RESULT_OK) { Destroy(door); return 0; }
   if (CreateSubObjectInstance(reference, m_sub_panel, &door->panel, num_elements, params_handle, handshake) != TITAN_RESULT_OK) { Destroy(door); return 0; }

   // Set object hierarchy; return failure if unsuccessful
   if (Simulation->SetSimulationObjectParent(door->panel.sim_object, door->frame.sim_object) != TITAN_RESULT_OK) { Destroy(door); return 0; }

   // Return door data structure
   return TO_HANDLE64(door);
}

/*
   Load door instance data from supplied file <file> into output handles <instance> and <element>, where:
      <caller_id> is the private reference ID of the reference loaded
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the instance loaded upon successful operation
            0 upon failure load the instance
*/
titan_result_t MyDoor::AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   door_data_t                                        *door;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_UINT                                         dummy_32;
   TITAN_SIZE64                                       num_bytes_read;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Read number of elements in the door instance; return failure if unsuccessful
   if ((result = File->Read(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(door, door_data_t, num_elements) == NULL) return TITAN_RESULT_NO_MEMORY;
   door->reference = caller_id;

   // Load element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Read door instance configuration (inverted On/Off); return failure if unsuccessful
      if ((result = File->Read(file, &dummy_32, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      if (dummy_32) door->inverted = true;
      else door->inverted = false;

      // Load the door frame instance; return failure if unsuccessful
      if (Instance->Load(door->reference, file, m_sub_frame, door->frame.instance, element, handshake) != TITAN_RESULT_OK) 
         {
         // Destroy the door instance
         Destroy(door); 
         // Return failure
         return TITAN_RESULT_FILE_LOAD_ERROR; 
         }

      // Load the door panel instance; return failure if unsuccessful
      if (Instance->Load(door->reference, file, m_sub_panel, door->panel.instance, element, handshake) != TITAN_RESULT_OK)
         {
         // Destroy the door instance
         Destroy(door); 
         // Return failure
         return TITAN_RESULT_FILE_LOAD_ERROR; 
         }

      // Get the instance objects of the door; return failure if unsuccessful
      if ((GetSubObjectInstance(m_sub_frame, &door->frame, handshake)) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;
      if ((GetSubObjectInstance(m_sub_panel, &door->panel, handshake)) != TITAN_RESULT_OK) return TITAN_RESULT_NO_DATA;

      // Set object hierarchy by assiging the panel simulation object as a child of the frame simulation object; return failure if unsuccessful
      if (Simulation->SetSimulationObjectParent(door->panel.sim_object, door->frame.sim_object) != TITAN_RESULT_OK)
         {
         // Destroy the door instance
         Destroy(door); 
         // Return failure
         return TITAN_RESULT_INCOMPATIBLE; 
         }
      }

   // Set output handles
   instance = TO_HANDLE64(door);
   element = 0;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Save door instance data to supplied file <file> from supplied <instance> and <element>, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyDoor::AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   door_data_t                                        *door;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_UINT                                         dummy_32;
   TITAN_SIZE64                                       num_bytes_written;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) door = _RP(door_data_t, element);
   else if (instance) door = _RP(door_data_t, instance);
   else return TITAN_RESULT_INVALID_INPUT;

   // Set number of elements
   num_elements = 1;

   // Write number of elements in the door instance; return failure if unsuccessful
   if ((result = File->Write(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

   // Write element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Write door inverted state: On/Off
      dummy_32 = TO_32(door->inverted);
      // Write number of elements in the door instance; return failure if unsuccessful
      if ((result = File->Write(file, &dummy_32, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
      
      // Write the door frame instance; return failure if unsuccessful
      if (Instance->Save(file, m_sub_frame, door->reference, door->frame.instance, 0, handshake) != TITAN_RESULT_OK)
         {
         // Return failure
         return TITAN_RESULT_FILE_SAVE_ERROR;
         }

      // Write the door frame instance; return failure if unsuccessful
      if (Instance->Save(file, m_sub_panel, door->reference, door->panel.instance, 0, handshake) != TITAN_RESULT_OK)
         {
         // Return failure
         return TITAN_RESULT_FILE_SAVE_ERROR;
         }
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Destroy door instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyDoor::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   door_data_t                                      *door;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if ((door = _RP(door_data_t, instance)) == NULL) return TITAN_RESULT_INVALID_INPUT;

   // If the instance has a valid simulation door
   if (door->panel.instance != 0) Instance->Destroy(m_sub_panel, door->panel.instance, handshake);
   if (door->frame.instance != 0) Instance->Destroy(m_sub_frame, door->frame.instance, handshake);

   // Destroy the door
   TITAN_FREE(door);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Return sub-instance of current instance based on supplied <instance> <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the sub-instance retrieved upon successful operation
            0 upon failure to retrieve the sub-instance
*/
TITAN_HANDLE64 MyDoor::AtlasSim_GetSub(TITAN_HANDLE64 sub_concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   door_data_t                                        *door;

   // If this is a panel selection
   if (sub_concept == m_sub_panel)
      {
      // Get instance data from element if it is available; otherwise, instance handle has instance data
      if ((door = _RP(door_data_t, instance)) == NULL) return 0;

      // Return handle of the panel instance
      return door->panel.instance;
      }

   // If this is a frame selection
   if (sub_concept == m_sub_frame)
      {
      // Get instance data from element if it is available; otherwise, instance handle has instance data
      if ((door = _RP(door_data_t, instance)) == NULL) return 0;

      // Return handle of the frame instance
      return door->frame.instance;
      }

   // Return failure otherwise
   return 0;
}

/*
   Get door simulation object based on supplied <instance> and <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle to the simulation object of the supplied door instance/element upon successful operation
            0 upon failure to retrieved the requested simulation object
*/
TITAN_HANDLE64 MyDoor::AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   door_data_t                                        *door;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) door = _RP(door_data_t, element);
   else if (instance) door = _RP(door_data_t, instance);
   else return 0;

   // If the door instance is valid, return its corresponding simulated object
   if (door) return door->frame.sim_object;

   // Return failure otherwise
   return 0;
}

/*
   End of file
*/
