<?php

/**
 * @file
 * Contains hook implementations for all relevant core module.
 */

/**
 * Implements hook_deploy_entity_dependenies().
 */
function user_deploy_entity_dependencies($entity, $entity_type) {
  $dependencies = array();
  switch ($entity_type) {
    case 'node':
    case 'comment':
    case 'file':
      $dependencies['user'] = array(
        $entity->uid => TRUE,
      );
      break;
  }
  return $dependencies;
}
