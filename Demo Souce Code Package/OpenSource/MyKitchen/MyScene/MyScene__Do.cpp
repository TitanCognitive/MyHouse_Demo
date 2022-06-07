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
   DO: Remember scene
   Returns: Relevance
*/
atlas_probe_result_t MyScene::DO_Remember(ATLAS_SIMULATE_PARAMS) 
{ 
   MyScene                                            *self = _RP(MyScene, user_data);
   scene_data_t                                       *scene;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;
   TITAN_NAME                                         filepath[256];
   TITAN_NAME                                         filename[256];
   TITAN_HANDLE64                                     file;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get reference name
   reference_info.name = filename;
   reference_info.name_max_size = 250;
   
   // Get the directive first object (transitive) and validate that it is in fact a scene; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &scene)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Append filename extension
   sprintf(filepath, "SIM/Kitchen/Data/%s.%s", filename, SCENE_FILE_EXTENSION);

   // Create a new file; return failure if unsuccessful
   if (File->OpenWrite(filepath, file) != TITAN_RESULT_OK)
      {
      // Report failure
      printf("Scene: Could not open file '%s' for writing. Scene will not be saved!\n", filename);
      // Close the opened file
      File->Close(file);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Write the scene instance; return failure if unsuccessful
   if (self->Instance->Save(file, self->m_concept, reference_info.instance, reference_info.element) != TITAN_RESULT_OK)
      {
      // Report failure
      printf("Scene: Could not open file '%s' for writing. Scene will not be saved!\n", filename);
      // Close the opened file
      File->Close(file);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Close the opened file
   File->Close(file);

   // Report success
   printf("Scene remembered successfully\n");

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Recall scene
   Returns: Relevance
*/
atlas_probe_result_t MyScene::DO_Recall(ATLAS_SIMULATE_PARAMS) 
{ 
   MyScene                                            *self = _RP(MyScene, user_data);
   scene_data_t                                       *scene;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;
   TITAN_NAME                                         filepath[256];
   TITAN_NAME                                         filename[256];
   TITAN_NAME                                         concept_name[256];
   TITAN_HANDLE64                                     file;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get reference name
   reference_info.name = filename;
   reference_info.concept_name = concept_name;
   reference_info.name_max_size = 250;

   // Get the directive first object (transitive) and validate that it is in fact a scene; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &scene)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Turn off the console
   self->Agent->Tell(scene->reference, 0, "deactivate console");

   // Append filename extension
   sprintf(filepath, "SIM/Kitchen/Data/%s.%s", filename, SCENE_FILE_EXTENSION);

   // Create an existing file; return failure if unsuccessful
   if (File->OpenRead(filepath, file) != TITAN_RESULT_OK)
      {
      // Report failure
      printf("Scene: Could not open file '%s' for reading. Scene will not be loaded!\n", filename);
      // Close the opened file
      File->Close(file);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Write the instance
   if (self->Instance->Load(reference_info.reference, file, self->m_concept, reference_info.instance, reference_info.element) != TITAN_RESULT_OK)
      {
      // Report scene loading error
      printf("Error: Could not load scene '%s'.\n", filename);
      // Close the opened file
      File->Close(file);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Close the opened file
   File->Close(file);

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Forget scene
   Returns: Relevance
*/
atlas_probe_result_t MyScene::DO_Forget(ATLAS_SIMULATE_PARAMS) 
{ 
   MyScene                                           *self = _RP(MyScene, user_data);
   scene_data_t                                      *scene;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a scene; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &scene)) == 0)
      {
      // Return failure
      return NO_RESPONSE();
      }

   // Return true
   return TO_RESPONSE(100);
}

/*
   End of file
*/
