<?php

/**
 * Implements hook_rules_action_info().
 */
function deploy_manager_rules_action_info() {
  $types = entity_get_info();
  $type_options = array_keys($types);
  $actions = array(
    'deploy_manager_action_add_to_plan' => array(
      'label' => t('Add an entity to a deployment plan'),
      'group' => t('Deploy'),
      'parameter' => array(
        'plan' => array(
          'type' => 'deploy_plan',
          'label' => t('Plan to add to'),
          'options list' => 'deploy_manager_get_plan_options'
          ),
        'entity' => array(
          'type' => $type_options,
          'label' => t('Entity to add'),
          'wrapped' => TRUE,
        ),
      ),
    )
  );
  return $actions;
}

/**
 * Action callback for the "Add to deploy plan" action.
 */
function deploy_manager_action_add_to_plan($plan_name, $wrapped_entity) {
  if (!deploy_managed_plan_exists($plan_name)) {
    watchdog('deploy manager', 'Cannot add to non-existent deploy plan @plan', array('@plan' => $plan_name));
    return;
  }
  deploy_manager_add_to_plan($plan_name, $wrapped_entity);
}

/**
 * Implements hook_rules_data_info().
 */
function deploy_manager_rules_data_info() {
  return array(
    'deploy_plan' => array(
      'label' => t('deploy plan'),
      'group' => t('Deploy'),
      'ui class' => 'RulesDataUIText',
    ),
  );
}

/**
 * Options callback for the deploy_plan data type.
 */
function deploy_manager_get_plan_options() {
  $plans = deploy_manager_get_managed_plans();
  $options = array();
  foreach ($plans as $machine_name => $info) {
    $options[$machine_name] = $info->name;
  }
  return $options;
}