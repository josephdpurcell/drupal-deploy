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

  function process($endpoint_name) {
    if ($endpoint = deploy_endpoint_load($endpoint_name)) {
      $endpoint->deploy($this->aggregator->plan, $this->aggregator);
      drupal_set_message(t('Plan %plan has been deployed to %endpoint.', array('%plan' => $this->aggregator->plan->title, '%endpoint' => $endpoint->title)));
    }
  }

  function cleanup() {}

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }
}
