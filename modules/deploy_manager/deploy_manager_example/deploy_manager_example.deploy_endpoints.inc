<?php
/**
 * @file
 * deploy_manager_example.deploy_endpoints.inc
 */

/**
 * Implementation of hook_deploy_endpoints_default().
 */
function deploy_manager_example_deploy_endpoints_default() {
  $export = array();

  $endpoint = new DeployEndpoint;
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 1;
  $endpoint->name = 'deploy_manager_example_endpoint';
  $endpoint->title = 'Deploy Manager example endpoint';
  $endpoint->description = 'Example remote endpoint not using any authentication.';
  $endpoint->debug = 0;
  $endpoint->authenticator_plugin = 'DeployAuthenticatorNone';
  $endpoint->authenticator_config = array();
  $endpoint->service_plugin = 'DeployServiceRestJSON';
  $endpoint->service_config = array(
    'url' => 'http://example.com/api',
  );
  $export['deploy_manager_example_endpoint'] = $endpoint;

  return $export;
}
