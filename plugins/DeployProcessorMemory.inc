<?php

class DeployProcessorMemory implements DeployProcessor {

  var $config = array();

  var $provider = NULL;

  function __construct(DeployProvider $provider, Array $config = array()) {
    $this->config = $config;
    $this->provider = $provider;
  }

  function process($endpoint_name) {
    $endpoint = deploy_endpoint_load($endpoint_name);
    $endpoint->deploy($this->provider->getEntities());
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
