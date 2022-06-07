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
#include "Atlas/TitanAtlas.h"
#include "AtlasWrapper.h"

/*
   Class constructor
*/
AtlasWrapper::AtlasWrapper()
{
   // Initialize framework
   Atlas = NULL;
}

/*
   Class destructor
*/
AtlasWrapper::~AtlasWrapper()
{
}

/*
   Create an Atlas instance with the starting configuration parameters <start_parameters>, where:
      <manifest_path> is the manifest to be loaded upon starting the Cognitive Space and creating a 'Atlas' domain
      <story_path> is the script to be executed upon entering 'Atlas' domain

   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t AtlasWrapper::Create(const TITAN_STRING start_parameters, const TITAN_STRING manifest_path, const TITAN_STRING script_path)
{
   titan_result_t                                     result;
   AtlasGetInterface                                  GetInterface;
   const TITAN_STRING                                 titan_path = "../_Atlas/KB/Codex/C00000001";
   const TITAN_STRING                                 atlas_version = "TITAN_ATLAS";

   // Load the interface method from; return failure if unsuccessful
   if ((GetInterface = LoadAtlas_WIN32(titan_path, atlas_version)) == NULL)
      {
      // Report error
      printf("TitanClient: Could not find Titan module (%s)\n", titan_path);
      // Return failure
      return TITAN_RESULT_RESOURCE_NOT_FOUND;
      }

   // Get Atlas interface from the dynamic library/object
   Atlas = (AtlasAPI) GetInterface();

   // If the interface is invalid
   if (Atlas == NULL) 
      { 
      // Report error
      printf("AtlasWrapper: Could not create Atlas (%s) environment\n", atlas_version); 
      // Return failure
      return TITAN_RESULT_INIT_FAIL; 
      }

   // Start Atlas; return result if unsuccessful
   if ((result = Atlas->Start(start_parameters, manifest_path, script_path)) != TITAN_RESULT_OK) return result;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   Load a Win32 library method from library <package_name> and method name <method_name>, where:
      <package_name> is the path name of the package loaded
      <method_name> is the method/Atlas version to be loaded from the package
      <return_handle> is an optional parameter to retrieve the handle of library/object loaded upon success

   Returns: Pointer to the AtlasGetInterface method of Titan environment upon successful load
            NULL upon failure to load the dynamic library/object package
*/
AtlasGetInterface AtlasWrapper::LoadAtlas_WIN32(const TITAN_STRING package_name, const TITAN_STRING method_name, TITAN_POINTER *return_handle)
{
   char                                               filename[256];
   TITAN_POINTER                                      handle;
   
   // Validate input; return failure if unsuccessful
   if (package_name == NULL || package_name[0] <= ' ') return NULL;
  
   // Specify filename
   sprintf(filename, "%s.dll", package_name);

   // If the file name is valid
   if (return_handle == NULL || *return_handle == NULL)
      {
      //  Load the library, if the library could not be loaded
      if ((handle = (TITAN_POINTER) LoadLibrary(filename)) == NULL)
         {
         // Report problem
         printf("TitanClient: Could not load library: %s\n", filename);
         // Return failure
         return NULL;
         }
      }

   // Store library handle if requested as an output parameter
   if (return_handle) *return_handle = handle;

   // Make sure the input is valid; otherwise, return failure
   if (handle == NULL) return NULL;

   // Return method address
   return (AtlasGetInterface) GetProcAddress((HMODULE)handle, method_name);  
}

/*
   End of file
*/
