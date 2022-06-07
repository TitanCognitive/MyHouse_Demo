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

#ifndef TITAN_ATLAS_MANAGER_DEVICE_INTERFACE_H
#define TITAN_ATLAS_MANAGER_DEVICE_INTERFACE_H

   // Atlas terminal request types
   enum atlas_device_interface_request_t
      {
      ATLAS_DEVICE_INTERFACE_DESTROY,                 // Device interface is about to be destroyed.
      ATLAS_DEVICE_INTERFACE_ACTIVATE,                // Device interface is about to be activated.
      ATLAS_DEVICE_INTERFACE_DEACTIVATE,              // Device interface is about to be deactivated.
      ATLAS_DEVICE_INTERFACE_ATTACH_TERMINAL,         // Device interface is about to attach to a terminal.
      ATLAS_DEVICE_INTERFACE_DETACH_TERMINAL,         // Device interface is about to detach from a terminal.
      ATLAS_DEVICE_INTERFACE_ATTACH_DEVICE,           // Device interface is about to attach to a device.
      ATLAS_DEVICE_INTERFACE_DETACH_DEVICE,           // Device interface is about to detach from a device.
      };

   // O2: Device interface manager
   typedef titan_result_t (*AtlasDeviceInterfaceManager)(TITAN_HANDLE64 device, atlas_device_interface_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
