<?php

/**
 * @file
 * Default rules configuration for deploy module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function deploy_default_rules_configuration() {
  $items = [];

  $items['rules_remove_entity_from_deployment_plan'] = entity_import('rules_config', '{ "rules_remove_entity_from_deployment_plan" : {
      "LABEL" : "Remove entity from deployment plan",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "deploy" ],
      "REQUIRES" : [ "deploy" ],
      "USES VARIABLES" : { "dme" : { "label" : "Plan entry", "type" : "deploy_manager_entities" } },
      "ACTION SET" : [ { "deploy_manager_entity_remove" : { "dme" : [ "dme" ] } } ]
    }
  }');

  return $items;
}
