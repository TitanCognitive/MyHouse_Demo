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
#include "MyScene/MyScene.h"

/*
   Class constructor
*/
MyScene::MyScene(TITAN_HANDLE64 handshake) : TitanAxiom(handshake)
{
   // Initialize variables
   m_reference = 0;
   m_GL = NULL;

   // Clear default instance
   memset(&m_instance, 0, sizeof(scene_data_t));
   memset(&m_intercept, 0, sizeof(my_scene_intercept_t));
}

/*
   Class destructor
*/
MyScene::~MyScene()
{
   // If the pure reference exists, destroy it
   if (m_reference) Reference->Destroy(m_reference);
}

/*
   Get element specified by component id <component_id> of supplied directive <directive>, where:
      <reference_info> is the structure used to retrieve reference data of the first component matching <component_id> in the supplied directive 
      <instance_data> is the output pointer to the instance of the directive component
      <query_flags> is the parameter passed to the directive interface Get (See Directive::Get in Atlas SDK)

   Returns: Handle of the directive component retrieved upon success
            0 upon failure to retrieve the requested directive component
*/
TITAN_HANDLE64 MyScene::GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, 
                                   scene_data_t **instance_data, TITAN_BITMAP64 query_flags)
{
   TITAN_HANDLE64                                     component;

   // Get the directive component; return failure if unsuccessful
   if ((component = Directive->Get(directive, component_id, reference_info, query_flags | ATLAS_GET_INSTANCE | ATLAS_GET_REFERENCE)) == 0) return 0;
   // Make sure the returned reference can be handled by this concept; return failure otherwise
   if (Cognitive->IsCompatible(reference_info.reference) != true) return 0;

   // If instance data is requested
   if (instance_data)
      {
      // Get reference instance and/or element
      if (reference_info.element) *instance_data = _RP(scene_data_t, reference_info.element);
      else if ((*instance_data = _RP(scene_data_t, reference_info.instance)) == NULL) return 0;
      }

   // Return component handle
   return component;
}

/*
   Add supplied object to the scene
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/   
titan_result_t MyScene::Register(scene_data_t *scene, TITAN_HANDLE64 reference_handle, TITAN_FLOAT *transform) 
{
   TITAN_HANDLE64                                     *references = scene->references;
   TITAN_UINT                                         free_reference = TITAN_UINT_ERROR;
   TITAN_UINT                                         reference_index;
   TITAN_NAME                                         reference_name[256];
   atlas_reference_info_t                             reference_info = { 0 };

   // Initialize reference data
   reference_info.name = reference_name;
   reference_info.name_max_size = 255;

   // Go through all existing objects in the scene
   for (reference_index = 0; reference_index < scene->num_references; reference_index++)
      {
      // See if the object already exists in the scene; if so, return success
      if (references[reference_index] == reference_handle) return TITAN_RESULT_OK;
      if (free_reference == TITAN_UINT_ERROR && references[reference_index] == 0) free_reference = reference_index;
      }

   // If the reference is not found and a free slot exists
   if (free_reference != TITAN_UINT_ERROR)
      {
      // Use the free slot
      references[free_reference] = reference_handle;
      
      // Get registering reference transform
      scene->transforms[free_reference] = transform;
      // Get the registering reference name
      if (Reference->GetInfo(Reference->GetPublicHandle(reference_handle), reference_info) == TITAN_RESULT_OK && reference_name[0] > ' ')
         {
         // Get registering reference name
         scene->names[free_reference] = Tools->CopyText(reference_name);
         }
      
      // Return success
      return TITAN_RESULT_OK;
      }

   // Use last slot in the array (expand it) for this object
   references[scene->num_references] = reference_handle;

   // Get registering reference transform
   scene->transforms[scene->num_references] = transform;
   // Get the registering reference name
   if (Reference->GetInfo(Reference->GetPublicHandle(reference_handle), reference_info) == TITAN_RESULT_OK && reference_name[0] > ' ')
      {
      // Get registering reference name
      scene->names[scene->num_references] = Tools->CopyText(reference_name);
      }

   // Increase scene size
   if (scene->num_references < scene->max_references) scene->num_references++;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Remove supplied object from the scene
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/   
titan_result_t MyScene::Cancel(scene_data_t *scene, TITAN_HANDLE64 reference_handle)
{
   TITAN_UINT                                         reference_index;
   TITAN_UINT                                         num_references = scene->num_references;
   TITAN_HANDLE64                                     *references = scene->references;

   // Go through all existing references in the scene
   for (reference_index = 0, num_references = scene->num_references; reference_index < num_references; reference_index++)
      {
      // See if the reference already exists in the scene; if so, return success
      if (references[reference_index] == reference_handle)
         {
         // Free the slot
         references[reference_index] = 0;
         scene->transforms[reference_index] = 0;
         // Free the reference name
         if (scene->names[reference_index]) TITAN_FREE(scene->names[reference_index]);
         // Return success
         return TITAN_RESULT_OK;
         }
      }

   // Return 'Not found'
   return TITAN_RESULT_NOT_FOUND;
}

/*
   Show registered references
*/
void MyScene::ToggleRendering(scene_data_t *scene)
{
   // If there is no video intercept
   if (m_intercept.self == NULL)
      {
      // Set intercept data
      m_intercept.scene = scene;
      m_intercept.self = this;
      // Turn on information rendering
      m_GL->SetPostFrameCallback(7, &DisplayData, &m_intercept);
      }
   // Otherwise, turn off the intercept
   else
      {
      // Clear intercept data
      m_intercept.self = NULL;
      // Turn on information rendering
      m_GL->SetPostFrameCallback(7, &DisplayData, NULL);
      }
}

/*
   End of file
*/
