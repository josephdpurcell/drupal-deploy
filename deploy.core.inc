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
      $dependencies['user'][$entity->uid] = TRUE;
      break;
  }
  return $dependencies;
}

/**
 * Implements hook_deploy_entity_dependencies().
 *
 * This hook will go through all fields for an entity and invoke a new hook for
 * all field modules. This saves a lot of work for each field implementaor.
 */
function field_deploy_entity_dependencies($entity, $entity_type) {
  $dependencies = array();

  list(,, $bundle_name) = entity_extract_ids($entity_type, $entity);
  $instances = field_info_instances($entity_type, $bundle_name);

  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($field_name);
    foreach ($entity->{$field_name} as $langcode => $items) {
      $field_dependencies = module_invoke($field['module'], 'deploy_field_dependencies', $entity_type, $entity, $field, $instance, $langcode, $items);
      // Let other modules alter dependencies for this field.
      drupal_alter('deploy_field_dependency', $field_dependencies, $entity_type, $entity, $field, $instance, $langcode, $items);

      if (!empty($field_dependencies)) {
        // TODO: This is not good enough. Thinking of how the dependency arrays
        // should look like, two modules returning 'user' dependencies will
        // overwrite each other.
        $dependencies = array_merge($dependencies, $field_dependencies);
      }
    }
  }
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependencies().
 */
function taxonomy_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();

  foreach ($items as $item) {
    $dependencies['taxonomy_term'][$item['tid']] = TRUE;
  }
  return $dependencies;
}
