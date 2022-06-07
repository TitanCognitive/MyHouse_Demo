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

#ifndef TITAN_ATLAS_MANAGER_DEVICE_PROXY_H
#define TITAN_ATLAS_MANAGER_DEVICE_PROXY_H

   // Atlas device proxy request types
   enum atlas_device_proxy_request_t
      {
      ATLAS_DEVICE_PROXY_DESTROY,                     // Device proxy is about to be destroyed.
      ATLAS_DEVICE_PROXY_ACTIVATE,                    // Device proxy is about to be activated.
      ATLAS_DEVICE_PROXY_DEACTIVATE,                  // Device proxy is about to be deactivated.
      ATLAS_DEVICE_PROXY_ATTACH_DEVICE,               // Device proxy is about to attach to a device.
      ATLAS_DEVICE_PROXY_DETACH_DEVICE,               // Device proxy is about to detach from a device.
      };

   // Atlas device proxy channel request types
   enum atlas_device_proxy_channel_request_t
      {
      ATLAS_DEVICE_PROXY_CHANNEL_DESTROY,             // Device proxy channel is about to be destroyed.
      ATLAS_DEVICE_PROXY_CHANNEL_RESET,               // Device proxy channel is about to be reset.
      ATLAS_DEVICE_PROXY_CHANNEL_ACTIVATE,            // Device proxy channel is about to be activated.
      ATLAS_DEVICE_PROXY_CHANNEL_DEACTIVATE,          // Device proxy channel is about to be deactivated.
      };

   // O4: Device proxy manager
   typedef titan_result_t (*AtlasDeviceProxyManager)(TITAN_HANDLE64 device_proxy, atlas_device_proxy_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);
   // O5: Device proxy channel  manager
   typedef titan_result_t (*AtlasDeviceProxyChannelManager)(TITAN_HANDLE64 device_proxy_channel, atlas_device_proxy_channel_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
