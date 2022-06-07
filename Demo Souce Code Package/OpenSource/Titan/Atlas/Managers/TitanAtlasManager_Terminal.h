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

#ifndef TITAN_ATLAS_MANAGER_TERMINAL_H
#define TITAN_ATLAS_MANAGER_TERMINAL_H

   // Atlas terminal request types
   enum atlas_terminal_request_t
      {
      ATLAS_TERMINAL_DESTROY,                         // Terminal is about to be destroyed.
      ATLAS_TERMINAL_SHOW,                            // Terminal window is about to be shown.
      ATLAS_TERMINAL_LOSTFOCUS,                       // Terminal window is about to lose focus.
      ATLAS_TERMINAL_SIZED,                           // Terminal window is resized.
      ATLAS_TERMINAL_SIZING,                          // Terminal window is resizing.
      ATLAS_TERMINAL_WINDOW_DESTROY,                  // Terminal window is about to be destroyed.
      ATLAS_TERMINAL_ATTACH_DEVICE_INTERFACE,         // Terminal window is about to attach to a device interface.
      ATLAS_TERMINAL_DETACH_DEVICE_INTERFACE,         // Terminal window is about to detach from a device interface.
      };

   // Atlas terminal window show request types
   enum atlas_termimnal_window_show_request_t
      {
      ATLAS_TERMINAL_WINDOW_SIZED,                    // Window has been restored or sized.
      ATLAS_TERMINAL_WINDOW_OVERTAKEN,                // Window has been overtaken taken by another maximized window.
      ATLAS_TERMINAL_WINDOW_RESTORED,                 // Window has been restored from a previously overtaking window.
      ATLAS_TERMINAL_WINDOW_MAXIMIZED,                // Window has been maximized.
      ATLAS_TERMINAL_WINDOW_MINIMIZED,                // Window has been minimized.
      };

   // Atlas terminal window size request types
   enum atlas_termimnal_window_size_request_t
      {
      ATLAS_TERMINAL_WINDOW_LEFT,                     // Left side of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_RIGHT,                    // Right side of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_TOP,                      // Top side of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_BOTTOM,                   // Bottom side of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_TOP_LEFT,                 // Top-left corner of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_TOP_RIGHT,                // Top-right corner of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_BOTTOM_LEFT,              // Bottom-left corner side of the window is being dragged/sized.
      ATLAS_TERMINAL_WINDOW_BOTTOM_RIGHT,             // Bottom-right corner side of the window is being dragged/sized.
      };

   // Atlas terminal  manger request for a show update
   struct atlas_terminal_request_show_t
      {
      TITAN_INT                                       width;                                 // Width of the terminal window size.
      TITAN_INT                                       height;                                // Height of the terminal window size.
      atlas_termimnal_window_show_request_t           request_type;                          // Request type.
      };

   // Atlas terminal manger request for sizing a window
   struct atlas_terminal_request_size_t
      {
      TITAN_INT                                       left;                                  // Left side coordinate of the window.
      TITAN_INT                                       right;                                 // Right side coordinate of the window.
      TITAN_INT                                       top;                                   // Top side coordinate of the window.
      TITAN_INT                                       bottom;                                // Bottom side coordinate of the window.
      atlas_termimnal_window_size_request_t           request_type;                          // Request type.
      };

   // O1: Terminal manager
   typedef titan_result_t (*AtlasTerminalManager)(TITAN_HANDLE64 terminal, atlas_terminal_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
