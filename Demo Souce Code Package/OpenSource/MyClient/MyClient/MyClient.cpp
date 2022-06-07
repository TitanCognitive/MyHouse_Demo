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
#include "MyClient.h"

// Define dependencies
#define TITAN_SYSTEM_MEMORY
#define TITAN_SYSTEM_TOOLS

// Include dependencies
#include TITAN_DEPENDENCIES

/*
   Class constructor
*/
MyClient::MyClient(TITAN_HANDLE64 handshake) : TitanAxiom(handshake)
{
   // Initialize variables
   m_concept = 0;
   m_local_domain_handle = 0;
   m_local_context_handle = 0;

   // Clear pointers

   // Clear data structures
   memset(&m_jimmy, sizeof(agent_profile_t), 0);
   memset(&m_clientbot, sizeof(agent_profile_t), 0);
}

/*
   Class destructor
*/
MyClient::~MyClient()
{
}

/*
   Create a client agent and a corresponding (active) Atlas clone that services it, where:
      <agent_name> is the name of the context agent being created
      <clone_name> is the name of the Atlas clone being created
      <domain_name> is the name of the domain to be entered by the context agent
      <zone_name> is the name of the zone to be entered by the context agent
      <frequency> frequency of activity of the Atlas clone
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyClient::CreateActiveAgentAndClone(const TITAN_STRING agent_name, const TITAN_STRING clone_name, const TITAN_STRING domain_name, const TITAN_STRING zone_name, TITAN_FLOAT frequency)
{
   // Create a client context agent named <agent_name>, return failure if unsuccessful
   printf("Creating an agent, '%s', in the '%s' domain, entering zone '%s'.\n", agent_name, domain_name, zone_name);
   if ((m_clientbot.agent = Agent->Create(domain_name, zone_name, 16, m_clientbot.reference, agent_name, m_concept, 1, 0)) == 0)
      {
      // Report error
      printf("Failed to create context agent '%s' in the '%s' domain.\n", agent_name, domain_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Create an Atlas clone to service the client agent; return failure if unsuccessful
   printf("Creating an Atlas clone, named '%s', running at <%.02f> Hz.\n", clone_name, frequency);
   if ((m_clientbot.clone = Simulation->CreateAtlasClone(m_clientbot.reference, clone_name, frequency, 4, 0)) == 0)
      {
      // Report error
      printf("Failed to create an Atlas clone '%s'.\n", clone_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Subscribe the context agent to the Atlas clone
   printf("Subscribing context agent '%s' to Atlas clone '%s'.\n", agent_name, clone_name);
   // Join an existing process; return failure if unsuccessful
   if (Agent->JoinAtlasClone(m_clientbot.agent, clone_name) != TITAN_RESULT_OK)
      {
      // Report issue
      printf("Context agent '%s' could not subscribe to Atlas clone '%s.\n", agent_name, clone_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Create an activity for the context agent to be handled by its hosting clone
   m_clientbot.activity.code_handle = TO_HANDLE64(this);
   m_clientbot.activity.data_handle = m_clientbot.reference;
   m_clientbot.activity.ActivityMethod = &MyClient::AgentActivity;

   printf("Starting an activity for the client agent, '%s', that will be handled by Atlas clone '%s'.\n", agent_name, clone_name);
   // Start the activity; return failure if unsuccessful
   if ((m_clientbot.activity_id = Agent->StartActivity(m_clientbot.reference, 0, &m_clientbot.activity)) == 0)
      {
      // Report error
      printf("Failed to create an activity for '%s'.\n", agent_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Return success
   return TITAN_RESULT_OK;

}

/*
   Create a context agent and a corresponding (passive) Atlas clone that services it
      <agent_name> is the name of the context agent being created
      <clone_name> is the name of the Atlas clone being created
      <domain_name> is the name of the domain to be entered by the context agent
      <zone_name> is the name of the zone to be entered by the context agent
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyClient::CreatePassiveAgentAndClone(const TITAN_STRING agent_name, const TITAN_STRING clone_name, const TITAN_STRING domain_name, const TITAN_STRING zone_name)
{
   // Create a client context agent named <agent_name>, return failure if unsuccessful
   printf("Creating an agent, '%s', in the '%s' domain, entering zone '%s'.\n", agent_name, domain_name, zone_name);
   if ((m_jimmy.agent = Agent->Create(domain_name, zone_name, 16, m_jimmy.reference, agent_name, m_concept, 1, 0)) == 0)
      {
      // Report error
      printf("Failed to create context agent '%s' in the '%s' domain.\n", agent_name, domain_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Create an Atlas clone to service the client agent; return failure if unsuccessful
   printf("Creating an Atlas clone, named '%s'.\n", clone_name);
   // Create the Atlas clone with a -1 frequency - this makes the clone exclusive to the agent that subscribes to it
   if ((m_jimmy.clone = Simulation->CreateAtlasClone(m_jimmy.reference, clone_name, -1.0f, 4, 0)) == 0)
      {
      // Report error
      printf("Failed to create an Atlas clone '%s'.\n", clone_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Subscribe the context agent to the Atlas clone
   printf("Subscribing context agent '%s' to Atlas clone '%s'.\n", agent_name, clone_name);
   // Join an existing process; return failure if unsuccessful
   if (Agent->JoinAtlasClone(m_jimmy.agent, clone_name) != TITAN_RESULT_OK)
      {
      // Report issue
      printf("Context agent '%s' could not subscribe to Atlas clone '%s.\n", agent_name, clone_name);
      // Return failure
      return TITAN_RESULT_ERROR;
      }

   // Return success
   return TITAN_RESULT_OK;
}

/*
   End of file
*/
