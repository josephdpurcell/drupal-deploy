<?php
/**
 * @file
 * deploy_manager_example.rules_defaults.inc
 */

/**
 * Implementation of hook_default_rules_configuration().
 */
function deploy_manager_example_default_rules_configuration() {
  $items = array();
  $items['rules_add_to_example_plan'] = entity_import('rules_config', '{ "rules_add_to_example_plan" : {
      "LABEL" : "Add to example plan",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "deploy_manager", "rules" ],
      "ON" : [ "node_insert", "node_update" ],
      "DO" : [
        { "deploy_manager_action_add_to_plan" : { "plan" : "deploy_manager_example_plan", "entity" : [ "node" ] } }
      ]
    }
  }');
  return $items;
}
