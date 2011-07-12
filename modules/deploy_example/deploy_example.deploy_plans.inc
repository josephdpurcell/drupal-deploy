<?php
/**
 * @file
 * deploy_example.deploy_plans.inc
 */

/**
 * Implementation of hook_deploy_plans_default().
 */
function deploy_example_deploy_plans_default() {
  $export = array();

  $plan = new DeployPlan;
  $plan->disabled = FALSE; /* Edit this to true to make a default plan disabled initially */
  $plan->api_version = 1;
  $plan->name = 'deploy_example_plan';
  $plan->title = 'Example Plan';
  $plan->description = 'Example plan that deploys all new content on the site.';
  $plan->debug = 0;
  $plan->aggregator_plugin = 'DeployAggregatorViews';
  $plan->aggregator_config = array(
    'view_name' => 'deploy_example_aggregator',
  );
  $plan->processor_plugin = 'DeployProcessorMemory';
  $plan->processor_config = array();
  $plan->endpoints = array(
    'deploy_example_endpoint' => 'deploy_example_endpoint',
  );
  $export['deploy_example_plan'] = $plan;

  return $export;
}
