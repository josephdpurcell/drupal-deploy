<?php

/**
 * @file
 * Features support to export entities from any <em>fetch-only</em> plan.
 */

/**
 * Implementation of [component]_features_export_options().
 */
function deploy_plan_entities_features_export_options() {
  $options = array();
  $plans = deploy_plan_load_all(array('fetch_only' => 1));
  foreach ($plans as $plan) {
    $options[check_plain($plan->name)] = check_plain($plan->title);
  }
  return $options;
}

/**
 * Implementation of [component]_features_export().
 */
function deploy_plan_entities_features_export($data, &$export, $module_name) {
  foreach ($data as $name) {
    $export['features']['deploy_plan_entities'][$name] = $name;
  }
}

/**
 * Implementation of [component]_features_export_render().
 */
function deploy_plan_entities_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $plans = array();';
  $code[] = '';

  foreach ($data as $name) {
    $plan = deploy_plan_load($name);
    $iterator = $plan->getIterator();
    foreach ($iterator as $current) {
      $entity_type = $current['entity_type'];
      $entity = $current['entity'];
      $entity_info = entity_get_info($entity_type);

      // We need to remove the primary keys. The UUID module will take care of
      // the UUID mappings.
      $id_key = $entity_info['entity keys']['id'];
      if (isset($entity->{$id_key})) {
        unset($entity->{$id_key});
      }
      if (!empty($entity_info['entity keys']['revision'])) {
        // Get the keys for local revison ID and revision UUID.
        $vid_key = $entity_info['entity keys']['revision'];
        if (isset($entity->{$vid_key})) {
          unset($entity->{$vid_key});
        }
        // We also need to remove the VUUID key, since reverting can result in
        // new revision, and the feature will then always be overidden.
        if (!empty($entity_info['entity keys']['revision uuid'])) {
          $vuuid_key = $entity_info['entity keys']['revision uuid'];
          unset($entity->{$vuuid_key});
        }
      }
      // We need to unset some timestamp properties, since those can change and
      // the feature will then always be overidden.
      $keys = array('created', 'updated', 'changed', 'revision_timestamp');
      foreach ($keys as $key) {
        if (isset($entity->{$key})) {
          unset($entity->{$key});
        }
      }

      $code[] = '  $plans[\'' . check_plain($plan->name) . '\'][] = array(';
      $code[] = '    \'entity_type\' => \'' . $entity_type . '\',';
      $code[] = '    \'entity\' => (object) ' . features_var_export($entity, '    ');
      $code[] = '  );';
    }
  }

  $code[] = '';
  $code[] = '  return $plans;';
  $code = implode("\n", $code);

  return array('deploy_plan_default_entities' => $code);
}

/**
 * Implementation of [component]_features_export_rebuild().
 */
function deploy_plan_entities_features_rebuild($module_name) {
  $plans = module_invoke($module_name, 'deploy_plan_default_entities');
  if (!empty($plans)) {
    foreach ($plans as $name => $entities) {
      foreach ($entities as $current) {
        $info = entity_get_info($current['entity_type']);
        $uuid_key = $info['entity keys']['uuid'];
        $uuid = $current['entity']->{$uuid_key};
        // We can't do all the IDs in one call, since a plan can contain many
        // different entity types.
        $id = reset(entity_get_id_by_uuid($current['entity_type'], array($uuid)));
        // Delete.
        entity_delete($current['entity_type'], $id);
        // Rebuild.
        entity_uuid_save($current['entity_type'], $current['entity']);
      }
    }
  }
}

/**
 * Implementation of [component]_features_export_revert().
 */
function deploy_plan_entities_features_revert($module_name) {
  deploy_plan_entities_features_rebuild($module_name);
}
