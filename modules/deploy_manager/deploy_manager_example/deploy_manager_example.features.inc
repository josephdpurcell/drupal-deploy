<?php
/**
 * @file
 * deploy_manager_example.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function deploy_manager_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "deploy" && $api == "deploy_endpoints") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "deploy" && $api == "deploy_plans") {
    return array("version" => "1");
  }
}
