<?php

class DeployProcessorQueue implements DeployProcessor {

  var $config = array();

  var $provider = NULL;

  function __construct(DeployProvider $provider, Array $config = array()) {
    $this->config = $config;
    $this->provider = $provider;
  }

  function process(DeployEndpoint $endpoint) {
    foreach ($this->provider->getResources() as $resource) {
      // @todo Add resource and endpoint to queue.
    }
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
