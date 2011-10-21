<?php

/**
 * @file
 * Contains hook implementations for all relevant core module.
 */

/**
 * Implements hook_deploy_entity_dependenies().
 *
 * We don't treat user UID 1 as a dependency!
 *
 * UID 1 is considered a root user, which already exists in most endpoint
 * systems and thus causes conflicts, like when endpoints happen to be
 * another Drupal site.
 */
function node_deploy_entity_dependencies($entity, $entity_type) {
  if ($entity_type == 'node') {
    $dependencies = array();
    // We don't treat UID 1 as a dependency.
    if ($entity->uid != 1) {
      $dependencies['user'] = array($entity->uid => TRUE);
    }
    if (!empty($entity->tnid) && $entity->tnid != $entity->nid) {
      $dependencies['node'] = array($entity->tnid => TRUE);
    }
    return $dependencies;
  }
}

/**
 * Implements hook_deploy_entity_alter().
 *
 * Since we aren't treating user UID 1 as a dependency we must alter that UUID
 * back to UID 1 before deploying an entity.
 */
function node_deploy_entity_alter(&$entity, $entity_type) {
  if ($entity_type == 'node') {
    $uuids = array($entity->uid);
    if (!empty($entity->revision_uid) && $entity->revision_uid != $entity->uid) {
      $uuids[] = $entity->revision_uid;
    }
    $uids = entity_get_id_by_uuid('user', array($uuids));
    foreach ($uids as $uuid => $uid) {
      if ($uid == 1) {
        if ($entity->uid == $uuid) {
          $entity->uid = '1';
        }
        if ($entity->revision_uid == $uuid) {
          $entity->revision_uid = '1';
        }
      }
    }
  }
}

/**
 * Implements hook_deploy_entity_dependenies().
 *
 * @see node_deploy_entity_dependencies().
 */
function comment_deploy_entity_dependencies($entity, $entity_type) {
  if ($entity_type == 'comment') {
    $dependencies = array();
    // We don't treat UID 1 as a dependency.
    if ($entity->uid != 1) {
      $dependencies['user'] = array($entity->uid => TRUE);
    }
    $dependencies['node'] = array($entity->nid => TRUE);
    return $dependencies;
  }
}

/**
 * Implements hook_deploy_entity_alter().
 *
 * @see node_deploy_entity_alter().
 */
function comment_deploy_entity_alter(&$entity, $entity_type) {
  if ($entity_type == 'comment') {
    $uid = reset(entity_get_id_by_uuid('user', array($entity->uid)));
    if ($uid == 1) {
      $entity->uid = '1';
    }
  }
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
 * Implements hook_deploy_entity_alter().
 *
 * This hook will go through all fields for an entity and invoke a new hook for
 * all field modules. This saves a lot of work for each field implementaor.
 */
function field_deploy_entity_alter(&$entity, $entity_type) {
  $dependencies = array();
  list(,, $bundle_name) = entity_extract_ids($entity_type, $entity);
  $instances = field_info_instances($entity_type, $bundle_name);

  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($field_name);
    foreach ($entity->{$field_name} as $langcode => &$items) {
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
 * Implements hook_deploy_field_dependencies().
 */
function taxonomy_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  foreach ($items as $item) {
    $dependencies['taxonomy_term'][$item['tid']] = TRUE;
  }
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependencies().
 */
function node_reference_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  foreach ($items as $item) {
    $dependencies['node'][$item['nid']] = TRUE;
  }
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependencies().
 *
 * @see node_deploy_entity_dependencies().
 */
function user_reference_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  foreach ($items as $item) {
    // We don't treat UID 1 as a dependency.
    if ($item['uid'] != 1) {
      $dependencies['user'][$item['uid']] = TRUE;
    }
  }
  return $dependencies;
}

/**
 * Implements hook_deploy_field_dependency().
 */
function file_deploy_field_dependencies($entity_type, $entity, $field, $instance, $langcode, $items) {
  $dependencies = array();
  foreach ($items as $item) {
    $dependencies['file'][$item['fid']] = TRUE;
    if ($item['uid'] != 1) {
      $dependencies['user'][$item['uid']] = TRUE;
    }
  }
  return $dependencies;
}

/**
 * Implements hook_deploy_entity_alter().
 */
function file_deploy_entity_alter(&$entity, $entity_type) {
  if ($entity_type == 'file') {
    $uid = reset(entity_get_id_by_uuid('user', array($entity->uid)));
    if ($uid == 1) {
      $entity->uid = '1';
    }
  }
}

/**
 * Implements hook_deploy_field_alter().
 */
function file_deploy_field_alter($entity_type, &$entity, $field, $instance, $langcode, &$items) {
  $uuids = array();
  foreach ($items as $item) {
    $uuids[] = $item['uid'];
  }
  // Get all UIDs in one query.
  $uids = entity_get_id_by_uuid('user', array($uuids));
  foreach ($items as &$item) {
    if (isset($uids[$item['uid']]) && $uids[$item['uid']] == 1) {
      $item['uid'] = '1';
    }
  }
}
