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

#ifndef TITAN_ATLAS_MANAGER_DIRECTIVE_H
#define TITAN_ATLAS_MANAGER_DIRECTIVE_H

   // Atlas ping message type
   enum atlas_ping_type_t
      {     
      ATLAS_PING_VALIDATE,                            // Agent should validate ping directive.
      ATLAS_PING_OBSERVE,                             // Agent should observe ping directive.
      ATLAS_PING_TRANSLATE,                           // Agent should translate ping directive to natural language.
      ATLAS_PING_REMEMBER,                            // Agent should remember ping directive.
      ATLAS_PING_FORGET,                              // Agent should forget ping directive.
      ATLAS_PING_SIMULATE,                            // Agent should simulate ping data.
      };

   // Atlas directive request types
   enum atlas_directive_request_t
      {
      ATLAS_DIRECTIVE_GET,                            // Request component or state information from a directive.
      ATLAS_DIRECTIVE_REPHRASE,                       // Request a directive component to be rephrased.
      ATLAS_DIRECTIVE_SET_HANDSHAKE,                  // Request directive handshakes to be set.
      ATLAS_DIRECTIVE_REMEMBER,                       // Request for directive to be remembered.
      ATLAS_DIRECTIVE_FORGET,                         // Request for directive to be forgotten.
      };

   // Atlas directive header structure definition
   struct atlas_directive_header_t
      {
      TITAN_BITMAP32                                  components;                            // Bitmap of component bits populating the directive (subject, object, etc.)
      TITAN_UBYTE                                     query;                                 // Query flags (Who, When,Where, What, Why, How, Amount/Count)
      TITAN_BITMAP8                                   tense;                                 // Tense bits of the directive (past, present, future, continuous, etc.)
      TITAN_BYTE                                      value;                                 // Intensity of expression
      };

   // XX: Directive manager method prototype
   typedef titan_result_t (*AtlasDirectiveManager)(TITAN_HANDLE64 caller_id, atlas_directive_request_t request_type, atlas_method_t *method, TITAN_HANDLE64 message_data, TITAN_HANDLE64 handshake);

#endif

/*
   End of file
*/
