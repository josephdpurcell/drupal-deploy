<?php

class DeployAggregatorManager implements DeployAggregator {

  var $config = array();

  function __construct(Array $config = array()) {
    $defaults = array('delete_on_deploy' => 0);
    $this->config = array_merge($defaults, $config);
  }

  /**
   * @return DeployIteratorIterator
   *
   * @see DeployAggregator::getIterator()
   */
  function getIterator() {
    $plan = deploy_active();
    $items = deploy_manager_get_plan_items($plan);
    $iterator = new DeployIterator($items);
    return new DeployIteratorIterator($iterator);
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
