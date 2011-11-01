<?php

/**
 * @file
 * Contains hook implementations for all relevant core module.
 *
 * @todo
 *   Replace all these hook implementations with a generic solution that uses
 *   info from hook_entity_field_info() and hook_entity_property_info(). That
 *   info should contain info about what and how dependencies are added.
 */

/**
 * @defgroup deploy_property Propery implementations
 * @{
 */

/**
 * Implements hook_deploy_entity_dependenies().
 */
function node_deploy_entity_dependencies($entity, $entity_type) {
  if ($entity_type == 'node') {
    $dependencies = array();
    deploy_add_dependencies($dependencies, $entity, 'user', array('uid', 'revision_uid'));
    deploy_add_dependencies($dependencies, $entity, 'node', 'tnid');
    return $dependencies;
  }
}

/**
 * Implements hook_deploy_entity_dependenies().
 */
function comment_deploy_entity_dependencies($entity, $entity_type) {
  if ($entity_type == 'comment') {
    $dependencies = array();
    deploy_add_dependencies($dependencies, $entity, 'user', 'uid');
    deploy_add_dependencies($dependencies, $entity, 'node', 'nid');
    return $dependencies;
  }
}

/**
 * Implements hook_deploy_entity_dependencies()
 */
function taxonomy_deploy_entity_dependencies($entity, $entity_type) {
  if ($entity_type == 'taxonomy_term') {
    $dependencies = array();
    $terms = taxonomy_get_parents($entity->tid);
    foreach ($terms as $tid => $term) {
      $dependencies['taxonomy_term'][$tid] = TRUE;
    }
    return $dependencies;
  }
}

/**
 * Implements hook_deploy_entity_dependencies().
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
        foreach ($field_dependencies as $entity_type => $entity_ids) {
          foreach ($entity_ids as $entity_id => $something) {
            $dependencies[$entity_type][$entity_id] = TRUE;
          }
        }
      }
    }
  }
  return $dependencies;
}

/**
 * @} End of "Property implementations"
 */

/**
 * @defgroup deploy_field Field implementations
 * @{
 */

/**
 * Implements hook_deploy_field_dependencies().
 */
function taxonomy_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  deploy_add_dependencies($dependencies, $items, 'taxonomy_term', 'tid');
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependency().
 */
function file_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  deploy_add_dependencies($dependencies, $items, 'file', 'fid');
  deploy_add_dependencies($dependencies, $items, 'user', 'uid');
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependencies().
 */
function node_reference_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  deploy_add_dependencies($dependencies, $items, 'node', 'nid');
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependencies().
 */
function user_reference_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  deploy_add_dependencies($dependencies, $items, 'user', 'uid');
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependencies().
 */
function entityreference_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  foreach ($items as $item) {
    $info = entity_get_info($item['target_type']);
    deploy_add_dependencies($dependencies, $item, $item['target_type'], $info['entity keys']['id']);
  }
  return $dependencies;
}

/**
 * @} End of "Field implementations"
 */

/**
 * @defgroup deploy_alter Alterations
 * @{
 */

/**
 * Implements hook_entity_load().
 *
 * The 'parent' property isn't attached when terms are loaded. Deploy needs that
 * variable to correctly deploy terms.
 */
function taxonomy_entity_load(&$entities, $entity_type) {
  if ($entity_type == 'taxonomy_term') {
    foreach ($entities as &$entity) {
      if (!isset($entity->parent)) {
        $parents = taxonomy_get_parents($entity->tid);
        if (!empty($parents)) {
          $entity->parent = array();
          foreach ($parents as $parent) {
            $entity->parent[] = $parent->tid;
          }
        }
      }
    }
  }
}

/**
 * Implements hook_deploy_entity_alter().
 *
 * This hook will go through all fields for an entity and invoke a new hook for
 * all field modules. This saves a lot of work for each field implementaor.
 */
function field_deploy_entity_alter(&$entity, $entity_type) {
  if (empty($entity) || empty($entity_type)) {
    return;
  }

  $dependencies = array();
  list(,, $bundle_name) = entity_extract_ids($entity_type, $entity);
  $instances = field_info_instances($entity_type, $bundle_name);

  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($field_name);
    foreach ($entity->{$field_name} as $langcode => &$items) {
      foreach ($items as &$item) {
        // The 'safe_value' key is attached on runtime. We don't need to deploy
        // that.
        if (isset($item['safe_value'])) {
          unset($item['safe_value']);
        }
      }
      // TODO: Can we do this with drupal_alter()? Reason for this quick and
      // mashup is because I want to keep the argument order consistent between
      // field hooks, and still possibility to alter arguments.
      $function = $field['module'] . '_deploy_field_alter';
      if (function_exists($function)) {
        $function($entity_type, $entity, $field, $instance, $langcode, $items);
      }
    }
  }
  return $dependencies;
}

/**
 * @} End of "Alterations"
 */
