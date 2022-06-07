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
   Attach axiom to the knowledge base
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/ 
titan_result_t MyClient:: AtlasKB_Attach(TITAN_HANDLE64 caller_id, TITAN_HANDLE64 concept, const TITAN_STRING attach_params)
{  
   titan_result_t                                     result;

   // Get system dependencies   
   // If the memory API is not initialized; request a new interace
   if (Memory == NULL) return TITAN_RESULT_NOT_INITIALIZED;
   // If the tools API is not initialized; request a new interace
   if (Tools == NULL) return TITAN_RESULT_NOT_INITIALIZED;
   
   // Remember concept handle
   m_concept = concept;

   // BE block setup   
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "On", 0, TO_HANDLE64(this), &MyClient::BeOn)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Be(ATLAS_PERSPECTIVE_SUBJECT, "Off", 0, TO_HANDLE64(this), &MyClient::BeOff)) != TITAN_RESULT_OK) return result;
   // DO block setup
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Start", 0, TO_HANDLE64(this), &MyClient::DoStart)) != TITAN_RESULT_OK) return result;
   if ((result = Cognitive->Do(ATLAS_PERSPECTIVE_OBJECT, "Stop", 0, TO_HANDLE64(this), &MyClient::DoStop)) != TITAN_RESULT_OK) return result;

   // Set instantiator; return failure if unsuccessful
   if ((result = Cognitive->SetInstantiator(TO_INSTANTIATOR(this))) != TITAN_RESULT_OK) return result;

   // Return success
   return TITAN_RESULT_OK;
}

/*
   BE: On
   Set the directive subject application to ON (running)
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyClient::BeOn(ATLAS_SIMULATE_PARAMS)
{
   MyClient                                           *self = _RP(MyClient, user_data);
   client_data_t                                      *client;
   atlas_reference_info_t                             reference = { 0 };

   // Get the subject; return failure if unsuccessful
   if (self->Directive->Get(directive, ATLAS_DIRECTIVE_SUBJECT, reference) != TITAN_RESULT_OK) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference.element) client = _RP(client_data_t, reference.element);
   else client = _RP(client_data_t, reference.instance);

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return client state
      if (client->on == true) return TO_RESPONSE(100);
      return NO_RESPONSE();
      }

   // Set client to ON
   client->on = true;

   // Return true
   return TO_RESPONSE(100);
}

/*
   BE: Off
   Set the directive subject application to OFF (not running)
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyClient::BeOff(ATLAS_SIMULATE_PARAMS)
{
   MyClient                                           *self = _RP(MyClient, user_data);
   client_data_t                                      *client;
   atlas_reference_info_t                             reference = { 0 };

   // Get subject group from directive; return failure if unsuccessful
   if (self->Directive->Get(directive, ATLAS_DIRECTIVE_SUBJECT, reference, ATLAS_GET_INSTANCE) != TITAN_RESULT_OK) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference.element) client = _RP(client_data_t, reference.element);
   else client = _RP(client_data_t, reference.instance);

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return client state
      if (client->on == false) return TO_RESPONSE(100);
      return NO_RESPONSE();
      }

   // Set client to OFF
   client->on = false;

   // Return true
   return TO_RESPONSE(100);
}

/*
   DO: Start
   Start directive object application / Set the directive object application to ON (running)
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyClient::DoStart(ATLAS_SIMULATE_PARAMS) 
{ 
   MyClient                                           *self = _RP(MyClient, user_data);
   client_data_t                                      *client;
   atlas_reference_info_t                             reference = { 0 };
   TITAN_HANDLE64                                     subject_handle;
   const TITAN_STRING                                 manifest_name = "SIM/Kitchen/my_kitchen_manifest.txt";
   const TITAN_STRING                                 script_name = "SIM/Kitchen/my_kitchen_script.txt";

   // Get subject group from directive; return failure if unsuccessful
   if ((subject_handle = self->Directive->Get(directive, ATLAS_DIRECTIVE_OBJECT1, reference, ATLAS_GET_INSTANCE)) == 0) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference.element) client = _RP(client_data_t, reference.element);
   else client = _RP(client_data_t, reference.instance);

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return client state
      if (client->running == true) return TO_RESPONSE(100);
      return TO_RESPONSE(-100);
      }

   // If the client is running
   if (client->running == true)
      {
      // Return success
      return TO_RESPONSE(100);
      }

   // Create a client agent, 'Willie', and its supporting clone, 'Emily', inside the 'Atlas' domain and enter the 'Lobby' zone
   if (self->CreateActiveAgentAndClone("Willie", "Emily", "Atlas", "Lobby", 3.0f) != TITAN_RESULT_OK)
      {
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Create a new domain, named 'House', using local reference; return failure if unsuccessful
   printf("Creating a new domain 'House' with a 'Kitchen' zone.\n");
   if ((self->m_local_domain_handle = self->Domain->Create(self->m_clientbot.reference, "House", "Kitchen", 2)) == 0)
      {
      // Report error
      printf("Failed to create a new domain 'House' with a starting zone 'Kitchen'\n");
      // Exit application
      exit(0);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Create a context agent, 'Jimmy the Cook', and its supporting clone, 'Cook Bot', inside the 'House' domain and enter the 'Kitchen' zone
   if (self->CreatePassiveAgentAndClone("Jimmy the Cook", "Cook Bot", "House", "Kitchen") != TITAN_RESULT_OK)
      {
      // Exit application
      exit(0);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Start simulation running at 240Hz with an attention level 5, and a history level 3; return failure if unsuccessful
   if (self->Domain->StartSimulation(self->m_clientbot.reference, 240.0f, 5, 3) != TITAN_RESULT_OK)
      {
      // Report error
      printf("Failed to create a domain simulation.\n");
      // Exit application
      exit(0);
      // Return failure
      return TO_RESPONSE(-100);
      }

   // Load manifest and report result
   switch (self->Domain->LoadManifest(self->m_jimmy.reference, manifest_name))
      {
      case TITAN_RESULT_OK:
         printf("MyClient: Manifest '%s' loaded successfully!\n", manifest_name);
         break;
      case TITAN_RESULT_INVALID_INPUT:
         printf("MyClient: Cannot load manifest '%s'. The input parameters are invalid!\n", manifest_name); 
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      case TITAN_RESULT_FILE_NOT_FOUND:
         printf("MyClient: Cannot load manifest '%s'. The file could not be opened!\n", manifest_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      // If the context agent loading this manifest is not represented by a clone
      case TITAN_RESULT_NOT_INITIALIZED:
         printf("MyClient: Cannot load manifest '%s'. The loading agent needs to be hosted by a clone first!\n", manifest_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      // If there is parse error
      case TITAN_RESULT_PARSE_ERROR:
         printf("MyClient: Cannot load manifest '%s'. There was a parsing error!\n", manifest_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      // If there is learning error
      case TITAN_RESULT_INSERTION_FAIL:
         printf("MyClient: Cannot load manifest '%s'. There was a concept learning error!\n", manifest_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      }

   printf("Loading the kitchen story.\n");
   // Start the initial script; return failure if unsuccessful
   switch (self->Agent->ExecuteScript(self->m_jimmy.reference, script_name))
      {
      case TITAN_RESULT_OK:
         printf("MyClient: Script '%s' processed successfully!\n", script_name);
         break;
      case TITAN_RESULT_INVALID_INPUT:
         printf("MyClient: Cannot process script '%s'. The input parameters are invalid!\n", script_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      case TITAN_RESULT_FILE_NOT_FOUND:
         printf("MyClient: Cannot process script '%s'. The file could not be opened!\n", script_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
         // If the context agent loading this script is not represented by a clone
      case TITAN_RESULT_NOT_INITIALIZED:
         printf("MyClient: Cannot process script '%s'. The loading agent needs to be hosted by a clone first!\n", script_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
         // If there is parse error
      case TITAN_RESULT_PARSE_ERROR:
         printf("MyClient: Cannot process script '%s'. There was a parsing error!\n", script_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
         // If there is learning error
      case TITAN_RESULT_INSERTION_FAIL:
         printf("MyClient: Cannot process script '%s'. A reference could not be created!\n", script_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      case TITAN_RESULT_RESOURCE_NOT_FOUND:
         printf("MyClient: Cannot process script '%s'. A referenced concept was not found!\n", script_name);
         // Exit application
         exit(0);
         return TO_RESPONSE(-100);
      }

   // Start the client
   client->running = true;

   // Report starting of the client
   printf("The client has started.\n");

   // Return success
   return TO_RESPONSE(100);
}

/*
   DO: Stop
   Stop directive object application / Set the directive object application to OFF (not running)
   Returns: Cognitive probe result (See probe results in SDK)
*/
atlas_probe_result_t MyClient::DoStop(ATLAS_SIMULATE_PARAMS) 
{ 
   MyClient                                           *self = _RP(MyClient, user_data);
   client_data_t                                      *client;
   atlas_reference_info_t                             reference = { 0 };

   // Get subject group from directive; return failure if unsuccessful
   if (self->Directive->Get(directive, ATLAS_DIRECTIVE_SUBJECT, reference, ATLAS_GET_INSTANCE) != TITAN_RESULT_OK) return NO_RESPONSE();

   // Get data from element if it exists; otherwise, use instance data
   if (reference.element) client = _RP(client_data_t, reference.element);
   else client = _RP(client_data_t, reference.instance);

   // If this expression is a query
   if (directive_header.query == ATLAS_EXP_QUERY)
      {
      // Return client state
      if (client->running == false) return TO_RESPONSE(100);
      return NO_RESPONSE();
      }

   // Stop the client
   client->running = false;

   // Stop running
   exit(0);

   // Return success
   return TO_RESPONSE(100);
}

/*
   Create a client instance
   Returns: Handle to the client instance created upon successful operation
            0 upon failure to create client instance
*/
TITAN_HANDLE64 MyClient::AtlasSim_Create(TITAN_HANDLE64 caller_id, TITAN_ULONG num_elements, TITAN_HANDLE64 params_handle, TITAN_HANDLE64 handshake)
{
   client_data_t                                      *client;

   // Allocate concept data structure
   if (TITAN_ALLOCATE_FLUSH_ARRAY(client, client_data_t, num_elements) == NULL) return 0;

   // Set person to happy initially
   client->on = false;
   client->running = false;

   // Return object data structure
   return TO_HANDLE64(client);
}

/*
   Destroy client instance concept data
   Returns: TITAN_RESULT_OK upon successful operation
            TITAN_RESULT_* upon failure
*/
titan_result_t MyClient::AtlasSim_Destroy(TITAN_HANDLE64 instance, TITAN_HANDLE64 handshake)
{
   client_data_t                                     *client;

   // Validate input; return failure if unscucessful
   if (instance == 0) return TITAN_RESULT_INVALID_INPUT;

   // Get client instance data
   client = _RP(client_data_t, instance);

   // Free client data
   TITAN_FREE(client);

   // Return success
   return TITAN_RESULT_OK;
}

/*
   End of file
*/
