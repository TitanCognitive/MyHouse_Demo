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
#include "MyConsole/MyConsole.h"

/*
   BE: Online
   Turn on rendering of directive subject console
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyConsole::BE_Online(ATLAS_SIMULATE_PARAMS)
{
   MyConsole                                          *self = _RP(MyConsole, user_data);
   console_data_t                                     *console;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Get the directive subject and validate that it is in fact a console; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_SUBJECT, reference_info, &console)) == 0) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference_info.element) console = _RP(console_data_t, reference_info.element);
   else console = _RP(console_data_t, reference_info.instance);
   if (console == NULL) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If this console is happy, return true
      if (console->rendering == true) return TO_RESPONSE(100);
      return NO_RESPONSE();
      }
   else
      {
      // If this is not a query, set value
      console->rendering = true;
      }
 
   // Return true
   return TO_RESPONSE(100);
}

/*
   End of file
*/
