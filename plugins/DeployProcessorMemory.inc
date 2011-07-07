<?php

class DeployProcessorMemory implements DeployProcessor {

  var $aggregator = NULL;

  var $config = array();

  function __construct(DeployAggregator $aggregator, Array $config = array()) {
    $this->aggregator = $aggregator;
    $this->config = $config;
  }

  function process($endpoint_name) {
    if ($endpoint = deploy_endpoint_load($endpoint_name)) {
      $endpoint->deploy($this->aggregator);
    }
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
