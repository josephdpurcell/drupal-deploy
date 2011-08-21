<?php
/**
 * @file
 * deploy_manager_example.deploy_plans.inc
 */

/**
 * Implementation of hook_deploy_plans_default().
 */
function deploy_manager_example_deploy_plans_default() {
  $export = array();

  $plan = new DeployPlan;
  $plan->disabled = FALSE; /* Edit this to true to make a default plan disabled initially */
  $plan->api_version = 1;
  $plan->name = 'deploy_manager_example_plan';
  $plan->title = 'Manager example plan';
  $plan->description = 'Manager example plan that only deploys managed content on the site.';
  $plan->debug = 0;
  $plan->aggregator_plugin = 'DeployAggregatorManager';
  $plan->aggregator_config = array(
    'delete_on_deploy' => 1,
  );
  $plan->processor_plugin = 'DeployProcessorMemory';
  $plan->processor_config = array();
  $plan->endpoints = array(
    'deploy_manager_example_endpoint' => 'deploy_manager_example_endpoint',
  );
  $export['deploy_manager_example_plan'] = $plan;

  return $export;
}
