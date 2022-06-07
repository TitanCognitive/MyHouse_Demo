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
#include "MyLight/MyLight.h"

/*
   Create a light instance
   Returns: Handle to the light instance created upon successful operation
            0 upon failure to create light instance
*/
TITAN_HANDLE64 MyLight::AtlasSim_Create(TITAN_HANDLE64 reference, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   light_data_t                                       *light;
   atlas_value_t                                      value;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(light, light_data_t, num_elements) == NULL) return 0;

   // Remember light reference control handle
   light->reference = reference;
   if (Instance->Create(reference, m_object_concept, light->body_handle, 1, params_handle) != TITAN_RESULT_OK) { TITAN_FREE(light); return 0; }
   
   // Get light position from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_transform_label;
   if (Instance->GetValue(m_object_concept, value, light->body_handle, 0) != TITAN_RESULT_OK) { TITAN_FREE(light); return 0; }
   light->transform = _RP(titan_mat4_t, value.amount.raw_value);

   // Get light color from its sub concept; return failure if unsuccessful
   value.amount.format_handle = m_color_label;
   if (Instance->GetValue(m_object_concept, value, light->body_handle, 0) != TITAN_RESULT_OK) { TITAN_FREE(light); return 0; }
   light->color = _RP(titan_vec4_t, value.amount.raw_value);
  
   // Add new light to the rendering engine
   if ((light->light_index = m_GL->Light->AddLight()) < 0) { TITAN_FREE(light); return 0; }

   // Set light parameters
   light->attenuation = 1.25f;
   light->focus = 2.0f;
   light->aperture_angle = 45.0f;

   // Set light position
   m_GL->Light->SetLightTransform(light->light_index, light->transform->f.value);
   m_GL->Light->SetLightColor(light->light_index, light->color->f.value);
   m_GL->Light->SetLightIntensity(light->light_index, light->attenuation);
   m_GL->Light->SetLightFocus(light->light_index, light->focus);
   m_GL->Light->SetLightCutoffAngle(light->light_index, light->aperture_angle);
   m_GL->Light->EnableLight(light->light_index);

   // Get the instance objects of the door; return failure if unsuccessful
   if ((Instance->GetSimulationObject(m_object_concept, light->body_handle, 0, light->body_vro, handshake)) != TITAN_RESULT_OK) return 0;

   // Return light data structure
   return TO_HANDLE64(light);
}

/*
   Load light instance data from supplied file <file> into output handles <instance> and <element>, where:
      <caller_id> is the private reference ID of the reference loaded
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the instance loaded upon successful operation
            0 upon failure load the instance
*/
titan_result_t MyLight::AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   light_data_t                                       *light;
   atlas_value_t                                      value;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_UINT                                         dummy_32;
   TITAN_SIZE64                                       num_bytes_read;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Read number of elements in the light instance; return failure if unsuccessful
   if ((result = File->Read(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(light, light_data_t, num_elements) == NULL) return TITAN_RESULT_NO_MEMORY;
   light->reference = caller_id;

   // Read element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Read the light state (Emmitting: On/Off); return failure if unsuccessful
      if ((result = File->Read(file, &dummy_32, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      if (dummy_32) light->on = true;
      else light->on = false;

      // Read the light attenuation; return failure if unsuccessful
      if ((result = File->Read(file, &light->attenuation, sizeof(TITAN_FLOAT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      // Read the light focus angle;return failure if unsuccessful
      if ((result = File->Read(file, &light->focus, sizeof(TITAN_FLOAT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      // Read the light aperture angle; return failure if unsuccessful
      if ((result = File->Read(file, &light->aperture_angle, sizeof(TITAN_FLOAT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;

      // Read the light type (Directional: On/Off) ; return failure if unsuccessful
      if ((result = File->Read(file, &dummy_32, sizeof(TITAN_UINT), 1, num_bytes_read)) != TITAN_RESULT_OK) return result;
      if (dummy_32) light->directional = true;
      else light->directional = false;

      TITAN_NAME                                         reference_name[256];
      atlas_reference_info_t                             reference_info = { 0 };

      // Initialize reference data
      reference_info.name = reference_name;
      reference_info.name_max_size = 255;

      Reference->GetInfo(Reference->GetPublicHandle(caller_id), reference_info);

      // Read the concept sub-instances
      if ((result = Instance->Load(light->reference, file, m_object_concept, light->body_handle, element, handshake)) != TITAN_RESULT_OK)
         {
         // Return failure
         return result;
         }

      // Get light position from its sub concept; return failure if unsuccessful
      value.amount.format_handle = m_transform_label;
      if (Instance->GetValue(m_object_concept, value, light->body_handle, 0) != TITAN_RESULT_OK) { TITAN_FREE(light); return TITAN_RESULT_FILE_LOAD_ERROR; }
      light->transform = _RP(titan_mat4_t, value.amount.raw_value);

      // Get light color from its sub concept; return failure if unsuccessful
      value.amount.format_handle = m_color_label;
      if (Instance->GetValue(m_object_concept, value, light->body_handle, 0) != TITAN_RESULT_OK) { TITAN_FREE(light); return TITAN_RESULT_FILE_LOAD_ERROR; }
      light->color = _RP(titan_vec4_t, value.amount.raw_value);

      // Add new light to the rendering engine
      if ((light->light_index = m_GL->Light->AddLight()) < 0) { TITAN_FREE(light); return TITAN_RESULT_FILE_LOAD_ERROR; }

      // Set light parameters
      m_GL->Light->SetLightType(light->light_index, light->directional);
      m_GL->Light->SetLightTransform(light->light_index, light->transform->f.value);
      m_GL->Light->SetLightColor(light->light_index, light->color->f.value);
      m_GL->Light->SetLightIntensity(light->light_index, light->attenuation);
      m_GL->Light->SetLightFocus(light->light_index, light->focus);
      m_GL->Light->SetLightCutoffAngle(light->light_index, light->aperture_angle);
      m_GL->Light->EnableLight(light->light_index);
      }

   // Set output handles+
   instance = TO_HANDLE64(light);
   element = 0;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Save light instance data to supplied file <file> from supplied <instance> and <element>, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyLight::AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   light_data_t                                       *light;
   TITAN_UINT                                         element_index, num_elements;
   TITAN_UINT                                         dummy_32;
   TITAN_SIZE64                                       num_bytes_written;
   
   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) light = _RP(light_data_t, element);
   else if (instance) light = _RP(light_data_t, instance);
   else return TITAN_RESULT_INVALID_INPUT;

   // Set number of elements
   num_elements = 1;

   // Write number of elements in the light instance; return failure if unsuccessful
   if ((result = File->Write(file, &num_elements, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

   // Write element data for all elements
   for (element_index = 0; element_index < num_elements; element_index++)
      {
      // Write the light state (Emmitting: On/Off); return failure if unsuccessful
      dummy_32 = TO_32(light->on);
      if ((result = File->Write(file, &dummy_32, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

      // Write the light attenuation; return failure if unsuccessful
      if ((result = File->Write(file, &light->attenuation, sizeof(TITAN_FLOAT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
      // Write the light focus angle;return failure if unsuccessful
      if ((result = File->Write(file, &light->focus, sizeof(TITAN_FLOAT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
      // Write the light aperture angle; return failure if unsuccessful
      if ((result = File->Write(file, &light->aperture_angle, sizeof(TITAN_FLOAT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

      // Write the light type (Directional: On/Off) ; return failure if unsuccessful
      dummy_32 = TO_32(light->directional);
      if ((result = File->Write(file, &dummy_32, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;

      // Write light information
      if ((result = Instance->Save(file, m_object_concept, light->reference, light->body_handle, 0, handshake)) != TITAN_RESULT_OK)
         {
         // Return failure
         return result;
         }
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Destroy light instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyLight::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   light_data_t                                      *light;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if ((light = _RP(light_data_t, instance)) == NULL) return TITAN_RESULT_INVALID_INPUT;

   // If the instance has a valid simulation object; destroy it
   if (light->body_handle != 0) 
      {
      // Destroy the light instance in the renderer
      if (light->light_index >= 0) m_GL->Light->DestroyLight(light->light_index);
      // Destroy the instance
      Instance->Destroy(m_object_concept, light->body_handle, handshake);
      }

   // Destroy the light
   TITAN_FREE(light);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Return sub-instance of current instance based on supplied <instance> <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the sub-instance retrieved upon successful operation
            0 upon failure to retrieve the sub-instance
*/
TITAN_HANDLE64 MyLight::AtlasSim_GetSub(TITAN_HANDLE64 sub_concept, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   light_data_t                                      *light;

   // If this is a sub object
   if (sub_concept == m_object_concept)
      {
      // Get instance data from element if it is available; otherwise, instance handle has instance data
      if ((light = _RP(light_data_t, instance)) == NULL) return 0;

      // Return handle of the sub object
      return light->body_handle;
      }

   // Return failure otherwise
   return 0;
}

/*
   Get light simulation object based on supplied <instance> and <element> pair, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle to the simulation object of the supplied light instance/element upon successful operation
            0 upon failure to retrieved the requested simulation object
*/
TITAN_HANDLE64 MyLight::AtlasSim_GetSimulationObject(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   light_data_t                                       *light;

   // Get instance data from element if it is available; otherwise, instance handle has instance data
   if (element) light = _RP(light_data_t, element);
   else if (instance) light = _RP(light_data_t, instance);
   else return 0;

   // If the light instance is valid, return its corresponding simulated object
   if (light) return light->body_vro;

   // Return failure otherwise
   return 0;
}

/*
   End of file
*/
