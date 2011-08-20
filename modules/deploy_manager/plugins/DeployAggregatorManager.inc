<?php

class DeployAggregatorManager extends DeployAggregatorBase {

  var $config = array();

  /**
   * Holds a reference to the plan object that initiated it.
   *
   * @var DeployPlan
   */
  var $plan;

  function __construct(DeployPlan $plan = NULL, Array $config = array()) {
    $defaults = array(
      'debug' => FALSE,
      'delete_on_deploy' => FALSE,
    );
    $this->config = array_merge($defaults, $config);
    $this->plan = $plan;
  }

  /**
   * Get aggregated entities.
   */
  function getEntities() {
    return deploy_manager_get_plan_items($this->plan->name);
  }

  function configForm(&$form_state) {
    $form = array();
    $form['delete_on_deploy'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete successfully deployed items'),
      '#description' => t('If checked, each successfully deployed item will be automatically removed from the plan.'),
      '#default_value' => $this->config['delete_on_deploy'],
    );
    return $form;
  }

}
