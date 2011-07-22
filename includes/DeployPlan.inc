<?php

class DeployPlan {

  var $debug = FALSE;

  var $aggregator_plugin = NULL;

  var $aggregator_config = array();

  var $processor_plugin = NULL;

  var $processor_config = NULL;

  var $processor = NULL;

  var $endpoints = array();

  function deploy() {
    // TODO: Remove this hack - deploy_active() is a temporary solution.
    deploy_active($this->name);
    // Since the CTools Export API is declarative by nature, we can't rely on
    // constructor injection and deploy_plan_create() as the only factory.
    $aggregator_config = $this->aggregator_config + array('debug' => $this->debug);
    $aggregator = new $this->aggregator_plugin($aggregator_config);
    $processor_config = $this->processor_config + array('debug' => $this->debug);
    $this->processor = new $this->processor_plugin($aggregator, $processor_config);

    foreach ($this->endpoints as $endpoint_name) {
      $this->processor->process($endpoint_name);
    }
    deploy_active(NULL, TRUE);
  }

}