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
#ifndef MY_CLIENT_CONCEPT_H
#define MY_CLIENT_CONCEPT_H

   // Dependencies
   #include "Atlas/TitanAtlasAxiom.h"

   // Person structure
   struct client_data_t
      {
      TITAN_BOOL                                      on;
      TITAN_BOOL                                      running;
      };

   // Client axiom class 
   class MyClient : public TitanAxiom
      {
      // User-defined agent profile
      struct agent_profile_t
         {
         TITAN_HANDLE64                               clone;                                 // Clone handle
         TITAN_HANDLE64                               agent;                                 // Context agent handle
         TITAN_HANDLE64                               reference;                             // Reference handle of the context agent
         TITAN_HANDLE64                               activity_id;                           // Activity ID of the context agent
         atlas_method_t                               activity;                              // Activity of the context agent
         };

      public:
         // Framework methods:
         MyClient(TITAN_HANDLE64 handshake);
         ~MyClient();
         static void AgentActivity(atlas_method_t *method);

      protected:
         // Axiom methods:
         // T1: Attach concept to the knowledge base
         virtual titan_result_t AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params);
         // Instantiator methods:
         // I1: Create an instance for supplied reference
         virtual TITAN_HANDLE64 AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake = 0);
         // I2: Destroy a supplied instance
         virtual titan_result_t AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake = 0);


         // NLP methods:
         // BE:
         TITAN_NLP_QUERY BeOn(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY BeOff(ATLAS_SIMULATE_PARAMS);
         // DO:
         TITAN_NLP_QUERY DoStart(ATLAS_SIMULATE_PARAMS);
         TITAN_NLP_QUERY DoStop(ATLAS_SIMULATE_PARAMS);
      
      private:
         // Private methods:
         titan_result_t CreateActiveAgentAndClone(const TITAN_STRING agent_name, const TITAN_STRING clone_name, const TITAN_STRING domain_name, const TITAN_STRING zone_name, TITAN_FLOAT frequency);
         titan_result_t CreatePassiveAgentAndClone(const TITAN_STRING agent_name, const TITAN_STRING clone_name, const TITAN_STRING domain_name, const TITAN_STRING zone_name);

      protected:
         // Protected variables:

      private:
         // Private variables:
         TITAN_HANDLE64                               m_concept;                             // Concept handle
         agent_profile_t                              m_clientbot;                           // Client agent
         agent_profile_t                              m_jimmy;                               // Jimmy context agent profile
         TITAN_HANDLE64                               m_local_domain_handle;                 // Domain handle
         TITAN_HANDLE64                               m_local_context_handle;                // Domain context handle
      };

#endif

/*
   End of file
*/
