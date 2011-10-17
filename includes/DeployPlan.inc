<?php

class DeployPlanException extends Exception {}

class DeployPlan {

  var $debug = FALSE;

  var $aggregator_plugin = NULL;

  var $aggregator_config = array();

  var $aggregator = NULL;

  var $processor_plugin = NULL;

  var $processor_config = array();

  var $processor = NULL;

  var $endpoints = array();

  function init() {
    // Since the CTools Export API is declarative by nature, we can't rely on
    // constructor injection and deploy_plan_create() as the only factory.
    $schema = drupal_get_schema('deploy_plans');
    // Unserialize our serialized params.
    foreach ($schema['fields'] as $field => $info) {
      if (!empty($info['serialize']) && !is_array($this->{$field})) {
        $this->{$field} = (array)unserialize($this->{$field});
      }
    }
    if (!empty($this->aggregator_plugin)) {
      $aggregator_config = $this->aggregator_config + array('debug' => $this->debug);
      $this->aggregator = new $this->aggregator_plugin($this, $aggregator_config);
    }
    if (!empty($this->processor_plugin)) {
      $processor_config = $this->processor_config + array('debug' => $this->debug);
      $this->processor = new $this->processor_plugin($this->aggregator, $processor_config);
    }
  }

  function deploy() {
    if (empty($this->processor)) {
      $this->init();
    }
    if (empty($this->processor) || $this->fetch_only) {
      $exception = new DeployPlanException(t("The plan @name can't be deployed in push fashion because it missing a processor plugin or is configured <em>fetch-only</em>.", array('@name' => $plan->name)));
      watchdog_exception('error', $exception);
      return FALSE;
    }
    foreach ($this->endpoints as $endpoint_name) {
      $this->processor->process($endpoint_name);
    }
    $this->processor->cleanup();
  }

  function getEntities() {
    if (empty($this->aggregator)) {
      $this->init();
    }
    return $this->aggregator->getEntities();
  }

  function getIterator() {
    if (empty($this->aggregator)) {
      $this->init();
    }
    return $this->aggregator;
  }
}
