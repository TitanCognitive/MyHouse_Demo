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

// Dependencies
#include "Common/Titan.h"

// Titan framework dependencies
#define TITAN_SYSTEM_MEMORY
#define TITAN_SYSTEM_TOOLS
#include TITAN_DEPENDENCIES

// Dependencies
#include "MyLight/MyLight.h"

// Initialize the static variables
TITAN_HANDLE64                                        MyLight::m_coordinate_interface_label = NULL;
TITAN_HANDLE64                                        MyLight::m_coordinate_label = NULL;
TITAN_HANDLE64                                        MyLight::m_transform_label = NULL;
TITAN_HANDLE64                                        MyLight::m_color_label = NULL;

/*
   Class constructor
*/
MyLight::MyLight(TITAN_HANDLE64 handshake) : TitanAxiom(handshake)
{
   // Initialize framework
   // Initialize variables
   m_reference = 0;

   // Initialize pointers
   m_GL = NULL;
   m_scene = NULL;
   m_camera = NULL;
}

/*
   Class destructor
*/
MyLight::~MyLight()
{
   // If the pure reference exists, destroy it
   if (m_reference) Reference->Destroy(m_reference);
}

/*
   Get element specified by component id <component_id> of supplied directive <directive>, where:
      <reference_info> is the structure used to retrieve reference data of the first component matching <component_id> in the supplied directive 
      <instance_data> is the output pointer to the instance of the directive component
      <query_flags> is the parameter passed to the directive interface Get (See Directive::Get in Atlas SDK)

   Returns: Handle of the directive component retrieved upon success
            0 upon failure to retrieve the requested directive component
*/
TITAN_HANDLE64 MyLight::GetElement(TITAN_HANDLE64 directive, atlas_directive_component_t component_id, atlas_reference_info_t &reference_info, 
                                   light_data_t **instance_data, TITAN_BITMAP64 query_flags)
{
   TITAN_HANDLE64                                     component;

   // Get the directive component; return failure if unsuccessful
   if ((component = Directive->Get(directive, component_id, reference_info, query_flags | ATLAS_GET_INSTANCE | ATLAS_GET_REFERENCE)) == 0) return 0;
   // Make sure the returned reference can be handled by this concept; return failure otherwise
   if (Cognitive->IsCompatible(reference_info.reference) != true) return 0;

   // If instance data is requested
   if (instance_data)
      {
      // Get reference instance and/or element
      if (reference_info.element) *instance_data = _RP(light_data_t, reference_info.element);
      else if ((*instance_data = _RP(light_data_t, reference_info.instance)) == NULL) return 0;
      }

   // Return component handle
   return component;
}

/*
   End of file
*/
