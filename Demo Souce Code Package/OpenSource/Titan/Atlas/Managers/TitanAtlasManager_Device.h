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

#ifndef TITAN_ATLAS_MANAGER_DEVICE_H
#define TITAN_ATLAS_MANAGER_DEVICE_H

   // Atlas device request types
   enum atlas_device_request_t
      {
      ATLAS_DEVICE_DESTROY,                           // Device is about to be destroyed.
      ATLAS_DEVICE_RESET,                             // Device is about to reset.
      ATLAS_DEVICE_ACTIVATE,                          // Device is about to be activated.
      ATLAS_DEVICE_DEACTIVATE,                        // Device is about to be deactivated.
      ATLAS_DEVICE_ATTACH_DEVICE_INTERFACE,           // Device is about to attach to a device interface.
      ATLAS_DEVICE_DETACH_DEVICE_INTERFACE,           // Device is about to detach from a device interface.
      ATLAS_DEVICE_ATTACH_DEVICE_PROXY,               // Device is about to attach to a device proxy.
      ATLAS_DEVICE_DETACH_DEVICE_PROXY,               // Device is about to detach from a device proxy.
      };

   // O3: Device manager
   typedef titan_result_t (*AtlasDeviceManager)(TITAN_HANDLE64 device, atlas_device_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);
   // Device sampling method prototype
   typedef titan_result_t (*AtlasDeviceGetMethod)(TITAN_HANDLE64 device_handle, atlas_device_method_t *method, TITAN_HANDLE64 value_id, TITAN_HANDLE64 &value);
   // Device sampling method prototype
   typedef titan_result_t (*AtlasDeviceSetMethod)(TITAN_HANDLE64 device_handle, atlas_device_method_t *method, TITAN_HANDLE64 value_id, TITAN_HANDLE64 value);
   // Device sampling method prototype
   typedef titan_result_t(*AtlasDeviceSampleMethod)(TITAN_HANDLE64 device_handle, atlas_device_method_t *method, TITAN_DOUBLE sample_time);
   // Device sampling method prototype
   typedef titan_result_t(*AtlasDeviceResetMethod)(TITAN_HANDLE64 device_handle, atlas_device_method_t *method);

   // Atlas device method structure
   struct atlas_device_method_t
      {
      AtlasDeviceGetMethod                            GetMethod;                             // Atlas device get method
      AtlasDeviceSetMethod                            SetMethod;                             // Atlas device set method
      AtlasDeviceSampleMethod                         SampleMethod;                          // Atlas device sample method
      AtlasDeviceResetMethod                          ResetMethod;                           // Atlas device reset method
      TITAN_HANDLE64                                  code_handle;                           // Code handle sent to the manager method
      TITAN_HANDLE64                                  data_handle;                           // Data handle sent to the manager method
      };

#endif

/*
   End of file
*/
