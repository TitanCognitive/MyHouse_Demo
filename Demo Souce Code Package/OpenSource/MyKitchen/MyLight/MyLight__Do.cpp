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

/*
   Do: Activate
   Turn on the directive object light
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Activate(ATLAS_SIMULATE_PARAMS) 
{ 
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get subject group from directive; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light is turned on, return true
      if (light->on) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Turn on the light
   light->on = true;
   self->m_GL->Light->EnableLight(light->light_index);
   //wwm_GL->Light->EnableLight()

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Brighten 
   Increase intensity of the directive object light by 20%
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Brighten(ATLAS_SIMULATE_PARAMS)
{
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get subject group from directive; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light is bright, return true
      if (light->attenuation < 1.0f) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Increase light intensity
   light->attenuation *= (1.0f/1.8f);
   self->m_GL->Light->SetLightIntensity(light->light_index, light->attenuation);

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Dim 
   Reduce intensity of the directive object light by 20%
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Dim(ATLAS_SIMULATE_PARAMS)
{
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get subject group from directive; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light is not bright, return true
      if (light->attenuation < 1.0f) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Decrease light intensity
   light->attenuation *= 1.8f;
   self->m_GL->Light->SetLightIntensity(light->light_index, light->attenuation);

   // Return true
   return TO_RESPONSE(100);
}


/*
   Do: Focus 
   Focus emission (reduce spotlight radius) of the directive object light. The light must be directional.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Focus(ATLAS_SIMULATE_PARAMS)
{
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a light; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light is focused , return true
      if (light->focus >= 20.0f) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Increase light focus
   light->focus *= 1.2f;
   if (light->focus > 128.0f) light->focus = 128.0f;
   self->m_GL->Light->SetLightFocus(light->light_index, light->focus);

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Diffuse 
   Defuse emission (reduce spotlight radius) of the directive object light. The light must be directional.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Diffuse(ATLAS_SIMULATE_PARAMS)
{
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a light; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light is not focused, return true
      if (light->focus < 20.0f) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Decrease light focus
   light->focus *= (1.0f / 1.2f);
   if (light->focus < 0.1f) light->focus = 0.1f;
   self->m_GL->Light->SetLightFocus(light->light_index, light->focus);

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Open 
   Open aperture of the directive object light. The light must be directional.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Open(ATLAS_SIMULATE_PARAMS)
{
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a light; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light cone is open, return true
      if (light->aperture_angle >= 90.0f) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Increase light cut-off angle
   light->aperture_angle *= 1.2f;
   if (light->aperture_angle > 180.0f) light->aperture_angle = 180.0f;
   self->m_GL->Light->SetLightCutoffAngle(light->light_index, light->aperture_angle);

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Close 
   Close aperture of the directive object light. The light must be directional.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Close(ATLAS_SIMULATE_PARAMS)
{
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a light; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light cone is not open, return true
      if (light->aperture_angle < 90.0f) return TO_RESPONSE(100);
      // Otherwise, return false
      return TO_RESPONSE(-100);
      }

   // Decrease light cut-off angle
   light->aperture_angle *= (1.0f / 1.2f);
   if (light->aperture_angle < 0.1f) light->aperture_angle = 0.1f;
   self->m_GL->Light->SetLightCutoffAngle(light->light_index, light->aperture_angle);

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Select
   Select directive object light for editing. Light will take keyboard and mouse focus.
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_Deactivate(ATLAS_SIMULATE_PARAMS) 
{ 
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a light; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // If the light is turned on, return true
      if (light->on) return TO_RESPONSE(-100);
      // Otherwise, return false
      return TO_RESPONSE(100);
      }

   // Turn on the light
   light->on = false;
   self->m_GL->Light->DisableLight(light->light_index);
   //wwm_GL->Light->EnableLight()

   // Return true
   return TO_RESPONSE(100);
}

/*
   Do: Destroy
   Destroy directive object reference and its associated instance
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyLight::DO_SYS_Destroy(ATLAS_SIMULATE_PARAMS) 
{ 
   MyLight                                           *self = _RP(MyLight, user_data);
   light_data_t                                      *light;
   atlas_reference_info_t                             reference_info = { 0 };
   TITAN_HANDLE64                                     component;

   // Validate input; return failure if unsuccessful
   if (self == NULL) return NO_RESPONSE();

   // Get the directive first object (transitive) and validate that it is in fact a light; return failure if unsuccessful
   if ((component = self->GetElement(directive, ATLAS_DIRECTIVE_OBJECT1, reference_info, &light)) == 0) return NO_RESPONSE();

   // Destroy the system light and its reference
   if (light->reference) self->Reference->Destroy(light->reference);

   // Return true
   return NO_RESPONSE();
}


/*
   End of file
*/
