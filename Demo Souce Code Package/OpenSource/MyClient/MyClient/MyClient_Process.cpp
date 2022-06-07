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
#include "MyClient.h"

// Define dependencies
#define TITAN_SYSTEM_MEMORY
#define TITAN_SYSTEM_TOOLS

// Include dependencies
#include TITAN_DEPENDENCIES

/*
   Activity of the client context agent
*/
void MyClient::AgentActivity(atlas_method_t *method)
{
   MyClient                                           *self = _RP(MyClient, method->code_handle);
   TITAN_HANDLE64                                     reference = method->data_handle;
   TITAN_NAME                                         input_text[512];
   TITAN_DOUBLE                                       start_time, end_time;

   // Print prompt
   printf("\nYes?\n");

   // Grab user input
   fflush(stdin);
   fseek(stdin, 0, SEEK_END);

   // Parse input one line at a time
   if (scanf("%512[^\n]", input_text) != 0)
      {
      // If a termination is requested, quit
      if (strcmp(input_text, "/q") == 0) exit(0);

      // Get system time
      start_time = Tools->GetTime();
      // Process the input expression
      self->Agent->Tell(reference, 0, input_text, NULL);
      end_time = Tools->GetTime() - start_time;
      // Print output
      printf("Time elapsed: %.05f msec\n", TO_FLOAT(end_time));
      }
}

/*
   End of file
*/
