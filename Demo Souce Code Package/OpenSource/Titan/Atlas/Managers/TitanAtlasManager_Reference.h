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

#ifndef TITAN_ATLAS_MANAGER_REFERENCE_H
#define TITAN_ATLAS_MANAGER_REFERENCE_H

   // Atlas reference request types
   enum atlas_reference_request_t
      {
      ATLAS_REFERENCE_DESTROY,                        // Request reference destruction.
      ATLAS_REFERENCE_SET_MANAGER,                    // Request reference manager update.
      ATLAS_REFERENCE_GET_TRANSFORM,                  // Request for a reference transformation matrix.
      ATLAS_REFERENCE_REGISTER_MANAGER,               // Request for a reference manager to be registered.
      ATLAS_REFERENCE_LOAD,                           // Request for a reference to be loaded.
      ATLAS_REFERENCE_SAVE,                           // Request for a reference to be saved.
      ATLAS_REFERENCE_OBSERVE,                        // Request to observe reference.
      ATLAS_REFERENCE_ADD_LABEL,                      // Request for a label to be added to a reference.
      ATLAS_REFERENCE_REMOVE_LABEL,                   // Request for a label to be removed from a reference.
      ATLAS_REFERENCE_GET_PUBLIC_HANDLE,              // Request for a reference's public handle.
      };

   // Atlas reference manager "get attribute" request structure
   struct atlas_reference_request_attribute_t
      {
      TITAN_HANDLE64                                  reference;                             // Handle of the reference being queried.
      TITAN_UINT                                      attribute_id;                          // ID of the attribute requested.
      };

   // Atlas reference manager "get/set label" request structure
   struct atlas_reference_request_label_t
      {
      TITAN_HANDLE64                                  reference;                             // Handle of the reference being queried.
      TITAN_HANDLE64                                  label;                                 // Reference label.
      };

   // RX: Reference manager method prototype
   typedef titan_result_t (*AtlasReferenceManager)(TITAN_HANDLE64 caller_id, atlas_reference_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
