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
#include "MyObject/MyObject.h"
#include "MyScene/MyScene.h"

/*
   Create a object instance
   Returns: Handle to the object instance created upon successful operation
            0 upon failure to create object instance
*/
TITAN_HANDLE64 MyObject::AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   object_data_t                                      *object;
   TITAN_HANDLE64                                     sim_object;
   TITAN_DOUBLE                                       start_time;

   // Create a simulation object; return failure if unsuccessful
   if ((sim_object = Simulation->CreateSimulationObject(caller_id, m_concept)) == NULL) return 0;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(object, object_data_t, num_elements) == NULL) return 0;

   // Set object to happy initially
   object->reference = caller_id;
   object->sim_object = sim_object;
   object->coordinates.prev = 0;
   object->coordinates.next = 0;
   object->coordinates.trigger.code_handle = TO_HANDLE64(this);
   object->coordinates.trigger.data_handle = TO_HANDLE64(object);
   object->coordinates.trigger.Trigger = NULL; // &Think;

   // Set object orientation
   VectorSet4(object->coordinates.angle.f.value, 0.0f, 0.0f, 0.0f, 0.0f);
   // Set object position
   VectorSet4(object->coordinates.position.f.value, 0.0f, 0.0f, 0.0f, 0.0f);
   // Set the scale
   VectorSet4(object->coordinates.scale.f.value, 1.0f, 1.0f, 1.0f, 0.0f);

   // If the object has a model, assign it
   if (params_handle)
      {
      // Set object color; return failure if unsuccessful
      VectorClear4(object->color.f.value, 1.0f);
      if (Color->SetColorData(sim_object, object->color.f.value) != TITAN_RESULT_OK) return 0;

      // Set model instance to 'invalid'
      object->model_instance = -1;
      // Start timeout clock
      start_time = Tools->GetTime();
      // Precache model; return failure if unsuccessful
      if (m_GL->PrecacheResource(MY_GL_PRECACHE_FLAG_MODEL, TO_STRING(params_handle), &LoadModel, this, object) != TITAN_RESULT_OK)
         {
         // Fail the model
         object->model_instance = -2;
         }
      // While the model instance is 'invalid' and the timeout limit (3000ms) hasn't been reached
      while (object->model_instance == -1 && Tools->GetTime() - start_time < 3000) { Tools->YieldThread(); }

      // If there was an error loading the model
      if (object->model_handle < 0)
         {
         // Report error
         printf("Could not load model: %s\n", TO_STRING(params_handle));
         object->model_handle = -1;
         object->model_instance = -1;
         }
      }
   else
      {
      // Set model and instance handles
      object->model_handle = -1;
      object->model_instance = -1;
      }

   // Start simulation
   Location->SetCoordinate(object->sim_object, &object->coordinates, &object->transform, true);

   // Add object to the scene
   if (m_scene) m_scene->Scene->Register(m_scene, caller_id, object->transform.f.value);

   // Return object data structure
   return TO_HANDLE64(object);
}

/*
   Load object instance data from supplied file <file> into output handles <instance> and <element>, where:
      <caller_id> is the private reference ID of the reference loaded
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the instance loaded upon successful operation
            0 upon failure load the instance
*/
titan_result_t MyObject::AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   object_data_t                                      *object;
   TITAN_HANDLE64                                     sim_object;
   TITAN_NAME                                         model_name[256];
   TITAN_UINT                                         element_index, num_elements;
   TITAN_UINT                                         name_size;
   TITAN_SIZE64                                       num_bytes_read;
   TITAN_DOUBLE                                       start_time;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Read object instance simulation object; return failure if unsuccessful
   if ((result = Simulation->LoadSimulationObject(caller_id, file, m_concept, sim_object)) != TITAN_RESULT_OK) return result;

   // Read number of elements in the camera instance; return failure if unsuccessful
   if ((result = File->Read(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(object, object_data_t, num_elements) == NULL) return TITAN_RESULT_NO_MEMORY;
   object->reference = caller_id;
   
   // Set object instance simulation object handle
   object->sim_object = sim_object;

   // Read element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Read object coordinates; return failure if unsuccessful
      if ((result = File->Read(file, &object->coordinates, sizeof(atlas_coordinate_t), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      // Read the object transform; return failure if unsuccessful
      if ((result = File->Read(file, &object->transform, sizeof(titan_mat4_t), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      // Read the object color; return failure if unsuccessful
      if ((result = File->Read(file, &object->color, sizeof(titan_vec4_t), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      // Read the model name size; return failure if unsuccessful
      if ((result = File->Read(file, &name_size, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;

      // Overwrite coordinate pointer data
      object->coordinates.next = object->coordinates.prev = NULL;
      object->coordinates.trigger.Trigger = NULL;
      object->coordinates.trigger.code_handle = NULL;
      object->coordinates.trigger.data_handle = NULL;
      // Set object color; return failure if unsuccessful
      if ((result = Color->SetColorData(object->sim_object, object->color.f.value)) != TITAN_RESULT_OK) return result;

      // If the name is valid
      if (name_size)
         {
         // Set maximum size for the name
         if (name_size > 255) return TITAN_RESULT_PARSE_ERROR;
         // Read the model name; return failure if unsuccessful
         if ((result = File->Read(file, model_name, sizeof(TITAN_BYTE), name_size, num_bytes_read)) != TITAN_RESULT_OK) return result;

         // Set model instance to 'invalid'
         object->model_instance = -1;
         // Start timeout clock
         start_time = Tools->GetTime();
         // Precache model; return failure if unsuccessful
         if (m_GL->PrecacheResource(MY_GL_PRECACHE_FLAG_MODEL, model_name, &LoadModel, this, object) != TITAN_RESULT_OK)
            {
            // Fail the model
            object->model_instance = -2;
            }
         // While the model instance is 'invalid' and the timeout limit (3000ms) hasn't been reached
         while (object->model_instance == -1 && Tools->GetTime() - start_time < 3000) { Tools->YieldThread(); }

         // If there was an error loading the model
         if (object->model_handle < 0)
            {
            // Handle error if needed
            printf("Error: Could not load model '%s'\n", model_name);
            object->model_handle = -1;
            object->model_instance = -1;
            }
         }
      else
         {
         // Set model to invalid
         object->model_handle = -1;
         object->model_instance = -1;
         }

      // Register object reference and its coordinate with the scene
      if (m_scene) m_scene->Scene->Register(m_scene, caller_id, object->transform.f.value);

      // Register object coordinate with the simulation
      Location->SetCoordinate(object->sim_object, &object->coordinates, &object->transform, true);
      }

   // Set output handles
   instance = TO_HANDLE64(object);
   element = 0;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Save object instance data to supplied file <file> from supplied <instance> and <element>, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyObject::AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   object_data_t                                      *object;
   TITAN_STRING                                       model_name;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_UINT                                         name_size;
   TITAN_SIZE64                                       num_bytes_written;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) object = _RP(object_data_t, element);
   else if (instance) object = _RP(object_data_t, instance);
   else return TITAN_RESULT_INVALID_INPUT;

   // Write object instance simulation object; return failure if unsuccessful
   if ((result = Simulation->SaveSimulationObject(file, object->sim_object)) != TITAN_RESULT_OK) return result;

   // Set number of elements
   num_elements = 1;
  // Write number of elements in the object; return failure if unsuccessful
   if ((result = File->Write(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

   // Write element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Write object coordinates; return failure if unsuccessful
      if ((result = File->Write(file, &object->coordinates, sizeof(atlas_coordinate_t), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
      // Write the object transform; return failure if unsuccessful
      if ((result = File->Write(file, &object->transform, sizeof(titan_mat4_t), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
      // Write the object color; return failure if unsuccessful
      if ((result = File->Write(file, &object->color, sizeof(titan_vec4_t), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
      // Assume the model name is invalid
      name_size = 0;
      // If the model index and name are valid, get the name size
      if (object->model_handle >= 0 && (model_name = m_GL->Model->GetModelPath(object->model_handle)) != NULL)
         {
         // Get model name size
         name_size = TO_32(strlen(model_name)) + 1;
         }
      // Limit the name size to 255 characters
      if (name_size > 255) name_size = 255;

      // Read the model name size; return failure if unsuccessful
      if ((result = File->Write(file, &name_size, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

      // If the name is valid
      if (name_size)
         {
         // Write the model name; return failure if unsuccessful
         if ((result = File->Write(file, model_name, sizeof(TITAN_BYTE), name_size, num_bytes_written)) != TITAN_RESULT_OK) return result;
         }
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Get object simulation object based on supplied <instance> and <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle to the simulation object of the supplied specified object instance/element upon successful operation
            0 upon failure to retrieved the requested simulation object
*/
TITAN_HANDLE64 MyObject::AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   object_data_t                                      *object;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) object = _RP(object_data_t, element);
   else if (instance) object = _RP(object_data_t, instance);
   else return 0;

   // If the object instance is valid, return its corresponding simulated object
   if (object) return object->sim_object;

   // Return failure otherwise
   return 0;
}

/*
   Destroy object instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyObject::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   object_data_t                                      *object;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if ((object = _RP(object_data_t, instance)) == NULL) return TITAN_RESULT_INVALID_INPUT;

   // Remove the object from the scene
   if (m_scene) m_scene->Scene->Cancel(m_scene, object->reference);

   // If the instance has a valid simulation object
   if (object->sim_object != 0)
      {
      // Stop simulating the object
      Location->Pause(object->sim_object);
      // Destroy the simulation object
      Simulation->DestroySimulationObject(object->sim_object);
      }

   // If the object has a valid model instance
   if (object->model_instance >= 0)
      {
      // Destroy the model instance
      m_GL->Model->DestroyModelInstance(object->model_handle, object->model_instance, object->transform.f.value, object->color.f.value);
      }

   // Destroy the object
   TITAN_FREE(object);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Get the object instance value, specified by format <value.format_handle> from supplied <instance> and <element> pair, where:
      <value> is the value retrieved
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyObject::AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake)
{
   object_data_t                                      *object;

   // Get data from element if it exists; otherwise, use instance data
   if (element) object = _RP(object_data_t, element);
   else if (instance) object = _RP(object_data_t, instance);
   else
      {
      // If the coordinate interface is requested
      if (value.amount.format_handle == m_coordinate_interface_label)
         {
         // Set the window camera interface value
         value.amount.format_handle = 0;
         value.amount.raw_value = Location;
         // Return success
         return TITAN_RESULT_OK;
         }

      // Return invalid input
      return TITAN_RESULT_INVALID_INPUT;
      }

   // If instance is invalid, return failure
   if (object == NULL) return TITAN_RESULT_INVALID_INPUT;

   // If the coordinate is requested
   if (value.amount.format_handle == m_coordinate_label)
      {
      // Set the window camera interface value
      value.amount.format_handle = 0;
      value.amount.raw_value = &object->coordinates;
      // Return success
      return TITAN_RESULT_OK;
      }

   // If the transform is requested
   if (value.amount.format_handle == m_transform_label)
      {
      // Set the window camera interface value
      value.amount.format_handle = 0;
      value.amount.raw_value = &object->transform;
      // Return success
      return TITAN_RESULT_OK;
      }

   // If the color is requested
   if (value.amount.format_handle == m_color_label)
      {
      // Set the window camera interface value
      value.amount.format_handle = 0;
      value.amount.raw_value = &object->color;
      // Return success
      return TITAN_RESULT_OK;
      }

   // Return failure otherwise
   return TITAN_RESULT_ERROR;
}

/*
   End of file
*/
