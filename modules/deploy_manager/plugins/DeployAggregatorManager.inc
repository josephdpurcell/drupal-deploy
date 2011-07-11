<?php

class DeployAggregatorManager implements DeployAggregator {

  var $config = array();

  function __construct(Array $config = array()) {
    $defaults = array(
      'plan_name' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  /**
   * @return DeployIteratorIterator
   *
   * @see DeployAggregator::getIterator()
   */
  function getIterator() {
    $items = deploy_manager_get_plan_items($this->config['plan_name']);
    $iterator = new DeployIterator($items);
    return new DeployIteratorIterator($iterator);
  }

  function configForm(&$form_state) {
    // This is a hack for now - what's the best way to be able to access the
    // plan name from the aggregator?
    $form = array();
    $form['plan_name'] = array(
      '#type' => 'value',
      '#value' => $form_state['item']->name,
    );
    return $form;
  }

}
