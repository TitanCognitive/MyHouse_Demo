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
#include "MyScene/MyScene.h"

/*
   Create a scene instance
   Returns: Handle to the scene instance created upon successful operation
            0 upon failure to create scene instance
*/
TITAN_HANDLE64 MyScene::AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   scene_data_t                                       *scene;
   TITAN_UINT                                         max_objects = 1024, max_cameras = 8;
 
   // If the instance has parameters
   if (params_handle)
      {
      // Set object and camera list sizes
      sscanf(TO_STRING(params_handle), "%u %u", &max_objects, &max_cameras);
      // Return failure if the sizes are not valid
      if (max_objects == 0 || max_objects > 1024) return 0;
      if (max_cameras == 0 || max_cameras > 8) return 0;
      }

   // Allocate scene data structure; return failure if unsuccessful
   if (TITAN_ALLOCATE_FLUSH_ARRAY(scene, scene_data_t, num_elements) == NULL) return 0;

   // Set scene to happy initially
   scene->reference = caller_id;
   scene->Scene = this;
   
   // Allocate scene reference data; return failure if unsuccessful
   if (TITAN_ALLOCATE_FLUSH_ARRAY(scene->transforms, TITAN_FLOAT *, max_objects) == NULL) return 0;  
   if (TITAN_ALLOCATE_FLUSH_ARRAY(scene->names, TITAN_STRING, max_objects) == NULL) return 0;
   if (TITAN_ALLOCATE_FLUSH_ARRAY(scene->references, TITAN_HANDLE64, max_objects) == NULL) return 0;
   scene->max_references = max_objects;
   scene->num_references = 0;

   // Return scene data structure
   return TO_HANDLE64(scene);
}

/*
   Destroy scene instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyScene::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   scene_data_t                                       *scene;

   // Validate input; return failure if unscucessful
   if (instance == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get scene instance data
   scene = _RP(scene_data_t, instance);

   // Free scene reference data; return failure if unsuccessful
   if (scene->transforms) TITAN_FREE(scene->transforms);
   if (scene->names) TITAN_FREE(scene->names);
   if (scene->references) TITAN_FREE(scene->references);
   
   // Free scene data
   TITAN_FREE(scene);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Load scene instance data from supplied file <file> into output handles <instance> and <element>, where:
      <caller_id> is the private reference ID of the reference loaded
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: Handle of the instance loaded upon successful operation
            0 upon failure load the instance
*/
titan_result_t MyScene::AtlasSim_Load(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 file, TITAN_HANDLE64 &instance, TITAN_HANDLE64 &element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result = TITAN_RESULT_OK;
   scene_data_t                                       *scene;
   TITAN_UINT                                         index, data_size;
   TITAN_UINT                                         num_references;
   TITAN_HANDLE64                                     reference_handle, concept_handle;
   TITAN_NAME                                         filename[256];
   TITAN_NAME                                         concept_name[256];
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_SIZE64                                       num_bytes_read;
   TITAN_FLOAT                                        fade_color[4];

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get reference name
   reference_info.name = filename;
   reference_info.name_max_size = 250;

   // Get data from element if it exists; otherwise, use instance data
   if (element) scene = _RP(scene_data_t, element);
   else if (instance) scene = _RP(scene_data_t, instance);
   else return TITAN_RESULT_INVALID_INPUT;

   // Report file size
   printf("File size: %llu\n", File->GetSize(file));

   // Read number of referneces in the scene instance; return failure if unsuccessful
   if (File->Read(file, &num_references, sizeof(TITAN_UINT), 1, num_bytes_read) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_LOAD_ERROR;

   // Go through all the references in the scene
   for (index = 0; index < num_references; index++)
      {
      // Read reference name; return failure if unsuccessful
      if (File->Read(file, &data_size, sizeof(TITAN_UINT), 1, num_bytes_read) != TITAN_RESULT_OK || data_size > 255) return TITAN_RESULT_FILE_LOAD_ERROR;
      if (data_size == 0) return TITAN_RESULT_DATA_MISMATCH;
      if (File->Read(file, filename, sizeof(TITAN_BYTE), data_size, num_bytes_read) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_LOAD_ERROR;
      
      // Read the reference concept name; return failure if unsuccessful
      if (File->Read(file, &data_size, sizeof(TITAN_UINT), 1, num_bytes_read) != TITAN_RESULT_OK || data_size > 255) return TITAN_RESULT_FILE_LOAD_ERROR;
      if (data_size == 0) return TITAN_RESULT_DATA_MISMATCH;
      if (File->Read(file, concept_name, sizeof(TITAN_BYTE), data_size, num_bytes_read) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_LOAD_ERROR;
      
      // Get concept ID from the knowledge base; return failure if the concept is not found
      if ((concept_handle = Cognitive->GetConcept(scene->reference, concept_name)) == 0)
         {
         // Report failure
         printf("Scene: Could not find concept '%s' in the knowledge base\n", concept_name);
         // Return failure
         return TITAN_RESULT_FILE_LOAD_ERROR;
         }
      
      // Report scene loading progress
      // FIXME printf("   Loading reference %3u/%3u: %s named %s\n", index + 1, num_references, concept_name, filename);
      // Read reference under file name <filename>; return failure if unsuccessful
      if ((result = Reference->Load(m_reference, file, concept_handle, filename, reference_handle)) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_LOAD_ERROR; 
      }

   // Remember the scene
   printf("Scene recalled successfully\n");

   // Set fade color to transparent
   VectorSet4(fade_color, .0f, 0.0f, 0.0f, 0.0f);
   // Fade to color in 1 second
   m_GL->FadeToColor(fade_color, 2000.0f);

   // Return success
   return result;
}

/*
   Save scene instance data to supplied file <file> from supplied <instance> and <element>, where:
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyScene::AtlasSim_Save(TITAN_HANDLE64 file, TITAN_HANDLE64 instance, TITAN_HANDLE64 element, TITAN_HANDLE64 handshake)
{
   titan_result_t                                     result;
   scene_data_t                                       *scene;
   TITAN_UINT                                         index, data_size;
   TITAN_HANDLE64                                     reference_handle;
   TITAN_NAME                                         filename[256];
   TITAN_NAME                                         concept_name[256];
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_SIZE64                                       num_bytes_written;

   // If the file was not specified, return failure
   if (file == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get reference name
   reference_info.name = filename;
   reference_info.name_max_size = 250;
   reference_info.concept_name = concept_name;
   reference_info.concept_name_max_size = 250;

   // Get data from element if it exists; otherwise, use instance data
   if (element) scene = _RP(scene_data_t, element);
   else if (instance) scene = _RP(scene_data_t, instance);
   else return TITAN_RESULT_INVALID_INPUT;

   // Write number of references in the scene instance; return failure if unsuccessful
   if ((result = File->Write(file, &scene->num_references, sizeof(TITAN_UINT), 1, num_bytes_written)) != TITAN_RESULT_OK) return result;
   
   // Go through all the references in the scene instance
   for (index = 0; index < scene->num_references; index++)
      {
      // Report scene saving progress
      printf("   Saving reference %3u/%3u\n", index, scene->num_references);
      // Write all objects 
      if (scene->references[index])
         {
         // Ignore invalid references
         if ((reference_handle = Reference->GetPublicHandle(scene->references[index])) == 0) continue;
         
         // Get reference name; return failure if unsuccessul
         if ((result = Reference->GetInfo(reference_handle, reference_info, 0)) != TITAN_RESULT_OK) return result;
         
         // Write file name, capped at 250 bytes; return failure if unsuccessful
         data_size = TO_32(strlen(filename) + 1);
         if (data_size > 250) return TITAN_RESULT_FILE_SAVE_ERROR;
         if (File->Write(file, &data_size, sizeof(TITAN_UINT), 1, num_bytes_written) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_SAVE_ERROR;
         if (File->Write(file, filename, sizeof(TITAN_BYTE), data_size, num_bytes_written) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_SAVE_ERROR;

         // Write concept name, capped at 250 bytes; return failure if unsuccessful
         data_size = TO_32(strlen(concept_name) + 1);
         if (data_size > 250) return TITAN_RESULT_FILE_SAVE_ERROR;
         if (File->Write(file, &data_size, sizeof(TITAN_UINT), 1, num_bytes_written) != TITAN_RESULT_OK || data_size > 255) return TITAN_RESULT_FILE_SAVE_ERROR;
         if (File->Write(file, concept_name, sizeof(TITAN_BYTE), data_size, num_bytes_written) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_SAVE_ERROR;
         
         // Write reference data; return failure if unsuccessful
         if ((result = Reference->Save(m_reference, file, reference_handle)) != TITAN_RESULT_OK) return TITAN_RESULT_FILE_SAVE_ERROR;
         }
      }

   // Remember the scene
   printf("Scene remembered successfully\n");

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Get the scene instance value, specified by format <value.format_handle> from supplied <instance> and <element> pair, where:
      <value> is the value retrieved
      <handshake> is the handshake to be processed at the discretion of this method

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyScene::AtlasSim_GetValue(TITAN_HANDLE64 instance, TITAN_HANDLE64 element, atlas_value_t &value, TITAN_HANDLE64 handshake)
{
   scene_data_t                                       *scene;

   // Get data from element if it exists; otherwise, use instance data
   if (element) scene = _RP(scene_data_t, element);
   else if (instance) scene = _RP(scene_data_t, instance);
   else return TITAN_RESULT_ERROR;

   // If instance is invalid, return failure
   if (scene == NULL) return TITAN_RESULT_ERROR;

   // If the video interface is requested
   if (value.amount.format_handle == m_scene_interface_label)
      {
      // Return the window video interface
      value.amount.format_handle = 0;
      value.amount.handle_value = TO_HANDLE64(scene);
      // Return success
      return TITAN_RESULT_OK;
      }

   // Return failure otherwise
   return TITAN_RESULT_ERROR;
}

/*
   End of file
*/
