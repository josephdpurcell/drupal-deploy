<?php

/**
 * Implements hook_rules_action_info().
 */
function deploy_rules_action_info() {
  $actions = array(
    'deploy_action_deploy_plan' => array(
      'label' => t('Deploy a plan'),
      'group' => t('Deploy'),
      'parameter' => array(
        'plan' => array(
          'type' => 'deploy_plan',
          'label' => t('Plan to add to'),
          'options list' => 'deploy_get_plan_options'
          ),
      ),
    )
  );
  return $actions;
}

/**
 * Action callback for the "Deploy a plan" action.
 */
function deploy_action_deploy_plan($name) {
  if ($plan = deploy_plan_load($name)) {
    $plan->deploy();
    return;
  }
  watchdog('deploy', 'Cannot deploy non-existent plan @name', array('@name' => $name), WATCHDOG_ERROR);
  return;
}

/**
 * Implements hook_rules_data_info().
 */
function deploy_rules_data_info() {
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
function deploy_get_plan_options() {
  $plans = deploy_plan_load_all();
  $options = array();
  foreach ($plans as $machine_name => $info) {
    $options[$machine_name] = $info->name;
  }
  return $options;
}