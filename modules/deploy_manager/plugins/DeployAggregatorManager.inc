<?php

/**
 * Class for a deploy aggregator that aggregates entities based on what has been
 * added to the deploy plan using the Deploy Manager UI.
 */
class DeployAggregatorManager extends DeployAggregatorBase {

  public $config = array();

  /**
   * Holds a reference to the plan object that initiated it.
   *
   * @var DeployPlan
   */
  public $plan;

  public function __construct(DeployPlan $plan = NULL, Array $config = array()) {
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
  public function getEntities() {
    return deploy_manager_get_plan_items($this->plan->name);
  }

  public function configForm(&$form_state) {
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
