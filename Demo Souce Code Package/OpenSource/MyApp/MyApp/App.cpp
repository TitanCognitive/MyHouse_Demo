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
#include "AtlasWrapper/AtlasWrapper.h"

/*
   Program main entry point
   Returns: 0 upon success
           -1 upon failure

   MyApp.exe
      [-config] to set start parameters (configuration string)
      [-client] starting client concept
      [-manifest] to set the starting manifest
      [-script] to set the starting script
*/
int main(int argument_count, char *arguments[])
{
   AtlasWrapper                                       Atlas;
   TITAN_BOOL                                         hide_console = true;
   int                                                argument_index;
   char                                               *config_string = NULL;
   char                                               *manifest_name = "manifest.txt";
   char                                               *script_name = "script.txt";

   // Go through all the arguments in the command line that can take an extra parameter
   for (argument_index = 1; argument_index < argument_count - 1; argument_index++)
      {
      // If this is a configuration keyword
      if (strcmp(arguments[argument_index], "-config") == 0)
         {
         // Get Atlas configuration string
         config_string = arguments[argument_index + 1];
         }
      // If this is a manifest keyword
      else if (strcmp(arguments[argument_index], "-manifest") == 0)
         {
         // Get manifest name
         manifest_name = arguments[argument_index + 1];
         }
      // If this is a script keyword
      else if (strcmp(arguments[argument_index], "-script") == 0)
         {
         // Get script name
         script_name = arguments[argument_index + 1];
         }
      // If this application console is in debug/show mode
      if (strcmp(arguments[argument_index], "-show") == 0)
         {
         // Do not hide console window
         hide_console = false;
         }
      }

   // Go through the last argument in the command line, which won't take an extra parameter
   for (; argument_index < argument_count; argument_index++)
      {
      // If this application console is in debug/show mode
      if (strcmp(arguments[argument_index], "-show") == 0)
         {
         // Do not hide console window
         hide_console = false;
         }
      }

   // If the console window is to be hidden
   if (hide_console)
      {
      // Hide the window
      ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
      }

   // Initialize Atlas; report error if unsuccessful
   if (Atlas.Create(config_string, manifest_name, script_name) != TITAN_RESULT_OK)
      { 
      // Report error and exit program
      printf("MyClient: Unable to initialize Titan core\n"); 
      // Pause the system before leaving in debug mode
      #ifdef _DEBUG  
      system("Pause");
      #endif
      // Return failure
      return -1;
      }
   
   // Stall this thread while Atlas runs in parallel.
   // Under different circumstances, the calling application can resume its typical routine.
   for (;;) Delay(100000);

   // Pause the system before leaving in debug mode
   #ifdef _DEBUG  
   system("Pause");
   #endif
    
   // We are done
   return 0;
}

/*
   End of file
*/
