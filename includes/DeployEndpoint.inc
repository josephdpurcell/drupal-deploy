<?php

class DeployEndpoint {

  var $debug = FALSE;

  var $service_plugin = NULL;

  var $service_config = array();

  var $authenticator_plugin = NULL;

  var $authenticator_config = array();

  var $authenticator = NULL;

  function deploy(DeployPlan $plan, Traversable $iterator) {
    // Since the CTools Export API is declarative by nature, we can't realy on
    // contructor injection and deploy_endpoint_create() as the only factory.
    if (!empty($this->service_plugin)) {
      $service_config = $this->service_config + array('debug' => $this->debug);
      $service = new $this->service_plugin($plan, $service_config);
    }
    if (!empty($this->authenticator_plugin)) {
      $authenticator_config = $this->authenticator_config + array('debug' => $this->debug);
      $this->authenticator = new $this->authenticator_plugin($service, $authenticator_config);
    }

    $this->authenticator->deploy($iterator);
  }
}
