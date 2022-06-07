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

#ifndef TITAN_ATLAS_H
#define TITAN_ATLAS_H
   
   // Main Titan definitions
   #include "Common/TitanDefinitions.h"
   
   #ifdef _WIN32
   #include <stdlib.h>
   #include <stdio.h>
      #include <windows.h>
      #define Delay(X)                                Sleep(X * 1000)                        // Sleep in seconds (1000 * ms)
   #else
      #include <unistd.h>
      #define Delay(X)                                sleep(X)                               // Sleep in seconds
   #endif

   // Main interface method prototype
   typedef TITAN_POINTER (*AtlasGetInterface)();

   // Atlas interface
   typedef struct _AtlasAPI
      {
      // Start Titan
      virtual titan_result_t Start(const TITAN_STRING start_parameters, const TITAN_STRING manifest_path = NULL, const TITAN_STRING script_path = NULL) = 0;
      // Stop Titan
      virtual titan_result_t Stop(const TITAN_STRING stop_parameters = NULL) = 0;
      } *AtlasAPI;


#endif

/*
   End of file
*/
