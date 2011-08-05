<?php

class DeployProcessorMemory implements DeployProcessor {

  var $aggregator = NULL;

  var $config = array();

  function __construct(DeployAggregator $aggregator, Array $config = array()) {
    $this->aggregator = $aggregator;
    $this->config += array(
      'debug' => FALSE,
    );
    $this->config = array_merge($this->config, $config);
  }

  function process(DeployPlan $plan, $endpoint_name) {
    if ($endpoint = deploy_endpoint_load($endpoint_name)) {
      $endpoint->deploy($plan, $this->aggregator);
    }
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

  /**
   * Called during either batch or queue processing of plan items.
   */
  static function process_item($item) {
    $endpoint = deploy_endpoint_load($item['endpoint']);
    $plan = deploy_plan_load($item['plan']);
    if ($plan && $endpoint) {
      $collection = array(
        $item['type'] => array($item['eid'] => TRUE)
      );
      $iterator = new DeployIterator($collection);
      $aggregator = new DeployAggregateSingle($iterator);
      $endpoint->deploy($plan, $aggregator);
    }
  }
}
