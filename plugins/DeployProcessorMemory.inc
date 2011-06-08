<?php

class DeployProcessorMemory implements DeployProcessor {

  var $config = array();

  var $aggregator = NULL;

  function __construct(DeployAggregator $aggregator, Array $config = array()) {
    $this->config = $config;
    $this->aggregator = $aggregator;
  }

  function process($endpoint_name) {
    $endpoint = deploy_endpoint_load($endpoint_name);
    $endpoint->deploy($this->aggregator);
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
