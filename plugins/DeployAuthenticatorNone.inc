<?php

/**
 * Authentication class that uses basic authentication.
 */
class DeployAuthenticatorNone implements DeployAuthenticator {

  /**
   * The service being used to deploy the plan.
   *
   * @var DeployService
   */
  public $service = NULL;

  /**
   * Configuration options.
   *
   * @var array
   */
  public $config = array();

  public function __construct(DeployService $service, Array $config = array()) {
    $this->service = $service;
    $this->config += array(
      'debug' => FALSE,
    );
    $this->config = array_merge($this->config, $config);
  }

  public function deploy(Traversable $iterator) {
    $this->service->deploy($iterator);
  }

  public function configForm(&$form_state) {
    return array();
  }

}
