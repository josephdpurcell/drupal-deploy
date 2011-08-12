<?php

/**
 * @file
 * Hooks provided by the Deploy module.
 */

/**
 * Allow modules to define dependencies to an entity.
 *
 * This hook is mostly useful to determine dependencies based on entity
 * properties or other primitive values. Implement
 * 'hook_deploy_field_dependencies' if your field module must declare a field
 * related dependency to an entity.
 *
 * @todo
 *   Document how the dependency array should look like. This is a subject to
 *   change.
 */
function hook_deploy_entity_dependencies($entity, $entity_type) {

}

/**
 * Allow other modules to alter dependencies.
 */
function hook_deploy_entity_dependencies_alter(&$dependencies, $entity, $entity_type) {

}

/**
 * Allow field modules to define entity denepdencies for their fields.
 *
 * This hook should be seen as an extension of the Field API and thus, does not
 * use the 'deploy' namespace.
 */
function hook_deploy_field_dependencies() {

}

/**
 * Allow module to react on a deployment.
 *
 * @todo
 *   Rename to 'hook_entity_deploy' and add a 'hook_entity_predeploy' to mimic
 *   ie. 'hook_entity_presave' and 'hook_entity_insert'.
 */
function hook_deploy_item_deployed($sender, $args) {

}
