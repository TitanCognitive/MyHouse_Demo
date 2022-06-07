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

#ifndef TITAN_ATLAS_ATTIBUTES_H
#define TITAN_ATLAS_ATTIBUTES_H

   // Atlas attribute IDs
   #define ATLAS_ATTRIBUTE_LOCATION                   0x01                                   // Handle location feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_SIZE                       0x02                                   // Handle size feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_SHAPE                      0x04                                   // Handle shape feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_TEXTURE                    0x08                                   // Handle texture feature internally when this bit is ON
   #define ATLAS_ATTRIBUTE_COLOR                      0x10                                   // Handle color feature internally when this bit is ON

   // Atlas attribute scope states
   enum atlas_attribute_scope_t
      {
      ATLAS_ATTRIB_SCOPE_CONCEPT = 0,                                                        // Attribute is common for all instances of a concept
      ATLAS_ATTRIB_SCOPE_INSTANCE,                                                           // Attribute is common for all elements of an instance
      ATLAS_ATTRIB_SCOPE_ELEMENT,                                                            // Attribute is different for each element in the instance
      TITAN_ATTRIB_SCOPE_COUNT,                                                              // Total number attribute types supported
      };

   // Atlas simulation coordinate structure
   struct atlas_coordinate_t
      {
      TITAN_BITMAP32                                  update_flags;                          // Coordinate update flags
      TITAN_BITMAP32                                  trigger_flags;                         // Coordinate trigger flags
      titan_vec4_t                                    position;                              // Position of coordinate
      titan_vec4_t                                    angle;                                 // Angle of coordinate
      titan_vec4_t                                    scale;                                 // Scale of coordinate
      TITAN_UINT                                      prev;                                  // Previous coordinate index in the animation
      TITAN_UINT                                      next;                                  // Next coordinate index in the animation
      atlas_method_t                                  trigger;                               // Coordinate event trigger
      };

   // Atlas simulation joint animation structure definition
   struct atlas_animation_track_t
      {
      TITAN_UINT                                      num_coordinates;                       // Number of joint coordinates in the animation
      TITAN_BITMAP32                                  reserved;                              // Reserved
      atlas_coordinate_t                              *coordinates;                          // Joint animation coordinates
      };

   // Atlas simulation animation structure definition
   struct atlas_animation_t
      {
      TITAN_UINT                                      num_tracks;                            // Number of tracks in the animation (usually one per joint)
      TITAN_BITMAP32                                  reserved;                              // Reserved
      atlas_animation_track_t                         *track;                                // Animation tracks (usually one per joint)
      };

   // Atlas simulation joint animation structure
   struct atlas_joint_animation_t
      {
      TITAN_UINT                                      num_coordinates;                       // Number of joint coordinates in the animation
      TITAN_FLOAT                                     sim_time;                              // Animation simulation time
      TITAN_UINT                                      current_index;                         // Current coordinate index
      TITAN_UINT                                      next_index;                            // Next coordinate index
      atlas_coordinate_t                              *coordinates;                          // Joint animation coordinates
      };

#endif

/*
   End of file
*/
