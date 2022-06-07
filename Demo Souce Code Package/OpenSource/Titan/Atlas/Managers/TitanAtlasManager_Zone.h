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

#ifndef TITAN_ATLAS_MANAGER_ZONE_H
#define TITAN_ATLAS_MANAGER_ZONE_H

   // Atlas context zone request types
   enum atlas_zone_request_t
      {
      ATLAS_ZONE_DESTROY,                             // Request zone destruction.
      ATLAS_ZONE_ENTER,                               // Request to enter a zone by an agent.
      ATLAS_ZONE_EXIT,                                // Request to exit a zone by an agent.
      };

   // Atlas zone manager access request structure
   struct atlas_zone_request_access_t
      {
      TITAN_HANDLE64                                  domain_label;                          // Label of the domain entered by the new context agent.
      TITAN_HANDLE64                                  zone_label;                            // Label of the zone entered by the new context agent.
      TITAN_HANDLE64                                  agent_id;                              // Handle of the agent entering the domain.
      };

   // ZX: Context zone manager method prototype
   typedef titan_result_t (*AtlasZoneManager)(TITAN_HANDLE64 caller_id, atlas_zone_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
