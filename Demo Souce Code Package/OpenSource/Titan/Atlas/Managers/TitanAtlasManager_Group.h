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

#ifndef TITAN_ATLAS_MANAGER_GROUP_H
#define TITAN_ATLAS_MANAGER_GROUP_H

   // Atlas group request types
   enum atlas_group_request_t
      {
      ATLAS_GROUP_DESTROY,                            // Request group destruction.
      ATLAS_GROUP_LOCK,                               // Request to lock the group.
      ATLAS_GROUP_ADD_MEMBER,                         // Request to add a group member.
      ATLAS_GROUP_REMOVE_MEMBER,                      // Request to remove a group member.
      ATLAS_GROUP_CLEAR,                              // Request to clear the group.
      ATLAS_GROUP_GET_MEMBER_COUNT,                   // Request number of members in the group.
      ATLAS_GROUP_GET_MEMBER,                         // Request group member.
      };

   // GX: Group manager method prototype
   typedef titan_result_t (*AtlasGroupManager)(TITAN_HANDLE64 caller_id, atlas_group_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
